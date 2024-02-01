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

#if defined OPENCV_ENABLE_NONFREE && defined HAVE_OPENCV_XFEATURES2D
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


constexpr auto sift_default_value_features_number{5000};
constexpr auto sift_default_value_octave_layers{3};
constexpr auto sift_default_value_contrast_threshold{0.04};
constexpr auto sift_default_value_edge_threshold{10.};
constexpr auto sift_default_value_sigma{1.6};


class TL_EXPORT SiftProperties
    : public Sift
{

private:

    int mFeaturesNumber{sift_default_value_features_number};
    int mOctaveLayers{sift_default_value_octave_layers};
    double mContrastThreshold{sift_default_value_contrast_threshold};
    double mEdgeThreshold{sift_default_value_edge_threshold};
    double mSigma{sift_default_value_sigma};

public:

    SiftProperties();
    SiftProperties(const SiftProperties &siftProperties);
    ~SiftProperties() override = default;

// Sift interface

public:

    auto featuresNumber() const -> int override;
    auto octaveLayers() const -> int override;
    auto contrastThreshold() const -> double override;
    auto edgeThreshold() const -> double override;
    auto sigma() const -> double override;
    void setFeaturesNumber(int featuresNumber) override;
    void setOctaveLayers(int octaveLayers) override;
    void setContrastThreshold(double contrastThreshold) override;
    void setEdgeThreshold(double edgeThreshold) override;
    void setSigma(double sigma) override;

// Feature interface

public:

    void reset() override;
    auto name() const -> std::string final;

};


/*----------------------------------------------------------------*/



class TL_EXPORT SiftDetectorDescriptor
  : public SiftProperties,
    public KeypointDetector,
    public DescriptorExtractor
{

private:

#if (CV_VERSION_MAJOR > 4 || (CV_VERSION_MAJOR == 4 && CV_VERSION_MINOR >= 4))
    cv::Ptr <cv::SIFT>  mSift;
#elif defined OPENCV_ENABLE_NONFREE && defined HAVE_OPENCV_XFEATURES2D
    cv::Ptr<cv::xfeatures2d::SIFT> mSift;
#endif // HAVE_OPENCV_XFEATURES2D

public:

    SiftDetectorDescriptor();
    SiftDetectorDescriptor(const SiftDetectorDescriptor &siftDetectorDescriptor);
    SiftDetectorDescriptor(int featuresNumber,
                           int octaveLayers,
                           double contrastThreshold,
                           double edgeThreshold,
                           double sigma);
    ~SiftDetectorDescriptor() override = default;

private:

    void update();

// KeypointDetector interface

public:

    auto detect(const cv::Mat &img, cv::InputArray &mask = cv::noArray()) -> std::vector<cv::KeyPoint> override;

// DescriptorExtractor interface

public:

    auto extract(const cv::Mat &img, std::vector<cv::KeyPoint> &keyPoints) -> cv::Mat override;

// Sift interface

public:

    void setFeaturesNumber(int featuresNumber) override;
    void setOctaveLayers(int octaveLayers) override;
    void setContrastThreshold(double contrastThreshold) override;
    void setEdgeThreshold(double edgeThreshold) override;
    void setSigma(double sigma) override;

// Feature interface

public:

    void reset() override;

};

/*! \} */ // end of FeatureDetectorAndDescriptor

/*! \} */ // end of Features

} // namespace tl

