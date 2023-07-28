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

#include "tidop/core/exception.h"


namespace tl
{


SiftProperties::SiftProperties()
{
}

SiftProperties::SiftProperties(const SiftProperties &siftProperties)
  : Sift(siftProperties),
    mFeaturesNumber(siftProperties.mFeaturesNumber),
    mOctaveLayers(siftProperties.mOctaveLayers),
    mContrastThreshold(siftProperties.mContrastThreshold),
    mEdgeThreshold(siftProperties.mEdgeThreshold),
    mSigma(siftProperties.mSigma)
{
}

int SiftProperties::featuresNumber() const
{
    return mFeaturesNumber;
}

int SiftProperties::octaveLayers() const
{
    return mOctaveLayers;
}

double SiftProperties::contrastThreshold() const
{
    return mContrastThreshold;
}

double SiftProperties::edgeThreshold() const
{
    return mEdgeThreshold;
}

double SiftProperties::sigma() const
{
    return mSigma;
}

void SiftProperties::setFeaturesNumber(int featuresNumber)
{
    mFeaturesNumber = featuresNumber;
}

void SiftProperties::setOctaveLayers(int octaveLayers)
{
    mOctaveLayers = octaveLayers;
}

void SiftProperties::setContrastThreshold(double contrastThreshold)
{
    mContrastThreshold = contrastThreshold;
}

void SiftProperties::setEdgeThreshold(double edgeThreshold)
{
    mEdgeThreshold = edgeThreshold;
}

void SiftProperties::setSigma(double sigma)
{
    mSigma = sigma;
}

void SiftProperties::reset()
{
    mFeaturesNumber = sift_default_value_features_number;
    mOctaveLayers = sift_default_value_octave_layers;
    mContrastThreshold = sift_default_value_contrast_threshold;
    mEdgeThreshold = sift_default_value_edge_threshold;
    mSigma = sift_default_value_sigma;
}

std::string SiftProperties::name() const
{
    return std::string("SIFT");
}


/*----------------------------------------------------------------*/


SiftDetectorDescriptor::SiftDetectorDescriptor()
{
    update();
}

SiftDetectorDescriptor::SiftDetectorDescriptor(const SiftDetectorDescriptor &siftDetectorDescriptor)
    : SiftProperties(siftDetectorDescriptor),
    KeypointDetector(siftDetectorDescriptor),
    DescriptorExtractor(siftDetectorDescriptor)
{
    update();
}

SiftDetectorDescriptor::SiftDetectorDescriptor(int featuresNumber,
                                               int octaveLayers,
                                               double contrastThreshold,
                                               double edgeThreshold,
                                               double sigma)
{
    SiftProperties::setFeaturesNumber(featuresNumber);
    SiftProperties::setOctaveLayers(octaveLayers);
    SiftProperties::setContrastThreshold(contrastThreshold);
    SiftProperties::setEdgeThreshold(edgeThreshold);
    SiftProperties::setSigma(sigma);
    update();
}

void SiftDetectorDescriptor::update()
{
#if (CV_VERSION_MAJOR > 4 || (CV_VERSION_MAJOR == 4 && CV_VERSION_MINOR >= 4))
    mSift = cv::SIFT::create(SiftProperties::featuresNumber(),
                             SiftProperties::octaveLayers(),
                             SiftProperties::contrastThreshold(),
                             SiftProperties::edgeThreshold(),
                             SiftProperties::sigma());
#elif defined HAVE_OPENCV_XFEATURES2D && defined OPENCV_ENABLE_NONFREE
    mSift = cv::xfeatures2d::SIFT::create(SiftProperties::featuresNumber(),
                                          SiftProperties::octaveLayers(),
                                          SiftProperties::contrastThreshold(),
                                          SiftProperties::edgeThreshold(),
                                          SiftProperties::sigma());
#endif
}

std::vector<cv::KeyPoint> SiftDetectorDescriptor::detect(const cv::Mat &img, cv::InputArray &mask)
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

cv::Mat SiftDetectorDescriptor::extract(const cv::Mat &img, std::vector<cv::KeyPoint> &keyPoints)
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

void SiftDetectorDescriptor::setFeaturesNumber(int featuresNumber)
{
    SiftProperties::setFeaturesNumber(featuresNumber);
    update();
}

void SiftDetectorDescriptor::setOctaveLayers(int octaveLayers)
{
    SiftProperties::setOctaveLayers(octaveLayers);
    update();
}

void SiftDetectorDescriptor::setContrastThreshold(double contrastThreshold)
{
    SiftProperties::setContrastThreshold(contrastThreshold);
    update();
}

void SiftDetectorDescriptor::setEdgeThreshold(double edgeThreshold)
{
    SiftProperties::setEdgeThreshold(edgeThreshold);
    update();
}

void SiftDetectorDescriptor::setSigma(double sigma)
{
    SiftProperties::setSigma(sigma);
    update();
}

void SiftDetectorDescriptor::reset()
{
    SiftProperties::reset();
    update();
}


} // namespace tl







