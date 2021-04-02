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
 * License along with Foobar. If not, see <http://www.gnu.org/licenses/>. *
 *                                                                        *
 * @license LGPL-3.0 <https://www.gnu.org/licenses/lgpl-3.0.html>         *
 *                                                                        *
 **************************************************************************/

#include "boost.h"

#include <tidop/core/messages.h>

namespace tl
{

BoostProperties::BoostProperties()
  : mDescriptorType("BINBOOST_256"),
    bUseOrientation(true),
    mScaleFactor(6.25)
{

}

BoostProperties::BoostProperties(const BoostProperties &boostProperties)
  : Boost(boostProperties),
    mDescriptorType(boostProperties.mDescriptorType),
    bUseOrientation(boostProperties.bUseOrientation),
    mScaleFactor(boostProperties.mScaleFactor)
{

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
  if (descriptorType.compare("BGM") == 0 ||
      descriptorType.compare("BGM_HARD") == 0 ||
      descriptorType.compare("BGM_BILINEAR") == 0 ||
      descriptorType.compare("LBGM") == 0||
      descriptorType.compare("BINBOOST_64") == 0||
      descriptorType.compare("BINBOOST_128") == 0||
      descriptorType.compare("BINBOOST_256") == 0) {
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
  bUseOrientation = true;
  mScaleFactor = 6.25;
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
  : BoostProperties(boostDescriptor),
    DescriptorExtractor(boostDescriptor)
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

void BoostDescriptor::update()
{
#ifdef HAVE_OPENCV_XFEATURES2D

#if CV_VERSION_MAJOR >= 4 || (CV_VERSION_MAJOR >= 3 && CV_VERSION_MINOR > 2)
  int descriptor_type = cv::xfeatures2d::BoostDesc::BGM;
  std::string descriptorType = BoostProperties::descriptorType();
  if (descriptorType.compare("BGM") == 0 ) {
    descriptor_type = cv::xfeatures2d::BoostDesc::BGM;
  } else if (descriptorType.compare("BGM_HARD") == 0){
    descriptor_type = cv::xfeatures2d::BoostDesc::BGM_HARD;
  } else if (descriptorType.compare("BGM_BILINEAR") == 0){
    descriptor_type = cv::xfeatures2d::BoostDesc::BGM_BILINEAR;
  } else if (descriptorType.compare("LBGM") == 0){
    descriptor_type = cv::xfeatures2d::BoostDesc::LBGM;
  } else if (descriptorType.compare("BINBOOST_64") == 0){
    descriptor_type = cv::xfeatures2d::BoostDesc::BINBOOST_64;
  } else if (descriptorType.compare("BINBOOST_128") == 0){
    descriptor_type = cv::xfeatures2d::BoostDesc::BINBOOST_128;
  } else if (descriptorType.compare("BINBOOST_256") == 0){
    descriptor_type = cv::xfeatures2d::BoostDesc::BINBOOST_256;
  }

  mBoost = cv::xfeatures2d::BoostDesc::create(descriptor_type,
                                              BoostProperties::useOrientation(),
                                              static_cast<float>(BoostProperties::scaleFactor()));
#endif

#endif
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

#ifdef HAVE_OPENCV_XFEATURES2D 

#if CV_VERSION_MAJOR >= 4 || (CV_VERSION_MAJOR == 3 && CV_VERSION_MINOR > 2)
  mBoost->compute(img, keyPoints, descriptors);
#  else
  TL_COMPILER_WARNING("Boost Descriptor not supported in OpenCV versions < 3.3")
  throw std::exception("Boost Descriptor not supported in OpenCV versions < 3.3");
#endif

#else
  TL_COMPILER_WARNING("OpenCV not built with extra modules. Boost Descriptor not supported")
  throw std::exception("OpenCV not built with extra modules. Boost Descriptor not supported");
#endif // HAVE_OPENCV_XFEATURES2D

  return descriptors;
}


} // namespace tl


