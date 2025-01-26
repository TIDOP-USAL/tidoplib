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

#include "tidop/featmatch/base/features.h"

#ifdef HAVE_OPENCV_XFEATURES2D 
#include <opencv2/xfeatures2d.hpp>
#endif // HAVE_OPENCV_XFEATURES2D

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


class TL_EXPORT FreakProperties
  : public Freak
{

private:

    bool mOrientationNormalized{true};
    bool mScaleNormalized{true};
    double mPatternScale{22.};
    int mOctaves{4};

public:

    FreakProperties();
    FreakProperties(const FreakProperties &freakProperties);
    ~FreakProperties() override = default;

// Freak interface

public:

    auto orientationNormalized() const -> bool override;
    auto scaleNormalized() const -> bool override;
    auto patternScale() const -> double override;
    auto octaves() const -> int override;
    void setOrientationNormalized(bool orientationNormalized) override;
    void setScaleNormalized(bool scaleNormalized) override;
    void setPatternScale(double patternScale) override;
    void setOctaves(int octaves) override;

// Feature interface

public:

    void reset() override;
    auto name() const -> std::string final;

};


/*----------------------------------------------------------------*/


class TL_EXPORT FreakDescriptor
  : public FreakProperties,
    public FeatureDescriptor
{

private:

#ifdef HAVE_OPENCV_XFEATURES2D 
    cv::Ptr<cv::xfeatures2d::FREAK> mFREAK;
#endif // HAVE_OPENCV_XFEATURES2D

public:

    FreakDescriptor();
    FreakDescriptor(const FreakDescriptor &freakDescriptor);
    FreakDescriptor(bool orientationNormalized,
                    bool scaleNormalized,
                    double patternScale,
                    int octaves);
    ~FreakDescriptor() override = default;

private:

    void update();

// FeatureDescriptor interface

public:

    auto extract(const cv::Mat &img, std::vector<cv::KeyPoint> &keyPoints) -> cv::Mat override;

// Freak interface

public:

    void setOrientationNormalized(bool orientationNormalized) override;
    void setScaleNormalized(bool scaleNormalized) override;
    void setPatternScale(double patternScale) override;
    void setOctaves(int octaves) override;

// Feature interface

public:

    void reset() override;

};

/*! \} */ // end of FeatureDetectorAndDescriptor

/*! \} */ // end of Features

} // namespace tl
