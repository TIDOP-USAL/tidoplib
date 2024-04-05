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

namespace tl
{



GPolygon::GPolygon()
  : GraphicEntity(GraphicEntity::Type::polygon_2d)
{
}

GPolygon::GPolygon(size_t size)
  : Polygon<Point<double>>(size),
    GraphicEntity(GraphicEntity::Type::polygon_2d)
{
}

GPolygon::GPolygon(const Polygon<Point<double>> &polygon)
  : Polygon<Point<double>>(polygon),
    GraphicEntity(GraphicEntity::Type::polygon_2d)
{
}

GPolygon::GPolygon(const GPolygon &gPolygon)
  : Polygon<Point<double>>(gPolygon),
    GraphicEntity(gPolygon)
{
}

GPolygon::GPolygon(GPolygon &&gPolygon) TL_NOEXCEPT
  : Polygon<Point<double>>(std::forward<Polygon<Point<double>>>(gPolygon)),
    GraphicEntity(std::forward<GraphicEntity>(gPolygon))
{
}

GPolygon::~GPolygon()
{
}

auto GPolygon::operator =(const GPolygon& gPolygon) -> GPolygon&
{
    if (this != &gPolygon) {
        Polygon<Point<double>>::operator=(gPolygon);
        GraphicEntity::operator=(gPolygon);
    }
    return *this;
}

auto GPolygon::operator =(GPolygon&& gPolygon) TL_NOEXCEPT -> GPolygon&
{
    if (this != &gPolygon) {
        Polygon<Point<double>>::operator=(std::forward<Polygon<Point<double>>>(gPolygon));
        GraphicEntity::operator=(std::forward<GraphicEntity>(gPolygon));
    }
    return *this;
}






GPolygon3D::GPolygon3D()
  : GraphicEntity(GraphicEntity::Type::polygon_3d)
{
}

GPolygon3D::GPolygon3D(size_t size)
  : Polygon3D<Point3<double>>(size),
    GraphicEntity(GraphicEntity::Type::polygon_3d)
{
}

GPolygon3D::GPolygon3D(const Polygon3D<Point3<double>> &polygon)
  : Polygon3D<Point3<double>>(polygon),
    GraphicEntity(GraphicEntity::Type::polygon_3d)
{
}

GPolygon3D::GPolygon3D(const GPolygon3D &gPolygon3D)
  : Polygon3D<Point3<double>>(gPolygon3D),
    GraphicEntity(gPolygon3D)
{
}

GPolygon3D::GPolygon3D(GPolygon3D &&gPolygon3D) TL_NOEXCEPT
  : Polygon3D<Point3<double>>(std::forward<Polygon3D<Point3<double>>>(gPolygon3D)),
    GraphicEntity(std::forward<GraphicEntity>(gPolygon3D))
{
}

GPolygon3D::~GPolygon3D()
{
}

auto GPolygon3D::operator =(const GPolygon3D& gPolygon3D) -> GPolygon3D&
{
    if (this != &gPolygon3D) {
        Polygon3D<Point3<double>>::operator=(gPolygon3D);
        GraphicEntity::operator=(gPolygon3D);
    }
    return *this;
}

auto GPolygon3D::operator =(GPolygon3D&& gPolygon3D) TL_NOEXCEPT -> GPolygon3D&
{
    if (this != &gPolygon3D) {
        Polygon3D<Point3<double>>::operator=(std::forward<Polygon3D<Point3<double>>>(gPolygon3D));
        GraphicEntity::operator=(std::forward<GraphicEntity>(gPolygon3D));
    }
    return *this;
}





GMultiPolygon::GMultiPolygon()
  : GraphicEntity(GraphicEntity::Type::multipolygon_2d)
{
}

GMultiPolygon::GMultiPolygon(size_t size)
  : MultiPolygon<Point<double>>(size),
    GraphicEntity(GraphicEntity::Type::multipolygon_2d)
{
}

GMultiPolygon::GMultiPolygon(const MultiPolygon<Point<double>> &multiPolygon)
  : MultiPolygon<Point<double>>(multiPolygon),
    GraphicEntity(GraphicEntity::Type::multipolygon_2d)
{
}

GMultiPolygon::GMultiPolygon(const GMultiPolygon &multiPolygon)
  : MultiPolygon<Point<double>>(multiPolygon),
    GraphicEntity(multiPolygon)
{
}

GMultiPolygon::GMultiPolygon(GMultiPolygon&& multiPolygon) TL_NOEXCEPT
  : MultiPolygon<Point<double>>(std::forward<MultiPolygon<Point<double>>>(multiPolygon)),
    GraphicEntity(std::forward<GraphicEntity>(multiPolygon))
{
}

GMultiPolygon::~GMultiPolygon()
{
}

auto GMultiPolygon::operator =(const GMultiPolygon &multiPolygon) -> GMultiPolygon&
{
    if (this != &multiPolygon) {
        MultiPolygon<Point<double>>::operator=(multiPolygon);
        GraphicEntity::operator=(multiPolygon);
    }

    return *this;
}

auto GMultiPolygon::operator=(GMultiPolygon &&multiPolygon) noexcept -> GMultiPolygon&
{
    if (this != &multiPolygon) {
        MultiPolygon<Point<double>>::operator=(std::forward<MultiPolygon<Point<double>>>(multiPolygon));
        GraphicEntity::operator=(std::forward<GraphicEntity>(multiPolygon));
    }

    return *this;
}


GMultiPolygon3D::GMultiPolygon3D()
  : GraphicEntity(GraphicEntity::Type::multipolygon_3d)
{
}

GMultiPolygon3D::GMultiPolygon3D(const MultiPolygon3D<Point3<double>> &multiPolygon)
  : MultiPolygon3D<Point3<double>>(multiPolygon),
    GraphicEntity(GraphicEntity::Type::multipolygon_3d)
{
}

GMultiPolygon3D::GMultiPolygon3D(const GMultiPolygon3D &multiPolygon3D)
  : MultiPolygon3D<Point3<double>>(multiPolygon3D),
    GraphicEntity(multiPolygon3D)
{
}

GMultiPolygon3D::GMultiPolygon3D(GMultiPolygon3D &&multiPolygon3D) TL_NOEXCEPT
  : MultiPolygon3D<Point3<double>>(std::forward<MultiPolygon3D<Point3<double>>>(multiPolygon3D)),
    GraphicEntity(multiPolygon3D){
}

GMultiPolygon3D::~GMultiPolygon3D()
{
}

auto GMultiPolygon3D::operator =(const GMultiPolygon3D &multiPolygon3D) -> GMultiPolygon3D&
{
    if (this != &multiPolygon3D) {
        MultiPolygon3D<Point3<double>>::operator=(multiPolygon3D);
        GraphicEntity::operator=(multiPolygon3D);
    }
    return *this;
}

auto GMultiPolygon3D::operator=(GMultiPolygon3D &&multiPolygon3D) noexcept -> GMultiPolygon3D&
{
    if (this != &multiPolygon3D) {
        MultiPolygon3D<Point3<double>>::operator=(std::forward<MultiPolygon3D<Point3<double>>>(multiPolygon3D));
        GraphicEntity::operator=(std::forward<GraphicEntity>(multiPolygon3D));
    }
    return *this;
}

} // End namespace tl
