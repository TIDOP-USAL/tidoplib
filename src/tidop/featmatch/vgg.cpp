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

#include "vgg.h"

#include <tidop/core/messages.h>

namespace tl
{


VggProperties::VggProperties()
  : mDescriptorType("VGG_120"),
    mScaleFactor(6.25),
    mSigma(1.4),
    bUseNormalizeDescriptor(false),
    bUseNormalizeImage(true),
    bUseScaleOrientation(true)
{
}

VggProperties::VggProperties(const VggProperties &vggProperties)
  : Vgg(vggProperties),
    mDescriptorType(vggProperties.mDescriptorType),
    mScaleFactor(vggProperties.mScaleFactor),
    mSigma(vggProperties.mSigma),
    bUseNormalizeDescriptor(vggProperties.bUseNormalizeDescriptor),
    bUseNormalizeImage(vggProperties.bUseNormalizeImage),
    bUseScaleOrientation(vggProperties.bUseScaleOrientation)
{
}

std::string VggProperties::descriptorType() const
{
  return mDescriptorType;
}

void VggProperties::setDescriptorType(const std::string &descriptorType)
{
  if (descriptorType.compare("VGG_120") == 0 ||
      descriptorType.compare("VGG_80") == 0 ||
      descriptorType.compare("VGG_64") == 0 ||
      descriptorType.compare("VGG_48") == 0) {
    mDescriptorType = descriptorType;
  }
}

double VggProperties::scaleFactor() const
{
  return mScaleFactor;
}

void VggProperties::setScaleFactor(double scaleFactor)
{
  mScaleFactor = scaleFactor;
}

double VggProperties::sigma() const
{
  return mSigma;
}

void VggProperties::setSigma(double sigma)
{
  mSigma = sigma;
}

bool VggProperties::useNormalizeDescriptor() const
{
  return bUseNormalizeDescriptor;
}

void VggProperties::setUseNormalizeDescriptor(bool useNormalizeDescriptor)
{
  bUseNormalizeDescriptor = useNormalizeDescriptor;
}

bool VggProperties::useNormalizeImage() const
{
  return bUseNormalizeImage;
}

void VggProperties::setUseNormalizeImage(bool useNormalizeImage)
{
  bUseNormalizeImage = useNormalizeImage;
}

bool VggProperties::useScaleOrientation() const
{
  return bUseScaleOrientation;
}

void VggProperties::setUseScaleOrientation(bool useScaleOrientation)
{
  bUseScaleOrientation = useScaleOrientation;
}

void VggProperties::reset()
{
  mDescriptorType = "VGG_120";
  mScaleFactor = 6.25;
  mSigma = 1.4;
  bUseNormalizeDescriptor = false;
  bUseNormalizeImage = true;
  bUseScaleOrientation = true;
}

std::string VggProperties::name() const
{
  return std::string("VGG");
}



/*----------------------------------------------------------------*/



VggDescriptor::VggDescriptor()
{
  update();
}

VggDescriptor::VggDescriptor(const VggDescriptor &vggDescriptor)
  : VggProperties(vggDescriptor)
{
  update();
}

VggDescriptor::VggDescriptor(std::string descriptorType,
                             double scaleFactor,
                             double sigma,
                             bool useNormalizeDescriptor,
                             bool useNormalizeImage,
                             bool useScaleOrientation)
{
  VggProperties::setDescriptorType(descriptorType);
  VggProperties::setScaleFactor(scaleFactor);
  VggProperties::setSigma(sigma);
  VggProperties::setUseNormalizeDescriptor(useNormalizeDescriptor);
  VggProperties::setUseNormalizeImage(useNormalizeImage);
  VggProperties::setUseScaleOrientation(useScaleOrientation);
  update();
}

void VggDescriptor::update()
{
#ifdef HAVE_OPENCV_XFEATURES2D 
#if CV_VERSION_MAJOR >= 4 || (CV_VERSION_MAJOR >= 3 && CV_VERSION_MINOR > 2)
  int descriptor_type = cv::xfeatures2d::VGG::VGG_120;
  std::string descriptorType = VggProperties::descriptorType();
  if (descriptorType.compare("VGG_120") == 0 ) {
    descriptor_type = cv::xfeatures2d::VGG::VGG_120;
  } else if (descriptorType.compare("VGG_80") == 0){
    descriptor_type = cv::xfeatures2d::VGG::VGG_80;
  } else if (descriptorType.compare("VGG_64") == 0){
    descriptor_type = cv::xfeatures2d::VGG::VGG_64;
  } else if (descriptorType.compare("VGG_48") == 0){
    descriptor_type = cv::xfeatures2d::VGG::VGG_48;
  }

  mVGG = cv::xfeatures2d::VGG::create(descriptor_type,
                                      static_cast<float>(VggProperties::sigma()),
                                      VggProperties::useNormalizeImage(),
                                      VggProperties::useScaleOrientation(),
                                      static_cast<float>(VggProperties::scaleFactor()),
                                      VggProperties::useNormalizeDescriptor());
#endif
#endif // HAVE_OPENCV_XFEATURES2D
}

void VggDescriptor::reset()
{
  VggProperties::reset();
  update();
}

void VggDescriptor::setDescriptorType(const std::string &descriptorType)
{
  VggProperties::setDescriptorType(descriptorType);
  update();
}

void VggDescriptor::setScaleFactor(double scaleFactor)
{
  VggProperties::setScaleFactor(scaleFactor);
  update();
}

void VggDescriptor::setSigma(double sigma)
{
  VggProperties::setSigma(sigma);
  update();
}

void VggDescriptor::setUseNormalizeDescriptor(bool useNormalizeDescriptor)
{
  VggProperties::setUseNormalizeDescriptor(useNormalizeDescriptor);
  update();
}

void VggDescriptor::setUseNormalizeImage(bool useNormalizeImage)
{
  VggProperties::setUseNormalizeImage(useNormalizeImage);
  update();
}

void VggDescriptor::setUseScaleOrientation(bool useScaleOrientation)
{
  VggProperties::setUseScaleOrientation(useScaleOrientation);
  update();
}

cv::Mat VggDescriptor::extract(const cv::Mat &img, std::vector<cv::KeyPoint> &keyPoints)
{
  cv::Mat descriptors;

#ifdef HAVE_OPENCV_XFEATURES2D 
#  if CV_VERSION_MAJOR >= 4 || (CV_VERSION_MAJOR == 3 && CV_VERSION_MINOR > 2)
  mVGG->compute(img, keyPoints, descriptors);
#  else
  TL_COMPILER_WARNING("VGG Descriptor not supported in OpenCV versions < 3.3 ")
  throw std::exception("VGG Descriptor not supported in OpenCV versions < 3.3");
#  endif
#else
  TL_COMPILER_WARNING("OpenCV not built with extra modules. VGG Descriptor not supported")
  throw std::exception("OpenCV not built with extra modules. VGG Descriptor not supported");
#endif // HAVE_OPENCV_XFEATURES2D

  return descriptors;
}

} // namespace tl


