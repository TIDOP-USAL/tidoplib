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

#include "akaze.h"

#include "tidop/core/messages.h"


namespace tl
{


AkazeProperties::AkazeProperties()
  : mDescriptorType(akaze_default_value_descriptor_type),
    mDiffusivity(akaze_default_value_diffusivity)
{
}

AkazeProperties::AkazeProperties(const AkazeProperties &akazeProperties) = default;

AkazeProperties::AkazeProperties(AkazeProperties &&akazeProperties) TL_NOEXCEPT
  : mDescriptorType(std::move(akazeProperties.mDescriptorType)),
    mDescriptorSize(akazeProperties.mDescriptorSize),
    mDescriptorChannels(akazeProperties.mDescriptorChannels),
    mThreshold(akazeProperties.mThreshold),
    mOctaves(akazeProperties.mOctaves),
    mOctaveLayers(akazeProperties.mOctaveLayers),
    mDiffusivity(std::move(akazeProperties.mDiffusivity))
{
}

AkazeProperties::~AkazeProperties() = default;

AkazeProperties &AkazeProperties::operator =(const AkazeProperties &akazeProperties)
{
  if (this != &akazeProperties) {
    mDescriptorType = akazeProperties.mDescriptorType;
    mDescriptorSize = akazeProperties.mDescriptorSize;
    mDescriptorChannels = akazeProperties.mDescriptorChannels;
    mThreshold = akazeProperties.mThreshold;
    mOctaves = akazeProperties.mOctaves;
    mOctaveLayers = akazeProperties.mOctaveLayers;
    mDiffusivity = akazeProperties.mDiffusivity;
  }
  return *this;
}

AkazeProperties &AkazeProperties::operator =(AkazeProperties &&akazeProperties) TL_NOEXCEPT
{
  if (this != &akazeProperties) {
    mDescriptorType = std::move(akazeProperties.mDescriptorType);
    mDescriptorSize = akazeProperties.mDescriptorSize;
    mDescriptorChannels = akazeProperties.mDescriptorChannels;
    mThreshold = akazeProperties.mThreshold;
    mOctaves = akazeProperties.mOctaves;
    mOctaveLayers = akazeProperties.mOctaveLayers;
    mDiffusivity = std::move(akazeProperties.mDiffusivity);
  }
  return *this;
}

std::string AkazeProperties::descriptorType() const
{
  return mDescriptorType;
}

int AkazeProperties::descriptorSize() const
{
  return mDescriptorSize;
}

int AkazeProperties::descriptorChannels() const
{
  return mDescriptorChannels;
}

double AkazeProperties::threshold() const
{
  return mThreshold;
}

int AkazeProperties::octaves() const
{
  return mOctaves;
}

int AkazeProperties::octaveLayers() const
{
  return mOctaveLayers;
}

std::string AkazeProperties::diffusivity() const
{
  return mDiffusivity;
}

void AkazeProperties::setDescriptorType(const std::string &descriptorType)
{
  if (descriptorType == "KAZE" ||
      descriptorType == "KAZE_UPRIGHT" ||
      descriptorType == "MLDB" ||
      descriptorType == "MLDB_UPRIGHT"){
    mDescriptorType = descriptorType;
  }
}

void AkazeProperties::setDescriptorSize(int descriptorSize)
{
  mDescriptorSize = descriptorSize;
}

void AkazeProperties::setDescriptorChannels(int channels)
{
  mDescriptorChannels = channels;
}

void AkazeProperties::setThreshold(double threshold)
{
  mThreshold = threshold;
}

void AkazeProperties::setOctaves(int octaves)
{
  mOctaves = octaves;
}

void AkazeProperties::setOctaveLayers(int octaveLayers)
{
  mOctaveLayers = octaveLayers;
}

void AkazeProperties::setDiffusivity(const std::string &diffusivity)
{
  if (diffusivity == "DIFF_PM_G1" ||
      diffusivity == "DIFF_PM_G2" ||
      diffusivity == "DIFF_WEICKERT" ||
      diffusivity == "DIFF_CHARBONNIER"){
    mDiffusivity = diffusivity;
  }
}

void AkazeProperties::reset()
{
  mDescriptorType = akaze_default_value_descriptor_type;
  mDescriptorSize = akaze_default_value_descriptor_size;
  mDescriptorChannels = akaze_default_value_descriptor_channels;
  mThreshold = akaze_default_value_threshold;
  mOctaves = akaze_default_value_octaves;
  mOctaveLayers = akaze_default_value_octave_layers;
  mDiffusivity = akaze_default_value_diffusivity;
}

std::string AkazeProperties::name() const
{
  return std::string("AKAZE");
}



/*----------------------------------------------------------------*/


AkazeDetectorDescriptor::AkazeDetectorDescriptor()
{
  mAkaze = cv::AKAZE::create(convertDescriptorType(AkazeProperties::descriptorType()),
                             AkazeProperties::descriptorSize(),
                             AkazeProperties::descriptorChannels(),
                             static_cast<float>(AkazeProperties::threshold()),
                             AkazeProperties::octaves(),
                             AkazeProperties::octaveLayers(),
                             convertDiffusivity(AkazeProperties::diffusivity()));
}

AkazeDetectorDescriptor::AkazeDetectorDescriptor(const AkazeDetectorDescriptor &akazeDetectorDescriptor)
  : AkazeProperties(akazeDetectorDescriptor),
    KeypointDetector(akazeDetectorDescriptor),
    DescriptorExtractor(akazeDetectorDescriptor)
{
  this->initAkazeFromProperties();
}

AkazeDetectorDescriptor::AkazeDetectorDescriptor(AkazeDetectorDescriptor &&akazeDetectorDescriptor) TL_NOEXCEPT
  : AkazeProperties(std::forward<AkazeProperties>(akazeDetectorDescriptor))
{
  this->initAkazeFromProperties();
}

AkazeDetectorDescriptor::AkazeDetectorDescriptor(const std::string &descriptorType,
                                                 int descriptorSize,
                                                 int descriptorChannels,
                                                 double threshold,
                                                 int octaves,
                                                 int octaveLayers,
                                                 const std::string &diffusivity)
  : mAkaze(cv::AKAZE::create())
{
  setDescriptorType(descriptorType);
  setDescriptorSize(descriptorSize);
  setDescriptorChannels(descriptorChannels);
  setThreshold(threshold);
  setOctaves(octaves);
  setOctaveLayers(octaveLayers);
  setDiffusivity(diffusivity);
}

AkazeDetectorDescriptor &AkazeDetectorDescriptor::operator =(const AkazeDetectorDescriptor &akazeDetectorDescriptor)
{
  if (this != &akazeDetectorDescriptor){
    AkazeProperties::operator=(akazeDetectorDescriptor);
    this->initAkazeFromProperties();
  }
  return *this;
}

AkazeDetectorDescriptor &AkazeDetectorDescriptor::operator =(AkazeDetectorDescriptor &&akazeDetectorDescriptor) TL_NOEXCEPT
{
  if (this != &akazeDetectorDescriptor){
    AkazeProperties::operator=(std::forward<AkazeProperties>(akazeDetectorDescriptor));
    this->initAkazeFromProperties();
  }
  return *this;
}

#if CV_VERSION_MAJOR >= 4
cv::AKAZE::DescriptorType AkazeDetectorDescriptor::convertDescriptorType(const std::string &descriptorType)
{
  cv::AKAZE::DescriptorType descriptor_type = cv::AKAZE::DescriptorType::DESCRIPTOR_MLDB;

  if (descriptorType == "KAZE"){
    descriptor_type = cv::AKAZE::DESCRIPTOR_KAZE;
  } else if (descriptorType == "KAZE_UPRIGHT"){
    descriptor_type = cv::AKAZE::DESCRIPTOR_KAZE_UPRIGHT;
  } else if (descriptorType == "MLDB"){
    descriptor_type = cv::AKAZE::DESCRIPTOR_MLDB;
  } else if (descriptorType == "MLDB_UPRIGHT"){
    descriptor_type = cv::AKAZE::DESCRIPTOR_MLDB_UPRIGHT;
  }

  return descriptor_type;
}

cv::KAZE::DiffusivityType AkazeDetectorDescriptor::convertDiffusivity(const std::string &diffusivity)
{
  cv::KAZE::DiffusivityType diff = cv::KAZE::DiffusivityType::DIFF_PM_G1;

  if (diffusivity == "DIFF_PM_G1"){
    diff = cv::KAZE::DIFF_PM_G1;
  } else if (diffusivity == "DIFF_PM_G2"){
    diff = cv::KAZE::DIFF_PM_G2;
  } else if (diffusivity == "DIFF_WEICKERT"){
    diff = cv::KAZE::DIFF_WEICKERT;
  } else if (diffusivity == "DIFF_CHARBONNIER"){
    diff = cv::KAZE::DIFF_CHARBONNIER;
  }

  return diff;
}

void AkazeDetectorDescriptor::initAkazeFromProperties()
{
  mAkaze = cv::AKAZE::create(convertDescriptorType(AkazeProperties::descriptorType()),
                             AkazeProperties::descriptorSize(),
                             AkazeProperties::descriptorChannels(),
                             static_cast<float>(AkazeProperties::threshold()),
                             AkazeProperties::octaves(),
                             AkazeProperties::octaveLayers(),
                             convertDiffusivity(AkazeProperties::diffusivity()));
}

#else

int AkazeDetectorDescriptor::convertDescriptorType(const std::string &descriptorType)
{
  int descriptor_type = cv::AKAZE::DESCRIPTOR_MLDB;

  if (descriptorType == "KAZE"){
    descriptor_type = cv::AKAZE::DESCRIPTOR_KAZE;
  } else if (descriptorType == "KAZE_UPRIGHT"){
    descriptor_type = cv::AKAZE::DESCRIPTOR_KAZE_UPRIGHT;
  } else if (descriptorType == "MLDB"){
    descriptor_type = cv::AKAZE::DESCRIPTOR_MLDB;
  } else if (descriptorType == "MLDB_UPRIGHT"){
    descriptor_type = cv::AKAZE::DESCRIPTOR_MLDB_UPRIGHT;
  }

  return descriptor_type;
}

int AkazeDetectorDescriptor::convertDiffusivity(const std::string &diffusivity)
{
  int diff = cv::KAZE::DIFF_PM_G1;

  if (diffusivity == "DIFF_PM_G1"){
    diff = cv::KAZE::DIFF_PM_G1;
  } else if (diffusivity == "DIFF_PM_G2"){
    diff = cv::KAZE::DIFF_PM_G2;
  } else if (diffusivity == "DIFF_WEICKERT"){
    diff = cv::KAZE::DIFF_WEICKERT;
  } else if (diffusivity == "DIFF_CHARBONNIER") == 0){
    diff = cv::KAZE::DIFF_CHARBONNIER;
  }

