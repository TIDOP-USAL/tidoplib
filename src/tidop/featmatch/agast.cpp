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

#include "agast.h"

#include "tidop/core/messages.h"


namespace tl
{


AgastProperties::AgastProperties()
  : mThreshold(10),
    mNonmaxSuppression(true),
    mDetectorType("OAST_9_16")
{
}

AgastProperties::AgastProperties(const AgastProperties &agast)
  : Agast(agast),
    mThreshold(agast.mThreshold),
    mNonmaxSuppression(agast.nonmaxSuppression()),
    mDetectorType(agast.mDetectorType)
{
}

AgastProperties::AgastProperties(AgastProperties &&agast) noexcept
  : mThreshold(agast.mThreshold),
    mNonmaxSuppression(agast.mNonmaxSuppression),
    mDetectorType(std::move(agast.mDetectorType))
{
}

AgastProperties::~AgastProperties()
{
}

AgastProperties &AgastProperties::operator =(const AgastProperties &agast)
{
  if (this != &agast) {
    mThreshold = agast.mThreshold;
    mNonmaxSuppression = agast.mNonmaxSuppression;
    mDetectorType = agast.mDetectorType;
  }
  return *this;
}

AgastProperties &AgastProperties::operator =(AgastProperties &&agast) noexcept
{
  if (this != &agast) {
    mThreshold = agast.mThreshold;
    mNonmaxSuppression = agast.mNonmaxSuppression;
    mDetectorType = std::move(agast.mDetectorType);
  }
  return *this;
}

int AgastProperties::threshold() const
{
  return mThreshold;
}

bool AgastProperties::nonmaxSuppression() const
{
  return mNonmaxSuppression;
}

std::string AgastProperties::detectorType() const
{
  return mDetectorType;
}

void AgastProperties::setThreshold(int threshold)
{
  mThreshold = threshold;
}

void AgastProperties::setNonmaxSuppression(bool nonmaxSuppression)
{
  mNonmaxSuppression = nonmaxSuppression;
}

void AgastProperties::setDetectorType(const std::string &detectorType)
{
  if (detectorType.compare("AGAST_5_8") == 0 ||
      detectorType.compare("AGAST_7_12d") == 0 ||
      detectorType.compare("AGAST_7_12s") == 0 ||
      detectorType.compare("OAST_9_16") == 0) {
    mDetectorType = detectorType;
  }
}

void AgastProperties::reset()
{
  mThreshold = 10;
  mNonmaxSuppression = true;
  mDetectorType = "OAST_9_16";
}

std::string AgastProperties::name() const
{
  return std::string("AGAST");
}


/*----------------------------------------------------------------*/


AgastDetector::AgastDetector()
{
  this->initAgastFromProperties();
}

AgastDetector::AgastDetector(const AgastDetector &agastDetector)
  : AgastProperties(agastDetector)
{
  this->initAgastFromProperties();
}

AgastDetector::AgastDetector(AgastDetector &&agastDetector) noexcept
  : AgastProperties(std::forward<AgastProperties>(agastDetector))
{
  this->initAgastFromProperties();
}

AgastDetector::AgastDetector(int threshold, 
                             bool nonmaxSuppression, 
                             const std::string &detectorType)
  : mAgast(cv::AgastFeatureDetector::create())
{
  setThreshold(threshold);
  setNonmaxSuppression(nonmaxSuppression);
  setDetectorType(detectorType);
}

AgastDetector::~AgastDetector()
{
  mAgast.release();
}

AgastDetector &AgastDetector::operator =(const AgastDetector &agastDetector)
{
  if (this != &agastDetector){
    AgastProperties::operator=(agastDetector);
    this->initAgastFromProperties();
  }
  return *this;
}

AgastDetector &AgastDetector::operator =(AgastDetector &&agastDetector) noexcept
{
  if (this != &agastDetector){
    AgastProperties::operator=(std::forward<AgastProperties>(agastDetector));
    this->initAgastFromProperties();
  }
  return *this;
}

#if CV_VERSION_MAJOR >= 4
TL_DISABLE_WARNING(26812)
cv::AgastFeatureDetector::DetectorType AgastDetector::convertDetectorType(const std::string &detectorType)
{
  cv::AgastFeatureDetector::DetectorType detector_type = cv::AgastFeatureDetector::DetectorType::OAST_9_16;

  if (detectorType.compare("AGAST_5_8") == 0 ) {
    detector_type = cv::AgastFeatureDetector::AGAST_5_8;
  } else if (detectorType.compare("AGAST_7_12d") == 0){
    detector_type = cv::AgastFeatureDetector::AGAST_7_12d;
  } else if (detectorType.compare("AGAST_7_12s") == 0){
    detector_type = cv::AgastFeatureDetector::AGAST_7_12s;
  }
  return detector_type;
}
TL_ENABLE_WARNING(26812)
#else
int AgastDetector::convertDetectorType(const std::string &detectorType)
{
  int detector_type = cv::AgastFeatureDetector::OAST_9_16;
  if (detectorType.compare("AGAST_5_8") == 0 ) {
    detector_type = cv::AgastFeatureDetector::AGAST_5_8;
  } else if (detectorType.compare("AGAST_7_12d") == 0){
    detector_type = cv::AgastFeatureDetector::AGAST_7_12d;
  } else if (detectorType.compare("AGAST_7_12s") == 0){
    detector_type = cv::AgastFeatureDetector::AGAST_7_12s;
  }
  return detector_type;
}
#endif

void AgastDetector::initAgastFromProperties()
{
  mAgast = cv::AgastFeatureDetector::create(AgastProperties::threshold(),
                                            AgastProperties::nonmaxSuppression(),
                                            convertDetectorType(AgastProperties::detectorType()));
}

std::vector<cv::KeyPoint> AgastDetector::detect(const cv::Mat &img,
                                                cv::InputArray &mask)
{
  std::vector<cv::KeyPoint> keyPoints;
  mAgast->detect(img, keyPoints, mask);
  return keyPoints;
}

void AgastDetector::setThreshold(int threshold)
{
  AgastProperties::setThreshold(threshold);
  mAgast->setThreshold(threshold);
}

void AgastDetector::setNonmaxSuppression(bool nonmaxSuppression)
{
  AgastProperties::setNonmaxSuppression(nonmaxSuppression);
  mAgast->setNonmaxSuppression(nonmaxSuppression);
}

void AgastDetector::setDetectorType(const std::string &detectorType)
{
  AgastProperties::setDetectorType(detectorType);
  mAgast->setType(convertDetectorType(AgastProperties::detectorType()));
}

void AgastDetector::reset()
{
  AgastProperties::reset();

  mAgast->setThreshold(AgastProperties::threshold());
  mAgast->setNonmaxSuppression(AgastProperties::nonmaxSuppression());
  mAgast->setType(convertDetectorType(AgastProperties::detectorType()));
}


} // namespace tl
