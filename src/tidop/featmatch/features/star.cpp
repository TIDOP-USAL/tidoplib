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

#include "tidop/featmatch/features/star.h"

#include "tidop/core/base/exception.h"


namespace tl
{


/* STAR properties */

StarProperties::StarProperties()
  : Feature("STAR", Feature::Type::star)
{
    reset();
}

StarProperties::StarProperties(const StarProperties &properties)
  : Feature(properties)
{
}

StarProperties::StarProperties(StarProperties &&properties) TL_NOEXCEPT
  : Feature(std::forward<Feature>(properties))
{
}

auto StarProperties::operator=(const StarProperties &properties) -> StarProperties &
{
    if (this != &properties) {
        Feature::operator=(properties);
    }
    return *this;
}

auto StarProperties::operator=(StarProperties &&properties) TL_NOEXCEPT -> StarProperties &
{
    if (this != &properties) {
        Feature::operator=(std::forward<Feature>(properties));
    }
    return *this;
}

auto StarProperties::maxSize() const -> int
{
    return getProperty<int>("MaxSize");
}

auto StarProperties::responseThreshold() const -> int
{
    return getProperty<int>("ResponseThreshold");
}

auto StarProperties::lineThresholdProjected() const -> int
{
    return getProperty<int>("LineThresholdProjected");
}

auto StarProperties::lineThresholdBinarized() const -> int
{
    return getProperty<int>("LineThresholdBinarized");
}

auto StarProperties::suppressNonmaxSize() const -> int
{
    return getProperty<int>("SuppressNonmaxSize");
}

void StarProperties::setMaxSize(int maxSize)
{
    setProperty("MaxSize", maxSize);
}

void StarProperties::setResponseThreshold(int responseThreshold)
{
    setProperty("ResponseThreshold", responseThreshold);
}

void StarProperties::setLineThresholdProjected(int lineThresholdProjected)
{
    setProperty("LineThresholdProjected", lineThresholdProjected);
}

void StarProperties::setLineThresholdBinarized(int lineThresholdBinarized)
{
    setProperty("LineThresholdBinarized", lineThresholdBinarized);
}

void StarProperties::setSuppressNonmaxSize(int suppressNonmaxSize)
{
    setProperty("SuppressNonmaxSize", suppressNonmaxSize);
}

void StarProperties::reset()
{
    setMaxSize(star_default_value_max_size);
    setResponseThreshold(star_default_value_response_threshold);
    setLineThresholdProjected(star_default_value_line_threshold_projected);
    setLineThresholdBinarized(star_default_value_line_threshold_binarized);
    setSuppressNonmaxSize(star_default_value_suppress_nonmax_size);
}





/* STAR detector */

StarDetector::StarDetector()
  : mProperties()
{
    init();
}

StarDetector::StarDetector(const StarProperties &properties)
  : mProperties(properties)
{
    init();
}

StarDetector::StarDetector(const StarDetector &star)
    : mProperties(star.mProperties)
{
    init();
}

StarDetector::StarDetector(StarDetector &&star) TL_NOEXCEPT
    : mProperties(std::move(star.mProperties))
#ifdef HAVE_OPENCV_XFEATURES2D 
    , mSTAR(std::move(star.mSTAR))
#endif // HAVE_OPENCV_XFEATURES2D
{
}

auto StarDetector::operator =(const StarDetector &star) -> StarDetector &
{
    if (this != &star) {
        mProperties = star.mProperties;
        init();
    }
    return *this;
}

auto StarDetector::operator =(StarDetector &&star) TL_NOEXCEPT -> StarDetector &
{
    if (this != &star) {
        mProperties = std::move(star.mProperties);
#ifdef HAVE_OPENCV_XFEATURES2D 
        mSTAR = std::move(star.mSTAR);
#endif // HAVE_OPENCV_XFEATURES2D
    }
    return *this;
}

void StarDetector::init()
{
#ifdef HAVE_OPENCV_XFEATURES2D
    mSTAR = cv::xfeatures2d::StarDetector::create(mProperties.maxSize(),
                                                  mProperties.responseThreshold(),
                                                  mProperties.lineThresholdProjected(),
                                                  mProperties.lineThresholdBinarized(),
                                                  mProperties.suppressNonmaxSize());
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


} // namespace tl
