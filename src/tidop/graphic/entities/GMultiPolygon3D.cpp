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

#include "tidop/graphic/entities/GMultiPolygon3D.h"
#include "tidop/graphic/render/Painter.h"
#include "tidop/geometry/algorithms/spatial/Envelope.h"

namespace tl
{

GMultiPolygon3D::GMultiPolygon3D()
  : GraphicEntity(GraphicEntity::Type::multipolygon_3d)
{
}

GMultiPolygon3D::GMultiPolygon3D(const MultiPolygon<Point3d> &multiPolygon)
  : mGeometry(multiPolygon),
    GraphicEntity(GraphicEntity::Type::multipolygon_3d)
{
}

GMultiPolygon3D::GMultiPolygon3D(size_t size)
  : mGeometry(size),
    GraphicEntity(GraphicEntity::Type::multipolygon_3d)
{
}

auto GMultiPolygon3D::window() const -> BoundingBox<Point2d>
{
    auto bbox = tl::envelope(this->mGeometry);
    return BoundingBox<Point2d>(static_cast<Point2d>(bbox.min()), static_cast<Point2d>(bbox.max()));
}

void GMultiPolygon3D::draw(Painter &painter) const
{
    size_t n = this->mGeometry.size();
    MultiPolygon<Point2d> tmp(n);
    for (size_t i = 0; i < n; ++i) {
        const auto &poly = this->mGeometry[i];
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

auto GMultiPolygon3D::clone() const -> std::unique_ptr<GraphicEntity>
{
    return std::make_unique<GMultiPolygon3D>(*this);
}

} // namespace tl
