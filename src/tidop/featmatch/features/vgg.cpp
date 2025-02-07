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

#include "tidop/featmatch/features/vgg.h"

#include "tidop/core/base/exception.h"

namespace tl
{


/* VGG properties */

VggProperties::VggProperties()
  : Feature("VGG", Feature::Type::vgg)
{
    reset();
}

VggProperties::VggProperties(const VggProperties &properties)
  : Feature(properties)
{
}

VggProperties::VggProperties(VggProperties &&properties) TL_NOEXCEPT
  : Feature(std::forward<Feature>(properties))
{
}

auto VggProperties::operator=(const VggProperties &properties) -> VggProperties &
{
    if (this != &properties) {
        Feature::operator=(properties);
    }
    return *this;
}

auto VggProperties::operator=(VggProperties &&properties) TL_NOEXCEPT -> VggProperties &
{
    if (this != &properties) {
        Feature::operator=(std::forward<Feature>(properties));
    }
    return *this;
}

auto VggProperties::descriptorType() const -> std::string
{
    return getProperty<std::string>("DescriptorType");
}

void VggProperties::setDescriptorType(const std::string &descriptorType)
{
    if (descriptorType == "VGG_120" ||
        descriptorType == "VGG_80" ||
        descriptorType == "VGG_64" ||
        descriptorType == "VGG_48") {
        setProperty("DescriptorType", descriptorType);
    } else {
        Message::warning("'DescriptorType' value not valid: {}", descriptorType);
    }
}

auto VggProperties::scaleFactor() const -> float
{
    return getProperty<float>("ScaleFactor");
}

void VggProperties::setScaleFactor(float scaleFactor)
{
    setProperty("ScaleFactor", scaleFactor);
}

auto VggProperties::sigma() const -> float
{
    return getProperty<float>("Sigma");
}

void VggProperties::setSigma(float sigma)
{
    setProperty("Sigma", sigma);
}

auto VggProperties::useNormalizeDescriptor() const -> bool
{
    return getProperty<bool>("UseNormalizeDescriptor");
}

void VggProperties::setUseNormalizeDescriptor(bool useNormalizeDescriptor)
{
    setProperty("UseNormalizeDescriptor", useNormalizeDescriptor);
}

auto VggProperties::useNormalizeImage() const -> bool
{
    return getProperty<bool>("UseNormalizeImage");
}

void VggProperties::setUseNormalizeImage(bool useNormalizeImage)
{
    setProperty("UseNormalizeImage", useNormalizeImage);
}

auto VggProperties::useScaleOrientation() const -> bool
{
    return getProperty<bool>("UseScaleOrientation");
}

void VggProperties::setUseScaleOrientation(bool useScaleOrientation)
{
    setProperty("UseScaleOrientation", useScaleOrientation);
}

void VggProperties::reset()
{
    setDescriptorType(vgg_default_value_descriptor_type);
    setScaleFactor(vgg_default_value_scale_factor);
    setSigma(vgg_default_value_sigma);
    setUseNormalizeDescriptor(vgg_default_value_use_normalize_descriptor);
    setUseNormalizeImage(vgg_default_value_use_normalize_image);
    setUseScaleOrientation(vgg_default_value_useScale_orientation);
}





/* VGG descriptor */

VggDescriptor::VggDescriptor()
  : mProperties()
{
    init();
}

VggDescriptor::VggDescriptor(const VggProperties &properties)
  : mProperties(properties)
{
    init();
}

VggDescriptor::VggDescriptor(const VggDescriptor &vgg)
  : mProperties(vgg.mProperties)
{
    init();
}

VggDescriptor::VggDescriptor(VggDescriptor &&vgg) TL_NOEXCEPT
  : mProperties(std::move(vgg.mProperties))
#ifdef HAVE_OPENCV_XFEATURES2D 
#if CV_VERSION_MAJOR >= 4 || (CV_VERSION_MAJOR >= 3 && CV_VERSION_MINOR > 2)
    , mVGG(std::move(vgg.mVGG))
#endif
#endif // HAVE_OPENCV_XFEATURES2D
{
}

auto VggDescriptor::operator =(const VggDescriptor &vgg) -> VggDescriptor &
{
    if (this != &vgg) {
        mProperties = vgg.mProperties;
        init();
    }
    return *this;
}

auto VggDescriptor::operator =(VggDescriptor &&vgg) TL_NOEXCEPT -> VggDescriptor &
{
    if (this != &vgg) {
        mProperties = std::move(vgg.mProperties);
#ifdef HAVE_OPENCV_XFEATURES2D 
#if CV_VERSION_MAJOR >= 4 || (CV_VERSION_MAJOR >= 3 && CV_VERSION_MINOR > 2)
        mVGG = std::move(vgg.mVGG);
#endif
#endif // HAVE_OPENCV_XFEATURES2D
    }
    return *this;
}

void VggDescriptor::init()
{
#ifdef HAVE_OPENCV_XFEATURES2D 
#if CV_VERSION_MAJOR >= 4 || (CV_VERSION_MAJOR >= 3 && CV_VERSION_MINOR > 2)
    int descriptor_type = cv::xfeatures2d::VGG::VGG_120;
    std::string descriptorType = mProperties.descriptorType();
    if (descriptorType == "VGG_120") {
        descriptor_type = cv::xfeatures2d::VGG::VGG_120;
    } else if (descriptorType == "VGG_80") {
        descriptor_type = cv::xfeatures2d::VGG::VGG_80;
    } else if (descriptorType == "VGG_64") {
        descriptor_type = cv::xfeatures2d::VGG::VGG_64;
    } else if (descriptorType == "VGG_48") {
        descriptor_type = cv::xfeatures2d::VGG::VGG_48;
    }

    mVGG = cv::xfeatures2d::VGG::create(descriptor_type,
                                        mProperties.sigma(),
                                        mProperties.useNormalizeImage(),
                                        mProperties.useScaleOrientation(),
                                        mProperties.scaleFactor(),
                                        mProperties.useNormalizeDescriptor());
#endif
#endif // HAVE_OPENCV_XFEATURES2D
}

auto VggDescriptor::extract(const cv::Mat &img, std::vector<cv::KeyPoint> &keyPoints) -> cv::Mat
{
    cv::Mat descriptors;

    try {

#ifdef HAVE_OPENCV_XFEATURES2D 
#  if CV_VERSION_MAJOR >= 4 || (CV_VERSION_MAJOR == 3 && CV_VERSION_MINOR > 2)
        mVGG->compute(img, keyPoints, descriptors);
#  else
        TL_COMPILER_WARNING("VGG Descriptor not supported in OpenCV versions < 3.3 ")
        throw TL_ERROR("VGG Descriptor not supported in OpenCV versions < 3.3");
#  endif
#else
        TL_COMPILER_WARNING("OpenCV not built with extra modules. VGG Descriptor not supported")
        throw TL_ERROR("OpenCV not built with extra modules. VGG Descriptor not supported");
#endif // HAVE_OPENCV_XFEATURES2D

    } catch (...) {
        TL_THROW_EXCEPTION_WITH_NESTED("Catched exception");
    }

    return descriptors;
}

} // namespace tl


