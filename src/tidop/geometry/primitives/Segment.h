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

/*! \file Segment.h
 * \brief Geometric segment implementation.
 *
 * This file defines the Segment class template, which represents a line segment
 * defined by two endpoints.
 * ### Classes
 * - \ref tl::Segment : Main template class for a line segment.
 * ### Type Aliases
 * - \ref tl::Segment2i, \ref tl::Segment2f, \ref tl::Segment2d : 2D integer, float, and double segments.
 * - \ref tl::Segment3i, \ref tl::Segment3f, \ref tl::Segment3d : 3D integer, float, and double segments.
 * \see tl::GeometryBase, tl::Point
 */

#pragma once

#include <utility>

#include "tidop/geometry/base/Dimension.h"
#include "tidop/geometry/base/Geometry.h"
#include "tidop/geometry/base/Traits.h"
#include "tidop/geometry/primitives/Point.h"
#include "tidop/geometry/spatial/BoundingBox.h"
#include "tidop/geometry/algorithms/spatial/Envelope.h"
#include "tidop/geometry/algorithms/measurement/Length.h"

namespace tl
{
	
/*! \addtogroup Primitives
 *  \{
 */


/*!
 * \class Segment
 * \brief A geometric segment defined by two endpoints.
 *
 * \tparam Point_t The point type (e.g., Point<double, Dimension::dim2>).
 */
template<typename Point_t>
class Segment
  : public GeometryBase<Segment<Point_t>>
{

public:

    /*! \brief Type of points stored in the segment. */
    using value_type = Point_t;

    /*! \brief Scalar type of the point coordinates. */
    using scalar = typename Point_t::value_type;

private:

    std::array<Point_t, 2> mPoints;

public:

    /*!
     * \brief Default constructor.
     *
     * Creates a segment with uninitialized endpoints.
     */
    Segment();

    /*!
     * \brief Copy constructor.
     * \param[in] segment The segment to copy.
     */
    Segment(const Segment &segment) = default;;

    /*!
     * \brief Move constructor.
     * \param[in] segment The segment to move.
     */
    Segment(Segment &&segment) noexcept = default;

    /*!
     * \brief Constructs a segment from two given points.
     * \param[in] _pt1 First endpoint.
     * \param[in] _pt2 Second endpoint.
     */
    Segment(Point_t _pt1, Point_t _pt2);

    /*!
     * \brief Constructs a segment from a starting point, angle, and length.
     * \param[in] pt Starting point.
     * \param[in] angle Angle in radians (measured counterclockwise from the x-axis).
     * \param[in] length Length of the segment.
     * \param[in] bCenter If true, the given point is the center; otherwise, it is the starting point.
     *
     * If `bCenter` is true, the segment is created symmetrically around `pt`.
     * Otherwise, the segment starts at `pt` and extends in the direction given by `angle`.
     */
    Segment(const Point_t &pt, double angle, double length, bool bCenter = true);

    /*!
     * \brief Constructor from segment with different dimension.
     */
    template<typename OtherPoint_t>
    explicit Segment(const Segment<OtherPoint_t> &other);
	
    /*!
     * \brief Copy assignment operator.
     * \param[in] segment The segment to copy.
     * \return Reference to the modified segment.
     */
    auto operator = (const Segment &segment) -> Segment & = default;

    /*!
     * \brief Move assignment operator.
     * \param[in] segment The segment to move.
     * \return Reference to the modified segment.
     */
    auto operator = (Segment &&segment) noexcept -> Segment & = default;
    
    ~Segment() override = default;
    
    /*!
     * \brief Conversion to a segment of a different point type.
     * \tparam OtherPoint_t The target point type.
     */
    template<typename OtherPoint_t> 
	operator Segment<OtherPoint_t>() const;
    
    /*!
     * \brief Access the first endpoint (non-const version).
     * \return Reference to the first endpoint.
     */
    auto pt1() noexcept -> Point_t &;

    /*!
     * \brief Access the first endpoint (const version).
     * \return Const reference to the first endpoint.
     */
    auto pt1() const noexcept -> const Point_t &;
    
    /*!
     * \brief Access the second endpoint (non-const version).
     * \return Reference to the second endpoint.
     */
    auto pt2() noexcept -> Point_t &;

    /*!
     * \brief Access the second endpoint (const version).
     * \return Const reference to the second endpoint.
     */
    auto pt2() const noexcept -> const Point_t &;
	
    /*!
     * \brief Access the start point (non-const version).
     * \return Reference to the start point (same as pt1).
     */
    auto start() noexcept -> Point_t &;

    /*!
     * \brief Access the start point (const version).
     * \return Const reference to the start point (same as pt1).
     */
    auto start() const noexcept -> const Point_t &;
    
    /*!
     * \brief Access the end point (non-const version).
     * \return Reference to the end point (same as pt2).
     */
    auto end() noexcept -> Point_t &;

    /*!
     * \brief Access the end point (const version).
     * \return Const reference to the end point (same as pt2).
     */
    auto end() const noexcept -> const Point_t &;

    /*!
     * \brief Computes the bounding box of the segment.
     */
    auto boundingBox() const;

    /*!
     * \brief Checks if the segment is empty (i.e., both endpoints are identical).
     * \return True if the segment is empty, false otherwise.
     */
    auto isEmpty() const -> bool;

    /*!
     * \brief Computes the length of the segment.
     * \return The Euclidean distance between `pt1` and `pt2`.
     */
    auto length() const -> double;
    
    /*!
     * \brief Computes the directional vector of the segment.
     * \return A point representing the vector from `pt1` to `pt2`.
     */
    auto vector() const noexcept;
};


// TYPE ALIASES FOR SEGMENT

/*! \brief 2D segment with integer coordinates. */
using Segment2i = Segment<Point<int, Dimension::dim2>>;

/*! \brief 2D segment with double coordinates. */
using Segment2d = Segment<Point<double, Dimension::dim2>>;

/*! \brief 2D segment with float coordinates. */
using Segment2f = Segment<Point<float, Dimension::dim2>>;

/*! \brief 3D segment with integer coordinates. */
using Segment3i = Segment<Point<int, Dimension::dim3>>;

/*! \brief 3D segment with double coordinates. */
using Segment3d = Segment<Point<double, Dimension::dim3>>;

/*! \brief 3D segment with float coordinates. */
using Segment3f = Segment<Point<float, Dimension::dim3>>;

// METHOD IMPLEMENTATIONS

template<typename Point_t>
Segment<Point_t>::Segment()
{
}

template<typename Point_t>
Segment<Point_t>::Segment(Point_t _pt1, Point_t _pt2)
  : mPoints{_pt1, _pt2}
{
}

template<typename Point_t>
Segment<Point_t>::Segment(const Point_t &point,
                          double angle,
                          double length,
                          bool bCenter)
{

    double a = std::cos(angle);
    double b = std::sin(angle);
    double l1 = 0;
    double l2 = length;

    if (bCenter) {
        l1 = l2 = length / 2.;
    }

    mPoints[0].x() = numberCast<scalar>(point.x() - l1 * -b);
    mPoints[0].y() = numberCast<scalar>(point.y() - l1 * a);
    mPoints[1].x() = numberCast<scalar>(point.x() + l2 * -b);
    mPoints[1].y() = numberCast<scalar>(point.y() + l2 * a);
}

template<typename Point_t>
template<typename OtherPoint_t>
Segment<Point_t>::Segment(const Segment<OtherPoint_t> &other)
{
    mPoints[0] = static_cast<Point_t>(other.pt1());
    mPoints[1] = static_cast<Point_t>(other.pt2());
}

template<typename Point_t> template<typename OtherPoint_t>
Segment<Point_t>::operator Segment<OtherPoint_t>() const
{
    using scalar2 = typename OtherPoint_t::value_type;

    Segment<OtherPoint_t> s;

    s.pt1() = static_cast<OtherPoint_t>(pt1());
    s.pt2() = static_cast<OtherPoint_t>(pt2());

    return s;
}

template<typename Point_t>
auto Segment<Point_t>::pt1() noexcept -> Point_t &
{ 
    return mPoints[0];
}

template<typename Point_t>
auto Segment<Point_t>::pt1() const noexcept -> const Point_t &
{ 
    return mPoints[0];
}

template<typename Point_t>
auto Segment<Point_t>::pt2() noexcept -> Point_t &
{ 
    return mPoints[1];
}

template<typename Point_t>
auto Segment<Point_t>::pt2() const noexcept -> const Point_t &
{ 
    return mPoints[1]; 
}

template<typename Point_t>
auto Segment<Point_t>::start() noexcept -> Point_t &
{ 
    return mPoints[0]; 
}

template<typename Point_t>
auto Segment<Point_t>::start() const noexcept -> const Point_t &
{ 
    return mPoints[0];
}

template<typename Point_t>
auto Segment<Point_t>::end() noexcept -> Point_t &
{ 
    return mPoints[1];
}

template<typename Point_t>
auto Segment<Point_t>::end() const noexcept -> const Point_t &
{ 
    return mPoints[1]; 
}

template<typename Point_t>
auto Segment<Point_t>::boundingBox() const
{
    return tl::envelope(*this);
}

template<typename Point_t>
auto Segment<Point_t>::isEmpty() const -> bool
{
    using sub_type = typename Point_t::value_type;
    return (pt1().x() == consts::zero<sub_type> &&
            pt1().y() == consts::zero<sub_type> &&
            pt2().x() == consts::zero<sub_type> &&
            pt2().y() == consts::zero<sub_type>);
    return true;
}

template<typename Point_t>
auto Segment<Point_t>::length() const -> double
{ 
    return tl::length(*this);
}

/*!
* \brief Computes the directional vector of the segment.
* \return A point representing the vector from `pt1` to `pt2`.
*/
template<typename Point_t>
auto Segment<Point_t>::vector() const noexcept
{
    return mPoints[1] - mPoints[0];
}


/*! \} */

} // End namespace tl