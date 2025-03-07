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

#include "fast.h"

#include "tidop/core/base/exception.h"


namespace tl
{


/* FAST properties */

FastProperties::FastProperties()
  : Feature("FAST", Feature::Type::fast)
{
    reset();
}

FastProperties::FastProperties(const FastProperties &properties)
  : Feature(properties)
{
}

FastProperties::FastProperties(FastProperties &&properties) TL_NOEXCEPT
  : Feature(std::forward<Feature>(properties))
{
}

auto FastProperties::operator=(const FastProperties &properties) -> FastProperties &
{
    if (this != &properties) {
        Feature::operator=(properties);
    }
    return *this;
}

auto FastProperties::operator=(FastProperties &&properties) TL_NOEXCEPT -> FastProperties &
{
    if (this != &properties) {
        Feature::operator=(std::forward<Feature>(properties));
    }
    return *this;
}

int FastProperties::threshold() const
{
    return getProperty<int>("Threshold");
}

bool FastProperties::nonmaxSuppression() const
{
    return getProperty<bool>("NonmaxSuppression");
}

std::string FastProperties::detectorType() const
{
    return getProperty<std::string>("DetectorType");
}

void FastProperties::setThreshold(int threshold)
{
    setProperty("Threshold", threshold);
}

void FastProperties::setNonmaxSuppression(bool nonmaxSuppression)
{
    setProperty("NonmaxSuppression", nonmaxSuppression);
}

void FastProperties::setDetectorType(const std::string &detectorType)
{
    if (detectorType == "TYPE_5_8" ||
        detectorType == "TYPE_7_12" ||
        detectorType == "TYPE_9_16") {
        setProperty("DetectorType", detectorType);
    } else {
        Message::warning("'DetectorType' value not valid: {}", detectorType);
    }
}

void FastProperties::reset()
{
    setThreshold(fast_default_value_threshold);
    setNonmaxSuppression(fast_default_value_nonmax_suppression);
    setDetectorType(fast_default_value_detector_type);
}



/* FAST detector */

FastDetector::FastDetector()
  : mProperties()
{
    init();
}

FastDetector::FastDetector(const FastProperties &properties)
    : mProperties(properties)
{
    init();
}

FastDetector::FastDetector(const FastDetector &fast)
  : mProperties(fast.mProperties)
{
    init();
}

FastDetector::FastDetector(FastDetector &&fast) TL_NOEXCEPT
  : mProperties(std::move(fast.mProperties)),
    mFast(std::move(fast.mFast))
{
}

auto FastDetector::operator =(const FastDetector &fast) -> FastDetector &
{
    if (this != &fast) {
        mProperties = fast.mProperties;
        init();
    }
    return *this;
}

auto FastDetector::operator =(FastDetector &&fast) TL_NOEXCEPT -> FastDetector &
{
    if (this != &fast) {
        mProperties = std::move(fast.mProperties);
        mFast = std::move(fast.mFast);
    }
    return *this;
}

#if CV_VERSION_MAJOR >= 4

auto FastDetector::convertDetectorType(const std::string& detectorType) -> cv::FastFeatureDetector::DetectorType
{
    cv::FastFeatureDetector::DetectorType type = cv::FastFeatureDetector::DetectorType::TYPE_9_16;

    if (detectorType == "TYPE_5_8") {
        type = cv::FastFeatureDetector::TYPE_5_8;
    } else if (detectorType == "TYPE_7_12") {
        type = cv::FastFeatureDetector::TYPE_7_12;
    } else if (detectorType == "TYPE_9_16") {
        type = cv::FastFeatureDetector::TYPE_9_16;
    }

    return type;
}

#else

int FastDetector::convertDetectorType(const std::string &detectorType)
{
    int type = cv::FastFeatureDetector::TYPE_9_16;

    if (detectorType == "TYPE_5_8") {
        type = cv::FastFeatureDetector::TYPE_5_8;
    } else if (detectorType == "TYPE_7_12") {
        type = cv::FastFeatureDetector::TYPE_7_12;
    } else if (detectorType == "TYPE_9_16") {
        type = cv::FastFeatureDetector::TYPE_9_16;
    }

    return type;
}
#endif

void FastDetector::init()
{
    mFast = cv::FastFeatureDetector::create(mProperties.threshold(),
                                            mProperties.nonmaxSuppression(),
                                            convertDetectorType(mProperties.detectorType()));
}

auto FastDetector::detect(const cv::Mat& img, cv::InputArray& mask) -> std::vector<cv::KeyPoint>
{
    std::vector<cv::KeyPoint> keyPoints;

    try {

        mFast->detect(img, keyPoints, mask);

    } catch (...) {
        TL_THROW_EXCEPTION_WITH_NESTED("Catched exception");
    }

    return keyPoints;
}




/* FAST detector Cuda */

FastDetectorCuda::FastDetectorCuda()
  : mProperties() 
{
    init();
}

FastDetectorCuda::FastDetectorCuda(const FastProperties &properties)
  : mProperties(properties)
{
    init();
}

FastDetectorCuda::FastDetectorCuda(const FastDetectorCuda &fast)
  : mProperties(fast.mProperties)
{
    init();
}

FastDetectorCuda::FastDetectorCuda(FastDetectorCuda &&fast) TL_NOEXCEPT
  : mProperties(std::move(fast.mProperties)),
    mFast(std::move(fast.mFast))
{
}

auto FastDetectorCuda::operator =(const FastDetectorCuda &fast) -> FastDetectorCuda &
{
    if (this != &fast) {
        mProperties = fast.mProperties;
        init();
    }
    return *this;
}

auto FastDetectorCuda::operator =(FastDetectorCuda &&fast) TL_NOEXCEPT -> FastDetectorCuda &
{
    if (this != &fast) {
        mProperties = std::move(fast.mProperties);
        mFast = std::move(fast.mFast);
    }
    return *this;
}

auto FastDetectorCuda::convertDetectorType(const std::string& detectorType) -> int
{
    int type = cv::FastFeatureDetector::TYPE_9_16;

    if (detectorType == "TYPE_5_8") {
        type = cv::FastFeatureDetector::TYPE_5_8;
    } else if (detectorType == "TYPE_7_12") {
        type = cv::FastFeatureDetector::TYPE_7_12;
    } else if (detectorType == "TYPE_9_16") {
        type = cv::FastFeatureDetector::TYPE_9_16;
    }

    return type;
}

void FastDetectorCuda::init()
{
#ifdef HAVE_OPENCV_CUDAFEATURES2D
    mFast = cv::cuda::FastFeatureDetector::create(mProperties.threshold(),
                                                  mProperties.nonmaxSuppression(),
                                                  convertDetectorType(mProperties.detectorType()),
                                                  10000);
#endif // HAVE_OPENCV_CUDAFEATURES2D
}

auto FastDetectorCuda::detect(const cv::Mat& img, cv::InputArray& mask) -> std::vector<cv::KeyPoint>
{
    std::vector<cv::KeyPoint> keyPoints;

    try {

#ifdef HAVE_OPENCV_CUDAFEATURES2D
        cv::cuda::GpuMat g_img(img);
        cv::cuda::GpuMat g_mask(mask);
        mFast->detect(g_img, keyPoints, g_mask);
#else
        TL_COMPILER_WARNING("OpenCV not built with CUDAFEATURES2D. Cuda Fast Detector not supported")
        throw TL_ERROR("OpenCV not built with CUDAFEATURES2D. Cuda Fast Detector not supported");
#endif // HAVE_OPENCV_CUDAFEATURES2D

    } catch (...) {
        TL_THROW_EXCEPTION_WITH_NESTED("Catched exception");
    }

    return keyPoints;
}


} // namespace tl
