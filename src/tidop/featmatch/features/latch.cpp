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

#include "tidop/core/base/exception.h"


namespace tl
{

/* LATCH properties */

LatchProperties::LatchProperties()
  : Feature("LATCH", Feature::Type::latch)
{
    reset();
}

LatchProperties::LatchProperties(const LatchProperties &properties)
  : Feature(properties)
{
}

LatchProperties::LatchProperties(LatchProperties &&properties) TL_NOEXCEPT
  : Feature(std::forward<Feature>(properties))
{
}

LatchProperties::~LatchProperties() = default;

auto LatchProperties::operator =(const LatchProperties &properties) -> LatchProperties &
{
    if (this != &properties) {
        Feature::operator=(properties);
    }

    return *this;
}

auto LatchProperties::operator =(LatchProperties &&properties) TL_NOEXCEPT -> LatchProperties &
{
    if (this != &properties) {
        Feature::operator=(std::forward<Feature>(properties));
    }

    return *this;
}

auto LatchProperties::bytes() const -> std::string
{
    return getProperty<std::string>("Bytes");
}

auto LatchProperties::rotationInvariance() const -> bool
{
    return getProperty<bool>("RotationInvariance");
}

auto LatchProperties::halfSsdSize() const -> int
{
    return getProperty<int>("HalfSsdSize");
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
        setProperty("Bytes", bytes);
    } else {
        Message::warning("'Bytes' value not valid: {}", bytes);
    }
}

void LatchProperties::setRotationInvariance(bool rotationInvariance)
{
    setProperty("RotationInvariance", rotationInvariance);
}

void LatchProperties::setHalfSsdSize(int halfSsdSize)
{
    setProperty("HalfSsdSize", halfSsdSize);
}

void LatchProperties::reset()
{
    setBytes("32");
    setRotationInvariance(true);
    setHalfSsdSize(3);
}



/* LATCH Descriptor */

LatchDescriptor::LatchDescriptor()
    : mProperties()
{
    init();
}

LatchDescriptor::LatchDescriptor(const LatchProperties &properties)
    : mProperties(properties)
{
    init();
}

LatchDescriptor::LatchDescriptor(const LatchDescriptor &briefDescriptor)
    : mProperties(briefDescriptor.mProperties)
{
    init();
}

LatchDescriptor::LatchDescriptor(LatchDescriptor &&briefDescriptor) TL_NOEXCEPT
  : mProperties(std::move(briefDescriptor.mProperties))
#ifdef HAVE_OPENCV_XFEATURES2D 
    ,
    mLATCH(std::move(briefDescriptor.mLATCH))
#endif // HAVE_OPENCV_XFEATURES2D
{
    init();
}

auto LatchDescriptor::operator =(const LatchDescriptor &briefDescriptor) -> LatchDescriptor &
{
    if (this != &briefDescriptor) {
        mProperties = briefDescriptor.mProperties;
        init();
    }

    return *this;
}

auto LatchDescriptor::operator =(LatchDescriptor &&briefDescriptor) TL_NOEXCEPT -> LatchDescriptor &
{
    if (this != &briefDescriptor) {
        mProperties = std::move(briefDescriptor.mProperties);
#ifdef HAVE_OPENCV_XFEATURES2D 
        mLATCH = std::move(briefDescriptor.mLATCH);
#endif // HAVE_OPENCV_XFEATURES2D
    }

    return *this;
}

LatchDescriptor::~LatchDescriptor() = default;

void LatchDescriptor::init()
{
#ifdef HAVE_OPENCV_XFEATURES2D 
    mLATCH = cv::xfeatures2d::LATCH::create(std::stoi(mProperties.bytes()),
                                                      mProperties.rotationInvariance(),
                                                      mProperties.halfSsdSize());
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


} // namespace tl
