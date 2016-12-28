#include "img_processing.h"

#include "core/messages.h"

#include "opencv2/highgui/highgui.hpp"
#include "opencv2/xphoto/white_balance.hpp"

#include <cstdarg>
#include <cstdio>


namespace I3D
{


/* ---------------------------------------------------------------------------------- */

void ImgProcessingList::add(const std::shared_ptr<ImgProcessing> &ip)
{
  mProcessList.push_back(ip);
}

ProcessExit ImgProcessingList::execute(const cv::Mat &matIn, cv::Mat *matOut) const
{
  if (matIn.empty()) return ProcessExit::FAILURE;
  matIn.copyTo(*matOut);
  for (const auto process : mProcessList) {
    if (process->execute(*matOut, matOut) == ProcessExit::FAILURE)
      return ProcessExit::FAILURE;
  }
  return ProcessExit::SUCCESS;
}

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

ProcessExit morphologicalOperation::execute(const cv::Mat &matIn, cv::Mat *matOut) const
{
  try {
    cv::Mat element = getStructuringElement(mShapes, cv::Size(2 * mSize + 1, 2 * mSize + 1), cv::Point(mSize, mSize));
    switch (type)
    {
    case I3D::process_type::MORPH_DILATION:
      cv::dilate(matIn, *matOut, element, mAnchor, mIterations, mBorderType);
      break;
    case I3D::process_type::MORPH_EROTION:
      cv::erode(matIn, *matOut, element, mAnchor, mIterations, mBorderType);
      break;
    case I3D::process_type::MORPH_OPENING:
      morphologyEx(matIn, *matOut, cv::MORPH_OPEN, element);
      break;
    case I3D::process_type::MORPH_CLOSING:
      morphologyEx(matIn, *matOut, cv::MORPH_CLOSE, element);
      break;
    case I3D::process_type::MORPH_GRADIENT:
      morphologyEx(matIn, *matOut, cv::MORPH_GRADIENT, element);
      break;
    case I3D::process_type::MORPH_TOPHAT:
      morphologyEx(matIn, *matOut, cv::MORPH_TOPHAT, element);
      break;
    case I3D::process_type::MORPH_BLACKHAT:
      morphologyEx(matIn, *matOut, cv::MORPH_BLACKHAT, element);
      break;
    default:
      return ProcessExit::FAILURE;
    }
  } catch (cv::Exception &e) {
    logPrintError(e.what());
    return ProcessExit::FAILURE;
  }
  return ProcessExit::SUCCESS;
}

void morphologicalOperation::setParameters(int size, cv::MorphShapes shapes, cv::Point anchor, int iterations, int borderType, const cv::Scalar &borderValue)
{
  mSize = size;
  mShapes = shapes;
  mAnchor = anchor;
  mIterations = iterations;
  mBorderType = borderType;
  mBorderValue = borderValue;
}

/* ---------------------------------------------------------------------------------- */


ProcessExit Resize::execute(const cv::Mat &matIn, cv::Mat *matOut) const
{
  try {
    if (mWidth == 0 && mScaleX == 0) throw std::runtime_error("Invalid parameter values");
    if (mScaleX) {
      cv::resize(matIn, *matOut, cv::Size(), mScaleX/100., mScaleY/100.);
    } else {
      if (mHeight == 0) {
        double scale = static_cast<double>(mWidth / matIn.cols);
        cv::resize(matIn, *matOut, cv::Size(), scale, scale);
      } else {
        cv::resize(matIn, *matOut, cv::Size(mWidth, mHeight));
      }
    }

  } catch (cv::Exception &e){
    printError(e.what());
    return ProcessExit::FAILURE;
  }
  return ProcessExit::SUCCESS;
}

void Resize::setParameters(int width, int height = 0)
{
  mWidth = width;
  mHeight = height;
  mScaleX = mScaleY = 0.;
}

void Resize::setParameters(double scaleX, double scaleY)
{
  mWidth = mHeight = 0;
  mScaleX = scaleX;
  mScaleY = scaleY ? scaleY : scaleX;
}

/* ---------------------------------------------------------------------------------- */


ProcessExit ResizeCanvas::execute(const cv::Mat &matIn, cv::Mat *matOut) const
{
  try {
    cv::Mat aux = cv::Mat::zeros(cv::Size(mWidth,mHeight),matIn.type());
    matIn.copyTo(aux.colRange(0, matIn.cols).rowRange(0, matIn.rows));
    *matOut = aux;
  } catch (cv::Exception &e){
    printError(e.what());
    return ProcessExit::FAILURE;
  }
  return ProcessExit::SUCCESS;
}

void ResizeCanvas::setParameters(int width, int height, const Color &color, const Position &position)
{
  mWidth = width;
  mHeight = height;
  mPosition = position;
  mColor = color;
}

//void ResizeCanvas::update()
//{
//  switch (mPosition) {
//    case I3D::ResizeCanvas::Position::BOTTOM_CENTER:
//      
//      break;
//    case I3D::ResizeCanvas::Position::BOTTOM_LEFT:
//      break;
//    case I3D::ResizeCanvas::Position::BOTTOM_RIGHT:
//      break;
//    case I3D::ResizeCanvas::Position::CENTER:
//      break;
//    case I3D::ResizeCanvas::Position::CENTER_LEFT:
//      break;
//    case I3D::ResizeCanvas::Position::CENTER_RIGHT:
//      break;
//    case I3D::ResizeCanvas::Position::TOP_CENTER:
//      break;
//    case I3D::ResizeCanvas::Position::TOP_LEFT:
//      mTopLeft = cv::Point(0, 0);
//      break;
//    case I3D::ResizeCanvas::Position::TOP_RIGHT:
//      mTopLeft = cv::Point(0, 0);
//      break;
//    default:
//      break;
//  }
//}

/* ---------------------------------------------------------------------------------- */


ProcessExit Normalize::execute(const cv::Mat &matIn, cv::Mat *matOut) const
{
  try {
    cv::normalize(matIn, *matOut, mLowRange, mUpRange, cv::NORM_MINMAX);
  } catch (cv::Exception &e){
    logPrintError(e.what());
    return ProcessExit::FAILURE;
  }
  return ProcessExit::SUCCESS;
}

void Normalize::setParameters(double _lowRange, double _upRange)
{
  mLowRange = _lowRange;
  mUpRange = _upRange;
}

/* ---------------------------------------------------------------------------------- */


ProcessExit Binarize::execute(const cv::Mat &matIn, cv::Mat *matOut) const
{
  double th = mThresh, max = mMaxVal;
  try {
    if (th == 0.0 && max == 0.0) {
      cv::Scalar m, stdv;
      cv::meanStdDev(matIn, m, stdv);
      th = m[0] - stdv[0];
      max = m[0] + stdv[0];
    }
    cv::threshold(matIn, *matOut, th, max, bInverse ? cv::THRESH_BINARY_INV : cv::THRESH_BINARY);
  } catch (cv::Exception &e){
    logPrintError(e.what());
    return ProcessExit::FAILURE;
  }
  return ProcessExit::SUCCESS;
}

void Binarize::setParameters(double thresh, double maxval, bool inverse)
{
  mThresh = thresh;
  mMaxVal = maxval;
  bInverse = inverse;
}

/* ---------------------------------------------------------------------------------- */


ProcessExit EqualizeHistogram::execute(const cv::Mat &matIn, cv::Mat *matOut) const
{
  try {
    cv::equalizeHist(matIn, *matOut);
  } catch (cv::Exception &e){
    logPrintError(e.what());
    return ProcessExit::FAILURE;
  }
  return ProcessExit::SUCCESS;
}

/* ---------------------------------------------------------------------------------- */


ProcessExit FunctionProcess::execute(const cv::Mat &matIn, cv::Mat *matOut) const
{
  try {
    f(matIn, matOut);
  } catch (cv::Exception &e){
    logPrintError(e.what());
    return ProcessExit::FAILURE;
  }
  return ProcessExit::SUCCESS;
}

/* ---------------------------------------------------------------------------------- */



} // End namespace I3D
