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

#include "tidop/graphic/render/Painter.h"

#ifdef TL_HAVE_OPENCV
#include "opencv2/core/core.hpp"
#endif

#include "tidop/graphic/render/Canvas.h"

namespace tl
{

Painter::Painter(Canvas &canvas)
  : mCanvas(canvas)
{
    mDefaultStyle.setPen(Pen(Color(Color::Name::black), 1));
    Brush brush;
    brush.setStyle(Brush::Style::no_fill);
    mDefaultStyle.setBrush(brush);
    mDefaultStyle.setSymbol(Symbol(Symbol::Shape::cross, Color(Color::Name::black)));
    mDefaultStyle.setLabel(Label(Font("Arial", 12), Color(Color::Name::black)));
}

Painter::~Painter() = default;

void Painter::drawPoint(const GPoint &point)
{
    drawImpl(point.geometry(), resolveStyle(point.style()));
}

void Painter::drawPoint(const Point2d &point) const
{
    drawImpl(point, mDefaultStyle);
}

void Painter::drawLineString(const GLineString &lineString) const
{
    const GraphicStyle &entityStyle = lineString.style();
    GraphicStyle style = resolveStyle(entityStyle);
    drawImpl(lineString.geometry(), style);
}

void Painter::drawLineString(const LineString<Point2d> &lineString) const
{
    drawImpl(lineString, mDefaultStyle);
}

void Painter::drawPolygon(const GPolygon &polygon) const
{
    const GraphicStyle &entityStyle = polygon.style();
    GraphicStyle style = resolveStyle(entityStyle);
    drawImpl(polygon.geometry(), style);
}

void Painter::drawPolygon(const Polygon<Point2d> &polygon) const
{
    drawImpl(polygon, mDefaultStyle);
}

void Painter::drawMultiPoint(const GMultiPoint &multipoint) const
{
    const GraphicStyle &entityStyle = multipoint.style();
    GraphicStyle style = resolveStyle(entityStyle);
    drawImpl(multipoint.geometry(), style);
}

void Painter::drawMultiPoint(const MultiPoint<Point2d> &multipoint) const
{
    drawImpl(multipoint, mDefaultStyle);
}

void Painter::drawMultiLineString(const GMultiLineString &multiLineString) const
{
    const GraphicStyle &entityStyle = multiLineString.style();
    GraphicStyle style = resolveStyle(entityStyle);
    drawImpl(multiLineString.geometry(), style);
}

void Painter::drawMultiLineString(const MultiLineString<Point2d> &multiLineString) const
{
    drawImpl(multiLineString, mDefaultStyle);
}

void Painter::drawMultiPolygon(const GMultiPolygon &multiPolygon) const
{
    const GraphicStyle &entityStyle = multiPolygon.style();
    GraphicStyle style = resolveStyle(entityStyle);
    drawImpl(multiPolygon.geometry(), style);
}

void Painter::drawMultiPolygon(const MultiPolygon<Point2d> &multiPolygon) const
{
    drawImpl(multiPolygon, mDefaultStyle);
}

#ifdef TL_HAVE_OPENCV
void Painter::drawPicture(const cv::Mat &bmp) const
{
    unusedParameter(bmp);
}
#endif // TL_HAVE_OPENCV

void Painter::drawText(const Point2d &point, std::string_view text) const
{
    if (!mTransform.isEmpty()) {
        Point2d point_transform = mTransform.transform(point);
        mCanvas.drawText(point_transform, text, *mDefaultStyle.label());
    } else {
        mCanvas.drawText(point, text, *mDefaultStyle.label());
    }
}

void Painter::setTransform(Affine<double, 2> affine)
{
    mTransform = std::move(affine);
}

auto Painter::resolveStyle(const GraphicStyle &entityStyle) const -> GraphicStyle
{
    GraphicStyle style = mDefaultStyle;

    if (entityStyle.symbol()) {
        style.setSymbol(*entityStyle.symbol());
    }
    if (entityStyle.pen()) {
        style.setPen(*entityStyle.pen());
    }
    if (entityStyle.label()) {
        style.setLabel(*entityStyle.label());
    }
    if (entityStyle.brush()) {
        style.setBrush(*entityStyle.brush());
    }

    return style;   
}

void Painter::drawImpl(const Point2d &geometry, const GraphicStyle &style) const
{
    if (geometry.isEmpty()) return;

    auto &geometry_transformed = mTransform.isEmpty() ? geometry : mTransform.transform(geometry);

    mCanvas.draw(geometry_transformed, *style.symbol(), *style.pen());
}

void Painter::drawImpl(const LineString<Point2d> &geometry, const GraphicStyle &style) const
{
    if (geometry.isEmpty()) return;

    LineString<Point2d> geometry_transformed = geometry;

    if (!mTransform.isEmpty()) {
        size_t size = geometry.size();
        for (size_t i = 0; i < size; i++) {
            geometry_transformed[i] = mTransform.transform(geometry[i]);
        }
    }

    mCanvas.draw(geometry_transformed, *style.pen());
}

void Painter::drawImpl(const Polygon<Point2d> &geometry, const GraphicStyle &style) const
{
    if (geometry.isEmpty()) return;

    Polygon<Point2d> geometry_transformed = geometry;
    if (!mTransform.isEmpty()) {

        const auto &outer = geometry.outer();

        for (size_t i = 0; i < outer.size(); ++i) {
            geometry_transformed.outer()[i] = mTransform.transform(outer[i]);
        }

        const auto &inners = geometry.inners();
        for (size_t i = 0; i < inners.size(); ++i) {
            const auto &inner = inners[i];
            LinearRing<Point2d> inner_ring(inner.size());
            for (size_t j = 0; j < inner.size(); ++j) {
                inner_ring[j] = mTransform.transform(inner[j]);
            }
            geometry_transformed.addInner(inner_ring);
        }
    }

    mCanvas.draw(geometry_transformed, *style.pen(), *style.brush());
}

void Painter::drawImpl(const MultiPoint<Point2d> &geometry, const GraphicStyle &style) const
{
    if (geometry.isEmpty()) return;

    const Symbol symbol = *style.symbol();
    const Pen pen = *style.pen();

    if (mTransform.isEmpty()) {

        for (const auto &point : geometry) {
            mCanvas.draw(point, symbol, pen);
        }

    } else {

        for (const auto &point : geometry) {
            Point2d transformed = mTransform.transform(point);
            mCanvas.draw(transformed, symbol, pen);
        }

    }
}

void Painter::drawImpl(const MultiLineString<Point2d> &geometry, const GraphicStyle &style) const
{
    if (geometry.isEmpty()) return;

    const Pen pen = *style.pen();

    if (mTransform.isEmpty()) {

        for(const auto &lineString : geometry) {
            mCanvas.draw(lineString, pen);
        }

    } else {

        for (const auto &lineString : geometry) {

            LineString<Point2d> transformedLineString(lineString.size());
            for (size_t j = 0; j < lineString.size(); ++j) {
                transformedLineString[j] = mTransform.transform(lineString[j]);
            }
            mCanvas.draw(transformedLineString, pen);
        }
    }
}

void Painter::drawImpl(const MultiPolygon<Point2d> &geometry, const GraphicStyle &style) const
{
    if (geometry.isEmpty()) return;

    if (mTransform.isEmpty()) {

        for(const auto &polygon : geometry) {
            mCanvas.draw(polygon, *style.pen(), *style.brush());
        }

    } else {

        for (const auto &polygon : geometry) {

            Polygon<Point2d> transformedPolygon(polygon.outer().size());
            const auto &outer = polygon.outer();

            for (size_t j = 0; j < outer.size(); ++j) {
                transformedPolygon.outer()[j] = mTransform.transform(outer[j]);
            }

            const auto &inners = polygon.inners();
            for (size_t i = 0; i < inners.size(); ++i) {
                const auto &inner = inners[i];
                LinearRing<Point2d> inner_ring(inner.size());
                for (size_t j = 0; j < inner.size(); ++j) {
                    inner_ring[j] = mTransform.transform(inner[j]);
                }
                transformedPolygon.addInner(inner_ring);
            }

            mCanvas.draw(transformedPolygon, *style.pen(), *style.brush());

        }

    }
}



} // namespace tl
