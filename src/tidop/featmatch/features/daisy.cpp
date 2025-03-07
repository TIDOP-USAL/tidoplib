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

#include "daisy.h"

#include "tidop/core/base/exception.h"


namespace tl
{

/* DAISY properties */

DaisyProperties::DaisyProperties()
 : Feature("DAISY", Feature::Type::daisy)
{
    reset();
}

DaisyProperties::DaisyProperties(const DaisyProperties &properties)
  : Feature(properties)
{
}

DaisyProperties::DaisyProperties(DaisyProperties &&properties) TL_NOEXCEPT
  : Feature(std::forward<Feature>(properties))
{
}

auto DaisyProperties::operator=(const DaisyProperties &properties) -> DaisyProperties &
{
    if (this != &properties) {
        Feature::operator=(properties);
    }
    return *this;
}

auto DaisyProperties::operator=(DaisyProperties &&properties) TL_NOEXCEPT -> DaisyProperties &
{
    if (this != &properties) {
        Feature::operator=(std::forward<Feature>(properties));
    }
    return *this;
}

auto DaisyProperties::radius() const -> float
{
    return getProperty<float>("Radius");
}

auto DaisyProperties::radialDivisions() const -> int
{
    return getProperty<int>("QRadius");
}

auto DaisyProperties::angularDivision() const -> int
{
    return getProperty<int>("QTheta");
}

auto DaisyProperties::gradientOrientationsDivision() const -> int
{
    return getProperty<int>("QHist");
}

auto DaisyProperties::normalization() const -> std::string
{
    return getProperty<std::string>("Norm");
}

auto DaisyProperties::interpolation() const -> bool
{
    return getProperty<bool>("Interpolation");
}

auto DaisyProperties::useOrientation() const -> bool
{
    return getProperty<bool>("UseOrientation");
}

void DaisyProperties::setRadius(float radius)
{
    setProperty("Radius", radius);
}

void DaisyProperties::setRadialDivisions(int radialDivisions)
{
    setProperty("QRadius", radialDivisions);
}

void DaisyProperties::setAngularDivision(int angularDivision)
{
    setProperty("QTheta", angularDivision);
}

void DaisyProperties::setGradientOrientationsDivision(int gradientOrientationsDivision)
{
    setProperty("QHist", gradientOrientationsDivision);
}

void DaisyProperties::setNormalization(const std::string &normalization)
{
    if (normalization == "NRM_NONE" ||
        normalization == "NRM_PARTIAL" ||
        normalization == "NRM_FULL" ||
        normalization == "NRM_SIFT") {
        setProperty("Norm", normalization);
    } else {
        Message::warning("'Norm' value not valid: {}", normalization);
    }
}

void DaisyProperties::setInterpolation(bool interpolation)
{
    setProperty("Interpolation", interpolation);
}

void DaisyProperties::setUseOrientation(bool useOrientation)
{
    setProperty("UseOrientation", useOrientation);
}

void DaisyProperties::reset()
{
    setRadius(daisy_default_value_radius);
    setRadialDivisions(daisy_default_value_qradius);
    setAngularDivision(daisy_default_value_qtheta);
    setGradientOrientationsDivision(daisy_default_value_qhist);
    setNormalization(daisy_default_value_norm);
    setInterpolation(daisy_default_value_interpolation);
    setUseOrientation(daisy_default_value_use_orientation);
}



/* DAISY descriptor */

DaisyDescriptor::DaisyDescriptor()
  : mProperties()
{
    init();
}

DaisyDescriptor::DaisyDescriptor(const DaisyProperties &properties)
  : mProperties(properties)
{
}

DaisyDescriptor::DaisyDescriptor(const DaisyDescriptor &daisy)
  : mProperties(daisy.mProperties)
{
    init();
}

DaisyDescriptor::DaisyDescriptor(DaisyDescriptor &&daisy) TL_NOEXCEPT
  : mProperties(std::move(daisy.mProperties)),
    mDAISY(std::move(daisy.mDAISY))
{
}

auto DaisyDescriptor::operator=(const DaisyDescriptor &daisy) -> DaisyDescriptor &
{
    if (this != &daisy) {
        mProperties = daisy.mProperties;
        init();
    }
    return *this;
}

auto DaisyDescriptor::operator =(DaisyDescriptor &&daisy) TL_NOEXCEPT -> DaisyDescriptor &
{
    if (this != &daisy) {
        mProperties = std::move(daisy.mProperties);
        mDAISY = std::move(daisy.mDAISY);
    }
    return *this;
}

void DaisyDescriptor::init()
{
#ifdef HAVE_OPENCV_XFEATURES2D

#if CV_VERSION_MAJOR >= 4
    cv::xfeatures2d::DAISY::NormalizationType daisy_norm = cv::xfeatures2d::DAISY::NormalizationType::NRM_NONE;
#else
    int daisy_norm = cv::xfeatures2d::DAISY::NRM_NONE;
#endif

    std::string normalization = mProperties.normalization();
    if (normalization == "NRM_NONE") {
        daisy_norm = cv::xfeatures2d::DAISY::NRM_NONE;
    } else if (normalization == "NRM_PARTIAL") {
        daisy_norm = cv::xfeatures2d::DAISY::NRM_PARTIAL;
    } else if (normalization == "NRM_FULL") {
        daisy_norm = cv::xfeatures2d::DAISY::NRM_FULL;
    } else if (normalization == "NRM_SIFT") {
        daisy_norm = cv::xfeatures2d::DAISY::NRM_SIFT;
    }

    mDAISY = cv::xfeatures2d::DAISY::create(mProperties.radius(),
                                            mProperties.radialDivisions(),
                                            mProperties.angularDivision(),
                                            mProperties.gradientOrientationsDivision(),
                                            daisy_norm, cv::noArray(),
                                            mProperties.interpolation(),
                                            mProperties.useOrientation());
#endif // HAVE_OPENCV_XFEATURES2D
}

auto DaisyDescriptor::extract(const cv::Mat &img, std::vector<cv::KeyPoint> &keyPoints) -> cv::Mat
{
    cv::Mat descriptors;

    try {

#ifdef HAVE_OPENCV_XFEATURES2D 
        mDAISY->compute(img, keyPoints, descriptors);
#else
        TL_COMPILER_WARNING("OpenCV not built with extra modules. Daisy Descriptor not supported")
            throw TL_ERROR("OpenCV not built with extra modules. Daisy Descriptor not supported");
#endif // HAVE_OPENCV_XFEATURES2D

    } catch (...) {
        TL_THROW_EXCEPTION_WITH_NESTED("Catched exception");
    }

    return descriptors;
}


} // namespace tl
