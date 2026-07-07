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

#include "tidop/graphic/entities/GPoint3D.h"
#include "tidop/graphic/render/Painter.h"

namespace tl
{

GPoint3D::GPoint3D()
  : GraphicEntity(GraphicEntity::Type::point_3d) 
{
}

GPoint3D::GPoint3D(double x, double y, double z)
  : mGeometry(x, y, z),
    GraphicEntity(GraphicEntity::Type::point_3d)
{
}

GPoint3D::GPoint3D(const Point3d &pt)
  : mGeometry(pt),
    GraphicEntity(GraphicEntity::Type::point_3d)
{
}

auto GPoint3D::window() const -> BoundingBox<Point2d>
{
    Point2d pt(this->geometry().x(), this->geometry().y());
    return {pt, pt};
}

void GPoint3D::draw(Painter &painter) const
{
    painter.drawPoint(Point2d(this->geometry().x(), this->geometry().y()));
}

auto GPoint3D::clone() const -> std::unique_ptr<GraphicEntity>
{
    return std::make_unique<GPoint3D>(*this);
}


} // namespace tl
