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
 * License along with TidopLib. If not, see <http://www.gnu.org/licenses>.*
 *                                                                        *
 * @license LGPL-3.0 <https://www.gnu.org/licenses/lgpl-3.0.html>         *
 *                                                                        *
 **************************************************************************/

#include "tidop/rastertools/process/transform/resize.h"

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

        TL_ASSERT(!matIn.empty(), "Incorrect input data. Empty image");
        TL_ASSERT(mWidth > 0 || mScaleX != 0, "Invalid parameter value");

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






ResizeCanvas::ResizeCanvas(int width,
                           int height,
                           const Color &color,
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
                           const Color &color)
  : ImageProcess(ProcessType::resize_canvas),
    mWidth(width),
    mHeight(height),
    mTopLeft(point),
    mColor(color),
    mPosition(Position::custom)
{
}

void ResizeCanvas::run(const cv::Mat &matIn, cv::Mat &matOut) const
{
    try {

        TL_ASSERT(matIn.empty(), "Incorrect input data");

        cv::Rect rect;
        if (mPosition != Position::custom) {
            rect.x = mTopLeft.x;
            rect.y = mTopLeft.y;
        } else {
            switch (mPosition) {
            case ResizeCanvas::Position::bottom_center:
                rect.x = (mWidth - matIn.cols) / 2;
                rect.y = mHeight - matIn.rows;
                break;
            case ResizeCanvas::Position::bottom_left:
                rect.x = 0;
                rect.y = mHeight - matIn.rows;
                break;
            case ResizeCanvas::Position::bottom_right:
                rect.x = mWidth - matIn.cols;
                rect.y = mHeight - matIn.rows;
                break;
            case ResizeCanvas::Position::center:
                rect.x = (mWidth - matIn.cols) / 2;
                rect.y = (mHeight - matIn.rows) / 2;
                break;
            case ResizeCanvas::Position::center_left:
                rect.x = 0;
                rect.y = (mHeight - matIn.rows) / 2;
                break;
            case ResizeCanvas::Position::center_right:
                rect.x = mWidth - matIn.cols;
                rect.y = (mHeight - matIn.rows) / 2;
                break;
            case ResizeCanvas::Position::top_center:
                rect.x = (mWidth - matIn.cols) / 2;
                rect.y = 0;
                break;
            case ResizeCanvas::Position::top_left:
                rect.x = 0;
                rect.y = 0;
                break;
            case ResizeCanvas::Position::top_right:
                rect.x = mWidth - matIn.cols;
                rect.y = 0;
                break;
            default:
                throw std::invalid_argument("Invalid position");
            }
        }

        rect.width = matIn.cols;
        rect.height = matIn.rows;

        cv::Mat aux(mHeight, mWidth, matIn.type(), cv::Scalar(mColor.red(), mColor.green(), mColor.blue()));
        matIn.copyTo(aux(rect));
        matOut = aux;

    } catch (...) {
        std::throw_with_nested(std::runtime_error("ResizeCanvas::run() failed"));
    }
}



} // End namespace tl
