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


constexpr auto brisk_default_value_threshold{30};
constexpr auto brisk_default_value_octaves{3};
constexpr auto brisk_default_value_pattern_scale{1.0};


class TL_EXPORT BriskProperties
  : public Brisk
{


private:

    int mThreshold{brisk_default_value_threshold};
    int mOctaves{brisk_default_value_octaves};
    double mPatternScale{brisk_default_value_pattern_scale};

public:

    BriskProperties();
    BriskProperties(const BriskProperties &briskProperties);
    ~BriskProperties() override = default;

// Brisk interface

public:

    auto threshold() const -> int override;
    auto octaves() const -> int override;
    auto patternScale() const -> double override;
    void setThreshold(int threshold) override;
    void setOctaves(int octaves) override;
    void setPatternScale(double patternScale) override;

// Feature interface

public:

    void reset() override;
    auto name() const -> std::string final;

};


/*----------------------------------------------------------------*/


class TL_EXPORT BriskDetectorDescriptor
  : public BriskProperties,
    public KeypointDetector,
    public DescriptorExtractor
{

private:

    cv::Ptr<cv::BRISK> mBrisk;

public:

    BriskDetectorDescriptor();
    BriskDetectorDescriptor(const BriskDetectorDescriptor &briskDetectorDescriptor);
    BriskDetectorDescriptor(int threshold,
                            int octaves,
                            double patternScale);
    ~BriskDetectorDescriptor() override = default;

protected:

    void update();


// KeypointDetector interface

public:

    auto detect(const cv::Mat &img, cv::InputArray &mask = cv::noArray()) -> std::vector<cv::KeyPoint> override;

// DescriptorExtractor interface

public:

    auto extract(const cv::Mat &img, std::vector<cv::KeyPoint> &keyPoints) -> cv::Mat override;

// Brisk interface

public:

    void setThreshold(int threshold) override;
    void setOctaves(int octaves) override;
    void setPatternScale(double patternScale) override;

// Feature interface

public:

    void reset() override;

};

/*! \} */ // end of FeatureDetectorAndDescriptor

/*! \} */ // end of Features

} // namespace tl
