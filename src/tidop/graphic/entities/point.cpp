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

namespace tl
{


GPoint::GPoint()
  : GraphicEntity(GraphicEntity::Type::point_2d)
{
}

GPoint::GPoint(double x, double y)
  : Point<double>(x, y),
    GraphicEntity(GraphicEntity::Type::point_2d)
{
}

GPoint::GPoint(const Point<double> &pt)
  : Point<double>(pt),
    GraphicEntity(GraphicEntity::Type::point_2d)
{
}

GPoint::GPoint(const GPoint &pt)
  : Point<double>(pt),
    GraphicEntity(pt)
{
}

GPoint::GPoint(GPoint &&pt) TL_NOEXCEPT
  : Point<double>(std::forward<Point<double>>(pt)),
    GraphicEntity(std::forward<GraphicEntity>(pt))
{
}

GPoint::~GPoint()
{
}

auto GPoint::operator =(const GPoint& gPoint) -> GPoint&
{
    if (this != &gPoint) {
        Point<double>::operator=(gPoint);
        GraphicEntity::operator=(gPoint);
    }
    return *this;
}

auto GPoint::operator =(GPoint&& gPoint) TL_NOEXCEPT -> GPoint&
{
    if (this != &gPoint) {
        Point<double>::operator=(std::forward<Point<double>>(gPoint));
        GraphicEntity::operator=(std::forward<GraphicEntity>(gPoint));
    }
    return *this;
}







GPoint3D::GPoint3D()
  : GraphicEntity(GraphicEntity::Type::point_3d)
{
}

GPoint3D::GPoint3D(double x, double y, double z)
  : Point3<double>(x, y, z),
    GraphicEntity(GraphicEntity::Type::point_3d)
{
}

GPoint3D::GPoint3D(const Point3<double> &pt)
  : Point3<double>(pt),
    GraphicEntity(GraphicEntity::Type::point_3d)
{
}

GPoint3D::GPoint3D(const GPoint3D &pt)
  : Point3<double>(pt),
    GraphicEntity(pt)
{
}

GPoint3D::GPoint3D(GPoint3D &&pt) TL_NOEXCEPT
  : Point3<double>(std::forward<Point3<double>>(pt)),
    GraphicEntity(std::forward<GraphicEntity>(pt))
{
}

GPoint3D::~GPoint3D()
{
}

auto GPoint3D::operator =(const GPoint3D &gPoint) -> GPoint3D&
{
    if (this != &gPoint) {
        Point3<double>::operator=(gPoint);
        GraphicEntity::operator=(gPoint);
    }
    return *this;
}

auto GPoint3D::operator =(GPoint3D &&gPoint) TL_NOEXCEPT -> GPoint3D&
{
    if (this != &gPoint) {
        Point3<double>::operator=(std::forward<Point3<double>>(gPoint));
        GraphicEntity::operator=(std::forward<GraphicEntity>(gPoint));
    }
    return *this;
}






GMultiPoint::GMultiPoint()
  : GraphicEntity(GraphicEntity::Type::multipoint_2d)
{
}

GMultiPoint::GMultiPoint(size_t size)
  : MultiPoint<Point<double>>(size),
    GraphicEntity(GraphicEntity::Type::multipoint_2d)
{
}

GMultiPoint::GMultiPoint(const MultiPoint<Point<double>> &multiPoint)
  : MultiPoint<Point<double>>(multiPoint),
    GraphicEntity(GraphicEntity::Type::multipoint_2d)
{
}

GMultiPoint::GMultiPoint(const GMultiPoint &gMultiPoint)
  : MultiPoint<Point<double>>(gMultiPoint),
    GraphicEntity(gMultiPoint)
{
}

GMultiPoint::GMultiPoint(GMultiPoint &&gMultiPoint) TL_NOEXCEPT
  : MultiPoint<Point<double>>(std::forward<MultiPoint<Point<double>>>(gMultiPoint)),
    GraphicEntity(std::forward<GraphicEntity>(gMultiPoint))
{
}

GMultiPoint::~GMultiPoint()
{
}

auto GMultiPoint::operator =(const GMultiPoint& gMultiPoint) -> GMultiPoint&
{
    if (this != &gMultiPoint) {
        MultiPoint<Point<double>>::operator=(gMultiPoint);
        GraphicEntity::operator=(gMultiPoint);
    }
    return *this;
}

auto GMultiPoint::operator=(GMultiPoint&& gMultiPoint) TL_NOEXCEPT -> GMultiPoint&
{
    if (this != &gMultiPoint) {
        MultiPoint<Point<double>>::operator=(std::forward<MultiPoint<Point<double>>>(gMultiPoint));
        GraphicEntity::operator=(gMultiPoint);
    }
    return *this;
}


GMultiPoint3D::GMultiPoint3D()
  : GraphicEntity(GraphicEntity::Type::multipoint_3d)
{
}

GMultiPoint3D::GMultiPoint3D(size_t size)
  : MultiPoint3D<Point3<double>>(size),
    GraphicEntity(GraphicEntity::Type::multipoint_3d)
{
}

GMultiPoint3D::GMultiPoint3D(const MultiPoint3D<Point3<double>> &multiPoint)
  : MultiPoint3D<Point3<double>>(multiPoint),
    GraphicEntity(GraphicEntity::Type::multipoint_3d)
{
}

GMultiPoint3D::GMultiPoint3D(const GMultiPoint3D &gMultiPoint3D)
  : MultiPoint3D<Point3<double>>(gMultiPoint3D),
    GraphicEntity(gMultiPoint3D)
{
}

GMultiPoint3D::GMultiPoint3D(GMultiPoint3D &&gMultiPoint3D) TL_NOEXCEPT
  : MultiPoint3D<Point3<double>>(std::forward<MultiPoint3D<Point3<double>>>(gMultiPoint3D)),
    GraphicEntity(std::forward<GraphicEntity>(gMultiPoint3D)){
}

GMultiPoint3D::~GMultiPoint3D()
{
}

auto GMultiPoint3D::operator =(const GMultiPoint3D &gMultiPoint3D) -> GMultiPoint3D&
{
    if (this != &gMultiPoint3D) {
        MultiPoint3D<Point3<double>>::operator=(gMultiPoint3D);
        GraphicEntity::operator=(gMultiPoint3D);
    }
    return *this;
}

auto GMultiPoint3D::operator=(GMultiPoint3D &&gMultiPoint3D) TL_NOEXCEPT -> GMultiPoint3D&
{
    if (this != &gMultiPoint3D) {
        MultiPoint3D<Point3<double>>::operator=(std::forward<MultiPoint3D<Point3<double>>>(gMultiPoint3D));
        GraphicEntity::operator=(std::forward<GraphicEntity>(gMultiPoint3D));
    }
    return *this;
}


} // End namespace tl
