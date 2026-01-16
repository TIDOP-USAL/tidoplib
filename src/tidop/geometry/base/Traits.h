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

#include "tidop/geometry/base/Dimension.h"
#include "tidop/math/base/Traits.h"
#include "tidop/math/base/data.h"

namespace tl
{

template<typename T, Dimension D> class Point;
template<typename Point_t> class Segment;
template<typename Point_t> class LineString;
template<typename Point_t> class LinearRing;
template<typename Point_t> class Polygon;
template<typename Point_t> class MultiPoint;
template<typename Point_t> class MultiLineString;
template<typename Point_t> class MultiPolygon;
template<typename Point_t> class BoundingBox;
template<typename T> class Circle;

/*! \addtogroup GeometricEntities
 *  \{
 */

enum class GeometryType
{
    point,
    segment,
    linestring,
    polygon,
    multipoint,
    multilinestring,
    multipolygon,
    shape,
    bbox
};


/* GEOMETRY TRAITS */

template<typename G>
struct geometry_traits
{
    static constexpr bool is_geometry = false;
    static constexpr Dimension dimension = Dimension::dynamic;
    static constexpr bool is_multi = false;
    static constexpr GeometryType type = GeometryType::shape;
    using value_type = void;
};

template<typename T, Dimension D>
struct geometry_traits<Point<T, D>>
{
    static_assert(is_valid_dimension(D), "Invalid point dimension");

    static constexpr bool is_geometry = true;
    static constexpr bool is_multi = false;
    static constexpr Dimension dimension = D;
    static constexpr GeometryType type = GeometryType::point;

    using value_type = T;
};

template<typename Point_t>
struct geometry_traits<Segment<Point_t>>
{
    static constexpr bool is_geometry = true;
    static constexpr bool is_multi = false;
    static constexpr Dimension dimension = geometry_traits<Point_t>::dimension;
    static constexpr GeometryType type = GeometryType::segment;

    using value_type = typename geometry_traits<Point_t>::value_type;
    using point_type = Point_t;
};

template<typename Point_t>
struct geometry_traits<LineString<Point_t>>
{
    static constexpr bool is_geometry = true;
    static constexpr bool is_multi = false;
    static constexpr Dimension dimension = geometry_traits<Point_t>::dimension;
    static constexpr GeometryType type = GeometryType::linestring;

    using value_type = typename geometry_traits<Point_t>::value_type;
    using point_type = Point_t;
};

template<typename Point_t>
struct geometry_traits<LinearRing<Point_t>>
{
    static constexpr bool is_geometry = false;
};

template<typename Point_t>
struct geometry_traits<Polygon<Point_t>>
{
    static constexpr bool is_geometry = true;
    static constexpr bool is_multi = false;
    static constexpr Dimension dimension = geometry_traits<Point_t>::dimension;
    static constexpr GeometryType type = GeometryType::polygon;

    using value_type = typename geometry_traits<Point_t>::value_type;
    using point_type = Point_t;
};

template<typename Point_t>
struct geometry_traits<MultiPoint<Point_t>>
{
    static constexpr bool is_geometry = true;
    static constexpr bool is_multi = true;
    static constexpr Dimension dimension = geometry_traits<Point_t>::dimension;
    static constexpr GeometryType type = GeometryType::multipoint;

    using value_type = typename geometry_traits<Point_t>::value_type;
    using point_type = Point_t;
};

template<typename Point_t>
struct geometry_traits<MultiLineString<Point_t>>
{
    static constexpr bool is_geometry = true;
    static constexpr bool is_multi = true;
    static constexpr Dimension dimension = geometry_traits<Point_t>::dimension;
    static constexpr GeometryType type = GeometryType::multilinestring;

    using value_type = typename geometry_traits<Point_t>::value_type;
    using point_type = Point_t;
};

template<typename Point_t>
struct geometry_traits<MultiPolygon<Point_t>>
{
    static constexpr bool is_geometry = true;
    static constexpr bool is_multi = true;
    static constexpr Dimension dimension = geometry_traits<Point_t>::dimension;
    static constexpr GeometryType type = GeometryType::multipolygon;

