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

/*! \file Traits.h
 * \brief Geometry type traits, tags, and compile-time utilities.
 *
 * This file defines compile-time type traits for geometric entities, including
 * geometry type identification, dimension queries, and value type extraction.
 * It also provides tags for tag dispatching and C++20 concepts for geometric types.
 * ### Enumerations
 * - \ref tl::GeometryType : Enumeration of all supported geometry types.
 * ### Classes
 * - \ref tl::geometry_traits : Primary template for geometry traits.
 * - \ref tl::geometry_tag : Mapping from GeometryType to tag types.
 * ### Type Aliases
 * - \ref tl::geometry_tag_t : Helper alias to get the tag for a geometry type.
 * ### Variables
 * - \ref tl::is_geometry_v : Checks if a type is a geometry.
 * - \ref tl::dimension_of_v : Gets the dimension of a geometry.
 * - \ref tl::geometry_type_v : Gets the geometry type enum.
 * - \ref tl::is_2d_v, \ref tl::is_3d_v, \ref tl::is_4d_v : Dimension checks.
 * ### Concepts (C++20)
 * - \ref tl::GeometryConcept : Concept for geometry types.
 * - \ref tl::Geometry2DConcept : Concept for 2D geometry types.
 * - \ref tl::Geometry3DConcept : Concept for 3D geometry types.
 */

#pragma once

#include "tidop/config.h"

#include "tidop/core/base/Concepts.h"
#include "tidop/geometry/base/Dimension.h"
#include "tidop/math/base/Traits.h"
#include "tidop/math/base/data.h"

namespace tl
{

// Forward declarations of geometry types
template<typename T, typename Tag> class Point;
template<typename Point_t> class Segment;
template<typename Point_t> class LineString;
template<typename Point_t> class LinearRing;
template<typename Point_t> class Polygon;
template<typename Point_t> class MultiPoint;
template<typename Point_t> class MultiLineString;
template<typename Point_t> class MultiPolygon;
template<typename Point_t> class GeometryCollection;
template<typename Point_t> class BoundingBox;
template<typename Point_t> class SegmentView;

/*! \addtogroup Geometry
 *  \{
 */

/*!
 * \enum GeometryType
 * \brief Enumeration of all supported geometry types.
 */
enum class GeometryType
{
    point,              /*!< Point geometry. */
    segment,            /*!< Line segment. */
    linestring,         /*!< Polyline (sequence of points). */
    polygon,            /*!< Polygon with outer ring and optional holes. */
    multipoint,         /*!< Collection of points. */
    multilinestring,    /*!< Collection of polylines. */
    multipolygon,       /*!< Collection of polygons. */
    collection,         /*!< Geometry collection. */
    //shape,              /*!< Generic shape type. */
    bbox                /*!< Bounding box. */
};


/* Point type tags */

struct xy_tag
{ 
    static constexpr bool is_ogc = true;
    static constexpr size_t spatial_dims = 2; 
    static constexpr size_t storage_size = 2; 
};

struct xyz_tag
{ 
    static constexpr bool is_ogc = true;
    static constexpr size_t spatial_dims = 3; 
    static constexpr size_t storage_size = 3;
};

struct xym_tag 
{ 
    static constexpr bool is_ogc = true;
    static constexpr size_t spatial_dims = 2; 
    static constexpr size_t storage_size = 3;
};

struct xyzm_tag
{
    static constexpr bool is_ogc = true;
    static constexpr size_t spatial_dims = 3; 
    static constexpr size_t storage_size = 4;
};

struct xyzw_tag 
{ 
    static constexpr bool is_ogc = false;
    static constexpr size_t spatial_dims = 4;
    static constexpr size_t storage_size = 4;
};


/* Geometry tags */

struct point_tag {};
struct segment_tag {};
struct bbox_tag {};
struct linestring_tag {};
struct polygon_tag {};
struct multipoint_tag {};
struct multipolygon_tag {};
struct multilinestring_tag {};
struct collection_tag {};


/* Point Traits */

template<typename T>
struct point_traits
{
    static_assert(sizeof(T) == 0, "point_traits<T>: T is not a valid Point type");
};

template<typename T, typename Tag>
struct point_traits<Point<T, Tag>>
{
    using value_type = T;
    using tag_type = Tag;
    using calculation_type = std::conditional_t<std::is_integral_v<value_type>,
                                                double,
                                                value_type>;

