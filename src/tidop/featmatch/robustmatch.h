/**************************************************************************
 *                                                                        *
 * Copyright (C) 2021 by Tidop Research Group                             *
 * Copyright (C) 2021 by Esteban Ruiz de Oña Crespo                       *
 *                                                                        *
 * This file is part of TidopLib                                          *
 *                                                                        *
 * TidopLib is free software: you can redistribute it and/or modify       *
 * it under the terms of the GNU Lesser General Public License as         *
 * published by the Free Software Foundation, either version 3 of the     *
 * License, or (at your option) any later version.                        *
 *                                                                        *
 * TidopLib is distributed in the hope that it will be useful,            *
 * but WITHOUT ANY WARRANTY; without even the implied warranty of         *
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the          *
 * GNU Lesser General Public License for more details.                    *
 *                                                                        *
 * You should have received a copy of the GNU Lesser General Public       *
 * License along with TidopLib. If not, see <http://www.gnu.org/licenses>.*
 *                                                                        *
 * @license LGPL-3.0 <https://www.gnu.org/licenses/lgpl-3.0.html>         *
 *                                                                        *
 **************************************************************************/

#pragma once

#include "tidop/featmatch/matcher.h"

namespace tl
{

class TL_EXPORT RobustMatchingProperties
  : public RobustMatcher
{

private:

    double mRatio;
    bool mCrossCheck;
    std::shared_ptr<GeometricTest> mGeometricTest;

public:

    RobustMatchingProperties();
    ~RobustMatchingProperties() override = default;

// RobustMatcher interface

public:

    auto ratio() const -> double override;
    void setRatio(double ratio) override;
    auto crossCheck() const -> bool override;
    void setCrossCheck(bool crossCheck) override;
    auto geometricTest() const -> std::shared_ptr<GeometricTest> override;
    void setGeometricTest(std::shared_ptr<GeometricTest> geometricTest) override;

// MatchingStrategy interface

public:

    void reset() override;
    auto name() const -> std::string override;

};


/*----------------------------------------------------------------*/


class TL_EXPORT RobustMatchingImp
  : public RobustMatchingProperties,
    public MatchingAlgorithm
{

protected:

    std::shared_ptr<DescriptorMatcher> mDescriptorMatcher;

public:

    explicit RobustMatchingImp(std::shared_ptr<DescriptorMatcher> descriptorMatcher);
    RobustMatchingImp(std::shared_ptr<DescriptorMatcher> descriptorMatcher,
                      double ratio,
                      bool crossCheck,
                      std::shared_ptr<GeometricTest> geometricTest);
    ~RobustMatchingImp() override = default;

    /*!
     * \brief Sets the matching method
     * \param[in] matcher
     */
    void setDescriptorMatcher(const std::shared_ptr<DescriptorMatcher> &descriptorMatcher);

    /*!
     * \brief Ratio test
     * \param[in] matches
     * \param[in] ratio
     * \param[out] goodMatches
     * \param[out] wrongMatches
     */
    static auto ratioTest(const std::vector<std::vector<cv::DMatch>> &matches,
                          double ratio,
                          std::vector<std::vector<cv::DMatch>> *wrongMatches = nullptr) -> std::vector<std::vector<cv::DMatch>>
    {
        std::vector<std::vector<cv::DMatch>> goodMatches;

        for (size_t i = 0; i < matches.size(); i++) {

            if (matches[i].size() > 1) {
                // check distance ratio
                if (matches[i][0].distance / matches[i][1].distance <= static_cast<float>(ratio)) {
                    goodMatches.push_back(matches[i]);
                } else {
                    if (wrongMatches) {
                        wrongMatches->push_back(matches[i]);
                    }
                }
            }

        }

        return goodMatches;
    }

    /*!
     * \brief Cross test
     * Search for symmetrical matches
     * \param[in] matches12
     * \param[in] matches21
     * \param[out] goodMatches
     * \param[out] wrongMatches
     */
    static auto crossCheckTest(const std::vector<std::vector<cv::DMatch>> &matches12,
                               const std::vector<std::vector<cv::DMatch>> &matches21,
                               std::vector<cv::DMatch> *wrongMatches = nullptr) -> std::vector<cv::DMatch>
    {
        std::vector<cv::DMatch> goodMatches;

        for (size_t i = 0; i < matches12.size(); i++) {

            if (matches12[i].empty() || matches12[i].size() < 2)
                continue;

            bool findGoodMatch = false;
            for (size_t j = 0; j < matches21.size(); j++) {

                if (matches21[j].empty() || matches21[j].size() < 2)
                    continue;

                if (matches12[i][0].queryIdx == matches21[j][0].trainIdx &&
                    matches21[j][0].queryIdx == matches12[i][0].trainIdx) {
                    goodMatches.push_back(matches12[i][0]);
                    findGoodMatch = true;
                    break;
                }

            }

            if (findGoodMatch == false && wrongMatches)
                wrongMatches->push_back(matches12[i][0]);

        }

        return goodMatches;
    }

    auto geometricFilter(const std::vector<cv::DMatch> &matches,
                         const std::vector<cv::KeyPoint> &keypoints1,
                         const std::vector<cv::KeyPoint> &keypoints2,
                         std::vector<cv::DMatch> *wrongMatches = nullptr) -> std::vector<cv::DMatch>;

    /*!
     * \brief Matching
     * \param[in] queryDescriptor Query descriptor
     * \param[in] trainDescriptor Train descriptor
     * \param[out] wrongMatches Wrong matches
     * \return Good matches
     */
    auto match(const cv::Mat &queryDescriptor,
               const cv::Mat &trainDescriptor,
               std::vector<cv::DMatch> *wrongMatches = nullptr) -> std::vector<cv::DMatch>;

private:

    /*!
     * \brief Robust matching
     * Feature matching using ratio and symmetry tests
     * \param[in] queryDescriptor Query descriptor
     * \param[in] trainDescriptor Train descriptor
     * \param[out] wrongMatches Wrong matches
     * \return Good matches
     */
    auto robustMatch(const cv::Mat &queryDescriptor,
                     const cv::Mat &trainDescriptor,
                     std::vector<cv::DMatch> *wrongMatches) -> std::vector<cv::DMatch>;

    /*!
     * \brief Robust matching
     * Feature matching using ratio test
     * \param[in] queryDescriptor Query descriptor
     * \param[in] trainDescriptor Train descriptor
     * \param[out] wrongMatches Wrong matches
     * \return Good matches
     */
    auto fastRobustMatch(const cv::Mat &queryDescriptor,
                         const cv::Mat &trainDescriptor,
                         std::vector<cv::DMatch> *wrongMatches) -> std::vector<cv::DMatch>;


// MatchingAlgorithm interface

public:

    bool compute(const cv::Mat &queryDescriptor,
                 const cv::Mat &trainDescriptor,
                 const std::vector<cv::KeyPoint> &keypoints1,
                 const std::vector<cv::KeyPoint> &keypoints2,
                 std::vector<cv::DMatch> *goodMatches,
                 std::vector<cv::DMatch> *wrongMatches = nullptr,
                 const cv::Size &queryImageSize = cv::Size(),
                 const cv::Size &trainImageSize = cv::Size()) override;

};

} // namespace tl
