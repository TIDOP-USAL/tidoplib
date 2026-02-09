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

/*!
 * \file Intersects.h
 * \brief Intersects algorithms for geometric predicates.
 */

#pragma once

#include "tidop/geometry/base/Traits.h"
#include "tidop/geometry/primitives/Point.h"
#include "tidop/geometry/primitives/Segment.h"
#include "tidop/geometry/primitives/LineString.h"
#include "tidop/geometry/Operations.h"

#include <ranges>
#include <variant>

namespace tl
{

/*! \addtogroup SpatialPredicates
 *  \{
 */

/*!
 * \brief Checks if two geometries intersect (share any point in common).
 * 
 * This function implements the OGC Simple Features `ST_Intersects` predicate.
 * Two geometries intersect if they have at least one point in common,
 * whether in their interiors or on their boundaries.
 * 
 * \tparam G1 Type of the first geometry. Must satisfy the GeometryConcept.
 * \tparam G2 Type of the second geometry. Must satisfy the GeometryConcept.
 * 
 * \param[in] g1 First geometry.
 * \param[in] g2 Second geometry.
 * 
 * \return `true` if `g1` and `g2` intersect, `false` otherwise.
 * 
 * \note This is a symmetric predicate: `intersects(a, b) == intersects(b, a)`.
 * \note For empty geometries, always returns `false`.
 * 
 * ### Examples:
 * \code{.cpp}
 * // Basic point-line intersection
 * Point2d p(5.0, 5.0);
 * Segment2d seg(Point2d(0.0, 0.0), Point2d(10.0, 10.0));
 * bool b1 = intersects(p, seg);  // true - point lies on segment
 * 
 * // Line-polygon intersection
 * LineString2d line{Point2d(0.0, 5.0), Point2d(10.0, 5.0)};
 * Polygon2d poly{{Point2d(0.0, 0.0), Point2d(10.0, 0.0), Point2d(10.0, 10.0), Point2d(0.0, 10.0)}};
 * bool b2 = intersects(line, poly);  // true - line crosses polygon
 * 
 * // Polygon-polygon intersection (touching)
 * Polygon2d poly1{{Point2d(0.0, 0.0), Point2d(5.0, 0.0), Point2d(5.0, 5.0), Point2d(0.0, 5.0)}};
 * Polygon2d poly2{{Point2d(5.0, 0.0), Point2d(10.0, 0.0), Point2d(10.0, 5.0), Point2d(5.0, 5.0)}};
 * bool b3 = intersects(poly1, poly2);  // true - share boundary point
 * 
 * // No intersection
 * Point2d p1(1.0, 1.0);
 * Point2d p2(2.0, 2.0);
 * bool b4 = intersects(p1, p2);  // false - distinct points
 * 
 * // Multi-geometry intersection
 * MultiPoint2d mp{Point2d(1.0, 1.0), Point2d(2.0, 2.0), Point2d(3.0, 3.0)};
 * LineString2d line2{Point2d(0.0, 0.0), Point2d(4.0, 4.0)};
 * bool b5 = intersects(mp, line2);  // true - line passes through points
 * \endcode
 * 
 * ### Supported Geometry Combinations:
 * 
 * The following geometry type combinations are supported:
 * | Type 1 | Type 2 | Description |
 * |--------|--------|-------------|
 * | Point | Point | Equality test with tolerance |
 * | Point | Segment | Point on segment test |
 * | Point | LineString | Point on any segment |
 * | Point | Polygon | Point in interior or on boundary |
 * | Segment | Segment | Segment intersection (2D) |
 * | Segment | LineString | Any segment intersection |
 * | Segment | Polygon | Segment in interior or crossing boundary |
 * | LineString | LineString | Any segment intersection |
 * | LineString | Polygon | Any point inside or segment crossing |
 * | Polygon | Polygon | Interior overlap or boundary contact |
 * | *Multi* types | Any | Element-wise intersection |
 * | GeometryCollection | Any | Element-wise intersection |
 * | BoundingBox | Any | Fast bounding box intersection |
 * 
 */
template<typename G1, typename G2>
auto intersects(const G1 &g1, const G2 &g2) -> bool;


template<typename Point_t>
auto intersects(const Segment<Point_t> &seg, const LinearRing<Point_t> &ring)
{

    if (ring.size() < 2) return false;

    // 1️ - Chequear si alguno de los extremos está en el interior o en el borde
    if (locatePointInRing(ring, seg.pt1()) != Location::Exterior ||
        locatePointInRing(ring, seg.pt2()) != Location::Exterior) {
        return true;
    }

    // 2️ - Revisar cada arista del anillo
    for (size_t i = 0; i < ring.size(); ++i) {
        size_t j = (i + 1) % ring.size();
        Segment<Point_t> edge(ring[i], ring[j]);

        if (intersects(seg, edge)) {
            return true;
        }
    }

    return false;
}

/*! \} */

} // End namespace tl

#include "Intersects.impl.h"
