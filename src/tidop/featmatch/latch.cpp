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

#include "latch.h"

#include "tidop/core/exception.h"


namespace tl
{

LatchProperties::LatchProperties()
  : mBytes("32")
{
}

LatchProperties::LatchProperties(const LatchProperties &latchProperties)
  : Latch(latchProperties),
    mBytes(latchProperties.mBytes),
    mRotationInvariance(latchProperties.mRotationInvariance),
    mHalfSsdSize(latchProperties.mHalfSsdSize)
{
}

auto LatchProperties::bytes() const -> std::string
{
    return mBytes;
}

auto LatchProperties::rotationInvariance() const -> bool
{
    return mRotationInvariance;
}

auto LatchProperties::halfSsdSize() const -> int
{
    return mHalfSsdSize;
}

void LatchProperties::setBytes(const std::string &bytes)
{
    if (bytes == "1" ||
        bytes == "2" ||
        bytes == "4" ||
        bytes == "8" ||
        bytes == "16" ||
        bytes == "32" ||
        bytes == "64") {
        mBytes = bytes;
    }
}

void LatchProperties::setRotationInvariance(bool rotationInvariance)
{
    mRotationInvariance = rotationInvariance;
}

void LatchProperties::setHalfSsdSize(int halfSsdSize)
{
    mHalfSsdSize = halfSsdSize;
}

void LatchProperties::reset()
{
    mBytes = "32";
    mRotationInvariance = true;
    mHalfSsdSize = 3;
}

auto LatchProperties::name() const -> std::string
{
    return std::string("LATCH");
}


/*----------------------------------------------------------------*/


LatchDescriptor::LatchDescriptor()
{
    update();
}

LatchDescriptor::LatchDescriptor(const LatchDescriptor &latchDescriptor)
  : LatchProperties(latchDescriptor),
    DescriptorExtractor(latchDescriptor)
{
    update();
}

LatchDescriptor::LatchDescriptor(const std::string &bytes,
                                 bool rotationInvariance,
                                 int halfSsdSize)
  : LatchProperties()
{
    LatchProperties::setBytes(bytes);
    LatchProperties::setRotationInvariance(rotationInvariance);
    LatchProperties::setHalfSsdSize(halfSsdSize);
    update();
}

void LatchDescriptor::update()
{
#ifdef HAVE_OPENCV_XFEATURES2D 
    mLATCH = cv::xfeatures2d::LATCH::create(std::stoi(LatchProperties::bytes()),
                                            LatchProperties::rotationInvariance(),
                                            LatchProperties::halfSsdSize());
#endif // HAVE_OPENCV_XFEATURES2D
}

auto LatchDescriptor::extract(const cv::Mat &img, std::vector<cv::KeyPoint> &keyPoints) -> cv::Mat
{
    cv::Mat descriptors;

    try {

#ifdef HAVE_OPENCV_XFEATURES2D 
        mLATCH->compute(img, keyPoints, descriptors);
#else
        TL_COMPILER_WARNING("OpenCV not built with extra modules. Latch Descriptor not supported")
        throw TL_ERROR("OpenCV not built with extra modules. Latch Descriptor not supported");
#endif // HAVE_OPENCV_XFEATURES2D

    } catch (...) {
        TL_THROW_EXCEPTION_WITH_NESTED("Catched exception");
    }

    return descriptors;
}

void LatchDescriptor::setBytes(const std::string &bytes)
{
    LatchProperties::setBytes(bytes);
    update();
}

void LatchDescriptor::setRotationInvariance(bool rotationInvariance)
{
    LatchProperties::setRotationInvariance(rotationInvariance);
    update();
}

void LatchDescriptor::setHalfSsdSize(int halfSsdSize)
{
    LatchProperties::setHalfSsdSize(halfSsdSize);
    update();
}

void LatchDescriptor::reset()
{
    LatchProperties::reset();
    update();
}


} // namespace tl
