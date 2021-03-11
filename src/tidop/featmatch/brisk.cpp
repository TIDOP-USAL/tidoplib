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

#include "brisk.h"

#include "tidop/core/messages.h"


namespace tl
{


BriskProperties::BriskProperties()
  : mThreshold(30),
    mOctaves(3),
    mPatternScale(1.0)
{}

BriskProperties::BriskProperties(const BriskProperties &briskProperties)
  : Brisk(briskProperties),
    mThreshold(briskProperties.mThreshold),
    mOctaves(briskProperties.mOctaves),
    mPatternScale(briskProperties.mPatternScale)
{}

int BriskProperties::threshold() const
{
  return mThreshold;
}

int BriskProperties::octaves() const
{
  return mOctaves;
}

double BriskProperties::patternScale() const
{
  return mPatternScale;
}

void BriskProperties::setThreshold(int threshold)
{
  mThreshold = threshold;
}

void BriskProperties::setOctaves(int octaves)
{
  mOctaves = octaves;
}

void BriskProperties::setPatternScale(double patternScale)
{
  mPatternScale = patternScale;
}

void BriskProperties::reset()
{
  mThreshold = 30;
  mOctaves = 3;
  mPatternScale = 1.0;
}

std::string BriskProperties::name() const
{
  return std::string("BRISK");
}


/*----------------------------------------------------------------*/


BriskDetectorDescriptor::BriskDetectorDescriptor()
  : BriskProperties(),
    KeypointDetector(),
    DescriptorExtractor()
{
  update();
}

BriskDetectorDescriptor::BriskDetectorDescriptor(const BriskDetectorDescriptor &briskDetectorDescriptor)
  : BriskProperties(briskDetectorDescriptor),
    KeypointDetector(briskDetectorDescriptor),
    DescriptorExtractor(briskDetectorDescriptor)
{
  update();
}


BriskDetectorDescriptor::BriskDetectorDescriptor(int threshold,
                                                 int octaves,
                                                 double patternScale)
  : BriskProperties(),
    KeypointDetector(),
    DescriptorExtractor()
{
  BriskProperties::setThreshold(threshold);
  BriskProperties::setOctaves(octaves);
  BriskProperties::setPatternScale(patternScale);
  update();
}

void BriskDetectorDescriptor::update()
{
  mBrisk = cv::BRISK::create(BriskProperties::threshold(),
                             BriskProperties::octaves(),
                             static_cast<float>(BriskProperties::patternScale()));
}

std::vector<cv::KeyPoint> BriskDetectorDescriptor::detect(const cv::Mat &img,
                                                          cv::InputArray &mask)
{
  std::vector<cv::KeyPoint> keyPoints;
  mBrisk->detect(img, keyPoints, mask);
  return keyPoints;
}

cv::Mat BriskDetectorDescriptor::extract(const cv::Mat &img,
                                         std::vector<cv::KeyPoint> &keyPoints)
{
  cv::Mat descriptors;
  mBrisk->compute(img, keyPoints, descriptors);
  return descriptors;
}

void BriskDetectorDescriptor::setThreshold(int threshold)
{
  BriskProperties::setThreshold(threshold);
  update();
}

void BriskDetectorDescriptor::setOctaves(int octaves)
{
  BriskProperties::setOctaves(octaves);
  update();
}

void BriskDetectorDescriptor::setPatternScale(double patternScale)
{
  BriskProperties::setPatternScale(patternScale);
  update();
}

void BriskDetectorDescriptor::reset()
{
  BriskProperties::reset();
  update();
}


} // namespace tl
