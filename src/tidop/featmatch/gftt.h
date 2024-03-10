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


class TL_EXPORT GfttProperties
  : public Gftt
{

private:

    int mMaxFeatures{1000};
    double mQualityLevel{0.01};
    double mMinDistance{1};
    int mBlockSize{3};
    bool mHarrisDetector{false};
    double mK{0.04};

public:

    GfttProperties();
    GfttProperties(const GfttProperties &gfttProperties);
    ~GfttProperties() override = default;

// Gftt interface

public:

    auto maxFeatures() const -> int override;
    auto qualityLevel() const -> double override;
    auto minDistance() const -> double override;
    auto blockSize() const -> int override;
    auto harrisDetector() const -> bool override;
    auto k() const -> double override;
    void setMaxFeatures(int maxFeatures) override;
    void setQualityLevel(double qlevel) override;
    void setMinDistance(double minDistance) override;
    void setBlockSize(int blockSize) override;
    void setHarrisDetector(bool value) override;
    void setK(double k) override;

// Feature interface

public:

    void reset() override;
    auto name() const -> std::string final;

};


/*----------------------------------------------------------------*/


class TL_EXPORT GfttDetector
  : public GfttProperties,
    public KeypointDetector
{

private:

    cv::Ptr<cv::GFTTDetector> mGFTT;

public:

    GfttDetector();
    GfttDetector(const GfttDetector &gfttDetector);
    GfttDetector(int maxFeatures,
                 double qualityLevel,
                 double minDistance,
                 int blockSize,
                 bool harrisDetector,
                 double k);
    ~GfttDetector() override = default;

// KeypointDetector interface

public:

    auto detect(const cv::Mat &img, cv::InputArray &mask = cv::noArray()) -> std::vector<cv::KeyPoint> override;

// Gftt interface

public:

    void setMaxFeatures(int maxFeatures) override;
    void setQualityLevel(double qlevel) override;
    void setMinDistance(double minDistance) override;
    void setBlockSize(int blockSize) override;
    void setHarrisDetector(bool value) override;
    void setK(double k) override;

// Feature interface

public:

    void reset() override;

};

/*! \} */ // end of FeatureDetectorAndDescriptor

/*! \} */ // end of Features

} // namespace tl
