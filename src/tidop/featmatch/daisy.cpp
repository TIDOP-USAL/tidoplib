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

#include "tidop/core/exception.h"


namespace tl
{

DaisyProperties::DaisyProperties()
  : mNorm(daisy_default_value_norm)
{
}

DaisyProperties::DaisyProperties(const DaisyProperties &daisyProperties) = default;

auto DaisyProperties::radius() const -> double
{
    return mRadius;
}

auto DaisyProperties::qRadius() const -> int
{
    return mQRadius;
}

auto DaisyProperties::qTheta() const -> int
{
    return mQTheta;
}

auto DaisyProperties::qHist() const -> int
{
    return mQHist;
}

auto DaisyProperties::norm() const -> std::string
{
    return mNorm;
}

auto DaisyProperties::interpolation() const -> bool
{
    return mInterpolation;
}

auto DaisyProperties::useOrientation() const -> bool
{
    return mUseOrientation;
}

void DaisyProperties::setRadius(double radius)
{
    mRadius = radius;
}

void DaisyProperties::setQRadius(int qRadius)
{
    mQRadius = qRadius;
}

void DaisyProperties::setQTheta(int qTheta)
{
    mQTheta = qTheta;
}

void DaisyProperties::setQHist(int qHist)
{
    mQHist = qHist;
}

void DaisyProperties::setNorm(const std::string &norm)
{
    if (norm == "NRM_NONE" ||
        norm == "NRM_PARTIAL" ||
        norm == "NRM_FULL" ||
        norm == "NRM_SIFT") {
        mNorm = norm;
    }
}

void DaisyProperties::setInterpolation(bool interpolation)
{
    mInterpolation = interpolation;
}

void DaisyProperties::setUseOrientation(bool useOrientation)
{
    mUseOrientation = useOrientation;
}

void DaisyProperties::reset()
{
    mRadius = daisy_default_value_radius;
    mQRadius = daisy_default_value_qradius;
    mQTheta = daisy_default_value_qtheta;
    mQHist = daisy_default_value_qhist;
    mNorm = daisy_default_value_norm;
    mInterpolation = daisy_default_value_interpolation;
    mUseOrientation = daisy_default_value_use_orientation;
}

auto DaisyProperties::name() const -> std::string
{
    return std::string("DAISY");
}


/*----------------------------------------------------------------*/


DaisyDescriptor::DaisyDescriptor()
{
    update();
}

DaisyDescriptor::DaisyDescriptor(const DaisyDescriptor &daisyDescriptor)
  : DaisyProperties(daisyDescriptor),
    DescriptorExtractor(daisyDescriptor)
{
    update();
}

DaisyDescriptor::DaisyDescriptor(double radius,
                                 int qRadius,
                                 int qTheta,
                                 int qHist,
                                 const std::string &norm,
                                 bool interpolation,
                                 bool useOrientation)
{
    DaisyProperties::setRadius(radius);
    DaisyProperties::setQRadius(qRadius);
    DaisyProperties::setQTheta(qTheta);
    DaisyProperties::setQHist(qHist);
    DaisyProperties::setNorm(norm);
    DaisyProperties::setInterpolation(interpolation);
    DaisyProperties::setUseOrientation(useOrientation);
    update();
}

void DaisyDescriptor::update()
{
#ifdef HAVE_OPENCV_XFEATURES2D

#if CV_VERSION_MAJOR >= 4
    cv::xfeatures2d::DAISY::NormalizationType daisy_norm = cv::xfeatures2d::DAISY::NormalizationType::NRM_NONE;
#else
    int daisy_norm = cv::xfeatures2d::DAISY::NRM_NONE;
#endif

    std::string norm = DaisyProperties::norm();
    if (norm == "NRM_NONE") {
        daisy_norm = cv::xfeatures2d::DAISY::NRM_NONE;
    } else if (norm == "NRM_PARTIAL") {
        daisy_norm = cv::xfeatures2d::DAISY::NRM_PARTIAL;
    } else if (norm == "NRM_FULL") {
        daisy_norm = cv::xfeatures2d::DAISY::NRM_FULL;
    } else if (norm == "NRM_SIFT") {
        daisy_norm = cv::xfeatures2d::DAISY::NRM_SIFT;
    }

    mDAISY = cv::xfeatures2d::DAISY::create(static_cast<float>(DaisyProperties::radius()),
                                            DaisyProperties::qRadius(),
                                            DaisyProperties::qTheta(),
                                            DaisyProperties::qHist(),
                                            daisy_norm, cv::noArray(),
                                            DaisyProperties::interpolation(),
                                            DaisyProperties::useOrientation());
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

void DaisyDescriptor::setRadius(double radius)
{
    DaisyProperties::setRadius(radius);
    update();
}

void DaisyDescriptor::setQRadius(int qRadius)
{
    DaisyProperties::setQRadius(qRadius);
    update();
}

void DaisyDescriptor::setQTheta(int qTheta)
{
    DaisyProperties::setQTheta(qTheta);
    update();
}

void DaisyDescriptor::setQHist(int qHist)
{
    DaisyProperties::setQHist(qHist);
    update();
}

void DaisyDescriptor::setNorm(const std::string &norm)
{
    DaisyProperties::setNorm(norm);
    update();
}

void DaisyDescriptor::setInterpolation(bool interpolation)
{
    DaisyProperties::setInterpolation(interpolation);
    update();
}

void DaisyDescriptor::setUseOrientation(bool useOrientation)
{
    DaisyProperties::setUseOrientation(useOrientation);
    update();
}

void DaisyDescriptor::reset()
{
    DaisyProperties::reset();
    update();
}


} // namespace tl
