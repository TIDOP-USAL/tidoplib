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

#include "tidop/featmatch/features/agast.h"

#include "tidop/core/base/exception.h"


namespace tl
{

/* AGAST properties */

AgastProperties::AgastProperties()
  : Feature("AGAST", Type::agast)
{
    reset();
}

AgastProperties::AgastProperties(const AgastProperties &properties)
  : Feature(properties)
{
}

AgastProperties::AgastProperties(AgastProperties &&properties) TL_NOEXCEPT
  : Feature(std::forward<Feature>(properties))
{
}

auto AgastProperties::operator=(const AgastProperties &properties) -> AgastProperties &
{
    if (this != &properties) {
        Feature::operator=(properties);
    }
    return *this;
}

auto AgastProperties::operator=(AgastProperties &&properties) TL_NOEXCEPT -> AgastProperties &
{
    if (this != &properties) {
        Feature::operator=(std::forward<Feature>(properties));
    }
    return *this;
}

auto AgastProperties::threshold() const -> int
{
    return getProperty<int>("Threshold");
}

auto AgastProperties::nonmaxSuppression() const -> bool
{
    return getProperty<bool>("NonmaxSuppression");
}

auto AgastProperties::detectorType() const -> std::string
{
    return getProperty<std::string>("DetectorType");
}

void AgastProperties::setThreshold(int threshold)
{
    setProperty("Threshold", threshold);
}

void AgastProperties::setNonmaxSuppression(bool nonmaxSuppression)
{
    setProperty("NonmaxSuppression", nonmaxSuppression);
}

void AgastProperties::setDetectorType(const std::string &detectorType)
{
    if (detectorType == "AGAST_5_8" ||
        detectorType == "AGAST_7_12d" ||
        detectorType == "AGAST_7_12s" ||
        detectorType == "OAST_9_16") {
        setProperty("DetectorType", detectorType);
    } else {
        Message::warning("'DetectorType' value not valid: {}", detectorType);
    }
}

void AgastProperties::reset()
{
    setThreshold(agast_default_value_threshold);
    setNonmaxSuppression(agast_default_value_nonmax_suppression);
    setDetectorType(agast_default_value_detector_type);
}



/* AGAST detector */

AgastDetector::AgastDetector()
  : mProperties(), 
    mAgast(cv::AgastFeatureDetector::create())
{
    this->initAgastFromProperties();
}

AgastDetector::AgastDetector(const AgastProperties &properties)
  : mProperties(properties), 
    mAgast(cv::AgastFeatureDetector::create())
{
    this->initAgastFromProperties();
}

AgastDetector::AgastDetector(const AgastDetector &agastDetector)
  : mProperties(agastDetector.mProperties),
    mAgast(cv::AgastFeatureDetector::create())
{
    this->initAgastFromProperties();
}

AgastDetector::AgastDetector(AgastDetector &&agastDetector) TL_NOEXCEPT
  : mProperties(std::move(agastDetector.mProperties)), 
    mAgast(std::move(agastDetector.mAgast))
{
}

AgastDetector::~AgastDetector()
{
    mAgast.reset();
}

auto AgastDetector::operator =(const AgastDetector &agastDetector) -> AgastDetector &
{
    if (this != &agastDetector) {
        mProperties = agastDetector.mProperties;
        this->initAgastFromProperties();
    }
    return *this;
}

auto AgastDetector::operator =(AgastDetector &&agastDetector) TL_NOEXCEPT -> AgastDetector &
{
    if (this != &agastDetector) {
        mProperties = std::move(agastDetector.mProperties);
        mAgast = std::move(agastDetector.mAgast);
    }
    return *this;
}

void AgastDetector::initAgastFromProperties()
{
    mAgast->setThreshold(mProperties.threshold());
    mAgast->setNonmaxSuppression(mProperties.nonmaxSuppression());
    mAgast->setType(convertDetectorType(mProperties.detectorType()));
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


} // namespace tl
