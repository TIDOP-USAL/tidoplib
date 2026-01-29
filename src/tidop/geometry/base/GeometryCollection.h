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

/*! \file GeometryCollection.hpp
 * \brief Heterogeneous collection of geometries following OGC specification.
 *
 * This file defines the GeometryCollection class template, which serves as a
 * container for geometries of different types (Point, LineString, Polygon, etc.)
 * while preserving insertion order. It supports OGC GeometryCollection semantics
 * including nested collections.
 *
 * The collection maintains separate storage for each geometry type (SoA-like)
 * and an index (mOrder) to preserve insertion order and provide O(1) access
 * by position. This design balances type safety, memory locality, and fast iteration.
 *
 * ### Classes
 * - \ref tl::GeometryCollection : Main template class for heterogeneous geometry collections.
 *
 * ### Type Aliases
 * - \ref tl::GeometryReference : Variant of non-const geometry references.
 * - \ref tl::ConstGeometryReference : Variant of const geometry references.
 *
 * ### Key Features
 * - **OGC Compliance**: Follows Open Geospatial Consortium GeometryCollection specification.
 * - **Type Safety**: Uses std::variant for type-safe heterogeneous storage.
 * - **Order Preservation**: Maintains insertion order via mOrder index.
 * - **Efficient Access**: O(1) access by position, O(1) addition.
 * - **Memory Efficiency**: SoA-like storage per geometry type.
 * - **Nested Collections**: Supports recursive GeometryCollection storage.
 *
 * \see tl::Geometry, tl::Point, tl::LineString, tl::Polygon,
 *      tl::MultiPoint, tl::MultiLineString, tl::MultiPolygon
 */

#pragma once

#include "tidop/config.h"

#include "tidop/geometry/base/Geometry.h"
#include "tidop/geometry/base/Traits.h"
#include "tidop/geometry/primitives/Point.h"
#include "tidop/geometry/primitives/LineString.h"
#include "tidop/geometry/primitives/Polygon.h"
#include "tidop/geometry/primitives/MultiPoint.h"
#include "tidop/geometry/primitives/MultiLineString.h"
#include "tidop/geometry/primitives/MultiPolygon.h"

#include <vector>
#include <variant>

