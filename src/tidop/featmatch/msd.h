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

#include "tidop/featmatch/features.h"

#include <memory>

#if defined HAVE_OPENCV_XFEATURES2D && (CV_VERSION_MAJOR >= 3 || (CV_VERSION_MAJOR >= 3 && CV_VERSION_MINOR >= 1))
#include <opencv2/xfeatures2d.hpp>
#else
class MsdDetector;
#endif


namespace tl
{


class TL_EXPORT MsdProperties
  : public Msd
{

private:

    double mThresholdSaliency;
    int mPatchRadius;
    int mKNN;
    int mAreaRadius;
    double mScaleFactor;
    int mNMSRadius;
    int mNScales;
    int mNMSScaleR;
    bool mComputeOrientations;
    bool mAffineMSD;
    int mAffineTilts;

public:

    MsdProperties();
    MsdProperties(const MsdProperties &msd);
    MsdProperties(MsdProperties &&msd) TL_NOEXCEPT;
    ~MsdProperties() override;

    auto operator =(const MsdProperties &msd) -> MsdProperties &;
    auto operator =(MsdProperties &&msd) TL_NOEXCEPT -> MsdProperties &;

// Msd interface

public:

    virtual auto thresholdSaliency() const -> double override;
    virtual auto patchRadius() const -> int override;
    virtual auto knn() const -> int override;
    virtual auto searchAreaRadius() const -> int override;
    virtual auto scaleFactor() const -> double override;
    virtual auto NMSRadius() const -> int override;
    virtual auto nScales() const ->int override;
    virtual auto NMSScaleRadius() const -> int override;
    virtual auto computeOrientation() const -> bool override;
    virtual auto affineMSD() const -> bool override;
    virtual auto affineTilts() const -> int override;
    virtual void setThresholdSaliency(double thresholdSaliency) override;
    virtual void setPatchRadius(int patchRadius) override;
    virtual void setKNN(int knn) override;
    virtual void setSearchAreaRadius(int searchAreaRadius) override;
    virtual void setScaleFactor(double scaleFactor) override;
    virtual void setNMSRadius(int NMSRadius) override;
    virtual void setNScales(int nScales) override;
    virtual void setNMSScaleRadius(int NMSScaleR) override;
    virtual void setComputeOrientation(bool computeOrientation) override;
    virtual void setAffineMSD(bool affineMSD) override;
    virtual void setAffineTilts(int affineTilts) override;

// Feature interface

public:

    void reset() override;
    std::string name() const final;

};


/*----------------------------------------------------------------*/


class MsdDetector
  : public MsdProperties,
    public KeypointDetector
{

protected:

#if defined HAVE_OPENCV_XFEATURES2D && (CV_VERSION_MAJOR >= 3 || (CV_VERSION_MAJOR >= 3 && CV_VERSION_MINOR >= 1))
    cv::Ptr<cv::xfeatures2d::MSDDetector> mMSD;
#else
    std::shared_ptr<::MsdDetector> mMSD;
#endif

public:

    MsdDetector();
    MsdDetector(double thresholdSaliency,
                int patchRadius,
                int knn,
                int searchAreaRadius,
                double scaleFactor,
                int NMSRadius,
                int nScales,
                int NMSScaleR,
                bool computeOrientation,
                bool affineMSD,
                int affineTilts);
    ~MsdDetector() override;

private:

    void update();

#if CV_VERSION_MAJOR < 3 || (CV_VERSION_MAJOR == 3 && CV_VERSION_MINOR < 1) || !defined HAVE_OPENCV_XFEATURES2D

    bool pointIsAcceptable(const cv::KeyPoint &vl_keypoint, int width, int height);
    void compensate_affine_coor1(float *x0, float *y0, int w1, int h1, float t1, float t2, float Rtheta);
    void affineSkew(double tilt, double phi, cv::Mat &img, cv::Mat &mask, cv::Mat &Ai);

#endif

// KeypointDetector interface

public:

    auto detect(const cv::Mat &img, cv::InputArray &mask = cv::noArray()) -> std::vector<cv::KeyPoint> override;

// Msd interface

public:

    void setThresholdSaliency(double thresholdSaliency) override;
    void setPatchRadius(int patchRadius) override;
    void setKNN(int knn) override;
    void setSearchAreaRadius(int searchAreaRadius) override;
    void setScaleFactor(double scaleFactor) override;
    void setNMSRadius(int NMSRadius) override;
    void setNScales(int nScales) override;
    void setNMSScaleRadius(int NMSScaleR) override;
    void setComputeOrientation(bool computeOrientation) override;
    void setAffineMSD(bool affineMSD) override;
    void setAffineTilts(int affineTilts) override;

// Feature interface

public:

    void reset() override;

};


} // namespace tl