    static constexpr size_t spatial_dims = Tag::spatial_dims;
    static constexpr size_t storage_size = Tag::storage_size;
    static constexpr bool has_m = Tag::storage_size > Tag::spatial_dims;
    static constexpr Dimension dimension = Tag::spatial_dims == 2 ? Dimension::dim2 :
                                           Tag::spatial_dims == 3 ? Dimension::dim3 :
                                           Dimension::dim4;
};


/* Geometry Traits */

/*!
 * \struct geometry_traits
 * \brief Primary template for geometry type traits.
 *
 * \tparam G Geometry type.
 * Specializations define properties for each geometry type.
 */
template<typename G>
struct geometry_traits
{
    static constexpr bool is_geometry = false;
    static constexpr bool is_multi = false;
    static constexpr Dimension dimension = Dimension::dynamic;
    static constexpr GeometryType type = GeometryType::point;
};

/*!
 * \brief Traits specialization for Point.
 * \tparam T Data type for the coordinates (e.g., float, double, int).
 * \tparam Tag Type tag defining the coordinate layout (e.g., xy_tag, xyz_tag, xym_tag, xyzm_tag).
 */
template<typename T, typename Tag>
struct geometry_traits<Point<T, Tag>>
{
    static constexpr bool is_geometry = true;
    static constexpr bool is_multi = false;
    static constexpr GeometryType type = GeometryType::point;
    static constexpr Dimension dimension = point_traits<Point<T, Tag>>::dimension;
    static constexpr bool has_m = point_traits<Point<T, Tag>>::has_m;
    using geometry_tag = point_tag;
    using point_type = Point<T, Tag>;
};

template<GeometryType geom_type, typename GeomTag_t, typename Point_t, bool is_multi = false>
struct geometry_from_point_traits
{
    static constexpr bool is_geometry = true;
    static constexpr bool is_multi = is_multi;
    static constexpr GeometryType type = geom_type;
    static constexpr Dimension dimension = point_traits<Point_t>::dimension;
    static constexpr bool has_m = point_traits<Point_t>::has_m;

    using geometry_tag = GeomTag_t;
    using point_type = Point_t;
};

/*!
 * \brief Traits specialization for Segment.
 * \tparam Point_t Point type.
 */
template<typename Point_t>
struct geometry_traits<Segment<Point_t>>
  : geometry_from_point_traits<GeometryType::segment, segment_tag, Point_t>
{
};

template<typename Point_t>
struct geometry_traits<SegmentView<Point_t>> 
{
    static constexpr bool is_geometry = true;
    static constexpr bool is_multi = false;
    static constexpr GeometryType type = GeometryType::segment;
    static constexpr Dimension dimension = point_traits<Point_t>::dimension;
    static constexpr bool has_m = point_traits<Point_t>::has_m;

