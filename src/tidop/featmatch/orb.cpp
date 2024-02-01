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

#include "tidop/core/exception.h"


namespace tl
{


OrbProperties::OrbProperties()
  : Orb(),
    mFeaturesNumber(5000),
    mScaleFactor(1.2),
    mLevelsNumber(8),
    mEdgeThreshold(31),
    mFirstLevel(0),
    mWTA_K(2),
    mScoreType("Harris"),
    mPatchSize(31),
    mFastThreshold(20)
{
}

OrbProperties::OrbProperties(const OrbProperties &orbProperties)
  : Orb(orbProperties),
    mFeaturesNumber(orbProperties.mFeaturesNumber),
    mScaleFactor(orbProperties.mScaleFactor),
    mLevelsNumber(orbProperties.mLevelsNumber),
    mEdgeThreshold(orbProperties.mEdgeThreshold),
    mFirstLevel(orbProperties.mFirstLevel),
    mWTA_K(orbProperties.mWTA_K),
    mScoreType(orbProperties.mScoreType),
    mPatchSize(orbProperties.mPatchSize),
    mFastThreshold(orbProperties.mFastThreshold)
{
}

auto OrbProperties::featuresNumber() const -> int
{
    return mFeaturesNumber;
}

auto OrbProperties::scaleFactor() const -> double
{
    return mScaleFactor;
}

auto OrbProperties::levelsNumber() const -> int
{
    return mLevelsNumber;
}

auto OrbProperties::edgeThreshold() const -> int
{
    return mEdgeThreshold;
}

auto OrbProperties::firstLevel() const -> int
{
    return mFirstLevel;
}

auto OrbProperties::wta_k() const -> int
{
    return mWTA_K;
}

auto OrbProperties::scoreType() const -> std::string
{
    return mScoreType;
}

auto OrbProperties::patchSize() const -> int
{
    return mPatchSize;
}

auto OrbProperties::fastThreshold() const -> int
{
    return mFastThreshold;
}

void OrbProperties::setScaleFactor(double scaleFactor)
{
    mScaleFactor = scaleFactor;
}

void OrbProperties::setFeaturesNumber(int featuresNumber)
{
    mFeaturesNumber = featuresNumber;
}

void OrbProperties::setLevelsNumber(int levelsNumber)
{
    mLevelsNumber = levelsNumber;
}

void OrbProperties::setEdgeThreshold(int edgeThreshold)
{
    mEdgeThreshold = edgeThreshold;
}

void OrbProperties::setFirstLevel(int firstLevel)
{
    mFirstLevel = firstLevel;
}

void OrbProperties::setWTA_K(int WTA_K)
{
    mWTA_K = WTA_K;
}

void OrbProperties::setScoreType(const std::string &scoreType)
{
    mScoreType = scoreType;
}

void OrbProperties::setPatchSize(int patchSize)
{
    mPatchSize = patchSize;
}

void OrbProperties::setFastThreshold(int fastThreshold)
{
    mFastThreshold = fastThreshold;
}

void OrbProperties::reset()
{
    mFeaturesNumber = 5000;
    mScaleFactor = 1.2;
    mLevelsNumber = 8;
    mEdgeThreshold = 31;
    mFirstLevel = 0;
    mWTA_K = 2;
    mScoreType = "Harris";
    mPatchSize = 31;
    mFastThreshold = 20;
}

auto OrbProperties::name() const -> std::string
{
    return std::string("ORB");
}


/*----------------------------------------------------------------*/


OrbDetectorDescriptor::OrbDetectorDescriptor()
{
    mOrb = cv::ORB::create(OrbProperties::featuresNumber(),
                           static_cast<float>(OrbProperties::scaleFactor()),
                           OrbProperties::levelsNumber(),
                           OrbProperties::edgeThreshold(),
                           OrbProperties::firstLevel(),
                           OrbProperties::wta_k(),
                           convertScoreType(OrbProperties::scoreType()),
                           OrbProperties::patchSize(),
                           OrbProperties::fastThreshold());
}

OrbDetectorDescriptor::OrbDetectorDescriptor(const OrbDetectorDescriptor &orbDetectorDescriptor)
  : OrbProperties(orbDetectorDescriptor),
    KeypointDetector(orbDetectorDescriptor),
    DescriptorExtractor(orbDetectorDescriptor)
{
    mOrb = cv::ORB::create(OrbProperties::featuresNumber(),
                           static_cast<float>(OrbProperties::scaleFactor()),
                           OrbProperties::levelsNumber(),
                           OrbProperties::edgeThreshold(),
                           OrbProperties::firstLevel(),
                           OrbProperties::wta_k(),
                           convertScoreType(OrbProperties::scoreType()),
                           OrbProperties::patchSize(),
                           OrbProperties::fastThreshold());
}

OrbDetectorDescriptor::OrbDetectorDescriptor(int featuresNumber,
                                             double scaleFactor,
                                             int levelsNumber,
                                             int edgeThreshold,
                                             int wta_k,
                                             const std::string &scoreType,
                                             int patchSize,
                                             int fastThreshold)
  : mOrb(cv::ORB::create())
{
    setFeaturesNumber(featuresNumber);
    setScaleFactor(scaleFactor);
    setLevelsNumber(levelsNumber);
    setEdgeThreshold(edgeThreshold);
    setWTA_K(wta_k);
    setScoreType(scoreType);
    setPatchSize(patchSize);
    setFastThreshold(fastThreshold);
}

#if CV_VERSION_MAJOR >= 4

auto OrbDetectorDescriptor::convertScoreType(const std::string &scoreType) -> cv::ORB::ScoreType
{
    cv::ORB::ScoreType score = cv::ORB::ScoreType::HARRIS_SCORE;

    if (scoreType.compare("Harris") == 0) {
        score = cv::ORB::HARRIS_SCORE;
    } else if (scoreType.compare("FAST") == 0) {
        score = cv::ORB::FAST_SCORE;
    }

    return score;
}

#else

auto OrbDetectorDescriptor::convertScoreType(const std::string &scoreType) -> int
{
    int score = cv::ORB::HARRIS_SCORE;

    if (scoreType.compare("Harris") == 0) {
        score = cv::ORB::HARRIS_SCORE;
    } else if (scoreType.compare("FAST") == 0) {
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

void OrbDetectorDescriptor::setFeaturesNumber(int featuresNumber)
{
    OrbProperties::setFeaturesNumber(featuresNumber);
    mOrb->setMaxFeatures(featuresNumber);
}

void OrbDetectorDescriptor::setScaleFactor(double scaleFactor)
{
    OrbProperties::setScaleFactor(scaleFactor);
    mOrb->setScaleFactor(scaleFactor);
}

void OrbDetectorDescriptor::setLevelsNumber(int levelsNumber)
{
    OrbProperties::setLevelsNumber(levelsNumber);
    mOrb->setNLevels(levelsNumber);
}

void OrbDetectorDescriptor::setEdgeThreshold(int edgeThreshold)
{
    OrbProperties::setEdgeThreshold(edgeThreshold);
    mOrb->setEdgeThreshold(edgeThreshold);
}

void OrbDetectorDescriptor::setFirstLevel(int firstLevel)
{
    OrbProperties::setFirstLevel(firstLevel);
    mOrb->setFirstLevel(firstLevel);
}

void OrbDetectorDescriptor::setWTA_K(int WTA_K)
{
    OrbProperties::setWTA_K(WTA_K);
    mOrb->setWTA_K(WTA_K);
}

void OrbDetectorDescriptor::setScoreType(const std::string &scoreType)
{
    OrbProperties::setScoreType(scoreType);
    mOrb->setScoreType(convertScoreType(scoreType));
}

void OrbDetectorDescriptor::setPatchSize(int patchSize)
{
    OrbProperties::setPatchSize(patchSize);
    mOrb->setPatchSize(patchSize);
}

void OrbDetectorDescriptor::setFastThreshold(int fastThreshold)
{
    OrbProperties::setFastThreshold(fastThreshold);
    mOrb->setFastThreshold(fastThreshold);
}

void OrbDetectorDescriptor::reset()
{
    OrbProperties::reset();
    mOrb->setMaxFeatures(OrbProperties::featuresNumber());
    mOrb->setScaleFactor(OrbProperties::scaleFactor());
    mOrb->setNLevels(OrbProperties::levelsNumber());
    mOrb->setEdgeThreshold(OrbProperties::edgeThreshold());
    mOrb->setFirstLevel(OrbProperties::firstLevel());
    mOrb->setWTA_K(OrbProperties::wta_k());
    mOrb->setScoreType(convertScoreType(OrbProperties::scoreType()));
    mOrb->setPatchSize(OrbProperties::patchSize());
    mOrb->setFastThreshold(OrbProperties::fastThreshold());
}


/*----------------------------------------------------------------*/




OrbCudaDetectorDescriptor::OrbCudaDetectorDescriptor()
{
    update();
}

OrbCudaDetectorDescriptor::OrbCudaDetectorDescriptor(const OrbCudaDetectorDescriptor &orbCudaDetectorDescriptor)
  : OrbProperties(orbCudaDetectorDescriptor),
    KeypointDetector(orbCudaDetectorDescriptor),
    DescriptorExtractor(orbCudaDetectorDescriptor)
{
    update();
}

OrbCudaDetectorDescriptor::OrbCudaDetectorDescriptor(int featuresNumber,
                                                     double scaleFactor,
                                                     int levelsNumber,
                                                     int edgeThreshold,
                                                     int wta_k,
                                                     const std::string &scoreType,
                                                     int patchSize,
                                                     int fastThreshold)
{
    setFeaturesNumber(featuresNumber);
    setScaleFactor(scaleFactor);
    setLevelsNumber(levelsNumber);
    setEdgeThreshold(edgeThreshold);
    setWTA_K(wta_k);
    setScoreType(scoreType);
    setPatchSize(patchSize);
    setFastThreshold(fastThreshold);
}

auto OrbCudaDetectorDescriptor::convertScoreType(const std::string &scoreType) -> int
{
    int score = cv::ORB::HARRIS_SCORE;

    if (scoreType.compare("Harris") == 0) {
        score = cv::ORB::HARRIS_SCORE;
    } else if (scoreType.compare("FAST") == 0) {
        score = cv::ORB::FAST_SCORE;
    }

    return score;
}

void OrbCudaDetectorDescriptor::update()
{
#ifdef HAVE_OPENCV_CUDAFEATURES2D
    mOrb = cv::cuda::ORB::create(OrbProperties::featuresNumber(),
                                 static_cast<float>(OrbProperties::scaleFactor()),
                                 OrbProperties::levelsNumber(),
                                 OrbProperties::edgeThreshold(),
                                 OrbProperties::firstLevel(),
                                 OrbProperties::wta_k(),
                                 convertScoreType(OrbProperties::scoreType()),
                                 OrbProperties::patchSize(),
                                 OrbProperties::fastThreshold());
#endif // HAVE_OPENCV_CUDAFEATURES2D
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

void OrbCudaDetectorDescriptor::setFeaturesNumber(int featuresNumber)
{
    OrbProperties::setFeaturesNumber(featuresNumber);
    update();
}

void OrbCudaDetectorDescriptor::setScaleFactor(double scaleFactor)
{
    OrbProperties::setScaleFactor(scaleFactor);
    update();
}

void OrbCudaDetectorDescriptor::setLevelsNumber(int levelsNumber)
{
    OrbProperties::setLevelsNumber(levelsNumber);
    update();
}

void OrbCudaDetectorDescriptor::setEdgeThreshold(int edgeThreshold)
{
    OrbProperties::setEdgeThreshold(edgeThreshold);
    update();
}

void OrbCudaDetectorDescriptor::setFirstLevel(int firstLevel)
{
    OrbProperties::setFirstLevel(firstLevel);
    update();
}

void OrbCudaDetectorDescriptor::setWTA_K(int WTA_K)
{
    OrbProperties::setWTA_K(WTA_K);
    update();
}

void OrbCudaDetectorDescriptor::setScoreType(const std::string &scoreType)
{
    OrbProperties::setScoreType(scoreType);
    update();
}

void OrbCudaDetectorDescriptor::setPatchSize(int patchSize)
{
    OrbProperties::setPatchSize(patchSize);
    update();
}

void OrbCudaDetectorDescriptor::setFastThreshold(int fastThreshold)
{
    OrbProperties::setFastThreshold(fastThreshold);
    update();
}

void OrbCudaDetectorDescriptor::reset()
{
    OrbProperties::reset();
    update();
}


} // namespace tl

