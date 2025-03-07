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

#include "robustmatch.h"

#include "tidop/core/base/exception.h"
#include "tidop/core/base/common.h"
#include "tidop/featmatch/matching/test/geomtest.h"
#include "tidop/featmatch/matching/test/crosstest.h"
#include "tidop/featmatch/matching/test/ratiotest.h"

#ifdef HAVE_OPENCV_XFEATURES2D
#include <opencv2/xfeatures2d.hpp>
#endif // HAVE_OPENCV_XFEATURES2D
#include <opencv2/calib3d.hpp>

namespace tl
{

RobustMatchingProperties::RobustMatchingProperties()
  : MatchingStrategy("Robust Matcher", Strategy::robust_matching)
{
    reset();
}

RobustMatchingProperties::RobustMatchingProperties(const RobustMatchingProperties &properties)
  : MatchingStrategy(properties)
{
}

RobustMatchingProperties::RobustMatchingProperties(RobustMatchingProperties &&properties) TL_NOEXCEPT
  : MatchingStrategy(std::forward<MatchingStrategy>(properties))
{
}

auto RobustMatchingProperties::operator=(const RobustMatchingProperties &properties) -> RobustMatchingProperties &
{
    if (this != &properties) {
        MatchingStrategy::operator=(properties);
    }
    return *this;
}

auto RobustMatchingProperties::operator=(RobustMatchingProperties &&properties) TL_NOEXCEPT -> RobustMatchingProperties &
{
    if (this != &properties) {
        MatchingStrategy::operator=(std::forward<MatchingStrategy>(properties));
    }
    return *this;
}

auto RobustMatchingProperties::ratio() const -> double
{
    return getProperty<double>("Ratio");
}

void RobustMatchingProperties::setRatio(double ratio)
{
    setProperty("Ratio" , ratio);
}

auto RobustMatchingProperties::crossCheck() const -> bool
{
    return getProperty<bool>("CrossCheck");
}

void RobustMatchingProperties::setCrossCheck(bool crossCheck)
{
    setProperty("CrossCheck", crossCheck);
}

void RobustMatchingProperties::reset()
{
    setRatio(0.8);
    setCrossCheck(true);
}






RobustMatchingImp::RobustMatchingImp(const std::shared_ptr<DescriptorMatcher> &descriptorMatcher)
  : mDescriptorMatcher(descriptorMatcher)
{

}

RobustMatchingImp::RobustMatchingImp(const std::shared_ptr<DescriptorMatcher> &descriptorMatcher,
                                     const RobustMatchingProperties &properties,
                                     const std::shared_ptr<GeometricTest> &geometricTest)
  : mDescriptorMatcher(descriptorMatcher),
    mProperties(properties),
    mGeometricTest(geometricTest)
{

}

auto RobustMatchingImp::geometricFilter(const std::vector<cv::DMatch> &matches,
                                        const std::vector<cv::KeyPoint> &keypoints1,
                                        const std::vector<cv::KeyPoint> &keypoints2,
                                        std::vector<cv::DMatch> *wrongMatches) -> std::vector<cv::DMatch>
{
    std::vector<cv::DMatch> filter_matches;

    if (!mGeometricTest) return matches;

    // Convert keypoints into Point2f
    size_t nPoints = matches.size();
    std::vector<cv::Point2f> pts1(nPoints);
    std::vector<cv::Point2f> pts2(nPoints);
    for (size_t i = 0; i < nPoints; i++) {
        pts1[i] = keypoints1[static_cast<size_t>(matches[i].queryIdx)].pt;
        pts2[i] = keypoints2[static_cast<size_t>(matches[i].trainIdx)].pt;
    }

    std::vector<uchar> inliers = mGeometricTest->exec(cv::Mat(pts1), cv::Mat(pts2));

    for (size_t i = 0; i < nPoints; i++) {
        if (inliers[i]) {
            filter_matches.push_back(matches[i]);
        } else {
            if (wrongMatches) wrongMatches->push_back(matches[i]);
        }
    }

    return filter_matches;
}

auto RobustMatchingImp::match(const cv::Mat &queryDescriptor,
                              const cv::Mat &trainDescriptor,
                              std::vector<cv::DMatch> *wrongMatches) -> std::vector<cv::DMatch>
{
    if (mProperties.crossCheck()) {
        return this->robustMatch(queryDescriptor, trainDescriptor, wrongMatches);
    } else {
        return this->fastRobustMatch(queryDescriptor, trainDescriptor, wrongMatches);
    }
}

auto RobustMatchingImp::robustMatch(const cv::Mat &queryDescriptor,
                                    const cv::Mat &trainDescriptor,
                                    std::vector<cv::DMatch> *wrongMatches) -> std::vector<cv::DMatch>
{
    std::vector<cv::DMatch> goodMatches;

    try {
        std::vector<std::vector<cv::DMatch>> matches12;
        std::vector<std::vector<cv::DMatch>> matches21;

        mDescriptorMatcher->match(queryDescriptor, trainDescriptor, matches12);

        mDescriptorMatcher->match(trainDescriptor, queryDescriptor, matches21);

        std::vector<std::vector<cv::DMatch>> wrong_matches12;
        std::vector<std::vector<cv::DMatch>> wrong_matches21;
        auto good_matches12 = nearestNeighbourDistanceRatioTest(matches12, mProperties.ratio(), &wrong_matches12);
        auto good_matches21 = nearestNeighbourDistanceRatioTest(matches21, mProperties.ratio(), &wrong_matches21);

        matches12.clear();
        matches21.clear();

        if (wrongMatches) {
            for (auto &wrong_match : wrong_matches12) {
                wrongMatches->push_back(wrong_match[0]);
            }
        }

        goodMatches = crossCheckTest(good_matches12, good_matches21, wrongMatches);

    } catch (...) {
        std::throw_with_nested(std::runtime_error("RobustMatchingImp::robustMatch() failed"));
    }

    return goodMatches;
}

auto RobustMatchingImp::fastRobustMatch(const cv::Mat &queryDescriptor,
                                        const cv::Mat &trainDescriptor,
                                        std::vector<cv::DMatch> *wrongMatches) -> std::vector<cv::DMatch>
{
    std::vector<cv::DMatch> goodMatches;

    try {

        std::vector<std::vector<cv::DMatch>> matches;
        mDescriptorMatcher->match(queryDescriptor, trainDescriptor, matches);

        std::vector<std::vector<cv::DMatch>> ratio_test_wrong_matches;
        auto ratio_test_matches = nearestNeighbourDistanceRatioTest(matches, mProperties.ratio(), &ratio_test_wrong_matches);

        for (auto &match : ratio_test_matches) {
            goodMatches.push_back(match[0]);
        }

        if (wrongMatches) {
            for (auto &wrong_match : ratio_test_wrong_matches) {
                wrongMatches->push_back(wrong_match[0]);
            }
        }

    } catch (...) {
        TL_THROW_EXCEPTION_WITH_NESTED("Catched exception");
    }

    return goodMatches;
}

auto RobustMatchingImp::compute(const cv::Mat &queryDescriptor,
                                const cv::Mat &trainDescriptor,
                                const std::vector<cv::KeyPoint> &keypoints1,
                                const std::vector<cv::KeyPoint> &keypoints2,
                                std::vector<cv::DMatch> *wrongMatches,
                                const cv::Size &queryImageSize,
                                const cv::Size &trainImageSize) -> std::vector<cv::DMatch>
{
    unusedParameter(queryImageSize, trainImageSize);

    std::vector<cv::DMatch> goodMatches;

    try {

        goodMatches = this->match(queryDescriptor, trainDescriptor, wrongMatches);
        goodMatches = this->geometricFilter(goodMatches, keypoints1, keypoints2, wrongMatches);

    } catch (std::exception &e) {
        printException(e);
    }

    return goodMatches;
}

} // namespace tl
