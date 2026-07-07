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

#include "tidop/graphic/render/Canvas.h"
#include "tidop/graphic/render/Painter.h"
#include "tidop/geometry/primitives/Point.h"
#include "tidop/geometry/algorithms/spatial/Envelope.h"
#include "tidop/core/base/TypeConversions.h"

#ifdef TL_HAVE_OPENCV
#include "opencv2/core/core.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#endif

namespace tl
{


#ifdef TL_HAVE_OPENCV

CanvasCV::CanvasCV(Size<int> size)
  : mSize(std::move(size)),
    mBgColor(Color::Name::white)
{
    update();
}

void CanvasCV::draw(const Point2d &point, const Symbol &symbol, const Pen &pen)
{
    auto pen_color = colorToCvScalar(pen.color());
    auto pen_width = pen.width();

    Point2i pt = static_cast<Point2i>(point + symbol.offset());

    switch (symbol.shape()) {
        case Symbol::Shape::cross:
            cv::drawMarker(mCanvas, cv::Point(pt.x(), pt.y()), pen_color, cv::MARKER_CROSS, 10, 1);
            break;
        case Symbol::Shape::diagonal_cross:
            cv::drawMarker(mCanvas, cv::Point(pt.x(), pt.y()), pen_color, cv::MARKER_TILTED_CROSS, 10, 1);
            break;
        case Symbol::Shape::circle:
            cv::circle(mCanvas, cv::Point(pt.x(), pt.y()), 10, pen_color, 1);
            break;
        case Symbol::Shape::circle_filled:
            cv::circle(mCanvas, cv::Point(pt.x(), pt.y()), 10, pen_color, -1);
            break;
        case Symbol::Shape::square:
            cv::drawMarker(mCanvas, cv::Point(pt.x(), pt.y()), pen_color, cv::MARKER_SQUARE, 10, 1);
            break;
        case Symbol::Shape::square_filled:
            cv::drawMarker(mCanvas, cv::Point(pt.x(), pt.y()), pen_color, cv::MARKER_SQUARE, 10, -1);
            break;
        case Symbol::Shape::triangle:
            cv::drawMarker(mCanvas, cv::Point(pt.x(), pt.y()), pen_color, cv::MARKER_TRIANGLE_UP, 10, 1);
            break;
        case Symbol::Shape::triangle_filled:
            cv::drawMarker(mCanvas, cv::Point(pt.x(), pt.y()), pen_color, cv::MARKER_TRIANGLE_UP, 10, -1);
            break;
        case Symbol::Shape::star:
            cv::drawMarker(mCanvas, cv::Point(pt.x(), pt.y()), pen_color, cv::MARKER_STAR, 10, 1);
            break;
        case Symbol::Shape::star_filled:
            cv::drawMarker(mCanvas, cv::Point(pt.x(), pt.y()), pen_color, cv::MARKER_STAR, 10, -1);
            break;
        case Symbol::Shape::vertical_bar:
            break;
        default:
            //TODO: Igual es mejor no pintar ya que puede ser un texto
            cv::line(mCanvas, cv::Point(pt.x(), pt.y()), cv::Point(pt.x(), pt.y()), pen_color, pen_width);
            break;
    }

    //¿Mover a drawText?
//    auto style_label = style.label();
//    if (style_label && !style_label->text().empty()) {
//
//        Color foregroundColor = style_label->foregroundColor();
//
//#ifdef HAVE_QT
//        Font font = style_label->font();
//        cv::QtFont qt_font = cv::fontQt(font.name(),font.size(),
//                                        colorToCvScalar(foregroundColor),
//                                        font.isBold() ? cv::QtFontWeights::QT_FONT_BOLD : cv::QtFontWeights::QT_FONT_NORMAL,
//                                        font.isItalic() ? cv::QtFontStyles::QT_STYLE_ITALIC : cv::QtFontStyles::QT_STYLE_NORMAL);
//        cv::addText(mCanvas, style_label->text(), cv::Point(pt.x(), pt.y()), qt_font);
//#else
//        int fontFace = cv::FONT_HERSHEY_SIMPLEX;
//        double fontScale = 2;
//        int thickness = 3;
//        int baseline = 0;
//        //cv::Size textSize = cv::getTextSize(style_label->text(), fontFace,
//        //                                    fontScale, thickness, &baseline);
//        baseline += thickness;
//
//        cv::putText(mCanvas, style_label->text(), cv::Point(roundToInteger(point.x()), roundToInteger(point.y())), fontFace, fontScale,
//                    colorToCvScalar(foregroundColor), thickness, 8);
//#endif
//
//    }
}

void CanvasCV::draw(const LineString<Point2d> &lineString, const Pen &pen)
{
    size_t n = lineString.size();
    std::vector<std::vector<cv::Point>> pts(1, std::vector<cv::Point>(n));
    for (size_t i = 0; i < n; i++) {
        pts[0][i].x = static_cast<int>(lineString[i].x());
        pts[0][i].y = static_cast<int>(lineString[i].y());
    }

    if (!pen.pattern().empty()) {
        ///TODO: drawPolyLine(grd, cpts, npts, GVE_ReadyStyle::PenColor, GVE_ReadyStyle::PenWidth, GVE_ReadyStyle::PenPattern);
    } else {
        cv::polylines(mCanvas, pts, false, colorToCvScalar(pen.color()), pen.width());
    }
}


void CanvasCV::draw(const Polygon<Point2d> &polygon, const Pen &pen, const Brush &brush)
{
    // Añadir islas (huecos) si las hay
    const auto &outer = polygon.outer();
    size_t n = outer.size();
    std::vector<std::vector<cv::Point>> pts(1, std::vector<cv::Point>(n));
    for (size_t i = 0; i < n; i++) {
        pts[0][i].x = static_cast<int>(outer[i].x());
        pts[0][i].y = static_cast<int>(outer[i].y());
    }

    if (brush.style() == Brush::Style::solid) {
        Color fore_color = brush.foregroundColor();
        cv::fillPoly(mCanvas, pts, colorToCvScalar(fore_color));
    }

    if (!pen.pattern().empty()) {
        ///TODO: drawPolyLine(grd, cpts, npts, GVE_ReadyStyle::PenColor, GVE_ReadyStyle::PenWidth, GVE_ReadyStyle::PenPattern);
    } else {
        cv::polylines(mCanvas, pts, true, colorToCvScalar(pen.color()), pen.width());
    }

    // Mover a drawText()??
//    if (label && !label->text().empty()) {
//
//        Color foregroundColor = label->foregroundColor();
//        auto &point = envelope(polygon).min();
//
//#ifdef HAVE_QT
//        Font font = label->font();
//        cv::QtFont qt_font = cv::fontQt(font.name(), font.size(),
//            colorToCvScalar(foregroundColor),
//            font.isBold() ? cv::QtFontWeights::QT_FONT_BOLD : cv::QtFontWeights::QT_FONT_NORMAL,
//            font.isItalic() ? cv::QtFontStyles::QT_STYLE_ITALIC : cv::QtFontStyles::QT_STYLE_NORMAL);
//        cv::addText(mCanvas, label->text(), cv::Point(point.x(), point.y()), qt_font);
//#else
//        int fontFace = cv::FONT_HERSHEY_SIMPLEX;
//        double fontScale = 2;
//        int thickness = 3;
//        //int baseline = 0;
//        
//        cv::putText(mCanvas, label->text(), cv::Point(roundToInteger(point.x()), roundToInteger(point.y())), fontFace, fontScale,
//            colorToCvScalar(foregroundColor), thickness, 8);
//#endif
//
//
//    }

    /// TODO: completar
    //if (Symbol *style_symbol = style.symbol()) {
    //  ...
    //}
}

//void CanvasCV::draw(const MultiPoint<Point2d> &multiPoint, const GraphicStyle &style)
//{
//    for (const auto &pt : multiPoint) {
//        draw(pt, style);
//    }
//}
//
//void CanvasCV::draw(const MultiLineString<Point2d> &multiLineString, const GraphicStyle &style)
//{
//    for (const auto &ls : multiLineString) {
//        draw(ls, style);
//    }
//}
//
//void CanvasCV::draw(const MultiPolygon<Point2d> &multiPolygon, const GraphicStyle &style)
//{
//    for (const auto &poly : multiPolygon) {
//        draw(poly, style);
//    }
//}

void CanvasCV::drawText(const Point2d &point, std::string_view text, const Label &label)
{
    Color foregroundColor = label.foregroundColor();

#ifdef HAVE_QT
    Font font = label.font();
    cv::QtFont qt_font = cv::fontQt(font.name(), font.size(),
        colorToCvScalar(foregroundColor),
        font.isBold() ? cv::QtFontWeights::QT_FONT_BOLD : cv::QtFontWeights::QT_FONT_NORMAL,
        font.isItalic() ? cv::QtFontStyles::QT_STYLE_ITALIC : cv::QtFontStyles::QT_STYLE_NORMAL);
    cv::addText(mCanvas, text, cv::Point(point.x(), point.y()), qt_font);
#else
    int fontFace = cv::FONT_HERSHEY_SIMPLEX;
    double fontScale = 2;
    int thickness = 3;
    int baseline = 0;
    //cv::Size textSize = cv::getTextSize(text, fontFace,
    //                                    fontScale, thickness, &baseline);
    baseline += thickness;
    cv::putText(mCanvas, text.data(), cv::Point(roundToInteger(point.x()), roundToInteger(point.y())), fontFace, fontScale,
                colorToCvScalar(foregroundColor), thickness, 8);
#endif
}

void CanvasCV::update()
{
    if (mCanvas.rows != mSize.height() || mCanvas.cols != mSize.width()) {
        mCanvas = cv::Mat(mSize.height(), mSize.width(), CV_MAKETYPE(CV_8U, 3));
    }

    mCanvas.setTo(colorToCvScalar(mBgColor));
}



#endif // TL_HAVE_OPENCV



} // namespace tl
