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

constexpr auto boost_default_value_use_orientation = true;
constexpr auto boost_default_value_scale_factor = 6.25;

/*! \addtogroup Features
 * 
 *  \{
 */

/*! \addtogroup FeatureDetectorAndDescriptor
 * 
 *  \{
 */


class TL_EXPORT BoostProperties
    : public Boost
{

private:

    std::string mDescriptorType;
    bool bUseOrientation{boost_default_value_use_orientation};
    double mScaleFactor{boost_default_value_scale_factor};

public:

    BoostProperties();
    BoostProperties(const BoostProperties &boostProperties);
    BoostProperties(BoostProperties &&boostProperties) TL_NOEXCEPT;
    ~BoostProperties() override;

    auto operator =(const BoostProperties &boostProperties) -> BoostProperties&;
    auto operator =(BoostProperties &&boostProperties) TL_NOEXCEPT -> BoostProperties&;

// Boost interface

public:

    auto descriptorType() const -> std::string override;
    auto useOrientation() const -> bool override;
    auto scaleFactor() const -> double override;
    void setDescriptorType(const std::string &descriptorType) override;
    void setUseOrientation(bool useOrientation) override;
    void setScaleFactor(double scaleFactor) override;

// Feature interface

public:

    void reset() override;
    auto name() const -> std::string override;

};


/*----------------------------------------------------------------*/

class TL_EXPORT BoostDescriptor
  : public BoostProperties,
    public FeatureDescriptor
{

private:

#ifdef HAVE_OPENCV_XFEATURES2D 
#if CV_VERSION_MAJOR >= 4 || (CV_VERSION_MAJOR >= 3 && CV_VERSION_MINOR > 2)
    cv::Ptr<cv::xfeatures2d::BoostDesc> mBoost;
#endif
#endif // HAVE_OPENCV_XFEATURES2D

public:

    BoostDescriptor();
    BoostDescriptor(const BoostDescriptor &boostDescriptor);
    BoostDescriptor(BoostDescriptor &&boostDescriptor) TL_NOEXCEPT;
    BoostDescriptor(const std::string &descriptorType,
                    bool useOrientation,
                    double scaleFactor);
    ~BoostDescriptor() override = default;

    auto operator =(const BoostDescriptor &boostDescriptor) -> BoostDescriptor&;
    auto operator =(BoostDescriptor &&boostDescriptor) TL_NOEXCEPT -> BoostDescriptor&;

private:

    void update();

// Feature interface

public:

    void reset() override;

// Boost interface

public:

    void setDescriptorType(const std::string &descriptorType) override;
    void setUseOrientation(bool useOrientation) override;
    void setScaleFactor(double scaleFactor) override;

// FeatureDescriptor interface

public:

    auto extract(const cv::Mat &img, std::vector<cv::KeyPoint> &keyPoints) -> cv::Mat override;

};

/*! \} */ // end of FeatureDetectorAndDescriptor

/*! \} */ // end of Features

} // namespace tl
