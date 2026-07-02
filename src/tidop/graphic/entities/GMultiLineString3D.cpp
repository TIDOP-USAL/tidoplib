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

#include "tidop/graphic/entities/GMultiLineString3D.h"
#include "tidop/graphic/render/Painter.h"

namespace tl
{

GMultiLineString3D::GMultiLineString3D(size_t size)
  : mGeometry(size),
    GraphicEntity(GraphicEntity::Type::multiline_3d)
{
}

GMultiLineString3D::GMultiLineString3D(const MultiLineString<Point3d> &gMultiLineString3D)
  : mGeometry(gMultiLineString3D),
    GraphicEntity(GraphicEntity::Type::multiline_3d)
{
}

auto GMultiLineString3D::window() const -> BoundingBox<Point2d>
{
    auto bbox = tl::envelope(this->mGeometry);
    return BoundingBox<Point2d>(static_cast<Point2d>(bbox.min()), static_cast<Point2d>(bbox.max()));
}

void GMultiLineString3D::draw(Painter &painter) const
{
    size_t n = mGeometry.size();
    MultiLineString<Point2d> tmp(n);
    for (size_t i = 0; i < n; ++i) {
        const auto &ln = this->mGeometry[i];
        LineString<Point2d> ls(ln.size());
        for (size_t j = 0; j < ln.size(); ++j)
            ls[j] = Point2d(ln[j].x(), ln[j].y());
        tmp[i] = ls;
    }
    painter.drawMultiLineString(tmp);
}

auto GMultiLineString3D::clone() const -> std::unique_ptr<GraphicEntity>
{
    return std::make_unique<GMultiLineString3D>(*this);
}


} // namespace tl
