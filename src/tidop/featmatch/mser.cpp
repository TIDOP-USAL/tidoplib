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

#include "tidop/core/exception.h"

namespace tl
{


MserProperties::MserProperties()
  : mDelta(5),
    mMinArea(60),
    mMaxArea(14400),
    mMaxVariation(0.25),
    mMinDiversity(0.2),
    mMaxEvolution(200),
    mAreaThreshold(1.01),
    mMinMargin(0.003),
    mEdgeBlurSize(5)
{
}

MserProperties::MserProperties(const MserProperties &mserProperties)
  : Mser(mserProperties),
    mDelta(mserProperties.mDelta),
    mMinArea(mserProperties.minArea()),
    mMaxArea(mserProperties.mMaxArea),
    mMaxVariation(mserProperties.mMaxVariation),
    mMinDiversity(mserProperties.mMinDiversity),
    mMaxEvolution(mserProperties.mMaxEvolution),
    mAreaThreshold(mserProperties.mAreaThreshold),
    mMinMargin(mserProperties.mMinMargin),
    mEdgeBlurSize(mserProperties.mEdgeBlurSize)
{
}


auto MserProperties::delta() const -> int
{
    return mDelta;
}

auto MserProperties::minArea() const -> int
{
    return mMinArea;
}

auto MserProperties::maxArea() const -> int
{
    return mMaxArea;
}

auto MserProperties::maxVariation() const -> double
{
    return mMaxVariation;
}

auto MserProperties::minDiversity() const -> double
{
    return mMinDiversity;
}

auto MserProperties::maxEvolution() const -> int
{
    return mMaxEvolution;
}

auto MserProperties::areaThreshold() const -> double
{
    return mAreaThreshold;
}

auto MserProperties::minMargin() const -> double
{
    return mMinMargin;
}

auto MserProperties::edgeBlurSize() const -> int
{
    return mEdgeBlurSize;
}

void MserProperties::setDelta(int delta)
{
    mDelta = delta;
}

void MserProperties::setMinArea(int minArea)
{
    mMinArea = minArea;
}

void MserProperties::setMaxArea(int maxArea)
{
    mMaxArea = maxArea;
}

void MserProperties::setMaxVariation(double maxVariation)
{
    mMaxVariation = maxVariation;
}

void MserProperties::setMinDiversity(double minDiversity)
{
    mMinDiversity = minDiversity;
}

void MserProperties::setMaxEvolution(int maxEvolution)
{
    mMaxEvolution = maxEvolution;
}

void MserProperties::setAreaThreshold(double areaThreshold)
{
    mAreaThreshold = areaThreshold;
}

void MserProperties::setMinMargin(double minMargin)
{
    mMinMargin = minMargin;
}

void MserProperties::setEdgeBlurSize(int edgeBlurSize)
{
    mEdgeBlurSize = edgeBlurSize;
}

void MserProperties::reset()
{
    mDelta = 5;
    mMinArea = 60;
    mMaxArea = 14400;
    mMaxVariation = 0.25;
    mMinDiversity = .2;
    mMaxEvolution = 200;
    mAreaThreshold = 1.01;
    mMinMargin = 0.003;
    mEdgeBlurSize = 5;
}

auto MserProperties::name() const -> std::string
{
    return std::string("MSER");
}


/*----------------------------------------------------------------*/


MserDetector::MserDetector()
{
    update();
}

MserDetector::MserDetector(const MserDetector &mserDetector)
  : MserProperties(mserDetector),
    KeypointDetector(mserDetector)
{
    update();
}

MserDetector::MserDetector(int delta,
                           int minArea,
                           int maxArea,
                           double maxVariation,
                           double minDiversity,
                           int maxEvolution,
                           double areaThreshold,
                           double minMargin,
                           int edgeBlurSize)
{
    MserProperties::setDelta(delta);
    MserProperties::setMinArea(minArea);
    MserProperties::setMaxArea(maxArea);
    MserProperties::setMaxVariation(maxVariation);
    MserProperties::setMinDiversity(minDiversity);
    MserProperties::setMaxEvolution(maxEvolution);
    MserProperties::setAreaThreshold(areaThreshold);
    MserProperties::setMinMargin(minMargin);
    MserProperties::setEdgeBlurSize(edgeBlurSize);
    update();
}

void MserDetector::update()
{
    mMSER = cv::MSER::create(MserProperties::delta(),
                             MserProperties::minArea(),
                             MserProperties::maxArea(),
                             MserProperties::maxVariation(),
                             MserProperties::minDiversity(),
                             MserProperties::maxEvolution(),
                             MserProperties::areaThreshold(),
                             MserProperties::minMargin(),
                             MserProperties::edgeBlurSize());
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

void MserDetector::setDelta(int delta)
{
    MserProperties::setDelta(delta);
    mMSER->setDelta(delta);
}

void MserDetector::setMinArea(int minArea)
{
    MserProperties::setMinArea(minArea);
    mMSER->setMinArea(minArea);
}

void MserDetector::setMaxArea(int maxArea)
{
    MserProperties::setMaxArea(maxArea);
    mMSER->setMaxArea(maxArea);
}

void MserDetector::setMaxVariation(double maxVariation)
{
    MserProperties::setMaxVariation(maxVariation);
    update();
}

void MserDetector::setMinDiversity(double minDiversity)
{
    MserProperties::setMinDiversity(minDiversity);
    update();
}

void MserDetector::setMaxEvolution(int maxEvolution)
{
    MserProperties::setMaxEvolution(maxEvolution);
    update();
}

void MserDetector::setAreaThreshold(double areaThreshold)
{
    MserProperties::setAreaThreshold(areaThreshold);
    update();
}

void MserDetector::setMinMargin(double minMargin)
{
    MserProperties::setMinMargin(minMargin);
    update();
}

void MserDetector::setEdgeBlurSize(int edgeBlurSize)
{
    MserProperties::setEdgeBlurSize(edgeBlurSize);
    update();
}

void MserDetector::reset()
{
    MserProperties::reset();
    update();
}


} // namespace tl
