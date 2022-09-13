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

namespace graph
{ 


/* ---------------------------------------------------------------------------------- */

Painter::Painter()
  : mTrf(nullptr),
    mCanvas(nullptr)
{
}

Painter::Painter(Canvas *canvas)
  : mTrf(nullptr),
    mCanvas(canvas)
{
}


Painter::~Painter()
{
}

void Painter::drawPoint(const GPoint &point) 
{
  if (mCanvas){
    mCanvas->drawPoint(point);
  } else {
     msgError("Canvas not defined");
  }
}

void Painter::drawPoint(const PointD &point)
{
  if (mCanvas){
    mCanvas->drawPoint(point, *this);
  } else {
     msgError("Canvas not defined");
  }
}

void Painter::drawLineString(const GLineString &lineString)
{
  if (mCanvas){
    mCanvas->drawLineString(lineString);
  } else {
    msgError("Canvas not defined");
  }
}

void Painter::drawPolygon(const GPolygon &polygon)
{
  if (mCanvas){
    Polygon<Point<double>> polygon_transform(polygon.size());
    for (size_t i = 0; i < polygon.size(); i++) {
      polygon_transform[i] = dynamic_cast<TransformBase<PointD> *>(mTrf)->transform(polygon[i], Transform::Order::inverse);
    }
    
    mCanvas->drawPolygon(polygon_transform, *this);
  } else {
    msgError("Canvas not defined");
  }
}
  
void Painter::drawMultiPoint(const GMultiPoint &multipoint)
{

}

void Painter::drawMultiLineString(const GMultiLineString &multiLineString)
{

}

void Painter::drawMultiPolygon(const GMultiPolygon &multiPolygon)
{

}

#ifdef TL_HAVE_OPENCV
void Painter::drawPicture(const cv::Mat &bmp)
{
  ///mCanvas->
}
#endif // TL_HAVE_OPENCV

void Painter::drawText(const PointD &point, const std::string &text)
{
  mCanvas->drawText(point, text, *this);
}

void Painter::setCanvas(Canvas *canvas)
{
  mCanvas = canvas;
}

//void Painter::setPen(const std::shared_ptr<Pen> &pen)
//{
//  mGraphicStyle->setPen(pen);
//}
//
//void Painter::setBrush(const std::shared_ptr<Brush> &brush)
//{
//  mGraphicStyle->setBrush(brush);
//}
//
//void Painter::setSymbol(const std::shared_ptr<Symbol> &symbol)
//{
//  mGraphicStyle->setSymbol(symbol);
//}
//
//void Painter::setLabel(const std::shared_ptr<Label> &label)
//{
//  mGraphicStyle->setLabel(label);
//}

void Painter::setTransform(Transform/*<PointF>*/ *trf)
{
  mTrf = trf;
}



} // Fin namespace graph

} // End namespace tl
