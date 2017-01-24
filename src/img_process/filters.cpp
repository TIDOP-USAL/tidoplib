#include "filters.h"

#include "core/messages.h"

namespace I3D
{

/* ---------------------------------------------------------------------------------- */

ProcessExit BilateralFilter::execute(const cv::Mat &matIn, cv::Mat *matOut) const
{
  cv::Mat mat_aux = cv::Mat::zeros(matIn.size(), CV_8UC1);
  try {
    cv::bilateralFilter(matIn, mat_aux, mDiameter, mSigmaColor, mSigmaSpace, mBorderType);
    mat_aux.copyTo(*matOut);
  } catch (cv::Exception &e){
    logPrintError(e.what());
    return ProcessExit::FAILURE;
  }
  return ProcessExit::SUCCESS;
}

void BilateralFilter::setParameters(int diameter, double sigmaColor, double sigmaSpace, int borderType)
{
  mDiameter = diameter;
  mSigmaColor = sigmaColor;
  mSigmaSpace = sigmaSpace;
  mBorderType = borderType;
}

/* ---------------------------------------------------------------------------------- */

ProcessExit Blur::execute(const cv::Mat &matIn, cv::Mat *matOut) const
{
  try {
    blur(matIn, *matOut, mKernelSize, mAnchor, mBorderType);
  } catch (cv::Exception &e){
    logPrintError(e.what());
    return ProcessExit::FAILURE;
  }
  return ProcessExit::SUCCESS;
}

void Blur::setParameters(cv::Size ksize, cv::Point anchor, int borderType)
{
  mKernelSize = ksize;
  mAnchor = anchor;
  mBorderType = borderType;
}

/* ---------------------------------------------------------------------------------- */

ProcessExit BoxFilter::execute(const cv::Mat &matIn, cv::Mat *matOut) const
{
  try {
    boxFilter(matIn, *matOut, mDepth, mKernelSize, mAnchor, mNormalize, mBorderType);
  } catch (cv::Exception &e){
    logPrintError(e.what());
    return ProcessExit::FAILURE;
  }
  return ProcessExit::SUCCESS;
}

void BoxFilter::setParameters(int ddepth, cv::Size ksize, cv::Point anchor, bool normalize, int borderType)
{
  mDepth = ddepth;
  mKernelSize = ksize;
  mAnchor = anchor;
  mNormalize = normalize;
  mBorderType = borderType;
}

/* ---------------------------------------------------------------------------------- */

ProcessExit Filter2D::execute(const cv::Mat &matIn, cv::Mat *matOut) const
{
  try {
    cv::filter2D(matIn, *matOut, mDepth, mKernel, mAnchor, mDelta, mBorderType);
  } catch (cv::Exception &e){
    logPrintError(e.what());
    return ProcessExit::FAILURE;
  }
  return ProcessExit::SUCCESS;
}

void Filter2D::setParameters(int ddepth, cv::Mat kernel, cv::Point anchor, double delta, int borderType)
{
  mDepth = ddepth;
  mKernel = kernel;
  mAnchor = anchor;
  mDelta = delta;
  mBorderType = borderType;
}

/* ---------------------------------------------------------------------------------- */

ProcessExit GaussianBlur::execute(const cv::Mat &matIn, cv::Mat *matOut) const
{
  try {
    cv::GaussianBlur(matIn, *matOut, mKernelSize, mSigmaX, mSigmaY, mBorderType);
  } catch (cv::Exception &e){
    logPrintError(e.what());
    return ProcessExit::FAILURE;
  }
  return ProcessExit::SUCCESS;
}

void GaussianBlur::setParameters(cv::Size size, double sigmax, double sigmay, int bordertype)
{
  mKernelSize = size;
  mSigmaX = sigmax;
  mSigmaY = sigmay;
  mBorderType = bordertype;
}

/* ---------------------------------------------------------------------------------- */

ProcessExit Laplacian::execute(const cv::Mat &matIn, cv::Mat *matOut) const
{
  try {
    cv::Laplacian(matIn, *matOut, mDepth, mKernelsize, mScale, mDelta, mBorderType);
  } catch (cv::Exception &e) {
    logPrintError(e.what());
    return ProcessExit::FAILURE;
  }
  return ProcessExit::SUCCESS;
}

void Laplacian::setParameters(int ddepth, int ksize, double scale, double delta, int borderType)
{
  mDepth = ddepth;
  mKernelsize = ksize;
  mScale = scale;
  mDelta = delta;
  mBorderType = borderType;
}

/* ---------------------------------------------------------------------------------- */

ProcessExit MedianBlur::execute(const cv::Mat &matIn, cv::Mat *matOut) const
{
  try {
    cv::medianBlur(matIn, *matOut, mKernelSize);
  } catch (cv::Exception &e) {
    logPrintError(e.what());
    return ProcessExit::FAILURE;
  }
  return ProcessExit::SUCCESS;
}

void MedianBlur::setParameters(int ksize)
{
  mKernelSize = ksize;
}

/* ---------------------------------------------------------------------------------- */

ProcessExit Sobel::execute(const cv::Mat &matIn, cv::Mat *matOut) const
{
  cv::Mat grad_x, grad_y;
  cv::Mat abs_grad_x, abs_grad_y;
  try {
    cv::Sobel(matIn, grad_x, mDepth, mDx, mDy, mKernelSize, mScale, mDelta, mBorderType);
    convertScaleAbs(grad_x, abs_grad_x);
    threshold(abs_grad_x, *matOut, mThresh, mMaxVal, cv::THRESH_BINARY);
  } catch (cv::Exception &e){
    logPrintError(e.what());
    return ProcessExit::FAILURE;
  }
  return ProcessExit::SUCCESS;
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

ProcessExit Canny::execute(const cv::Mat &matIn, cv::Mat *matOut) const
{
  double th1 = mThreshold1, th2 = mThreshold2;
  try {
    if (th1 == 0.0 && th2 == 0.0) {
      cv::Scalar m, stdv;
      cv::meanStdDev(matIn, m, stdv);
      th1 = m[0] - stdv[0];
      th2 = m[0] + stdv[0];
    }
    cv::Canny(matIn, *matOut, th1, th2, 3);
  } catch (cv::Exception &e){
    logPrintError(e.what());
    return ProcessExit::FAILURE;
  }
  return ProcessExit::SUCCESS;
}

void Canny::setParameters(double threshold1, double threshold2)
{
  mThreshold1 = threshold1;
  mThreshold2 = threshold2;
}

/* ---------------------------------------------------------------------------------- */

}