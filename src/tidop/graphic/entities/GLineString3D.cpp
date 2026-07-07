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

#include "tidop/graphic/entities/GLineString3D.h"
#include "tidop/graphic/render/Painter.h"

namespace tl
{

GLineString3D::GLineString3D()
    : GraphicEntity(GraphicEntity::Type::linestring_3d)
{
}

GLineString3D::GLineString3D(size_t size)
  : mGeometry(size),
    GraphicEntity(GraphicEntity::Type::linestring_3d)
{
}

GLineString3D::GLineString3D(const LineString<Point3d> &gLineString3D)
  : mGeometry(gLineString3D),
    GraphicEntity(GraphicEntity::Type::linestring_3d)
{
}

void GLineString3D::draw(Painter &painter) const
{
    size_t n = mGeometry.size();
    LineString2d ls(n);
    for (size_t i = 0; i < n; ++i)
        ls[i] = Point2d(mGeometry[i].x(), mGeometry[i].y());
    painter.drawLineString(ls);
}

auto GLineString3D::window() const -> BoundingBox<Point2d>
{
    auto bbox = tl::envelope(this->mGeometry);
    return BoundingBox<Point2d>(static_cast<Point2d>(bbox.min()), static_cast<Point2d>(bbox.max()));

}

auto GLineString3D::clone() const -> std::unique_ptr<GraphicEntity>
{
    return std::make_unique<GLineString3D>(*this);
}


} // namespace tl