    using geometry_tag = segment_tag;
    using point_type = Point_t;
};

/*!
 * \brief Traits specialization for LineString.
 * \tparam Point_t Point type.
 */
template<typename Point_t>
struct geometry_traits<LineString<Point_t>>
  : geometry_from_point_traits<GeometryType::linestring, 
                               linestring_tag,
                               Point_t>
{
};

/*!
 * \brief Traits specialization for LinearRing.
 * \tparam Point_t Point type.
 * \note LinearRing is not considered a standalone geometry type.
 */
template<typename Point_t>
struct geometry_traits<LinearRing<Point_t>>
{
    static constexpr bool is_geometry = false;
    static constexpr bool is_multi = false;
    static constexpr Dimension dimension = point_traits<Point_t>::dimension;
    static constexpr bool has_m = point_traits<Point_t>::has_m;
    using geometry_tag = linestring_tag;
    using point_type = Point_t;
};

/*!
 * \brief Traits specialization for Polygon.
 * \tparam Point_t Point type.
 */
template<typename Point_t>
struct geometry_traits<Polygon<Point_t>>
    : geometry_from_point_traits<GeometryType::polygon, polygon_tag, Point_t>
{
};

/*!
 * \brief Traits specialization for MultiPoint.
 * \tparam Point_t Point type.
 */
template<typename Point_t>
struct geometry_traits<MultiPoint<Point_t>>
    : geometry_from_point_traits<GeometryType::multipoint, multipoint_tag, Point_t, true>
{
};

/*!
 * \brief Traits specialization for MultiLineString.
 * \tparam Point_t Point type.
 */
template<typename Point_t>
struct geometry_traits<MultiLineString<Point_t>>
    : geometry_from_point_traits<GeometryType::multilinestring, multilinestring_tag, Point_t, true>
{
};


/*!
 * \brief Traits specialization for MultiPolygon.
 * \tparam Point_t Point type.
 */
template<typename Point_t>
struct geometry_traits<MultiPolygon<Point_t>>
    : geometry_from_point_traits<GeometryType::multipolygon, multipolygon_tag, Point_t, true>
{
};

/*!
 * \brief Traits specialization for GeometryCollection.
 * \tparam Point_t Point type.
 */
template<typename Point_t>
struct geometry_traits<GeometryCollection<Point_t>>
    : geometry_from_point_traits<GeometryType::collection, collection_tag, Point_t, true>
{
};

/*!
 * \brief Traits specialization for BoundingBox.
 * \tparam Point_t Point type.
 */
template<typename Point_t>
struct geometry_traits<BoundingBox<Point_t>>
    : geometry_from_point_traits<GeometryType::bbox, bbox_tag, Point_t>
{
};

/*!
 * \brief Traits specialization for Vector (treated as Point).
 * \tparam T Coordinate type.
 * \tparam S Vector size (or DynamicData).
 */
template<typename T, size_t S>
struct geometry_traits<Vector<T, S>>
{
    static constexpr bool is_geometry = true;
    static constexpr bool is_multi = false;
    static constexpr Dimension dimension = (S == DynamicData) ? Dimension::dynamic : static_cast<Dimension>(S);
    static constexpr bool has_m = false;
    static constexpr GeometryType type = GeometryType::point;
    using geometry_tag = point_tag;
    using point_type = Vector<T, S>;
};


/*!
 * \brief Helper alias to get the tag type for a geometry.
 * \tparam G Geometry type.
 */
template<typename G>
using geometry_tag_t = typename geometry_traits<G>::geometry_tag;

/* HELPER ALIASES AND VARIABLES */

/*!
 * \brief Variable template to check if a type is a geometry.
 * \tparam G Type to check.
 */
template<typename G>
inline constexpr bool is_geometry_v = geometry_traits<G>::is_geometry;

/*!
 * \brief Variable template to get the dimension of a geometry.
 * \tparam G Geometry type.
 */
template<typename G>
inline constexpr Dimension dimension_of_v = geometry_traits<G>::dimension;

/*!
 * \brief Alias template to get the value type (coordinate type) of a geometry.
 * \tparam G Geometry type.
 */
template<typename G>
using value_type_t = typename geometry_traits<G>::value_type;

/*!
 * \brief Variable template to get the geometry type enum of a geometry.
 * \tparam G Geometry type.
 */
template<typename G>
inline constexpr GeometryType geometry_type_v = geometry_traits<G>::type;

/*!
 * \brief Variable template to check if a geometry has a specific dimension.
 * \tparam G Geometry type.
 * \tparam D Dimension to check.
 */
template<typename G, Dimension D>
inline constexpr bool has_dimension_v = (dimension_of_v<G> == D);

template<typename G>
inline constexpr bool has_m_v = geometry_traits<G>::has_m;

/*!
 * \brief Variable template to check if a geometry is 2D.
 * \tparam G Geometry type.
 */
template<typename G>
inline constexpr bool is_2d_v = has_dimension_v<G, Dimension::dim2>;

/*!
 * \brief Variable template to check if a geometry is 3D.
 * \tparam G Geometry type.
 */
template<typename G>
inline constexpr bool is_3d_v = has_dimension_v<G, Dimension::dim3>;

/*!
 * \brief Variable template to check if a geometry is 4D.
 * \tparam G Geometry type.
 */
template<typename G>
inline constexpr bool is_4d_v = has_dimension_v<G, Dimension::dim4>;

/*!
 * \brief Variable template for multi-geometry check.
 * \tparam G Type to check.
 */
template<typename G>
inline constexpr bool is_multi_geometry_v = geometry_traits<G>::is_multi;

template<typename G>
inline constexpr bool is_geometry_collection_v = std::is_same_v<geometry_traits<G>::geometry_tag, collection_tag>;

template<typename P>
struct remove_measure;

template<typename T, typename Tag>
struct remove_measure<Point<T, Tag>> 
{

private:

