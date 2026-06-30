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
#endif

#include "tidop/graphic/canvas.h"

namespace tl
{

Painter::Painter()
  : mCanvas(nullptr)
{
    mDefaultStyle.setBrush(Brush());
    mDefaultStyle.setLabel(Label());
    mDefaultStyle.setPen(Pen());
    mDefaultStyle.setSymbol(Symbol());
}

Painter::Painter(Canvas *canvas)
  : mCanvas(canvas)
{
}

Painter::~Painter() = default;

void Painter::drawPoint(const GPoint &point)
{
    if (!mCanvas) {
        Message::error("Canvas not defined");
        return;
    }

    //Point2d geometry = mTransform.isEmpty() ? point.geometry() : mTransform.transform(point.geometry());

    const GraphicStyle &entityStyle = point.style();

    GraphicStyle style = resolvedStyle(entityStyle);

    //mCanvas->draw(geometry, style);
    drawImpl(point.geometry(), style);

    //if (!mTransform.isEmpty()) {

    //    Point2d point_transform = mTransform.transform(point.geometry());
    //    mCanvas->draw(point_transform, point);

    //} else {

    //    mCanvas->draw(point.geometry(), point);
    //}

}

void Painter::drawPoint(const Point2d &point) const
{
    drawImpl(point, mDefaultStyle);
}

void Painter::drawLineString(const GLineString &lineString) const
{
    //if (!mCanvas) {
    //    Message::error("Canvas not defined");
    //    return;
    //}

    //LineString<Point2d> geometry = lineString.geometry();

    //if (!mTransform.isEmpty()) {
    //    size_t size = geometry.size();
    //    for (size_t i = 0; i < size; i++) {
    //        geometry[i] = mTransform.transform(geometry[i]);
    //    }
    //}

    const GraphicStyle &entityStyle = lineString.style();
    GraphicStyle style = resolvedStyle(entityStyle);
    drawImpl(lineString.geometry(), style);

}

void Painter::drawLineString(const LineString<Point2d> &lineString) const
{
    //if (!mCanvas) {
    //    Message::error("Canvas not defined");
    //    return;
    //}

    //LineString<Point2d> geometry = lineString;

    //if (!mTransform.isEmpty()) {
    //    size_t size = geometry.size();
    //    for (size_t i = 0; i < size; i++) {
    //        geometry[i] = mTransform.transform(geometry[i]);
    //    }
    //}

    //    mCanvas->drawLineString(lineString, mDefaultStyle);
    drawImpl(lineString, mDefaultStyle);
}

void Painter::drawPolygon(const GPolygon &polygon) const
{
    //if (mCanvas) {

    //    if (!mTransform.isEmpty()) {

    //        const auto &geometry = polygon.geometry();
    //        const auto &outer = geometry.outer();
    //        Polygon<Point2d> polygon_transform(outer.size());

    //        for (size_t i = 0; i < outer.size(); ++i) {
    //            polygon_transform.outer()[i] = mTransform.transform(outer[i]);
    //        }

    //        const auto &inners = geometry.inners();
    //        for (size_t i = 0; i < inners.size(); ++i) {
    //            const auto &inner = inners[i];
    //            LinearRing<Point2d> inner_ring(inner.size());
    //            for (size_t j = 0; j < inner.size(); ++j) {
    //                inner_ring[j] = mTransform.transform(inner[j]);
    //            }
    //            polygon_transform.addInner(inner_ring);
    //        }

    //        mCanvas->drawPolygon(polygon_transform, polygon);

    //    } else {
    //        mCanvas->drawPolygon(polygon.geometry(), polygon);
    //    }

    //} else {
    //    Message::error("Canvas not defined");
    //}
    const GraphicStyle &entityStyle = polygon.style();
    GraphicStyle style = resolvedStyle(entityStyle);
    drawImpl(polygon.geometry(), style);
}

void Painter::drawPolygon(const Polygon<Point2d> &polygon) const
{
    drawImpl(polygon, mDefaultStyle);

    //if (mCanvas) {

    //    if (!mTransform.isEmpty()) {

    //        const auto &outer = polygon.outer();
    //        Polygon<Point2d> polygon_transform(outer.size());

    //        for (size_t i = 0; i < outer.size(); ++i) {
    //            polygon_transform.outer()[i] = mTransform.transform(outer[i]);
    //        }

    //        const auto &inners = polygon.inners();
    //        for (size_t i = 0; i < inners.size(); ++i) {
    //            const auto &inner = inners[i];
    //            LinearRing<Point2d> inner_ring(inner.size());
    //            for (size_t j = 0; j < inner.size(); ++j) {
    //                inner_ring[j] = mTransform.transform(inner[j]);
    //            }
    //            polygon_transform.addInner(inner_ring);
    //        }

    //        mCanvas->drawPolygon(polygon_transform, *this);

    //    } else {
    //        mCanvas->drawPolygon(polygon, *this);
    //    }

    //} else {
    //    Message::error("Canvas not defined");
    //}
}

void Painter::drawMultiPoint(const GMultiPoint &multipoint) const
{
    const GraphicStyle &entityStyle = multipoint.style();
    GraphicStyle style = resolvedStyle(entityStyle);
    drawImpl(multipoint.geometry(), style);
//    if (mCanvas) {
//
//        if (!mTransform.isEmpty()) {
//
//            const auto &geometry = multipoint.geometry();
//            size_t size = geometry.size();
//
//            MultiPoint<Point2d> mp_trans(size);
//
//            for (size_t i = 0; i < size; ++i) {
//                mp_trans[i] = mTransform.transform(geometry[i]);
//            }
//
//            mCanvas->drawMultiPoint(mp_trans, multipoint);
//
//        } else {
//            mCanvas->drawMultiPoint(multipoint.geometry(), multipoint);
//        }
//
//    } else {
//        Message::error("Canvas not defined");
//    }
}

void Painter::drawMultiPoint(const MultiPoint<Point2d> &multipoint) const
{
    drawImpl(multipoint, mDefaultStyle);
//    if (mCanvas) {
//
//        if (!mTransform.isEmpty()) {
//
//            MultiPoint<Point2d> mp_trans(multipoint.size());
//
//            for (size_t i = 0; i < multipoint.size(); ++i) {
//                mp_trans[i] = mTransform.transform(multipoint[i]);
//            }
//
//            mCanvas->drawMultiPoint(mp_trans, *this);
//
//        } else {
//            mCanvas->drawMultiPoint(multipoint, *this);
//        }
//
//    } else {
//        Message::error("Canvas not defined");
//    }
}

void Painter::drawMultiLineString(const GMultiLineString &multiLineString) const
{
//    if (mCanvas) {
//
//        if (!mTransform.isEmpty()) {
//
//            const auto &geometry = multiLineString.geometry();
//            size_t size = geometry.size();
//
//            MultiLineString<Point2d> mls_trans(size);
//
//            for (size_t i = 0; i < size; ++i) {
//                const auto &ls = geometry[i];
//                mls_trans[i].resize(ls.size());
//                for (size_t j = 0; j < ls.size(); ++j) {
//                    mls_trans[i][j] = mTransform.transform(ls[j]);
//                }
//            }
//
//            mCanvas->drawMultiLineString(mls_trans, multiLineString);
//
//        } else {
//            mCanvas->drawMultiLineString(multiLineString.geometry(), multiLineString);
//        }
//
//    } else {
//        Message::error("Canvas not defined");
//    }
    const GraphicStyle &entityStyle = multiLineString.style();
    GraphicStyle style = resolvedStyle(entityStyle);
    drawImpl(multiLineString.geometry(), style);
}

void Painter::drawMultiLineString(const MultiLineString<Point2d> &multiLineString) const
{
    drawImpl(multiLineString, mDefaultStyle);
}

void Painter::drawMultiPolygon(const GMultiPolygon &multiPolygon) const
{
    const GraphicStyle &entityStyle = multiPolygon.style();
    GraphicStyle style = resolvedStyle(entityStyle);
    drawImpl(multiPolygon.geometry(), style);
//    if (mCanvas) {
//
//        if (!mTransform.isEmpty()) {
//
//            MultiPolygon<Point2d> mp_trans;
//
//            const auto &geometry = multiPolygon.geometry();
//            size_t size = geometry.size();
//
//            for (size_t i = 0; i < size; ++i) {
//                const auto &poly = geometry[i];
//                const auto &outer = poly.outer();
//                Polygon<Point2d> polygon(outer.size());
//                for (size_t j = 0; j < outer.size(); ++j) {
//                    polygon.outer()[j] = mTransform.transform(outer[j]);
//                }
//                // Holes are ignored as in drawPolygon
//                mp_trans.push_back(polygon);
//            }
//            
//            mCanvas->drawMultiPolygon(mp_trans, multiPolygon);
//
//        } else {
//            mCanvas->drawMultiPolygon(multiPolygon.geometry(), multiPolygon);
//        }
//
//    } else {
//        Message::error("Canvas not defined");
//    }
}

void Painter::drawMultiPolygon(const MultiPolygon<Point2d> &multiPolygon) const
{
    drawImpl(multiPolygon, mDefaultStyle);
//    if (mCanvas) {
//
//        if (!mTransform.isEmpty()) {
//
//            MultiPolygon<Point2d> mp_trans;
//
//            for (size_t i = 0; i < multiPolygon.size(); ++i) {
//                const auto &poly = multiPolygon[i];
//                const auto &outer = poly.outer();
//                Polygon<Point2d> polygon(outer.size());
//                for (size_t j = 0; j < outer.size(); ++j) {
//                    polygon.outer()[j] = mTransform.transform(outer[j]);
//                }
//                // Holes are ignored as in drawPolygon
//                mp_trans.push_back(polygon);
//            }
//
//            mCanvas->drawMultiPolygon(mp_trans, *this);
//
//        } else {
//            mCanvas->drawMultiPolygon(multiPolygon, *this);
//        }
//
//    } else {
//        Message::error("Canvas not defined");
//    }
}

#ifdef TL_HAVE_OPENCV
void Painter::drawPicture(const cv::Mat &bmp) const
{
    ///mCanvas->
    unusedParameter(bmp);
}
#endif // TL_HAVE_OPENCV

void Painter::drawText(const Point2d &point, const std::string &text) const
{
    if (mCanvas) {

        if (!mTransform.isEmpty()) {
            Point2d point_transform = mTransform.transform(point);
            mCanvas->drawText(point_transform, text, mDefaultStyle);
        } else {
            mCanvas->drawText(point, text, mDefaultStyle);
        }

    } else {
        Message::error("Canvas not defined");
    }
}

void Painter::setCanvas(Canvas *canvas)
{
    mCanvas = canvas;
}

//void Painter::pushStyle(const GraphicStyle &style)
//{
//    mStyleStack.push_back(static_cast<const GraphicStyle &>(*this));
//    GraphicStyle::operator=(style);
//}
//
//void Painter::popStyle()
//{
//    if (!mStyleStack.empty()) {
//        GraphicStyle::operator=(mStyleStack.back());
//        mStyleStack.pop_back();
//    }
//}

void Painter::setTransform(const Affine<double, 2> &affine)
{
    mTransform = affine;
}

auto Painter::resolvedStyle(const GraphicStyle &entityStyle) const -> GraphicStyle
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
    if (!mCanvas) {
        Message::error("Canvas not defined");
        return;
    }

