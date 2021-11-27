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

#include "tidop/graphic/canvas.h"

#include "tidop/geometry/entities/point.h"

#ifdef HAVE_OPENCV
#include "opencv2/core/core.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"
#endif

#include "ogrsf_frmts.h"

namespace tl
{

namespace graph
{ 


/* ---------------------------------------------------------------------------------- */


/* ---------------------------------------------------------------------------------- */

#ifdef HAVE_OPENCV

CanvasCV::CanvasCV()
  : Canvas(),
    mWidth(100),
    mHeight(100),
    mBgColor(Color::Name::white)
{
  update();
}

CanvasCV::CanvasCV(const CanvasCV &canvas)
  : Canvas(),
    mWidth(canvas.mWidth),
    mHeight(canvas.mHeight),
    mBgColor(canvas.mBgColor)
{
  update();
}

CanvasCV::~CanvasCV()
{
}

void CanvasCV::drawPoint(const GPoint &point)
{
  StyleSymbol *style_symbol = point.styleSymbol();
  StylePen *style_pen = point.stylePen();
  Color c = style_pen->color();
  cv::Scalar color = colorToCvScalar(c);
  PointD pt_offset(style_symbol->offsetX(), style_symbol->offsetY());
  PointI pt = point + pt_offset;

  switch (style_symbol->name()) {
  case StyleSymbol::Name::cross:
    cv::drawMarker(mCanvas, cv::Point(pt.x, pt.y), color, cv::MARKER_CROSS, 10, 1);
    break;
  case StyleSymbol::Name::diagonal_cross:
    cv::drawMarker(mCanvas, cv::Point(pt.x, pt.y), color, cv::MARKER_TILTED_CROSS, 10, 1);
    break;
  case StyleSymbol::Name::circle:
    cv::circle(mCanvas, cv::Point(pt.x, pt.y), 10, color, 1);
    break;
  case StyleSymbol::Name::circle_filled:
    cv::circle(mCanvas, cv::Point(pt.x, pt.y), 10, color, -1);
    break;
  case StyleSymbol::Name::square:
    cv::drawMarker(mCanvas, cv::Point(pt.x, pt.y), color, cv::MARKER_SQUARE, 10, 1);
    break;
  case StyleSymbol::Name::square_filled:
    cv::drawMarker(mCanvas, cv::Point(pt.x, pt.y), color, cv::MARKER_SQUARE, 10, -1);
    break;
  case StyleSymbol::Name::triangle:
    cv::drawMarker(mCanvas, cv::Point(pt.x, pt.y), color, cv::MARKER_TRIANGLE_UP, 10, 1);
    break;
  case StyleSymbol::Name::triangle_filled:
    cv::drawMarker(mCanvas, cv::Point(pt.x, pt.y), color, cv::MARKER_TRIANGLE_UP, 10, -1);
    break;
  case StyleSymbol::Name::star:
    cv::drawMarker(mCanvas, cv::Point(pt.x, pt.y), color, cv::MARKER_STAR, 10, 1);
    break;
  case StyleSymbol::Name::star_filled:
    cv::drawMarker(mCanvas, cv::Point(pt.x, pt.y), color, cv::MARKER_STAR, 10, -1);
    break;
  case StyleSymbol::Name::vertical_bar:
    break;
  default:
    cv::line(mCanvas, cv::Point(pt.x, pt.y), cv::Point(pt.x, pt.y), color, style_pen->width());
    break;
  }

  StyleLabel *style_label = point.styleLabel();
  Font font = style_label->font();
  Color foreColor = style_label->foregroundColor();
  cv::QtFont qt_font = cv::fontQt(font.name(),font.size(), colorToCvScalar(foreColor),
                                  font.isBold() ? cv::QtFontWeights::QT_FONT_BOLD : cv::QtFontWeights::QT_FONT_NORMAL,
                                  font.isItalic() ? cv::QtFontStyles::QT_STYLE_ITALIC : cv::QtFontStyles::QT_STYLE_NORMAL);
  cv::addText(mCanvas, style_label->text(), cv::Point(pt.x, pt.y), qt_font);
}

void CanvasCV::drawPoint(const PointD &point, const GraphicStyle &style)
{
  StyleSymbol *style_symbol = style.styleSymbol();
  StylePen *style_pen = style.stylePen();
  Color c = style_pen->color();
  cv::Scalar color = colorToCvScalar(c);
  PointD pt_offset(style_symbol->offsetX(), style_symbol->offsetY());
  PointI pt = point + pt_offset;

  switch (style_symbol->name()) {
  case StyleSymbol::Name::cross:
    cv::drawMarker(mCanvas, cv::Point(pt.x, pt.y), color, cv::MARKER_CROSS, 10, 1);
    break;
  case StyleSymbol::Name::diagonal_cross:
    cv::drawMarker(mCanvas, cv::Point(pt.x, pt.y), color, cv::MARKER_TILTED_CROSS, 10, 1);
    break;
  case StyleSymbol::Name::circle:
    cv::circle(mCanvas, cv::Point(pt.x, pt.y), 10, color, 1);
    break;
  case StyleSymbol::Name::circle_filled:
    cv::circle(mCanvas, cv::Point(pt.x, pt.y), 10, color, -1);
    break;
  case StyleSymbol::Name::square:
    cv::drawMarker(mCanvas, cv::Point(pt.x, pt.y), color, cv::MARKER_SQUARE, 10, 1);
    break;
  case StyleSymbol::Name::square_filled:
    cv::drawMarker(mCanvas, cv::Point(pt.x, pt.y), color, cv::MARKER_SQUARE, 10, -1);
    break;
  case StyleSymbol::Name::triangle:
    cv::drawMarker(mCanvas, cv::Point(pt.x, pt.y), color, cv::MARKER_TRIANGLE_UP, 10, 1);
    break;
  case StyleSymbol::Name::triangle_filled:
    cv::drawMarker(mCanvas, cv::Point(pt.x, pt.y), color, cv::MARKER_TRIANGLE_UP, 10, -1);
    break;
  case StyleSymbol::Name::star:
    cv::drawMarker(mCanvas, cv::Point(pt.x, pt.y), color, cv::MARKER_STAR, 10, 1);
    break;
  case StyleSymbol::Name::star_filled:
    cv::drawMarker(mCanvas, cv::Point(pt.x, pt.y), color, cv::MARKER_STAR, 10, -1);
    break;
  case StyleSymbol::Name::vertical_bar:
    break;
  default:
    cv::line(mCanvas, cv::Point(pt.x, pt.y), cv::Point(pt.x, pt.y), color, style_pen->width());
    break;
  }

  StyleLabel *style_label = style.styleLabel();
  Font font = style_label->font();
  Color foreColor = style_label->foregroundColor();
  cv::QtFont qt_font = cv::fontQt(font.name(),font.size(),
                                  colorToCvScalar(foreColor),
                                  font.isBold() ? cv::QtFontWeights::QT_FONT_BOLD : cv::QtFontWeights::QT_FONT_NORMAL,
                                  font.isItalic() ? cv::QtFontStyles::QT_STYLE_ITALIC : cv::QtFontStyles::QT_STYLE_NORMAL);
  cv::addText(mCanvas, style_label->text(), cv::Point(pt.x, pt.y), qt_font);
}

void CanvasCV::drawLineString(const GLineString &lineString)
{
  size_t n = lineString.size();
  std::vector<std::vector<cv::Point>> pts(1,std::vector<cv::Point>(n));
  for (size_t i = 0; i < n; i++){
    pts[0][i].x = static_cast<int>(lineString[i].x);
    pts[0][i].y = static_cast<int>(lineString[i].y);
  }

//  const cv::Point *cpts = (const cv::Point*) cv::Mat(pts).data;
//  int npts = cv::Mat(pts).rows;

  StylePen *style_pen = lineString.stylePen();
  if (!style_pen->pattern().empty()){
    ///TODO: drawPolyLine(grd, cpts, npts, GVE_ReadyStyle::PenColor, GVE_ReadyStyle::PenWidth, GVE_ReadyStyle::PenPattern);
  } else {
    Color color = style_pen->color();
    cv::polylines(mCanvas, pts, false, colorToCvScalar(color), style_pen->width());
  }
}

void CanvasCV::drawPolygon(const GPolygon &polygon)
{
  drawPolygon(polygon, polygon);
}

void CanvasCV::drawPolygon(const PolygonD &polygon, const GraphicStyle &style)
{
  size_t n = polygon.size();
  std::vector<std::vector<cv::Point>> pts(1, std::vector<cv::Point>(n));
  for (size_t i = 0; i < n; i++) {
    pts[0][i].x = static_cast<int>(polygon[i].x);
    pts[0][i].y = static_cast<int>(polygon[i].y);
  }

  if (StyleBrush *style_brush = style.styleBrush()) {
    Color fore_color = style_brush->foreColor();
    cv::fillPoly(mCanvas, pts, colorToCvScalar(fore_color));
  }

  if (StylePen *style_pen = style.stylePen()) {
    Color color = style_pen->color();
    uint8_t width = style_pen->width();
    if (!style_pen->pattern().empty()) {
      ///TODO: drawPolyLine(grd, cpts, npts, GVE_ReadyStyle::PenColor, GVE_ReadyStyle::PenWidth, GVE_ReadyStyle::PenPattern);
    } else {
      cv::polylines(mCanvas, pts, true, colorToCvScalar(color), width);
    }
  }

  if (StyleLabel *style_label = style.styleLabel()) {
    /// TODO: completar
  }

  if (StyleSymbol *style_symbol = style.styleSymbol()) {
    /// TODO: completar
  }
}

void CanvasCV::drawText(const PointD &point, const std::string &text)
{

}

void CanvasCV::drawText(const PointD &point, const std::string &text, const GraphicStyle &style)
{
  int fontFace = cv::FONT_HERSHEY_SCRIPT_SIMPLEX;
  double fontScale = 2;
  int thickness = 3;
  int baseline = 0;
  cv::Size textSize = cv::getTextSize(text, fontFace,
                                      fontScale, thickness, &baseline);
  baseline += thickness;

  cv::putText(mCanvas, text, cv::Point(point.x, point.y), fontFace, fontScale,
              cv::Scalar::all(255), thickness, 8);
}

void CanvasCV::setPicture(const cv::Mat &bmp)
{
  /// insertar imagen. 
  bmp.copyTo(mCanvas);
}

CanvasCV &CanvasCV::operator =(const CanvasCV &canvas)
{
  if (this != &canvas){
    mWidth = canvas.mWidth;
    mHeight = canvas.mHeight;
    mBgColor = canvas.mBgColor;
  }
  return *this;
}

void CanvasCV::update()
{
  mCanvas = cv::Mat(mHeight, mWidth, CV_MAKETYPE(CV_8U, 3), colorToCvScalar(mBgColor));
}

cv::Scalar CanvasCV::colorToCvScalar(const Color &color)
{
  return cv::Scalar(static_cast<double>(color.blue()),
                    static_cast<double>(color.green()),
                    static_cast<double>(color.red()));
}


#endif // HAVE_OPENCV


/* ---------------------------------------------------------------------------------- */


} // Fin namespace graph

} // End namespace tl
