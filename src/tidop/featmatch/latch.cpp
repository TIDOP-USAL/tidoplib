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

#include "latch.h"

#include "tidop/core/messages.h"


namespace tl
{

LatchProperties::LatchProperties()
  : mBytes("32"),
    mRotationInvariance(true),
    mHalfSsdSize(3)
{
}

LatchProperties::LatchProperties(const LatchProperties &latchProperties)
  : Latch(latchProperties),
    mBytes(latchProperties.mBytes),
    mRotationInvariance(latchProperties.mRotationInvariance),
    mHalfSsdSize(latchProperties.mHalfSsdSize)
{
}

std::string LatchProperties::bytes() const
{
  return mBytes;
}

bool LatchProperties::rotationInvariance() const
{
  return mRotationInvariance;
}

int LatchProperties::halfSsdSize() const
{
  return mHalfSsdSize;
}

void LatchProperties::setBytes(const std::string &bytes)
{
  if (bytes.compare("1") == 0 ||
      bytes.compare("2") == 0 ||
      bytes.compare("4") == 0 ||
      bytes.compare("8") == 0 ||
      bytes.compare("16") == 0 ||
      bytes.compare("32") == 0 ||
      bytes.compare("64") == 0) {
    mBytes = bytes;
  }
}

void LatchProperties::setRotationInvariance(bool rotationInvariance)
{
  mRotationInvariance = rotationInvariance;
}

void LatchProperties::setHalfSsdSize(int halfSsdSize)
{
  mHalfSsdSize = halfSsdSize;
}

void LatchProperties::reset()
{
  mBytes = "32";
  mRotationInvariance = true;
  mHalfSsdSize = 3;
}

std::string LatchProperties::name() const
{
  return std::string("LATCH");
}


/*----------------------------------------------------------------*/


LatchDescriptor::LatchDescriptor()
{
  update();
}

LatchDescriptor::LatchDescriptor(const LatchDescriptor &latchDescriptor)
  : LatchProperties(latchDescriptor),
    DescriptorExtractor(latchDescriptor)
{
  update();
}

LatchDescriptor::LatchDescriptor(const std::string &bytes,
                                 bool rotationInvariance,
                                 int halfSsdSize)
  : LatchProperties(),
    DescriptorExtractor()
{
  LatchProperties::setBytes(bytes);
  LatchProperties::setRotationInvariance(rotationInvariance);
  LatchProperties::setHalfSsdSize(halfSsdSize);
  update();
}

void LatchDescriptor::update()
{
  mLATCH = cv::xfeatures2d::LATCH::create(std::stoi(LatchProperties::bytes()),
                                          LatchProperties::rotationInvariance(),
                                          LatchProperties::halfSsdSize());
}

cv::Mat LatchDescriptor::extract(const cv::Mat &img, std::vector<cv::KeyPoint> &keyPoints)
{
  cv::Mat descriptors;
  mLATCH->compute(img, keyPoints, descriptors);
  return descriptors;
}

void LatchDescriptor::setBytes(const std::string &bytes)
{
  LatchProperties::setBytes(bytes);
  update();
}

void LatchDescriptor::setRotationInvariance(bool rotationInvariance)
{
  LatchProperties::setRotationInvariance(rotationInvariance);
  update();
}

void LatchDescriptor::setHalfSsdSize(int halfSsdSize)
{
  LatchProperties::setHalfSsdSize(halfSsdSize);
  update();
}

void LatchDescriptor::reset()
{
  LatchProperties::reset();
  update();
}



} // namespace tl
