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

#include "tidop/featmatch/matching/matching.h"

namespace tl
{

/*! \addtogroup FeatureMatching
 *
 *  \{
 */


class TL_EXPORT RobustMatchingProperties
  : public MatchingStrategy
{

//private:
//
//    double mRatio;
//    bool mCrossCheck;

public:

    /*!
     * \brief Default constructor. Initializes the properties with default values.
     */
    RobustMatchingProperties();

    /*!
     * \brief Copy constructor.
     * \param[in] properties Another instance to copy properties from.
     */
    RobustMatchingProperties(const RobustMatchingProperties &properties);

    /*!
     * \brief Move constructor.
     * \param[in] properties Another instance to move properties from.
     */
    RobustMatchingProperties(RobustMatchingProperties &&properties) TL_NOEXCEPT;

    /*!
     * \brief Destructor.
     */
    ~RobustMatchingProperties() override = default;

    /*!
     * \brief Copy assignment operator.
     * \param[in] properties Another instance to copy properties from.
     * \return Reference to this instance.
     */
    auto operator=(const RobustMatchingProperties &properties) -> RobustMatchingProperties &;

    /*!
     * \brief Move assignment operator.
     * \param[in] properties Another instance to move properties from.
     * \return Reference to this instance.
     */
    auto operator=(RobustMatchingProperties &&properties) TL_NOEXCEPT -> RobustMatchingProperties &;

public:

    auto ratio() const -> double;
    void setRatio(double ratio);
    auto crossCheck() const -> bool;
    void setCrossCheck(bool crossCheck);

// MatchingStrategy interface

public:

    void reset() override;

};





class TL_EXPORT RobustMatchingImp
  : public MatchingAlgorithm
{

protected:

    std::shared_ptr<DescriptorMatcher> mDescriptorMatcher;
    RobustMatchingProperties mProperties;
    std::shared_ptr<GeometricTest> mGeometricTest;

public:

    explicit RobustMatchingImp(const std::shared_ptr<DescriptorMatcher> &descriptorMatcher);
    RobustMatchingImp(const std::shared_ptr<DescriptorMatcher> &descriptorMatcher,
                      const RobustMatchingProperties &properties,
                      const std::shared_ptr<GeometricTest> &geometricTest);
    ~RobustMatchingImp() override = default;

    /*!
     * \brief Get the GMS properties.
     * \return Reference to the properties object.
     *
     * The following property names are valid when accessed via the `Properties` class:
     * - `"Ratio"`
     * - `"CrossCheck"`
     */
    auto properties() -> RobustMatchingProperties &{ return mProperties; }

    /*!
     * \brief Get the GMS properties (const version).
     * \return Const reference to the properties object.
     *
     * The following property names are valid when accessed via the `Properties` class:
     * - `"Ratio"`
     * - `"CrossCheck"`
     */
    auto properties() const -> const RobustMatchingProperties &{ return mProperties; }

private:

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

    auto compute(const cv::Mat &queryDescriptor,
                 const cv::Mat &trainDescriptor,
                 const std::vector<cv::KeyPoint> &keypoints1,
                 const std::vector<cv::KeyPoint> &keypoints2,
                 std::vector<cv::DMatch> *wrongMatches = nullptr,
                 const cv::Size &queryImageSize = cv::Size(),
                 const cv::Size &trainImageSize = cv::Size()) -> std::vector<cv::DMatch> override;

};

/*! \} */

} // namespace tl
