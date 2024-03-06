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

#include "star.h"

#include "tidop/core/exception.h"


namespace tl
{


StarProperties::StarProperties() = default;

StarProperties::StarProperties(const StarProperties &starProperties)
  : Star(starProperties),
    mMaxSize(starProperties.mMaxSize),
    mResponseThreshold(starProperties.mResponseThreshold),
    mLineThresholdProjected(starProperties.mLineThresholdProjected),
    mLineThresholdBinarized(starProperties.mLineThresholdBinarized),
    mSuppressNonmaxSize(starProperties.mSuppressNonmaxSize)
{
}

auto StarProperties::maxSize() const -> int
{
    return mMaxSize;
}

auto StarProperties::responseThreshold() const -> int
{
    return mResponseThreshold;
}

auto StarProperties::lineThresholdProjected() const -> int
{
    return mLineThresholdProjected;
}

auto StarProperties::lineThresholdBinarized() const -> int
{
    return mLineThresholdBinarized;
}

auto StarProperties::suppressNonmaxSize() const -> int
{
    return mSuppressNonmaxSize;
}

void StarProperties::setMaxSize(int maxSize)
{
    mMaxSize = maxSize;
}

void StarProperties::setResponseThreshold(int responseThreshold)
{
    mResponseThreshold = responseThreshold;
}

void StarProperties::setLineThresholdProjected(int lineThresholdProjected)
{
    mLineThresholdProjected = lineThresholdProjected;
}

void StarProperties::setLineThresholdBinarized(int lineThresholdBinarized)
{
    mLineThresholdBinarized = lineThresholdBinarized;
}

void StarProperties::setSuppressNonmaxSize(int suppressNonmaxSize)
{
    mSuppressNonmaxSize = suppressNonmaxSize;
}

void StarProperties::reset()
{
    mMaxSize = star_default_value_max_size;
    mResponseThreshold = star_default_value_response_threshold;
    mLineThresholdProjected = star_default_value_line_threshold_projected;
    mLineThresholdBinarized = star_default_value_line_threshold_binarized;
    mSuppressNonmaxSize = star_default_value_suppress_nonmax_size;
}

auto StarProperties::name() const -> std::string
{
    return std::string("STAR");
}


/*----------------------------------------------------------------*/


StarDetector::StarDetector()
{
    update();
}

StarDetector::StarDetector(const StarDetector &starDetector)
    : StarProperties(starDetector)
{
    update();
}

StarDetector::StarDetector(int maxSize,
                           int responseThreshold,
                           int lineThresholdProjected,
                           int lineThresholdBinarized,
                           int suppressNonmaxSize)
{
    StarProperties::setMaxSize(maxSize);
    StarProperties::setResponseThreshold(responseThreshold);
    StarProperties::setLineThresholdProjected(lineThresholdProjected);
    StarProperties::setLineThresholdBinarized(lineThresholdBinarized);
    StarProperties::setSuppressNonmaxSize(suppressNonmaxSize);
    update();
}

void StarDetector::update()
{
#ifdef HAVE_OPENCV_XFEATURES2D
    mSTAR = cv::xfeatures2d::StarDetector::create(StarProperties::maxSize(),
                                                  StarProperties::responseThreshold(),
                                                  StarProperties::lineThresholdProjected(),
                                                  StarProperties::lineThresholdBinarized(),
                                                  StarProperties::suppressNonmaxSize());
#endif // HAVE_OPENCV_XFEATURES2D
}

auto StarDetector::detect(const cv::Mat &img, cv::InputArray &mask) -> std::vector<cv::KeyPoint>
{
    std::vector<cv::KeyPoint> keyPoints;

    try {

#ifdef HAVE_OPENCV_XFEATURES2D 
        mSTAR->detect(img, keyPoints, mask);
#else
        TL_COMPILER_WARNING("OpenCV not built with extra modules. Star Detector not supported")
        throw TL_ERROR("OpenCV not built with extra modules. Star Detector not supported");
#endif // HAVE_OPENCV_XFEATURES2D

    } catch (...) {
        TL_THROW_EXCEPTION_WITH_NESTED("Catched exception");
    }

    return keyPoints;
}

void StarDetector::setMaxSize(int maxSize)
{
    StarProperties::setMaxSize(maxSize);
    update();
}

void StarDetector::setResponseThreshold(int responseThreshold)
{
    StarProperties::setResponseThreshold(responseThreshold);
    update();
}

void StarDetector::setLineThresholdProjected(int lineThresholdProjected)
{
    StarProperties::setLineThresholdProjected(lineThresholdProjected);
    update();
}

void StarDetector::setLineThresholdBinarized(int lineThresholdBinarized)
{
    StarProperties::setLineThresholdBinarized(lineThresholdBinarized);
    update();
}

void StarDetector::setSuppressNonmaxSize(int suppressNonmaxSize)
{
    StarProperties::setSuppressNonmaxSize(suppressNonmaxSize);
    update();
}

void StarDetector::reset()
{
    StarProperties::reset();
    update();
}


} // namespace tl