    using value_type = typename geometry_traits<Point_t>::value_type;
    using point_type = Point_t;
};

template<typename Point_t>
struct geometry_traits<BoundingBox<Point_t>>
{
    static constexpr bool is_geometry = true;
    static constexpr bool is_multi = false;
    static constexpr Dimension dimension = geometry_traits<Point_t>::dimension;
    static constexpr GeometryType type = GeometryType::bbox;

    using value_type = typename Point_t::value_type;
    using point_type = Point_t;
};


// Caso especial para vector
template<typename T, size_t S>
struct geometry_traits<Vector<T, S>>
{
    static constexpr bool is_geometry = true;
    static constexpr bool is_multi = false;

    static constexpr Dimension dimension = (S == DynamicData) ? Dimension::dynamic : static_cast<Dimension>(S);
    static constexpr GeometryType type = GeometryType::point;

    using value_type = T;
};

/* GEOMETRY TAGS */

struct point_tag {};
struct segment_tag {};
struct bbox_tag {};
struct linestring_tag {};
struct polygon_tag {};
struct multipoint_tag {};
struct multipolygon_tag {};
struct multilinestring_tag {};

template<GeometryType> struct geometry_tag;

template<> struct geometry_tag<GeometryType::point> { using type = point_tag; };
template<> struct geometry_tag<GeometryType::segment> { using type = segment_tag; };
template<> struct geometry_tag<GeometryType::bbox> { using type = bbox_tag; };
template<> struct geometry_tag<GeometryType::linestring> { using type = linestring_tag; };
template<> struct geometry_tag<GeometryType::polygon> { using type = polygon_tag; };
template<> struct geometry_tag<GeometryType::multipoint> { using type = multipoint_tag; };
template<> struct geometry_tag<GeometryType::multipolygon> { using type = multipolygon_tag; };
template<> struct geometry_tag<GeometryType::multilinestring> { using type = multilinestring_tag; };

template<typename G>
using geometry_tag_t = typename geometry_tag<geometry_traits<G>::type>::type;


/* HELPER ALIASES AND VARIABLES */

/*!
 * \brief Check if a type is a geometry.
 */
template<typename G>
inline constexpr bool is_geometry_v = geometry_traits<G>::is_geometry;

/*!
 * \brief Get the dimension of a geometry.
 */
template<typename G>
inline constexpr Dimension dimension_of_v = geometry_traits<G>::dimension;

/*!
 * \brief Get the value type (coordinate type) of a geometry.
 */
template<typename G>
using value_type_t = typename geometry_traits<G>::value_type;

/*!
 * \brief Get the geometry type (enum) of a geometry.
 */
template<typename G>
inline constexpr GeometryType geometry_type_v = geometry_traits<G>::type;

/*!
 * \brief Check if a geometry has a specific dimension.
 */
template<typename G, Dimension D>
inline constexpr bool has_dimension_v = (dimension_of_v<G> == D);

/*!
 * \brief Check if a geometry is 2D.
 */
template<typename G>
inline constexpr bool is_2d_v = has_dimension_v<G, Dimension::dim2>;

/*!
 * \brief Check if a geometry is 3D.
 */
template<typename G>
inline constexpr bool is_3d_v = has_dimension_v<G, Dimension::dim3>;

/*!
 * \brief Check if a geometry is 4D.
 */
template<typename G>
inline constexpr bool is_4d_v = has_dimension_v<G, Dimension::dim4>;



/* CONCEPTS (C++20) */

#if (TL_CPP_VERSION > 17)

/*!
 * \brief Concept for geometry types.
 */
template<typename G>
concept GeometryConcept = is_geometry_v<G>;

/*!
 * \brief Concept for 2D geometry types.
 */
template<typename G>
concept Geometry2DConcept = GeometryConcept<G> && is_2d_v<G>;

/*!
 * \brief Concept for 3D geometry types.
 */
template<typename G>
concept Geometry3DConcept = GeometryConcept<G> && is_3d_v<G>;

#endif

/*! \} */ 

template<typename T, Dimension D>
struct VectorTraits<Point<T, D>>
{
    using value_type = T;
    static constexpr std::size_t size = static_cast<std::size_t>(D);
    using result_type = Point<T, D>;
    using difference_type = Vector<T, static_cast<std::size_t>(D)>;
};

template<typename T, Dimension D>
struct is_point<Point<T, D>> : std::true_type {};

} // End namespace tl