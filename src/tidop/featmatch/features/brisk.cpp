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

#include "brisk.h"

#include "tidop/core/base/exception.h"


namespace tl
{


/* BRISK properties */

BriskProperties::BriskProperties()
  : Feature("BRISK", Feature::Type::brisk)
{
    reset();
}

BriskProperties::BriskProperties(const BriskProperties &properties)
  : Feature(properties)
{

}

BriskProperties::BriskProperties(BriskProperties &&properties) TL_NOEXCEPT
  : Feature(std::forward<Feature>(properties))
{

}

auto BriskProperties::operator=(const BriskProperties &properties) -> BriskProperties &
{
    if (this != &properties) {
        Feature::operator=(properties);
    }
    return *this;
}

auto BriskProperties::operator=(BriskProperties &&properties) TL_NOEXCEPT -> BriskProperties &
{
    if (this != &properties) {
        Feature::operator=(std::forward<Feature>(properties));
    }
    return *this;
}

auto BriskProperties::threshold() const -> int
{
    return getProperty<int>("Threshold");
}

auto BriskProperties::octaves() const -> int
{
    return getProperty<int>("Octaves");
}

auto BriskProperties::patternScale() const -> float
{
    return getProperty<float>("PatternScale");
}

void BriskProperties::setThreshold(int threshold)
{
    setProperty("Threshold", threshold);
}

void BriskProperties::setOctaves(int octaves)
{
    setProperty("Octaves", octaves);
}

void BriskProperties::setPatternScale(float patternScale)
{
    setProperty("PatternScale", patternScale);
}

void BriskProperties::reset()
{
    setThreshold(brisk_default_value_threshold);
    setOctaves(brisk_default_value_octaves);
    setPatternScale(brisk_default_value_pattern_scale);
}



/* BRISK Detector/Descriptor */

BriskDetectorDescriptor::BriskDetectorDescriptor()
  : mProperties()
{
    init();
}

BriskDetectorDescriptor::BriskDetectorDescriptor(const BriskProperties &properties)
  : mProperties(properties)
{
    init();
}

BriskDetectorDescriptor::BriskDetectorDescriptor(const BriskDetectorDescriptor &brisk)
  : mProperties(brisk.mProperties)
{
    init();
}

BriskDetectorDescriptor::BriskDetectorDescriptor(BriskDetectorDescriptor &&brisk) TL_NOEXCEPT
  : mProperties(std::move(brisk.mProperties)),
    mBrisk(std::move(brisk.mBrisk))
{
}


auto BriskDetectorDescriptor::operator=(const BriskDetectorDescriptor &brisk) -> BriskDetectorDescriptor &
{
    if (this != &brisk) {
        mProperties = brisk.mProperties;
        init();
    }
    return *this;
}

auto BriskDetectorDescriptor::operator=(BriskDetectorDescriptor &&brisk) TL_NOEXCEPT -> BriskDetectorDescriptor &
{
    if (this != &brisk) {
        mProperties = std::move(brisk.mProperties);
        mBrisk = std::move(brisk.mBrisk);
    }
    return *this;
}


void BriskDetectorDescriptor::init()
{
    mBrisk = cv::BRISK::create(mProperties.threshold(),
                               mProperties.octaves(),
                               mProperties.patternScale());
}

std::vector<cv::KeyPoint> BriskDetectorDescriptor::detect(const cv::Mat &img,
                                                          cv::InputArray &mask)
{
    std::vector<cv::KeyPoint> keyPoints;

    try {

        mBrisk->detect(img, keyPoints, mask);

    } catch (...) {
        TL_THROW_EXCEPTION_WITH_NESTED("Catched exception");
    }

    return keyPoints;
}

cv::Mat BriskDetectorDescriptor::extract(const cv::Mat &img,
                                         std::vector<cv::KeyPoint> &keyPoints)
{
    cv::Mat descriptors;

    try {

        mBrisk->compute(img, keyPoints, descriptors);

    } catch (...) {
        TL_THROW_EXCEPTION_WITH_NESTED("Catched exception");
    }

    return descriptors;
}


} // namespace tl
