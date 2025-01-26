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


BriskProperties::BriskProperties() = default;

BriskProperties::BriskProperties(const BriskProperties &briskProperties) = default;

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
    mThreshold = brisk_default_value_threshold;
    mOctaves = brisk_default_value_octaves;
    mPatternScale = brisk_default_value_pattern_scale;
}

std::string BriskProperties::name() const
{
    return std::string("BRISK");
}


/*----------------------------------------------------------------*/


BriskDetectorDescriptor::BriskDetectorDescriptor()
{
    update();
}

BriskDetectorDescriptor::BriskDetectorDescriptor(const BriskDetectorDescriptor &briskDetectorDescriptor)
  : BriskProperties(briskDetectorDescriptor),
    FeatureDetector(briskDetectorDescriptor),
    FeatureDescriptor(briskDetectorDescriptor)
{
    update();
}


BriskDetectorDescriptor::BriskDetectorDescriptor(int threshold,
                                                 int octaves,
                                                 double patternScale)
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
