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
 * License along with Foobar. If not, see <http://www.gnu.org/licenses/>. *
 *                                                                        *
 * @license LGPL-3.0 <https://www.gnu.org/licenses/lgpl-3.0.html>         *
 *                                                                        *
 **************************************************************************/

#ifndef TL_FEATMATCH_AGAST_DETECTOR_H
#define TL_FEATMATCH_AGAST_DETECTOR_H

#include "config_tl.h"

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

public:

  AgastProperties();
  AgastProperties(const AgastProperties &agast);
  AgastProperties(AgastProperties &&agast) TL_NOEXCEPT;
  ~AgastProperties() override;

  AgastProperties &operator =(const AgastProperties &agast);
  AgastProperties &operator =(AgastProperties &&agast) TL_NOEXCEPT;

// Agast interface

public:

  int threshold() const override;
  bool nonmaxSuppression() const override;
  std::string detectorType() const override;
  void setThreshold(int threshold) override;
  void setNonmaxSuppression(bool nonmaxSuppression) override;
  void setDetectorType(const std::string &detectorType) override;

// Feature interface

public:

  void reset() override;
  std::string name() const final;

private:

  int mThreshold{agast_default_value_threshold};
  bool mNonmaxSuppression{agast_default_value_nonmax_suppression};
  std::string mDetectorType;

};


/*----------------------------------------------------------------*/


class TL_EXPORT AgastDetector
  : public AgastProperties,
    public KeypointDetector
{

public:

  AgastDetector();
  AgastDetector(const AgastDetector &agastDetector);
  AgastDetector(AgastDetector &&agastDetector) TL_NOEXCEPT;
  AgastDetector(int threshold, 
                bool nonmaxSuppression, 
                const std::string &detectorType);
  ~AgastDetector() override;
  AgastDetector &operator =(const AgastDetector &agastDetector);
  AgastDetector &operator =(AgastDetector &&agastDetector) TL_NOEXCEPT;

private:

#if CV_VERSION_MAJOR >= 4
  cv::AgastFeatureDetector::DetectorType convertDetectorType(const std::string &detectorType);
#else
  int convertDetectorType(const std::string &detectorType);
#endif

  void initAgastFromProperties();

// KeypointDetector interface

public:

  std::vector<cv::KeyPoint> detect(const cv::Mat &img,
                                   cv::InputArray &mask = cv::noArray()) override;

// Agast interface

public:

  void setThreshold(int threshold) override;
  void setNonmaxSuppression(bool nonmaxSuppression) override;
  void setDetectorType(const std::string &detectorType) override;

// Feature interface

public:

  void reset() override;

private:

  cv::Ptr<cv::AgastFeatureDetector> mAgast;

};

/*! \} */ // end of FeatureDetectorAndDescriptor

/*! \} */ // end of Features

} // namespace tl


#endif // TL_FEATMATCH_AGAST_DETECTOR_H
