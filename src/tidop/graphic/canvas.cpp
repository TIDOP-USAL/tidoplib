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

#include "tidop/graphic/canvas.h"
#include "tidop/graphic/painter.h"
#include "tidop/geometry/entities/point.h"

#ifdef TL_HAVE_OPENCV
#include "opencv2/core/core.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"
#endif

namespace tl
{

namespace graph
{ 


/* ---------------------------------------------------------------------------------- */

Canvas::Canvas()
  : mPainter(nullptr)
{
}

void Canvas::setPainter(Painter *painter)
{
  mPainter = painter;
}


/* ---------------------------------------------------------------------------------- */

#ifdef TL_HAVE_OPENCV

CanvasCV::CanvasCV()
  : Canvas(),
    mSize(100, 100),
    mBgColor(Color::Name::white)
{
  update();
}

CanvasCV::CanvasCV(const CanvasCV &canvas)
  : Canvas(),
    mSize(canvas.mSize),
    mBgColor(canvas.mBgColor)
{
  update();
}

CanvasCV::~CanvasCV()
{
}

//void CanvasCV::drawPoint(const GPoint &point)
//{
//  Symbol *style_symbol = point.symbol();
//  Pen *style_pen = point.pen();
//  Color c = style_pen->color();
//  cv::Scalar color = colorToCvScalar(c);
//  PointD pt_offset(style_symbol->offsetX(), style_symbol->offsetY());
//  PointI pt = point + pt_offset;
//
//  switch (style_symbol->name()) {
//  case Symbol::Name::cross:
//    cv::drawMarker(mCanvas, cv::Point(pt.x, pt.y), color, cv::MARKER_CROSS, 10, 1);
//    break;
//  case Symbol::Name::diagonal_cross:
//    cv::drawMarker(mCanvas, cv::Point(pt.x, pt.y), color, cv::MARKER_TILTED_CROSS, 10, 1);
//    break;
//  case Symbol::Name::circle:
//    cv::circle(mCanvas, cv::Point(pt.x, pt.y), 10, color, 1);
//    break;
//  case Symbol::Name::circle_filled:
//    cv::circle(mCanvas, cv::Point(pt.x, pt.y), 10, color, -1);
//    break;
//  case Symbol::Name::square:
//    cv::drawMarker(mCanvas, cv::Point(pt.x, pt.y), color, cv::MARKER_SQUARE, 10, 1);
//    break;
//  case Symbol::Name::square_filled:
//    cv::drawMarker(mCanvas, cv::Point(pt.x, pt.y), color, cv::MARKER_SQUARE, 10, -1);
//    break;
//  case Symbol::Name::triangle:
//    cv::drawMarker(mCanvas, cv::Point(pt.x, pt.y), color, cv::MARKER_TRIANGLE_UP, 10, 1);
//    break;
//  case Symbol::Name::triangle_filled:
//    cv::drawMarker(mCanvas, cv::Point(pt.x, pt.y), color, cv::MARKER_TRIANGLE_UP, 10, -1);
//    break;
//  case Symbol::Name::star:
//    cv::drawMarker(mCanvas, cv::Point(pt.x, pt.y), color, cv::MARKER_STAR, 10, 1);
//    break;
//  case Symbol::Name::star_filled:
//    cv::drawMarker(mCanvas, cv::Point(pt.x, pt.y), color, cv::MARKER_STAR, 10, -1);
//    break;
//  case Symbol::Name::vertical_bar:
//    break;
//  default:
//    cv::line(mCanvas, cv::Point(pt.x, pt.y), cv::Point(pt.x, pt.y), color, style_pen->width());
//    break;
//  }
//
//  Label *style_label = point.label();
//  Font font = style_label->font();
//  Color foregroundColor = style_label->foregroundColor();
//  cv::QtFont qt_font = cv::fontQt(font.name(),font.size(), colorToCvScalar(foregroundColor),
//                                  font.isBold() ? cv::QtFontWeights::QT_FONT_BOLD : cv::QtFontWeights::QT_FONT_NORMAL,
//                                  font.isItalic() ? cv::QtFontStyles::QT_STYLE_ITALIC : cv::QtFontStyles::QT_STYLE_NORMAL);
//  cv::addText(mCanvas, style_label->text(), cv::Point(pt.x, pt.y), qt_font);
//}

void CanvasCV::drawPoint(const PointD &point, const GraphicStyle &style)
{
  Symbol *style_symbol = style.symbol();
  Pen *style_pen = style.pen();
  Color c = style_pen->color();
  cv::Scalar color = colorToCvScalar(c);
  PointD pt_offset(style_symbol->offsetX(), style_symbol->offsetY());
  PointI pt = point + pt_offset;

  switch (style_symbol->name()) {
  case Symbol::Name::cross:
    cv::drawMarker(mCanvas, cv::Point(pt.x, pt.y), color, cv::MARKER_CROSS, 10, 1);
    break;
  case Symbol::Name::diagonal_cross:
    cv::drawMarker(mCanvas, cv::Point(pt.x, pt.y), color, cv::MARKER_TILTED_CROSS, 10, 1);
    break;
  case Symbol::Name::circle:
    cv::circle(mCanvas, cv::Point(pt.x, pt.y), 10, color, 1);
    break;
  case Symbol::Name::circle_filled:
    cv::circle(mCanvas, cv::Point(pt.x, pt.y), 10, color, -1);
    break;
  case Symbol::Name::square:
    cv::drawMarker(mCanvas, cv::Point(pt.x, pt.y), color, cv::MARKER_SQUARE, 10, 1);
    break;
  case Symbol::Name::square_filled:
    cv::drawMarker(mCanvas, cv::Point(pt.x, pt.y), color, cv::MARKER_SQUARE, 10, -1);
    break;
  case Symbol::Name::triangle:
    cv::drawMarker(mCanvas, cv::Point(pt.x, pt.y), color, cv::MARKER_TRIANGLE_UP, 10, 1);
    break;
  case Symbol::Name::triangle_filled:
    cv::drawMarker(mCanvas, cv::Point(pt.x, pt.y), color, cv::MARKER_TRIANGLE_UP, 10, -1);
    break;
  case Symbol::Name::star:
    cv::drawMarker(mCanvas, cv::Point(pt.x, pt.y), color, cv::MARKER_STAR, 10, 1);
    break;
  case Symbol::Name::star_filled:
    cv::drawMarker(mCanvas, cv::Point(pt.x, pt.y), color, cv::MARKER_STAR, 10, -1);
    break;
  case Symbol::Name::vertical_bar:
    break;
  default:
    cv::line(mCanvas, cv::Point(pt.x, pt.y), cv::Point(pt.x, pt.y), color, style_pen->width());
    break;
  }

  Label *style_label = style.label();
  if(!style_label->text().empty()) {
    //Font font = style_label->font();
    Color foregroundColor = style_label->foregroundColor();
    //cv::QtFont qt_font = cv::fontQt(font.name(),font.size(),
    //                                colorToCvScalar(foregroundColor),
    //                                font.isBold() ? cv::QtFontWeights::QT_FONT_BOLD : cv::QtFontWeights::QT_FONT_NORMAL,
    //                                font.isItalic() ? cv::QtFontStyles::QT_STYLE_ITALIC : cv::QtFontStyles::QT_STYLE_NORMAL);
    //cv::addText(mCanvas, style_label->text(), cv::Point(pt.x, pt.y), qt_font);

    int fontFace = cv::FONT_HERSHEY_SIMPLEX;
    double fontScale = 2;
    int thickness = 3;
    int baseline = 0;
    cv::Size textSize = cv::getTextSize(style_label->text(), fontFace,
                                        fontScale, thickness, &baseline);
    baseline += thickness;

    cv::putText(mCanvas, style_label->text(), cv::Point(roundToInteger(point.x), roundToInteger(point.y)), fontFace, fontScale,
                colorToCvScalar(foregroundColor), thickness, 8);

  }
}

//void CanvasCV::drawLineString(const GLineString &lineString)
//{
//  size_t n = lineString.size();
//  std::vector<std::vector<cv::Point>> pts(1,std::vector<cv::Point>(n));
//  for (size_t i = 0; i < n; i++){
//    pts[0][i].x = static_cast<int>(lineString[i].x);
//    pts[0][i].y = static_cast<int>(lineString[i].y);
//  }
//
//  const cv::Point *cpts = (const cv::Point*) cv::Mat(pts).data;
//  int npts = cv::Mat(pts).rows;
//
//  Pen *style_pen = lineString.pen();
//  if (!style_pen->pattern().empty()){
//    TODO: drawPolyLine(grd, cpts, npts, GVE_ReadyStyle::PenColor, GVE_ReadyStyle::PenWidth, GVE_ReadyStyle::PenPattern);
//  } else {
//    Color color = style_pen->color();
//    cv::polylines(mCanvas, pts, false, colorToCvScalar(color), style_pen->width());
//  }
//}

void CanvasCV::drawLineString(const LineStringD &lineString, const GraphicStyle &style)
{
  size_t n = lineString.size();
  std::vector<std::vector<cv::Point>> pts(1, std::vector<cv::Point>(n));
  for(size_t i = 0; i < n; i++) {
    pts[0][i].x = static_cast<int>(lineString[i].x);
    pts[0][i].y = static_cast<int>(lineString[i].y);
  }

  //  const cv::Point *cpts = (const cv::Point*) cv::Mat(pts).data;
  //  int npts = cv::Mat(pts).rows;

  Pen *style_pen = style.pen();
  if(!style_pen->pattern().empty()) {
    ///TODO: drawPolyLine(grd, cpts, npts, GVE_ReadyStyle::PenColor, GVE_ReadyStyle::PenWidth, GVE_ReadyStyle::PenPattern);
  } else {
    Color color = style_pen->color();
    cv::polylines(mCanvas, pts, false, colorToCvScalar(color), style_pen->width());
  }
}

//void CanvasCV::drawPolygon(const GPolygon &polygon)
//{
//  drawPolygon(polygon, polygon);
//}

void CanvasCV::drawPolygon(const PolygonD &polygon, const GraphicStyle &style)
{
  size_t n = polygon.size();
  std::vector<std::vector<cv::Point>> pts(1, std::vector<cv::Point>(n));
  for (size_t i = 0; i < n; i++) {
    pts[0][i].x = static_cast<int>(polygon[i].x);
    pts[0][i].y = static_cast<int>(polygon[i].y);
  }

  if (Brush *style_brush = style.brush()) {
    Color fore_color = style_brush->foregroundColor();
    cv::fillPoly(mCanvas, pts, colorToCvScalar(fore_color));
  }

  if (Pen *style_pen = style.pen()) {
    Color color = style_pen->color();
    uint8_t width = style_pen->width();
    if (!style_pen->pattern().empty()) {
      ///TODO: drawPolyLine(grd, cpts, npts, GVE_ReadyStyle::PenColor, GVE_ReadyStyle::PenWidth, GVE_ReadyStyle::PenPattern);
    } else {
      cv::polylines(mCanvas, pts, true, colorToCvScalar(color), width);
    }
  }

  if (Label *style_label = style.label()) {
    /// TODO: completar
  }

  if (Symbol *style_symbol = style.symbol()) {
    /// TODO: completar
  }
}

//void CanvasCV::drawText(const PointD &point, const std::string &text)
//{
//
//}

void CanvasCV::drawText(const PointD &point, const std::string &text, const GraphicStyle &style)
{
  Label *style_label = style.label();
  Color foregroundColor = style_label->foregroundColor();
  int fontFace = cv::FONT_HERSHEY_SIMPLEX;
  double fontScale = 2;
  int thickness = 3;
  int baseline = 0;
  cv::Size textSize = cv::getTextSize(text, fontFace,
                                      fontScale, thickness, &baseline);
  baseline += thickness;


  cv::putText(mCanvas, text, cv::Point(roundToInteger(point.x), roundToInteger(point.y)), fontFace, fontScale,
              colorToCvScalar(foregroundColor), thickness, 8);
}

void CanvasCV::setPicture(const cv::Mat &bmp)
{
  /// insertar imagen. 
  bmp.copyTo(mCanvas);
}

CanvasCV &CanvasCV::operator =(const CanvasCV &canvas)
{
  if (this != &canvas){
    mSize = canvas.mSize;
    mBgColor = canvas.mBgColor;
  }
  return *this;
}

void CanvasCV::update()
{
  mCanvas = cv::Mat(mSize.height, mSize.width, CV_MAKETYPE(CV_8U, 3), colorToCvScalar(mBgColor));
}

cv::Scalar CanvasCV::colorToCvScalar(const Color &color)
{
  return cv::Scalar(static_cast<double>(color.blue()),
                    static_cast<double>(color.green()),
                    static_cast<double>(color.red()));
}

int CanvasCV::width() const
{
  return mSize.width;
}

int CanvasCV::height() const
{
  return mSize.height;
}

Color CanvasCV::backgroundColor() const
{
  return mBgColor;
}

void CanvasCV::setWidth(int width)
{
  mSize.width = width;
  update();
}

void CanvasCV::setHeight(int height)
{
  mSize.height = height;
  update();
}

void CanvasCV::setSize(int width, int height)
{
  mSize.width = width;
  mSize.height = height;
  update();
}

inline void CanvasCV::setSize(const Size<int> &size)
{
  mSize = size;
  update();
}

Size<int> CanvasCV::size() const
{
  return mSize;
}

void CanvasCV::setBackgroundColor(const Color &color)
{
  mBgColor = color;
  update();
}

cv::Mat CanvasCV::bmp()
{
  return mCanvas;
}


#endif // TL_HAVE_OPENCV


/* ---------------------------------------------------------------------------------- */


} // Fin namespace graph

} // End namespace tl
