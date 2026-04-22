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

/*! \file MultiPoint.h
 * \brief Collection of Point objects.
 *
 * This file defines the MultiPoint class template, which represents a
 * collection of Point objects. It inherits from GeometryBase and GeometryContainer.
 * ### Classes
 * - \ref tl::MultiPoint : Main template class for a collection of points.
 * ### Type Aliases
 * - \ref tl::MultiPoint2i, \ref tl::MultiPoint2f, \ref tl::MultiPoint2d : 2D integer, float, and double multi-points.
 * - \ref tl::MultiPoint3i, \ref tl::MultiPoint3f, \ref tl::MultiPoint3d : 3D integer, float, and double multi-points.
 * \see tl::GeometryBase, tl::GeometryContainer, tl::Point
 */

#pragma once

#include "tidop/geometry/base/Geometry.h"
#include "tidop/geometry/base/GeometryContainer.h"
#include "tidop/geometry/primitives/Point.h"

namespace tl
{

/*! \addtogroup Primitives
 *  \{
 */

/*!
 * \class MultiPoint
 * \brief A collection of Point objects.
 *
 * \tparam Point_t Type of the points (e.g., Point2d, Point3f).
 */
template<typename Point_t>
class MultiPoint 
  : public Geometry<MultiPoint<Point_t>>, 
    public GeometryContainer<Point_t> 
{
	
public:

    using GeometryContainer<Point_t>::GeometryContainer;
};


// TYPE ALIASES

/*! \brief 2D multi-point with integer coordinates. */
using MultiPoint2i = MultiPoint<Point2i>;

/*! \brief 2D multi-point with float coordinates. */
using MultiPoint2f = MultiPoint<Point2f>;

/*! \brief 2D multi-point with double coordinates. */
using MultiPoint2d = MultiPoint<Point2d>;

/*! \brief 3D multi-point with integer coordinates. */
using MultiPoint3i = MultiPoint<Point3i>;

/*! \brief 3D multi-point with float coordinates. */
using MultiPoint3f = MultiPoint<Point3f>;

/*! \brief 3D multi-point with double coordinates. */
using MultiPoint3d = MultiPoint<Point3d>;

/*! \brief 4D multi-point with integer coordinates. */
using MultiPoint4i = MultiPoint<Point4i>;

/*! \brief 4D multi-point with float coordinates. */
using MultiPoint4f = MultiPoint<Point4f>;

/*! \brief 4D multi-point with double coordinates. */
using MultiPoint4d = MultiPoint<Point4d>;

/*! \brief 2D multi-point with integer coordinates with measure. */
using MultiPoint2im = MultiPoint<Point2im>;

/*! \brief 2D multi-point with float coordinates with measure. */
using MultiPoint2fm = MultiPoint<Point2fm>;

/*! \brief 2D multi-point with double coordinates with measure. */
using MultiPoint2dm = MultiPoint<Point2dm>;

/*! \brief 3D multi-point with integer coordinates with measure. */
using MultiPoint3im = MultiPoint<Point3im>;

/*! \brief 3D multi-point with float coordinates with measure. */
using MultiPoint3fm = MultiPoint<Point3fm>;

/*! \brief 3D multi-point with double coordinates with measure. */
using MultiPoint3dm = MultiPoint<Point3dm>;

/*! \} */

} // End namespace tl
