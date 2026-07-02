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

#include "tidop/graphic/entities/GMultiPoint3D.h"
#include "tidop/graphic/render/Painter.h"
#include "tidop/geometry/algorithms/spatial/Envelope.h"

namespace tl
{

GMultiPoint3D::GMultiPoint3D(size_t size)
  : mGeometry(size),
    GraphicEntity(GraphicEntity::Type::multipoint_3d)
{
}

GMultiPoint3D::GMultiPoint3D(const MultiPoint<Point3d> &multiPoint)
  : mGeometry(multiPoint),
    GraphicEntity(GraphicEntity::Type::multipoint_3d)
{
}

auto GMultiPoint3D::window() const -> BoundingBox<Point2d>
{
    auto bbox = tl::envelope(this->geometry());
    return BoundingBox<Point2d>(static_cast<Point2d>(bbox.min()), static_cast<Point2d>(bbox.max()));
}

void GMultiPoint3D::draw(Painter &painter) const
{
    auto size = mGeometry.size();
    MultiPoint<Point2d> tmp(size);
    for (size_t i = 0; i < size; ++i)
        tmp[i] = Point2d(mGeometry[i].x(), mGeometry[i].y());
    painter.drawMultiPoint(tmp);
}

auto GMultiPoint3D::clone() const -> std::unique_ptr<GraphicEntity>
{
    return std::make_unique<GMultiPoint3D>(*this);
}


} // namespace tl
