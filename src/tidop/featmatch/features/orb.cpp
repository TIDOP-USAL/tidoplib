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

#include "orb.h"

#include "tidop/core/base/exception.h"


namespace tl
{


/* ORB properties */

OrbProperties::OrbProperties()
  : Feature("ORB", Feature::Type::orb)
{
    reset();
}

OrbProperties::OrbProperties(const OrbProperties &properties)
  : Feature(properties)
{
}

OrbProperties::OrbProperties(OrbProperties &&properties) TL_NOEXCEPT
  : Feature(std::forward<Feature>(properties))
{
}

auto OrbProperties::operator=(const OrbProperties &properties) -> OrbProperties &
{
    if (this != &properties) {
        Feature::operator=(properties);
    }
    return *this;
}

auto OrbProperties::operator=(OrbProperties &&properties) TL_NOEXCEPT -> OrbProperties &
{
    if (this != &properties) {
        Feature::operator=(std::forward<Feature>(properties));
    }
    return *this;
}

auto OrbProperties::featuresNumber() const -> int
{
    return getProperty<int>("FeaturesNumber");
}

auto OrbProperties::scaleFactor() const -> float
{
    return getProperty<float>("ScaleFactor");
}

auto OrbProperties::levelsNumber() const -> int
{
    return getProperty<int>("LevelsNumber");
}

auto OrbProperties::edgeThreshold() const -> int
{
    return getProperty<int>("EdgeThreshold");
}

auto OrbProperties::firstLevel() const -> int
{
    return getProperty<int>("FirstLevel");
}

auto OrbProperties::wta_k() const -> int
{
    return getProperty<int>("WTA_K");
}

auto OrbProperties::scoreType() const -> std::string
{
    return getProperty<std::string>("ScoreType");
}

auto OrbProperties::patchSize() const -> int
{
    return getProperty<int>("PatchSize");
}

auto OrbProperties::fastThreshold() const -> int
{
    return getProperty<int>("FastThreshold");
}

void OrbProperties::setScaleFactor(float scaleFactor)
{
    setProperty("ScaleFactor", scaleFactor);
}

void OrbProperties::setFeaturesNumber(int featuresNumber)
{
    setProperty("FeaturesNumber", featuresNumber);
}

void OrbProperties::setLevelsNumber(int levelsNumber)
{
    setProperty("LevelsNumber", levelsNumber);
}

void OrbProperties::setEdgeThreshold(int edgeThreshold)
{
    setProperty("EdgeThreshold", edgeThreshold);
}

void OrbProperties::setFirstLevel(int firstLevel)
{
    setProperty("FirstLevel", firstLevel);
}

void OrbProperties::setWTA_K(int WTA_K)
{
    setProperty("WTA_K", WTA_K);
}

void OrbProperties::setScoreType(const std::string &scoreType)
{
    setProperty("ScoreType", scoreType);
}

void OrbProperties::setPatchSize(int patchSize)
{
    setProperty("PatchSize", patchSize);
}

void OrbProperties::setFastThreshold(int fastThreshold)
{
    setProperty("FastThreshold", fastThreshold);
}

void OrbProperties::reset()
{
    setFeaturesNumber(5000);
    setScaleFactor(1.2f);
    setLevelsNumber(8);
    setEdgeThreshold(31);
    setFirstLevel(0);
    setWTA_K(2);
    setScoreType("Harris");
    setPatchSize(31);
    setFastThreshold(20);
}




/* Orb detector/descriptor */

OrbDetectorDescriptor::OrbDetectorDescriptor()
  : mProperties()
{
    init();
}


OrbDetectorDescriptor::OrbDetectorDescriptor(const OrbProperties &properties)
  : mProperties(properties)
{
    init();
}

OrbDetectorDescriptor::OrbDetectorDescriptor(const OrbDetectorDescriptor &orb)
  : mProperties(orb.mProperties)
{
    init();
}

OrbDetectorDescriptor::OrbDetectorDescriptor(OrbDetectorDescriptor &&orb) TL_NOEXCEPT
  : mProperties(std::move(orb.mProperties)), 
    mOrb(std::move(orb.mOrb))
{
}

auto OrbDetectorDescriptor::operator =(const OrbDetectorDescriptor &orb) -> OrbDetectorDescriptor &
{
    if (this != &orb) {
        mProperties = orb.mProperties;
        init();
    }
    return *this;
}

auto OrbDetectorDescriptor::operator =(OrbDetectorDescriptor &&orb) TL_NOEXCEPT -> OrbDetectorDescriptor &
{
    if (this != &orb) {
        mProperties = std::move(orb.mProperties);
        mOrb = std::move(orb.mOrb);
    }
    return *this;
}

void OrbDetectorDescriptor::init()
{
    mOrb = cv::ORB::create(mProperties.featuresNumber(),
                           mProperties.scaleFactor(),
                           mProperties.levelsNumber(),
                           mProperties.edgeThreshold(),
                           mProperties.firstLevel(),
                           mProperties.wta_k(),
                           convertScoreType(mProperties.scoreType()),
                           mProperties.patchSize(),
                           mProperties.fastThreshold());
}

#if CV_VERSION_MAJOR >= 4

auto OrbDetectorDescriptor::convertScoreType(const std::string &scoreType) -> cv::ORB::ScoreType
{
    cv::ORB::ScoreType score = cv::ORB::ScoreType::HARRIS_SCORE;

    if (scoreType == "Harris") {
        score = cv::ORB::HARRIS_SCORE;
    } else if (scoreType == "FAST") {
        score = cv::ORB::FAST_SCORE;
    }

    return score;
}

#else

auto OrbDetectorDescriptor::convertScoreType(const std::string &scoreType) -> int
{
    int score = cv::ORB::HARRIS_SCORE;

    if (scoreType == "Harris") {
        score = cv::ORB::HARRIS_SCORE;
    } else if (scoreType == "FAST") {
        score = cv::ORB::FAST_SCORE;
    }

    return score;
}

#endif

auto OrbDetectorDescriptor::detect(const cv::Mat &img, cv::InputArray &mask) -> std::vector<cv::KeyPoint>
{
    std::vector<cv::KeyPoint> keyPoints;

    try {

        mOrb->detect(img, keyPoints, mask);

    } catch (...) {
        std::throw_with_nested(std::runtime_error("OrbDetectorDescriptor::detect() failed"));
    }

    return keyPoints;
}

auto OrbDetectorDescriptor::extract(const cv::Mat &img, std::vector<cv::KeyPoint> &keyPoints) -> cv::Mat
{
    cv::Mat descriptors;

    try {

        mOrb->compute(img, keyPoints, descriptors);

    } catch (...) {
        std::throw_with_nested(std::runtime_error("OrbDetectorDescriptor::extract() failed"));
    }

    return descriptors;
}



/* Orb detector/descriptor CUDA */

OrbCudaDetectorDescriptor::OrbCudaDetectorDescriptor()
  : mProperties()
{
    init();
}

OrbCudaDetectorDescriptor::OrbCudaDetectorDescriptor(const OrbProperties &properties)
  : mProperties(properties)
{
    init();
}

OrbCudaDetectorDescriptor::OrbCudaDetectorDescriptor(const OrbCudaDetectorDescriptor &orb)
  : mProperties(orb.mProperties)
{
    init();
}

OrbCudaDetectorDescriptor::OrbCudaDetectorDescriptor(OrbCudaDetectorDescriptor &&orb) TL_NOEXCEPT
  : mProperties(std::move(orb.mProperties))
#ifdef HAVE_OPENCV_CUDAFEATURES2D  
    , mOrb(std::move(orb.mOrb))
#endif // HAVE_OPENCV_CUDAFEATURES2D
{
}

auto OrbCudaDetectorDescriptor::operator =(const OrbCudaDetectorDescriptor &orb) -> OrbCudaDetectorDescriptor &
{
    if (this != &orb) {
        mProperties = orb.mProperties;
        init();
    }
    return *this;
}

auto OrbCudaDetectorDescriptor::operator =(OrbCudaDetectorDescriptor &&orb) TL_NOEXCEPT -> OrbCudaDetectorDescriptor &
{
    if (this != &orb) {
        mProperties = std::move(orb.mProperties);
        mOrb = std::move(orb.mOrb);
    }
    return *this;
}

auto OrbCudaDetectorDescriptor::convertScoreType(const std::string &scoreType) -> int
{
    int score = cv::ORB::HARRIS_SCORE;

    if (scoreType == "Harris") {
        score = cv::ORB::HARRIS_SCORE;
    } else if (scoreType == "FAST") {
        score = cv::ORB::FAST_SCORE;
    }

    return score;
}

void OrbCudaDetectorDescriptor::init()
{
    mOrb = cv::cuda::ORB::create(mProperties.featuresNumber(),
                                 mProperties.scaleFactor(),
                                 mProperties.levelsNumber(),
                                 mProperties.edgeThreshold(),
                                 mProperties.firstLevel(),
                                 mProperties.wta_k(),
                                 convertScoreType(mProperties.scoreType()),
                                 mProperties.patchSize(),
                                 mProperties.fastThreshold());

}

auto OrbCudaDetectorDescriptor::detect(const cv::Mat &img, cv::InputArray &mask) -> std::vector<cv::KeyPoint>
{
    std::vector<cv::KeyPoint> keyPoints;

    try {

#ifdef HAVE_OPENCV_CUDAFEATURES2D
        cv::cuda::GpuMat g_img(img);
        cv::cuda::GpuMat g_mask(mask);
        mOrb->detect(g_img, keyPoints, g_mask);
#else
        TL_COMPILER_WARNING("OpenCV not built with CUDAFEATURES2D. Cuda ORB Detector/Descriptor not supported")
        throw TL_ERROR("OpenCV not built with CUDAFEATURES2D. Cuda ORB Detector/Descriptor not supported");
#endif // HAVE_OPENCV_CUDAFEATURES2D

    } catch (...) {
        TL_THROW_EXCEPTION_WITH_NESTED("Catched exception");
    }

    return keyPoints;
}

auto OrbCudaDetectorDescriptor::extract(const cv::Mat &img, std::vector<cv::KeyPoint> &keyPoints) -> cv::Mat
{
    cv::Mat descriptors;

    try {

#ifdef HAVE_OPENCV_CUDAFEATURES2D
        cv::cuda::GpuMat g_img(img);
        cv::cuda::GpuMat g_descriptors;
        mOrb->compute(g_img, keyPoints, g_descriptors);
        g_descriptors.download(descriptors);
#else
        TL_COMPILER_WARNING("OpenCV not built with CUDAFEATURES2D. Cuda ORB Detector/Descriptor not supported")
        throw TL_ERROR("OpenCV not built with CUDAFEATURES2D. Cuda ORB Detector/Descriptor not supported");
#endif // HAVE_OPENCV_CUDAFEATURES2D

    } catch (...) {
        TL_THROW_EXCEPTION_WITH_NESTED("Catched exception");
    }

    return descriptors;
}

} // namespace tl

