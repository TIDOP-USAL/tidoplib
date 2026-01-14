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
	
/*! \addtogroup GeometricEntities
 *  \{
 */


/*!
 * \class Segment
 * \brief A geometric segment defined by two endpoints.
 * \tparam Point_t The point type (e.g., Point<double, Dimension::dim2>)
 */
template<typename Point_t>
class Segment
  : public GeometryBase<Segment<Point_t>>
{

public:

    using value_type = Point_t;
    using scalar = typename Point_t::value_type;

private:

    std::array<Point_t, 2> mPoints;

public:

    /*!
     * \brief Default constructor.
     *
     * Creates an empty segment with uninitialized points.
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
    explicit Segment(const Segment<OtherPoint_t> &other)
    {
        mPoints[0] = static_cast<Point_t>(other.pt1());
        mPoints[1] = static_cast<Point_t>(other.pt2());
    }
	
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
    
    // Accessors with new names (compatible with old interface)
    auto pt1() noexcept -> Point_t& { return mPoints[0]; }
    auto pt1() const noexcept -> const Point_t& { return mPoints[0]; }
    
    auto pt2() noexcept -> Point_t & { return mPoints[1]; }
    auto pt2() const noexcept -> const Point_t & { return mPoints[1]; }
	
    auto start() noexcept -> Point_t & { return mPoints[0]; }
    auto start() const noexcept -> const Point_t & { return mPoints[0]; }
    
    auto end() noexcept -> Point_t & { return mPoints[1]; }
    auto end() const noexcept -> const Point_t & { return mPoints[1]; }


    /*!
     * \brief Computes the angle of the segment relative to the x-axis.
     * \return The angle in radians.
     */
    //auto angleOX() const -> double;

    /*!
     * \brief Computes the angle of the segment relative to the y-axis.
     * \return The angle in radians.
     */
    //auto angleOY() const -> double;
    
    /*!
     * \brief Computes the bounding box of the segment.
     * \return The bounding box as a `Window<Point_t>`.
     */
    //auto window() const -> Window<Point_t>;
    auto boundingBox() const
    {
        return tl::envelope(*this);
    }

    /**
     * \brief Alias for boundingBox() to follow OGC/GIS standards.
     */
    //auto envelope() const
    //{
    //    return boundingBox();
    //}

    /*!
     * \brief Checks if the segment is empty (i.e., both endpoints are identical).
     * \return True if the segment is empty, false otherwise.
     */
    auto isEmpty() const -> bool;

    /*!
     * \brief Checks if two segments are close to each other within a given distance.
     * \param[in] l2 The segment to compare against.
     * \param[in] dist Maximum allowed separation distance.
     * \return True if the segments are closer than `dist`, false otherwise.
     */
    //auto isNear(const Segment<Point_t> &l2, double dist = 10.) const -> bool;
    
    /*!
     * \brief Checks if the segment is parallel to another segment.
     * \param[in] l2 The segment to compare against.
     * \param[in] tol Angular tolerance in radians.
     *        If `tol == 0`, the segments must be exactly parallel.
     *        If `tol > 0`, segments with an angular difference less than `tol` are considered parallel.
     * \return True if the segments are parallel, false otherwise.
     */
    //auto isParallel(const Segment<Point_t> &l2, double tol = 0.) const -> bool;
    
    /*!
     * \brief Computes the length of the segment.
     * \return The Euclidean distance between `pt1` and `pt2`.
     */
    auto length() const -> double;
    
    /*!
     * \brief Computes the directional vector of the segment.
     * \return A point representing the vector from `pt1` to `pt2`.
     */
    auto vector() const noexcept
    {
        return mPoints[1] - mPoints[0];
    }
    
    /*!
     * \brief Divides the segment into `n` equal parts.
     * \param[in] n Number of partitions.
     * \return A vector containing `n` smaller segments.
     *
     * Each sub-segment is of equal length and follows the same direction as the original segment.
     */
    //auto split(size_t n) const -> std::vector<Segment<Point_t>>;
	
    //void accept(GeometryVisitor& visitor) override 
    //{
    //    visitor.visit(*this);
    //}
};


using Segment2i = Segment<Point<int, Dimension::dim2>>;
using Segment2d = Segment<Point<double, Dimension::dim2>>;
using Segment2f = Segment<Point<float, Dimension::dim2>>;
using Line = Segment2i;
using Segment3i = Segment<Point<int, Dimension::dim3>>;
using Segment3d = Segment<Point<double, Dimension::dim3>>;
using Segment3f = Segment<Point<float, Dimension::dim3>>;


// Segment implementation

template<typename Point_t>
Segment<Point_t>::Segment()
{
}

//template<typename Point_t>
//Segment<Point_t>::Segment(const Segment &segment)
//  : pt1(segment.pt1), 
//    pt2(segment.pt2) 
//{
//}
//
//template<typename Point_t>
//Segment<Point_t>::Segment(Segment &&segment) TL_NOEXCEPT
//  : pt1(std::move(segment.pt1)), 
//    pt2(std::move(segment.pt2)) 
//{
//}

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

template<typename Point_t> template<typename OtherPoint_t>
Segment<Point_t>::operator Segment<OtherPoint_t>() const
{
    using scalar2 = typename OtherPoint_t::value_type;

    Segment<OtherPoint_t> s;

    s.pt1() = static_cast<OtherPoint_t>(pt1());
    s.pt2() = static_cast<OtherPoint_t>(pt2());

    return s;
}

//template<typename Point_t>
//auto Segment<Point_t>::angleOX() const -> double
//{
//    double angle = 0.0;
//    if (pt1 != pt2) {
//        angle = vectorAngleOX(vector());
//    }
//
//    return angle;
//}
//
//template<typename Point_t>
//auto Segment<Point_t>::angleOY() const -> double
//{
//    double angle = 0.0;
//    if (pt1 != pt2) {
//        angle = vectorAngleOY(vector());
//    }
//
//    return angle;
//}

//template<typename Point_t>
//auto Segment<Point_t>::window() const -> Window<Point_t>
//{
//    Window<Point_t> w(pt1, pt2);
//    w.normalized();
//    return w;
//}

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

//template<typename Point_t>
//auto Segment<Point_t>::isNear(const Segment<Point_t> &l2, double dist) const -> bool
//{
//    double dist1 = minDistanceSegments(*this, l2);
//    return (dist1 <= dist);
//}
//
//template<typename Point_t>
//auto Segment<Point_t>::isParallel(const Segment<Point_t> &l2, double tol) const -> bool
//{
//    return (std::abs(angleOX() - l2.angleOX()) < tol);
//}

template<typename Point_t>
auto Segment<Point_t>::length() const -> double
{ 
    return tl::length(*this);
}


//template<typename Point_t>
//auto Segment<Point_t>::split(size_t n) const -> std::vector<Segment<Point_t>> 
//{
//    std::vector<Segment<Point_t>> segments;
//
//    Point_t point1 = pt1;
//    Point_t point2;
//    for (size_t i = 1; i <= n; i++) {
//        point2 = pt1 * (1 - i / static_cast<double>(n)) + pt2 * i / static_cast<double>(n);
//        segments.emplace_back(point1, point2);
//        point1 = point2;
//    }
//
//    return segments;
//}


/*! \} */

} // End namespace tl