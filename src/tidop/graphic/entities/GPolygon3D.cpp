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

#include "tidop/graphic/entities/GPolygon3D.h"
#include "tidop/graphic/render/Painter.h"
#include "tidop/geometry/algorithms/spatial/Envelope.h"

namespace tl
{


GPolygon3D::GPolygon3D(size_t size)
  : mGeometry(size),
    GraphicEntity(GraphicEntity::Type::polygon_3d)
{
}

GPolygon3D::GPolygon3D(const Polygon<Point3d> &polygon)
  : mGeometry(polygon),
    GraphicEntity(GraphicEntity::Type::polygon_3d)
{
}

auto GPolygon3D::window() const -> BoundingBox<Point2d>
{
    auto bbox = tl::envelope(this->geometry());
    return BoundingBox<Point2d>(static_cast<Point2d>(bbox.min()), static_cast<Point2d>(bbox.max()));
}

void GPolygon3D::draw(Painter &painter) const
{
    auto &outer = this->geometry().outer();
    Polygon<Point2d> poly2d(outer.size());
    for (size_t i = 0; i < outer.size(); ++i)
        poly2d.outer()[i] = Point2d(outer[i].x(), outer[i].y());
    painter.drawPolygon(poly2d);
}

auto GPolygon3D::clone() const -> std::unique_ptr<GraphicEntity>
{
    return std::make_unique<GPolygon3D>(*this);
}


} // namespace tl
