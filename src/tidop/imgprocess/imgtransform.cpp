#include "imgtransform.h"

#ifdef HAVE_OPENCV


namespace tl
{

Resize::Resize(const Size<int> &size)
  : ImageProcess(ProcessType::resize),
    mWidth(size.width), 
    mHeight(size.height),
    mScaleX(0.), 
    mScaleY(0.)
{
}

Resize::Resize(int width, int height)
  : ImageProcess(ProcessType::resize),
    mWidth(width), 
    mHeight(height),
    mScaleX(0.), 
    mScaleY(0.)
{
}

Resize::Resize(double scaleX, double scaleY)
  : ImageProcess(ProcessType::resize), 
    mWidth(0), 
    mHeight(0),
    mScaleX(scaleX), 
    mScaleY(scaleY ? scaleY : scaleX)
{
}

void Resize::run(const cv::Mat &matIn, cv::Mat &matOut) const
{
  TL_ASSERT(!matIn.empty(), "Incorrect input data. Empty image")
  TL_ASSERT(mWidth > 0 || mScaleX != 0, "Invalid parameter value")

  if (mScaleX) {
    cv::resize(matIn, matOut, cv::Size(), mScaleX, mScaleY);
  } else {
    if (mHeight == 0) {
      double scale = static_cast<double>(mWidth / matIn.cols);
      cv::resize(matIn, matOut, cv::Size(), scale, scale);
    } else {
      cv::resize(matIn, matOut, cv::Size(mWidth, mHeight));
    }
  }
}

void Resize::setSize(const Size<int> &size)
{
  mWidth = size.width;
  mHeight = size.height;
  mScaleX = mScaleY = 0.;
}

void Resize::setSize(int width, int height = 0)
{
  mWidth = width;
  mHeight = height;
  mScaleX = mScaleY = 0.;
}

void Resize::setScale(double scaleX, double scaleY)
{
  mWidth = mHeight = 0;
  mScaleX = scaleX;
  mScaleY = scaleY ? scaleY : scaleX;
}

/* ---------------------------------------------------------------------------------- */


ResizeCanvas::ResizeCanvas(int width, 
                           int height, 
                           const graph::Color &color,
                           const Position &position)
  : ImageProcess(ProcessType::resize_canvas), 
    mWidth(width), 
    mHeight(height), 
    mColor(color),
    mPosition(position)
{
}



ResizeCanvas::ResizeCanvas(int width, 
                           int height,
                           const cv::Point &point, 
                           const graph::Color &color)
  : ImageProcess(ProcessType::resize_canvas), 
    mWidth(width), 
    mHeight(height), 
    mTopLeft(point),
    mColor(color)
{
}

void ResizeCanvas::run(const cv::Mat &matIn, cv::Mat &matOut) const
{
  TL_ASSERT(matIn.empty(), "Incorrect input data")

  TL_TODO("No esta terminada")
  cv::Mat aux = cv::Mat::zeros(cv::Size(mWidth, mHeight), matIn.type());
  matIn.copyTo(aux.colRange(0, matIn.cols).rowRange(0, matIn.rows));
  matOut = aux;
}

void ResizeCanvas::setParameters(int width, int height, const graph::Color &color, const Position &position)
{
  mWidth = width;
  mHeight = height;
  mPosition = position;
  mColor = color;
}

void ResizeCanvas::update()
{
  switch (mPosition) {
    case ResizeCanvas::Position::bottom_center:
      
      break;
    case ResizeCanvas::Position::bottom_left:
      break;
    case ResizeCanvas::Position::bottom_right:
      break;
    case ResizeCanvas::Position::center:
      break;
    case ResizeCanvas::Position::center_left:
      break;
    case ResizeCanvas::Position::center_right:
      break;
    case ResizeCanvas::Position::top_center:
      break;
    case ResizeCanvas::Position::top_left:
      mTopLeft = cv::Point(0, 0);
      break;
    case ResizeCanvas::Position::top_right:
      mTopLeft = cv::Point(0, 0);
      break;
    default:
      break;
  }
}

/* ---------------------------------------------------------------------------------- */

} // End namespace tl

#endif // HAVE_OPENCV