    auto &geometry_transformed = mTransform.isEmpty() ? geometry : mTransform.transform(geometry);

    mCanvas->draw(geometry_transformed, style);
}

void Painter::drawImpl(const LineString<Point2d> &geometry, const GraphicStyle &style) const
{
    if (!mCanvas) {
        Message::error("Canvas not defined");
        return;
    }

    LineString<Point2d> geometry_transformed = geometry;

    if (!mTransform.isEmpty()) {
        size_t size = geometry.size();
        for (size_t i = 0; i < size; i++) {
            geometry_transformed[i] = mTransform.transform(geometry[i]);
        }
    }

    mCanvas->draw(geometry_transformed, style);
}

void Painter::drawImpl(const Polygon<Point2d> &geometry, const GraphicStyle &style) const
{
    if (!mCanvas) {
        Message::error("Canvas not defined");
        return;
    }

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

    mCanvas->draw(geometry_transformed, style);
}

void Painter::drawImpl(const MultiPoint<Point2d> &geometry, const GraphicStyle &style) const
{
    if (!mCanvas) {
        Message::error("Canvas not defined");
        return;
    }

    MultiPoint<Point2d> geometry_transformed = geometry;

    if (!mTransform.isEmpty()) {

        size_t size = geometry.size();

        for (size_t i = 0; i < size; ++i) {
            geometry_transformed[i] = mTransform.transform(geometry[i]);
        }

    }

    mCanvas->draw(geometry_transformed, style);
}

