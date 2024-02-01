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

#include "agast.h"

#include "tidop/core/exception.h"


namespace tl
{


AgastProperties::AgastProperties()
  : mThreshold(agast_default_value_threshold),
    mNonmaxSuppression(agast_default_value_nonmax_suppression),
    mDetectorType(agast_default_value_detector_type)
{
}

AgastProperties::AgastProperties(const AgastProperties &agast)
  : Agast(agast),
    mThreshold(agast.mThreshold),
    mNonmaxSuppression(agast.nonmaxSuppression()),
    mDetectorType(agast.mDetectorType)
{
}

AgastProperties::AgastProperties(AgastProperties &&agast) TL_NOEXCEPT
  : mThreshold(agast.mThreshold),
    mNonmaxSuppression(std::exchange(agast.mNonmaxSuppression,true)),
    mDetectorType(std::move(agast.mDetectorType))
{
}

AgastProperties::~AgastProperties() = default;

auto AgastProperties::operator =(const AgastProperties &agast) -> AgastProperties&
{
    if (this != &agast) {
        mThreshold = agast.mThreshold;
        mNonmaxSuppression = agast.mNonmaxSuppression;
        mDetectorType = agast.mDetectorType;
    }

    return *this;
}

auto AgastProperties::operator =(AgastProperties &&agast) TL_NOEXCEPT -> AgastProperties&
{
    if (this != &agast) {
        mThreshold = agast.mThreshold;
        mNonmaxSuppression = std::exchange(agast.mNonmaxSuppression, true);
        mDetectorType = std::move(agast.mDetectorType);
    }

    return *this;
}

auto AgastProperties::threshold() const -> int
{
    return mThreshold;
}

auto AgastProperties::nonmaxSuppression() const -> bool
{
    return mNonmaxSuppression;
}

auto AgastProperties::detectorType() const -> std::string
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
    if (detectorType == "AGAST_5_8" ||
        detectorType == "AGAST_7_12d" ||
        detectorType == "AGAST_7_12s" ||
        detectorType == "OAST_9_16") {
        mDetectorType = detectorType;
    }
}

void AgastProperties::reset()
{
    mThreshold = agast_default_value_threshold;
    mNonmaxSuppression = agast_default_value_nonmax_suppression;
    mDetectorType = agast_default_value_detector_type;
}

auto AgastProperties::name() const -> std::string
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

AgastDetector::AgastDetector(AgastDetector &&agastDetector) TL_NOEXCEPT
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
    mAgast.reset();
}

auto AgastDetector::operator =(const AgastDetector &agastDetector) -> AgastDetector&
{
    if (this != &agastDetector) {
        AgastProperties::operator=(agastDetector);
        this->initAgastFromProperties();
    }

    return *this;
}

auto AgastDetector::operator =(AgastDetector &&agastDetector) TL_NOEXCEPT -> AgastDetector&
{
    if (this != &agastDetector) {
        AgastProperties::operator=(std::forward<AgastProperties>(agastDetector));
        this->initAgastFromProperties();
    }

    return *this;
}

#if CV_VERSION_MAJOR >= 4
auto AgastDetector::convertDetectorType(const std::string &detectorType) -> cv::AgastFeatureDetector::DetectorType
{
    cv::AgastFeatureDetector::DetectorType detector_type = cv::AgastFeatureDetector::DetectorType::OAST_9_16;

    if (detectorType == "AGAST_5_8") {
        detector_type = cv::AgastFeatureDetector::AGAST_5_8;
    } else if (detectorType == "AGAST_7_12d") {
        detector_type = cv::AgastFeatureDetector::AGAST_7_12d;
    } else if (detectorType == "AGAST_7_12s") {
        detector_type = cv::AgastFeatureDetector::AGAST_7_12s;
    }

    return detector_type;
}
#else
auto AgastDetector::convertDetectorType(const std::string &detectorType) -> int
{
    int detector_type = cv::AgastFeatureDetector::OAST_9_16;
    if (detectorType.compare("AGAST_5_8") == 0) {
        detector_type = cv::AgastFeatureDetector::AGAST_5_8;
    } else if (detectorType.compare("AGAST_7_12d") == 0) {
        detector_type = cv::AgastFeatureDetector::AGAST_7_12d;
    } else if (detectorType.compare("AGAST_7_12s") == 0) {
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

auto AgastDetector::detect(const cv::Mat &img, cv::InputArray &mask) -> std::vector<cv::KeyPoint>
{
    std::vector<cv::KeyPoint> keyPoints;

    try {

        mAgast->detect(img, keyPoints, mask);

    } catch (...) {
        TL_THROW_EXCEPTION_WITH_NESTED("Catched exception");
    }

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
