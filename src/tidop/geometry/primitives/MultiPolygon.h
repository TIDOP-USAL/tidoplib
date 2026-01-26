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

/*! \file MultiPolygon.h
 * \brief Collection of Polygon objects.
 *
 * This file defines the MultiPolygon class template, which represents a
 * collection of Polygon objects. It inherits from GeometryBase and EntityContainer.
 * ### Classes
 * - \ref tl::MultiPolygon : Main template class for a collection of polygons.
 * ### Type Aliases
 * - \ref tl::MultiPolygon2i, \ref tl::MultiPolygon2f, \ref tl::MultiPolygon2d : 2D integer, float, and double multi-polygons.
 * - \ref tl::MultiPolygon3i, \ref tl::MultiPolygon3f, \ref tl::MultiPolygon3d : 3D integer, float, and double multi-polygons.
 * \see tl::GeometryBase, tl::EntityContainer, tl::Polygon
 */

#pragma once

#include "tidop/core/base/type_conversions.h"
#include "tidop/geometry/base/Dimension.h"
#include "tidop/geometry/base/Geometry.h"
#include "tidop/geometry/base/Traits.h"
#include "tidop/math/algebra/vector.h"

#include <array>

namespace tl
{
	
/*! \addtogroup Primitives
 *  \{
 */

/*!
 * \class MultiPolygon
 * \brief A collection of Polygon objects.
 *
 * \tparam Point_t Type of the points in the polygons (e.g., Point2d, Point3f).
 */
template<typename Point_t>
class MultiPolygon
  : public GeometryBase<MultiPolygon<Point_t>>, 
    public EntityContainer<Polygon<Point_t>> 
{
    using EntityContainer<Polygon<Point_t>>::EntityContainer;
};


// TYPE ALIASES

/*! \brief 2D multi-polygon with integer coordinates. */
using MultiPolygon2i = MultiPolygon<Point2i>;

/*! \brief 2D multi-polygon with float coordinates. */
using MultiPolygon2f = MultiPolygon<Point2f>;

/*! \brief 2D multi-polygon with double coordinates. */
using MultiPolygon2d = MultiPolygon<Point2d>;

/*! \brief 3D multi-polygon with integer coordinates. */
using MultiPolygon3i = MultiPolygon<Point3i>;

/*! \brief 3D multi-polygon with float coordinates. */
using MultiPolygon3f = MultiPolygon<Point3f>;

/*! \brief 3D multi-polygon with double coordinates. */
using MultiPolygon3d = MultiPolygon<Point3d>;

/*! \brief 4D multi-polygon with integer coordinates. */
using MultiPolygon4i = MultiPolygon<Point4i>;

/*! \brief 4D multi-polygon with float coordinates. */
using MultiPolygon4f = MultiPolygon<Point4f>;

/*! \brief 4D multi-polygon with double coordinates. */
using MultiPolygon4d = MultiPolygon<Point4d>;

/*! \brief 2D multi-polygon with integer coordinates with measure. */
using MultiPolygon2im = MultiPolygon<Point2im>;

/*! \brief 2D multi-polygon with float coordinates with measure. */
using MultiPolygon2fm = MultiPolygon<Point2fm>;

/*! \brief 2D multi-polygon with double coordinates with measure. */
using MultiPolygon2dm = MultiPolygon<Point2dm>;

/*! \brief 3D multi-polygon with integer coordinates with measure. */
using MultiPolygon3im = MultiPolygon<Point3im>;

/*! \brief 3D multi-polygon with float coordinates with measure. */
using MultiPolygon3fm = MultiPolygon<Point3fm>;

/*! \brief 3D multi-polygon with double coordinates with measure. */
using MultiPolygon3dm = MultiPolygon<Point3dm>;


/*! \} */

} // End namespace tl