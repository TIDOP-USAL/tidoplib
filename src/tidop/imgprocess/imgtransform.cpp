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
 * License along with Foobar. If not, see <http://www.gnu.org/licenses/>. *
 *                                                                        *
 * @license LGPL-3.0 <https://www.gnu.org/licenses/lgpl-3.0.html>         *
 *                                                                        *
 **************************************************************************/

#include "imgtransform.h"

#ifdef TL_HAVE_OPENCV


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
  try {
  
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

  } catch (...) {
    std::throw_with_nested(std::runtime_error("Resize::run() failed"));
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
  try {

    TL_ASSERT(matIn.empty(), "Incorrect input data")

    TL_TODO("No esta terminada")
    cv::Mat aux = cv::Mat::zeros(cv::Size(mWidth, mHeight), matIn.type());
    matIn.copyTo(aux.colRange(0, matIn.cols).rowRange(0, matIn.rows));
    matOut = aux;
  
  } catch (...) {
    std::throw_with_nested(std::runtime_error("ResizeCanvas::run() failed"));
  }
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

#endif // TL_HAVE_OPENCV
