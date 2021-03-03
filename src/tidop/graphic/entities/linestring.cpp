/************************************************************************
 *                                                                      *
 * Copyright (C) 2020 by Tidop Research Group                           *
 *                                                                      *
 * This file is part of TidopLib                                        *
 *                                                                      *
 * TidopLib is free software: you can redistribute it and/or modify     *
 * it under the terms of the GNU General Public License as published by *
 * the Free Software Foundation, either version 3 of the License, or    *
 * (at your option) any later version.                                  *
 *                                                                      *
 * TidopLib is distributed in the hope that it will be useful,          *
 * but WITHOUT ANY WARRANTY; without even the implied warranty of       *
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the        *
 * GNU General Public License for more details.                         *
 *                                                                      *
 * You should have received a copy of the GNU General Public License    *
 * along with Foobar.  If not, see <http://www.gnu.org/licenses/>.      *
 *                                                                      *
 * @license GPL-3.0+ <http://spdx.org/licenses/GPL-3.0+>                *
 *                                                                      *
 ************************************************************************/

#include "tidop/graphic/entities/linestring.h"

namespace tl
{

namespace graph
{ 

/* ---------------------------------------------------------------------------------- */

GLineString::GLineString() 
  : LineString<Point<double>>(), 
    GraphicEntity(GraphicEntity::Type::linestring_2d)
{
}

GLineString::GLineString(const LineString<Point<double>> &lineString) 
  : LineString<Point<double>>(lineString), 
    GraphicEntity(GraphicEntity::Type::linestring_2d)
{
}

GLineString::GLineString(const GLineString &lineString) 
  : LineString<Point<double>>(lineString), 
    GraphicEntity(lineString)
{
}

GLineString::GLineString(GLineString &&lineString) TL_NOEXCEPT
  : LineString<Point<double>>(std::forward<LineString<Point<double>>>(lineString)), 
    GraphicEntity(std::forward<GraphicEntity>(lineString))
{
}

GLineString::~GLineString()
{
}

GLineString &GLineString::operator = (const GLineString &gLineString)
{
  if (this != &gLineString) {
    LineString<Point<double>>::operator=(gLineString);
    GraphicEntity::operator=(gLineString);
  }
  return *this;
}

GLineString &GLineString::operator = (GLineString &&gLineString) TL_NOEXCEPT
{
  if (this != &gLineString) {
    LineString<Point<double>>::operator=(std::forward<LineString<Point<double>>>(gLineString));
    GraphicEntity::operator=(std::forward<GraphicEntity>(gLineString));
  }
  return *this;
}

bool GLineString::isMultiEntity() const
{
  return true;
}

bool GLineString::isSimpleEntity() const
{
  return false;
}


/* ---------------------------------------------------------------------------------- */

GLineString3D::GLineString3D() 
  : LineString3D<Point3<double>>(),
    GraphicEntity(GraphicEntity::Type::linestring_3d)
{
}

GLineString3D::GLineString3D(const LineString3D<Point3<double>> &gLineString3D)
  : LineString3D<Point3<double>>(gLineString3D),
    GraphicEntity(GraphicEntity::Type::linestring_3d)
{
}

GLineString3D::GLineString3D(const GLineString3D &gLineString3D) 
  : LineString3D<Point3<double>>(gLineString3D),
    GraphicEntity(gLineString3D)
{
}

GLineString3D::GLineString3D(GLineString3D &&gLineString3D) TL_NOEXCEPT
  : LineString3D<Point3<double>>(std::forward<LineString3D<Point3<double>>>(gLineString3D)),
    GraphicEntity(std::forward<GraphicEntity>(gLineString3D))
{
}

GLineString3D::~GLineString3D()
{
}

GLineString3D &GLineString3D::operator = (const GLineString3D &gLineString3D)
{
  if (this != &gLineString3D) {
    LineString3D<Point3<double>>::operator=(gLineString3D);
    GraphicEntity::operator=(gLineString3D);
  }
  return *this;
}

GLineString3D &GLineString3D::operator = (GLineString3D &&gLineString3D) TL_NOEXCEPT
{
  if (this != &gLineString3D) {
    LineString3D<Point3<double>>::operator=(std::forward<LineString3D<Point3<double>>>(gLineString3D));
    GraphicEntity::operator=(std::forward<GraphicEntity>(gLineString3D));
  }
  return *this;
}

bool GLineString3D::isMultiEntity() const
{
  return false;
}

bool GLineString3D::isSimpleEntity() const
{
  return true;
}



/* ---------------------------------------------------------------------------------- */

GMultiLineString::GMultiLineString()
  : MultiLineString<Point<double>>(), 
    GraphicEntity(GraphicEntity::Type::multiline_2d)
{
}

GMultiLineString::GMultiLineString(const MultiLineString<Point<double>> &multiLineString)
  : MultiLineString<Point<double>>(multiLineString), 
    GraphicEntity(GraphicEntity::Type::multiline_2d)
{
}

GMultiLineString::GMultiLineString(const GMultiLineString &gMultiLineString)
  : MultiLineString<Point<double>>(gMultiLineString), 
    GraphicEntity(gMultiLineString)
{
}

GMultiLineString::GMultiLineString(GMultiLineString &&gMultiLineString) TL_NOEXCEPT
  : MultiLineString<Point<double>>(std::forward<MultiLineString<Point<double>>>(gMultiLineString)), 
    GraphicEntity(std::forward<GraphicEntity>(gMultiLineString))
{
}

GMultiLineString::~GMultiLineString()
{
}

GMultiLineString &GMultiLineString::operator = (const GMultiLineString &gMultiLineString)
{
  if (this != &gMultiLineString) {
    MultiLineString<Point<double>>::operator=(gMultiLineString);
    GraphicEntity::operator=(gMultiLineString);
  }
  return *this;
}

GMultiLineString &GMultiLineString::operator = (GMultiLineString &&gMultiLineString) TL_NOEXCEPT
{
  if (this != &gMultiLineString) {
    MultiLineString<Point<double>>::operator=(std::forward<MultiLineString<Point<double>>>(gMultiLineString));
    GraphicEntity::operator=(std::forward<GraphicEntity>(gMultiLineString));
  }
  return *this;
}

bool GMultiLineString::isMultiEntity() const
{
  return true;
}

bool GMultiLineString::isSimpleEntity() const
{
  return false;
}


/* ---------------------------------------------------------------------------------- */

GMultiLineString3D::GMultiLineString3D()
  : MultiLineString3D<Point3<double>>(),
    GraphicEntity(GraphicEntity::Type::multiline_3d)
{
}

GMultiLineString3D::GMultiLineString3D(const MultiLineString3D<Point3<double>> &multiLineString)
  : MultiLineString3D<Point3<double>>(multiLineString),
    GraphicEntity(GraphicEntity::Type::multiline_3d)
{
}

GMultiLineString3D::GMultiLineString3D(const GMultiLineString3D &gMultiLineString3D)
  : MultiLineString3D<Point3<double>>(gMultiLineString3D),
    GraphicEntity(gMultiLineString3D)
{
}

GMultiLineString3D::GMultiLineString3D(GMultiLineString3D &&gMultiLineString3D) TL_NOEXCEPT
  : MultiLineString3D<Point3<double>>(std::forward<MultiLineString3D<Point3<double>>>(gMultiLineString3D)),
    GraphicEntity(std::forward<GraphicEntity>(gMultiLineString3D))
{
}

GMultiLineString3D::~GMultiLineString3D()
{
}

GMultiLineString3D &GMultiLineString3D::operator = (const GMultiLineString3D &gMultiLineString3D)
{
  if (this != &gMultiLineString3D) {
    MultiLineString3D<Point3<double>>::operator=(gMultiLineString3D);
    GraphicEntity::operator=(gMultiLineString3D);
  }
  return *this;
}

GMultiLineString3D &GMultiLineString3D::operator = (GMultiLineString3D &&gMultiLineString3D) TL_NOEXCEPT
{
  if (this != &gMultiLineString3D) {
    MultiLineString3D<Point3<double>>::operator=(std::forward<MultiLineString3D<Point3<double>>>(gMultiLineString3D));
    GraphicEntity::operator=(std::forward<GraphicEntity>(gMultiLineString3D));
  }
  return *this;
}

bool GMultiLineString3D::isMultiEntity() const
{
  return true;
}

bool GMultiLineString3D::isSimpleEntity() const
{
  return false;
}


/* ---------------------------------------------------------------------------------- */

} // Fin namespace graph

} // End namespace TL
