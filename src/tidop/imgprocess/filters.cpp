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

#include "filters.h"

#ifdef TL_HAVE_OPENCV

#include "tidop/core/messages.h"

namespace tl
{

/* ---------------------------------------------------------------------------------- */

BilateralFilter::BilateralFilter(int diameter, double sigmaColor, double sigmaSpace, int borderType)
  : ImageProcess(ImageProcess::ProcessType::bilateral), 
    mDiameter(diameter), 
    mSigmaColor(sigmaColor), 
    mSigmaSpace(sigmaSpace), 
    mBorderType(borderType) 
{
}

void BilateralFilter::run(const cv::Mat &matIn, cv::Mat &matOut) const
{
  try {

    TL_ASSERT(!matIn.empty(), "Incorrect input data. Empty image");

    cv::Mat mat_aux = cv::Mat::zeros(matIn.size(), CV_8UC1);
    cv::bilateralFilter(matIn, mat_aux, mDiameter, mSigmaColor, mSigmaSpace, mBorderType);
    mat_aux.copyTo(matOut);
  
  } catch (...) {
    TL_THROW_EXCEPTION_WITH_NESTED("");
  }
}

void BilateralFilter::setParameters(int diameter, 
                                    double sigmaColor, 
                                    double sigmaSpace, 
                                    int borderType)
{
  mDiameter = diameter;
  mSigmaColor = sigmaColor;
  mSigmaSpace = sigmaSpace;
  mBorderType = borderType;
}

/* ---------------------------------------------------------------------------------- */

Blur::Blur(const cv::Size &ksize, 
           const cv::Point &anchor, 
           int borderType)
  : ImageProcess(ImageProcess::ProcessType::blur), 
    mKernelSize(ksize), 
    mAnchor(anchor), 
    mBorderType(borderType) 
{
}

void Blur::run(const cv::Mat &matIn, cv::Mat &matOut) const
{
  try {
  
    TL_ASSERT(!matIn.empty(), "Incorrect input data. Empty image");

    cv::blur(matIn, matOut, mKernelSize, mAnchor, mBorderType);

  } catch (...) {
    TL_THROW_EXCEPTION_WITH_NESTED("");
  }
}

void Blur::setParameters(const cv::Size ksize, 
                         const cv::Point &anchor, 
                         int borderType)
{
  mKernelSize = ksize;
  mAnchor = anchor;
  mBorderType = borderType;
}

/* ---------------------------------------------------------------------------------- */

BoxFilter::BoxFilter(int ddepth, 
                     const cv::Size &ksize, 
                     const cv::Point &anchor, 
                     bool normalize, 
                     int borderType)
  : ImageProcess(ImageProcess::ProcessType::box_filter), 
    mDepth(ddepth), 
    mKernelSize(ksize), 
    mAnchor(anchor), 
    mNormalize(normalize), 
    mBorderType(borderType) 
{
}

void BoxFilter::run(const cv::Mat &matIn, cv::Mat &matOut) const
{
  try {
 
    TL_ASSERT(!matIn.empty(), "Incorrect input data. Empty image");

    cv::boxFilter(matIn, matOut, mDepth, mKernelSize, mAnchor, mNormalize, mBorderType);

  } catch (...) {
    TL_THROW_EXCEPTION_WITH_NESTED("");
  }
}

void BoxFilter::setParameters(int ddepth, 
                              const cv::Size &ksize, 
                              const cv::Point &anchor, 
                              bool normalize, 
                              int borderType)
{
  mDepth = ddepth;
  mKernelSize = ksize;
  mAnchor = anchor;
  mNormalize = normalize;
  mBorderType = borderType;
}

/* ---------------------------------------------------------------------------------- */

Convolution::Convolution(int ddepth, 
                         const cv::Mat &kernel, 
                         const cv::Point &anchor, 
                         double delta, 
                         int borderType)
  : ImageProcess(ImageProcess::ProcessType::convolution), 
    mDepth(ddepth), 
    mKernel(kernel), 
    mAnchor(anchor), 
    mDelta(delta), 
    mBorderType(borderType) 
{
}


void Convolution::run(const cv::Mat &matIn, cv::Mat &matOut) const
{
  try {

    TL_ASSERT(!matIn.empty(), "Incorrect input data. Empty image");

    cv::filter2D(matIn, matOut, mDepth, mKernel, mAnchor, mDelta, mBorderType);

  } catch (...) {
    TL_THROW_EXCEPTION_WITH_NESTED("");
  }
}

void Convolution::setParameters(int ddepth, 
                                const cv::Mat &kernel,
                                const cv::Point &anchor, 
                                double delta, 
                                int borderType)
{
  mDepth = ddepth;
  mKernel = kernel;
  mAnchor = anchor;
  mDelta = delta;
  mBorderType = borderType;
}


/* ---------------------------------------------------------------------------------- */

GaussianBlur::GaussianBlur(const cv::Size &kernelSize, 
                           double sigmaX, 
                           double sigmaY, 
                           int borderType)
  : ImageProcess(ProcessType::gaussian_blur), 
    mKernelSize(kernelSize), 
    mSigmaX(sigmaX), 
    mSigmaY(sigmaY), 
    mBorderType(borderType) 
{
}

void GaussianBlur::run(const cv::Mat &matIn, cv::Mat &matOut) const
{
  try {

    TL_ASSERT(!matIn.empty(), "Incorrect input data. Empty image");

    cv::GaussianBlur(matIn, matOut, mKernelSize, mSigmaX, mSigmaY, mBorderType);

  } catch (...) {
    TL_THROW_EXCEPTION_WITH_NESTED("");
  }
}

void GaussianBlur::setParameters(const cv::Size &kernelSize, 
                                 double sigmax, 
                                 double sigmay, 
                                 int bordertype)
{
  mKernelSize = kernelSize;
  mSigmaX = sigmax;
  mSigmaY = sigmay;
  mBorderType = bordertype;
}

/* ---------------------------------------------------------------------------------- */

Laplacian::Laplacian(int ddepth, 
                     int ksize, 
                     double scale, 
                     double delta, 
                     int bordertype)
  : ImageProcess(ProcessType::laplacian), 
    mDepth(ddepth), 
    mKernelsize(ksize), 
    mScale(scale), 
    mDelta(delta), 
    mBorderType(bordertype) 
{
}

void Laplacian::run(const cv::Mat &matIn, cv::Mat &matOut) const
{
  try {

    TL_ASSERT(!matIn.empty(), "Incorrect input data. Empty image");

    cv::Laplacian(matIn, matOut, mDepth, mKernelsize, mScale, mDelta, mBorderType);

  } catch (...) {
    TL_THROW_EXCEPTION_WITH_NESTED("");
  }
}

void Laplacian::setParameters(int ddepth, 
                              int ksize, 
                              double scale, 
                              double delta, 
                              int borderType)
{
  mDepth = ddepth;
  mKernelsize = ksize;
  mScale = scale;
  mDelta = delta;
  mBorderType = borderType;
}

/* ---------------------------------------------------------------------------------- */

MedianBlur::MedianBlur(int ksize)
  : ImageProcess(ProcessType::median_blur), 
    mKernelSize(ksize) 
{
}

void MedianBlur::run(const cv::Mat &matIn, cv::Mat &matOut) const
{
  try {

    TL_ASSERT(!matIn.empty(), "Incorrect input data. Empty image");
  
    cv::medianBlur(matIn, matOut, mKernelSize);

  } catch (...) {
    TL_THROW_EXCEPTION_WITH_NESTED("");
  }
}

void MedianBlur::setParameters(int ksize)
{
  mKernelSize = ksize;
}

/* ---------------------------------------------------------------------------------- */

Sobel::Sobel(int dx, 
             int dy, 
             int ksize, 
             double scale, 
             double delta,
             int ddepth, 
             double thresh, 
             double maxval, 
             int bordertype)
  : ImageProcess(ProcessType::sobel),
    mDx(dx), 
    mDy(dy), 
    mKernelSize(ksize), 
    mScale(scale), 
    mDelta(delta), 
    mDepth(ddepth),
    mThresh(thresh), 
    mMaxVal(maxval), 
    mBorderType(bordertype) 
{
}

void Sobel::run(const cv::Mat &matIn, cv::Mat &matOut) const
{
  try {
    
    TL_ASSERT(!matIn.empty(), "Incorrect input data. Empty image");

    cv::Mat grad_x, grad_y;
    cv::Mat abs_grad_x, abs_grad_y;

    cv::Sobel(matIn, grad_x, mDepth, mDx, mDy, mKernelSize, mScale, mDelta, mBorderType);

    TL_TODO("No tiene mucho sentido añadir esto dentro del filtro Sobel")
    convertScaleAbs(grad_x, abs_grad_x);
    threshold(abs_grad_x, matOut, mThresh, mMaxVal, cv::THRESH_BINARY);

  } catch (...) {
    TL_THROW_EXCEPTION_WITH_NESTED("");
  }

}

void Sobel::setParameters(int dx, int dy, int ksize, double scale, double delta, int ddepth, double thresh, double maxval, int bordertype )
{
  mDx = dx;
  mDy = dy;
  mKernelSize = ksize;
  mScale = scale;
  mDelta = delta;
  mDepth = ddepth;
  mThresh = thresh;
  mMaxVal = maxval;
  mBorderType = bordertype;
}

/* ---------------------------------------------------------------------------------- */

Canny::Canny(double threshold1, double threshold2)
  : ImageProcess(ProcessType::canny), 
    mThreshold1(threshold1), 
    mThreshold2(threshold2) 
{
}

void Canny::run(const cv::Mat &matIn, cv::Mat &matOut) const
{
  try {

    TL_ASSERT(!matIn.empty(), "Incorrect input data. Empty image");

    double th1 = mThreshold1;
    double th2 = mThreshold2;

    if (th1 == 0.0 && th2 == 0.0) {
      cv::Scalar mean;
      cv::Scalar stdv;
      cv::meanStdDev(matIn, mean, stdv);
      th1 = mean[0] - stdv[0];
      th2 = mean[0] + stdv[0];
    }

    cv::Canny(matIn, matOut, th1, th2, 3);

  } catch (...) {
    TL_THROW_EXCEPTION_WITH_NESTED("");
  }
}

void Canny::setParameters(double threshold1, double threshold2)
{
  mThreshold1 = threshold1;
  mThreshold2 = threshold2;
}

/* ---------------------------------------------------------------------------------- */

}

#endif // TL_HAVE_OPENCV
