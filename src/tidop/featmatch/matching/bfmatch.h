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

#include "tidop/featmatch/base/matcher.h"

#include <vector>

#ifdef HAVE_OPENCV_CUDAFEATURES2D
#include <opencv2/cudafeatures2d.hpp>
#endif // HAVE_OPENCV_CUDAFEATURES2D

namespace tl
{

class TL_EXPORT BruteForceMatcherProperties
    : public BruteForceMatcher
{

private:

    Norm mNormType{Norm::l2};

public:

    BruteForceMatcherProperties();
    ~BruteForceMatcherProperties() override = default;

// Match interface

public:

    void reset() override;
    std::string name() const final;

// BruteForceMatcher interface

public:

    Norm normType() const override;
    void setNormType(Norm normType) override;

};


/*----------------------------------------------------------------*/


class TL_EXPORT BruteForceMatcherImp
  : public BruteForceMatcherProperties,
    public DescriptorMatcher
{

private:

    cv::Ptr<cv::BFMatcher> mBFMatcher;

public:

    BruteForceMatcherImp();
    explicit BruteForceMatcherImp(Norm normType);
    ~BruteForceMatcherImp() override = default;

private:

    void update();

// DescriptorMatcher interface

public:

    void match(const cv::Mat &queryDescriptors,
               const cv::Mat &trainDescriptors,
               std::vector<cv::DMatch> &matches,
               const cv::Mat mask = cv::Mat()) override;

    void match(const cv::Mat &queryDescriptors,
               const cv::Mat &trainDescriptors,
               std::vector<std::vector<cv::DMatch>> &matches,
               const cv::Mat mask = cv::Mat()) override;

// Match interface

public:

    void reset() override;

// BruteForceMatcher interface

public:

    void setNormType(Norm normType) override;

};


/*----------------------------------------------------------------*/


#ifdef HAVE_OPENCV_CUDAFEATURES2D

class TL_EXPORT BruteForceMatcherCuda
  : public BruteForceMatcherProperties,
    public DescriptorMatcher
{

private:

    cv::Ptr<cv::cuda::DescriptorMatcher> mBFMatcher;

public:

    BruteForceMatcherCuda();
    explicit BruteForceMatcherCuda(Norm normType);
    ~BruteForceMatcherCuda() override = default;

private:

    void update();

// DescriptorMatcher interface

public:

    void match(const cv::Mat &queryDescriptors,
               const cv::Mat &trainDescriptors,
               std::vector<cv::DMatch> &matches,
               const cv::Mat mask = cv::Mat()) override;

    void match(const cv::Mat &queryDescriptors,
               const cv::Mat &trainDescriptors,
               std::vector<std::vector<cv::DMatch>> &matches,
               const cv::Mat mask = cv::Mat()) override;

// Match interface

public:

    void reset() override;

// BruteForceMatcher interface

public:

    void setNormType(Norm normType) override;

};

#endif // HAVE_OPENCV_CUDAFEATURES2D

} // namespace tl
