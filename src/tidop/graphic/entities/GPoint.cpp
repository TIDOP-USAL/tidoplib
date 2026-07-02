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

#include "tidop/graphic/entities/GPoint.h"
#include "tidop/graphic/render/Painter.h"

#include <memory>

namespace tl
{


GPoint::GPoint(double x, double y)
  : mGeometry(x, y),
    GraphicEntity(GraphicEntity::Type::point_2d)
{
}

GPoint::GPoint(const Point2d &pt)
  : mGeometry(pt),
    GraphicEntity(GraphicEntity::Type::point_2d)
{
}

auto GPoint::window() const -> BoundingBox<Point2d>
{
    return {this->geometry(), this->geometry()};
}

void GPoint::draw(Painter &painter) const
{
    painter.drawPoint(*this);
}

auto GPoint::clone() const -> std::unique_ptr<GraphicEntity>
{
    return std::make_unique<GPoint>(*this);
}


} // namespace tl
