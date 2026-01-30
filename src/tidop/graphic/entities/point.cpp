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

#include "tidop/graphic/entities/point.h"
#include "tidop/graphic/painter.h"
#include "tidop/geometry/spatial/BoundingBox.h"

namespace tl
{


GPoint::GPoint()
  : GraphicEntity(GraphicEntity::Type::point_2d)
{
}

GPoint::GPoint(double x, double y)
  : Point2d(x, y),
    GraphicEntity(GraphicEntity::Type::point_2d)
{
}

GPoint::GPoint(const Point2d &pt)
  : Point2d(pt),
    GraphicEntity(GraphicEntity::Type::point_2d)
{
}

GPoint::GPoint(const GPoint &pt)
  : Point2d(pt),
    GraphicEntity(pt)
{
}

GPoint::GPoint(GPoint &&pt) TL_NOEXCEPT
  : Point2d(std::forward<Point2d>(pt)),
    GraphicEntity(std::forward<GraphicEntity>(pt))
{
}

GPoint::~GPoint()
{
}

auto GPoint::operator =(const GPoint& gPoint) -> GPoint&
{
    if (this != &gPoint) {
        Point2d::operator=(gPoint);
        GraphicEntity::operator=(gPoint);
    }
    return *this;
}

auto GPoint::operator =(GPoint&& gPoint) TL_NOEXCEPT -> GPoint&
{
    if (this != &gPoint) {
        Point2d::operator=(std::forward<Point2d>(gPoint));
        GraphicEntity::operator=(std::forward<GraphicEntity>(gPoint));
    }
    return *this;
}

auto GPoint::window() const -> BoundingBox<Point2d>
{
    Point2d pt(this->x(), this->y());
    return {pt, pt};
}

void GPoint::draw(Painter &painter) const
{
    painter.drawPoint(*this);
}





GPoint3D::GPoint3D()
  : GraphicEntity(GraphicEntity::Type::point_3d)
{
}

GPoint3D::GPoint3D(double x, double y, double z)
  : Point3d(x, y, z),
    GraphicEntity(GraphicEntity::Type::point_3d)
{
}

GPoint3D::GPoint3D(const Point3d &pt)
  : Point3d(pt),
    GraphicEntity(GraphicEntity::Type::point_3d)
{
}

GPoint3D::GPoint3D(const GPoint3D &pt)
  : Point3d(pt),
    GraphicEntity(pt)
{
}

GPoint3D::GPoint3D(GPoint3D &&pt) TL_NOEXCEPT
  : Point3d(std::forward<Point3d>(pt)),
    GraphicEntity(std::forward<GraphicEntity>(pt))
{
}

GPoint3D::~GPoint3D()
{
}

auto GPoint3D::operator =(const GPoint3D &gPoint) -> GPoint3D&
{
    if (this != &gPoint) {
        Point3d::operator=(gPoint);
        GraphicEntity::operator=(gPoint);
    }
    return *this;
}

auto GPoint3D::operator =(GPoint3D &&gPoint) TL_NOEXCEPT -> GPoint3D&
{
    if (this != &gPoint) {
        Point3d::operator=(std::forward<Point3d>(gPoint));
        GraphicEntity::operator=(std::forward<GraphicEntity>(gPoint));
    }
    return *this;
}

auto GPoint3D::window() const -> BoundingBox<Point2d>
{
    Point2d pt(this->x(), this->y());
    return {pt, pt};
}

void GPoint3D::draw(Painter &painter) const
{
    painter.drawPoint(Point2d(this->x(), this->y()));
}





GMultiPoint::GMultiPoint()
  : GraphicEntity(GraphicEntity::Type::multipoint_2d)
{
}

GMultiPoint::GMultiPoint(size_t size)
  : MultiPoint<Point2d>(size),
    GraphicEntity(GraphicEntity::Type::multipoint_2d)
{
}

GMultiPoint::GMultiPoint(const MultiPoint<Point2d> &multiPoint)
  : MultiPoint<Point2d>(multiPoint),
    GraphicEntity(GraphicEntity::Type::multipoint_2d)
{
}

GMultiPoint::GMultiPoint(const GMultiPoint &gMultiPoint)
  : MultiPoint<Point2d>(gMultiPoint),
    GraphicEntity(gMultiPoint)
{
}

GMultiPoint::GMultiPoint(GMultiPoint &&gMultiPoint) TL_NOEXCEPT
  : MultiPoint<Point2d>(std::forward<MultiPoint<Point2d>>(gMultiPoint)),
    GraphicEntity(std::forward<GraphicEntity>(gMultiPoint))
{
}

GMultiPoint::~GMultiPoint()
{
}

auto GMultiPoint::operator =(const GMultiPoint& gMultiPoint) -> GMultiPoint&
{
    if (this != &gMultiPoint) {
        MultiPoint<Point2d>::operator=(gMultiPoint);
        GraphicEntity::operator=(gMultiPoint);
    }
    return *this;
}

auto GMultiPoint::operator=(GMultiPoint&& gMultiPoint) TL_NOEXCEPT -> GMultiPoint&
{
    if (this != &gMultiPoint) {
        MultiPoint<Point2d>::operator=(std::forward<MultiPoint<Point2d>>(gMultiPoint));
        GraphicEntity::operator=(gMultiPoint);
    }
    return *this;
}

auto GMultiPoint::window() const -> BoundingBox<Point2d>
{
    return tl::envelope(static_cast<const MultiPoint<Point2d>&>(*this));
}

void GMultiPoint::draw(Painter &painter) const
{
    painter.drawMultiPoint(*this);
}




GMultiPoint3D::GMultiPoint3D()
  : GraphicEntity(GraphicEntity::Type::multipoint_3d)
{
}

GMultiPoint3D::GMultiPoint3D(size_t size)
  : MultiPoint<Point3d>(size),
    GraphicEntity(GraphicEntity::Type::multipoint_3d)
{
}

GMultiPoint3D::GMultiPoint3D(const MultiPoint<Point3d> &multiPoint)
  : MultiPoint<Point3d>(multiPoint),
    GraphicEntity(GraphicEntity::Type::multipoint_3d)
{
}

GMultiPoint3D::GMultiPoint3D(const GMultiPoint3D &gMultiPoint3D)
  : MultiPoint<Point3d>(gMultiPoint3D),
    GraphicEntity(gMultiPoint3D)
{
}

GMultiPoint3D::GMultiPoint3D(GMultiPoint3D &&gMultiPoint3D) TL_NOEXCEPT
  : MultiPoint<Point3d>(std::forward<MultiPoint<Point3d>>(gMultiPoint3D)),
    GraphicEntity(std::forward<GraphicEntity>(gMultiPoint3D)){
}

GMultiPoint3D::~GMultiPoint3D()
{
}

auto GMultiPoint3D::operator =(const GMultiPoint3D &gMultiPoint3D) -> GMultiPoint3D&
{
    if (this != &gMultiPoint3D) {
        MultiPoint<Point3d>::operator=(gMultiPoint3D);
        GraphicEntity::operator=(gMultiPoint3D);
    }
    return *this;
}

auto GMultiPoint3D::operator=(GMultiPoint3D &&gMultiPoint3D) TL_NOEXCEPT -> GMultiPoint3D&
{
    if (this != &gMultiPoint3D) {
        MultiPoint<Point3d>::operator=(std::forward<MultiPoint<Point3d>>(gMultiPoint3D));
        GraphicEntity::operator=(std::forward<GraphicEntity>(gMultiPoint3D));
    }
    return *this;
}

auto GMultiPoint3D::window() const -> BoundingBox<Point2d>
{
    auto bbox = tl::envelope(static_cast<const MultiPoint<Point3d>&>(*this));
    return BoundingBox<Point2d>(static_cast<Point2d>(bbox.pt1()), static_cast<Point2d>(bbox.pt2()));
}

void GMultiPoint3D::draw(Painter &painter) const
{
    GMultiPoint tmp(size());
    for (size_t i = 0; i < size(); ++i)
        tmp[i] = Point2d((*this)[i].x(), (*this)[i].y());
    painter.drawMultiPoint(tmp);
}

} // End namespace tl
