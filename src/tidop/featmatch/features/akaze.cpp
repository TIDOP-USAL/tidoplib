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

#include "tidop/featmatch/features/akaze.h"

#include "tidop/core/base/exception.h"


namespace tl
{

/* AKAZE properties */

AkazeProperties::AkazeProperties()
  : Feature("AKAZE", Feature::Type::akaze)
{
    reset();
}

AkazeProperties::AkazeProperties(const AkazeProperties &properties)
  : Feature(properties)
{
}

AkazeProperties::AkazeProperties(AkazeProperties &&properties) TL_NOEXCEPT
  : Feature(std::forward<Feature>(properties))
{
}

AkazeProperties::~AkazeProperties() = default;

auto AkazeProperties::operator =(const AkazeProperties &properties) -> AkazeProperties&
{
    if (this != &properties) {
        Feature::operator=(properties);
    }

    return *this;
}

auto AkazeProperties::operator =(AkazeProperties &&properties) TL_NOEXCEPT -> AkazeProperties&
{
    if (this != &properties) {
        Feature::operator=(std::forward<Feature>(properties));
    }

    return *this;
}

auto AkazeProperties::descriptorType() const -> std::string
{
    return getProperty<std::string>("DescriptorType");
}

auto AkazeProperties::descriptorSize() const -> int
{
    return getProperty<int>("DescriptorSize");
}

auto AkazeProperties::descriptorChannels() const -> int
{
    return getProperty<int>("DescriptorChannels");
}

auto AkazeProperties::threshold() const -> double
{
    return getProperty<double>("Threshold");
}

auto AkazeProperties::octaves() const -> int
{
    return getProperty<int>("Octaves");
}

auto AkazeProperties::octaveLayers() const -> int
{
    return getProperty<int>("OctaveLayers");
}

auto AkazeProperties::diffusivity() const -> std::string
{
    return getProperty<std::string>("Diffusivity");
}

void AkazeProperties::setDescriptorType(const std::string &descriptorType)
{
    if (descriptorType == "KAZE" ||
        descriptorType == "KAZE_UPRIGHT" ||
        descriptorType == "MLDB" ||
        descriptorType == "MLDB_UPRIGHT") {
        setProperty("DescriptorType", descriptorType);
    } else {
        Message::warning("'DescriptorType' value not valid: {}", descriptorType);
    }
}

void AkazeProperties::setDescriptorSize(int descriptorSize)
{
    setProperty("DescriptorSize", descriptorSize);
}

void AkazeProperties::setDescriptorChannels(int channels)
{
    setProperty("DescriptorChannels", channels);
}

void AkazeProperties::setThreshold(double threshold)
{
    setProperty("Threshold", threshold);
}

void AkazeProperties::setOctaves(int octaves)
{
    setProperty("Octaves", octaves);
}

void AkazeProperties::setOctaveLayers(int octaveLayers)
{
    setProperty("OctaveLayers", octaveLayers);
}

void AkazeProperties::setDiffusivity(const std::string &diffusivity)
{
    if (diffusivity == "DIFF_PM_G1" ||
        diffusivity == "DIFF_PM_G2" ||
        diffusivity == "DIFF_WEICKERT" ||
        diffusivity == "DIFF_CHARBONNIER") {
        setProperty("Diffusivity", diffusivity);
    } else {
        Message::warning("'Diffusivity' value not valid: {}", diffusivity);
    }
}

void AkazeProperties::reset()
{
    setDescriptorType(akaze_default_value_descriptor_type);
    setDescriptorSize(akaze_default_value_descriptor_size);
    setDescriptorChannels(akaze_default_value_descriptor_channels);
    setThreshold(akaze_default_value_threshold);
    setOctaves(akaze_default_value_octaves);
    setOctaveLayers(akaze_default_value_octave_layers);
    setDiffusivity(akaze_default_value_diffusivity);
}



/* AGAST detector/descriptor */

AkazeDetectorDescriptor::AkazeDetectorDescriptor()
  : mProperties(),
    mAkaze(cv::AKAZE::create())
{
    this->initAkazeFromProperties();
}

AkazeDetectorDescriptor::AkazeDetectorDescriptor(const AkazeProperties &properties)
  : mProperties(properties),
    mAkaze(cv::AKAZE::create())
{
    this->initAkazeFromProperties();
}

AkazeDetectorDescriptor::AkazeDetectorDescriptor(const AkazeDetectorDescriptor &akaze)
  : mProperties(akaze.mProperties),
    mAkaze(cv::AKAZE::create())
{
    this->initAkazeFromProperties();
}

AkazeDetectorDescriptor::AkazeDetectorDescriptor(AkazeDetectorDescriptor &&akaze) TL_NOEXCEPT
  : mProperties(std::move(akaze.mProperties)),
    mAkaze(std::move(akaze.mAkaze))
{
}

auto AkazeDetectorDescriptor::operator =(const AkazeDetectorDescriptor &akaze) -> AkazeDetectorDescriptor &
{
    if (this != &akaze) {
        mProperties = akaze.mProperties;
        this->initAkazeFromProperties();
    }

    return *this;
}

auto AkazeDetectorDescriptor::operator =(AkazeDetectorDescriptor &&akaze) TL_NOEXCEPT -> AkazeDetectorDescriptor&
{
    if (this != &akaze) {
        mProperties = std::move(akaze.mProperties);
        mAkaze = std::move(akaze.mAkaze);
    }

    return *this;
}

void AkazeDetectorDescriptor::initAkazeFromProperties()
{
    mAkaze->setDescriptorChannels(mProperties.descriptorChannels());
    mAkaze->setDescriptorSize(mProperties.descriptorSize());
    mAkaze->setDescriptorType(convertDescriptorType(mProperties.descriptorType()));
    mAkaze->setDiffusivity(convertDiffusivity(mProperties.diffusivity()));
    mAkaze->setNOctaves(mProperties.octaves());
    mAkaze->setNOctaveLayers(mProperties.octaveLayers());
    mAkaze->setThreshold(mProperties.threshold());
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


} // namespace tl
