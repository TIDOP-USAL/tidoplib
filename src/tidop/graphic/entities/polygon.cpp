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

#include "tidop/graphic/entities/polygon.h"
#include "tidop/graphic/painter.h"

namespace tl
{


GPolygon::GPolygon()
  : GraphicEntity(GraphicEntity::Type::polygon_2d)
{
}

GPolygon::GPolygon(size_t size)
  : Polygon<Point2d>(size),
    GraphicEntity(GraphicEntity::Type::polygon_2d)
{
}

GPolygon::GPolygon(const Polygon<Point2d> &polygon)
  : Polygon<Point2d>(polygon),
    GraphicEntity(GraphicEntity::Type::polygon_2d)
{
}

GPolygon::GPolygon(const GPolygon &gPolygon)
  : Polygon<Point2d>(gPolygon),
    GraphicEntity(gPolygon)
{
}

GPolygon::GPolygon(GPolygon &&gPolygon) TL_NOEXCEPT
  : Polygon<Point2d>(std::forward<Polygon<Point2d>>(gPolygon)),
    GraphicEntity(std::forward<GraphicEntity>(gPolygon))
{
}

GPolygon::~GPolygon()
{
}

auto GPolygon::operator =(const GPolygon& gPolygon) -> GPolygon&
{
    if (this != &gPolygon) {
        Polygon<Point2d>::operator=(gPolygon);
        GraphicEntity::operator=(gPolygon);
    }
    return *this;
}

auto GPolygon::operator =(GPolygon&& gPolygon) TL_NOEXCEPT -> GPolygon&
{
    if (this != &gPolygon) {
        Polygon<Point2d>::operator=(std::forward<Polygon<Point2d>>(gPolygon));
        GraphicEntity::operator=(std::forward<GraphicEntity>(gPolygon));
    }
    return *this;
}

auto GPolygon::window() const -> BoundingBox<Point2d>
{
    return tl::envelope(static_cast<const Polygon<Point2d> &>(*this));
}

void GPolygon::draw(Painter &painter) const
{
    painter.drawPolygon(*this);
}





GPolygon3D::GPolygon3D()
  : GraphicEntity(GraphicEntity::Type::polygon_3d)
{
}

GPolygon3D::GPolygon3D(size_t size)
  : Polygon<Point3d>(size),
    GraphicEntity(GraphicEntity::Type::polygon_3d)
{
}

GPolygon3D::GPolygon3D(const Polygon<Point3d> &polygon)
  : Polygon<Point3d>(polygon),
    GraphicEntity(GraphicEntity::Type::polygon_3d)
{
}

GPolygon3D::GPolygon3D(const GPolygon3D &gPolygon3D)
  : Polygon<Point3d>(gPolygon3D),
    GraphicEntity(gPolygon3D)
{
}

GPolygon3D::GPolygon3D(GPolygon3D &&gPolygon3D) TL_NOEXCEPT
  : Polygon<Point3d>(std::forward<Polygon<Point3d>>(gPolygon3D)),
    GraphicEntity(std::forward<GraphicEntity>(gPolygon3D))
{
}

GPolygon3D::~GPolygon3D()
{
}

auto GPolygon3D::operator =(const GPolygon3D& gPolygon3D) -> GPolygon3D&
{
    if (this != &gPolygon3D) {
        Polygon<Point3d>::operator=(gPolygon3D);
        GraphicEntity::operator=(gPolygon3D);
    }
    return *this;
}

auto GPolygon3D::operator =(GPolygon3D&& gPolygon3D) TL_NOEXCEPT -> GPolygon3D&
{
    if (this != &gPolygon3D) {
        Polygon<Point3d>::operator=(std::forward<Polygon<Point3d>>(gPolygon3D));
        GraphicEntity::operator=(std::forward<GraphicEntity>(gPolygon3D));
    }
    return *this;
}

auto GPolygon3D::window() const -> BoundingBox<Point2d>
{
    auto bbox = tl::envelope(static_cast<const Polygon<Point3d>&>(*this));
    return BoundingBox<Point2d>(static_cast<Point2d>(bbox.min()), static_cast<Point2d>(bbox.max()));
}

void GPolygon3D::draw(Painter &painter) const
{
    auto &outer = this->outer();
    Polygon<Point2d> poly2d(outer.size());
    for (size_t i = 0; i < outer.size(); ++i)
        poly2d.outer()[i] = Point2d(outer[i].x(), outer[i].y());
    painter.drawPolygon(poly2d);
}





GMultiPolygon::GMultiPolygon()
  : GraphicEntity(GraphicEntity::Type::multipolygon_2d)
{
}

GMultiPolygon::GMultiPolygon(size_t size)
  : MultiPolygon<Point2d>(size),
    GraphicEntity(GraphicEntity::Type::multipolygon_2d)
{
}

GMultiPolygon::GMultiPolygon(const MultiPolygon<Point2d> &multiPolygon)
  : MultiPolygon<Point2d>(multiPolygon),
    GraphicEntity(GraphicEntity::Type::multipolygon_2d)
{
}

GMultiPolygon::GMultiPolygon(const GMultiPolygon &multiPolygon)
  : MultiPolygon<Point2d>(multiPolygon),
    GraphicEntity(multiPolygon)
{
}

GMultiPolygon::GMultiPolygon(GMultiPolygon&& multiPolygon) TL_NOEXCEPT
  : MultiPolygon<Point2d>(std::forward<MultiPolygon<Point2d>>(multiPolygon)),
    GraphicEntity(std::forward<GraphicEntity>(multiPolygon))
{
}

GMultiPolygon::~GMultiPolygon()
{
}

auto GMultiPolygon::operator =(const GMultiPolygon &multiPolygon) -> GMultiPolygon&
{
    if (this != &multiPolygon) {
        MultiPolygon<Point2d>::operator=(multiPolygon);
        GraphicEntity::operator=(multiPolygon);
    }

    return *this;
}

auto GMultiPolygon::operator=(GMultiPolygon &&multiPolygon) noexcept -> GMultiPolygon&
{
    if (this != &multiPolygon) {
        MultiPolygon<Point2d>::operator=(std::forward<MultiPolygon<Point2d>>(multiPolygon));
        GraphicEntity::operator=(std::forward<GraphicEntity>(multiPolygon));
    }

    return *this;
}

auto GMultiPolygon::window() const -> BoundingBox<Point2d>
{
    return tl::envelope(static_cast<const MultiPolygon<Point2d>&>(*this));
}

void GMultiPolygon::draw(Painter &painter) const
{
    painter.drawMultiPolygon(*this);
}





GMultiPolygon3D::GMultiPolygon3D()
  : GraphicEntity(GraphicEntity::Type::multipolygon_3d)
{
}

GMultiPolygon3D::GMultiPolygon3D(const MultiPolygon<Point3d> &multiPolygon)
  : MultiPolygon<Point3d>(multiPolygon),
    GraphicEntity(GraphicEntity::Type::multipolygon_3d)
{
}

GMultiPolygon3D::GMultiPolygon3D(size_t size)
  : MultiPolygon<Point3d>(size),
    GraphicEntity(GraphicEntity::Type::multipolygon_3d)
{
}

GMultiPolygon3D::GMultiPolygon3D(const GMultiPolygon3D &multiPolygon3D)
  : MultiPolygon<Point3d>(multiPolygon3D),
    GraphicEntity(multiPolygon3D)
{
}

GMultiPolygon3D::GMultiPolygon3D(GMultiPolygon3D &&multiPolygon3D) TL_NOEXCEPT
  : MultiPolygon<Point3d>(std::forward<MultiPolygon<Point3d>>(multiPolygon3D)),
    GraphicEntity(multiPolygon3D){
}

GMultiPolygon3D::~GMultiPolygon3D()
{
}

auto GMultiPolygon3D::operator =(const GMultiPolygon3D &multiPolygon3D) -> GMultiPolygon3D&
{
    if (this != &multiPolygon3D) {
        MultiPolygon<Point3d>::operator=(multiPolygon3D);
        GraphicEntity::operator=(multiPolygon3D);
    }
    return *this;
}

auto GMultiPolygon3D::operator=(GMultiPolygon3D &&multiPolygon3D) noexcept -> GMultiPolygon3D&
{
    if (this != &multiPolygon3D) {
        MultiPolygon<Point3d>::operator=(std::forward<MultiPolygon<Point3d>>(multiPolygon3D));
        GraphicEntity::operator=(std::forward<GraphicEntity>(multiPolygon3D));
    }
    return *this;
}

auto GMultiPolygon3D::window() const -> BoundingBox<Point2d>
{
    auto bbox = tl::envelope(static_cast<const MultiPolygon<Point3d>&>(*this));
    return BoundingBox<Point2d>(static_cast<Point2d>(bbox.min()), static_cast<Point2d>(bbox.max()));
}

void GMultiPolygon3D::draw(Painter &painter) const
{
    GMultiPolygon tmp(size());
    for (size_t i = 0; i < size(); ++i) {
        const auto &poly = (*this)[i];
        auto &outer = poly.outer();
        auto &inners = poly.inners();
        Polygon<Point2d> p(outer.size());
        for (size_t j = 0; j < outer.size(); ++j) {
            p.outer()[j] = Point2d(outer[j].x(), outer[j].y());
        }
        for (size_t k = 0; k < inners.size(); ++k) {
            const auto &inner = inners[k];
            LinearRing<Point2d> inner_ring(inner.size());
            for (size_t l = 0; l < inner.size(); ++l) {
                inner_ring[l] = Point2d(inner[l].x(), inner[l].y());
            }
            p.addInner(inner_ring);
        }
        tmp[i] = p;
    }
    painter.drawMultiPolygon(tmp);
}

} // End namespace tl
