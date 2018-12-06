#include "filters.h"

#ifdef HAVE_OPENCV

#include "tidop/core/messages.h"

namespace TL
{

/* ---------------------------------------------------------------------------------- */

BilateralFilter::BilateralFilter(int diameter, double sigmaColor, double sigmaSpace, int borderType)
  : ImgProcessing(process_type::BILATERAL), 
    mDiameter(diameter), 
    mSigmaColor(sigmaColor), 
    mSigmaSpace(sigmaSpace), 
    mBorderType(borderType) 
{
}

ImgProcessing::Status BilateralFilter::execute(const cv::Mat &matIn, cv::Mat *matOut) const
{
  cv::Mat mat_aux = cv::Mat::zeros(matIn.size(), CV_8UC1);
  try {
    cv::bilateralFilter(matIn, mat_aux, mDiameter, mSigmaColor, mSigmaSpace, mBorderType);
    mat_aux.copyTo(*matOut);
  } catch (cv::Exception &e){
    msgError(e.what());
    return ImgProcessing::Status::PROCESS_ERROR;
  }
  return ImgProcessing::Status::OK;
}

void BilateralFilter::setParameters(int diameter, double sigmaColor, double sigmaSpace, int borderType)
{
  mDiameter = diameter;
  mSigmaColor = sigmaColor;
  mSigmaSpace = sigmaSpace;
  mBorderType = borderType;
}

/* ---------------------------------------------------------------------------------- */

Blur::Blur(cv::Size ksize, cv::Point anchor, int borderType)
  : ImgProcessing(process_type::BLUR), 
    mKernelSize(ksize), 
    mAnchor(anchor), 
    mBorderType(borderType) 
{
}

ImgProcessing::Status Blur::execute(const cv::Mat &matIn, cv::Mat *matOut) const
{
  if (matIn.empty()) return ImgProcessing::Status::INCORRECT_INPUT_DATA;
  try {
    blur(matIn, *matOut, mKernelSize, mAnchor, mBorderType);
  } catch (cv::Exception &e){
    msgError(e.what());
    return ImgProcessing::Status::PROCESS_ERROR;
  }
  return ImgProcessing::Status::OK;
}

void Blur::setParameters(cv::Size ksize, cv::Point anchor, int borderType)
{
  mKernelSize = ksize;
  mAnchor = anchor;
  mBorderType = borderType;
}

/* ---------------------------------------------------------------------------------- */

BoxFilter::BoxFilter(int ddepth, cv::Size ksize, cv::Point anchor, bool normalize, int borderType)
  : ImgProcessing(process_type::BOX_FILTER), 
    mDepth(ddepth), 
    mKernelSize(ksize), 
    mAnchor(anchor), 
    mNormalize(normalize), 
    mBorderType(borderType) 
{
}

ImgProcessing::Status BoxFilter::execute(const cv::Mat &matIn, cv::Mat *matOut) const
{
  if (matIn.empty()) return ImgProcessing::Status::INCORRECT_INPUT_DATA;
  try {
    boxFilter(matIn, *matOut, mDepth, mKernelSize, mAnchor, mNormalize, mBorderType);
  } catch (cv::Exception &e){
    msgError(e.what());
    return ImgProcessing::Status::PROCESS_ERROR;
  }
  return ImgProcessing::Status::OK;
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

// TODO: eliminar Filter2D

// Se desactiva el warning que se establece al hacer la clase deprecated para la propia clase
TL_DISABLE_WARNING(TL_WARNING_DEPRECATED)
Filter2D::Filter2D(int ddepth, cv::Mat kernel, cv::Point anchor, double delta, int borderType)
  : ImgProcessing(process_type::FILTER_2D), 
    mDepth(ddepth), 
    mKernel(kernel), 
    mAnchor(anchor), 
    mDelta(delta), 
    mBorderType(borderType) 
{
}


ImgProcessing::Status Filter2D::execute(const cv::Mat &matIn, cv::Mat *matOut) const
{
  if (matIn.empty()) return ImgProcessing::Status::INCORRECT_INPUT_DATA;
  try {
    cv::filter2D(matIn, *matOut, mDepth, mKernel, mAnchor, mDelta, mBorderType);
  } catch (cv::Exception &e){
    msgError(e.what());
    return ImgProcessing::Status::PROCESS_ERROR;
  }
  return ImgProcessing::Status::OK;
}

void Filter2D::setParameters(int ddepth, cv::Mat kernel, cv::Point anchor, double delta, int borderType)
{
  mDepth = ddepth;
  mKernel = kernel;
  mAnchor = anchor;
  mDelta = delta;
  mBorderType = borderType;
}

TL_ENABLE_WARNING(TL_WARNING_DEPRECATED)

Convolution::Convolution(int ddepth, cv::Mat kernel, cv::Point anchor, double delta, int borderType)
  : ImgProcessing(process_type::CONVOLUTION), 
    mDepth(ddepth), 
    mKernel(kernel), 
    mAnchor(anchor), 
    mDelta(delta), 
    mBorderType(borderType) 
{
}


ImgProcessing::Status Convolution::execute(const cv::Mat &matIn, cv::Mat *matOut) const
{
  if (matIn.empty()) return ImgProcessing::Status::INCORRECT_INPUT_DATA;
  try {
    cv::filter2D(matIn, *matOut, mDepth, mKernel, mAnchor, mDelta, mBorderType);
  } catch (cv::Exception &e){
    msgError(e.what());
    return ImgProcessing::Status::PROCESS_ERROR;
  }
  return ImgProcessing::Status::OK;
}

void Convolution::setParameters(int ddepth, cv::Mat kernel, cv::Point anchor, double delta, int borderType)
{
  mDepth = ddepth;
  mKernel = kernel;
  mAnchor = anchor;
  mDelta = delta;
  mBorderType = borderType;
}


/* ---------------------------------------------------------------------------------- */

GaussianBlur::GaussianBlur(cv::Size size, double sigmaX, double sigmaY, int borderType)
  : ImgProcessing(process_type::GAUSSIAN_BLUR), 
    mKernelSize(size), 
    mSigmaX(sigmaX), 
    mSigmaY(sigmaY), 
    mBorderType(borderType) 
{
}

ImgProcessing::Status GaussianBlur::execute(const cv::Mat &matIn, cv::Mat *matOut) const
{
  if (matIn.empty()) return ImgProcessing::Status::INCORRECT_INPUT_DATA;
  try {
    cv::GaussianBlur(matIn, *matOut, mKernelSize, mSigmaX, mSigmaY, mBorderType);
  } catch (cv::Exception &e){
    msgError(e.what());
    return ImgProcessing::Status::PROCESS_ERROR;
  }
  return ImgProcessing::Status::OK;
}

void GaussianBlur::setParameters(cv::Size size, double sigmax, double sigmay, int bordertype)
{
  mKernelSize = size;
  mSigmaX = sigmax;
  mSigmaY = sigmay;
  mBorderType = bordertype;
}

/* ---------------------------------------------------------------------------------- */

Laplacian::Laplacian(int ddepth, int ksize, double scale , double delta, int bordertype)
  : ImgProcessing(process_type::LAPLACIAN), 
    mDepth(ddepth), 
    mKernelsize(ksize), 
    mScale(scale), 
    mDelta(delta), 
    mBorderType(bordertype) 
{
}

ImgProcessing::Status Laplacian::execute(const cv::Mat &matIn, cv::Mat *matOut) const
{
  if (matIn.empty()) return ImgProcessing::Status::INCORRECT_INPUT_DATA;
  try {
    cv::Laplacian(matIn, *matOut, mDepth, mKernelsize, mScale, mDelta, mBorderType);
  } catch (cv::Exception &e) {
    msgError(e.what());
    return ImgProcessing::Status::PROCESS_ERROR;
  }
  return ImgProcessing::Status::OK;
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

MedianBlur::MedianBlur(int ksize)
  : ImgProcessing(process_type::MEDIAN_BLUR), 
    mKernelSize(ksize) 
{
}

ImgProcessing::Status MedianBlur::execute(const cv::Mat &matIn, cv::Mat *matOut) const
{
  if (matIn.empty()) return ImgProcessing::Status::INCORRECT_INPUT_DATA;
  try {
    cv::medianBlur(matIn, *matOut, mKernelSize);
  } catch (cv::Exception &e) {
    msgError(e.what());
    return ImgProcessing::Status::PROCESS_ERROR;
  }
  return ImgProcessing::Status::OK;
}

void MedianBlur::setParameters(int ksize)
{
  mKernelSize = ksize;
}

/* ---------------------------------------------------------------------------------- */

Sobel::Sobel(int dx, int dy, int ksize, double scale, double delta,
             int ddepth, double thresh, double maxval, int bordertype)
  : ImgProcessing(process_type::SOBEL),
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

ImgProcessing::Status Sobel::execute(const cv::Mat &matIn, cv::Mat *matOut) const
{
  if (matIn.empty()) return ImgProcessing::Status::INCORRECT_INPUT_DATA;
  cv::Mat grad_x, grad_y;
  cv::Mat abs_grad_x, abs_grad_y;
  try {
    cv::Sobel(matIn, grad_x, mDepth, mDx, mDy, mKernelSize, mScale, mDelta, mBorderType);
    //TODO: No tiene mucho sentido añadir esto dentro del filtro Sobel
    convertScaleAbs(grad_x, abs_grad_x);
    threshold(abs_grad_x, *matOut, mThresh, mMaxVal, cv::THRESH_BINARY);
  } catch (cv::Exception &e){
    msgError(e.what());
    return ImgProcessing::Status::PROCESS_ERROR;
  }
  return ImgProcessing::Status::OK;
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
  : ImgProcessing(process_type::CANNY), 
    mThreshold1(threshold1), 
    mThreshold2(threshold2) 
{
}

ImgProcessing::Status Canny::execute(const cv::Mat &matIn, cv::Mat *matOut) const
{
  if (matIn.empty()) return ImgProcessing::Status::INCORRECT_INPUT_DATA;
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
    msgError(e.what());
    return ImgProcessing::Status::PROCESS_ERROR;
  }
  return ImgProcessing::Status::OK;
}

void Canny::setParameters(double threshold1, double threshold2)
{
  mThreshold1 = threshold1;
  mThreshold2 = threshold2;
}

/* ---------------------------------------------------------------------------------- */

}

#endif // HAVE_OPENCV
