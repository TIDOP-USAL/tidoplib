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


class TL_EXPORT MserProperties
  : public Mser
{

private:

    int mDelta;
    int mMinArea;
    int mMaxArea;
    double mMaxVariation;
    double mMinDiversity;
    int mMaxEvolution;
    double mAreaThreshold;
    double mMinMargin;
    int mEdgeBlurSize;

public:

    MserProperties();
    MserProperties(const MserProperties &mserProperties);
    ~MserProperties() override = default;

// Mser interface

public:

    auto delta() const -> int override;
    auto minArea() const -> int override;
    auto maxArea() const -> int override;
    auto maxVariation() const -> double override;
    auto minDiversity() const -> double override;
    auto maxEvolution() const -> int override;
    auto areaThreshold() const -> double override;
    auto minMargin() const -> double override;
    auto edgeBlurSize() const -> int override;
    void setDelta(int delta) override;
    void setMinArea(int minArea) override;
    void setMaxArea(int maxArea) override;
    void setMaxVariation(double maxVariation) override;
    void setMinDiversity(double minDiversity) override;
    void setMaxEvolution(int maxEvolution) override;
    void setAreaThreshold(double areaThreshold) override;
    void setMinMargin(double minMargin) override;
    void setEdgeBlurSize(int edgeBlurSize) override;

// Feature interface

public:

    void reset() override;
    auto name() const -> std::string final;

};


/*----------------------------------------------------------------*/


class TL_EXPORT MserDetector
  : public MserProperties,
    public KeypointDetector
{

protected:

    cv::Ptr<cv::MSER> mMSER;

public:

    MserDetector();
    MserDetector(const MserDetector &mserDetector);
    MserDetector(int delta,
                 int minArea,
                 int maxArea,
                 double maxVariation,
                 double minDiversity,
                 int maxEvolution,
                 double areaThreshold,
                 double minMargin,
                 int edgeBlurSize);
    ~MserDetector() override = default;

private:

    void update();

// KeypointDetector interface

public:

    auto detect(const cv::Mat &img, 
                cv::InputArray &mask = cv::noArray()) -> std::vector<cv::KeyPoint> override;

// Mser interface

public:

    void setDelta(int delta) override;
    void setMinArea(int minArea) override;
    void setMaxArea(int maxArea) override;
    void setMaxVariation(double maxVariation) override;
    void setMinDiversity(double minDiversity) override;
    void setMaxEvolution(int maxEvolution) override;
    void setAreaThreshold(double areaThreshold) override;
    void setMinMargin(double minMargin) override;
    void setEdgeBlurSize(int edgeBlurSize) override;

// Feature interface

public:

    void reset() override;

};

/*! \} */ // end of FeatureDetectorAndDescriptor

/*! \} */ // end of Features

} // namespace tl
