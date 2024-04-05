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

#ifdef HAVE_OPENCV_XFEATURES2D 
#include <opencv2/xfeatures2d.hpp>
#endif // HAVE_OPENCV_XFEATURES2D
#ifdef HAVE_OPENCV_CUDAFEATURES2D
#include <opencv2/cudafeatures2d.hpp>
#include <opencv2/xfeatures2d/cuda.hpp>
#endif // HAVE_OPENCV_CUDAFEATURES2D

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

constexpr auto surf_default_value_hessian_threshold{100};
constexpr auto surf_default_value_octaves{4};
constexpr auto surf_default_value_octave_layers{3};
constexpr auto surf_default_value_extended_descriptor{false};
constexpr auto surf_default_value_upright{false};

class TL_EXPORT SurfProperties
  : public Surf
{

private:

    double mHessianThreshold{surf_default_value_hessian_threshold};
    int mOctaves{surf_default_value_octaves};
    int mOctaveLayers{surf_default_value_octave_layers};
    bool mExtendedDescriptor{surf_default_value_extended_descriptor};
    bool mUpright{surf_default_value_upright};

public:

    SurfProperties();
    SurfProperties(const SurfProperties &surfProperties);
    ~SurfProperties() override = default;

// Surf interface

public:

    auto hessianThreshold() const -> double override;
    auto octaves() const -> int override;
    auto octaveLayers() const -> int override;
    auto extendedDescriptor() const -> bool override;
    auto upright() const -> bool override;
    void setHessianThreshold(double hessianThreshold) override;
    void setOctaves(int octaves) override;
    void setOctaveLayers(int octaveLayers) override;
    void setExtendedDescriptor(bool extendedDescriptor) override;
    void setUpright(bool upright) override;

// Feature interface

public:

    void reset() override;
    auto name() const -> std::string final;

};


/*----------------------------------------------------------------*/


class TL_EXPORT SurfDetectorDescriptor
  : public SurfProperties,
    public KeypointDetector,
    public DescriptorExtractor
{

private:

#ifdef HAVE_OPENCV_XFEATURES2D 
  cv::Ptr<cv::xfeatures2d::SURF> mSurf;
#endif // HAVE_OPENCV_XFEATURES2D

public:

    SurfDetectorDescriptor();
    SurfDetectorDescriptor(const SurfDetectorDescriptor &surfDetectorDescriptor);
    SurfDetectorDescriptor(double hessianThreshold,
                           int octaves,
                           int octaveLayers,
                           bool extendedDescriptor,
                           bool upright);

    ~SurfDetectorDescriptor() override = default;

// KeypointDetector interface

public:

    auto detect(const cv::Mat &img,
                cv::InputArray &mask = cv::noArray()) -> std::vector<cv::KeyPoint> override;

// DescriptorExtractor interface

public:

    auto extract(const cv::Mat &img,
                 std::vector<cv::KeyPoint> &keyPoints) -> cv::Mat override;

// Surf interface

public:

    void setHessianThreshold(double hessianThreshold) override;
    void setOctaves(int octaves) override;
    void setOctaveLayers(int octaveLayers) override;
    void setExtendedDescriptor(bool extendedDescriptor) override;
    void setUpright(bool upright) override;

// Feature interface

public:

    void reset() override;

};


/*----------------------------------------------------------------*/



class TL_EXPORT SurfCudaDetectorDescriptor
  : public SurfProperties,
    public KeypointDetector,
    public DescriptorExtractor
{

protected:

#if defined HAVE_OPENCV_XFEATURES2D && defined HAVE_OPENCV_CUDAFEATURES2D 
    std::unique_ptr<cv::cuda::SURF_CUDA> mSurf;
#endif

public:

    SurfCudaDetectorDescriptor();
    SurfCudaDetectorDescriptor(const SurfCudaDetectorDescriptor &surfDetectorDescriptor);
    SurfCudaDetectorDescriptor(double hessianThreshold,
                               int octaves,
                               int octaveLayers,
                               bool extendedDescriptor,
                               bool upright);
    ~SurfCudaDetectorDescriptor() override = default;

// KeypointDetector interface

public:

    std::vector<cv::KeyPoint> detect(const cv::Mat &img,
                                     cv::InputArray &mask = cv::noArray()) override;

// DescriptorExtractor interface

public:

    cv::Mat extract(const cv::Mat &img,
                    std::vector<cv::KeyPoint> &keyPoints) override;

// Surf interface

public:

    void setHessianThreshold(double hessianThreshold) override;
    void setOctaves(int octaves) override;
    void setOctaveLayers(int octaveLayers) override;
    void setExtendedDescriptor(bool extendedDescriptor) override;
    void setUpright(bool upright) override;

// Feature interface

public:

    void reset() override;

};


/*! \} */ // end of FeatureDetectorAndDescriptor

/*! \} */ // end of Features

} // namespace tl

