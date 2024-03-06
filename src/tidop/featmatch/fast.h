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

#include "tidop/featmatch/features.h"

#ifdef HAVE_OPENCV_CUDAFEATURES2D
#include <opencv2/cudafeatures2d.hpp>
#endif // HAVE_OPENCV_CUDAFEATURES2D

namespace tl
{

/*! \addtogroup Features
 * 
 *  \{
 */

/*! \addtogroup FeatureDetectorAndDescriptor
 * 
 *  \{
 */


constexpr auto fast_default_value_threshold{10};
constexpr auto fast_default_value_nonmax_suppression{true};
constexpr auto fast_default_value_detector_type{"TYPE_9_16"};


class TL_EXPORT FastProperties
  : public Fast
{

private:

    int mThreshold{fast_default_value_threshold};
    bool mNonmaxSuppression{fast_default_value_nonmax_suppression};
    std::string mDetectorType;

public:

    FastProperties();
    FastProperties(const FastProperties &fastProperties);
    ~FastProperties() override = default;

// Fast interface

public:

    auto threshold() const -> int override;
    auto nonmaxSuppression() const -> bool override;
    auto detectorType() const -> std::string override;
    void setThreshold(int threshold) override;
    void setNonmaxSuppression(bool nonmaxSuppression) override;
    void setDetectorType(const std::string &detectorType) override;

// Feature interface

public:

    void reset() override;
    auto name() const -> std::string final;

};


/*----------------------------------------------------------------*/


class TL_EXPORT FastDetector
  : public FastProperties,
    public KeypointDetector
{

private:

  cv::Ptr<cv::FastFeatureDetector> mFast;

public:

    FastDetector();
    FastDetector(const FastDetector &fastDetector);
    FastDetector(int threshold,
                 bool nonmaxSuppression,
                 const std::string &detectorType);
    ~FastDetector() override = default;

private:

#if CV_VERSION_MAJOR >= 4
    auto convertDetectorType(const std::string &detectorType) -> cv::FastFeatureDetector::DetectorType;
#else
    auto convertDetectorType(const std::string &detectorType) -> int;
#endif

// KeypointDetector interface

public:

    auto detect(const cv::Mat &img, cv::InputArray &mask = cv::noArray()) -> std::vector<cv::KeyPoint> override;

// Fast interface

public:

    void setThreshold(int threshold) override;
    void setNonmaxSuppression(bool nonmaxSuppression) override;
    void setDetectorType(const std::string &detectorType) override;

// Feature interface

public:

    void reset() override;

};


/*----------------------------------------------------------------*/


class TL_EXPORT FastDetectorCuda
  : public FastProperties,
    public KeypointDetector
{

private:

#ifdef HAVE_OPENCV_CUDAFEATURES2D
    cv::Ptr<cv::cuda::FastFeatureDetector> mFast;
#endif // HAVE_OPENCV_CUDAFEATURES2D

public:

    FastDetectorCuda();
    FastDetectorCuda(const FastDetectorCuda &fastDetector);
    FastDetectorCuda(int threshold,
                     bool nonmaxSuppression,
                     const std::string &detectorType);
    ~FastDetectorCuda() override = default;

private:

    auto convertDetectorType(const std::string &detectorType) -> int;

    void update();

// KeypointDetector interface

public:

    auto detect(const cv::Mat &img, cv::InputArray &mask = cv::noArray()) -> std::vector<cv::KeyPoint> override;

// Fast interface

public:

    void setThreshold(int threshold) override;
    void setNonmaxSuppression(bool nonmaxSuppression) override;
    void setDetectorType(const std::string &detectorType) override;

// Feature interface

public:

    void reset() override;

};



/*! \} */ // end of FeatureDetectorAndDescriptor

/*! \} */ // end of Features

} // namespace tl
