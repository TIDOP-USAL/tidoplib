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

#include "tidop/core/utils.h"
#include "tidop/math/mathutils.h"
#include "tidop/geometry/entities/point.h"
#include "tidop/geometry/entities/entity.h"
#include "tidop/geometry/entities/window.h"
#include "tidop/geometry/algorithms.h"

namespace tl
{

/*! \addtogroup geometry
 *  \{
 */


/*!
 * \brief 2D segment class
 *
 */
template<typename Point_t>
class Segment
  : public Entity
{

public:

    using value_type = Point_t;
    using scalar = typename Point_t::value_type;

public:

    Point_t pt1;
    Point_t pt2;

public:

    Segment();
    Segment(const Segment &segment);
    Segment(Segment &&segment) TL_NOEXCEPT;
    Segment(Point_t _pt1, Point_t _pt2);
    Segment(const Point_t &pt, double angle, double length, bool bCenter = true);

    /*!
     * \brief Copy assignment operator
     */
    auto operator = (const Segment &segment) -> Segment &;

    /*!
     * \brief Move assignment operator
     */
    auto operator = (Segment &&segment) TL_NOEXCEPT -> Segment &;
    
    ~Segment() override = default;
    
    /*!
     * \brief Conversion to a segment of a different type
     */
    template<typename Point_t2> operator Segment<Point_t2>() const;
    
    /*!
     * \brief Angle measured with respect to the x-axis.
     * \return Angle in radians
     */
    auto angleOX() const -> double;
    
    /*!
     * \brief Angle measured with respect to the y-axis.
     * \return Angle in radians
     */
    auto angleOY() const -> double;
    
    /*!
     * \brief Window
     */
    auto window() const -> Window<Point_t>;
    
    /*!
     * \brief Check if the segment is empty
     */
    auto isEmpty() const -> bool;
    
    /*!
     * \brief Checks if two segments are close to each other
     * \param[in] l2 Segment to which it is compared
     * \param[in] dist Maximum separation distance
     */
    auto isNear(const Segment<Point_t> &l2, double dist = 10.) const -> bool;
    
    /*!
     * \brief Check if it is parallel to another segment
     * \param[in] l2 Segment to which it is compared
     * \param[in] tol Angular tolerance. If it is 0, the segments must be parallel.
     *  If it has another value, lines below this tolerance are accepted.
     */
    auto isParallel(const Segment<Point_t> &l2, double tol = 0.) const -> bool;
    
    /*!
     * \brief Segment length
     */
    auto length() const -> double;
    
    /*!
     * \brief Vector
     */
    auto vector() const -> Point_t;
    
    /*!
     * \brief divides a segment into n parts
     * \param[in] n number of partitions
     * \return Segments
     */
    auto split(size_t n) const -> std::vector<Segment<Point_t>>;
};


using SegmentI = Segment<Point<int>>;
using SegmentD = Segment<Point<double>>;
using SegmentF = Segment<Point<float>>;
using Line = SegmentI;




/*!
 * \brief 3D segment class
 *
 */
template<typename Point3_t>
class Segment3D
    : public Entity
{

public:

    using value_type = Point3_t;

public:

    Point3_t pt1;

    Point3_t pt2;

public:

    Segment3D();
    Segment3D(const Segment3D &segment);
    Segment3D(Segment3D &&segment) TL_NOEXCEPT;
    Segment3D(const Point3_t &_pt1, const Point3_t &_pt2);

    ~Segment3D() override = default;

    /*!
     * \brief Copy assignment operator
     */
    auto operator =(const Segment3D& segment) -> Segment3D&;

    /*!
     * \brief Move assignment operator
     */
    auto operator =(Segment3D&& segment) TL_NOEXCEPT -> Segment3D&;

    /*!
     * \brief Conversion to a segment of a different type
     */
    template<typename Point3_t2> operator Segment3D<Point3_t2>() const;

    /*!
     * \brief Bounding Box
     */
    auto boundingBox() const -> BoundingBox<Point3_t>;

    /*!
     * \brief Check if the segment is empty
     */
    auto isEmpty() const -> bool;

    /*!
     * \brief Segment length
     */
    auto length() const -> double;

    /*!
     * \brief Vector
     */
    auto vector() const -> Point3_t;
};






// Segment implementation

template<typename Point_t>
Segment<Point_t>::Segment()
  : Entity(Type::segment2d),
    pt1(), 
    pt2()
{
}

template<typename Point_t>
Segment<Point_t>::Segment(const Segment &segment)
  : Entity(Type::segment2d),
    pt1(segment.pt1), 
    pt2(segment.pt2) 
{
}

template<typename Point_t>
Segment<Point_t>::Segment(Segment &&segment) TL_NOEXCEPT
  : Entity(std::forward<Entity>(segment)), 
    pt1(std::move(segment.pt1)), 
    pt2(std::move(segment.pt2)) 
{
}

template<typename Point_t>
Segment<Point_t>::Segment(Point_t _pt1, Point_t _pt2)
  : Entity(Type::segment2d),
    pt1(std::move(_pt1)), 
    pt2(std::move(_pt2))
{
}

template<typename Point_t>
Segment<Point_t>::Segment(const Point_t &pt,
                          double angle,
                          double length,
                          bool bCenter)
  : Entity(Type::segment2d)
{

    double a = cos(angle);
    double b = sin(angle);
    double l1 = 0;
    double l2 = length;

    if (bCenter) {
        l1 = l2 = length / 2.;
    }

    pt1.x = numberCast<scalar>(pt.x - l1 * -b);
    pt1.y = numberCast<scalar>(pt.y - l1 * a);
    pt2.x = numberCast<scalar>(pt.x + l2 * -b);
    pt2.y = numberCast<scalar>(pt.y + l2 * a);
}


template<typename Point_t>
auto Segment<Point_t>::operator = (const Segment &segment) -> Segment<Point_t> &
{
    if (this != &segment) {
        Entity::operator = (segment);
        this->pt1 = segment.pt1;
        this->pt2 = segment.pt2;
    }

    return *this;
}

template<typename Point_t>
auto Segment<Point_t>::operator = (Segment &&segment) TL_NOEXCEPT -> Segment<Point_t> &
{
    if (this != &segment) {
        Entity::operator = (std::forward<Entity>(segment));
        this->pt1 = std::move(segment.pt1);
        this->pt2 = std::move(segment.pt2);
    }

    return *this;
}

template<typename Point_t> template<typename Point_t2>
Segment<Point_t>::operator Segment<Point_t2>() const
{
    using scalar2 = typename Point_t2::value_type;

    Segment<Point_t2> s;

    s.pt1.x = numberCast<scalar2>(pt1.x);
    s.pt1.y = numberCast<scalar2>(pt1.y);
    s.pt2.x = numberCast<scalar2>(pt2.x);
    s.pt2.y = numberCast<scalar2>(pt2.y);

    return s;
}

template<typename Point_t>
auto Segment<Point_t>::angleOX() const -> double
{
    double angle = 0.0;
    if (pt1 != pt2) {
        angle = vectorAngleOX(vector());
    }

    return angle;
}

template<typename Point_t>
auto Segment<Point_t>::angleOY() const -> double
{
    double angle = 0.0;
    if (pt1 != pt2) {
        angle = vectorAngleOY(vector());
    }

    return angle;
}

template<typename Point_t>
auto Segment<Point_t>::window() const -> Window<Point_t>
{
    Window<Point_t> w(pt1, pt2);
    w.normalized();
    return w;
}

template<typename Point_t>
auto Segment<Point_t>::isEmpty() const -> bool
{
    using sub_type = typename Point_t::value_type;
    return (pt1.x == consts::zero<sub_type> &&
            pt1.y == consts::zero<sub_type> &&
            pt2.x == consts::zero<sub_type> &&
            pt2.y == consts::zero<sub_type>);
}

template<typename Point_t>
auto Segment<Point_t>::isNear(const Segment<Point_t> &l2, double dist) const -> bool
{
    double dist1 = minDistanceSegments(*this, l2);
    return (dist1 <= dist);
}

template<typename Point_t>
auto Segment<Point_t>::isParallel(const Segment<Point_t> &l2, double tol) const -> bool
{
    return (std::abs(angleOX() - l2.angleOX()) < tol);
}

template<typename Point_t>
auto Segment<Point_t>::length() const -> double
{ 
    return distance(pt1, pt2); 
}

template<typename Point_t>
auto Segment<Point_t>::vector() const -> Point_t
{ 
    return (pt2 - pt1); 
}

template<typename Point_t>
auto Segment<Point_t>::split(size_t n) const -> std::vector<Segment<Point_t>> 
{
    std::vector<Segment<Point_t>> segments;

    Point_t point1 = pt1;
    Point_t point2;
    for (size_t i = 1; i <= n; i++) {
        point2 = pt1 * (1 - i / static_cast<double>(n)) + pt2 * i / static_cast<double>(n);
        segments.emplace_back(point1, point2);
        point1 = point2;
    }

    return segments;
}




// Segment3D implementation

template<typename Point3_t>
Segment3D<Point3_t>::Segment3D()
  : Entity(Type::segment3d),
    pt1(Point3_t()), 
    pt2(Point3_t()) 
{
}

template<typename T>
Segment3D<T>::Segment3D(const Segment3D &segment)
: Entity(segment), 
  pt1(segment.pt1), 
  pt2(segment.pt2) 
{
}

template<typename T>
Segment3D<T>::Segment3D(Segment3D &&segment) TL_NOEXCEPT
: Entity(std::forward<Entity>(segment)), 
  pt1(std::move(segment.pt1)), 
  pt2(std::move(segment.pt2)) 
{
}

template<typename Point3_t>
Segment3D<Point3_t>::Segment3D(const Point3_t &_pt1, const Point3_t &_pt2)
  : Entity(Type::segment3d),
    pt1(_pt1), 
    pt2(_pt2) 
{
}

template<typename Point3_t>
Segment3D<Point3_t> &Segment3D<Point3_t>::operator = (const Segment3D &segment)
{
    if (this != &segment) {
        Entity::operator=(segment);
        this->pt1 = segment.pt1;
        this->pt2 = segment.pt2;
    }
    return *this;
}

template<typename Point3_t>
Segment3D<Point3_t> &Segment3D<Point3_t>::operator = (Segment3D &&segment) TL_NOEXCEPT
{
    if (this != &segment) {
        Entity::operator=(std::forward<Entity>(segment));
        this->pt1 = std::move(segment.pt1);
        this->pt2 = std::move(segment.pt2);
    }
    return *this;
}

template<typename Point3_t> template<typename Point3_t2>
Segment3D<Point3_t>::operator Segment3D<Point3_t2>() const
{
    Segment3D<Point3_t2> s;

    using point_type = typename Point3_t2::value_type;

    s.pt1.x = numberCast<point_type>(pt1.x);
    s.pt1.y = numberCast<point_type>(pt1.y);
    s.pt1.z = numberCast<point_type>(pt1.z);
    s.pt2.x = numberCast<point_type>(pt2.x);
    s.pt2.y = numberCast<point_type>(pt2.y);
    s.pt2.z = numberCast<point_type>(pt2.z);

    return s;
}

template<typename Point3_t>
BoundingBox<Point3_t> Segment3D<Point3_t>::boundingBox() const
{
    return BoundingBox<Point3_t>(pt1, pt2);
}

template<typename Point3_t>
bool Segment3D<Point3_t>::isEmpty() const
{
    return (pt1.x == static_cast<typename Point3_t::value_type>(0) &&
            pt1.y == static_cast<typename Point3_t::value_type>(0) &&
            pt2.x == static_cast<typename Point3_t::value_type>(0) &&
            pt2.y == static_cast<typename Point3_t::value_type>(0));
}

//template<typename T> inline
//bool Segment<T>::isNear(const Segment<T> &l2, double dist) const
//{
//  double dist1 = minDistanceSegments(*this, l2);
//  return (dist1 < dist);
//}

template<typename Point3_t>
double Segment3D<Point3_t>::length() const 
{ 
    return distance3D(pt1, pt2); 
}

template<typename Point3_t>
Point3_t Segment3D<Point3_t>::vector() const 
{ 
    return (pt2 - pt1); 
}

using Segment3dI = Segment3D<Point3<int> >;
using Segment3dD = Segment3D<Point3<double> >;
using Segment3dF = Segment3D<Point3<float> >;



// Mover a algorithms

/*!
 * \brief GroupLines class
 */
class TL_EXPORT GroupLines
{

private:

    std::vector<Line> linesgroup;
    WindowI bbox;

public:

    GroupLines();
    GroupLines(const std::vector<Line> &lines);

    void add(const Line &line);
    double angleMean() const;
    void deleteLine(int id);
    WindowI window() const { return bbox; }
    size_t size() const { return linesgroup.size(); }
    const Line &operator[](size_t id) const { return linesgroup[id]; }
    Line &operator[](size_t id) { return linesgroup[id]; }
    const std::vector<Line> &getLines() const { return linesgroup; }

};


/*! \} */ // end of geometry

} // End namespace tl
