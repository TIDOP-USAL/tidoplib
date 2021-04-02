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

#include "brief.h"

#include "tidop/core/messages.h"


namespace tl
{


BriefProperties::BriefProperties()
  : mBytes("32"),
    mUseOrientation(false)
{
}

BriefProperties::BriefProperties(const BriefProperties & briefProperties)
  : Brief(briefProperties),
    mBytes(briefProperties.mBytes),
    mUseOrientation(briefProperties.mUseOrientation)
{
}

std::string BriefProperties::bytes() const
{
  return mBytes;
}

bool BriefProperties::useOrientation() const
{
  return mUseOrientation;
}

void BriefProperties::setBytes(const std::string &bytes)
{
  if (bytes.compare("16") == 0 ||
      bytes.compare("32") == 0 ||
      bytes.compare("64") == 0) {
    mBytes = bytes;
  }
}

void BriefProperties::setUseOrientation(bool useOrientation)
{
  mUseOrientation = useOrientation;
}

void BriefProperties::reset()
{
  mBytes = "32";
  mUseOrientation = false;
}

std::string BriefProperties::name() const
{
  return std::string("BRIEF");
}


/*----------------------------------------------------------------*/


BriefDescriptor::BriefDescriptor()
  : BriefProperties(),
    DescriptorExtractor()
{
  update();
}

BriefDescriptor::BriefDescriptor(const BriefDescriptor &briefDescriptor)
  : BriefProperties(briefDescriptor),
    DescriptorExtractor(briefDescriptor)
{
  update();
}

BriefDescriptor::BriefDescriptor(std::string bytes, bool useOrientation)
  : BriefProperties(),
    DescriptorExtractor()
{
  BriefProperties::setBytes(bytes);
  BriefProperties::setUseOrientation(useOrientation);
  update();
}

void BriefDescriptor::update()
{
#ifdef HAVE_OPENCV_XFEATURES2D 
  mBrief = cv::xfeatures2d::BriefDescriptorExtractor::create(std::stoi(BriefProperties::bytes()),
                                                             BriefProperties::useOrientation());
#endif // HAVE_OPENCV_XFEATURES2D
}

cv::Mat BriefDescriptor::extract(const cv::Mat &img,
                              std::vector<cv::KeyPoint> &keyPoints)
{
  cv::Mat descriptors;
#ifdef HAVE_OPENCV_XFEATURES2D 
  mBrief->compute(img, keyPoints, descriptors);
#else
  TL_COMPILER_WARNING("OpenCV not built with extra modules. Brief Descriptor not supported")
  throw std::exception("OpenCV not built with extra modules. Brief Descriptor not supported");
#endif // HAVE_OPENCV_XFEATURES2D

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
