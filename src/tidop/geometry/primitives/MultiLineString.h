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

/*! \file MultiLineString.h
 * \brief Collection of LineString objects.
 *
 * This file defines the MultiLineString class template, which represents a
 * collection of LineString objects. It inherits from GeometryBase and GeometryContainer.
 * ### Classes
 * - \ref tl::MultiLineString : Main template class for a collection of polylines.
 * ### Type Aliases
 * - \ref tl::MultiLineString2i, \ref tl::MultiLineString2f, \ref tl::MultiLineString2d : 2D integer, float, and double multi-polylines.
 * - \ref tl::MultiLineString3i, \ref tl::MultiLineString3f, \ref tl::MultiLineString3d : 3D integer, float, and double multi-polylines.
 * \see tl::GeometryBase, tl::GeometryContainer, tl::LineString
 */

#pragma once

#include "tidop/core/base/type_conversions.h"
#include "tidop/geometry/base/Dimension.h"
#include "tidop/geometry/base/Geometry.h"
#include "tidop/geometry/base/Traits.h"
#include "tidop/geometry/primitives/Point.h"
#include "tidop/geometry/primitives/LineString.h"
#include "tidop/math/algebra/vector.h"

#include <array>

namespace tl
{
	
/*! \addtogroup Primitives
 *  \{
 */

/*!
 * \class MultiLineString
 * \brief A collection of LineString objects.
 *
 * \tparam Point_t Type of the points in the LineStrings (e.g., Point2d, Point3f).
 */
template<typename Point_t>
class MultiLineString 
  : public Geometry<MultiLineString<Point_t>>, 
    public GeometryContainer<LineString<Point_t>> 
{
    using GeometryContainer<LineString<Point_t>>::GeometryContainer;
};

// TYPE ALIASES

/*! \brief 2D multi-polyline with integer coordinates. */
using MultiLineString2i = MultiLineString<Point2i>;

/*! \brief 3D multi-polyline with integer coordinates. */
using MultiLineString3i = MultiLineString<Point3i>;

/*! \brief 4D multi-polyline with integer coordinates. */
using MultiLineString4i = MultiLineString<Point4i>;

/*! \brief 2D multi-polyline with float coordinates. */
using MultiLineString2f = MultiLineString<Point2f>;

/*! \brief 3D multi-polyline with float coordinates. */
using MultiLineString3f = MultiLineString<Point3f>;

/*! \brief 4D multi-polyline with float coordinates. */
using MultiLineString4f = MultiLineString<Point4f>;

/*! \brief 2D multi-polyline with double coordinates. */
using MultiLineString2d = MultiLineString<Point2d>;

/*! \brief 3D multi-polyline with double coordinates. */
using MultiLineString3d = MultiLineString<Point3d>;

/*! \brief 4D multi-polyline with double coordinates. */
using MultiLineString4d = MultiLineString<Point4d>;

/*! \brief 2D multi-polyline with integer coordinates with measure. */
using MultiLineString2im = MultiLineString<Point2im>;

/*! \brief 2D multi-polyline with float coordinates with measure. */
using MultiLineString2fm = MultiLineString<Point2fm>;

/*! \brief 2D multi-polyline with double coordinates with measure. */
using MultiLineString2dm = MultiLineString<Point2dm>;

/*! \brief 3D multi-polyline with integer coordinates with measure. */
using MultiLineString3im = MultiLineString<Point3im>;

/*! \brief 3D multi-polyline with float coordinates with measure. */
using MultiLineString3fm = MultiLineString<Point3fm>;

/*! \brief 3D multi-polyline with double coordinates with measure. */
using MultiLineString3dm = MultiLineString<Point3dm>;


/*! \} */

} // End namespace tl
