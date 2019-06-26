#include "tidop/img_process/img_processing.h"

#include "tidop/core/messages.h"

#ifdef HAVE_OPENCV
#include <opencv2/highgui.hpp>
#include <opencv2/xphoto/white_balance.hpp>

#include <cstdarg>
#include <cstdio>


namespace tl
{


/* ---------------------------------------------------------------------------------- */

void ImgProcessingList::add(const std::shared_ptr<ImgProcessing> &ip)
{
  mProcessList.push_back(ip);
}

ProcessExit ImgProcessingList::execute(const cv::Mat &matIn, cv::Mat *matOut) const
{
  if (matIn.empty()) return ProcessExit::failure;
  matIn.copyTo(*matOut);
  for (const auto &process : mProcessList) {
    if (process->execute(*matOut, matOut) != ImgProcessing::Status::ok)
      return ProcessExit::failure;
  }
  return ProcessExit::success;
}

/* ---------------------------------------------------------------------------------- */

ImgProcessing::Status morphologicalOperation::execute(const cv::Mat &matIn, cv::Mat *matOut) const
{
  if (matIn.empty()) return ImgProcessing::Status::incorrect_input_data;
  try {
    cv::Mat element = getStructuringElement(mShapes, cv::Size(2 * mSize + 1, 2 * mSize + 1), cv::Point(mSize, mSize));
    switch (type)
    {
    case tl::process_type::morph_dilation:
      cv::dilate(matIn, *matOut, element, mAnchor, mIterations, mBorderType);
      break;
    case tl::process_type::morph_erotion:
      cv::erode(matIn, *matOut, element, mAnchor, mIterations, mBorderType);
      break;
    case tl::process_type::morph_opening:
      morphologyEx(matIn, *matOut, cv::MORPH_OPEN, element);
      break;
    case tl::process_type::morph_closing:
      morphologyEx(matIn, *matOut, cv::MORPH_CLOSE, element);
      break;
    case tl::process_type::morph_gradient:
      morphologyEx(matIn, *matOut, cv::MORPH_GRADIENT, element);
      break;
    case tl::process_type::morph_tophat:
      morphologyEx(matIn, *matOut, cv::MORPH_TOPHAT, element);
      break;
    case tl::process_type::morph_blackhat:
      morphologyEx(matIn, *matOut, cv::MORPH_BLACKHAT, element);
      break;
    default:
      return ImgProcessing::Status::incorrect_input_data;
    }
  } catch (cv::Exception &e) {
    msgError(e.what());
    return ImgProcessing::Status::process_error;
  }
  return ImgProcessing::Status::ok;
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


ImgProcessing::Status Normalize::execute(const cv::Mat &matIn, cv::Mat *matOut) const
{
  if (matIn.empty()) return ImgProcessing::Status::incorrect_input_data;
  try {
    cv::normalize(matIn, *matOut, mLowRange, mUpRange, cv::NORM_MINMAX);
  } catch (cv::Exception &e){
    msgError(e.what());
    return ImgProcessing::Status::process_error;
  }
  return ImgProcessing::Status::ok;
}

void Normalize::setParameters(double _lowRange, double _upRange)
{
  mLowRange = _lowRange;
  mUpRange = _upRange;
}

/* ---------------------------------------------------------------------------------- */

Binarize::Binarize(double thresh, double maxVal, bool bInverse)
  : ImgProcessing(process_type::binarize),
    mThresh(thresh),
    mMaxVal(maxVal),
    bInverse(bInverse) 
{
}

ImgProcessing::Status Binarize::execute(const cv::Mat &matIn, cv::Mat *matOut) const
{
  if (matIn.empty()) return ImgProcessing::Status::incorrect_input_data;
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
    msgError(e.what());
    return ImgProcessing::Status::process_error;
  }
  return ImgProcessing::Status::ok;
}

void Binarize::setParameters(double thresh, double maxval, bool inverse)
{
  mThresh = thresh;
  mMaxVal = maxval;
  bInverse = inverse;
}

void Binarize::setInverse(bool inverse) 
{ 
  bInverse = inverse; 
}

bool Binarize::getInverse() const 
{ 
  return bInverse; 
}

/* ---------------------------------------------------------------------------------- */

EqualizeHistogram::EqualizeHistogram()
  : ImgProcessing(process_type::equalize_hist) 
{
}

ImgProcessing::Status EqualizeHistogram::execute(const cv::Mat &matIn, cv::Mat *matOut) const
{
  if (matIn.empty()) return ImgProcessing::Status::incorrect_input_data;
  try {
    cv::equalizeHist(matIn, *matOut);
  } catch (cv::Exception &e) {
    msgError(e.what());
    return ImgProcessing::Status::process_error;
  }
  return ImgProcessing::Status::ok;
}

/* ---------------------------------------------------------------------------------- */


ImgProcessing::Status FunctionProcess::execute(const cv::Mat &matIn, cv::Mat *matOut) const
{
  if (matIn.empty()) return ImgProcessing::Status::incorrect_input_data;
  try {
    f(matIn, matOut);
  } catch (cv::Exception &e){
    msgError(e.what());
    return ImgProcessing::Status::process_error;
  }
  return ImgProcessing::Status::ok;
}


/* ---------------------------------------------------------------------------------- */

} // End namespace TL

#endif // HAVE_OPENCV
