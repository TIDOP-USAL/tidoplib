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
 * License along with Foobar. If not, see <http://www.gnu.org/licenses/>. *
 *                                                                        *
 * @license LGPL-3.0 <https://www.gnu.org/licenses/lgpl-3.0.html>         *
 *                                                                        *
 **************************************************************************/

#ifndef TL_FEATMATCH_ROBUST_MATCHING_H
#define TL_FEATMATCH_ROBUST_MATCHING_H

#include "tidop/featmatch/matcher.h"

namespace tl
{

class TL_EXPORT RobustMatchingProperties
  : public RobustMatcher
{

public:

  RobustMatchingProperties();
  ~RobustMatchingProperties() override = default;

// IRobustMatcherRefinement interface

public:

  double ratio() const override;
  void setRatio(double ratio) override;
  bool crossCheck() const override;
  void setCrossCheck(bool crossCheck) override;
  GeometricTest geometricTest() const override;
  void setGeometricTest(GeometricTest geometricTest) override;
  HomographyComputeMethod homographyComputeMethod() const override;
  void setHomographyComputeMethod(HomographyComputeMethod computeMethod) override;
  FundamentalComputeMethod fundamentalComputeMethod() const override;
  void setFundamentalComputeMethod(FundamentalComputeMethod computeMethod) override;
  EssentialComputeMethod essentialComputeMethod() const override;
  void setEssentialComputeMethod(EssentialComputeMethod computeMethod) override;
  double distance() const override;
  void setDistance(double distance) override;
  double confidence() const override;
  void setConfidence(double confidence) override;
  int maxIter() const override;
  void setMaxIters(int maxIter) override;

// MatchingStrategy interface

public:

  void reset() override;
  std::string name() const override;

private:

  double mRatio;
  bool mCrossCheck;
  GeometricTest mGeometricTest;
  HomographyComputeMethod mHomographyComputeMethod;
  FundamentalComputeMethod mFundamentalComputeMethod;
  EssentialComputeMethod mEssentialComputeMethod;
  double mDistance;
  double mConfidence;
  int mMaxIters;

};


/*----------------------------------------------------------------*/


class TL_EXPORT RobustMatchingImp
  : public RobustMatchingProperties,
    public MatchingAlgorithm
{

public:

  explicit RobustMatchingImp(std::shared_ptr<DescriptorMatcher> descriptorMatcher);
  RobustMatchingImp(std::shared_ptr<DescriptorMatcher> descriptorMatcher,
                    double ratio,
                    bool crossCheck,
                    GeometricTest geometricTest,
                    HomographyComputeMethod homographyComputeMethod,
                    FundamentalComputeMethod fundamentalComputeMethod,
                    EssentialComputeMethod essentialComputeMethod,
                    double distance,
                    double confidence,
                    int maxIter);
  ~RobustMatchingImp() override = default;

  /*!
   * \brief Establece el metodo de matching
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
  static std::vector<std::vector<cv::DMatch>> ratioTest(const std::vector<std::vector<cv::DMatch>> &matches,
                                                        double ratio,
                                                        std::vector<std::vector<cv::DMatch>> *wrongMatches = nullptr)
  {
    std::vector<std::vector<cv::DMatch>> goodMatches;

    for (size_t i = 0; i < matches.size(); i++){

      if (matches[i].size() > 1){
        // check distance ratio
        if (matches[i][0].distance / matches[i][1].distance <= static_cast<float>(ratio)) {
            goodMatches.push_back(matches[i]);
        } else {
          if (wrongMatches){
            wrongMatches->push_back(matches[i]);
          }
        }
      }

    }

    return goodMatches;
  }

  /*!
   * \brief test cruzado
   * Busqueda de matches simétricos
   * \param[in] matches12
   * \param[in] matches21
   * \param[out] goodMatches
   * \param[out] wrongMatches
   */
  static std::vector<cv::DMatch> crossCheckTest(const std::vector<std::vector<cv::DMatch>> &matches12,
                                                const std::vector<std::vector<cv::DMatch>> &matches21,
                                                std::vector<cv::DMatch> *wrongMatches = nullptr)
  {
    std::vector<cv::DMatch> goodMatches;

    for (size_t i = 0; i < matches12.size(); i++){

      if (matches12[i].empty() || matches12[i].size() < 2)
        continue;

      bool findGoodMatch = false;
      for (size_t j = 0; j < matches21.size(); j++){

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

  std::vector<cv::DMatch> geometricFilter(const std::vector<cv::DMatch> &matches,
                                          const std::vector<cv::KeyPoint>& keypoints1,
                                          const std::vector<cv::KeyPoint>& keypoints2,
                                          std::vector<cv::DMatch> *wrongMatches = nullptr);

  std::vector<cv::DMatch> filterByHomographyMatrix(const std::vector<cv::DMatch> &matches,
                                                   const std::vector<cv::Point2f>& points1,
                                                   const std::vector<cv::Point2f>& points2,
                                                   std::vector<cv::DMatch> *wrongMatches = nullptr);
  std::vector<cv::DMatch> filterByEssentialMatrix(const std::vector<cv::DMatch> &matches,
                                                  const std::vector<cv::Point2f>& points1,
                                                  const std::vector<cv::Point2f>& points2,
                                                  std::vector<cv::DMatch> *wrongMatches = nullptr);
  std::vector<cv::DMatch> filterByFundamentalMatrix(const std::vector<cv::DMatch> &matches,
                                                    const std::vector<cv::Point2f>& points1,
                                                    const std::vector<cv::Point2f>& points2,
                                                    std::vector<cv::DMatch> *wrongMatches = nullptr);

  /*!
   * \brief Matching
   * \param[in] queryDescriptor Query descriptor
   * \param[in] trainDescriptor Train descriptor
   * \param[out] wrongMatches Wrong matches
   * \return Good matches
   */
  std::vector<cv::DMatch> match(const cv::Mat &queryDescriptor,
                                const cv::Mat &trainDescriptor,
                                std::vector<cv::DMatch> *wrongMatches = nullptr);

private:

  /*!
   * \brief Robust matching
   * Feature matching using ratio and symmetry tests
   * \param[in] queryDescriptor Query descriptor
   * \param[in] trainDescriptor Train descriptor
   * \param[out] wrongMatches Wrong matches
   * \return Good matches
   */
  std::vector<cv::DMatch> robustMatch(const cv::Mat &queryDescriptor,
                                      const cv::Mat &trainDescriptor,
                                      std::vector<cv::DMatch> *wrongMatches);

  /*!
   * \brief Robust matching
   * Feature matching using ratio test
   * \param[in] queryDescriptor Query descriptor
   * \param[in] trainDescriptor Train descriptor
   * \param[out] wrongMatches Wrong matches
   * \return Good matches
   */
  std::vector<cv::DMatch> fastRobustMatch(const cv::Mat &queryDescriptor,
                                          const cv::Mat &trainDescriptor,
                                          std::vector<cv::DMatch> *wrongMatches);


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

protected:

  std::shared_ptr<DescriptorMatcher> mDescriptorMatcher;

};

} // namespace tl

#endif // TL_FEATMATCH_ROBUST_MATCHING_H
