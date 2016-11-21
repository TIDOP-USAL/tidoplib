#include "img_processing.h"

#include "core/messages.h"

#include "opencv2/highgui/highgui.hpp"

#include <cstdarg>
#include <cstdio>


namespace I3D
{


/* ---------------------------------------------------------------------------------- */

void ImgProcessingList::add(const std::shared_ptr<ImgProcessing> &ip)
{
  mProcessList.push_back(ip);
}

int ImgProcessingList::execute(const cv::Mat &matIn, cv::Mat *matOut) const
{
  int i_error = 0;
  if (matIn.empty()) {
    i_error = 1;
  } else {
    matIn.copyTo(*matOut);
    for (auto process : mProcessList) {
      if (int i_error = process->execute(*matOut, matOut))
        return i_error; //Error
    }
  }
  return i_error;
}

/* ---------------------------------------------------------------------------------- */

int BilateralFilter::execute(const cv::Mat &matIn, cv::Mat *matOut) const
{
  int i_error = 0;
  cv::Mat mat_aux = cv::Mat::zeros(matIn.size(), CV_8UC1);
  try {
    cv::bilateralFilter(matIn, mat_aux, mDiameter, mSigmaColor, mSigmaSpace, mBorderType);
    mat_aux.copyTo(*matOut);
  } catch (cv::Exception &e){
    logPrintError(e.what());
    i_error = 1;
  }
  return i_error;
}

void BilateralFilter::setParameters(int diameter, double sigmaColor, double sigmaSpace, int borderType)
{
  mDiameter = diameter;
  mSigmaColor = sigmaColor;
  mSigmaSpace = sigmaSpace;
  mBorderType = borderType;
}

/* ---------------------------------------------------------------------------------- */

int Blur::execute(const cv::Mat &matIn, cv::Mat *matOut) const
{
  int i_error = 0;
  try {
    blur(matIn, *matOut, mKernelSize, mAnchor, mBorderType);
  } catch (cv::Exception &e){
    logPrintError(e.what());
    i_error = 1;
  }
  return i_error;
}

void Blur::setParameters(cv::Size ksize, cv::Point anchor, int borderType)
{
  mKernelSize = ksize;
  mAnchor = anchor;
  mBorderType = borderType;
}

/* ---------------------------------------------------------------------------------- */

int BoxFilter::execute(const cv::Mat &matIn, cv::Mat *matOut) const
{
  int i_error = 0;
  try {
    boxFilter(matIn, *matOut, mDepth, mKernelSize, mAnchor, mNormalize, mBorderType);
  } catch (cv::Exception &e){
    logPrintError(e.what());
    i_error = 1;
  }
  return i_error;
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

int Filter2D::execute(const cv::Mat &matIn, cv::Mat *matOut) const
{
  int i_error = 0;
  try {
    cv::filter2D(matIn, *matOut, mDepth, mKernel, mAnchor, mDelta, mBorderType);
  } catch (cv::Exception &e){
    logPrintError(e.what());
    i_error = 1;
  }
  return i_error;
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

int GaussianBlur::execute(const cv::Mat &matIn, cv::Mat *matOut) const
{
  int i_error = 0;
  try {
    cv::GaussianBlur(matIn, *matOut, mKernelSize, mSigmaX, mSigmaY, mBorderType);
  } catch (cv::Exception &e){
    logPrintError(e.what());
    i_error = 1;
  }
  return i_error;
}

void GaussianBlur::setParameters(cv::Size size, double sigmax, double sigmay, int bordertype)
{
  mKernelSize = size;
  mSigmaX = sigmax;
  mSigmaY = sigmay;
  mBorderType = bordertype;
}

/* ---------------------------------------------------------------------------------- */

int Laplacian::execute(const cv::Mat &matIn, cv::Mat *matOut) const
{
  int i_error = 0;
  try {
    cv::Laplacian(matIn, *matOut, mDepth, mKernelsize, mScale, mDelta, mBorderType);
  } catch (cv::Exception &e) {
    logPrintError(e.what());
    i_error = 1;
  }
  return i_error;
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

int MedianBlur::execute(const cv::Mat &matIn, cv::Mat *matOut) const
{
  int i_error = 0;
  try {
    cv::medianBlur(matIn, *matOut, mKernelSize);
  } catch (cv::Exception &e) {
    logPrintError(e.what());
    i_error = 1;
  }
  return i_error;
}

void MedianBlur::setParameters(int ksize)
{
  mKernelSize = ksize;
}

/* ---------------------------------------------------------------------------------- */

int Sobel::execute(const cv::Mat &matIn, cv::Mat *matOut) const
{
  int i_error = 0;
  cv::Mat grad_x, grad_y;
  cv::Mat abs_grad_x, abs_grad_y;
  try {
    cv::Sobel(matIn, grad_x, mDepth, mDx, mDy, mKernelSize, mScale, mDelta, mBorderType);
    convertScaleAbs(grad_x, abs_grad_x);
    threshold(abs_grad_x, *matOut, mThresh, mMaxVal, cv::THRESH_BINARY);
  } catch (cv::Exception &e){
    logPrintError(e.what());
    i_error = 1;
  }
  return i_error;
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

int Canny::execute(const cv::Mat &matIn, cv::Mat *matOut) const
{
  int i_error = 0;
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
    i_error = 1;
  }
  return i_error;
}

void Canny::setParameters(double threshold1, double threshold2)
{
  mThreshold1 = threshold1;
  mThreshold2 = threshold2;
}

/* ---------------------------------------------------------------------------------- */

int Normalize::execute(const cv::Mat &matIn, cv::Mat *matOut) const
{
  int i_error = 0;
  try {
    cv::normalize(matIn, *matOut, mLowRange, mUpRange, cv::NORM_MINMAX);
  } catch (cv::Exception &e){
    logPrintError(e.what());
    i_error = 1;
  }
  return i_error;
}

void Normalize::setParameters(double _lowRange, double _upRange)
{
  mLowRange = _lowRange;
  mUpRange = _upRange;
}

/* ---------------------------------------------------------------------------------- */

int Binarize::execute(const cv::Mat &matIn, cv::Mat *matOut) const
{
  int i_error = 0;
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
    i_error = 1;
  }
  return i_error;
}

void Binarize::setParameters(double thresh, double maxval, bool inverse)
{
  mThresh = thresh;
  mMaxVal = maxval;
  bInverse = inverse;
}

/* ---------------------------------------------------------------------------------- */

int EqualizeHistogram::execute(const cv::Mat &matIn, cv::Mat *matOut) const
{
  int i_error = 0;
  try {
    cv::equalizeHist(matIn, *matOut);
  } catch (cv::Exception &e){
    logPrintError(e.what());
    i_error = 1;
  }
  return i_error;
}

/* ---------------------------------------------------------------------------------- */

int FunctionProcess::execute(const cv::Mat &matIn, cv::Mat *matOut) const
{
  int i_error = 0;
  try {
    f(matIn, matOut);
  }
  catch (cv::Exception &e){
    logPrintError(e.what());
    i_error = 1;
  }
  return i_error;
}

/* ---------------------------------------------------------------------------------- */

int morphologicalOperation::execute(const cv::Mat &matIn, cv::Mat *matOut) const
{
  int i_error = 0;
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
      i_error = 1;
      break;
    }
  } catch (cv::Exception &e) {
    logPrintError(e.what());
    i_error = 1;
  }
  return i_error;
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

int Resize::execute(const cv::Mat &matIn, cv::Mat *matOut) const
{
  int i_error = 0;
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
    i_error = 1;
  }
  return i_error;
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

int ResizeCanvas::execute(const cv::Mat &matIn, cv::Mat *matOut) const
{
  int i_error = 0;
  try {
    cv::Mat aux = cv::Mat::zeros(cv::Size(mWidth,mHeight),matIn.type());
    matIn.copyTo(aux.colRange(0, matIn.cols).rowRange(0, matIn.rows));
    *matOut = aux;
  } catch (cv::Exception &e){
    printError(e.what());
    i_error = 1;
  }
  return i_error;
}

void ResizeCanvas::setParameters(int width, int height/*, const Color &color = Color()*/)
{
  mWidth = width;
  mHeight = height;
  /*mColor = color;*/
}


/* ---------------------------------------------------------------------------------- */

} // End namespace I3D
