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

#include "mser.h"

#include "tidop/core/base/exception.h"

namespace tl
{

/* MSER properties */

MserProperties::MserProperties()
  : Feature("MSER", Feature::Type::mser)
{
    reset();
}

MserProperties::MserProperties(const MserProperties &properties)
  : Feature(properties)
{
}

MserProperties::MserProperties(MserProperties &&properties) TL_NOEXCEPT
  : Feature(std::forward<Feature>(properties))
{
}

auto MserProperties::operator=(const MserProperties &properties) -> MserProperties &
{
    if (this != &properties) {
        Feature::operator=(properties);
    }
    return *this;
}

auto MserProperties::operator=(MserProperties &&properties) TL_NOEXCEPT -> MserProperties &
{
    if (this != &properties) {
        Feature::operator=(std::forward<Feature>(properties));
    }
    return *this;
}


auto MserProperties::delta() const -> int
{
    return getProperty<int>("Delta");
}

auto MserProperties::minArea() const -> int
{
    return getProperty<int>("MinArea");
}

auto MserProperties::maxArea() const -> int
{
    return getProperty<int>("MaxArea");
}

auto MserProperties::maxVariation() const -> double
{
    return getProperty<double>("MaxVariation");
}

auto MserProperties::minDiversity() const -> double
{
    return getProperty<double>("MinDiversity");
}

auto MserProperties::maxEvolution() const -> int
{
    return getProperty<int>("MaxEvolution");
}

auto MserProperties::areaThreshold() const -> double
{
    return getProperty<double>("AreaThreshold");
}

auto MserProperties::minMargin() const -> double
{
    return getProperty<double>("MinMargin");
}

auto MserProperties::edgeBlurSize() const -> int
{
    return getProperty<int>("EdgeBlurSize");
}

void MserProperties::setDelta(int delta)
{
    setProperty("Delta", delta);
}

void MserProperties::setMinArea(int minArea)
{
    setProperty("MinArea", minArea);
}

void MserProperties::setMaxArea(int maxArea)
{
    setProperty("MaxArea", maxArea);
}

void MserProperties::setMaxVariation(double maxVariation)
{
    setProperty("MaxVariation", maxVariation);
}

void MserProperties::setMinDiversity(double minDiversity)
{
    setProperty("MinDiversity", minDiversity);
}

void MserProperties::setMaxEvolution(int maxEvolution)
{
    setProperty("MaxEvolution", maxEvolution);
}

void MserProperties::setAreaThreshold(double areaThreshold)
{
    setProperty("AreaThreshold", areaThreshold);
}

void MserProperties::setMinMargin(double minMargin)
{
    setProperty("MinMargin", minMargin);
}

void MserProperties::setEdgeBlurSize(int edgeBlurSize)
{
    setProperty("EdgeBlurSize", edgeBlurSize);
}

void MserProperties::reset()
{
     setDelta(5);
     setMinArea(60);
     setMaxArea(14400);
     setMaxVariation(0.25);
     setMinDiversity(.2);
     setMaxEvolution(200);
     setAreaThreshold(1.01);
     setMinMargin(0.003);
     setEdgeBlurSize(5);
}




/* MSER Detector */

MserDetector::MserDetector()
  : mProperties()
{
    init();
}

MserDetector::MserDetector(const MserProperties &properties)
  : mProperties(properties)
{
    init();
}

MserDetector::MserDetector(const MserDetector &mser)
  : mProperties(mser.mProperties)
{
    init();
}

MserDetector::MserDetector(MserDetector &&mser) TL_NOEXCEPT
  : mProperties(std::move(mser.mProperties)),
    mMSER(std::move(mser.mMSER))
{
}

auto MserDetector::operator =(const MserDetector &mser) -> MserDetector &
{
    if (this != &mser) {
        mProperties = mser.mProperties;
        init();
    }
    return *this;
}

auto MserDetector::operator =(MserDetector &&mser) TL_NOEXCEPT -> MserDetector &
{
    if (this != &mser) {
        mProperties = std::move(mser.mProperties);
        mMSER = std::move(mser.mMSER);
    }
    return *this;
}

void MserDetector::init()
{
    mMSER = cv::MSER::create(mProperties.delta(),
                             mProperties.minArea(),
                             mProperties.maxArea(),
                             mProperties.maxVariation(),
                             mProperties.minDiversity(),
                             mProperties.maxEvolution(),
                             mProperties.areaThreshold(),
                             mProperties.minMargin(),
                             mProperties.edgeBlurSize());
}

auto MserDetector::detect(const cv::Mat &img, cv::InputArray &mask) -> std::vector<cv::KeyPoint>
{
    std::vector<cv::KeyPoint> keyPoints;

    try {

        mMSER->detect(img, keyPoints, mask);

    } catch (...) {
        TL_THROW_EXCEPTION_WITH_NESTED("Catched exception");
    }

    return keyPoints;
}

} // namespace tl
