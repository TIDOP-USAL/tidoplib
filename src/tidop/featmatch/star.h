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

constexpr auto star_default_value_max_size{45};
constexpr auto star_default_value_response_threshold{30};
constexpr auto star_default_value_line_threshold_projected{10};
constexpr auto star_default_value_line_threshold_binarized{8};
constexpr auto star_default_value_suppress_nonmax_size{5};


class TL_EXPORT StarProperties
  : public Star
{

private:

    int mMaxSize{star_default_value_max_size};
    int mResponseThreshold{star_default_value_response_threshold};
    int mLineThresholdProjected{star_default_value_line_threshold_projected};
    int mLineThresholdBinarized{star_default_value_line_threshold_binarized};
    int mSuppressNonmaxSize{star_default_value_suppress_nonmax_size};

public:

    StarProperties();
    StarProperties(const StarProperties &starProperties);
    ~StarProperties() override = default;

// Star interface

public:

    auto maxSize() const -> int override;
    auto responseThreshold() const -> int override;
    auto lineThresholdProjected() const -> int override;
    auto lineThresholdBinarized() const -> int override;
    auto suppressNonmaxSize() const -> int override;
    void setMaxSize(int maxSize) override;
    void setResponseThreshold(int responseThreshold) override;
    void setLineThresholdProjected(int lineThresholdProjected) override;
    void setLineThresholdBinarized(int lineThresholdBinarized) override;
    void setSuppressNonmaxSize(int suppressNonmaxSize) override;

// Feature interface

public:

    void reset() override;
    auto name() const -> std::string final;

};



/*----------------------------------------------------------------*/


class TL_EXPORT StarDetector
  : public StarProperties,
    public KeypointDetector
{

private:

#ifdef HAVE_OPENCV_XFEATURES2D 
    cv::Ptr<cv::xfeatures2d::StarDetector> mSTAR;
#endif // HAVE_OPENCV_XFEATURES2D

public:

    StarDetector();
    StarDetector(const StarDetector &starDetector);
    StarDetector(int maxSize,
                 int responseThreshold,
                 int lineThresholdProjected,
                 int lineThresholdBinarized,
                 int suppressNonmaxSize);
    ~StarDetector() override = default;

private:

    void update();

// KeypointDetector interface

public:

    auto detect(const cv::Mat &img,
                cv::InputArray &mask = cv::noArray()) -> std::vector<cv::KeyPoint> override;

// Star interface

public:

    void setMaxSize(int maxSize) override;
    void setResponseThreshold(int responseThreshold) override;
    void setLineThresholdProjected(int lineThresholdProjected) override;
    void setLineThresholdBinarized(int lineThresholdBinarized) override;
    void setSuppressNonmaxSize(int suppressNonmaxSize) override;

// Feature interface

public:

    void reset() override;

};


/*! \} */ // end of FeatureDetectorAndDescriptor

/*! \} */ // end of Features

} // namespace tl