  return diff;
}
#endif

std::vector<cv::KeyPoint> AkazeDetectorDescriptor::detect(const cv::Mat &img,
                                                          cv::InputArray &mask)
{
  std::vector<cv::KeyPoint> keyPoints;

  try {

    mAkaze->detect(img, keyPoints, mask);

  } catch (...) {
    std::throw_with_nested(std::runtime_error("AkazeDetectorDescriptor::detect() failed"));
  }
  
  return keyPoints;
}


cv::Mat AkazeDetectorDescriptor::extract(const cv::Mat &img, 
                                         std::vector<cv::KeyPoint> &keyPoints)
{
  cv::Mat descriptors;
 
  try {

    mAkaze->compute(img, keyPoints, descriptors);

  } catch (...) {
    std::throw_with_nested(std::runtime_error("AkazeDetectorDescriptor::extract() failed"));
  }

  return descriptors;
}

void AkazeDetectorDescriptor::setDescriptorType(const std::string &descriptorType)
{
  AkazeProperties::setDescriptorType(descriptorType);
  mAkaze->setDescriptorType(convertDescriptorType(descriptorType));
}

void AkazeDetectorDescriptor::setDescriptorSize(int descriptorSize)
{
  AkazeProperties::setDescriptorSize(descriptorSize);
  mAkaze->setDescriptorSize(descriptorSize);
}

