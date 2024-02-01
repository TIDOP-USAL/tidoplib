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

#pragma once

#include "tidop/featmatch/features.h"

namespace tl
{

/*! \addtogroup Features
 * 
 *  \{
 */

/*! \addtogroup FeatureDetectorAndDescriptor
 * 
 *  \{
 */

constexpr auto akaze_default_value_descriptor_type{"MLDB"};
constexpr auto akaze_default_value_descriptor_size{0};
constexpr auto akaze_default_value_descriptor_channels{3};
constexpr auto akaze_default_value_threshold{0.001};
constexpr auto akaze_default_value_octaves{4};
constexpr auto akaze_default_value_octave_layers{4};
constexpr auto akaze_default_value_diffusivity{"DIFF_PM_G2"};


class TL_EXPORT AkazeProperties
    : public Akaze
{

private:

    std::string mDescriptorType;
    int mDescriptorSize{akaze_default_value_descriptor_size};
    int mDescriptorChannels{akaze_default_value_descriptor_channels};
    double mThreshold{akaze_default_value_threshold};
    int mOctaves{akaze_default_value_octaves};
    int mOctaveLayers{akaze_default_value_octave_layers};
    std::string mDiffusivity;

public:

    AkazeProperties();
    AkazeProperties(const AkazeProperties &akazeProperties);
    AkazeProperties(AkazeProperties &&akazeProperties) TL_NOEXCEPT;
    ~AkazeProperties() override;

    auto operator =(const AkazeProperties &akazeProperties) -> AkazeProperties&;
    auto operator =(AkazeProperties &&akazeProperties) TL_NOEXCEPT -> AkazeProperties&;

// Akaze interface

public:

    auto descriptorType() const -> std::string override;
    auto descriptorSize() const -> int override;
    auto descriptorChannels() const -> int override;
    auto threshold() const -> double override;
    auto octaves() const -> int override;
    auto octaveLayers() const -> int override;
    auto diffusivity() const -> std::string override;
    void setDescriptorType(const std::string &descriptorType) override;
    void setDescriptorSize(int descriptorSize) override;
    void setDescriptorChannels(int channels) override;
    void setThreshold(double threshold) override;
    void setOctaves(int octaves) override;
    void setOctaveLayers(int octaveLayers) override;
    void setDiffusivity(const std::string &diffusivity) override;

// Feature interface

public:

    void reset() override;
    auto name() const -> std::string final;

};


/*----------------------------------------------------------------*/


class TL_EXPORT AkazeDetectorDescriptor
  : public AkazeProperties,
    public KeypointDetector,
    public DescriptorExtractor
{

private:

    cv::Ptr<cv::AKAZE> mAkaze;

public:

    AkazeDetectorDescriptor();
    AkazeDetectorDescriptor(const AkazeDetectorDescriptor &akazeDetectorDescriptor);
    AkazeDetectorDescriptor(AkazeDetectorDescriptor &&akazeDetectorDescriptor) TL_NOEXCEPT;
    AkazeDetectorDescriptor(const std::string &descriptorType,
                            int descriptorSize,
                            int descriptorChannels,
                            double threshold,
                            int octaves,
                            int octaveLayers,
                            const std::string &diffusivity);

    ~AkazeDetectorDescriptor() override = default;

    auto operator =(const AkazeDetectorDescriptor &akazeDetectorDescriptor) -> AkazeDetectorDescriptor &;
    auto operator =(AkazeDetectorDescriptor &&akazeDetectorDescriptor) TL_NOEXCEPT -> AkazeDetectorDescriptor &;

private:

#if CV_VERSION_MAJOR >= 4
    auto convertDescriptorType(const std::string &descriptorType) -> cv::AKAZE::DescriptorType;
    auto convertDiffusivity(const std::string &diffusivity) -> cv::KAZE::DiffusivityType;
#else
    auto convertDescriptorType(const std::string &descriptorType) -> int;
    auto convertDiffusivity(const std::string &diffusivity) -> int;
#endif

    void initAkazeFromProperties();

// KeypointDetector interface

public:

    auto detect(const cv::Mat &img, cv::InputArray &mask = cv::noArray()) -> std::vector<cv::KeyPoint> override;

// DescriptorExtractor interface

public:

    auto extract(const cv::Mat &img, std::vector<cv::KeyPoint> &keyPoints) -> cv::Mat override;

// Akaze interface

public:

    void setDescriptorType(const std::string &descriptorType) override;
    void setDescriptorSize(int descriptorSize) override;
    void setDescriptorChannels(int channels) override;
    void setThreshold(double threshold) override;
    void setOctaves(int octaves) override;
    void setOctaveLayers(int octaveLayers) override;
    void setDiffusivity(const std::string &diffusivity) override;

// Feature interface

public:

    void reset() override;

};

/*! \} */ // end of FeatureDetectorAndDescriptor

/*! \} */ // end of Features

} // namespace tl

