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
 
#include "sift.h"

#include "tidop/core/base/exception.h"


namespace tl
{


/* SIFT Properties */
SiftProperties::SiftProperties()
  : Feature("SIFT", Feature::Type::sift)
{
    reset();
}

SiftProperties::SiftProperties(const SiftProperties &properties)
  : Feature(properties)
{
}

SiftProperties::SiftProperties(SiftProperties &&properties) TL_NOEXCEPT
  : Feature(std::forward<Feature>(properties))
{
}

auto SiftProperties::operator=(const SiftProperties &properties) -> SiftProperties &
{
    if (this != &properties) {
        Feature::operator=(properties);
    }
    return *this;
}

auto SiftProperties::operator=(SiftProperties &&properties) TL_NOEXCEPT -> SiftProperties &
{
    if (this != &properties) {
        Feature::operator=(std::forward<Feature>(properties));
    }
    return *this;
}

auto SiftProperties::featuresNumber() const -> int
{
    return getProperty<int>("FeaturesNumber");
}

auto SiftProperties::octaveLayers() const -> int
{
    return getProperty<int>("OctaveLayers");
}

auto SiftProperties::contrastThreshold() const -> double
{
    return getProperty<double>("ContrastThreshold");
}

auto SiftProperties::edgeThreshold() const -> double
{
    return getProperty<double>("EdgeThreshold");
}

auto SiftProperties::sigma() const -> double
{
    return getProperty<double>("Sigma");
}

void SiftProperties::setFeaturesNumber(int featuresNumber)
{
    setProperty("FeaturesNumber",featuresNumber);
}

void SiftProperties::setOctaveLayers(int octaveLayers)
{
    setProperty("OctaveLayers",octaveLayers);
}

void SiftProperties::setContrastThreshold(double contrastThreshold)
{
    setProperty("ContrastThreshold",contrastThreshold);
}

void SiftProperties::setEdgeThreshold(double edgeThreshold)
{
    setProperty("EdgeThreshold",edgeThreshold);
}

void SiftProperties::setSigma(double sigma)
{
    setProperty("Sigma",sigma);
}

void SiftProperties::reset()
{
    setFeaturesNumber(sift_default_value_features_number);
    setOctaveLayers(sift_default_value_octave_layers);
    setContrastThreshold(sift_default_value_contrast_threshold);
    setEdgeThreshold(sift_default_value_edge_threshold);
    setSigma(sift_default_value_sigma);
}




/* SIFT descriptor */

SiftDetectorDescriptor::SiftDetectorDescriptor()
  : mProperties()
{
    init();
}

SiftDetectorDescriptor::SiftDetectorDescriptor(const SiftProperties &properties)
  : mProperties(properties)
{
    init();
}

SiftDetectorDescriptor::SiftDetectorDescriptor(const SiftDetectorDescriptor &sift)
  : mProperties(sift.mProperties)
{
    init();
}

SiftDetectorDescriptor::SiftDetectorDescriptor(SiftDetectorDescriptor &&sift) TL_NOEXCEPT
  : mProperties(std::move(sift.mProperties))
#if (CV_VERSION_MAJOR > 4 || (CV_VERSION_MAJOR == 4 && CV_VERSION_MINOR >= 4)) || (defined HAVE_OPENCV_XFEATURES2D && defined OPENCV_ENABLE_NONFREE)
    , mSift(std::move(sift.mSift))
#endif
{
}

auto SiftDetectorDescriptor::operator =(const SiftDetectorDescriptor &sift) -> SiftDetectorDescriptor &
{
    if (this != &sift) {
        mProperties = sift.mProperties;
        init();
    }
    return *this;
}

auto SiftDetectorDescriptor::operator =(SiftDetectorDescriptor &&sift) TL_NOEXCEPT -> SiftDetectorDescriptor &
{
    if (this != &sift) {
        mProperties = std::move(sift.mProperties);
#if (CV_VERSION_MAJOR > 4 || (CV_VERSION_MAJOR == 4 && CV_VERSION_MINOR >= 4)) || (defined HAVE_OPENCV_XFEATURES2D && defined OPENCV_ENABLE_NONFREE)
        mSift = std::move(sift.mSift);
#endif
    }
    return *this;
}

void SiftDetectorDescriptor::init()
{
#if (CV_VERSION_MAJOR > 4 || (CV_VERSION_MAJOR == 4 && CV_VERSION_MINOR >= 4))
    mSift = cv::SIFT::create(mProperties.featuresNumber(),
                             mProperties.octaveLayers(),
                             mProperties.contrastThreshold(),
                             mProperties.edgeThreshold(),
                             mProperties.sigma());
#elif defined HAVE_OPENCV_XFEATURES2D && defined OPENCV_ENABLE_NONFREE
    mSift = cv::xfeatures2d::SIFT::create(mProperties.featuresNumber(),
                                          mProperties.octaveLayers(),
                                          mProperties.contrastThreshold(),
                                          mProperties.edgeThreshold(),
                                          mProperties.sigma());
#endif
}

auto SiftDetectorDescriptor::detect(const cv::Mat &img, cv::InputArray &mask) -> std::vector<cv::KeyPoint>
{
    std::vector<cv::KeyPoint> keyPoints;

    try {

#if (CV_VERSION_MAJOR > 4 || (CV_VERSION_MAJOR == 4 && CV_VERSION_MINOR >= 4)) || (defined HAVE_OPENCV_XFEATURES2D && defined OPENCV_ENABLE_NONFREE)
        mSift->detect(img, keyPoints, mask);
#else
        TL_COMPILER_WARNING("OpenCV not built with extra modules. Sift Detector/Descriptor not supported")
        throw TL_ERROR("OpenCV not built with extra modules. Sift Detector/Descriptor not supported");
#endif // HAVE_OPENCV_XFEATURES2D

    } catch (...) {
        TL_THROW_EXCEPTION_WITH_NESTED("Catched exception");
    }

    return keyPoints;
}

auto SiftDetectorDescriptor::extract(const cv::Mat &img, std::vector<cv::KeyPoint> &keyPoints) -> cv::Mat
{
    cv::Mat descriptors;

    try {

#if (CV_VERSION_MAJOR > 4 || (CV_VERSION_MAJOR == 4 && CV_VERSION_MINOR >= 4)) || (defined HAVE_OPENCV_XFEATURES2D && defined OPENCV_ENABLE_NONFREE)
        mSift->compute(img, keyPoints, descriptors);
#else
        TL_COMPILER_WARNING("OpenCV not built with extra modules. Sift Detector/Descriptor not supported")
        throw TL_ERROR("OpenCV not built with extra modules. Sift Detector/Descriptor not supported");
#endif // HAVE_OPENCV_XFEATURES2D

    } catch (...) {
        TL_THROW_EXCEPTION_WITH_NESTED("Catched exception");
    }

    return descriptors;
}


} // namespace tl







