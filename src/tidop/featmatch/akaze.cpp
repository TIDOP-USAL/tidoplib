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

#include "akaze.h"

#include "tidop/core/base/exception.h"


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

auto AkazeProperties::operator =(const AkazeProperties &akazeProperties) -> AkazeProperties&
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

auto AkazeProperties::operator =(AkazeProperties &&akazeProperties) TL_NOEXCEPT -> AkazeProperties&
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

auto AkazeProperties::descriptorType() const -> std::string
{
    return mDescriptorType;
}

auto AkazeProperties::descriptorSize() const -> int
{
    return mDescriptorSize;
}

auto AkazeProperties::descriptorChannels() const -> int
{
    return mDescriptorChannels;
}

auto AkazeProperties::threshold() const -> double
{
    return mThreshold;
}

auto AkazeProperties::octaves() const -> int
{
    return mOctaves;
}

auto AkazeProperties::octaveLayers() const -> int
{
    return mOctaveLayers;
}

auto AkazeProperties::diffusivity() const -> std::string
{
    return mDiffusivity;
}

void AkazeProperties::setDescriptorType(const std::string &descriptorType)
{
    if (descriptorType == "KAZE" ||
        descriptorType == "KAZE_UPRIGHT" ||
        descriptorType == "MLDB" ||
        descriptorType == "MLDB_UPRIGHT") {
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
        diffusivity == "DIFF_CHARBONNIER") {
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
	AkazeDetectorDescriptor::setDescriptorType(descriptorType);
    AkazeDetectorDescriptor::setDescriptorSize(descriptorSize);
    AkazeDetectorDescriptor::setDescriptorChannels(descriptorChannels);
    AkazeDetectorDescriptor::setThreshold(threshold);
    AkazeDetectorDescriptor::setOctaves(octaves);
    AkazeDetectorDescriptor::setOctaveLayers(octaveLayers);
    AkazeDetectorDescriptor::setDiffusivity(diffusivity);
}

auto AkazeDetectorDescriptor::operator =(const AkazeDetectorDescriptor &akazeDetectorDescriptor) -> AkazeDetectorDescriptor &
{
    if (this != &akazeDetectorDescriptor) {
        AkazeProperties::operator=(akazeDetectorDescriptor);
        this->initAkazeFromProperties();
    }

    return *this;
}

auto AkazeDetectorDescriptor::operator =(AkazeDetectorDescriptor &&akazeDetectorDescriptor) TL_NOEXCEPT -> AkazeDetectorDescriptor&
{
    if (this != &akazeDetectorDescriptor) {
        AkazeProperties::operator=(std::forward<AkazeProperties>(akazeDetectorDescriptor));
        this->initAkazeFromProperties();
    }

    return *this;
}

#if CV_VERSION_MAJOR >= 4
auto AkazeDetectorDescriptor::convertDescriptorType(const std::string &descriptorType) -> cv::AKAZE::DescriptorType
{
    cv::AKAZE::DescriptorType descriptor_type = cv::AKAZE::DescriptorType::DESCRIPTOR_MLDB;

    if (descriptorType == "KAZE") {
        descriptor_type = cv::AKAZE::DESCRIPTOR_KAZE;
    } else if (descriptorType == "KAZE_UPRIGHT") {
        descriptor_type = cv::AKAZE::DESCRIPTOR_KAZE_UPRIGHT;
    } else if (descriptorType == "MLDB") {
        descriptor_type = cv::AKAZE::DESCRIPTOR_MLDB;
    } else if (descriptorType == "MLDB_UPRIGHT") {
        descriptor_type = cv::AKAZE::DESCRIPTOR_MLDB_UPRIGHT;
    }

    return descriptor_type;
}

auto AkazeDetectorDescriptor::convertDiffusivity(const std::string &diffusivity) -> cv::KAZE::DiffusivityType
{
    cv::KAZE::DiffusivityType diff = cv::KAZE::DiffusivityType::DIFF_PM_G1;

    if (diffusivity == "DIFF_PM_G1") {
        diff = cv::KAZE::DIFF_PM_G1;
    } else if (diffusivity == "DIFF_PM_G2") {
        diff = cv::KAZE::DIFF_PM_G2;
    } else if (diffusivity == "DIFF_WEICKERT") {
        diff = cv::KAZE::DIFF_WEICKERT;
    } else if (diffusivity == "DIFF_CHARBONNIER") {
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

auto AkazeDetectorDescriptor::convertDescriptorType(const std::string &descriptorType) -> int
{
    int descriptor_type = cv::AKAZE::DESCRIPTOR_MLDB;

    if (descriptorType == "KAZE") {
        descriptor_type = cv::AKAZE::DESCRIPTOR_KAZE;
    } else if (descriptorType == "KAZE_UPRIGHT") {
        descriptor_type = cv::AKAZE::DESCRIPTOR_KAZE_UPRIGHT;
    } else if (descriptorType == "MLDB") {
        descriptor_type = cv::AKAZE::DESCRIPTOR_MLDB;
    } else if (descriptorType == "MLDB_UPRIGHT") {
        descriptor_type = cv::AKAZE::DESCRIPTOR_MLDB_UPRIGHT;
    }

    return descriptor_type;
}

auto AkazeDetectorDescriptor::convertDiffusivity(const std::string &diffusivity) -> int
{
    int diff = cv::KAZE::DIFF_PM_G1;

    if (diffusivity == "DIFF_PM_G1") {
        diff = cv::KAZE::DIFF_PM_G1;
    } else if (diffusivity == "DIFF_PM_G2") {
        diff = cv::KAZE::DIFF_PM_G2;
    } else if (diffusivity == "DIFF_WEICKERT") {
        diff = cv::KAZE::DIFF_WEICKERT;
    } else if (diffusivity == "DIFF_CHARBONNIER") == 0){
        diff = cv::KAZE::DIFF_CHARBONNIER;
  }

  return diff;
}
#endif

auto AkazeDetectorDescriptor::detect(const cv::Mat &img, cv::InputArray &mask) -> std::vector<cv::KeyPoint>
{
    std::vector<cv::KeyPoint> keyPoints;

    try {

        mAkaze->detect(img, keyPoints, mask);

    } catch (...) {
        TL_THROW_EXCEPTION_WITH_NESTED("Catched exception");
    }

    return keyPoints;
}


auto AkazeDetectorDescriptor::extract(const cv::Mat &img, std::vector<cv::KeyPoint> &keyPoints) -> cv::Mat
{
    cv::Mat descriptors;

    try {

        mAkaze->compute(img, keyPoints, descriptors);

    } catch (...) {
        TL_THROW_EXCEPTION_WITH_NESTED("Catched exception");
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
