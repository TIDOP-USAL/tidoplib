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

#include "tidop/graphic/entities/GMultiPolygon.h"
#include "tidop/graphic/render/Painter.h"
#include "tidop/geometry/algorithms/spatial/Envelope.h"

namespace tl
{

GMultiPolygon::GMultiPolygon()
  : GraphicEntity(GraphicEntity::Type::multipolygon_2d)
{
}

GMultiPolygon::GMultiPolygon(size_t size)
  : mGeometry(size),
    GraphicEntity(GraphicEntity::Type::multipolygon_2d)
{
}

GMultiPolygon::GMultiPolygon(const MultiPolygon<Point2d> &multiPolygon)
  : mGeometry(multiPolygon),
    GraphicEntity(GraphicEntity::Type::multipolygon_2d)
{
}

auto GMultiPolygon::window() const -> BoundingBox<Point2d>
{
    return tl::envelope(this->mGeometry);
}

void GMultiPolygon::draw(Painter &painter) const
{
    painter.drawMultiPolygon(*this);
}

auto GMultiPolygon::clone() const -> std::unique_ptr<GraphicEntity>
{
    return std::make_unique<GMultiPolygon>(*this);
}

} // namespace tl
