/************************************************************************
 *                                                                      *
 * Copyright (C) 2020 by Tidop Research Group                           *
 *                                                                      *
 * This file is part of TidopLib                                        *
 *                                                                      *
 * TidopLib is free software: you can redistribute it and/or modify     *
 * it under the terms of the GNU General Public License as published by *
 * the Free Software Foundation, either version 3 of the License, or    *
 * (at your option) any later version.                                  *
 *                                                                      *
 * TidopLib is distributed in the hope that it will be useful,          *
 * but WITHOUT ANY WARRANTY; without even the implied warranty of       *
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the        *
 * GNU General Public License for more details.                         *
 *                                                                      *
 * You should have received a copy of the GNU General Public License    *
 * along with Foobar.  If not, see <http://www.gnu.org/licenses/>.      *
 *                                                                      *
 * @license GPL-3.0+ <http://spdx.org/licenses/GPL-3.0+>                *
 *                                                                      *
 ************************************************************************/

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
  mBrief = cv::xfeatures2d::BriefDescriptorExtractor::create(std::stoi(BriefProperties::bytes()),
                                                             BriefProperties::useOrientation());
}

cv::Mat BriefDescriptor::extract(const cv::Mat &img,
                              std::vector<cv::KeyPoint> &keyPoints)
{
  cv::Mat descriptors;
  mBrief->compute(img, keyPoints, descriptors);
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
