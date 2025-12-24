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

#include "tidop/core/base/type_conversions.h"
#include "tidop/geometry/base/Dimension.h"
#include "tidop/geometry/base/Geometry.h"
#include "tidop/geometry/base/Traits.h"
#include "tidop/math/algebra/vector.h"

#include <array>

namespace tl
{
namespace geometry
{
/*! \addtogroup GeometricEntities
 *  \{
 */

/*!
 * \class Point
 * \brief N-dimensional point.
 *
 * \tparam T Data type for the coordinates.
 * \tparam D Dimension (2, 3, or 4)
 */
template<typename T, Dimension D = Dimension::dim2>
class Point 
  : public GeometryBase<Point<T, D>>,
    public VectorBase<Point<T, D>>
{

public:

    using value_type = T;
    using reference = T &;
    using const_reference = const T &;

private:

    static_assert(is_valid_dimension(D), "Point dimension must be 2, 3, or 4");

    static constexpr std::size_t _size = dimension_value(D);
    std::array<T, _size> mData;
    

public:

    /*!
     * \brief Default constructor. Initializes X and Y to default values.
     */
    Point()
    {
        mData.fill(static_cast<T>(0));
    }

    //Point(std::initializer_list<T> list)
    //  : Geometry(Geometry::Type::Point, D) {
    //    std::copy(list.begin(), list.end(), mData.begin());
    //}

    template<typename... Args, std::enable_if_t<sizeof...(Args) == _size, int> = 0>
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
    Point(Point &&point) TL_NOEXCEPT = default;

    /*!
     * \brief Constructs a Point from an array.
     * \param[in] array Array containing two elements [x, y].
     */
    explicit Point(const std::array<T, _size> &a)
    {
        for (std::size_t i = 0; i < _size; ++i)
            mData[i] = a[i];
    }

    /*!
     * \brief Constructor from Point with different dimension.
     */
    template<Dimension OtherD>
    explicit Point(const Point<T, OtherD> &other)
    {
        static_assert(is_valid_dimension(OtherD), "Invalid source dimension");

        constexpr size_t other_size = dimension_value(OtherD);
        constexpr size_t minDim = (_size < other_size) ? _size : other_size;

        mData.fill(static_cast<T>(0));

        for (size_t i = 0; i < minDim; ++i) {
            (*this)[i] = other[i];
        }
    }

    /*! \brief Destructor. */
    ~Point() override = default;

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
    auto operator = (Point &&point) TL_NOEXCEPT -> Point & = default;

    /*!
     * \brief Assignment from Vector.
     */
    //auto operator=(const Vector<T, N> &vector) -> Point &
    //{
    //    Vector<T, N>::operator=(vector);
    //    return *this;
    //}

    /*!
     * \brief Access the x-component of the point.
     * \return A const reference to the x-component.
     * \note Only valid for points with size at least 1.
     */
    auto x() const TL_NOEXCEPT -> const_reference;

    /*!
     * \brief Access the x-component of the point (non-const version).
     * \return A reference to the x-component.
     * \note Only valid for points with size at least 1.
     */
    auto x() TL_NOEXCEPT -> reference;

    /*!
     * \brief Access the y-component of the point.
     * \return A const reference to the y-component.
     * \note Only valid for points with size at least 2.
     */
    auto y() const TL_NOEXCEPT -> const_reference;

    /*!
     * \brief Access the y-component of the point (non-const version).
     * \return A reference to the y-component.
     * \note Only valid for points with size at least 2.
     */
    auto y() TL_NOEXCEPT -> reference;

    /*!
     * \brief Access the z-component of the point.
     * \return A const reference to the z-component.
     * \note Only valid for points with size at least 3.
     */
    auto z() const TL_NOEXCEPT -> const_reference;

    /*!
     * \brief Access the z-component of the point (non-const version).
     * \return A reference to the z-component.
     * \note Only valid for points with size at least 3.
     */
    auto z() TL_NOEXCEPT -> reference;

    /*!
     * \brief Access the w-component of the point.
     * \return A const reference to the w-component.
     * \note Only valid for points with size at least 4.
     */
    auto w() const TL_NOEXCEPT -> const_reference;

    /*!
     * \brief Access the w-component of the vector (non-const version).
     * \return A reference to the w-component.
     * \note Only valid for vectors with size at least 4.
     */
    auto w() TL_NOEXCEPT -> reference;

    auto operator[](std::size_t position) noexcept -> T &{ return mData[position]; }
    auto operator[](std::size_t position) const noexcept -> const T &{ return mData[position]; }
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
     * \brief Returns the dimension of the point.
     * \return Dimension
     */
    auto size() const noexcept -> std::size_t { return _size; }

    template<typename U, Dimension D2>
    explicit operator Point<U, D2>() const;
};


// TYPE ALIASES (usando Dimension enum)

template<typename T> using Point2 = Point<T, Dimension::dim2>;
template<typename T> using Point3 = Point<T, Dimension::dim3>;
template<typename T> using Point4 = Point<T, Dimension::dim4>;

using Point2f = Point<float, Dimension::dim2>;
using Point2d = Point<double, Dimension::dim2>;
using Point2i = Point<int, Dimension::dim2>;

using Point3f = Point<float, Dimension::dim3>;
using Point3d = Point<double, Dimension::dim3>;
using Point3i = Point<int, Dimension::dim3>;


template<typename T, Dimension D>
auto Point<T, D>::x() const TL_NOEXCEPT -> const_reference
{
    return mData[0];
}

template<typename T, Dimension D>
auto Point<T, D>::x() TL_NOEXCEPT -> reference
{
    return mData[0];
}

template<typename T, Dimension D>
auto Point<T, D>::y() const TL_NOEXCEPT -> const_reference
{
    return mData[1];
}

template<typename T, Dimension D>
auto Point<T, D>::y() TL_NOEXCEPT -> reference
{
    return mData[1];
}

template<typename T, Dimension D>
auto Point<T, D>::z() const TL_NOEXCEPT -> const_reference
{
    static_assert(D >= Dimension::dim3, "Error: Access to Z coordinate at a point with less than 3 dimensions.");
    return mData[2];
}

template<typename T, Dimension D>
auto Point<T, D>::z() TL_NOEXCEPT -> reference
{
    static_assert(D >= Dimension::dim3, "Error: Access to Z coordinate at a point with less than 3 dimensions.");
    return mData[2];
}

template<typename T, Dimension D>
auto Point<T, D>::w() const TL_NOEXCEPT -> const_reference
{
    static_assert(D >= Dimension::dim4, "Error: Access to W coordinate at a point with less than 4 dimensions.");
    return mData[3];
}

template<typename T, Dimension D>
auto Point<T, D>::w() TL_NOEXCEPT -> reference
{
    static_assert(D >= Dimension::dim4, "Error: Access to W coordinate at a point with less than 4 dimensions.");
    return mData[3];
}

template<typename T, Dimension D>
auto Point<T, D>::at(size_t position) -> reference
{
    return mData.at(position);
}

template<typename T, Dimension D>
inline auto Point<T, D>::at(size_t position) const -> const_reference
{
    return mData.at(position);
}

template<typename T, Dimension D>
template<typename U, Dimension D2>
Point<T, D>::operator Point<U, D2>() const
{
    Point<U, D2> result;

    constexpr std::size_t src_size = static_cast<std::size_t>(D);
    constexpr std::size_t dst_size = static_cast<std::size_t>(D2);
    constexpr std::size_t min_size = (src_size < dst_size) ? src_size : dst_size;

    for (std::size_t i = 0; i < min_size; ++i)
        result[i] = numberCast<U>(this->at(i));

    if constexpr (dst_size > src_size) {
        for (std::size_t i = src_size; i < dst_size; ++i) {
            if constexpr (i == 3)       // w
                result[i] = consts::one<U>;
            else
                result[i] = consts::zero<U>;
        }
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
template<typename T, Dimension D>
auto operator-(const Point<T, D> &a, const Point<T, D> &b) -> typename VectorTraits<Point<T, D>>::difference_type
{
    typename VectorTraits<Point<T, D>>::difference_type v;
    for (size_t i = 0; i < v.size(); ++i)
        v[i] = a[i] - b[i];
    return v;
}

/*! \} */


}
}