namespace tl
{

/*! \addtogroup Geometry
 *  \{
 */

/*!
 * \class GeometryCollection
 * \brief Heterogeneous collection of geometries following OGC specification.
 *
 * This class implements a GeometryCollection that can store geometries of
 * different types while preserving insertion order. It supports all OGC
 * geometry types: Point, LineString, Polygon, MultiPoint, MultiLineString,
 * MultiPolygon, and nested GeometryCollections.
 *
 * The collection uses a Structure-of-Arrays (SoA) storage pattern where
 * each geometry type is stored in a separate vector. This provides better
 * memory locality when processing geometries by type. An additional index
 * (mOrder) maintains the insertion order for sequential access.
 *
 * \tparam Point_t The point type used by all geometries in the collection.
 *                 Must satisfy the Point concept (provide x(), y() methods).
 *
 * ### Usage Example:
 * \code{.cpp}
 *
 * GeometryCollection<Point2d> collection;
 * collection.addPoint(Point2d(1.0, 2.0));
 * collection.addLineString(LineString<Point2d>({Point2d(0,0), Point2d(1,1)}));
 *
 * // Access using std::visit with compile-time type checking
 * for (size_t i = 0; i < collection.size(); ++i) {
 *     std::visit([](auto&& geom_ref) {
 *         using GeomType = std::decay_t<decltype(geom_ref.get())>;
 *
 *         if constexpr (std::is_same_v<GeomType, Point2d>) {
 *             std::cout << "Point: " << geom_ref.get().x() << ", "
 *                       << geom_ref.get().y() << std::endl;
 *         }
 *         else if constexpr (std::is_same_v<GeomType, LineString<Point2d>>) {
 *             std::cout << "LineString with "
 *                       << geom_ref.get().size() << " points" << std::endl;
 *         }
 *         // Handle other geometry types...
 *     }, collection[i]);
 * }
 * \endcode
 *
 * \see OGC Simple Feature Access Specification (06-103r4)
 * \see ISO/IEC 13249-3:2016 (SQL/MM Spatial)
 */
template<typename Point_t>
class GeometryCollection 
  : public Geometry<GeometryCollection<Point_t>>
{

private:

    std::vector<Point_t> mPoints;                                    /*!< Storage for Point geometries. */
    std::vector<LineString<Point_t>> mLineString;                    /*!< Storage for LineString geometries. */
    std::vector<Polygon<Point_t>> mPolygons;                         /*!< Storage for Polygon geometries. */
    std::vector<MultiPoint<Point_t>> mMultiPoints;                   /*!< Storage for MultiPoint geometries. */
    std::vector<MultiLineString<Point_t>> mMultiLineStrings;         /*!< Storage for MultiLineString geometries. */
    std::vector<MultiPolygon<Point_t>> mMultiPolygons;               /*!< Storage for MultiPolygon geometries. */
    std::vector<GeometryCollection<Point_t>> mCollections;           /*!< Storage for nested GeometryCollections. */

    /*!
     * \brief Internal identifier for geometries in a collection.
     *
     * This structure associates a geometry type with its index in the
     * corresponding type-specific vector. It's used to maintain insertion
     * order while allowing efficient per-type storage.
     *
     * \note This is an internal implementation detail and not part of the public API.
     */
    struct GeoID
    {
        GeometryType type;  /*!< Type of the geometry. */
        uint32_t index;     /*!< Index in the type-specific vector. */
    };

    std::vector<GeoID> mOrder; /*!< Index preserving insertion order and type mapping. */

public:

    /*!
     * \brief Variant type for non-const geometry references.
     *
     * This variant can hold a reference to any of the seven geometry types
     * supported by the collection. Use std::visit to process the reference
     * with compile-time type checking.
     */
    using GeometryReference = std::variant<std::reference_wrapper<Point_t>,
                                           std::reference_wrapper<LineString<Point_t>>,
                                           std::reference_wrapper<Polygon<Point_t>>,
                                           std::reference_wrapper<MultiPoint<Point_t>>,
                                           std::reference_wrapper<MultiLineString<Point_t>>,
                                           std::reference_wrapper<MultiPolygon<Point_t>>,
                                           std::reference_wrapper<GeometryCollection<Point_t>>>;

    /*!
     * \brief Variant type for const geometry references.
     *
     * Const version of GeometryReference for read-only access to collection
     * elements. Useful when working with const GeometryCollection instances.
     */
    using ConstGeometryReference = std::variant<std::reference_wrapper<const Point_t>,
                                                std::reference_wrapper<const LineString<Point_t>>,
                                                std::reference_wrapper<const Polygon<Point_t>>,
                                                std::reference_wrapper<const MultiPoint<Point_t>>,
                                                std::reference_wrapper<const MultiLineString<Point_t>>,
                                                std::reference_wrapper<const MultiPolygon<Point_t>>,
                                                std::reference_wrapper<const GeometryCollection<Point_t>>>;
public:

    /*!
     * \brief Default constructor.
     *
     * Creates an empty GeometryCollection.
     */
    GeometryCollection() = default;

    /*!
     * \brief Add a Point to the collection (copy version).
     * \param[in] point The Point to add.
     *
     * \note Updates the internal order index.
     */
    void addPoint(const Point_t &point);

    /*!
     * \brief Add a Point to the collection (move version).
     * \param[in] point The Point to move into the collection.
     *
     * \note Updates the internal order index.
     */
    void addPoint(Point_t &&point);
    
    /*!
     * \brief Add a LineString to the collection (copy version).
     * \param[in] lineString The LineString to add.
     *
     * \note Updates the internal order index.
     */
    void addLineString(const LineString<Point_t> &lineString);

    /*!
     * \brief Add a LineString to the collection (move version).
     * \param[in] lineString The LineString to move into the collection.
     *
     * \note Updates the internal order index.
     */
    void addLineString(LineString<Point_t> &&lineString);

    /*!
     * \brief Add a Polygon to the collection (copy version).
     * \param[in] polygon The Polygon to add.
     *
     * \note Updates the internal order index.
     */
    void addPolygon(const Polygon<Point_t> &polygon);

    /*!
     * \brief Add a Polygon to the collection (move version).
     * \param[in] polygon The Polygon to move into the collection.
     *
     * \note Updates the internal order index.
     */
    void addPolygon(Polygon<Point_t> &&polygon);

    /*!
     * \brief Add a MultiPoint to the collection (copy version).
     * \param[in] multiPoint The MultiPoint to add.
     *
     * \note Updates the internal order index.
     */
    void addMultiPoint(const MultiPoint<Point_t> &multiPoint);

    /*!
     * \brief Add a MultiPoint to the collection (move version).
     * \param[in] multiPoint The MultiPoint to move into the collection.
     *
     * \note Updates the internal order index.
     */
    void addMultiPoint(MultiPoint<Point_t> &&multiPoint);

    /*!
     * \brief Add a MultiLineString to the collection (copy version).
     * \param[in] multiLineString The MultiLineString to add.
     *
     * \note Updates the internal order index.
     */
    void addMultiLineString(const MultiLineString<Point_t> &multiLineString);

    /*!
     * \brief Add a MultiLineString to the collection (move version).
     * \param[in] multiLineString The MultiLineString to move into the collection.
     *
     * \note Updates the internal order index.
     */
    void addMultiLineString(MultiLineString<Point_t> &&multiLineString);

    /*!
     * \brief Add a MultiPolygon to the collection (copy version).
     * \param[in] multiPolygon The MultiPolygon to add.
     *
     * \note Updates the internal order index.
     */
    void addMultiPolygon(const MultiPolygon<Point_t> &multiPolygon);

    /*!
     * \brief Add a MultiPolygon to the collection (move version).
     * \param[in] multiPolygon The MultiPolygon to move into the collection.
     *
     * \note Updates the internal order index.
     */
    void addMultiPolygon(MultiPolygon<Point_t> &&multiPolygon);

    /*!
     * \brief Add a nested GeometryCollection (copy version).
     * \param[in] collection The GeometryCollection to add.
     *
     * \note Updates the internal order index.
     * \note Supports arbitrary nesting levels.
     */
    void addGeometryCollection(const GeometryCollection<Point_t> &collection);

    /*!
     * \brief Add a nested GeometryCollection (move version).
     * \param[in] collection The GeometryCollection to move into the collection.
     *
     * \note Updates the internal order index.
     * \note Supports arbitrary nesting levels.
     */
    void addGeometryCollection(GeometryCollection<Point_t> &&collection);

    /*!
     * \brief Get read-only access to the Point vector.
     * \return Const reference to the vector of Points.
     *
     * \note This provides direct access to all Points in insertion order.
     */
    auto points() const noexcept -> const std::vector<Point_t> &;

    /*!
     * \brief Get read-only access to the LineString vector.
     * \return Const reference to the vector of LineStrings.
     *
     * \note This provides direct access to all LineStrings in insertion order.
     */
    auto lineStrings() const noexcept -> const std::vector<LineString<Point_t>> &;

    /*!
     * \brief Get read-only access to the Polygon vector.
     * \return Const reference to the vector of Polygons.
     *
     * \note This provides direct access to all Polygons in insertion order.
     */
    auto polygons() const noexcept -> const std::vector<Polygon<Point_t>> &;

    /*!
     * \brief Get read-only access to the MultiPoint vector.
     * \return Const reference to the vector of MultiPoints.
     *
     * \note This provides direct access to all MultiPoints in insertion order.
     */
    auto multiPoints() const noexcept -> const std::vector<MultiPoint<Point_t>> &;

    /*!
     * \brief Get read-only access to the MultiLineString vector.
     * \return Const reference to the vector of MultiLineStrings.
     *
     * \note This provides direct access to all MultiLineStrings in insertion order.
     */
    auto multiLineStrings() const noexcept -> const std::vector<MultiLineString<Point_t>> &;

    /*!
     * \brief Get read-only access to the MultiPolygon vector.
     * \return Const reference to the vector of MultiPolygons.
     *
     * \note This provides direct access to all MultiPolygons in insertion order.
     */
    auto multiPolygons() const noexcept -> const std::vector<MultiPolygon<Point_t>> &;

    /*!
     * \brief Get read-only access to the GeometryCollection vector.
     * \return Const reference to the vector of nested GeometryCollections.
     *
     * \note This provides direct access to all nested collections in insertion order.
     */
    auto geometryCollections() const noexcept -> const std::vector<GeometryCollection<Point_t>> &;

    /*!
     * \brief Get mutable reference to a Point by its index in the Points vector.
     * \param[in] index Index in the Points vector.
     * \return Reference to the Point at the specified index.
     * \throws std::out_of_range if index >= points().size().
     */
    auto pointAt(size_t index) -> Point_t &;

    /*!
     * \brief Get const reference to a Point by its index in the Points vector.
     * \param[in] index Index in the Points vector.
     * \return Const reference to the Point at the specified index.
     * \throws std::out_of_range if index >= points().size().
     */
    auto pointAt(size_t index) const -> const Point_t &;

    /*!
     * \brief Get mutable reference to a LineString by its index in the LineStrings vector.
     * \param[in] index Index in the LineStrings vector.
     * \return Reference to the LineString at the specified index.
     * \throws std::out_of_range if index >= lineStrings().size().
     */
    auto lineStringAt(size_t index) -> LineString<Point_t> &;

    /*!
     * \brief Get const reference to a LineString by its index in the LineStrings vector.
     * \param[in] index Index in the LineStrings vector.
     * \return Const reference to the LineString at the specified index.
     * \throws std::out_of_range if index >= lineStrings().size().
     */
    auto lineStringAt(size_t index) const -> const LineString<Point_t> &;

    /*!
     * \brief Get mutable reference to a Polygon by its index in the Polygons vector.
     * \param[in] index Index in the Polygons vector.
     * \return Reference to the Polygon at the specified index.
     * \throws std::out_of_range if index >= polygons().size().
     */
    auto polygonAt(size_t index) -> Polygon<Point_t> &;

    /*!
     * \brief Get const reference to a Polygon by its index in the Polygons vector.
     * \param[in] index Index in the Polygons vector.
     * \return Const reference to the Polygon at the specified index.
     * \throws std::out_of_range if index >= polygons().size().
     */
    auto polygonAt(size_t index) const -> const Polygon<Point_t> &;

    /*!
     * \brief Get mutable reference to a MultiPoint by its index in the MultiPoints vector.
     * \param[in] index Index in the MultiPoints vector.
     * \return Reference to the MultiPoint at the specified index.
     * \throws std::out_of_range if index >= multiPoints().size().
     */
    auto multiPointAt(size_t index) -> MultiPoint<Point_t> &;

    /*!
     * \brief Get const reference to a MultiPoint by its index in the MultiPoints vector.
     * \param[in] index Index in the MultiPoints vector.
     * \return Const reference to the MultiPoint at the specified index.
     * \throws std::out_of_range if index >= multiPoints().size().
     */
    auto multiPointAt(size_t index) const -> const MultiPoint<Point_t> &;

    /*!
     * \brief Get mutable reference to a MultiLineString by its index in the MultiLineStrings vector.
     * \param[in] index Index in the MultiLineStrings vector.
     * \return Reference to the MultiLineString at the specified index.
     * \throws std::out_of_range if index >= multiLineStrings().size().
     */
    auto multiLineStringAt(size_t index) -> MultiLineString<Point_t> &;

    /*!
     * \brief Get const reference to a MultiLineString by its index in the MultiLineStrings vector.
     * \param[in] index Index in the MultiLineStrings vector.
     * \return Const reference to the MultiLineString at the specified index.
     * \throws std::out_of_range if index >= multiLineStrings().size().
     */
    auto multiLineStringAt(size_t index) const -> const MultiLineString<Point_t> &;

    /*!
     * \brief Get mutable reference to a MultiPolygon by its index in the MultiPolygons vector.
     * \param[in] index Index in the MultiPolygons vector.
     * \return Reference to the MultiPolygon at the specified index.
     * \throws std::out_of_range if index >= multiPolygons().size().
     */
    auto multiPolygonAt(size_t index) -> MultiPolygon<Point_t> &;

    /*!
     * \brief Get const reference to a MultiPolygon by its index in the MultiPolygons vector.
     * \param[in] index Index in the MultiPolygons vector.
     * \return Const reference to the MultiPolygon at the specified index.
     * \throws std::out_of_range if index >= multiPolygons().size().
     */
    auto multiPolygonAt(size_t index) const -> const MultiPolygon<Point_t> &;
    
    /*!
     * \brief Get mutable reference to a nested GeometryCollection by its index.
     * \param[in] index Index in the GeometryCollections vector.
     * \return Reference to the GeometryCollection at the specified index.
     * \throws std::out_of_range if index >= geometryCollections().size().
     */
    auto geometryCollectionAt(size_t index) -> GeometryCollection<Point_t> &;

    /*!
     * \brief Get const reference to a nested GeometryCollection by its index.
     * \param[in] index Index in the GeometryCollections vector.
     * \return Const reference to the GeometryCollection at the specified index.
     * \throws std::out_of_range if index >= geometryCollections().size().
     */
    auto geometryCollectionAt(size_t index) const -> const GeometryCollection<Point_t> &;

    /*!
     * \brief Remove a Point by its index in the Points vector.
     * \param[in] index Index in the Points vector.
     * \return true if successful, false if index is out of range.
     *
     * \note Updates the internal order index to maintain consistency.
     */
    auto removePoint(size_t index) -> bool;

    /*!
     * \brief Remove a LineString by its index in the LineStrings vector.
     * \param[in] index Index in the LineStrings vector.
     * \return true if successful, false if index is out of range.
     *
     * \note Updates the internal order index to maintain consistency.
     */
    auto removeLineString(size_t index) -> bool;

    /*!
     * \brief Remove a Polygon by its index in the Polygons vector.
     * \param[in] index Index in the Polygons vector.
     * \return true if successful, false if index is out of range.
     *
     * \note Updates the internal order index to maintain consistency.
     */
    auto removePolygon(size_t index) -> bool;

    /*!
     * \brief Remove a MultiPoint by its index in the MultiPoints vector.
     * \param[in] index Index in the MultiPoints vector.
     * \return true if successful, false if index is out of range.
     *
     * \note Updates the internal order index to maintain consistency.
     */
    auto removeMultiPoint(size_t index) -> bool;

    /*!
     * \brief Remove a MultiLineString by its index in the MultiLineStrings vector.
     * \param[in] index Index in the MultiLineStrings vector.
     * \return true if successful, false if index is out of range.
     *
     * \note Updates the internal order index to maintain consistency.
     */
    auto removeMultiLineString(size_t index) -> bool;

    /*!
     * \brief Remove a MultiPolygon by its index in the MultiPolygons vector.
     * \param[in] index Index in the MultiPolygons vector.
     * \return true if successful, false if index is out of range.
     *
     * \note Updates the internal order index to maintain consistency.
     */
    auto removeMultiPolygon(size_t index) -> bool;

    /*!
     * \brief Remove a nested GeometryCollection by its index.
     * \param[in] index Index in the GeometryCollections vector.
     * \return true if successful, false if index is out of range.
     *
     * \note Updates the internal order index to maintain consistency.
     */
    auto removeGeometryCollection(size_t index) -> bool;

    /*!
     * \brief Remove a geometry by its index in the insertion order.
     * \param[in] indexInCollection Index in the collection (0-based, following insertion order).
     * \return true if successful, false if index is out of range.
     *
     * \note This is the recommended method for removing geometries when you know
     *       their position in the collection rather than their position in a type-specific vector.
     */
    auto removeAt(size_t indexInCollection) -> bool;

    /*!
     * \brief Get the number of Point geometries in the collection.
     * \return Number of Point geometries.
     */
    auto pointCount() const noexcept -> size_t;

    /*!
     * \brief Get the number of LineString geometries in the collection.
     * \return Number of LineString geometries.
     */
    auto lineStringCount() const noexcept -> size_t;

    /*!
     * \brief Get the number of Polygon geometries in the collection.
     * \return Number of Polygon geometries.
     */
    auto polygonCount() const noexcept -> size_t;

    /*!
     * \brief Get the number of MultiPoint geometries in the collection.
     * \return Number of MultiPoint geometries.
     */
    auto multiPointCount() const noexcept -> size_t;

    /*!
     * \brief Get the number of MultiLineString geometries in the collection.
     * \return Number of MultiLineString geometries.
     */
    auto multiLineStringCount() const noexcept -> size_t;

    /*!
     * \brief Get the number of MultiPolygon geometries in the collection.
     * \return Number of MultiPolygon geometries.
     */
    auto multiPolygonCount() const noexcept -> size_t;

    /*!
     * \brief Get the number of nested GeometryCollection geometries.
     * \return Number of nested GeometryCollection geometries.
     */
    auto geometryCollectionCount() const noexcept -> size_t;

    /*!
     * \brief Get the total number of geometries in the collection.
     * \return Number of geometries (all types combined).
     */
    auto size() const noexcept -> size_t;

    /*!
     * \brief Check if the collection contains any Point geometries.
     * \return true if at least one Point exists, false otherwise.
     */
    auto hasPoints() const noexcept -> bool;

    /*!
     * \brief Check if the collection contains any LineString geometries.
     * \return true if at least one LineString exists, false otherwise.
     */
    auto hasLineStrings() const noexcept -> bool;

    /*!
     * \brief Check if the collection contains any Polygon geometries.
     * \return true if at least one Polygon exists, false otherwise.
     */
    auto hasPolygons() const noexcept -> bool;

    /*!
     * \brief Check if the collection contains any MultiPoint geometries.
     * \return true if at least one MultiPoint exists, false otherwise.
     */
    auto hasMultiPoints() const noexcept -> bool;

    /*!
     * \brief Check if the collection contains any MultiLineString geometries.
     * \return true if at least one MultiLineString exists, false otherwise.
     */
    auto hasMultiLineStrings() const noexcept -> bool;

    /*!
     * \brief Check if the collection contains any MultiPolygon geometries.
     * \return true if at least one MultiPolygon exists, false otherwise.
     */
    auto hasMultiPolygons() const noexcept -> bool;

    /*!
     * \brief Check if the collection contains any nested GeometryCollections.
     * \return true if at least one nested GeometryCollection exists, false otherwise.
     */
    auto hasGeometryCollections() const noexcept -> bool;

    /*!
     * \brief Check if the collection is empty.
     * \return true if the collection contains no geometries, false otherwise.
     */
    auto empty() const noexcept -> bool;

    /*!
     * \brief Remove all geometries from the collection.
     *
     * \note All internal vectors are cleared and memory may be deallocated.
     */
    void clear();

    /*!
     * \brief Access a geometry by its position in the collection (non-const version).
     * \param[in] i Index in the collection (0-based, following insertion order).
     * \return GeometryReference variant containing a reference to the geometry.
     * \throws std::runtime_error if index is out of range or unknown geometry type.
     *
     * ### Example with std::visit:
     * \code{.cpp}
     * // Process geometry with compile-time type checking
     * for (size_t i = 0; i < collection.size(); ++i) {
     *     std::visit([](auto&& geom_ref) {
     *         using GeomType = std::decay_t<decltype(geom_ref.get())>;
     *
     *         if constexpr (std::is_same_v<GeomType, Point2d>) {
     *             auto& point = geom_ref.get();
     *             std::cout << "Point: " << point.x() << ", " << point.y() << std::endl;
     *         }
     *         else if constexpr (std::is_same_v<GeomType, LineString<Point2d>>) {
     *             auto& line = geom_ref.get();
     *             std::cout << "LineString with " << line.size() << " points" << std::endl;
     *         }
     *         else if constexpr (std::is_same_v<GeomType, Polygon<Point2d>>) {
     *             auto& polygon = geom_ref.get();
     *             std::cout << "Polygon with " << polygon.outer().size()
     *                       << " exterior points" << std::endl;
     *         }
     *         // Handle other geometry types similarly...
     *     }, collection[i]);
     * }
     * \endcode
     */
    auto operator[](size_t i) -> GeometryReference;

    /*!
     * \brief Access a geometry by its position in the collection (const version).
     * \param[in] i Index in the collection (0-based, following insertion order).
     * \return ConstGeometryReference variant containing a const reference to the geometry.
     * \throws std::runtime_error if index is out of range or unknown geometry type.
     */
    auto operator[](size_t i) const -> ConstGeometryReference;

private:

    /*!
     * \brief Internal helper to update the order index after removal.
     * \param[in] removedIndex Index in the type-specific vector that was removed.
     * \param[in] geometryType Type of geometry that was removed.
     *
     * \note This function updates mOrder by:
     *       1. Removing the entry corresponding to the removed geometry.
     *       2. Decrementing indices of subsequent geometries of the same type.
     */
    void removeIndex(size_t index, GeometryType geometryType);
};


template<typename Point_t>
void GeometryCollection<Point_t>::addPoint(const Point_t &point)
{
    mOrder.push_back({GeometryType::point, static_cast<uint32_t>(mPoints.size())});
    mPoints.push_back(point);
}

template<typename Point_t>
void GeometryCollection<Point_t>::addPoint(Point_t &&point) 
{ 
    mOrder.push_back({GeometryType::point, static_cast<uint32_t>(mPoints.size())});
    mPoints.push_back(std::move(point));
}

template<typename Point_t>
void GeometryCollection<Point_t>::addLineString(const LineString<Point_t> &lineString)
{
    mOrder.push_back({GeometryType::linestring, static_cast<uint32_t>(mLineString.size())});
    mLineString.push_back(lineString);
}

template<typename Point_t>
void GeometryCollection<Point_t>::addLineString(LineString<Point_t> &&lineString)
{ 
    mOrder.push_back({GeometryType::linestring, static_cast<uint32_t>(mLineString.size())});
    mLineString.push_back(lineString); 
}

template<typename Point_t>
void GeometryCollection<Point_t>::addPolygon(const Polygon<Point_t> &polygon)
{
    mOrder.push_back({GeometryType::polygon, static_cast<uint32_t>(mPolygons.size())});
    mPolygons.push_back(polygon);
}

template<typename Point_t>
void GeometryCollection<Point_t>::addPolygon(Polygon<Point_t> &&polygon)
{ 
    mOrder.push_back({GeometryType::polygon, static_cast<uint32_t>(mPolygons.size())});
    mPolygons.push_back(std::move(polygon));
}

template<typename Point_t>
void GeometryCollection<Point_t>::addMultiPoint(const MultiPoint<Point_t> &multiPoint)
{
    mOrder.push_back({GeometryType::multipoint, static_cast<uint32_t>(mMultiPoints.size())});
    mMultiPoints.push_back(multiPoint); 
}

template<typename Point_t>
void GeometryCollection<Point_t>::addMultiPoint(MultiPoint<Point_t> &&multiPoint)
{ 
    mOrder.push_back({GeometryType::multipoint, static_cast<uint32_t>(mMultiPoints.size())});
    mMultiPoints.push_back(std::move(multiPoint)); 
}

template<typename Point_t>
void GeometryCollection<Point_t>::addMultiLineString(const MultiLineString<Point_t> &multiLineString)
{
    mOrder.push_back({GeometryType::multilinestring, static_cast<uint32_t>(mMultiLineStrings.size())});
    mMultiLineStrings.push_back(multiLineString);
}

template<typename Point_t>
void GeometryCollection<Point_t>::addMultiLineString(MultiLineString<Point_t> &&multiLineString) 
{ 
    mOrder.push_back({GeometryType::multilinestring, static_cast<uint32_t>(mMultiLineStrings.size())});
    mMultiLineStrings.push_back(std::move(multiLineString));
}

template<typename Point_t>
void GeometryCollection<Point_t>::addMultiPolygon(const MultiPolygon<Point_t> &multiPolygon) 
{ 
    mOrder.push_back({GeometryType::multipolygon, static_cast<uint32_t>(mMultiPolygons.size())});
    mMultiPolygons.push_back(multiPolygon);
}

template<typename Point_t>
void GeometryCollection<Point_t>::addMultiPolygon(MultiPolygon<Point_t> &&multiPolygon)
{ 
    mOrder.push_back({GeometryType::multipolygon, static_cast<uint32_t>(mMultiPolygons.size())});
    mMultiPolygons.push_back(std::move(multiPolygon)); 
}

template<typename Point_t>
void GeometryCollection<Point_t>::addGeometryCollection(const GeometryCollection<Point_t> &collection) 
{
    mOrder.push_back({GeometryType::collection, static_cast<uint32_t>(mCollections.size())});
    mCollections.push_back(collection);
}

template<typename Point_t>
void GeometryCollection<Point_t>::addGeometryCollection(GeometryCollection<Point_t> &&collection)
{
    mOrder.push_back({GeometryType::collection, static_cast<uint32_t>(mCollections.size())});
    mCollections.push_back(std::move(collection));
}

template<typename Point_t>
auto GeometryCollection<Point_t>::points() const noexcept -> const std::vector<Point_t> &
{ 
    return mPoints;
}

template<typename Point_t>
auto GeometryCollection<Point_t>::lineStrings() const noexcept -> const std::vector<LineString<Point_t>> &
{
    return mLineString;
}

template<typename Point_t>
auto GeometryCollection<Point_t>::polygons() const noexcept -> const std::vector<Polygon<Point_t>> &
{ 
    return mPolygons; 
}

template<typename Point_t>
auto GeometryCollection<Point_t>::multiPoints() const noexcept -> const std::vector<MultiPoint<Point_t>> &
{
    return mMultiPoints; 
}

template<typename Point_t>
auto GeometryCollection<Point_t>::multiLineStrings() const noexcept -> const std::vector<MultiLineString<Point_t>> &
{ 
    return mMultiLineStrings;
}

template<typename Point_t>
auto GeometryCollection<Point_t>::multiPolygons() const noexcept -> const std::vector<MultiPolygon<Point_t>> &
{ 
    return mMultiPolygons; 
}

template<typename Point_t>
auto GeometryCollection<Point_t>::geometryCollections() const noexcept -> const std::vector<GeometryCollection<Point_t>> &
{
    return mCollections;
}

template<typename Point_t>
auto GeometryCollection<Point_t>::pointAt(size_t index) -> Point_t &
{ 
    return mPoints.at(index);
}

template<typename Point_t>
auto GeometryCollection<Point_t>::pointAt(size_t index) const -> const Point_t &
{
    return mPoints.at(index);
}

template<typename Point_t>
auto GeometryCollection<Point_t>::lineStringAt(size_t index) -> LineString<Point_t> &
{
    return mLineString.at(index);
}

template<typename Point_t>
auto GeometryCollection<Point_t>::lineStringAt(size_t index) const -> const LineString<Point_t> &
{ 
    return mLineString.at(index); 
}

template<typename Point_t>
auto GeometryCollection<Point_t>::polygonAt(size_t index) -> Polygon<Point_t> &
{
    return mPolygons.at(index);
}

template<typename Point_t>
auto GeometryCollection<Point_t>::polygonAt(size_t index) const -> const Polygon<Point_t> &
{
    return mPolygons.at(index);
}

template<typename Point_t>
auto GeometryCollection<Point_t>::multiPointAt(size_t index) -> MultiPoint<Point_t> &
{
    return mMultiPoints.at(index);
}

template<typename Point_t>
auto GeometryCollection<Point_t>::multiPointAt(size_t index) const -> const MultiPoint<Point_t> &
{
    return mMultiPoints.at(index);
}

template<typename Point_t>
auto GeometryCollection<Point_t>::multiLineStringAt(size_t index) -> MultiLineString<Point_t> &
{
    return mMultiLineStrings.at(index);
}

template<typename Point_t>
auto GeometryCollection<Point_t>::multiLineStringAt(size_t index) const -> const MultiLineString<Point_t> &
{
    return mMultiLineStrings.at(index);
}

template<typename Point_t>
auto GeometryCollection<Point_t>::multiPolygonAt(size_t index) -> MultiPolygon<Point_t> &
{
    return mMultiPolygons.at(index);
}

template<typename Point_t>
auto GeometryCollection<Point_t>::multiPolygonAt(size_t index) const -> const MultiPolygon<Point_t> &
{
    return mMultiPolygons.at(index);
}

template<typename Point_t>
auto GeometryCollection<Point_t>::geometryCollectionAt(size_t index) -> GeometryCollection<Point_t> &
{
    return mCollections.at(index);
}

template<typename Point_t>
auto GeometryCollection<Point_t>::geometryCollectionAt(size_t index) const -> const GeometryCollection<Point_t> &
{
    return mCollections.at(index);
}

template<typename Point_t>
auto GeometryCollection<Point_t>::removePoint(size_t index) -> bool
{
    if (index >= mPoints.size()) return false;

    mPoints.erase(mPoints.begin() + index);

    removeIndex(index, GeometryType::point);

    return true;
}

template<typename Point_t>
auto GeometryCollection<Point_t>::removeLineString(size_t index) -> bool
{
    if (index >= mLineString.size()) return false;

    mLineString.erase(mLineString.begin() + index);

    removeIndex(index, GeometryType::linestring);

    return true;
}

template<typename Point_t>
auto GeometryCollection<Point_t>::removePolygon(size_t index) -> bool
{
    if (index >= mPolygons.size()) return false;

    mPolygons.erase(mPolygons.begin() + index);

    removeIndex(index, GeometryType::polygon);

    return true;
}

template<typename Point_t>
auto GeometryCollection<Point_t>::removeMultiPoint(size_t index) -> bool
{
    if (index >= mMultiPoints.size()) return false;

    mMultiPoints.erase(mMultiPoints.begin() + index);

    removeIndex(index, GeometryType::multipoint);

    return true;
}

template<typename Point_t>
auto GeometryCollection<Point_t>::removeMultiLineString(size_t index) -> bool
{
    if (index >= mMultiLineStrings.size()) return false;

    mMultiLineStrings.erase(mMultiLineStrings.begin() + index);

    removeIndex(index, GeometryType::multilinestring);

    return true;
}

template<typename Point_t>
auto GeometryCollection<Point_t>::removeMultiPolygon(size_t index) -> bool
{
    if (index >= mMultiPolygons.size()) return false;

    mMultiPolygons.erase(mMultiPolygons.begin() + index);

    removeIndex(index, GeometryType::multipolygon);

    return true;
}

template<typename Point_t>
auto GeometryCollection<Point_t>::removeGeometryCollection(size_t index) -> bool
{
    if (index >= mCollections.size()) return false;

    mCollections.erase(mCollections.begin() + index);

    removeIndex(index, GeometryType::collection);

    return true;
}

template<typename Point_t>
auto GeometryCollection<Point_t>::removeAt(size_t indexInCollection) -> bool
{
    if (indexInCollection >= mOrder.size()) return false;

    const GeoID &geoId = mOrder[indexInCollection];

    switch (geoId.type) {
        case GeometryType::point:
            return removePoint(geoId.index);
        case GeometryType::linestring:
            return removeLineString(geoId.index);
        case GeometryType::polygon:
            return removePolygon(geoId.index);
        case GeometryType::multipoint:
            return removeMultiPoint(geoId.index);
        case GeometryType::multilinestring:
            return removeMultiLineString(geoId.index);
        case GeometryType::multipolygon:
            return removeMultiPolygon(geoId.index);
        case GeometryType::collection:
            return removeGeometryCollection(geoId.index);
        default:
            return false;
    }
}

template<typename Point_t>
auto GeometryCollection<Point_t>::pointCount() const noexcept -> size_t
{
    return mPoints.size();
}

template<typename Point_t>
auto GeometryCollection<Point_t>::lineStringCount() const noexcept -> size_t
{
    return mLineString.size();
}

template<typename Point_t>
auto GeometryCollection<Point_t>::polygonCount() const noexcept -> size_t
{
    return mPolygons.size();
}

template<typename Point_t>
inline auto GeometryCollection<Point_t>::multiPointCount() const noexcept -> size_t
{
    return mMultiPoints.size();
}

template<typename Point_t>
auto GeometryCollection<Point_t>::multiLineStringCount() const noexcept -> size_t
{
    return mMultiLineStrings.size();
}

template<typename Point_t>
auto GeometryCollection<Point_t>::multiPolygonCount() const noexcept -> size_t
{
    return mMultiPolygons.size();
}

template<typename Point_t>
auto GeometryCollection<Point_t>::geometryCollectionCount() const noexcept -> size_t
{
    return mCollections.size();
}

template<typename Point_t>
auto GeometryCollection<Point_t>::size() const noexcept -> size_t
{
    return mOrder.size();
}

template<typename Point_t>
auto GeometryCollection<Point_t>::hasPoints() const noexcept -> bool
{
    return !mPoints.empty();
}

template<typename Point_t>
auto GeometryCollection<Point_t>::hasLineStrings() const noexcept -> bool
{
    return !mLineString.empty();
}

template<typename Point_t>
auto GeometryCollection<Point_t>::hasPolygons() const noexcept -> bool
{
    return !mPolygons.empty();
}

template<typename Point_t>
auto GeometryCollection<Point_t>::hasMultiPoints() const noexcept -> bool
{
    return !mMultiPoints.empty();
}

template<typename Point_t>
auto GeometryCollection<Point_t>::hasMultiLineStrings() const noexcept -> bool
{
    return !mMultiLineStrings.empty();
}

template<typename Point_t>
auto GeometryCollection<Point_t>::hasMultiPolygons() const noexcept -> bool
{
    return !mMultiPolygons.empty();
}

template<typename Point_t>
auto GeometryCollection<Point_t>::hasGeometryCollections() const noexcept -> bool
{
    return !mCollections.empty();
}

template<typename Point_t>
auto GeometryCollection<Point_t>::empty() const noexcept -> bool
{
    return mOrder.empty();
}

template<typename Point_t>
void GeometryCollection<Point_t>::clear()
{
    mPoints.clear();
    mLineString.clear();
    mPolygons.clear();
    mMultiPoints.clear();
    mMultiLineStrings.clear();
    mMultiPolygons.clear();
    mCollections.clear();
    mOrder.clear();
}

template<typename Point_t>
auto GeometryCollection<Point_t>::operator[](size_t i) -> GeometryReference
{
    const auto &id = mOrder[i];
    switch (id.type) {
    case GeometryType::point:
        return std::ref(mPoints[id.index]);
    case GeometryType::linestring:
        return std::ref(mLineString[id.index]);
    case GeometryType::polygon:
        return std::ref(mPolygons[id.index]);
    case GeometryType::multipoint:
        return std::ref(mMultiPoints[id.index]);
    case GeometryType::multilinestring:
        return std::ref(mMultiLineStrings[id.index]);
    case GeometryType::multipolygon:
        return std::ref(mMultiPolygons[id.index]);
    case GeometryType::collection:
        return std::ref(mCollections[id.index]);
    default:
        throw std::runtime_error("Unknown type");
    }
}

template<typename Point_t>
auto GeometryCollection<Point_t>::operator[](size_t i) const -> ConstGeometryReference
{
    const auto &id = mOrder[i];
    switch (id.type) {
    case GeometryType::point:
        return std::cref(mPoints[id.index]);
    case GeometryType::linestring:
        return std::cref(mLineString[id.index]);
    case GeometryType::polygon:
        return std::cref(mPolygons[id.index]);
    case GeometryType::multipoint:
        return std::cref(mMultiPoints[id.index]);
    case GeometryType::multilinestring:
        return std::cref(mMultiLineStrings[id.index]);
    case GeometryType::multipolygon:
        return std::cref(mMultiPolygons[id.index]);
    case GeometryType::collection:
        return std::cref(mCollections[id.index]);
    default:
        throw std::runtime_error("Unknown type");
    }
}

template<typename Point_t>
void GeometryCollection<Point_t>::removeIndex(size_t index, GeometryType geometryType)
{
    auto it = mOrder.begin();
    while (it != mOrder.end()) {
        if (it->type == geometryType) {
            if (it->index == index) {
                it = mOrder.erase(it);
                continue;
            } else if (it->index > index) {
                --(it->index);
            }
        }
        ++it;
    }
}


/*! \} */ 



}