    static constexpr size_t spatial_dims = Tag::spatial_dims;

    template<size_t Dim>
    struct tag_for_dim;

    template<>
    struct tag_for_dim<2> { using type = xy_tag; };

    template<>
    struct tag_for_dim<3> { using type = xyz_tag; };

    template<>
    struct tag_for_dim<4> { using type = xyzw_tag; };

public:
    using type = Point<T, typename tag_for_dim<spatial_dims>::type>;
};

template<typename P>
using remove_measure_t = typename remove_measure<P>::type;

// Helper para tipo común sin medida
template<typename P1, typename P2>
struct common_point_without_measure 
{
    static_assert(SameSpatialDimension<P1, P2>,
        "Points must have same spatial dimension");

    using value_type = std::common_type_t<typename point_traits<P1>::value_type,
                                          typename point_traits<P2>::value_type>;

    static constexpr size_t spatial_dims = point_traits<P1>::spatial_dims;

    template<size_t Dim>
    struct tag_for_dim;

    template<>
    struct tag_for_dim<2> { using type = xy_tag; };

    template<>
    struct tag_for_dim<3> { using type = xyz_tag; };

    template<>
    struct tag_for_dim<4> { using type = xyzw_tag; };

    using type = Point<value_type, typename tag_for_dim<spatial_dims>::type>;
};

template<typename P1, typename P2>
using common_point_without_measure_t = typename common_point_without_measure<P1, P2>::type;


template<typename T>
constexpr auto epsilon_for() -> T
{
    if constexpr (std::is_floating_point_v<T>) {
        return std::numeric_limits<T>::epsilon() * 100;
    } else {
        return T{0};
    }
}


/*! \} */ 

template<typename T, typename Tag>
struct VectorTraits<Point<T, Tag>>
{
    using value_type = T;
    static constexpr size_t size = point_traits<Point<T, Tag>>::spatial_dims;
    using result_type = Point<T, Tag>;
    using difference_type = Vector<T, size>;
};

template<typename T, typename Tag>
struct is_point<Point<T, Tag>> : std::true_type {};


template<typename OldTag, size_t Rows>
struct rebind_point_tag;

// XY -> 2D sin medida
template<> struct rebind_point_tag<xy_tag, 2> { using type = xy_tag; };
template<> struct rebind_point_tag<xy_tag, 3> { using type = xyz_tag; };
template<> struct rebind_point_tag<xy_tag, 4> { using type = xyzw_tag; };

// XYZ
template<> struct rebind_point_tag<xyz_tag, 2> { using type = xy_tag; };
template<> struct rebind_point_tag<xyz_tag, 3> { using type = xyz_tag; };
template<> struct rebind_point_tag<xyz_tag, 4> { using type = xyzw_tag; };

// XYM
template<> struct rebind_point_tag<xym_tag, 2> { using type = xy_tag; };
template<> struct rebind_point_tag<xym_tag, 3> { using type = xyz_tag; };
template<> struct rebind_point_tag<xym_tag, 4> { using type = xyzw_tag; };

// XYZM
template<> struct rebind_point_tag<xyzm_tag, 2> { using type = xy_tag; };
template<> struct rebind_point_tag<xyzm_tag, 3> { using type = xyz_tag; };
template<> struct rebind_point_tag<xyzm_tag, 4> { using type = xyzw_tag; };

// XYZW
template<> struct rebind_point_tag<xyzw_tag, 2> { using type = xy_tag; };
template<> struct rebind_point_tag<xyzw_tag, 3> { using type = xyz_tag; };
template<> struct rebind_point_tag<xyzw_tag, 4> { using type = xyzw_tag; };



} // End namespace tl