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


#pragma once

#include "tidop/config.h"

#include "tidop/geometry/base/Traits.h"

namespace tl
{


/* CONCEPTS (C++20) */

/*!
 * \brief Concept for geometry types.
 * \tparam G Type to test.
 */
template<typename G>
concept GeometryConcept = is_geometry_v<G>;

/*!
 * \brief Concept for 2D geometry types.
 * \tparam G Type to test.
 */
template<typename G>
concept Geometry2DConcept = GeometryConcept<G> && is_2d_v<G>;

/*!
 * \brief Concept for 3D geometry types.
 * \tparam G Type to test.
 */
template<typename G>
concept Geometry3DConcept = GeometryConcept<G> && is_3d_v<G>;

/*!
 * \brief Concept for 4D geometry types.
 * \tparam G Type to test.
 */
template<typename G>
concept Geometry4DConcept = GeometryConcept<G> && is_4d_v<G>;

/*!
 * \brief Concept for simple-geometry types.
 * \tparam G Type to test.
 */
template<typename G>
concept SimpleGeometryConcept = GeometryConcept<G> && !is_multi_geometry_v<G>;

/*!
 * \brief Concept for multi-geometry types.
 * \tparam G Type to test.
 */
template<typename G>
concept MultiGeometryConcept = GeometryConcept<G> && is_multi_geometry_v<G>;


template<typename P>
concept PointConcept = GeometryConcept<P> &&
                       geometry_type_v<P> == GeometryType::point &&
                       std::regular<P>;

template<typename P>
concept Point2DConcept = Geometry2DConcept<P> &&
                         geometry_type_v<P> == GeometryType::point &&
                         std::regular<P>;

template<typename P>
concept Point3DConcept = Geometry3DConcept<P> &&
                         geometry_type_v<P> == GeometryType::point &&
                         std::regular<P>;

template<typename G>
concept SegmentConcept = GeometryConcept<G> &&
                         geometry_type_v<G> == GeometryType::segment;

template<typename G>
concept Segment2DConcept = Geometry2DConcept<G> &&
                           geometry_type_v<G> == GeometryType::segment;

template<typename G>
concept Segment3DConcept = Geometry3DConcept<G> &&
                           geometry_type_v<G> == GeometryType::segment;

template<typename G>
concept LineStringConcept = GeometryConcept<G> &&
                            geometry_type_v<G> == GeometryType::linestring;

template<typename G>
concept LineString2DConcept = Geometry2DConcept<G> &&
                              geometry_type_v<G> == GeometryType::linestring;

template<typename G>
concept LineString3DConcept = Geometry3DConcept<G> &&
                              geometry_type_v<G> == GeometryType::linestring;

template<typename G>
concept PolygonConcept = GeometryConcept<G> &&
                         geometry_type_v<G> == GeometryType::polygon;

template<typename G>
concept Polygon2DConcept = Geometry2DConcept<G> &&
                           geometry_type_v<G> == GeometryType::polygon;

template<typename G>
concept Polygon3DConcept = Geometry3DConcept<G> &&
                           geometry_type_v<G> == GeometryType::polygon;


template<typename G>
concept GeometryCollectionConcept = GeometryConcept<G> && is_geometry_collection_v<G>;

template<typename G>
concept GeometryCollection2DConcept = Geometry2DConcept<G> && is_geometry_collection_v<G>;

template<typename G>
concept GeometryCollection3DConcept = Geometry3DConcept<G> && is_geometry_collection_v<G>;


template<typename G>
concept LinearContainerConcept = requires(G g)
{
    typename G::value_type;
    { g.size() } -> std::convertible_to<std::size_t>;
    { g[0] } -> std::same_as<typename G::value_type &>;
};

template<typename G>
concept LinearGeometryConcept = LinearContainerConcept<G>;

template<typename G>
concept LinearGeometry2DConcept = LinearContainerConcept<G> && 
                                  is_2d_v<typename G::value_type>;

template<typename G>
concept LinearGeometry3DConcept = LinearContainerConcept<G> && 
                                  is_3d_v<typename G::value_type>;

template<typename G1, typename G2>
concept SameSpatialDimension = GeometryConcept<G1> && 
                               GeometryConcept<G2> &&
                               (dimension_of_v<G1> == dimension_of_v<G2>);

template<typename G1, typename G2>
concept CompatibleGeometries = SameSpatialDimension<G1, G2> &&
                               ((has_m_v<G1> == has_m_v<G2>) || !has_m_v<G1> || !has_m_v<G2>);


template<typename Policy>
concept PrecisionPolicyConcept = requires(Policy p, typename Policy::scalar_type v)
{
    typename Policy::scalar_type;   // existe
    typename Policy::calc_type;     // existe
    { p.snap(v) } -> std::convertible_to<typename Policy::calc_type>;
};


/*! \} */ 

} // End namespace tl