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

#pragma once

#include "tidop/featmatch/base/features.h"

#ifdef HAVE_OPENCV_CUDAFEATURES2D
#include <opencv2/cudafeatures2d.hpp>
#endif // HAVE_OPENCV_CUDAFEATURES2D


namespace tl
{

/*! \addtogroup Features
 * 
 *  \{
 */

/*! \addtogroup FeatureDetectorAndDescriptor
 * 
 *  \{
 */


class TL_EXPORT OrbProperties
  : public Orb
{

private:

    int mFeaturesNumber;
    double mScaleFactor;
    int mLevelsNumber;
    int mEdgeThreshold;
    int mFirstLevel;
    int mWTA_K;
    std::string mScoreType;
    int mPatchSize;
    int mFastThreshold;

public:

    OrbProperties();
    OrbProperties(const OrbProperties &orbProperties);
    ~OrbProperties() override = default;

// Orb interface

public:

    auto featuresNumber() const -> int override;
    auto scaleFactor() const -> double override;
    auto levelsNumber() const -> int override;
    auto edgeThreshold() const -> int override;
    auto firstLevel() const -> int override;
    auto wta_k() const -> int override;
    auto scoreType() const -> std::string override;
    auto patchSize() const -> int override;
    auto fastThreshold() const -> int override;
    void setFeaturesNumber(int featuresNumber) override;
    void setScaleFactor(double scaleFactor) override;
    void setLevelsNumber(int levelsNumber) override;
    void setEdgeThreshold(int edgeThreshold) override;
    void setFirstLevel(int firstLevel) override;
    void setWTA_K(int WTA_K) override;
    void setScoreType(const std::string &scoreType) override;
    void setPatchSize(int patchSize) override;
    void setFastThreshold(int fastThreshold) override;

// Feature interface

public:

    void reset() override;
    auto name() const -> std::string final;

};


/*----------------------------------------------------------------*/


class TL_EXPORT OrbDetectorDescriptor
  : public OrbProperties,
    public FeatureDetector,
    public FeatureDescriptor
{

protected:

    cv::Ptr<cv::ORB> mOrb;

public:

    OrbDetectorDescriptor();
    OrbDetectorDescriptor(const OrbDetectorDescriptor &orbDetectorDescriptor);
    OrbDetectorDescriptor(int featuresNumber,
                          double scaleFactor,
                          int levelsNumber,
                          int edgeThreshold,
                          int wta_k,
                          const std::string &scoreType,
                          int patchSize,
                          int fastThreshold);

    ~OrbDetectorDescriptor() override = default;

private:

#if CV_VERSION_MAJOR >= 4
    auto convertScoreType(const std::string &scoreType) -> cv::ORB::ScoreType;
#else
    auto convertScoreType(const std::string &scoreType) -> int;
#endif

// FeatureDetector interface

public:

    auto detect(const cv::Mat &img, 
                cv::InputArray &mask = cv::noArray()) -> std::vector<cv::KeyPoint> override;

// FeatureDescriptor interface

public:

    auto extract(const cv::Mat &img,
                 std::vector<cv::KeyPoint> &keyPoints) -> cv::Mat override;

// Orb interface

public:

    void setFeaturesNumber(int featuresNumber) override;
    void setScaleFactor(double scaleFactor) override;
    void setLevelsNumber(int levelsNumber) override;
    void setEdgeThreshold(int edgeThreshold) override;
    void setFirstLevel(int firstLevel) override;
    void setWTA_K(int WTA_K) override;
    void setScoreType(const std::string &scoreType) override;
    void setPatchSize(int patchSize) override;
    void setFastThreshold(int fastThreshold) override;

// Feature interface

public:

    void reset() override;

};



/*----------------------------------------------------------------*/


class TL_EXPORT OrbCudaDetectorDescriptor
  : public OrbProperties,
    public FeatureDetector,
    public FeatureDescriptor
{

protected:

#ifdef HAVE_OPENCV_CUDAFEATURES2D
    cv::Ptr<cv::cuda::ORB> mOrb;
#endif // HAVE_OPENCV_CUDAFEATURES2D

public:

    OrbCudaDetectorDescriptor();
    OrbCudaDetectorDescriptor(const OrbCudaDetectorDescriptor &orbCudaDetectorDescriptor);
    OrbCudaDetectorDescriptor(int featuresNumber,
                              double scaleFactor,
                              int levelsNumber,
                              int edgeThreshold,
                              int wta_k,
                              const std::string &scoreType,
                              int patchSize,
                              int fastThreshold);

    ~OrbCudaDetectorDescriptor() override = default;

private:

    int convertScoreType(const std::string &scoreType);

    void update();

// FeatureDetector interface

public:

    std::vector<cv::KeyPoint> detect(const cv::Mat &img,
                                     cv::InputArray &mask = cv::noArray()) override;

// FeatureDescriptor interface

public:

    cv::Mat extract(const cv::Mat &img,
                    std::vector<cv::KeyPoint> &keyPoints) override;

// Orb interface

public:

    void setFeaturesNumber(int featuresNumber) override;
    void setScaleFactor(double scaleFactor) override;
    void setLevelsNumber(int levelsNumber) override;
    void setEdgeThreshold(int edgeThreshold) override;
    void setFirstLevel(int firstLevel) override;
    void setWTA_K(int WTA_K) override;
    void setScoreType(const std::string &scoreType) override;
    void setPatchSize(int patchSize) override;
    void setFastThreshold(int fastThreshold) override;

// Feature interface

public:

    void reset() override;

};



/*! \} */ // end of FeatureDetectorAndDescriptor

/*! \} */ // end of Features

} // namespace tl

