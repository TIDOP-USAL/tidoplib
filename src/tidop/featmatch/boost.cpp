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

#include <tidop/core/messages.h>

namespace tl
{

BoostProperties::BoostProperties()
  : mDescriptorType("BINBOOST_256")
{
}



BoostProperties::BoostProperties(const BoostProperties &boostProperties) = default;

BoostProperties::BoostProperties(BoostProperties &&boostProperties) TL_NOEXCEPT
  : mDescriptorType(std::exchange(boostProperties.mDescriptorType,"")),
    bUseOrientation(boostProperties.bUseOrientation),
    mScaleFactor(boostProperties.mScaleFactor)
{

}

BoostProperties::~BoostProperties() = default;

BoostProperties &BoostProperties::operator =(const BoostProperties &boostProperties)
{
  if (this != &boostProperties) {
    mDescriptorType = boostProperties.mDescriptorType;
    bUseOrientation = boostProperties.bUseOrientation;
    mScaleFactor = boostProperties.mScaleFactor;
  }
  return *this;
}

BoostProperties &BoostProperties::operator =(BoostProperties &&boostProperties) TL_NOEXCEPT
{
  if (this != &boostProperties) {
    mDescriptorType = std::exchange(boostProperties.mDescriptorType,"");
    bUseOrientation = std::exchange(boostProperties.bUseOrientation, boost_default_value_use_orientation);
    mScaleFactor = std::exchange(boostProperties.mScaleFactor, boost_default_value_scale_factor);
  }
  return *this;
}

std::string BoostProperties::descriptorType() const
{
  return mDescriptorType;
}

bool BoostProperties::useOrientation() const
{
  return bUseOrientation;
}

double BoostProperties::scaleFactor() const
{
  return mScaleFactor;
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
    mDescriptorType = descriptorType;
  }
}

void BoostProperties::setUseOrientation(bool useOrientation)
{
  bUseOrientation = useOrientation;
}

void BoostProperties::setScaleFactor(double scaleFactor)
{
  mScaleFactor = scaleFactor;
}

void BoostProperties::reset()
{
  mDescriptorType = "BINBOOST_256";
  bUseOrientation = boost_default_value_use_orientation;
  mScaleFactor = boost_default_value_scale_factor;
}

std::string BoostProperties::name() const
{
  return std::string("BOOST");
}


/*----------------------------------------------------------------*/


BoostDescriptor::BoostDescriptor()
{
  update();
}

BoostDescriptor::BoostDescriptor(const BoostDescriptor &boostDescriptor)
  : BoostProperties(boostDescriptor)
{
  update();
}

BoostDescriptor::BoostDescriptor(BoostDescriptor &&boostDescriptor) TL_NOEXCEPT
  : BoostProperties(std::forward<BoostProperties>(boostDescriptor))
{
  update();
}

BoostDescriptor::BoostDescriptor(const std::string &descriptorType,
                                 bool useOrientation,
                                 double scaleFactor)
{
  BoostProperties::setDescriptorType(descriptorType);
  BoostProperties::setUseOrientation(useOrientation);
  BoostProperties::setScaleFactor(scaleFactor);
  update();
}

BoostDescriptor &BoostDescriptor::operator =(const BoostDescriptor &boostDescriptor)
{
  if (this != &boostDescriptor){
    BoostProperties::operator=(boostDescriptor);
    update();
  }
  return *this;
}

BoostDescriptor &BoostDescriptor::operator =(BoostDescriptor &&boostDescriptor) TL_NOEXCEPT
{
  if (this != &boostDescriptor){
    BoostProperties::operator=(std::forward<BoostProperties>(boostDescriptor));
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
  std::string descriptorType = BoostProperties::descriptorType();
  if (descriptorType == "BGM") {
    descriptor_type = cv::xfeatures2d::BoostDesc::BGM;
  } else if (descriptorType == "BGM_HARD"){
    descriptor_type = cv::xfeatures2d::BoostDesc::BGM_HARD;
  } else if (descriptorType == "BGM_BILINEAR"){
    descriptor_type = cv::xfeatures2d::BoostDesc::BGM_BILINEAR;
  } else if (descriptorType == "LBGM"){
    descriptor_type = cv::xfeatures2d::BoostDesc::LBGM;
  } else if (descriptorType == "BINBOOST_64"){
    descriptor_type = cv::xfeatures2d::BoostDesc::BINBOOST_64;
  } else if (descriptorType == "BINBOOST_128"){
    descriptor_type = cv::xfeatures2d::BoostDesc::BINBOOST_128;
  } else if (descriptorType == "BINBOOST_256"){
    descriptor_type = cv::xfeatures2d::BoostDesc::BINBOOST_256;
  }

  mBoost = cv::xfeatures2d::BoostDesc::create(descriptor_type,
           BoostProperties::useOrientation(),
           static_cast<float>(BoostProperties::scaleFactor()));

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

void BoostDescriptor::reset()
{
  BoostProperties::reset();
  update();
}

void BoostDescriptor::setDescriptorType(const std::string &descriptorType)
{
  BoostProperties::setDescriptorType(descriptorType);
  update();
}

void BoostDescriptor::setUseOrientation(bool useOrientation)
{
  BoostProperties::setUseOrientation(useOrientation);
  update();
}

void BoostDescriptor::setScaleFactor(double scaleFactor)
{
  BoostProperties::setScaleFactor(scaleFactor);
  update();
}

cv::Mat BoostDescriptor::extract(const cv::Mat &img, 
                                 std::vector<cv::KeyPoint> &keyPoints)
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


