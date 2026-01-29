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

/*! \file Point.hpp
 * \brief N-dimensional point primitive implementation.
 *
 * This file defines the Point class template, which serves as the fundamental
 * building block for all geometric entities. It supports dimensions 2, 3, and 4
 * and provides vector-like operations through VectorBase. It also supports 
 * meassurent values (M) for 2 and 3 dimensions.
 * * ### Classes
 * - \ref tl::Point : Main template class for N-dimensional points.
 * * ### Type Aliases
 * - \ref tl::Point2d, \ref tl::Point3d, \ref tl::Point4d, tl::Point2dm, \ref tl::Point3dm : Common double-precision aliases.
 * - \ref tl::Point2f, \ref tl::Point3f, \ref tl::Point4f, tl::Point2fm, \ref tl::Point3fm : Common float-precision aliases.
 * - \ref tl::Point2i, \ref tl::Point3i, \ref tl::Point4f, tl::Point2im, \ref tl::Point3im : Common integral-precision aliases.
 * * \see tl::VectorBase, tl::GeometryBase
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
 * \class Point
 * 
 * \brief N-dimensional point representing a location in space with optional measure (M).
 *
 * This class template represents a point whose coordinate structure is defined by a Tag.
 * It strictly separates spatial dimensions (X, Y, Z) from storage size, allowing
 * support for OGC-compliant types like XYM or XYZM.
 *
 * \tparam T Data type for the coordinates (e.g., float, double, int).
 * \tparam Tag Type tag defining the coordinate layout (e.g., xy_tag, xyz_tag, xym_tag, xyzm_tag).
 * \ingroup GeometricPrimitives
 */
template<typename T, typename Tag = xy_tag>
class Point
  : public Geometry<Point<T, Tag>>,
    public VectorBase<Point<T, Tag>>
{

public:

    using value_type = T;
    using reference = T &;
    using const_reference = const T &;
    using pointer = T *;
    using const_pointer = const T *;

private:

    static constexpr size_t storage_size = Tag::storage_size;
    static constexpr size_t spatial_dims = Tag::spatial_dims;

    std::array<T, storage_size> mData;

public:

    /*!
     * \brief Default constructor. Initializes X and Y to default values.
     */
    Point();

    template<typename... Args, std::enable_if_t<sizeof...(Args) == storage_size, int> = 0>
    explicit Point(Args... args)
      : mData{static_cast<T>(args)...}
    {
        static_assert((std::is_convertible_v<Args, T> && ...), "All arguments must be convertible to the point's data type");
    }

    /*!
     * \brief Copy constructor.
     * \param[in] point Point object to be copied.
     */
    Point(const Point &point) = default;

    /*!
     * \brief Move constructor.
     * \param[in] point Point object to be moved.
     */
    Point(Point &&point) noexcept = default;

    /*!
     * \brief Constructs a Point from an array.
     * \param[in] array Array containing two elements [x, y].
     */
    explicit Point(const std::array<T, storage_size> &a);

    /*! \brief Destructor. */
    ~Point() = default;

    /*!
     * \brief Copy assignment operator.
     * \param[in] point Point object to be copied.
     * \return Reference to this Point.
     */
    auto operator = (const Point &point) -> Point & = default;

    /*!
     * \brief Move assignment operator.
     * \param[in] point Point object to be moved.
     * \return Reference to this Point.
     */
    auto operator = (Point &&point) noexcept -> Point & = default;

    /*!
     * \brief Access the x-component of the point.
     * \return A const reference to the x-component.
     * \note Only valid for points with size at least 1.
     */
    auto x() const noexcept -> const_reference;

    /*!
     * \brief Access the x-component of the point (non-const version).
     * \return A reference to the x-component.
     * \note Only valid for points with size at least 1.
     */
    auto x() noexcept -> reference;

    /*!
     * \brief Access the y-component of the point.
     * \return A const reference to the y-component.
     * \note Only valid for points with size at least 2.
     */
    auto y() const noexcept -> const_reference;

    /*!
     * \brief Access the y-component of the point (non-const version).
     * \return A reference to the y-component.
     * \note Only valid for points with size at least 2.
     */
    auto y() noexcept -> reference;

    /*!
     * \brief Access the z-component of the point.
     * \return A const reference to the z-component.
     * \note Only valid for points with size at least 3.
     */
    auto z() const noexcept -> const_reference;

    /*!
     * \brief Access the z-component of the point (non-const version).
     * \return A reference to the z-component.
     * \note Only valid for points with size at least 3.
     */
    auto z() noexcept -> reference;

    /*!
     * \brief Access the measure (M) component of the point.
     * \return A const reference to the measure value.
     * \note This method is only available for points using a Tag that includes measures (e.g., xym_tag).
     */
    auto m() const noexcept -> const_reference;

    /*!
     * \brief Access the measure (M) component of the point (non-const version).
     * \return A const reference to the measure value.
     * \note This method is only available for points using a Tag that includes measures (e.g., xym_tag).
     */
    auto m() noexcept -> reference;

    /*!
     * \brief Access the w-component of the point.
     * \return A const reference to the w-component.
     * \note Only valid for points with size at least 4.
     */
    auto w() const noexcept -> const_reference;

    /*!
     * \brief Access the w-component of the vector (non-const version).
     * \return A reference to the w-component.
     * \note Only valid for vectors with size at least 4.
     */
    auto w() noexcept -> reference;

    auto operator[](std::size_t position) noexcept -> reference;
    auto operator[](std::size_t position) const noexcept -> const_reference;

    /*!
     * \brief Accesses the element at the specified position with bounds checking.
     *
     * \param[in] position The position of the element to access.
     * \return A reference to the element at the specified position.
     * \throws std::out_of_range if the position is out of bounds.
     */
    auto at(size_t position) -> reference;

    /*!
     * \brief Accesses the element at the specified position with bounds checking (const version).
     *
     * \param[in] position The position of the element to access.
     * \return A const reference to the element at the specified position.
     * \throws std::out_of_range if the position is out of bounds.
     */
    auto at(size_t position) const -> const_reference;

    /*!
     * \brief Returns the spatial dimension of the point.
     * Storage components like Measure (M) are not counted here.
     * \return Number of spatial dimensions.
     */
    constexpr auto size() const noexcept -> size_t { return spatial_dims; }

    /*!
     * \brief Returns a pointer to the data array of the point.
     * \return A pointer to the data array.
     */
    auto data() noexcept -> pointer;

    /*!
     * \brief Returns a const pointer to the data array of the point.
     * \return A const pointer to the data array.
     */
    auto data() const noexcept -> const_pointer;

    template<typename U, typename Tag2>
    explicit operator Point<U, Tag2>() const;
};


