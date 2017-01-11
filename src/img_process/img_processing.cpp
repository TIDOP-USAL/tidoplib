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
