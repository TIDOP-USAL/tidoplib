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

#include "tidop/graphic/entities/GMultiPoint.h"
#include "tidop/graphic/render/Painter.h"
#include "tidop/geometry/algorithms/spatial/Envelope.h"

namespace tl
{


GMultiPoint::GMultiPoint(size_t size)
  : mGeometry(size),
    GraphicEntity(GraphicEntity::Type::multipoint_2d)
{
}

GMultiPoint::GMultiPoint(const MultiPoint<Point2d> &multiPoint)
  : mGeometry(multiPoint),
    GraphicEntity(GraphicEntity::Type::multipoint_2d)
{
}

auto GMultiPoint::window() const -> BoundingBox<Point2d>
{
    return tl::envelope(this->geometry());
}

void GMultiPoint::draw(Painter &painter) const
{
    painter.drawMultiPoint(*this);
}

auto GMultiPoint::clone() const -> std::unique_ptr<GraphicEntity>
{
    return std::make_unique<GMultiPoint>(*this);
}

} // namespace tl
