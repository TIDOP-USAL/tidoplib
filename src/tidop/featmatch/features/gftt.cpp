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

#include "tidop/core/base/exception.h"


namespace tl
{


/* GFTT properties */

GfttProperties::GfttProperties()
  : Feature("GFTT", Feature::Type::gftt)
{
    reset();
}

GfttProperties::GfttProperties(const GfttProperties &properties)
  : Feature(properties)
{
}

GfttProperties::GfttProperties(GfttProperties &&properties) TL_NOEXCEPT
  : Feature(std::forward<Feature>(properties))
{
}

auto GfttProperties::operator=(const GfttProperties &properties) -> GfttProperties &
{
    if (this != &properties) {
        Feature::operator=(properties);
    }
    return *this;
}

auto GfttProperties::operator=(GfttProperties &&properties) TL_NOEXCEPT -> GfttProperties &
{
    if (this != &properties) {
        Feature::operator=(std::forward<Feature>(properties));
    }
    return *this;
}

auto GfttProperties::maxFeatures() const -> int
{
    return getProperty<int>("MaxFeatures");
}

auto GfttProperties::qualityLevel() const -> double
{
    return getProperty<double>("QualityLevel");
}

auto GfttProperties::minDistance() const -> double
{
    return getProperty<double>("MinDistance");
}

auto GfttProperties::blockSize() const -> int
{
    return getProperty<int>("BlockSize");
}

auto GfttProperties::harrisDetector() const -> bool
{
    return getProperty<bool>("HarrisDetector");
}

auto GfttProperties::k() const -> double
{
    return getProperty<double>("K");
}

void GfttProperties::setMaxFeatures(int maxFeatures)
{
    setProperty("MaxFeatures", maxFeatures);
}

void GfttProperties::setQualityLevel(double qlevel)
{
    setProperty("QualityLevel", qlevel);
}

void GfttProperties::setMinDistance(double minDistance)
{
    setProperty("MinDistance", minDistance);
}

void GfttProperties::setBlockSize(int blockSize)
{
    setProperty("BlockSize", blockSize);
}

void GfttProperties::setHarrisDetector(bool value)
{
    setProperty("HarrisDetector", value);
}

void GfttProperties::setK(double k)
{
    setProperty("K", k);
}

void GfttProperties::reset()
{
    setMaxFeatures(1000);
    setQualityLevel(0.01);
    setMinDistance(1);
    setBlockSize(3);
    setHarrisDetector(false);
    setK(0.04);
}



/* GFTT detector */

GfttDetector::GfttDetector()
{
    init();
}

GfttDetector::GfttDetector(const GfttProperties &properties)
  : mProperties(properties)
{
    init();
}

GfttDetector::GfttDetector(const GfttDetector &gftt)
  : mProperties(gftt.mProperties)
{
    init();
}

GfttDetector::GfttDetector(GfttDetector &&gftt) TL_NOEXCEPT
  : mProperties(std::move(gftt.mProperties)),
    mGFTT(std::move(gftt.mGFTT))
{
}

auto GfttDetector::operator =(const GfttDetector &gftt) -> GfttDetector &
{
    if (this != &gftt) {
        mProperties = gftt.mProperties;
        init();
    }
    return *this;
}

auto GfttDetector::operator =(GfttDetector &&gftt) TL_NOEXCEPT -> GfttDetector &
{
    if (this != &gftt) {
        mProperties = std::move(gftt.mProperties);
        mGFTT = std::move(gftt.mGFTT);
    }
    return *this;
}

void GfttDetector::init()
{
    mGFTT = cv::GFTTDetector::create(mProperties.maxFeatures(),
                                     mProperties.qualityLevel(),
                                     mProperties.minDistance(),
                                     mProperties.blockSize(),
                                     mProperties.harrisDetector(),
                                     mProperties.k());
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

} // namespace tl
