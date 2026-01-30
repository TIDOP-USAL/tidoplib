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

/*! \file LineString.h
 * \brief Polyline (sequence of points) implementation.
 *
 * This file defines the LineString class template, which represents a polyline
 * as a sequence of points. It inherits from GeometryBase and GeometryContainer.
 * ### Classes
 * - \ref tl::LineString : Main template class for a polyline.
 * ### Type Aliases
 * - \ref tl::LineString2i, \ref tl::LineString2f, \ref tl::LineString2d : 2D integer, float, and double polylines.
 * - \ref tl::LineString3i, \ref tl::LineString3f, \ref tl::LineString3d : 3D integer, float, and double polylines.
 * \see tl::GeometryBase, tl::GeometryContainer, tl::Point
 */

#pragma once

#include <utility>

#include "tidop/geometry/base/Dimension.h"
#include "tidop/geometry/base/Geometry.h"
#include "tidop/geometry/base/Traits.h"
#include "tidop/geometry/base/GeometryContainer.h"
#include "tidop/geometry/primitives/Point.h"
#include "tidop/geometry/spatial/BoundingBox.h"
#include "tidop/geometry/algorithms/measurement/Length.h"

namespace tl
{
	
/*! \addtogroup Primitives
 *  \{
 */

/*!
 * \class LineString
 * \brief A polyline defined by a sequence of points.
 *
 * \tparam Point_t Type of the points in the polyline (e.g., Point2d, Point3f).
 */
template<typename Point_t>
class LineString 
  : public Geometry<LineString<Point_t>>,
    public GeometryContainer<Point_t>
{

public:

    /*! \brief Type of points stored in the polyline. */
    using value_type = Point_t;

public:

    using GeometryContainer<Point_t>::GeometryContainer;

    /*!
     * \brief Checks if the polyline is closed.
     *
     * A polyline is considered closed if the first and last points are equal.
     * \return true if the polyline is closed, false otherwise.
     */
    auto isClosed() const -> bool;

    /*!
     * \brief Computes the total length of the polyline.
     * \return The length of the polyline as a double.
     */
    auto length() const -> double;
};


// TYPE ALIASES

/*! \brief 2D polyline with integer coordinates. */
using LineString2i = LineString<Point2i>;

/*! \brief 2D polyline with float coordinates. */
using LineString2f = LineString<Point2f>;

/*! \brief 2D polyline with double coordinates. */
using LineString2d = LineString<Point2d>;

/*! \brief 3D polyline with integer coordinates. */
using LineString3i = LineString<Point3i>;

/*! \brief 3D polyline with float coordinates. */
using LineString3f = LineString<Point3f>;

/*! \brief 3D polyline with double coordinates. */
using LineString3d = LineString<Point3d>;

/*! \brief 4D polyline with integer coordinates. */
using LineString4i = LineString<Point4i>;

/*! \brief 4D polyline with float coordinates. */
using LineString4f = LineString<Point4f>;

/*! \brief 4D polyline with double coordinates. */
using LineString4d = LineString<Point4d>;

/*! \brief 2D polyline with integer coordinates and measure. */
using LineString2im = LineString<Point2im>;

/*! \brief 2D polyline with float coordinates and measure. */
using LineString2fm = LineString<Point2fm>;

/*! \brief 2D polyline with double coordinates and measure. */
using LineString2dm = LineString<Point2dm>;

/*! \brief 3D polyline with integer coordinates and measure. */
using LineString3im = LineString<Point3im>;

/*! \brief 3D polyline with float coordinates and measure. */
using LineString3fm = LineString<Point3fm>;

/*! \brief 3D polyline with double coordinates and measure. */
using LineString3dm = LineString<Point3dm>;


// METHOD IMPLEMENTATIONS

template<typename Point_t>
auto LineString<Point_t>::isClosed() const -> bool
{
    if (this->size() < 2) return false;
    return this->front() == this->back();
}

template<typename Point_t>
auto LineString<Point_t>::length() const -> double
{
    return tl::length(*this);
}


/*! \} */

} // End namespace tl
