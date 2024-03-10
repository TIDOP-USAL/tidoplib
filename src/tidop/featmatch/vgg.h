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


constexpr auto vgg_default_value_descriptor_type{"VGG_120"};
constexpr auto vgg_default_value_scale_factor{6.25};
constexpr auto vgg_default_value_sigma{1.4};
constexpr auto vgg_default_value_use_normalize_descriptor{false};
constexpr auto vgg_default_value_use_normalize_image{true};
constexpr auto vgg_default_value_useScale_orientation{true};


class TL_EXPORT VggProperties
  : public Vgg
{

private:

    std::string mDescriptorType;
    double mScaleFactor{vgg_default_value_scale_factor};
    double mSigma{vgg_default_value_sigma};
    bool bUseNormalizeDescriptor{vgg_default_value_use_normalize_descriptor};
    bool bUseNormalizeImage{vgg_default_value_use_normalize_image};
    bool bUseScaleOrientation{vgg_default_value_useScale_orientation};

public:

    VggProperties();
    VggProperties(const VggProperties &vggProperties);
    ~VggProperties() override = default;

// Vgg interface

public:

    auto descriptorType() const -> std::string override;
    void setDescriptorType(const std::string &descriptorType) override;
    auto scaleFactor() const -> double override;
    void setScaleFactor(double scaleFactor) override;
    auto sigma() const -> double override;
    void setSigma(double sigma) override;
    auto useNormalizeDescriptor() const -> bool override;
    void setUseNormalizeDescriptor(bool useNormalizeDescriptor) override;
    auto useNormalizeImage() const -> bool override;
    void setUseNormalizeImage(bool useNormalizeImage) override;
    auto useScaleOrientation() const -> bool override;
    void setUseScaleOrientation(bool useScaleOrientation) override;

// Feature interface

public:

    void reset() override;
    auto name() const ->std::string override;

};



/*----------------------------------------------------------------*/


class TL_EXPORT VggDescriptor
  : public VggProperties,
    public DescriptorExtractor
{

private:

#ifdef HAVE_OPENCV_XFEATURES2D 
#if CV_VERSION_MAJOR >= 4 || (CV_VERSION_MAJOR >= 3 && CV_VERSION_MINOR > 2)
    cv::Ptr<cv::xfeatures2d::VGG> mVGG;
#endif
#endif // HAVE_OPENCV_XFEATURES2D

public:

    VggDescriptor();
    VggDescriptor(const VggDescriptor &vggDescriptor);
    VggDescriptor(const std::string &descriptorType,
                  double scaleFactor,
                  double sigma,
                  bool useNormalizeDescriptor,
                  bool useNormalizeImage,
                  bool useScaleOrientation);
    ~VggDescriptor() override = default;

private:

    void update();

// Feature interface

public:

    void reset() override;

// Vgg interface

public:

    void setDescriptorType(const std::string &descriptorType) override;
    void setScaleFactor(double scaleFactor) override;
    void setSigma(double sigma) override;
    void setUseNormalizeDescriptor(bool useNormalizeDescriptor) override;
    void setUseNormalizeImage(bool useNormalizeImage) override;
    void setUseScaleOrientation(bool useScaleOrientation) override;

// DescriptorExtractor interface

public:

    auto extract(const cv::Mat &img,
                 std::vector<cv::KeyPoint> &keyPoints) -> cv::Mat override;


};

/*! \} */ // end of FeatureDetectorAndDescriptor

/*! \} */ // end of Features

} // namespace tl
