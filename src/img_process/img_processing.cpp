#include "img_process/img_processing.h"

#include "core/messages.h"
#include "graphic_entities/color.h"

#ifdef HAVE_OPENCV
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
  for (const auto &process : mProcessList) {
    if (process->execute(*matOut, matOut) != ImgProcessing::Status::OK)
      return ProcessExit::FAILURE;
  }
  return ProcessExit::SUCCESS;
}

/* ---------------------------------------------------------------------------------- */

ImgProcessing::Status morphologicalOperation::execute(const cv::Mat &matIn, cv::Mat *matOut) const
{
  if (matIn.empty()) return ImgProcessing::Status::INCORRECT_INPUT_DATA;
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
      return ImgProcessing::Status::INCORRECT_INPUT_DATA;
    }
  } catch (cv::Exception &e) {
    msgError(e.what());
    return ImgProcessing::Status::PROCESS_ERROR;
  }
  return ImgProcessing::Status::OK;
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


ImgProcessing::Status Resize::execute(const cv::Mat &matIn, cv::Mat *matOut) const
{
  if (matIn.empty()) return ImgProcessing::Status::INCORRECT_INPUT_DATA;

  if (mWidth == 0 && mScaleX == 0) {
    msgError("Invalid parameter values");
    return ImgProcessing::Status::INCORRECT_INPUT_DATA;
  }
  try {    
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
    msgError(e.what());
    return ImgProcessing::Status::PROCESS_ERROR;
  }
  return ImgProcessing::Status::OK;
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


ImgProcessing::Status ResizeCanvas::execute(const cv::Mat &matIn, cv::Mat *matOut) const
{
  if (matIn.empty()) return ImgProcessing::Status::INCORRECT_INPUT_DATA;
  try {
    cv::Mat aux = cv::Mat::zeros(cv::Size(mWidth,mHeight),matIn.type());
    matIn.copyTo(aux.colRange(0, matIn.cols).rowRange(0, matIn.rows));
    *matOut = aux;
  } catch (cv::Exception &e){
    msgError(e.what());
    return ImgProcessing::Status::PROCESS_ERROR;
  }
  return ImgProcessing::Status::OK;
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


ImgProcessing::Status Normalize::execute(const cv::Mat &matIn, cv::Mat *matOut) const
{
  if (matIn.empty()) return ImgProcessing::Status::INCORRECT_INPUT_DATA;
  try {
    cv::normalize(matIn, *matOut, mLowRange, mUpRange, cv::NORM_MINMAX);
  } catch (cv::Exception &e){
    msgError(e.what());
    return ImgProcessing::Status::PROCESS_ERROR;
  }
  return ImgProcessing::Status::OK;
}

void Normalize::setParameters(double _lowRange, double _upRange)
{
  mLowRange = _lowRange;
  mUpRange = _upRange;
}

/* ---------------------------------------------------------------------------------- */


ImgProcessing::Status Binarize::execute(const cv::Mat &matIn, cv::Mat *matOut) const
{
  if (matIn.empty()) return ImgProcessing::Status::INCORRECT_INPUT_DATA;
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
    return ImgProcessing::Status::PROCESS_ERROR;
  }
  return ImgProcessing::Status::OK;
}

void Binarize::setParameters(double thresh, double maxval, bool inverse)
{
  mThresh = thresh;
  mMaxVal = maxval;
  bInverse = inverse;
}

/* ---------------------------------------------------------------------------------- */


ImgProcessing::Status EqualizeHistogram::execute(const cv::Mat &matIn, cv::Mat *matOut) const
{
  if (matIn.empty()) return ImgProcessing::Status::INCORRECT_INPUT_DATA;
  try {
    cv::equalizeHist(matIn, *matOut);
  } catch (cv::Exception &e) {
    msgError(e.what());
    return ImgProcessing::Status::PROCESS_ERROR;
  }
  return ImgProcessing::Status::OK;
}

/* ---------------------------------------------------------------------------------- */


ImgProcessing::Status FunctionProcess::execute(const cv::Mat &matIn, cv::Mat *matOut) const
{
  if (matIn.empty()) return ImgProcessing::Status::INCORRECT_INPUT_DATA;
  try {
    f(matIn, matOut);
  } catch (cv::Exception &e){
    msgError(e.what());
    return ImgProcessing::Status::PROCESS_ERROR;
  }
  return ImgProcessing::Status::OK;
}


/* ---------------------------------------------------------------------------------- */


ImgProcessing::Status ColorConversion::execute(const cv::Mat &matIn, cv::Mat *matOut) const
{
  if (matIn.empty()) return ImgProcessing::Status::INCORRECT_INPUT_DATA;
  try {
    if (mModelIn == ColorConversion::Model::RGB && mModelOut == ColorConversion::Model::HSL) {
      rgbToHSL(matIn, matOut);
    } else if (mModelIn == ColorConversion::Model::RGB && mModelOut == ColorConversion::Model::HSV) {
      rgbToHSV(matIn, matOut);
    } else if (mModelIn == ColorConversion::Model::RGB && mModelOut == ColorConversion::Model::CMYK) {
      rgbToCmyk(matIn, matOut);
    } else if (mModelIn == ColorConversion::Model::RGB && mModelOut == ColorConversion::Model::LUMINANCE) {
      rgbToLuminance(matIn, matOut);
    } else {
      msgWarning("Conversión no disponible")
    }
  } catch (cv::Exception &e){
    msgError(e.what());
    return ImgProcessing::Status::PROCESS_ERROR;
  }
  return ImgProcessing::Status::OK;
}

//void ColorConversion::setParameters(Model modelIn, Model modelOut)
//{
//  mThresh = thresh;
//  mMaxVal = maxval;
//  bInverse = inverse;
//}

/* ---------------------------------------------------------------------------------- */


} // End namespace I3D

#endif // HAVE_OPENCV