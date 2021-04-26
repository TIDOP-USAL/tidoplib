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

#include "fast.h"

#include "tidop/core/messages.h"


namespace tl
{


FastProperties::FastProperties()
  : mDetectorType(fast_default_value_detector_type)
{

}

FastProperties::FastProperties(const FastProperties &fastProperties) = default;

int FastProperties::threshold() const
{
  return mThreshold;
}

bool FastProperties::nonmaxSuppression() const
{
  return mNonmaxSuppression;
}

std::string FastProperties::detectorType() const
{
  return mDetectorType;
}

void FastProperties::setThreshold(int threshold)
{
  mThreshold = threshold;
}

void FastProperties::setNonmaxSuppression(bool nonmaxSuppression)
{
  mNonmaxSuppression = nonmaxSuppression;
}

void FastProperties::setDetectorType(const std::string &detectorType)
{
  if (detectorType == "TYPE_5_8" ||
      detectorType == "TYPE_7_12" ||
      detectorType == "TYPE_9_16") {
    mDetectorType = detectorType;
  }
}

void FastProperties::reset()
{
  mThreshold = fast_default_value_threshold;
  mNonmaxSuppression = fast_default_value_nonmax_suppression;
  mDetectorType = fast_default_value_detector_type;
}

std::string FastProperties::name() const
{
  return std::string("FAST");
}


/*----------------------------------------------------------------*/


FastDetector::FastDetector()
{
  mFast = cv::FastFeatureDetector::create(FastProperties::threshold(),
                                          FastProperties::nonmaxSuppression(),
                                          convertDetectorType(FastProperties::detectorType()));
}

FastDetector::FastDetector(const FastDetector &fastDetector)
  : FastProperties(fastDetector),
    KeypointDetector(fastDetector)
{
  mFast = cv::FastFeatureDetector::create(FastProperties::threshold(),
                                          FastProperties::nonmaxSuppression(),
                                          convertDetectorType(FastProperties::detectorType()));
}

FastDetector::FastDetector(int threshold, 
                           bool nonmaxSuppression, 
                           const std::string &detectorType)
  : mFast(cv::FastFeatureDetector::create())
{
  this->setThreshold(threshold);
  this->setNonmaxSuppression(nonmaxSuppression);
  this->setDetectorType(detectorType);
}

#if CV_VERSION_MAJOR >= 4

cv::FastFeatureDetector::DetectorType FastDetector::convertDetectorType(const std::string &detectorType)
{
  cv::FastFeatureDetector::DetectorType type = cv::FastFeatureDetector::DetectorType::TYPE_9_16;

  if (detectorType == "TYPE_5_8"){
    type = cv::FastFeatureDetector::TYPE_5_8;
  } else if (detectorType == "TYPE_7_12") {
    type = cv::FastFeatureDetector::TYPE_7_12;
  } else if (detectorType == "TYPE_9_16") {
    type = cv::FastFeatureDetector::TYPE_9_16;
  }

  return type;
}

#else

int FastDetector::convertDetectorType(const std::string &detectorType)
{
  int type = cv::FastFeatureDetector::TYPE_9_16;

  if (detectorType == "TYPE_5_8"){
    type = cv::FastFeatureDetector::TYPE_5_8;
  } else if (detectorType == "TYPE_7_12") {
    type = cv::FastFeatureDetector::TYPE_7_12;
  } else if (detectorType == "TYPE_9_16") {
    type = cv::FastFeatureDetector::TYPE_9_16;
  }

  return type;
}
#endif

std::vector<cv::KeyPoint> FastDetector::detect(const cv::Mat &img, cv::InputArray &mask)
{
  std::vector<cv::KeyPoint> keyPoints;
  mFast->detect(img, keyPoints, mask);
  return keyPoints;
}

void FastDetector::setThreshold(int threshold)
{
  FastProperties::setThreshold(threshold);
  mFast->setThreshold(threshold);
}

void FastDetector::setNonmaxSuppression(bool nonmaxSuppression)
{
  FastProperties::setNonmaxSuppression(nonmaxSuppression);
  mFast->setNonmaxSuppression(nonmaxSuppression);
}

void FastDetector::setDetectorType(const std::string &detectorType)
{
  FastProperties::setDetectorType(detectorType);
  mFast->setType(convertDetectorType(detectorType));
}

void FastDetector::reset()
{
  FastProperties::reset();
  mFast->setThreshold(FastProperties::threshold());
  mFast->setNonmaxSuppression(FastProperties::nonmaxSuppression());
  mFast->setType(convertDetectorType(FastProperties::detectorType()));
}


/*----------------------------------------------------------------*/



FastDetectorCuda::FastDetectorCuda()
{
  this->update();
}

FastDetectorCuda::FastDetectorCuda(const FastDetectorCuda &fastDetector)
  : FastProperties(fastDetector),
    KeypointDetector(fastDetector)
{
  this->update();
}

FastDetectorCuda::FastDetectorCuda(int threshold,
                                   bool nonmaxSuppression,
                                   const std::string &detectorType)
{
  FastProperties::setThreshold(threshold);
  FastProperties::setNonmaxSuppression(nonmaxSuppression);
  FastProperties::setDetectorType(detectorType);
  this->update();
}

int FastDetectorCuda::convertDetectorType(const std::string &detectorType)
{
  int type = cv::FastFeatureDetector::TYPE_9_16;

  if (detectorType == "TYPE_5_8"){
    type = cv::FastFeatureDetector::TYPE_5_8;
  } else if (detectorType == "TYPE_7_12") {
    type = cv::FastFeatureDetector::TYPE_7_12;
  } else if (detectorType == "TYPE_9_16") {
    type = cv::FastFeatureDetector::TYPE_9_16;
  }

  return type;
}

void FastDetectorCuda::update()
{
#ifdef HAVE_OPENCV_CUDAFEATURES2D
  mFast = cv::cuda::FastFeatureDetector::create(FastProperties::threshold(),
                                                FastProperties::nonmaxSuppression(),
                                                convertDetectorType(FastProperties::detectorType()),
                                                10000);
#endif // HAVE_OPENCV_CUDAFEATURES2D
}

std::vector<cv::KeyPoint> FastDetectorCuda::detect(const cv::Mat &img, cv::InputArray &mask)
{
  std::vector<cv::KeyPoint> keyPoints;

#ifdef HAVE_OPENCV_CUDAFEATURES2D
  cv::cuda::GpuMat g_img(img);
  cv::cuda::GpuMat g_mask(mask);
  mFast->detect(g_img, keyPoints, g_mask);
#else
  TL_COMPILER_WARNING("OpenCV not built with CUDAFEATURES2D. Cuda Fast Detector not supported")
  throw std::exception("OpenCV not built with CUDAFEATURES2D. Cuda Fast Detector not supported");
#endif // HAVE_OPENCV_CUDAFEATURES2D

  return keyPoints;
}

void FastDetectorCuda::setThreshold(int threshold)
{
  FastProperties::setThreshold(threshold);
  update();
}

void FastDetectorCuda::setNonmaxSuppression(bool nonmaxSuppression)
{
  FastProperties::setNonmaxSuppression(nonmaxSuppression);
  update();
}

void FastDetectorCuda::setDetectorType(const std::string &detectorType)
{
  FastProperties::setDetectorType(detectorType);
  update();
}

void FastDetectorCuda::reset()
{
  FastProperties::reset();
  update();
}


} // namespace tl
