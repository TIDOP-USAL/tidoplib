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


BruteForceMatcherProperties::BruteForceMatcherProperties() = default;

void BruteForceMatcherProperties::reset()
{
    mNormType = Norm::l2;
}

std::string BruteForceMatcherProperties::name() const
{
    return std::string("Brute Force Matching");
}

BruteForceMatcherProperties::Norm BruteForceMatcherProperties::normType() const
{
    return mNormType;
}

void BruteForceMatcherProperties::setNormType(Norm normType)
{
    mNormType = normType;
}

/*----------------------------------------------------------------*/


BruteForceMatcherImp::BruteForceMatcherImp()
{
    update();
}

BruteForceMatcherImp::BruteForceMatcherImp(Norm normType)
{
    BruteForceMatcherProperties::setNormType(normType);
    update();
}

void BruteForceMatcherImp::update()
{
    int norm = cv::NORM_L2;
    Norm norm_type = BruteForceMatcherProperties::normType();
    if (norm_type == Norm::l1) {
        norm = cv::NORM_L1;
    } else if (norm_type == Norm::l2) {
        norm = cv::NORM_L2;
    } else if (norm_type == Norm::hamming) {
        norm = cv::NORM_HAMMING;
    } else if (norm_type == Norm::hamming2) {
        norm = cv::NORM_HAMMING2;
    }

    try {
        mBFMatcher = cv::BFMatcher::create(norm);
    } catch (...) {
        TL_THROW_EXCEPTION_WITH_NESTED("Catched exception");
    }
}

void BruteForceMatcherImp::match(const cv::Mat &queryDescriptors,
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

void BruteForceMatcherImp::match(const cv::Mat &queryDescriptors,
                                 const cv::Mat &trainDescriptors,
                                 std::vector<std::vector<cv::DMatch>> &matches,
                                 const cv::Mat mask)
{
    try {

        mBFMatcher->knnMatch(queryDescriptors, trainDescriptors, matches, 2, mask);

    } catch (...) {
        TL_THROW_EXCEPTION_WITH_NESTED("Catched exception");
    }
}

void BruteForceMatcherImp::reset()
{
    BruteForceMatcherProperties::reset();
    update();
}

void BruteForceMatcherImp::setNormType(Norm normType)
{
    BruteForceMatcherProperties::setNormType(normType);
    update();
}

/*----------------------------------------------------------------*/

#ifdef HAVE_OPENCV_CUDAFEATURES2D

BruteForceMatcherCuda::BruteForceMatcherCuda()
{
    update();
}

BruteForceMatcherCuda::BruteForceMatcherCuda(Norm normType)
{
    BruteForceMatcherProperties::setNormType(normType);
    update();
}

void BruteForceMatcherCuda::update()
{
    int norm = cv::NORM_L2;
    Norm norm_type = BruteForceMatcherProperties::normType();
    if (norm_type == Norm::l1) {
        norm = cv::NORM_L1;
    } else if (norm_type == Norm::l2) {
        norm = cv::NORM_L2;
    } else if (norm_type == Norm::hamming) {
        norm = cv::NORM_HAMMING;
    }
    ///La implementación de BFMatcher con Cuda no incluye NORM_HAMMING2
    /*else if (norm_type == BruteForceMatcherProperties::Norm::hamming2) {
      norm = cv::NORM_HAMMING2;
    }*/

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
                                  const cv::Mat mask)
{
    try {

        cv::cuda::GpuMat query_descriptors(queryDescriptors);
        cv::cuda::GpuMat train_descriptors(trainDescriptors);
        cv::cuda::GpuMat gpu_mask;
        if (!mask.empty()) gpu_mask.upload(mask);
        mBFMatcher->knnMatch(query_descriptors, train_descriptors, matches, 2, gpu_mask);

    } catch (...) {
        TL_THROW_EXCEPTION_WITH_NESTED("Catched exception");
    }
}

void BruteForceMatcherCuda::reset()
{
    BruteForceMatcherProperties::reset();
    update();
}

void BruteForceMatcherCuda::setNormType(Norm normType)
{
    BruteForceMatcherProperties::setNormType(normType);
    update();
}

#endif // HAVE_OPENCV_CUDAFEATURES2D

} // namespace tl