// TYPE ALIASES

template<typename T> using Point2 = Point<T, xy_tag>;
template<typename T> using Point3 = Point<T, xyz_tag>;
template<typename T> using Point4 = Point<T, xyzw_tag>;

using Point2f = Point<float, xy_tag>;
using Point2d = Point<double, xy_tag>;
using Point2i = Point<int, xy_tag>;

using Point3f = Point<float, xyz_tag>;
using Point3d = Point<double, xyz_tag>;
using Point3i = Point<int, xyz_tag>;

using Point4f = Point<float, xyzw_tag>;
using Point4d = Point<double, xyzw_tag>;
using Point4i = Point<int, xyzw_tag>;

using Point2fm = Point<float, xym_tag>;
using Point2dm = Point<double, xym_tag>;
using Point2im = Point<int, xym_tag>;

using Point3fm = Point<float, xyzm_tag>;
using Point3dm = Point<double, xyzm_tag>;
using Point3im = Point<int, xyzm_tag>;


// IMPLEMENTATION

template<typename T, typename Tag>
Point<T, Tag>::Point()
{
    mData.fill(static_cast<T>(0));
}

template<typename T, typename Tag>
Point<T, Tag>::Point(const std::array<T, storage_size> &a)
{
    mData = a;
}

template<typename T, typename Tag>
auto Point<T, Tag>::x() const noexcept -> const_reference
{
    return mData[0];
}

template<typename T, typename Tag>
auto Point<T, Tag>::x() noexcept -> reference
{
    return mData[0];
}

template<typename T, typename Tag>
auto Point<T, Tag>::y() const noexcept -> const_reference
{
    return mData[1];
}

template<typename T, typename Tag>
auto Point<T, Tag>::y() noexcept -> reference
{
    return mData[1];
}

