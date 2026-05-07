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

#include "tidop/graphic/entities/linestring.h"
#include "tidop/graphic/painter.h"
#include "tidop/geometry/algorithms/spatial/Envelope.h"

namespace tl
{



GLineString::GLineString()
  : GraphicEntity(GraphicEntity::Type::linestring_2d)
{
}

GLineString::GLineString(size_t size)
  : LineString<Point2d>(size),
    GraphicEntity(GraphicEntity::Type::linestring_2d)
{
}

GLineString::GLineString(const LineString<Point2d> &lineString)
  : LineString<Point2d>(lineString),
    GraphicEntity(GraphicEntity::Type::linestring_2d)
{
}

GLineString::GLineString(const GLineString &lineString)
  : LineString<Point2d>(lineString),
    GraphicEntity(lineString)
{
}

GLineString::GLineString(GLineString &&lineString) TL_NOEXCEPT
  : LineString<Point2d>(std::forward<LineString<Point2d>>(lineString)),
    GraphicEntity(std::forward<GraphicEntity>(lineString))
{
}

GLineString::~GLineString()
{
}

GLineString &GLineString::operator = (const GLineString &gLineString)
{
    if (this != &gLineString) {
        LineString<Point2d>::operator=(gLineString);
        GraphicEntity::operator=(gLineString);
    }
    return *this;
}

GLineString &GLineString::operator = (GLineString &&gLineString) TL_NOEXCEPT
{
    if (this != &gLineString) {
        LineString<Point2d>::operator=(std::forward<LineString<Point2d>>(gLineString));
        GraphicEntity::operator=(std::forward<GraphicEntity>(gLineString));
    }
    return *this;
}

void GLineString::draw(Painter &painter) const
{
    painter.drawLineString(*this);
}

auto GLineString::window() const -> BoundingBox<Point2d>
{
    auto bbox = tl::envelope(static_cast<const LineString<Point2d>&>(*this));
    return BoundingBox<Point2d>(static_cast<Point2d>(bbox.min()), static_cast<Point2d>(bbox.max()));

}





GLineString3D::GLineString3D()
  : GraphicEntity(GraphicEntity::Type::linestring_3d)
{
}

GLineString3D::GLineString3D(size_t size)
  : LineString<Point3d>(size),
    GraphicEntity(GraphicEntity::Type::linestring_3d)
{
}

GLineString3D::GLineString3D(const LineString<Point3d> &gLineString3D)
  : LineString<Point3d>(gLineString3D),
    GraphicEntity(GraphicEntity::Type::linestring_3d)
{
}

GLineString3D::GLineString3D(const GLineString3D &gLineString3D)
  : LineString<Point3d>(gLineString3D),
    GraphicEntity(gLineString3D)
{
}

GLineString3D::GLineString3D(GLineString3D &&gLineString3D) TL_NOEXCEPT
  : LineString<Point3d>(std::forward<LineString<Point3d>>(gLineString3D)),
    GraphicEntity(std::forward<GraphicEntity>(gLineString3D))
{
}

GLineString3D::~GLineString3D()
{
}

auto GLineString3D::operator =(const GLineString3D &gLineString3D) -> GLineString3D&
{
    if (this != &gLineString3D) {
        LineString<Point3d>::operator=(gLineString3D);
        GraphicEntity::operator=(gLineString3D);
    }
    return *this;
}

auto GLineString3D::operator =(GLineString3D &&gLineString3D) TL_NOEXCEPT -> GLineString3D&
{
    if (this != &gLineString3D) {
        LineString<Point3d>::operator=(std::forward<LineString<Point3d>>(gLineString3D));
        GraphicEntity::operator=(std::forward<GraphicEntity>(gLineString3D));
    }
    return *this;
}

void GLineString3D::draw(Painter &painter) const
{
    LineString2d ls(size());
    for (size_t i = 0; i < size(); ++i)
        ls[i] = Point2d((*this)[i].x(), (*this)[i].y());
    painter.drawLineString(ls);
}

auto GLineString3D::window() const -> BoundingBox<Point2d>
{
    auto bbox = tl::envelope(static_cast<const LineString<Point3d>&>(*this));
    return BoundingBox<Point2d>(static_cast<Point2d>(bbox.min()), static_cast<Point2d>(bbox.max()));

}




GMultiLineString::GMultiLineString()
  : GraphicEntity(GraphicEntity::Type::multiline_2d)
{
}

GMultiLineString::GMultiLineString(size_t size)
  : MultiLineString<Point2d>(size),
	GraphicEntity(GraphicEntity::Type::multiline_2d)
{
}

GMultiLineString::GMultiLineString(const MultiLineString<Point2d> &multiLineString)
  : MultiLineString<Point2d>(multiLineString),
    GraphicEntity(GraphicEntity::Type::multiline_2d)
{
}

GMultiLineString::GMultiLineString(const GMultiLineString &gMultiLineString)
  : MultiLineString<Point2d>(gMultiLineString),
    GraphicEntity(gMultiLineString)
{
}

GMultiLineString::GMultiLineString(GMultiLineString &&gMultiLineString) TL_NOEXCEPT
  : MultiLineString<Point2d>(std::forward<MultiLineString<Point2d>>(gMultiLineString)),
    GraphicEntity(std::forward<GraphicEntity>(gMultiLineString))
{
}

GMultiLineString::~GMultiLineString()
{
}

auto GMultiLineString::operator =(const GMultiLineString &gMultiLineString) -> GMultiLineString&
{
    if (this != &gMultiLineString) {
        MultiLineString<Point2d>::operator=(gMultiLineString);
        GraphicEntity::operator=(gMultiLineString);
    }
    return *this;
}

auto GMultiLineString::operator =(GMultiLineString &&gMultiLineString) TL_NOEXCEPT -> GMultiLineString&
{
    if (this != &gMultiLineString) {
        MultiLineString<Point2d>::operator=(std::forward<MultiLineString<Point2d>>(gMultiLineString));
        GraphicEntity::operator=(std::forward<GraphicEntity>(gMultiLineString));
    }
    return *this;
}

void GMultiLineString::draw(Painter &painter) const
{
    painter.drawMultiLineString(*this);
}

auto GMultiLineString::window() const -> BoundingBox<Point2d>
{
    return tl::envelope(static_cast<const MultiLineString<Point2d>&>(*this));
}


GMultiLineString3D::GMultiLineString3D()
  : GraphicEntity(GraphicEntity::Type::multiline_3d)
{
}

GMultiLineString3D::GMultiLineString3D(size_t size)
  : MultiLineString<Point3d>(size),
    GraphicEntity(GraphicEntity::Type::multiline_3d)
{
}

GMultiLineString3D::GMultiLineString3D(const MultiLineString<Point3d> &gMultiLineString3D)
  : MultiLineString<Point3d>(gMultiLineString3D),
    GraphicEntity(GraphicEntity::Type::multiline_3d)
{
}

GMultiLineString3D::GMultiLineString3D(const GMultiLineString3D &gMultiLineString3D)
  : MultiLineString<Point3d>(gMultiLineString3D),
    GraphicEntity(gMultiLineString3D)
{
}

GMultiLineString3D::GMultiLineString3D(GMultiLineString3D &&gMultiLineString3D) TL_NOEXCEPT
  : MultiLineString<Point3d>(std::forward<MultiLineString<Point3d>>(gMultiLineString3D)),
    GraphicEntity(std::forward<GraphicEntity>(gMultiLineString3D))
{
}

GMultiLineString3D::~GMultiLineString3D()
{
}

auto GMultiLineString3D::operator =(const GMultiLineString3D &gMultiLineString3D) -> GMultiLineString3D&
{
    if (this != &gMultiLineString3D) {
        MultiLineString<Point3d>::operator=(gMultiLineString3D);
        GraphicEntity::operator=(gMultiLineString3D);
    }
    return *this;
}

auto GMultiLineString3D::operator =(GMultiLineString3D &&gMultiLineString3D) TL_NOEXCEPT -> GMultiLineString3D&
{
    if (this != &gMultiLineString3D) {
        MultiLineString<Point3d>::operator=(std::forward<MultiLineString<Point3d>>(gMultiLineString3D));
        GraphicEntity::operator=(std::forward<GraphicEntity>(gMultiLineString3D));
    }
    return *this;
}

auto GMultiLineString3D::window() const -> BoundingBox<Point2d>
{
    auto bbox = tl::envelope(static_cast<const MultiLineString<Point3d>&>(*this));
    return BoundingBox<Point2d>(static_cast<Point2d>(bbox.min()), static_cast<Point2d>(bbox.max()));
}

void GMultiLineString3D::draw(Painter &painter) const
{
    GMultiLineString tmp(size());
    for (size_t i = 0; i < size(); ++i) {
        const auto &ln = (*this)[i];
        LineString<Point2d> ls(ln.size());
        for (size_t j = 0; j < ln.size(); ++j)
            ls[j] = Point2d(ln[j].x(), ln[j].y());
        tmp[i] = ls;
    }
    painter.drawMultiLineString(tmp);
}

} // End namespace tl
