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

#include "freak.h"

#include "tidop/core/base/exception.h"

namespace tl
{


/* FREAK properties */

FreakProperties::FreakProperties()
  : Feature("FREAK", Feature::Type::freak)
{
    reset();
}

FreakProperties::FreakProperties(const FreakProperties &properties)
  : Feature(properties)
{
}

FreakProperties::FreakProperties(FreakProperties &&properties) TL_NOEXCEPT
  : Feature(std::forward<Feature>(properties))
{
}

auto FreakProperties::operator=(const FreakProperties &properties) -> FreakProperties &
{
    if (this != &properties) {
        Feature::operator=(properties);
    }
    return *this;
}

auto FreakProperties::operator=(FreakProperties &&properties) TL_NOEXCEPT -> FreakProperties &
{
    if (this != &properties) {
        Feature::operator=(std::forward<Feature>(properties));
    }
    return *this;
}

auto FreakProperties::orientationNormalized() const -> bool
{
    return getProperty<bool>("OrientationNormalized");
}

auto FreakProperties::scaleNormalized() const -> bool
{
    return getProperty<bool>("ScaleNormalized");
}

auto FreakProperties::patternScale() const -> float
{
    return getProperty<float>("PatternScale");
}

auto FreakProperties::octaves() const -> int
{
    return getProperty<int>("Octaves");
}

void FreakProperties::setOrientationNormalized(bool orientationNormalized)
{
    setProperty("OrientationNormalized", orientationNormalized);
}

void FreakProperties::setScaleNormalized(bool scaleNormalized)
{
    setProperty("ScaleNormalized", scaleNormalized);
}

void FreakProperties::setPatternScale(float patternScale)
{
    setProperty("PatternScale", patternScale);
}

void FreakProperties::setOctaves(int octaves)
{
    setProperty("Octaves", octaves);
}

void FreakProperties::reset()
{
    setOrientationNormalized(true);
    setScaleNormalized(true);
    setPatternScale(22.f);
    setOctaves(4);
}



/* FREAK descriptor */

FreakDescriptor::FreakDescriptor()
  : mProperties()
{
    init();
}

FreakDescriptor::FreakDescriptor(const FreakProperties &properties)
  : mProperties(properties)
{
    init();
}

FreakDescriptor::FreakDescriptor(const FreakDescriptor &freak)
  : mProperties(freak.mProperties)
{
    init();
}

FreakDescriptor::FreakDescriptor(FreakDescriptor &&freak) TL_NOEXCEPT
  : mProperties(std::move(freak.mProperties))
#ifdef HAVE_OPENCV_XFEATURES2D 
    , mFREAK(std::move(freak.mFREAK))
#endif // HAVE_OPENCV_XFEATURES2D
{
}

auto FreakDescriptor::operator =(const FreakDescriptor &freak) -> FreakDescriptor &
{
    if (this != &freak) {
        mProperties = freak.mProperties;
        init();
    }
    return *this;
}

auto FreakDescriptor::operator =(FreakDescriptor &&freak) TL_NOEXCEPT -> FreakDescriptor &
{
    if (this != &freak) {
        mProperties = std::move(freak.mProperties);
#ifdef HAVE_OPENCV_XFEATURES2D 
        mFREAK = std::move(freak.mFREAK);
#endif // HAVE_OPENCV_XFEATURES2D
    }
    return *this;
}

void FreakDescriptor::init()
{
#ifdef HAVE_OPENCV_XFEATURES2D 
    mFREAK = cv::xfeatures2d::FREAK::create(mProperties.orientationNormalized(),
                                            mProperties.scaleNormalized(),
                                            mProperties.patternScale(),
                                            mProperties.octaves());
#endif // HAVE_OPENCV_XFEATURES2D
}

auto FreakDescriptor::extract(const cv::Mat &img, std::vector<cv::KeyPoint> &keyPoints) -> cv::Mat
{
    cv::Mat descriptors;

    try {

#ifdef HAVE_OPENCV_XFEATURES2D 
        mFREAK->compute(img, keyPoints, descriptors);
#else
        TL_COMPILER_WARNING("OpenCV not built with extra modules. Freak Descriptor not supported")
        throw TL_ERROR("OpenCV not built with extra modules. Freak Descriptor not supported");
#endif // HAVE_OPENCV_XFEATURES2D

    } catch (...) {
        TL_THROW_EXCEPTION_WITH_NESTED("Catched exception");
    }

    return descriptors;
}


} // namespace tl
