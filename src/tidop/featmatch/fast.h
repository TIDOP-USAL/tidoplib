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

#ifndef TL_FEATMATCH_FAST_DETECTOR_H
#define TL_FEATMATCH_FAST_DETECTOR_H

#include "config_tl.h"

#include "tidop/featmatch/features.h"

#ifdef HAVE_OPENCV_CUDAFEATURES2D
#include <opencv2/cudafeatures2d.hpp>
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


class TL_EXPORT FastProperties
  : public Fast
{
public:

  FastProperties();
  FastProperties(const FastProperties &fastProperties);
  ~FastProperties() override = default;

// Fast interface

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

  int mThreshold;
  bool mNonmaxSuppression;
  std::string mDetectorType;
};


/*----------------------------------------------------------------*/


class TL_EXPORT FastDetector
  : public FastProperties,
    public KeypointDetector
{

public:

  FastDetector();
  FastDetector(const FastDetector &fastDetector);
  FastDetector(int threshold, 
               bool nonmaxSuppression, 
               const std::string &detectorType);
  ~FastDetector() override = default;

private:

#if CV_VERSION_MAJOR >= 4
  cv::FastFeatureDetector::DetectorType convertDetectorType(const std::string &detectorType);
#else
  int convertDetectorType(const std::string &detectorType);
#endif

// KeypointDetector interface

public:

  std::vector<cv::KeyPoint> detect(const cv::Mat &img,
                                   cv::InputArray &mask = cv::noArray()) override;

// Fast interface

public:

  void setThreshold(int threshold) override;
  void setNonmaxSuppression(bool nonmaxSuppression) override;
  void setDetectorType(const std::string &detectorType) override;

// Feature interface

public:

  void reset() override;

protected:

  cv::Ptr<cv::FastFeatureDetector> mFast;

};


/*----------------------------------------------------------------*/


class TL_EXPORT FastDetectorCuda
  : public FastProperties,
    public KeypointDetector
{

public:

  FastDetectorCuda();
  FastDetectorCuda(const FastDetector &fastDetector);
  FastDetectorCuda(int threshold, 
                   bool nonmaxSuppression, 
                   const std::string &detectorType);
  ~FastDetectorCuda() override = default;

private:

  int convertDetectorType(const std::string &detectorType);
  
  void update();
  
// KeypointDetector interface

public:

  std::vector<cv::KeyPoint> detect(const cv::Mat &img,
                                   cv::InputArray &mask = cv::noArray()) override;

// Fast interface

public:

  void setThreshold(int threshold) override;
  void setNonmaxSuppression(bool nonmaxSuppression) override;
  void setDetectorType(const std::string &detectorType) override;

// Feature interface

public:

  void reset() override;

protected:

#ifdef HAVE_OPENCV_CUDAFEATURES2D
  cv::Ptr<cv::cuda::FastFeatureDetector> mFast;
#endif // HAVE_OPENCV_CUDAFEATURES2D

};



/*! \} */ // end of FeatureDetectorAndDescriptor

/*! \} */ // end of Features

} // namespace tl

#endif // TL_FEATMATCH_AGAST_DETECTOR_H
