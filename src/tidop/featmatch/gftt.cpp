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

#include "gftt.h"

#include "tidop/core/exception.h"


namespace tl
{

GfttProperties::GfttProperties()
{
}

GfttProperties::GfttProperties(const GfttProperties &gfttProperties)
  : Gftt(gfttProperties),
    mMaxFeatures(gfttProperties.mMaxFeatures),
    mQualityLevel(gfttProperties.mQualityLevel),
    mMinDistance(gfttProperties.mMinDistance),
    mBlockSize(gfttProperties.mBlockSize),
    mHarrisDetector(gfttProperties.mHarrisDetector),
    mK(gfttProperties.mK)
{
}

auto GfttProperties::maxFeatures() const -> int
{
    return mMaxFeatures;
}

auto GfttProperties::qualityLevel() const -> double
{
    return mQualityLevel;
}

auto GfttProperties::minDistance() const -> double
{
    return mMinDistance;
}

auto GfttProperties::blockSize() const -> int
{
    return mBlockSize;
}

auto GfttProperties::harrisDetector() const -> bool
{
    return mHarrisDetector;
}

auto GfttProperties::k() const -> double
{
    return mK;
}

void GfttProperties::setMaxFeatures(int maxFeatures)
{
    mMaxFeatures = maxFeatures;
}

void GfttProperties::setQualityLevel(double qlevel)
{
    mQualityLevel = qlevel;
}

void GfttProperties::setMinDistance(double minDistance)
{
    mMinDistance = minDistance;
}

void GfttProperties::setBlockSize(int blockSize)
{
    mBlockSize = blockSize;
}

void GfttProperties::setHarrisDetector(bool value)
{
    mHarrisDetector = value;
}

void GfttProperties::setK(double k)
{
    mK = k;
}

void GfttProperties::reset()
{
    mMaxFeatures = 1000;
    mQualityLevel = 0.01;
    mMinDistance = 1;
    mBlockSize = 3;
    mHarrisDetector = false;
    mK = 0.04;
}

auto GfttProperties::name() const -> std::string
{
    return std::string("GFTT");
}


/*----------------------------------------------------------------*/


GfttDetector::GfttDetector()
{
    mGFTT = cv::GFTTDetector::create(GfttProperties::maxFeatures(),
                                     GfttProperties::qualityLevel(),
                                     GfttProperties::minDistance(),
                                     GfttProperties::blockSize(),
                                     GfttProperties::harrisDetector(),
                                     GfttProperties::k());
}

GfttDetector::GfttDetector(const GfttDetector &gfttDetector)
    : GfttProperties(gfttDetector),
    KeypointDetector(gfttDetector)
{
    mGFTT = cv::GFTTDetector::create(GfttProperties::maxFeatures(),
                                     GfttProperties::qualityLevel(),
                                     GfttProperties::minDistance(),
                                     GfttProperties::blockSize(),
                                     GfttProperties::harrisDetector(),
                                     GfttProperties::k());
}

GfttDetector::GfttDetector(int maxFeatures,
                           double qualityLevel,
                           double minDistance,
                           int blockSize,
                           bool harrisDetector,
                           double k)
    : mGFTT(cv::GFTTDetector::create())
{
	GfttDetector::setMaxFeatures(maxFeatures);
	GfttDetector::setQualityLevel(qualityLevel);
	GfttDetector::setMinDistance(minDistance);
	GfttDetector::setBlockSize(blockSize);
	GfttDetector::setHarrisDetector(harrisDetector);
	GfttDetector::setK(k);
}

auto GfttDetector::detect(const cv::Mat &img, cv::InputArray &mask) -> std::vector<cv::KeyPoint>
{
    std::vector<cv::KeyPoint> key_points;

    try {

        mGFTT->detect(img, key_points, mask);

    } catch (...) {
        TL_THROW_EXCEPTION_WITH_NESTED("Catched exception");
    }

    return key_points;
}

void GfttDetector::setMaxFeatures(int maxFeatures)
{
    GfttProperties::setMaxFeatures(maxFeatures);
    mGFTT->setMaxFeatures(maxFeatures);
}

void GfttDetector::setQualityLevel(double qlevel)
{
    GfttProperties::setQualityLevel(qlevel);
    mGFTT->setQualityLevel(qlevel);
}

void GfttDetector::setMinDistance(double minDistance)
{
    GfttProperties::setMinDistance(minDistance);
    mGFTT->setMinDistance(minDistance);
}

void GfttDetector::setBlockSize(int blockSize)
{
    GfttProperties::setBlockSize(blockSize);
    mGFTT->setBlockSize(blockSize);
}

void GfttDetector::setHarrisDetector(bool value)
{
    GfttProperties::setHarrisDetector(value);
    mGFTT->setHarrisDetector(value);
}

void GfttDetector::setK(double k)
{
    GfttProperties::setK(k);
    mGFTT->setK(k);
}

void GfttDetector::reset()
{
    GfttProperties::reset();
    mGFTT->setMaxFeatures(GfttProperties::maxFeatures());
    mGFTT->setQualityLevel(GfttProperties::qualityLevel());
    mGFTT->setMinDistance(GfttProperties::minDistance());
    mGFTT->setBlockSize(GfttProperties::blockSize());
    mGFTT->setHarrisDetector(GfttProperties::harrisDetector());
    mGFTT->setK(GfttProperties::k());
}

} // namespace tl
