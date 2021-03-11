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

#include "kaze.h"

#include "tidop/core/messages.h"


namespace tl
{


KazeProperties::KazeProperties()
  : Kaze(),
    mExtended(false),
    mUpright(false),
    mThreshold(0.001),
    mOctaves(4),
    mOctaveLayers(4),
    mDiffusivity("DIFF_PM_G2")
{}

KazeProperties::KazeProperties(const KazeProperties &kazeProperties)
  : Kaze(kazeProperties),
    mExtended(kazeProperties.mExtended),
    mUpright(kazeProperties.mUpright),
    mThreshold(kazeProperties.mThreshold),
    mOctaves(kazeProperties.mOctaves),
    mOctaveLayers(kazeProperties.mOctaveLayers),
    mDiffusivity(kazeProperties.mDiffusivity)
{
}

bool KazeProperties::extendedDescriptor() const
{
  return mExtended;
}

bool KazeProperties::uprightDescriptor() const
{
  return mUpright;
}

double KazeProperties::threshold() const
{
  return mThreshold;
}

int KazeProperties::octaves() const
{
  return mOctaves;
}

int KazeProperties::octaveLayers() const
{
  return mOctaveLayers;
}

std::string KazeProperties::diffusivity() const
{
  return mDiffusivity;
}

void KazeProperties::setExtendedDescriptor(bool extended)
{
  mExtended = extended;
}

void KazeProperties::setUprightDescriptor(bool uprightDescriptor)
{
  mUpright = uprightDescriptor;
}

void KazeProperties::setThreshold(double threshold)
{
  mThreshold = threshold;
}

void KazeProperties::setOctaves(int octaves)
{
  mOctaves = octaves;
}

void KazeProperties::setOctaveLayers(int octaveLayers)
{
  mOctaveLayers = octaveLayers;
}

void KazeProperties::setDiffusivity(const std::string &diffusivity)
{
  if (diffusivity.compare("DIFF_PM_G1") == 0 ||
      diffusivity.compare("DIFF_PM_G2") == 0 ||
      diffusivity.compare("DIFF_WEICKERT") == 0 ||
      diffusivity.compare("DIFF_CHARBONNIER") == 0){
    mDiffusivity = diffusivity;
  }
}

void KazeProperties::reset()
{
  mExtended = false;
  mUpright = false;
  mThreshold = 0.001;
  mOctaves = 4;
  mOctaveLayers = 4;
  mDiffusivity = "DIFF_PM_G2";
}

std::string KazeProperties::name() const
{
  return std::string("KAZE");
}

/*----------------------------------------------------------------*/



KazeDetectorDescriptor::KazeDetectorDescriptor()
  : mKaze(cv::KAZE::create())
{
  this->updateCvKaze();
}

KazeDetectorDescriptor::KazeDetectorDescriptor(const KazeDetectorDescriptor &kazeDetectorDescriptor)
  : KazeProperties(kazeDetectorDescriptor),
    KeypointDetector(kazeDetectorDescriptor),
    DescriptorExtractor(kazeDetectorDescriptor),
    mKaze(cv::KAZE::create())
{
  this->updateCvKaze();
}

KazeDetectorDescriptor::KazeDetectorDescriptor(bool extendedDescriptor,
                                               bool uprightDescriptor,
                                               double threshold,
                                               int octaves,
                                               int octaveLayers,
                                               const std::string &diffusivity)
  : mKaze(cv::KAZE::create())
{
  setExtendedDescriptor(extendedDescriptor);
  setUprightDescriptor(uprightDescriptor);
  setThreshold(threshold);
  setOctaves(octaves);
  setOctaveLayers(octaveLayers);
  setDiffusivity(diffusivity);
}

#if CV_VERSION_MAJOR >= 4

TL_DISABLE_WARNING(26812)
cv::KAZE::DiffusivityType KazeDetectorDescriptor::convertDiffusivity(const std::string &diffusivity)
{
  cv::KAZE::DiffusivityType diff = cv::KAZE::DIFF_PM_G1;
  if (diffusivity.compare("DIFF_PM_G1") == 0){
    diff = cv::KAZE::DIFF_PM_G1;
  } else if (diffusivity.compare("DIFF_PM_G2") == 0){
    diff = cv::KAZE::DIFF_PM_G2;
  } else if (diffusivity.compare("DIFF_WEICKERT") == 0){
    diff = cv::KAZE::DIFF_WEICKERT;
  } else if (diffusivity.compare("DIFF_CHARBONNIER") == 0){
    diff = cv::KAZE::DIFF_CHARBONNIER;
  }
  return diff;
}
TL_ENABLE_WARNING(26812)

#else

int KazeDetectorDescriptor::convertDiffusivity(const std::string &diffusivity)
{
  int diff = cv::KAZE::DIFF_PM_G1;
  if (diffusivity.compare("DIFF_PM_G1") == 0){
    diff = cv::KAZE::DIFF_PM_G1;
  } else if (diffusivity.compare("DIFF_PM_G2") == 0){
    diff = cv::KAZE::DIFF_PM_G2;
  } else if (diffusivity.compare("DIFF_WEICKERT") == 0){
    diff = cv::KAZE::DIFF_WEICKERT;
  } else if (diffusivity.compare("DIFF_CHARBONNIER") == 0){
    diff = cv::KAZE::DIFF_CHARBONNIER;
  }
  return diff;
}

#endif

void KazeDetectorDescriptor::updateCvKaze()
{
  mKaze->setExtended(KazeProperties::extendedDescriptor());
  mKaze->setUpright(KazeProperties::uprightDescriptor());
  mKaze->setThreshold(KazeProperties::threshold());
  mKaze->setNOctaves(KazeProperties::octaves());
  mKaze->setNOctaveLayers(KazeProperties::octaveLayers());
  mKaze->setDiffusivity(convertDiffusivity(KazeProperties::diffusivity()));
}

std::vector<cv::KeyPoint> KazeDetectorDescriptor::detect(const cv::Mat &img, 
                                                         cv::InputArray &mask)
{
  std::vector<cv::KeyPoint> keyPoints;
  mKaze->detect(img, keyPoints, mask);
  return keyPoints;
}

cv::Mat KazeDetectorDescriptor::extract(const cv::Mat &img, 
                                        std::vector<cv::KeyPoint> &keyPoints)
{
  cv::Mat descriptors;
  mKaze->compute(img, keyPoints, descriptors);
  return descriptors;
}

void KazeDetectorDescriptor::setExtendedDescriptor(bool extended)
{
  KazeProperties::setExtendedDescriptor(extended);
  mKaze->setExtended(extended);
}

void KazeDetectorDescriptor::setUprightDescriptor(bool uprightDescriptor)
{
  KazeProperties::setUprightDescriptor(uprightDescriptor);
  mKaze->setUpright(uprightDescriptor);
}

void KazeDetectorDescriptor::setThreshold(double threshold)
{
  KazeProperties::setThreshold(threshold);
  mKaze->setThreshold(threshold);
}

void KazeDetectorDescriptor::setOctaves(int octaves)
{
  KazeProperties::setOctaves(octaves);
  mKaze->setNOctaves(octaves);
}

void KazeDetectorDescriptor::setOctaveLayers(int octaveLayers)
{
  KazeProperties::setOctaveLayers(octaveLayers);
  mKaze->setNOctaveLayers(octaveLayers);
}

void KazeDetectorDescriptor::setDiffusivity(const std::string &diffusivity)
{
  KazeProperties::setDiffusivity(diffusivity);
  mKaze->setDiffusivity(convertDiffusivity(diffusivity));
}

void KazeDetectorDescriptor::reset()
{
  KazeProperties::reset();
  this->updateCvKaze();
}


} // namespace tl