void Painter::drawImpl(const MultiLineString<Point2d> &geometry, const GraphicStyle &style) const
{
    if (!mCanvas) {
        Message::error("Canvas not defined");
        return;
    }

    MultiLineString<Point2d> geometry_transformed = geometry;

    if (!mTransform.isEmpty()) {
        size_t size = geometry.size();
        for (size_t i = 0; i < size; ++i) {
            const auto &ls = geometry[i];
            for (size_t j = 0; j < ls.size(); ++j) {
                geometry_transformed[i][j] = mTransform.transform(ls[j]);
            }
        }
    }

    mCanvas->draw(geometry_transformed, style);
}

void Painter::drawImpl(const MultiPolygon<Point2d> &geometry, const GraphicStyle &style) const
{
    if (!mCanvas) {
        Message::error("Canvas not defined");
        return;
    }

    MultiPolygon<Point2d> geometry_transformed = geometry;
    if (!mTransform.isEmpty()) {

        size_t size = geometry.size();

        for (size_t i = 0; i < size; ++i) {
            const auto &poly = geometry[i];
            const auto &outer = poly.outer();
            Polygon<Point2d> polygon(outer.size());
            for (size_t j = 0; j < outer.size(); ++j) {
                polygon.outer()[j] = mTransform.transform(outer[j]);
            }
            // Holes are ignored as in drawPolygon
            geometry_transformed.push_back(polygon);
        }
    }

    mCanvas->draw(geometry_transformed, style);
}



} // End namespace tl
