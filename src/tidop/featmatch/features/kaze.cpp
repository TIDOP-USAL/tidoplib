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

#include "kaze.h"

#include "tidop/core/base/exception.h"


namespace tl
{


/* KAZE properties */

KazeProperties::KazeProperties()
    : Feature("KAZE", Feature::Type::kaze)
{
    reset();
}

KazeProperties::KazeProperties(const KazeProperties &properties)
    : Feature(properties)
{
}

KazeProperties::KazeProperties(KazeProperties &&properties) TL_NOEXCEPT
    : Feature(std::forward<Feature>(properties))
{
}

KazeProperties::~KazeProperties() = default;

auto KazeProperties::operator =(const KazeProperties &properties) -> KazeProperties &
{
    if (this != &properties) {
        Feature::operator=(properties);
    }

    return *this;
}

auto KazeProperties::operator =(KazeProperties &&properties) TL_NOEXCEPT -> KazeProperties &
{
    if (this != &properties) {
        Feature::operator=(std::forward<Feature>(properties));
    }

    return *this;
}

auto KazeProperties::extendedDescriptor() const -> bool
{
    return getProperty<bool>("ExtendedDescriptor");
}

auto KazeProperties::uprightDescriptor() const -> bool
{
    return getProperty<bool>("UprightDescriptor");
}

auto KazeProperties::threshold() const -> float
{
    return getProperty<float>("Threshold");
}

auto KazeProperties::octaves() const -> int
{
    return getProperty<int>("Octaves");
}

auto KazeProperties::octaveLayers() const -> int
{
    return getProperty<int>("OctaveLayers");
}

auto KazeProperties::diffusivity() const -> std::string
{
    return getProperty<std::string>("Diffusivity");
}

void KazeProperties::setExtendedDescriptor(bool extended)
{
    setProperty("ExtendedDescriptor", extended);
}

void KazeProperties::setUprightDescriptor(bool uprightDescriptor)
{
    setProperty("UprightDescriptor", uprightDescriptor);
}

void KazeProperties::setThreshold(float threshold)
{
    setProperty("Threshold", threshold);
}

void KazeProperties::setOctaves(int octaves)
{
    setProperty("Octaves", octaves);
}

void KazeProperties::setOctaveLayers(int octaveLayers)
{
    setProperty("OctaveLayers", octaveLayers);
}

void KazeProperties::setDiffusivity(const std::string &diffusivity)
{
    if (diffusivity.compare("DIFF_PM_G1") == 0 ||
        diffusivity.compare("DIFF_PM_G2") == 0 ||
        diffusivity.compare("DIFF_WEICKERT") == 0 ||
        diffusivity.compare("DIFF_CHARBONNIER") == 0) {
        setProperty("Diffusivity", diffusivity);
    } else {
        Message::warning("'Diffusivity' value not valid: {}", diffusivity);
    }
}

void KazeProperties::reset()
{
    setExtendedDescriptor(false);
    setUprightDescriptor(false);
    setThreshold(0.001f);
    setOctaves(4);
    setOctaveLayers(4);
    setDiffusivity("DIFF_PM_G2");
}



/* kaze Detector/Descriptor */

KazeDetectorDescriptor::KazeDetectorDescriptor()
  : mProperties()
{
    init();
}

KazeDetectorDescriptor::KazeDetectorDescriptor(const KazeProperties &properties)
  : mProperties(properties)
{
    init();
}

KazeDetectorDescriptor::KazeDetectorDescriptor(const KazeDetectorDescriptor &briefDescriptor)
  : mProperties(briefDescriptor.mProperties)
{
    init();
}

KazeDetectorDescriptor::KazeDetectorDescriptor(KazeDetectorDescriptor &&briefDescriptor) TL_NOEXCEPT
  : mProperties(std::move(briefDescriptor.mProperties)),
    mKaze(std::move(briefDescriptor.mKaze))
{
    init();
}

KazeDetectorDescriptor::~KazeDetectorDescriptor() = default;

auto KazeDetectorDescriptor::operator =(const KazeDetectorDescriptor &briefDescriptor) -> KazeDetectorDescriptor &
{
    if (this != &briefDescriptor) {
        mProperties = briefDescriptor.mProperties;
        init();
    }

    return *this;
}

auto KazeDetectorDescriptor::operator =(KazeDetectorDescriptor &&briefDescriptor) TL_NOEXCEPT -> KazeDetectorDescriptor &
{
    if (this != &briefDescriptor) {
        mProperties = std::move(briefDescriptor.mProperties);
        mKaze = std::move(briefDescriptor.mKaze);
    }

    return *this;
}

#if CV_VERSION_MAJOR >= 4

auto KazeDetectorDescriptor::convertDiffusivity(const std::string &diffusivity) -> cv::KAZE::DiffusivityType
{
    cv::KAZE::DiffusivityType diff = cv::KAZE::DIFF_PM_G1;

    if (diffusivity.compare("DIFF_PM_G1") == 0) {
        diff = cv::KAZE::DIFF_PM_G1;
    } else if (diffusivity.compare("DIFF_PM_G2") == 0) {
        diff = cv::KAZE::DIFF_PM_G2;
    } else if (diffusivity.compare("DIFF_WEICKERT") == 0) {
        diff = cv::KAZE::DIFF_WEICKERT;
    } else if (diffusivity.compare("DIFF_CHARBONNIER") == 0) {
        diff = cv::KAZE::DIFF_CHARBONNIER;
    }

    return diff;
}

#else

auto KazeDetectorDescriptor::convertDiffusivity(const std::string &diffusivity) -> int
{
    int diff = cv::KAZE::DIFF_PM_G1;

    if (diffusivity.compare("DIFF_PM_G1") == 0) {
        diff = cv::KAZE::DIFF_PM_G1;
    } else if (diffusivity.compare("DIFF_PM_G2") == 0) {
        diff = cv::KAZE::DIFF_PM_G2;
    } else if (diffusivity.compare("DIFF_WEICKERT") == 0) {
        diff = cv::KAZE::DIFF_WEICKERT;
    } else if (diffusivity.compare("DIFF_CHARBONNIER") == 0) {
        diff = cv::KAZE::DIFF_CHARBONNIER;
    }

    return diff;
}

#endif

void KazeDetectorDescriptor::init()
{
    mKaze = cv::KAZE::create(mProperties.extendedDescriptor(),
                             mProperties.uprightDescriptor(),
                             mProperties.threshold(),
                             mProperties.octaves(),
                             mProperties.octaveLayers(),
                             convertDiffusivity(mProperties.diffusivity()));
}

auto KazeDetectorDescriptor::detect(const cv::Mat &img,
                                    cv::InputArray &mask) -> std::vector<cv::KeyPoint>
{
    std::vector<cv::KeyPoint> keyPoints;

    try {

        mKaze->detect(img, keyPoints, mask);

    } catch (...) {
        TL_THROW_EXCEPTION_WITH_NESTED("Catched exception");
    }

    return keyPoints;
}

auto KazeDetectorDescriptor::extract(const cv::Mat &img,
                                     std::vector<cv::KeyPoint> &keyPoints) -> cv::Mat
{
    cv::Mat descriptors;

    try {

        mKaze->compute(img, keyPoints, descriptors);

    } catch (...) {
        TL_THROW_EXCEPTION_WITH_NESTED("Catched exception");
    }

    return descriptors;
}


} // namespace tl







