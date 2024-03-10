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
 
#include "msd.h"

#include "tidop/core/exception.h"

#include <opencv2/imgproc.hpp>

#if CV_VERSION_MAJOR < 3 || (CV_VERSION_MAJOR == 3 && CV_VERSION_MINOR < 1) || !defined HAVE_OPENCV_XFEATURES2D
#include "msd/MSD.h"

#define _USE_MATH_DEFINES
#define ABS(x)    (((x) > 0) ? (x) : (-(x)))
#define M_PI 3.14159265358979323846

#endif

namespace tl
{

MsdProperties::MsdProperties()
  : mThresholdSaliency(250),
    mPatchRadius(3),
    mKNN(4),
    mAreaRadius(5),
    mScaleFactor(1.25),
    mNMSRadius(5),
    mNScales(-1),
    mNMSScaleR(0),
    mComputeOrientations(false),
    mAffineMSD(false),
    mAffineTilts(3)
{
}

MsdProperties::MsdProperties(const MsdProperties &msd)
  : Msd(msd),
    mThresholdSaliency(msd.mThresholdSaliency),
    mPatchRadius(msd.mPatchRadius),
    mKNN(msd.mKNN),
    mAreaRadius(msd.mAreaRadius),
    mScaleFactor(msd.mScaleFactor),
    mNMSRadius(msd.mNMSRadius),
    mNScales(msd.mNScales),
    mNMSScaleR(msd.mNMSScaleR),
    mComputeOrientations(msd.mComputeOrientations),
    mAffineMSD(msd.mAffineMSD),
    mAffineTilts(msd.mAffineTilts)
{
}

MsdProperties::MsdProperties(MsdProperties &&msd) TL_NOEXCEPT
  : mThresholdSaliency(msd.mThresholdSaliency),
    mPatchRadius(msd.mPatchRadius),
    mKNN(msd.mKNN),
    mAreaRadius(msd.mAreaRadius),
    mScaleFactor(msd.mScaleFactor),
    mNMSRadius(msd.mNMSRadius),
    mNScales(msd.mNScales),
    mNMSScaleR(msd.mNMSScaleR),
    mComputeOrientations(msd.mComputeOrientations),
    mAffineMSD(msd.mAffineMSD),
    mAffineTilts(msd.mAffineTilts)
{
}

MsdProperties::~MsdProperties() = default;

auto MsdProperties::operator =(const MsdProperties &msd) -> MsdProperties&
{
    if (this != &msd) {
        mThresholdSaliency = msd.mThresholdSaliency;
        mPatchRadius = msd.mPatchRadius;
        mKNN = msd.mKNN;
        mAreaRadius = msd.mAreaRadius;
        mScaleFactor = msd.mScaleFactor;
        mNMSRadius = msd.mNMSRadius;
        mNScales = msd.mNScales;
        mNMSScaleR = msd.mNMSScaleR;
        mComputeOrientations = msd.mComputeOrientations;
        mAffineMSD = msd.mAffineMSD;
        mAffineTilts = msd.mAffineTilts;
    }
    return *this;
}

auto MsdProperties::operator =(MsdProperties &&msd) TL_NOEXCEPT -> MsdProperties&
{
    if (this != &msd) {
        mThresholdSaliency = msd.mThresholdSaliency;
        mPatchRadius = msd.mPatchRadius;
        mKNN = msd.mKNN;
        mAreaRadius = msd.mAreaRadius;
        mScaleFactor = msd.mScaleFactor;
        mNMSRadius = msd.mNMSRadius;
        mNScales = msd.mNScales;
        mNMSScaleR = msd.mNMSScaleR;
        mComputeOrientations = msd.mComputeOrientations;
        mAffineMSD = msd.mAffineMSD;
        mAffineTilts = msd.mAffineTilts;
    }
    return *this;
}

double MsdProperties::thresholdSaliency() const
{
    return mThresholdSaliency;
}

int MsdProperties::patchRadius() const
{
    return mPatchRadius;
}

int MsdProperties::knn() const
{
    return mKNN;
}

int MsdProperties::searchAreaRadius() const
{
    return mAreaRadius;
}

double MsdProperties::scaleFactor() const
{
    return mScaleFactor;
}

int MsdProperties::NMSRadius() const
{
    return mNMSRadius;
}

int MsdProperties::nScales() const
{
    return mNScales;
}

int MsdProperties::NMSScaleRadius() const
{
    return mNMSScaleR;
}

bool MsdProperties::computeOrientation() const
{
    return mComputeOrientations;
}

bool MsdProperties::affineMSD() const
{
    return mAffineMSD;
}

int MsdProperties::affineTilts() const
{
    return mAffineTilts;
}

void MsdProperties::setThresholdSaliency(double thresholdSaliency)
{
    mThresholdSaliency = thresholdSaliency;
}

void MsdProperties::setPatchRadius(int patchRadius)
{
    mPatchRadius = patchRadius;
}

void MsdProperties::setKNN(int knn)
{
    mKNN = knn;
}

void MsdProperties::setSearchAreaRadius(int areaRadius)
{
    mAreaRadius = areaRadius;
}

void MsdProperties::setScaleFactor(double scaleFactor)
{
    mScaleFactor = scaleFactor;
}

void MsdProperties::setNMSRadius(int NMSRadius)
{
    mNMSRadius = NMSRadius;
}

void MsdProperties::setNScales(int nScales)
{
    mNScales = nScales;
}

void MsdProperties::setNMSScaleRadius(int NMSScaleR)
{
    mNMSScaleR = NMSScaleR;
}

void MsdProperties::setComputeOrientation(bool computeOrientations)
{
    mComputeOrientations = computeOrientations;
}

void MsdProperties::setAffineMSD(bool affineMSD)
{
    mAffineMSD = affineMSD;
}

void MsdProperties::setAffineTilts(int affineTilts)
{
    mAffineTilts = affineTilts;
}

void MsdProperties::reset()
{
    mThresholdSaliency = 250;
    mPatchRadius = 3;
    mKNN = 4;
    mAreaRadius = 5;
    mScaleFactor = 1.25;
    mNMSRadius = 5;
    mNScales = -1;
    mNMSScaleR = 0;
    mComputeOrientations = false;
    mAffineMSD = false;
    mAffineTilts = 3;
}

std::string MsdProperties::name() const
{
    return std::string("MSD");
}



/*----------------------------------------------------------------*/


MsdDetector::MsdDetector()
  : KeypointDetector()
{
#if CV_VERSION_MAJOR < 3 || (CV_VERSION_MAJOR == 3 && CV_VERSION_MINOR < 1) || !defined HAVE_OPENCV_XFEATURES2D
    mMSD = std::make_shared<::MsdDetector>();
#endif

    update();
}

MsdDetector::MsdDetector(double thresholdSaliency,
                         int pathRadius,
                         int knn,
                         int areaRadius,
                         double scaleFactor,
                         int NMSRadius,
                         int nScales,
                         int NMSScaleR,
                         bool computeOrientations,
                         bool affineMSD,
                         int affineTilts)
  : KeypointDetector()
{
#if CV_VERSION_MAJOR < 3 || (CV_VERSION_MAJOR == 3 && CV_VERSION_MINOR < 1)  || !defined HAVE_OPENCV_XFEATURES2D
    mMSD = std::make_shared<::MsdDetector>();
#endif

    MsdProperties::setThresholdSaliency(thresholdSaliency);
    MsdProperties::setPatchRadius(pathRadius);
    MsdProperties::setKNN(knn);
    MsdProperties::setSearchAreaRadius(areaRadius);
    MsdProperties::setScaleFactor(scaleFactor);
    MsdProperties::setNMSRadius(NMSRadius);
    MsdProperties::setNScales(nScales);
    MsdProperties::setNMSScaleRadius(NMSScaleR);
    MsdProperties::setComputeOrientation(computeOrientations);
    MsdProperties::setAffineMSD(affineMSD);
    MsdProperties::setAffineTilts(affineTilts);

    update();
}

MsdDetector::~MsdDetector() = default;

void MsdDetector::update()
{
#if (CV_VERSION_MAJOR > 3 || (CV_VERSION_MAJOR == 3 && CV_VERSION_MINOR >= 1)) && defined HAVE_OPENCV_XFEATURES2D
    mMSD = cv::xfeatures2d::MSDDetector::create(MsdProperties::patchRadius(),
                                                MsdProperties::searchAreaRadius(),
                                                MsdProperties::NMSRadius(),
                                                MsdProperties::NMSScaleRadius(),
                                                static_cast<float>(MsdProperties::thresholdSaliency()),
                                                MsdProperties::knn(),
                                                static_cast<float>(MsdProperties::scaleFactor()),
                                                MsdProperties::nScales(),
                                                MsdProperties::computeOrientation());
#else
    mMSD->setThSaliency(static_cast<float>(MsdProperties::thresholdSaliency()));
    mMSD->setPatchRadius(MsdProperties::patchRadius());
    mMSD->setKNN(MsdProperties::knn());
    mMSD->setSearchAreaRadius(MsdProperties::searchAreaRadius());
    mMSD->setScaleFactor(static_cast<float>(MsdProperties::scaleFactor()));
    mMSD->setNMSRadius(MsdProperties::NMSRadius());
    mMSD->setNScales(MsdProperties::nScales());
    mMSD->setNMSScaleRadius(MsdProperties::NMSScaleRadius());
    mMSD->setComputeOrientation(MsdProperties::computeOrientation());
#endif
}

auto MsdDetector::detect(const cv::Mat &img, cv::InputArray &mask) -> std::vector<cv::KeyPoint>
{
    std::vector<cv::KeyPoint> key_points;

    try {

#if (CV_VERSION_MAJOR > 3 || (CV_VERSION_MAJOR == 3 && CV_VERSION_MINOR >= 1)) && defined HAVE_OPENCV_XFEATURES2D

        mMSD->detect(img, key_points, mask);

#else
    tl::unusedParameter(mask);
    
        if (MsdProperties::affineMSD()) {

            float maxX = 0;
            float maxY = 0;

            int affineTilts = MsdProperties::affineTilts();
            for (int tl = 1; tl <= affineTilts; tl++) {
                double t = pow(2, 0.5 * tl);
                for (double phi = 0.; phi < 180.; phi += 72.0 / t) {
                    std::vector<cv::KeyPoint> kps;
                    cv::Mat timg, mask, Ai;

                    img.copyTo(timg);
                    affineSkew(t, phi, timg, mask, Ai);

                    kps = mMSD->detect(timg);


                    for (unsigned int i = 0; i < kps.size(); i++) {
                        cv::Point3f kpt(kps[i].pt.x, kps[i].pt.y, 1);
                        cv::Mat kpt_t = Ai * cv::Mat(kpt);

                        kps[i].pt.x = kpt_t.at<float>(0, 0);
                        kps[i].pt.y = kpt_t.at<float>(1, 0);
                        if (phi == 0. || pointIsAcceptable(kps[i], img.cols, img.rows)) {
                            if (kps[i].pt.x > maxX) {
                                maxX = kps[i].pt.x;
                            }
                            if (kps[i].pt.y > maxY) {
                                maxY = kps[i].pt.y;
                            }
                            key_points.push_back(kps[i]);
                        }
                        kpt_t.release();
                    }


                    timg.release();
                    mask.release();
                    Ai.release();
                }
            }

        } else {

            cv::Mat img2;
            img.copyTo(img2);
            key_points = mMSD->detect(img2);

        }

#endif

    } catch (...) {
        TL_THROW_EXCEPTION_WITH_NESTED("Catched exception");
    }

    return key_points;
}

#if (CV_VERSION_MAJOR < 3 || (CV_VERSION_MAJOR == 3 && CV_VERSION_MINOR < 1)) || !defined HAVE_OPENCV_XFEATURES2D

void MsdDetector::compensate_affine_coor1(float *x0, float *y0, int w1, int h1, float t1, float t2, float Rtheta)
{
    float x_ori, y_ori;
    float x_tmp, y_tmp;

    float x1 = *x0;
    float y1 = *y0;


    Rtheta = Rtheta * M_PI / 180.f;

    if (Rtheta <= M_PI / 2.f) {
        x_ori = 0;
        y_ori = w1 * sin(Rtheta) / t1;
    } else {
        x_ori = -w1 * cos(Rtheta) / t2;
        y_ori = (w1 * sin(Rtheta) + h1 * sin(Rtheta - M_PI / 2.f)) / t1;
    }

    float sin_Rtheta = sin(Rtheta);
    float cos_Rtheta = cos(Rtheta);


    /* project the coordinates of im1 to original image before tilt-rotation transform */
    /* Get the coordinates with respect to the 'origin' of the original image before transform */
    x1 = x1 - x_ori;
    y1 = y1 - y_ori;
    /* Invert tilt */
    x1 = x1 * t2;
    y1 = y1 * t1;
    /* Invert rotation (Note that the y direction (vertical) is inverse to the usual concention. Hence Rtheta instead of -Rtheta to inverse the rotation.) */
    x_tmp = cos_Rtheta * x1 - sin_Rtheta * y1;
    y_tmp = sin_Rtheta * x1 + cos_Rtheta * y1;
    x1 = x_tmp;
    y1 = y_tmp;

    *x0 = x1;
    *y0 = y1;
}



bool MsdDetector::pointIsAcceptable(const cv::KeyPoint &vl_keypoint, int width, int height)
{
    bool retVal = false;
    double x0, y0, vx1, vy1, vx2, vy2, vx3, vy3, vx4, vy4, d1, d2, d3, d4, scale1, BorderTh;

    x0 = vl_keypoint.pt.x;
    y0 = vl_keypoint.pt.y;
    scale1 = vl_keypoint.size;

    if (x0 <= 0 || y0 <= 0 || x0 >= width || y0 >= height) {
        return false;
    }

    vx1 = 0;
    vy1 = 0;

    vx2 = 0;
    vy2 = height;

    vx3 = width;
    vy3 = 0;

    vx4 = width;
    vy4 = height;

    d1 = sqrt(pow((x0 - vx1), 2) + pow((y0 - vy1), 2));
    d2 = sqrt(pow((x0 - vx2), 2) + pow((y0 - vy2), 2));
    d3 = sqrt(pow((x0 - vx3), 2) + pow((y0 - vy3), 2));
    d4 = sqrt(pow((x0 - vx4), 2) + pow((y0 - vy4), 2));

    float BorderFact = width / 100.f * sqrt(2.f);
    //if (scale1 < 1){ scale1 = 1;}
    BorderTh = BorderFact * scale1;

    if (d1 < BorderTh || d2 < BorderTh || d3 < BorderTh || d4 < BorderTh) {
        retVal = false;
    } else {
        retVal = true;
    }

    return retVal;
}

void MsdDetector::affineSkew(double tilt, double phi, cv::Mat &img, cv::Mat &mask, cv::Mat &Ai)
{
    int h = img.rows;
    int w = img.cols;

    mask = cv::Mat(h, w, CV_8UC1, cv::Scalar(255));

    cv::Mat A = cv::Mat::eye(2, 3, CV_32F);

    if (phi != 0.0) {
        phi *= CV_PI / 180.;
        double s = sin(phi);
        double c = cos(phi);

        A = (cv::Mat_<float>(2, 2) << c, -s, s, c);

        cv::Mat corners = (cv::Mat_<float>(4, 2) << 0, 0, w, 0, w, h, 0, h);
        cv::Mat tcorners = corners * A.t();
        cv::Mat tcorners_x, tcorners_y;
        tcorners.col(0).copyTo(tcorners_x);
        tcorners.col(1).copyTo(tcorners_y);
        std::vector<cv::Mat> channels;
        channels.push_back(tcorners_x);
        channels.push_back(tcorners_y);
        merge(channels, tcorners);

        cv::Rect rect = boundingRect(tcorners);
        A = (cv::Mat_<float>(2, 3) << c, -s, -rect.x, s, c, -rect.y);

        cv::warpAffine(img, img, A, cv::Size(rect.width, rect.height), cv::INTER_LINEAR, cv::BORDER_REPLICATE);
    }
    if (tilt != 1.0) {
        double s = 0.8 * sqrt(tilt * tilt - 1);
        GaussianBlur(img, img, cv::Size(0, 0), s, 0.01);
        resize(img, img, cv::Size(0, 0), 1.0 / tilt, 1.0, cv::INTER_NEAREST);
        A.row(0) = A.row(0) / tilt;
    }
    if (tilt != 1.0 || phi != 0.0) {
        h = img.rows;
        w = img.cols;
        cv::warpAffine(mask, mask, A, cv::Size(w, h), cv::INTER_NEAREST);
    }
    invertAffineTransform(A, Ai);
}

#endif

void MsdDetector::setThresholdSaliency(double thresholdSaliency)
{
    MsdProperties::setThresholdSaliency(thresholdSaliency);
#if defined HAVE_OPENCV_XFEATURES2D && (CV_VERSION_MAJOR > 3 || (CV_VERSION_MAJOR == 3 && CV_VERSION_MINOR >= 1))
    update();
#else
    mMSD->setThSaliency(static_cast<float>(thresholdSaliency));
#endif
}

void MsdDetector::setPatchRadius(int pathRadius)
{
    MsdProperties::setPatchRadius(pathRadius);
#if defined HAVE_OPENCV_XFEATURES2D && (CV_VERSION_MAJOR > 3 || (CV_VERSION_MAJOR == 3 && CV_VERSION_MINOR >= 1))
    update();
#else
    mMSD->setPatchRadius(pathRadius);
#endif
}

void MsdDetector::setKNN(int knn)
{
    MsdProperties::setKNN(knn);
#if defined HAVE_OPENCV_XFEATURES2D && (CV_VERSION_MAJOR > 3 || (CV_VERSION_MAJOR == 3 && CV_VERSION_MINOR >= 1))
    update();
#else
    mMSD->setKNN(knn);
#endif
}

void MsdDetector::setSearchAreaRadius(int areaRadius)
{
    MsdProperties::setSearchAreaRadius(areaRadius);
#if defined HAVE_OPENCV_XFEATURES2D && (CV_VERSION_MAJOR > 3 || (CV_VERSION_MAJOR == 3 && CV_VERSION_MINOR >= 1))
    update();
#else
    mMSD->setSearchAreaRadius(areaRadius);
#endif
}

void MsdDetector::setScaleFactor(double scaleFactor)
{
    MsdProperties::setScaleFactor(scaleFactor);
#if defined HAVE_OPENCV_XFEATURES2D && (CV_VERSION_MAJOR > 3 || (CV_VERSION_MAJOR == 3 && CV_VERSION_MINOR >= 1))
    update();
#else
    mMSD->setScaleFactor(static_cast<float>(scaleFactor));
#endif
}

void MsdDetector::setNMSRadius(int NMSRadius)
{
    MsdProperties::setNMSRadius(NMSRadius);
#if defined HAVE_OPENCV_XFEATURES2D && (CV_VERSION_MAJOR > 3 || (CV_VERSION_MAJOR == 3 && CV_VERSION_MINOR >= 1))
    update();
#else
    mMSD->setNMSRadius(NMSRadius);
#endif
}

void MsdDetector::setNScales(int nScales)
{
    MsdProperties::setNScales(nScales);
#if defined HAVE_OPENCV_XFEATURES2D && (CV_VERSION_MAJOR > 3 || (CV_VERSION_MAJOR == 3 && CV_VERSION_MINOR >= 1))
    update();
#else
    mMSD->setNScales(nScales);
#endif
}

void MsdDetector::setNMSScaleRadius(int NMSScaleR)
{
    MsdProperties::setNMSScaleRadius(NMSScaleR);
#if defined HAVE_OPENCV_XFEATURES2D && (CV_VERSION_MAJOR > 3 || (CV_VERSION_MAJOR == 3 && CV_VERSION_MINOR >= 1))
    update();
#else
    mMSD->setScaleFactor(NMSScaleR);
#endif
}

void MsdDetector::setComputeOrientation(bool computeOrientations)
{
    MsdProperties::setComputeOrientation(computeOrientations);
#if defined HAVE_OPENCV_XFEATURES2D && (CV_VERSION_MAJOR > 3 || (CV_VERSION_MAJOR == 3 && CV_VERSION_MINOR >= 1))
    update();
#else
    mMSD->setComputeOrientation(computeOrientations);
#endif
}

void MsdDetector::setAffineMSD(bool affineMSD)
{
    MsdProperties::setAffineMSD(affineMSD);
}

void MsdDetector::setAffineTilts(int tilts)
{
    MsdProperties::setAffineTilts(tilts);
}

void MsdDetector::reset()
{
    MsdProperties::reset();
    update();
}


} // namespace tl