template<typename T, typename Tag>
auto Point<T, Tag>::z() const noexcept -> const_reference
{
    static_assert(spatial_dims >= 3, "Error: Access to Z coordinate at a point with less than 3 dimensions.");
    return mData[2];
}

template<typename T, typename Tag>
auto Point<T, Tag>::z() noexcept -> reference
{
    static_assert(spatial_dims >= 3, "Error: Access to Z coordinate at a point with less than 3 dimensions.");
    return mData[2];
}

template<typename T, typename Tag>
auto Point<T, Tag>::m() const noexcept -> const_reference
{
    static_assert(spatial_dims < storage_size, "Error: Point without measure");
    return mData[spatial_dims];
}

template<typename T, typename Tag>
auto Point<T, Tag>::m() noexcept -> reference
{
    static_assert(spatial_dims < storage_size, "Error: Point without measure");
    return mData[spatial_dims];
}

template<typename T, typename Tag>
auto Point<T, Tag>::w() const noexcept -> const_reference
{
    static_assert(spatial_dims == 4, "Error: Access to W coordinate at a point with less than 4 dimensions.");
    return mData[3];
}

template<typename T, typename Tag>
auto Point<T, Tag>::w() noexcept -> reference
{
    static_assert(spatial_dims == 4, "Error: Access to W coordinate at a point with less than 4 dimensions.");
    return mData[3];
}

template<typename T, typename Tag>
auto Point<T, Tag>::operator[](std::size_t position) noexcept -> reference
{
    return mData[position];
}

template<typename T, typename Tag>
auto Point<T, Tag>::operator[](std::size_t position) const noexcept -> const_reference
{
    return mData[position];
}

template<typename T, typename Tag>
auto Point<T, Tag>::at(size_t position) -> reference
{
    if (position >= spatial_dims)
        throw std::out_of_range("Point spatial index out of range");
    return mData.at(position);
}

template<typename T, typename Tag>
auto Point<T, Tag>::at(size_t position) const -> const_reference
{
    if (position >= spatial_dims)
        throw std::out_of_range("Point spatial index out of range");
    return mData.at(position);
}

template<typename T, typename Tag>
auto Point<T, Tag>::data() noexcept -> pointer
{
    return mData.data();
}

template<typename T, typename Tag>
auto Point<T, Tag>::data() const noexcept -> const_pointer
{
    return mData.data();
}

template<typename T, typename Tag>
template<typename U, typename Tag2>
Point<T, Tag>::operator Point<U, Tag2>() const
{
    Point<U, Tag2> result;
    constexpr size_t min_size = (spatial_dims < Tag2::spatial_dims) ? spatial_dims : Tag2::spatial_dims;
    for (size_t i = 0; i < min_size; ++i) {
        result[i] = numberCast<U>(mData[i]);
    }

    // Medida (M)
    if constexpr (Tag2::spatial_dims < Tag2::storage_size) {
        if constexpr (Tag::spatial_dims < Tag::storage_size)
            result.m() = numberCast<U>(this->m());
    }

    return result;
}


/*!
 * \brief Subtract two points to get the vector between them.
 *
 * \tparam T Coordinate type.
 * \tparam D Dimension.
 *
 * \param[in] a First point.
 * \param[in] b Second point.
 *
 * \return Vector from point b to point a.
 */
template<typename T, typename Tag>
auto operator-(const Point<T, Tag> &a, const Point<T, Tag> &b) -> typename VectorTraits<Point<T, Tag>>::difference_type
{
    typename VectorTraits<Point<T, Tag>>::difference_type v;
    for (size_t i = 0; i < v.size(); ++i)
        v[i] = a[i] - b[i];
    return v;
}

template<typename T, typename Tag>
auto operator == (const Point<T, Tag> &pt1, const Point<T, Tag> &pt2) -> bool
{
    static constexpr size_t size = point_traits<Point<T, Tag>>::storage_size;

    for (size_t i = 0; i < size; ++i)
        if (pt1[i] != pt2[i]) return false;

    return true;
}

template<typename T, typename Tag>
auto operator != (const Point<T, Tag> &pt1, const Point<T, Tag> &pt2) -> bool
{
    return !(pt1 == pt2);
}


/*! \} */

} // End namespace tl
