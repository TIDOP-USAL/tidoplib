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


constexpr auto agast_default_value_threshold{10};
constexpr auto agast_default_value_nonmax_suppression{true};
constexpr auto agast_default_value_detector_type{"OAST_9_16"};


class TL_EXPORT AgastProperties
    : public Agast
{

private:

    int mThreshold;
    bool mNonmaxSuppression;
    std::string mDetectorType;

public:

    AgastProperties();
    AgastProperties(const AgastProperties &agast);
    AgastProperties(AgastProperties &&agast) TL_NOEXCEPT;
    ~AgastProperties() override;

    auto operator =(const AgastProperties &agast) -> AgastProperties&;
    auto operator =(AgastProperties &&agast) TL_NOEXCEPT  -> AgastProperties&;

// Agast interface

public:

    auto threshold() const -> int override;
    auto nonmaxSuppression() const -> bool override;
    auto detectorType() const -> std::string override;
    void setThreshold(int threshold) override;
    void setNonmaxSuppression(bool nonmaxSuppression) override;
    void setDetectorType(const std::string &detectorType) override;

// Feature interface

public:

    void reset() override;
    auto name() const -> std::string final;

};


/*----------------------------------------------------------------*/


class TL_EXPORT AgastDetector
  : public AgastProperties,
    public KeypointDetector
{

private:

    cv::Ptr<cv::AgastFeatureDetector> mAgast;

public:

    AgastDetector();
    AgastDetector(const AgastDetector &agastDetector);
    AgastDetector(AgastDetector &&agastDetector) TL_NOEXCEPT;
    AgastDetector(int threshold,
                  bool nonmaxSuppression,
                  const std::string &detectorType);
    ~AgastDetector() override;
    
    auto operator =(const AgastDetector &agastDetector) -> AgastDetector &;
    auto operator =(AgastDetector &&agastDetector) TL_NOEXCEPT -> AgastDetector&;

private:

#if CV_VERSION_MAJOR >= 4
    auto convertDetectorType(const std::string &detectorType) -> cv::AgastFeatureDetector::DetectorType;
#else
    auto convertDetectorType(const std::string &detectorType) -> int;
#endif

    void initAgastFromProperties();

// KeypointDetector interface

public:

    auto detect(const cv::Mat &img,
                cv::InputArray &mask = cv::noArray()) -> std::vector<cv::KeyPoint> override;

// Agast interface

public:

    void setThreshold(int threshold) override;
    void setNonmaxSuppression(bool nonmaxSuppression) override;
    void setDetectorType(const std::string &detectorType) override;

// Feature interface

public:

    void reset() override;

};

/*! \} */ // end of FeatureDetectorAndDescriptor

/*! \} */ // end of Features

} // namespace tl

