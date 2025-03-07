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

#include "bfmatch.h"

#include "tidop/core/base/exception.h"

namespace tl
{


BruteForceMatcherProperties::BruteForceMatcherProperties()
  : MatchingMethod("Brute Force Matching", Type::brute_force)
{
    reset();
}

BruteForceMatcherProperties::BruteForceMatcherProperties(const BruteForceMatcherProperties &properties)
  : MatchingMethod(properties)
{
}

BruteForceMatcherProperties::BruteForceMatcherProperties(BruteForceMatcherProperties &&properties) TL_NOEXCEPT
  : MatchingMethod(std::forward<MatchingMethod>(properties))
{
}

auto BruteForceMatcherProperties::operator=(const BruteForceMatcherProperties &properties) -> BruteForceMatcherProperties &
{
    if (this != &properties) {
        MatchingMethod::operator=(properties);
    }
    return *this;
}

auto BruteForceMatcherProperties::operator=(BruteForceMatcherProperties &&properties) TL_NOEXCEPT -> BruteForceMatcherProperties &
{
    if (this != &properties) {
        MatchingMethod::operator=(std::forward<MatchingMethod>(properties));
    }
    return *this;
}

auto BruteForceMatcherProperties::normType() const -> std::string
{
    return getProperty<std::string>("NormType");
}

void BruteForceMatcherProperties::setNormType(const std::string &normType)
{
    if (normType == "L1" ||
        normType == "L2" ||
        normType == "HAMMING" || 
        normType == "HAMMING2") {
        setProperty("NormType", normType);
    } else {
        Message::warning("'NormType' value not valid: {}", normType);
    }
}

void BruteForceMatcherProperties::reset()
{
    setNormType("L2");
}





/* Brute force matcher */

BruteForceMatcher::BruteForceMatcher()
  : mProperties()
{
    init();
}

BruteForceMatcher::BruteForceMatcher(const BruteForceMatcherProperties &properties)
  : mProperties(properties)
{
    init();
}

BruteForceMatcher::BruteForceMatcher(const BruteForceMatcher &bf)
  : mProperties(bf.mProperties)
{
    init();
}

BruteForceMatcher::BruteForceMatcher(BruteForceMatcher &&bf) TL_NOEXCEPT
  : mProperties(std::move(bf.mProperties)),
    mBFMatcher(std::move(bf.mBFMatcher))
{
}

auto BruteForceMatcher::operator =(const BruteForceMatcher &bf) -> BruteForceMatcher &
{
    if (this != &bf) {
        mProperties = bf.mProperties;
        init();
    }
    return *this;
}

auto BruteForceMatcher::operator =(BruteForceMatcher &&bf) TL_NOEXCEPT -> BruteForceMatcher &
{
    if (this != &bf) {
        mProperties = std::move(bf.mProperties);
        mBFMatcher = std::move(bf.mBFMatcher);
    }
    return *this;
}

void BruteForceMatcher::init()
{
    int norm = cv::NORM_L2;
    auto norm_type = mProperties.normType();
    if (norm_type == "L1") {
        norm = cv::NORM_L1;
    } else if (norm_type == "L2") {
        norm = cv::NORM_L2;
    } else if (norm_type == "HAMMING") {
        norm = cv::NORM_HAMMING;
    } else if (norm_type == "HAMMING2") {
        norm = cv::NORM_HAMMING2;
    }

    try {
        mBFMatcher = cv::BFMatcher::create(norm);
    } catch (...) {
        TL_THROW_EXCEPTION_WITH_NESTED("Catched exception");
    }
}

void BruteForceMatcher::match(const cv::Mat &queryDescriptors,
                                 const cv::Mat &trainDescriptors,
                                 std::vector<cv::DMatch> &matches,
                                 const cv::Mat mask)
{
    try {

        mBFMatcher->match(queryDescriptors, trainDescriptors, matches, mask);

    } catch (...) {
        TL_THROW_EXCEPTION_WITH_NESTED("Catched exception");
    }
}

void BruteForceMatcher::match(const cv::Mat &queryDescriptors,
                                 const cv::Mat &trainDescriptors,
                                 std::vector<std::vector<cv::DMatch>> &matches,
                                 int k,
                                 const cv::Mat mask)
{
    try {

        mBFMatcher->knnMatch(queryDescriptors, trainDescriptors, matches, k, mask);

    } catch (...) {
        TL_THROW_EXCEPTION_WITH_NESTED("Catched exception");
    }
}





#ifdef HAVE_OPENCV_CUDAFEATURES2D

/* Brute force matcher Cuda */

BruteForceMatcherCuda::BruteForceMatcherCuda()
  : mProperties()
{
    init();
}

BruteForceMatcherCuda::BruteForceMatcherCuda(const BruteForceMatcherProperties &properties)
  : mProperties(properties)
{
    init();
}

BruteForceMatcherCuda::BruteForceMatcherCuda(const BruteForceMatcherCuda &bf)
  : mProperties(bf.mProperties)
{
    init();
}

BruteForceMatcherCuda::BruteForceMatcherCuda(BruteForceMatcherCuda &&bf) TL_NOEXCEPT
  : mProperties(std::move(bf.mProperties)),
    mBFMatcher(std::move(bf.mBFMatcher))
{
}

auto BruteForceMatcherCuda::operator =(const BruteForceMatcherCuda &bf) -> BruteForceMatcherCuda &
{
    if (this != &bf) {
        mProperties = bf.mProperties;
        init();
    }
    return *this;
}

auto BruteForceMatcherCuda::operator =(BruteForceMatcherCuda &&bf) TL_NOEXCEPT -> BruteForceMatcherCuda &
{
    if (this != &bf) {
        mProperties = std::move(bf.mProperties);
        mBFMatcher = std::move(bf.mBFMatcher);
    }
    return *this;
}

void BruteForceMatcherCuda::init()
{
    int norm = cv::NORM_L2;
    auto norm_type = mProperties.normType();
    if (norm_type == "L1") {
        norm = cv::NORM_L1;
    } else if (norm_type == "L2") {
        norm = cv::NORM_L2;
    } else if (norm_type == "HAMMING") {
        norm = cv::NORM_HAMMING;
    } else if (norm_type == "HAMMING2") {
        Message::warning("The implementation of BFMatcher with Cuda does not include NORM_HAMMING2. L2 is used");
    }

    try {

        mBFMatcher = cv::cuda::DescriptorMatcher::createBFMatcher(norm);

    } catch (...) {
        TL_THROW_EXCEPTION_WITH_NESTED("Catched exception");
    }
}

void BruteForceMatcherCuda::match(const cv::Mat &queryDescriptors,
                                  const cv::Mat &trainDescriptors,
                                  std::vector<cv::DMatch> &matches,
                                  const cv::Mat mask)
{
    try {

        cv::cuda::GpuMat query_descriptors(queryDescriptors);
        cv::cuda::GpuMat train_descriptors(trainDescriptors);
        cv::cuda::GpuMat gpu_mask;
        if (!mask.empty()) gpu_mask.upload(mask);
        mBFMatcher->match(query_descriptors, train_descriptors, matches, gpu_mask);

    } catch (...) {
        TL_THROW_EXCEPTION_WITH_NESTED("Catched exception");
    }
}

void BruteForceMatcherCuda::match(const cv::Mat &queryDescriptors,
                                  const cv::Mat &trainDescriptors,
                                  std::vector<std::vector<cv::DMatch>> &matches,
                                  int k,
                                  const cv::Mat mask)
{
    try {

        cv::cuda::GpuMat query_descriptors(queryDescriptors);
        cv::cuda::GpuMat train_descriptors(trainDescriptors);
        cv::cuda::GpuMat gpu_mask;
        if (!mask.empty()) gpu_mask.upload(mask);
        mBFMatcher->knnMatch(query_descriptors, train_descriptors, matches, k, gpu_mask);

    } catch (...) {
        TL_THROW_EXCEPTION_WITH_NESTED("Catched exception");
    }
}


#endif // HAVE_OPENCV_CUDAFEATURES2D

} // namespace tl
