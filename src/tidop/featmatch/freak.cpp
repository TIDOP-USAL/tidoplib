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

#include "tidop/core/exception.h"

namespace tl
{


FreakProperties::FreakProperties()
{
}

FreakProperties::FreakProperties(const FreakProperties &freakProperties)
  : Freak(freakProperties),
    mOrientationNormalized(freakProperties.mOrientationNormalized),
    mScaleNormalized(freakProperties.mScaleNormalized),
    mPatternScale(freakProperties.mPatternScale),
    mOctaves(freakProperties.mOctaves)
{
}

bool FreakProperties::orientationNormalized() const
{
    return mOrientationNormalized;
}

bool FreakProperties::scaleNormalized() const
{
    return mScaleNormalized;
}

double FreakProperties::patternScale() const
{
    return mPatternScale;
}

int FreakProperties::octaves() const
{
    return mOctaves;
}

void FreakProperties::setOrientationNormalized(bool orientationNormalized)
{
    mOrientationNormalized = orientationNormalized;
}

void FreakProperties::setScaleNormalized(bool scaleNormalized)
{
    mScaleNormalized = scaleNormalized;
}

void FreakProperties::setPatternScale(double patternScale)
{
    mPatternScale = patternScale;
}

void FreakProperties::setOctaves(int octaves)
{
    mOctaves = octaves;
}

void FreakProperties::reset()
{
    mOrientationNormalized = true;
    mScaleNormalized = true;
    mPatternScale = 22.;
    mOctaves = 4;
}

std::string FreakProperties::name() const
{
    return std::string("FREAK");
}


/*----------------------------------------------------------------*/


FreakDescriptor::FreakDescriptor()
{
    update();
}

FreakDescriptor::FreakDescriptor(const FreakDescriptor &freakDescriptor)
  : FreakProperties(freakDescriptor),
    DescriptorExtractor(freakDescriptor)
{
    update();
}

FreakDescriptor::FreakDescriptor(bool orientationNormalized,
                                 bool scaleNormalized,
                                 double patternScale,
                                 int octaves)
{
    FreakProperties::setOrientationNormalized(orientationNormalized);
    FreakProperties::setScaleNormalized(scaleNormalized);
    FreakProperties::setPatternScale(patternScale);
    FreakProperties::setOctaves(octaves);
    update();
}

void FreakDescriptor::update()
{
#ifdef HAVE_OPENCV_XFEATURES2D 
    mFREAK = cv::xfeatures2d::FREAK::create(FreakProperties::orientationNormalized(),
                                            FreakProperties::scaleNormalized(),
                                            static_cast<float>(FreakProperties::patternScale()),
                                            FreakProperties::octaves());
#endif // HAVE_OPENCV_XFEATURES2D
}

cv::Mat FreakDescriptor::extract(const cv::Mat &img,
                                 std::vector<cv::KeyPoint> &keyPoints)
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

void FreakDescriptor::setOrientationNormalized(bool orientationNormalized)
{
    FreakProperties::setOrientationNormalized(orientationNormalized);
    update();
}

void FreakDescriptor::setScaleNormalized(bool scaleNormalized)
{
    FreakProperties::setScaleNormalized(scaleNormalized);
    update();
}

void FreakDescriptor::setPatternScale(double patternScale)
{
    FreakProperties::setPatternScale(patternScale);
    update();
}

void FreakDescriptor::setOctaves(int octaves)
{
    FreakProperties::setOctaves(octaves);
    update();
}

void FreakDescriptor::reset()
{
    FreakProperties::reset();
    update();
}


} // namespace tl