void AkazeDetectorDescriptor::setDescriptorChannels(int channels)
{
  AkazeProperties::setDescriptorChannels(channels);
  mAkaze->setDescriptorChannels(channels);
}

void AkazeDetectorDescriptor::setThreshold(double threshold)
{
  AkazeProperties::setThreshold(threshold);
  mAkaze->setThreshold(threshold);
}

void AkazeDetectorDescriptor::setOctaves(int octaves)
{
  AkazeProperties::setOctaves(octaves);
  mAkaze->setNOctaves(octaves);
}

void AkazeDetectorDescriptor::setOctaveLayers(int octaveLayers)
{
  AkazeProperties::setOctaveLayers(octaveLayers);
  mAkaze->setNOctaveLayers(octaveLayers);
}

void AkazeDetectorDescriptor::setDiffusivity(const std::string &diffusivity)
{
  AkazeProperties::setDiffusivity(diffusivity);
  mAkaze->setDiffusivity(convertDiffusivity(diffusivity));
}

void AkazeDetectorDescriptor::reset()
{
  AkazeProperties::reset();
  mAkaze->setDescriptorType(convertDescriptorType(AkazeProperties::descriptorType()));
  mAkaze->setDescriptorSize(AkazeProperties::descriptorSize());
  mAkaze->setDescriptorChannels(AkazeProperties::descriptorChannels());
  mAkaze->setThreshold(AkazeProperties::threshold());
  mAkaze->setNOctaves(AkazeProperties::octaves());
  mAkaze->setNOctaveLayers(AkazeProperties::octaveLayers());
  mAkaze->setDiffusivity(convertDiffusivity(AkazeProperties::diffusivity()));
}


} // namespace tl
