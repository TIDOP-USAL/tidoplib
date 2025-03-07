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

#include "boost.h"

#include "tidop/core/base/exception.h"

namespace tl
{

BoostProperties::BoostProperties()
  : Feature("BOOST", Feature::Type::boost)
{
    reset();
}

BoostProperties::BoostProperties(const BoostProperties &properties)
  : Feature(properties)
{
}

BoostProperties::BoostProperties(BoostProperties &&properties) TL_NOEXCEPT
  : Feature(std::forward<Feature>(properties))
{
}

BoostProperties::~BoostProperties() = default;

auto BoostProperties::operator =(const BoostProperties &properties) -> BoostProperties&
{
    if (this != &properties) {
        Feature::operator=(properties);
    }

    return *this;
}

auto BoostProperties::operator =(BoostProperties &&properties) TL_NOEXCEPT -> BoostProperties&
{
    if (this != &properties) {
        Feature::operator=(std::forward<Feature>(properties));
    }

    return *this;
}

auto BoostProperties::descriptorType() const -> std::string
{
    return getProperty<std::string>("DescriptorType");
}

auto BoostProperties::useOrientation() const -> bool
{
    return getProperty<bool>("UseOrientation");
}

auto BoostProperties::scaleFactor() const -> float
{
    return getProperty<float>("ScaleFactor");
}

void BoostProperties::setDescriptorType(const std::string &descriptorType)
{
    if (descriptorType == "BGM" ||
        descriptorType == "BGM_HARD" ||
        descriptorType == "BGM_BILINEAR" ||
        descriptorType == "LBGM" ||
        descriptorType == "BINBOOST_64" ||
        descriptorType == "BINBOOST_128" ||
        descriptorType == "BINBOOST_256") {
        setProperty("DescriptorType", descriptorType);
    } else {
        Message::warning("'DescriptorType' value not valid: {}", descriptorType);
    }
}

void BoostProperties::setUseOrientation(bool useOrientation)
{
    setProperty("UseOrientation", useOrientation);
}

void BoostProperties::setScaleFactor(float scaleFactor)
{
    setProperty("ScaleFactor" , scaleFactor);
}

void BoostProperties::reset()
{
    setDescriptorType(boost_default_value_descriptor_type);
    setUseOrientation(boost_default_value_use_orientation);
    setScaleFactor(boost_default_value_scale_factor);
}



/*----------------------------------------------------------------*/


BoostDescriptor::BoostDescriptor()
  : mProperties()
{
    update();
}

BoostDescriptor::BoostDescriptor(const BoostProperties &properties)
  : mProperties(properties)
{
    update();
}

BoostDescriptor::BoostDescriptor(const BoostDescriptor &boostDescriptor)
  : mProperties(boostDescriptor.mProperties)
{
    update();
}

BoostDescriptor::BoostDescriptor(BoostDescriptor &&boostDescriptor) TL_NOEXCEPT
  : mProperties(std::move(boostDescriptor.mProperties))
{
    update();
}

auto BoostDescriptor::operator =(const BoostDescriptor &boostDescriptor) -> BoostDescriptor&
{
    if (this != &boostDescriptor) {
        mProperties = boostDescriptor.mProperties;
        update();
    }

    return *this;
}

auto BoostDescriptor::operator =(BoostDescriptor &&boostDescriptor) TL_NOEXCEPT -> BoostDescriptor&
{
    if (this != &boostDescriptor) {
        mProperties = std::move(boostDescriptor.mProperties);
        update();
    }

    return *this;
}

void BoostDescriptor::update()
{
    try {

#ifdef HAVE_OPENCV_XFEATURES2D 

#  if CV_VERSION_MAJOR >= 4 || (CV_VERSION_MAJOR == 3 && CV_VERSION_MINOR > 2)

        int descriptor_type = cv::xfeatures2d::BoostDesc::BGM;
        std::string descriptorType = mProperties.descriptorType();
        if (descriptorType == "BGM") {
            descriptor_type = cv::xfeatures2d::BoostDesc::BGM;
        } else if (descriptorType == "BGM_HARD") {
            descriptor_type = cv::xfeatures2d::BoostDesc::BGM_HARD;
        } else if (descriptorType == "BGM_BILINEAR") {
            descriptor_type = cv::xfeatures2d::BoostDesc::BGM_BILINEAR;
        } else if (descriptorType == "LBGM") {
            descriptor_type = cv::xfeatures2d::BoostDesc::LBGM;
        } else if (descriptorType == "BINBOOST_64") {
            descriptor_type = cv::xfeatures2d::BoostDesc::BINBOOST_64;
        } else if (descriptorType == "BINBOOST_128") {
            descriptor_type = cv::xfeatures2d::BoostDesc::BINBOOST_128;
        } else if (descriptorType == "BINBOOST_256") {
            descriptor_type = cv::xfeatures2d::BoostDesc::BINBOOST_256;
        }

        mBoost = cv::xfeatures2d::BoostDesc::create(descriptor_type,
                                                    mProperties.useOrientation(),
                                                    mProperties.scaleFactor());

#  else

        TL_COMPILER_WARNING("Boost Descriptor not supported in OpenCV versions < 3.3")
            throw TL_ERROR("Boost Descriptor not supported in OpenCV versions < 3.3");

#  endif

#else

        TL_COMPILER_WARNING("OpenCV not built with extra modules. Boost Descriptor not supported")
            throw TL_ERROR("OpenCV not built with extra modules. Boost Descriptor not supported");

#endif // HAVE_OPENCV_XFEATURES2D

    } catch (...) {
        TL_THROW_EXCEPTION_WITH_NESTED("Catched exception");
    }
}

auto BoostDescriptor::extract(const cv::Mat &img, std::vector<cv::KeyPoint> &keyPoints) -> cv::Mat
{
    cv::Mat descriptors;

    try {

#ifdef HAVE_OPENCV_XFEATURES2D 

#  if CV_VERSION_MAJOR >= 4 || (CV_VERSION_MAJOR == 3 && CV_VERSION_MINOR > 2)
        mBoost->compute(img, keyPoints, descriptors);
#  else
        TL_COMPILER_WARNING("Boost Descriptor not supported in OpenCV versions < 3.3")
        throw TL_ERROR("Boost Descriptor not supported in OpenCV versions < 3.3");
#  endif

#else
        TL_COMPILER_WARNING("OpenCV not built with extra modules. Boost Descriptor not supported")
        throw TL_ERROR("OpenCV not built with extra modules. Boost Descriptor not supported");
#endif // HAVE_OPENCV_XFEATURES2D

    } catch (...) {
        TL_THROW_EXCEPTION_WITH_NESTED("Catched exception");
    }

    return descriptors;
}


} // namespace tl


