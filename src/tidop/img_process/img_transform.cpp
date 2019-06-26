#include "img_transform.h"

#ifdef HAVE_OPENCV


namespace tl
{

/* ---------------------------------------------------------------------------------- */


ImgProcessing::Status Resize::execute(const cv::Mat &matIn, cv::Mat *matOut) const
{
  if (matIn.empty()) return ImgProcessing::Status::incorrect_input_data;

  if (mWidth == 0 && mScaleX == 0) {
    msgError("Invalid parameter values");
    return ImgProcessing::Status::incorrect_input_data;
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
    return ImgProcessing::Status::process_error;
  }
  return ImgProcessing::Status::ok;
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
  if (matIn.empty()) return ImgProcessing::Status::incorrect_input_data;
  try {
    cv::Mat aux = cv::Mat::zeros(cv::Size(mWidth,mHeight),matIn.type());
    matIn.copyTo(aux.colRange(0, matIn.cols).rowRange(0, matIn.rows));
    *matOut = aux;
  } catch (cv::Exception &e){
    msgError(e.what());
    return ImgProcessing::Status::process_error;
  }
  return ImgProcessing::Status::ok;
}

void ResizeCanvas::setParameters(int width, int height, const graph::Color &color, const Position &position)
{
  mWidth = width;
  mHeight = height;
  mPosition = position;
  mColor = color;
}

//void ResizeCanvas::update()
//{
//  switch (mPosition) {
//    case TL::ResizeCanvas::Position::BOTTOM_CENTER:
//      
//      break;
//    case TL::ResizeCanvas::Position::BOTTOM_LEFT:
//      break;
//    case TL::ResizeCanvas::Position::BOTTOM_RIGHT:
//      break;
//    case TL::ResizeCanvas::Position::CENTER:
//      break;
//    case TL::ResizeCanvas::Position::CENTER_LEFT:
//      break;
//    case TL::ResizeCanvas::Position::CENTER_RIGHT:
//      break;
//    case TL::ResizeCanvas::Position::TOP_CENTER:
//      break;
//    case TL::ResizeCanvas::Position::TOP_LEFT:
//      mTopLeft = cv::Point(0, 0);
//      break;
//    case TL::ResizeCanvas::Position::TOP_RIGHT:
//      mTopLeft = cv::Point(0, 0);
//      break;
//    default:
//      break;
//  }
//}

/* ---------------------------------------------------------------------------------- */

} // End namespace TL

#endif // HAVE_OPENCV
