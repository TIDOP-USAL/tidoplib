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

#include "tidop/graphic/painter.h"

#ifdef TL_HAVE_OPENCV
#include "opencv2/core/core.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"
#endif

#include "tidop/graphic/canvas.h"
#include "tidop/geometry/transform/transform.h"

namespace tl
{

Painter::Painter()
  : mCanvas(nullptr)
{
}

Painter::Painter(Canvas *canvas)
  : mCanvas(canvas)
{
}

Painter::~Painter()
{
}

void Painter::drawPoint(const GPoint &point)
{
    if (mCanvas) {

        if (!mTransform.isEmpty()) {

            Point<double> point_transform = mTransform.transform(point);
            mCanvas->drawPoint(point_transform, point);

        } else {

            mCanvas->drawPoint(point, point);
        }

    } else {
        Message::error("Canvas not defined");
    }
}

void Painter::drawPoint(const Point<double> &point)
{
    if (mCanvas) {
        mCanvas->drawPoint(point, *this);
    } else {
        Message::error("Canvas not defined");
    }
}

void Painter::drawLineString(const GLineString &lineString)
{
    if (mCanvas) {

        if (!mTransform.isEmpty()) {

            LineString<Point<double>> linestring_transform(lineString.size());

            for (size_t i = 0; i < lineString.size(); i++) {
                linestring_transform[i] = mTransform.transform(lineString[i]);
            }

            mCanvas->drawLineString(linestring_transform, lineString);

        } else {

            mCanvas->drawLineString(lineString, lineString);

        }

    } else {
        Message::error("Canvas not defined");
    }
}

void Painter::drawLineString(const LineStringD &lineString)
{
    if (mCanvas) {
        mCanvas->drawLineString(lineString, *this);
    } else {
        Message::error("Canvas not defined");
    }
}

void Painter::drawPolygon(const GPolygon &polygon)
{
    if (mCanvas) {

        if (!mTransform.isEmpty()) {
            Polygon<Point<double>> polygon_transform(polygon.size());

            for (size_t i = 0; i < polygon.size(); i++) {
                polygon_transform[i] = mTransform.transform(polygon[i]);
            }

            mCanvas->drawPolygon(polygon_transform, polygon);
        } else {
            mCanvas->drawPolygon(polygon, polygon);
        }

    } else {
        Message::error("Canvas not defined");
    }
}

void Painter::drawPolygon(const PolygonD &polygon)
{
    if (mCanvas) {

        if (!mTransform.isEmpty()) {
            Polygon<Point<double>> polygon_transform(polygon.size());

            for (size_t i = 0; i < polygon.size(); i++) {
                polygon_transform[i] = mTransform.transform(polygon[i]);
            }

            mCanvas->drawPolygon(polygon_transform, *this);
        } else {
            mCanvas->drawPolygon(polygon, *this);
        }

    } else {
        Message::error("Canvas not defined");
    }
}

void Painter::drawMultiPoint(const GMultiPoint &multipoint)
{
    unusedParameter(multipoint);
}

void Painter::drawMultiLineString(const GMultiLineString &multiLineString)
{
    unusedParameter(multiLineString);
}

void Painter::drawMultiPolygon(const GMultiPolygon &multiPolygon)
{
    unusedParameter(multiPolygon);
}

#ifdef TL_HAVE_OPENCV
void Painter::drawPicture(const cv::Mat &bmp)
{
    ///mCanvas->
    unusedParameter(bmp);
}
#endif // TL_HAVE_OPENCV

void Painter::drawText(const Point<double> &point, const std::string &text)
{
    if (mCanvas) {

        if (!mTransform.isEmpty()) {
            Point<double> point_transform = mTransform.transform(point);
            mCanvas->drawText(point_transform, text, *this);
        } else {
            mCanvas->drawText(point, text, *this);
        }

    } else {
        Message::error("Canvas not defined");
    }
}

void Painter::setCanvas(Canvas *canvas)
{
    mCanvas = canvas;
}

void Painter::setTransform(const Affine<double, 2> &affine)
{
    mTransform = affine;
}



} // End namespace tl
