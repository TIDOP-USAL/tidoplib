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

/* BRIEF properties */

BriefProperties::BriefProperties()
  : Feature("BRIEF", Feature::Type::brief)
{
    reset();
}

BriefProperties::BriefProperties(const BriefProperties &properties)
  : Feature(properties)
{
}

BriefProperties::BriefProperties(BriefProperties &&properties) TL_NOEXCEPT
  : Feature(std::forward<Feature>(properties))
{
}

BriefProperties::~BriefProperties() = default;

auto BriefProperties::operator =(const BriefProperties &properties) -> BriefProperties&
{
    if (this != &properties) {
        Feature::operator=(properties);
    }

    return *this;
}

auto BriefProperties::operator =(BriefProperties &&properties) TL_NOEXCEPT -> BriefProperties&
{
    if (this != &properties) {
        Feature::operator=(std::forward<Feature>(properties));
    }

    return *this;
}

auto BriefProperties::bytes() const -> std::string
{
    return getProperty<std::string>("Bytes");
}

auto BriefProperties::useOrientation() const -> bool
{
    return getProperty<bool>("UseOrientation");
}

void BriefProperties::setBytes(const std::string &bytes)
{
    if (bytes == "16" ||
        bytes == "32" ||
        bytes == "64") {
        setProperty("Bytes", bytes);
    } else {
        Message::warning("'Bytes' value not valid: {}", bytes);
    }
}

void BriefProperties::setUseOrientation(bool useOrientation)
{
    setProperty("UseOrientation", useOrientation);
}

void BriefProperties::reset()
{
    setBytes(brief_default_value_bytes);
    setUseOrientation(brief_default_value_use_orientation);
}



/* BRIEF detector */

BriefDescriptor::BriefDescriptor()
  : mProperties()
{
    init();
}

BriefDescriptor::BriefDescriptor(const BriefProperties &properties)
  : mProperties(properties)
{
    init();
}

BriefDescriptor::BriefDescriptor(const BriefDescriptor &briefDescriptor)
  : mProperties(briefDescriptor.mProperties)
{
    init();
}

BriefDescriptor::BriefDescriptor(BriefDescriptor &&briefDescriptor) TL_NOEXCEPT
  : mProperties(std::move(briefDescriptor.mProperties)),
    mBrief(std::move(briefDescriptor.mBrief))
{
    init();
}

auto BriefDescriptor::operator =(const BriefDescriptor &briefDescriptor) -> BriefDescriptor&
{
    if (this != &briefDescriptor) {
        mProperties = briefDescriptor.mProperties;
        init();
    }

    return *this;
}

auto BriefDescriptor::operator =(BriefDescriptor &&briefDescriptor) TL_NOEXCEPT -> BriefDescriptor&
{
    if (this != &briefDescriptor) {
        mProperties = std::move(briefDescriptor.mProperties);
        mBrief = std::move(briefDescriptor.mBrief);
    }

    return *this;
}

BriefDescriptor::~BriefDescriptor() = default;

void BriefDescriptor::init()
{
#ifdef HAVE_OPENCV_XFEATURES2D 
    try {
        mBrief = cv::xfeatures2d::BriefDescriptorExtractor::create(std::stoi(mProperties.bytes()),
                                                                   mProperties.useOrientation());
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


} // namespace tl
