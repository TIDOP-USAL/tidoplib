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

#include "brief.h"

#include "tidop/core/base/exception.h"


namespace tl
{


BriefProperties::BriefProperties()
  : mBytes(brief_default_value_bytes)
{
}

BriefProperties::BriefProperties(const BriefProperties &briefProperties) = default;

BriefProperties::BriefProperties(BriefProperties &&briefProperties) TL_NOEXCEPT
  : Brief(std::forward<Brief>(briefProperties)),
    mBytes(std::exchange(briefProperties.mBytes, brief_default_value_bytes)),
    mUseOrientation(std::exchange(briefProperties.mUseOrientation, brief_default_value_use_orientation))
{
}

BriefProperties::~BriefProperties() = default;

auto BriefProperties::operator =(const BriefProperties &briefProperties) -> BriefProperties&
{
    if (this != &briefProperties) {
        mBytes = briefProperties.mBytes;
        mUseOrientation = briefProperties.mUseOrientation;
    }

    return *this;
}

auto BriefProperties::operator =(BriefProperties &&briefProperties) TL_NOEXCEPT -> BriefProperties&
{
    if (this != &briefProperties) {
        mBytes = std::exchange(briefProperties.mBytes, brief_default_value_bytes);
        mUseOrientation = std::exchange(briefProperties.mUseOrientation, brief_default_value_use_orientation);
    }

    return *this;
}

auto BriefProperties::bytes() const -> std::string
{
    return mBytes;
}

auto BriefProperties::useOrientation() const -> bool
{
    return mUseOrientation;
}

void BriefProperties::setBytes(const std::string &bytes)
{
    if (bytes == "16" ||
        bytes == "32" ||
        bytes == "64") {
        mBytes = bytes;
    }
}

void BriefProperties::setUseOrientation(bool useOrientation)
{
    mUseOrientation = useOrientation;
}

void BriefProperties::reset()
{
    mBytes = brief_default_value_bytes;
    mUseOrientation = brief_default_value_use_orientation;
}

auto BriefProperties::name() const -> std::string
{
    return std::string("BRIEF");
}


/*----------------------------------------------------------------*/


BriefDescriptor::BriefDescriptor()
{
    update();
}

BriefDescriptor::BriefDescriptor(const BriefDescriptor &briefDescriptor)
  : BriefProperties(briefDescriptor)
{
    update();
}

BriefDescriptor::BriefDescriptor(BriefDescriptor &&briefDescriptor) TL_NOEXCEPT
  : BriefProperties(std::forward<BriefProperties>(briefDescriptor))
{
    update();
}

auto BriefDescriptor::operator =(const BriefDescriptor &briefDescriptor) -> BriefDescriptor&
{
    if (this != &briefDescriptor) {
        BriefProperties::operator=(briefDescriptor);
        update();
    }

    return *this;
}

auto BriefDescriptor::operator =(BriefDescriptor &&briefDescriptor) TL_NOEXCEPT -> BriefDescriptor&
{
    if (this != &briefDescriptor) {
        BriefProperties::operator=(std::forward<BriefProperties>(briefDescriptor));
        update();
    }

    return *this;
}

BriefDescriptor::BriefDescriptor(const std::string &bytes, bool useOrientation)
{
    BriefProperties::setBytes(bytes);
    BriefProperties::setUseOrientation(useOrientation);
    update();
}

BriefDescriptor::~BriefDescriptor() = default;

void BriefDescriptor::update()
{
#ifdef HAVE_OPENCV_XFEATURES2D 
    try {
        mBrief = cv::xfeatures2d::BriefDescriptorExtractor::create(std::stoi(BriefProperties::bytes()),
                                                                   BriefProperties::useOrientation());
    } catch (...) {
        TL_THROW_EXCEPTION_WITH_NESTED("Catched exception");
    }
#endif // HAVE_OPENCV_XFEATURES2D
}

auto BriefDescriptor::extract(const cv::Mat &img, std::vector<cv::KeyPoint> &keyPoints) -> cv::Mat
{
    cv::Mat descriptors;

    try {

#ifdef HAVE_OPENCV_XFEATURES2D 
        mBrief->compute(img, keyPoints, descriptors);
#else
        TL_COMPILER_WARNING("OpenCV not built with extra modules. Brief Descriptor not supported")
            throw TL_ERROR("OpenCV not built with extra modules. Brief Descriptor not supported");
#endif // HAVE_OPENCV_XFEATURES2D

    } catch (...) {
        TL_THROW_EXCEPTION_WITH_NESTED("Catched exception");
    }

    return descriptors;
}

void BriefDescriptor::setBytes(const std::string &bytes)
{
    BriefProperties::setBytes(bytes);
    update();
}

void BriefDescriptor::setUseOrientation(bool useOrientation)
{
    BriefProperties::setUseOrientation(useOrientation);
    update();
}

void BriefDescriptor::reset()
{
    BriefProperties::reset();
    update();
}


} // namespace tl
