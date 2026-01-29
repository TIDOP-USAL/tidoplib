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

/*! \file BoundingBox.h
 * \brief Axis-aligned bounding box implementation.
 *
 * This file defines the BoundingBox class template, which represents an
 * axis-aligned bounding box (AABB) defined by two corner points (minimum
 * and maximum coordinates). It inherits from GeometryBase and provides
 * operations for creating, modifying, and querying bounding boxes.
 * ### Classes
 * - \ref tl::BoundingBox : Main template class for axis-aligned bounding boxes.
 * ### Type Aliases
 * - \ref tl::BoundingBox2i, \ref tl::BoundingBox2f, \ref tl::BoundingBox2d : 2D integer, float, and double bounding boxes.
 * - \ref tl::BoundingBox3i, \ref tl::BoundingBox3f, \ref tl::BoundingBox3d : 3D integer, float, and double bounding boxes.
 * \see tl::GeometryBase, tl::Point
 */

#pragma once

#include <algorithm>

#include "tidop/geometry/base/Geometry.h"
#include "tidop/geometry/primitives/Point.h"

namespace tl
{

/*! \addtogroup Geometry
 *  \{
 */

/*!
 * \class BoundingBox
 * \brief Axis-aligned bounding box defined by two corner points.
 *
 * Represents an axis-aligned bounding box (AABB) that encloses a set of
 * points or geometries. The box is defined by two points: the minimum
 * and maximum coordinates along each axis.
 *
 * \tparam Point_t Type of the points used to define the bounding box
 *                 (e.g., Point2d, Point3f).
 */
template<typename Point_t>
class BoundingBox final
  : public Geometry<BoundingBox<Point_t>>
{

public:

    /*! \brief Type of points stored in the bounding box. */
    using value_type = Point_t;

    /*! \brief Scalar type of the point coordinates. */
    using T = typename point_traits<Point_t>::value_type;

    /*! \brief Dimension of the bounding box (compile-time constant). */
    static constexpr size_t dimensions = dimension_value(point_traits<Point_t>::dimension);

private:

    std::array<Point_t, 2> mPoints; /*!< Array containing the two corner points [min, max]. */

public:

    /*!
     * \brief Default constructor.
     * Creates an empty/invalid bounding box.
     */
    BoundingBox();

    /*!
     * \brief Constructor that defines the bounding box using two corner points.
     * \param[in] pt1 First corner point.
     * \param[in] pt2 Second corner point.
     * The bounding box will be normalized (min and max computed automatically).
     */
    BoundingBox(const Point_t &pt1, const Point_t &pt2);

    /*!
     * \brief Constructor that defines the bounding box using a central point and dimensions (2D).
     * \tparam U Type of dimension values (must be convertible to T).
     * \param[in] pt Center of the bounding box.
     * \param[in] width Width of the bounding box (x-axis).
     * \param[in] height Height of the bounding box (y-axis).
     */
    template<typename U>
    BoundingBox(const Point_t &pt, U width, U height);

    /*!
     * \brief Constructor that defines the bounding box using a central point and dimensions (3D).
     * \tparam U Type of dimension values (must be convertible to T).
     * \param[in] pt Center of the bounding box.
     * \param[in] width Width of the bounding box (x-axis).
     * \param[in] depth Depth of the bounding box (z-axis).
     * \param[in] height Height of the bounding box (y-axis).
     */
    template<typename U>
    BoundingBox(const Point_t &pt, U width, U depth, U height);

    /*!
     * \brief Constructor that defines a cubic bounding box using a central point and side length.
     * \tparam U Type of side length (must be convertible to T).
     * \param[in] pt Center of the bounding box.
     * \param[in] side Length of each side (width = depth = height).
     */
    template<typename U>
    BoundingBox(const Point_t &pt, U side);

    /*!
     * \brief Constructs a bounding box from a collection of points.
     * \param[in] vertices Vector of points to compute bounding box for.
     * The bounding box will enclose all points in the collection.
     */
    explicit BoundingBox(const std::vector<Point_t> &vertices);

    /*!
     * \brief Constructor from bounding box with different point type.
     * \tparam OtherPoint_t Type of the other bounding box's points.
     * \param[in] other Bounding box to convert from.
     */
    template<typename OtherPoint_t>
    explicit BoundingBox(const BoundingBox<OtherPoint_t>&other);

    /*!
     * \brief Access the first corner point (non-const version).
     * \return Reference to the first corner point.
     */
    auto pt1() noexcept -> Point_t &;

    /*!
     * \brief Access the first corner point (const version).
     * \return Const reference to the first corner point.
     */
    auto pt1() const noexcept -> const Point_t &;

    /*!
     * \brief Access the second corner point (non-const version).
     * \return Reference to the second corner point.
     */
    auto pt2() noexcept -> Point_t &;

    /*!
     * \brief Access the second corner point (const version).
     * \return Const reference to the second corner point.
     */
    auto pt2() const noexcept -> const Point_t &;

    /*!
     * \brief Computes the width of the bounding box (x-axis extent).
     * \return Width as scalar type T.
     */
    auto width() const -> T;

    /*!
     * \brief Computes the height of the bounding box (y-axis extent).
     * \return Height as scalar type T.
     */
    auto height() const -> T;

    /*!
     * \brief Computes the depth of the bounding box (z-axis extent).
     * \return Depth as scalar type T.
     * \note Only valid for 3D bounding boxes.
     */
    auto depth() const -> T;

    /*!
     * \brief Expands the bounding box to include a point.
     * \param[in] pt Point to add to the bounding box.
     * If the point is outside the current bounds, the bounding box is expanded.
     */
    void add(const Point_t &pt);

    /*!
     * \brief Normalizes the bounding box.
     * Ensures pt1 is the minimum corner and pt2 is the maximum corner.
     */
    void normalized();

    /*!
     * \brief Computes the center point of the bounding box.
     * \return Center point of the bounding box.
     */
    auto center() const->Point_t;

    /*!
     * \brief Returns all vertices (corner points) of the bounding box.
     * \return Vector containing all 2^D corner points of the bounding box.
     */
    auto vertices() const->std::vector<Point_t>;

    /*!
     * \brief Expands this bounding box to include another bounding box.
     * \param[in] other Another bounding box to include.
     * The result is the union of both bounding boxes.
     */
    void extend(const BoundingBox<Point_t> &other);

    /*!
     * \brief Checks if the bounding box is empty.
     * \return true if the bounding box has zero volume, false otherwise.
     */
    auto isEmpty() const -> bool;

    /*!
     * \brief Checks if the bounding box is valid.
     * \return true if the bounding box is normalized and has positive extent, false otherwise.
     */
    auto isValid() const -> bool;

};


// TYPE ALIASES

/*! \brief 2D bounding box with integer coordinates. */
using BoundingBox2i = BoundingBox<Point2i>;

/*! \brief 2D bounding box with float coordinates. */
using BoundingBox2f = BoundingBox<Point2f>;

/*! \brief 2D bounding box with double coordinates. */
using BoundingBox2d = BoundingBox<Point2d>;

/*! \brief 3D bounding box with integer coordinates. */
using BoundingBox3i = BoundingBox<Point3i>;

/*! \brief 3D bounding box with float coordinates. */
using BoundingBox3f = BoundingBox<Point3f>;

/*! \brief 3D bounding box with double coordinates. */
using BoundingBox3d = BoundingBox<Point3d>;



template<typename Point_t>
BoundingBox<Point_t>::BoundingBox()
{
    for (size_t i = 0; i < dimensions; ++i) {
        mPoints[0][i] = std::numeric_limits<T>::max();
        mPoints[1][i] = std::numeric_limits<T>::lowest();
    }
}

template<typename Point_t>
BoundingBox<Point_t>::BoundingBox(const Point_t &pt1, const Point_t &pt2)
    : mPoints{pt1, pt2}
{
    normalized();
}

template<typename Point_t>
template<typename U>
BoundingBox<Point_t>::BoundingBox(const Point_t &pt, U width, U height)
{
    static_assert(dimensions == 2, "This constructor requires a 2D Point.");

    auto half_width = width / consts::two<T>;
    auto half_height = height / consts::two<T>;

    mPoints[0] = Point_t(pt.x() - half_width, pt.y() - half_height);
    mPoints[1] = Point_t(pt.x() + half_width, pt.y() + half_height);

    if constexpr (std::is_integral_v<T>) {
        mPoints[1].x() += static_cast<T>(width) % 2;
        mPoints[1].y() += static_cast<T>(height) % 2;
    }
}

template<typename Point_t>
template<typename U>
BoundingBox<Point_t>::BoundingBox(const Point_t &pt, U width, U depth, U height)
{
    static_assert(dimensions == 3, "This constructor requires a 3D Point.");

    auto half_width = width / consts::two<T>;
    auto half_depth = depth / consts::two<T>;
    auto half_height = height / consts::two<T>;

    mPoints[0] = Point_t(pt.x() - half_width, pt.y() - half_depth, pt.z() - half_height);
    mPoints[1] = Point_t(pt.x() + half_width, pt.y() + half_depth, pt.z() + half_height);

    if constexpr (std::is_integral_v<T>) {
        mPoints[1].x() += static_cast<T>(width) % 2;
        mPoints[1].y() += static_cast<T>(depth) % 2;
        mPoints[1].z() += static_cast<T>(height) % 2; 
    }
}

template<typename Point_t>
template<typename U>
BoundingBox<Point_t>::BoundingBox(const Point_t &pt, U side)
{
    T half_side = static_cast<T>(side) / 2;
    T extra = (std::is_integral_v<T> && (static_cast<int>(side) % 2 != 0)) ? 1 : 0;

    for (size_t i = 0; i < dimensions; ++i) {
        mPoints[0][i] = pt[i] - half_side;
        mPoints[1][i] = pt[i] + half_side + extra;
    }
}

template<typename Point_t>
BoundingBox<Point_t>::BoundingBox(const std::vector<Point_t> &vertices)
  : BoundingBox()
{
    for (const auto &v : vertices) {
        add(v);
    }
}

template<typename Point_t>
template<typename OtherPoint_t>
BoundingBox<Point_t>::BoundingBox(const BoundingBox<OtherPoint_t> &other)
{
    static_assert(geometry_traits<Point_t>::dimension == geometry_traits<OtherPoint_t>::dimension,
                  "BoundingBoxes must have the same dimension for conversion.");

    mPoints[0] = static_cast<Point_t>(other.pt1());
    mPoints[1] = static_cast<Point_t>(other.pt2());
}


template<typename Point_t>
auto BoundingBox<Point_t>::pt1() noexcept -> Point_t &
{ 
    return mPoints[0];
}

template<typename Point_t>
auto BoundingBox<Point_t>::pt1() const noexcept -> const Point_t &
{ 
    return mPoints[0];
}

template<typename Point_t>
auto BoundingBox<Point_t>::pt2() noexcept -> Point_t &
{ 
    return mPoints[1];
}

template<typename Point_t>
auto BoundingBox<Point_t>::pt2() const noexcept -> const Point_t &
{ 
    return mPoints[1];
}


template<typename Point_t>
auto BoundingBox<Point_t>::width() const -> T
{
    return this->isEmpty() ? consts::zero<T> : mPoints[1].x() - mPoints[0].x();
}

template<typename Point_t>
auto BoundingBox<Point_t>::height() const -> T
{
    return this->isEmpty() ? consts::zero<T> : mPoints[1].y() - mPoints[0].y();
}

template<typename Point_t>
auto BoundingBox<Point_t>::depth() const -> T
{
    static_assert(!is_2d_v<BoundingBox<Point_t>>, "Method only supported for 3 or 4 dimensions");

    return this->isEmpty() ? consts::zero<T> : mPoints[1].z() - mPoints[0].z();
}

template<typename Point_t>
void BoundingBox<Point_t>::add(const Point_t &pt)
{
    if (!isValid()) {
        mPoints[0] = mPoints[1] = pt;
        return;
    }

    for (size_t i = 0; i < dimensions; ++i) {
        mPoints[0][i] = std::min(mPoints[0][i], pt[i]);
        mPoints[1][i] = std::max(mPoints[1][i], pt[i]);
    }
}

// Normalización N-Dimensional
template<typename Point_t>
void BoundingBox<Point_t>::normalized()
{
    for (size_t i = 0; i < dimensions; ++i) {
        if (mPoints[0][i] > mPoints[1][i]) std::swap(mPoints[0][i], mPoints[1][i]);
    }
}

template<typename Point_t>
auto BoundingBox<Point_t>::center() const -> Point_t
{
    Point_t center{};
    if (!this->isEmpty()) {
        auto vector = (mPoints[1] - mPoints[0]) / consts::two<T>;
        for (size_t i = 0; i < dimensions; ++i) {
            center[i] = mPoints[0][i] + vector[i];
        }
    }

    return center;
}

template<typename Point_t>
auto BoundingBox<Point_t>::vertices() const -> std::vector<Point_t>
{
    return {
        Point_t(pt1().x(), pt1().y(), pt1().z()),
        Point_t(pt1().x(), pt2().y(), pt1().z()),
        Point_t(pt2().x(), pt2().y(), pt1().z()),
        Point_t(pt2().x(), pt1().y(), pt1().z()),
        Point_t(pt1().x(), pt1().y(), pt2().z()),
        Point_t(pt1().x(), pt2().y(), pt2().z()),
        Point_t(pt2().x(), pt2().y(), pt2().z()),
        Point_t(pt2().x(), pt1().y(), pt2().z())
    };
}

template<typename Point_t>
void BoundingBox<Point_t>::extend(const BoundingBox<Point_t> &other)
{
    *this = merge(*this, other);
}

template<typename Point_t>
auto BoundingBox<Point_t>::isEmpty() const -> bool
{
    return mPoints[0][0] == std::numeric_limits<T>::max();
}

template<typename Point_t>
auto BoundingBox<Point_t>::isValid() const -> bool
{
    for (size_t i = 0; i < dimensions; ++i) {
        if (mPoints[0][i] > mPoints[1][i]) return false;
    }

    return true;
}



template<typename Point_t>
auto merge(const BoundingBox<Point_t> &b1, const BoundingBox<Point_t> &b2) -> BoundingBox<Point_t>
{
    if (!b1.isValid()) return b2;
    if (!b2.isValid()) return b1;

    Point_t new_min, new_max;

    for (size_t i = 0; i < VectorTraits<Point_t>::size; ++i) {
        new_min[i] = std::min(b1.pt1()[i], b2.pt1()[i]);
        new_max[i] = std::max(b1.pt2()[i], b2.pt2()[i]);
    }

    return BoundingBox<Point_t>(new_min, new_max);
}

/*! \} */

} // End namespace tl

