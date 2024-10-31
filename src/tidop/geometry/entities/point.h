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

#include "tidop/core/utils.h"
#include "tidop/core/common.h"
#include "tidop/geometry/entities/entity.h"
#include "tidop/math/algebra/vector.h"

namespace tl
{

/*! \addtogroup geometry
 *  \{
 */


/*!
 * \brief 2D point class
 */
template<typename T>
class Point
  : public Entity
{

public:

    /*!
     * \brief type
     */
    using value_type = T;

public:

    /*!
     * \brief Coordinate X
     */
    T x;

    /*!
     * \brief Coordinate Y
     */
    T y;

public:

    /*!
     * \brief Default constructor
     */
    Point();

    /*!
     * \brief Constructor
     * \param[in] x Coordinate x
     * \param[in] y Coordinate y
     */
    Point(T x, T y);

    /*!
     * \brief Copy constructor
     * \param[in] point Point object to be copied
     */
    Point(const Point<T> &point);

    /*!
     * \brief Move constructor
     * \param[in] point Point object to move
     */
    Point(Point<T> &&point) TL_NOEXCEPT;

    Point(const std::array<T, 2> &array);
    Point(const Vector<T, 2> &vector);

    ~Point() override = default;

    /*!
     * \brief Copy assignment operator
     * \param[in] point Point object to be copied
     */
    auto operator = (const Point<T> &point) -> Point<T>&;

    /*!
     * \brief Move assignment operator
     * \param[in] point Point object to move
     */
    auto operator = (Point<T> &&point) TL_NOEXCEPT -> Point<T>&;

    /*!
     * \brief Type conversion
     */
    template<typename T2> 
    operator Point<T2>() const;

    /*!
     * \brief Conversion to a 3D point
     */
    template<typename T2> 
    operator Point3<T2>() const;

    /*!
     * \brief Conversion to static Vector
     */
    auto vector() const -> Vector<T, 2>;

    /*!
     * \brief Conversion to dynamic Vector
     */
    auto dynVector() const -> Vector<T>;

    friend std::ostream &operator<<(std::ostream &os, const Point<T> &p)
    {
        os << p.x << " " << p.y;
        return os;
    }

    friend std::istream &operator>>(std::istream &is, Point<T> &p)
    {
        is >> p.x >> p.y;
        return is;
    }
};


/*!
 * \brief 3D point class
 *
 */
template<typename T>
class Point3
  : public Entity
{

public:

    /*!
     * \brief type
     */
    using value_type = T;

public:

    /*!
     * \brief Coordinate X
     */
    T x;

    /*!
     * \brief Coordinate Y
     */
    T y;

    /*!
     * \brief Coordinate Z
     */
    T z;

public:

    /*!
     * \brief Default constructor
     */
    Point3();

    /*!
     * \brief Constructor
     * \param[in] x Coordinate x
     * \param[in] y Coordinate y
     * \param[in] z Coordinate z
     */
    Point3(T x, T y, T z);

    /*!
     * \brief Copy constructor
     * \param[in] point Objeto Point object to be copied
     */
    Point3(const Point3<T> &point);

    /*!
     * \brief Move constructor
     * \param[in] point Objeto Point object to move
     */
    Point3(Point3<T> &&point) TL_NOEXCEPT;

    Point3(const std::array<T, 3> &v);
    Point3(const Vector<T, 3> &vector);

    ~Point3() override = default;

    /*!
     * \brief Assignment operator
     * \param[in] point Point object to be copied
     */
    auto operator = (const Point3<T> &point) -> Point3<T> &;

    /*!
     * \brief Move assignment operator
     * \param[in] point Point object to move
     */
    auto operator = (Point3<T> &&point) TL_NOEXCEPT -> Point3<T> &;

    /*!
     * \brief Type conversion
     */
    template<typename T2> operator Point3<T2>() const;

    /*!
     * \brief Conversion to a 2D point
     */
    template<typename T2> operator Point<T2>() const;

    /*!
     * \brief Conversion to static Vector
     */
    auto vector() const -> Vector<T, 3>;

    /*!
     * \brief Conversion to dynamic Vector
     */
    auto dynVector() const -> Vector<T>;

    friend std::ostream &operator<<(std::ostream &os, const Point3<T> &p)
    {
        os << p.x << " " << p.y << " " << p.z;
        return os;
    }

    friend std::istream &operator>>(std::istream &is, Point3<T> &p)
    {
        is >> p.x >> p.y >> p.z;
        return is;
    }
};






using Point2i = Point<int>;
using Point2d = Point<double>;
using Point2f = Point<float>;


template<typename T>
Point<T>::Point()
  : Entity(Type::point2d),
    x(consts::zero<T>), 
    y(consts::zero<T>)
{
}

template<typename T>
Point<T>::Point(T x, T y)
  : Entity(Type::point2d),
    x(x), 
    y(y) 
{
}

template<typename T>
Point<T>::Point(const Point<T> &point)
  : Entity(Type::point2d),
    x(point.x),
    y(point.y)
{
}

template<typename T>
Point<T>::Point(Point<T> &&point) TL_NOEXCEPT
  : Entity(std::forward<Entity>(point)),
    x(std::exchange(point.x, consts::zero<T>)),
    y(std::exchange(point.y, consts::zero<T>))
{
}

template<typename T>
Point<T>::Point(const std::array<T, 2> &v)
  : Entity(Type::point2d),
    x(v[0]), 
    y(v[1]) 
{
}

template<typename T>
Point<T>::Point(const Vector<T, 2> &vector)
  : Entity(Type::point2d),
    x(vector[0]),
    y(vector[1])
{
}

template<typename T>
auto Point<T>::operator = (const Point<T> &point) -> Point<T>&
{
    if (this != &point) {

        Entity::operator = (point);
        this->x = point.x;
        this->y = point.y;

    }

    return *this;
}

template<typename T>
auto Point<T>::operator = (Point<T> &&point) TL_NOEXCEPT -> Point<T>&
{
    if (this != &point) {

        Entity::operator = (std::forward<Entity>(point));
        this->x = std::exchange(point.x, consts::zero<T>);
        this->y = std::exchange(point.y, consts::zero<T>);

    }

    return *this;
}

template<typename T> template<typename T2>
Point<T>::operator Point<T2>() const
{
    Point<T2> point;

    point.x = numberCast<T2>(this->x);
    point.y = numberCast<T2>(this->y);

    return point;
}

template<typename T> template<typename T2>
Point<T>::operator Point3<T2>() const
{
    Point3<T2> point;

    point.x = numberCast<T2>(this->x);
    point.y = numberCast<T2>(this->y);
    point.z = numberCast<T2>(0);

    return point;
}

template<typename T>
auto Point<T>::vector() const -> Vector<T, 2>
{
    return Vector<T, 2>{this->x, this->y};
}

template<typename T>
auto Point<T>::dynVector() const -> Vector<T>
{
    return Vector<T>{this->x, this->y};
}

template<typename T> 
auto operator += (Point<T> &pt1, const Point<T> &pt2) -> Point<T>&
{
    pt1.x += pt2.x;
    pt1.y += pt2.y;

    return pt1;
}

template<typename T1, typename T2>
auto operator += (Point<T1> &pt1, const Point<T2> &pt2) -> Point<T1>&
{
  pt1 += static_cast<Point<T1>>(pt2);

  return pt1;
}

template<typename T>
auto operator -= (Point<T> &pt1, const Point<T> &pt2) -> Point<T>&
{
    pt1.x -= pt2.x;
    pt1.y -= pt2.y;

    return pt1;
}

template<typename T1, typename T2>
auto operator -= (Point<T1> &pt1, const Point<T2> &pt2) -> Point<T1>&
{
    pt1 -= static_cast<Point<T1>>(pt2);

    return pt1;
}

template<typename T1, typename T2>
auto operator *= (Point<T1> &pt, T2 b) -> Point<T1>&
{
    pt.x = numberCast<T1>(pt.x * b);
    pt.y = numberCast<T1>(pt.y * b);

    return pt;
}

template<typename T1, typename T2>
auto operator /=(Point<T1> &pt, T2 b) -> enableIfIntegral<T1,Point<T1>&>
{
    pt.x = numberCast<T1>(pt.x / static_cast<double>(b));
    pt.y = numberCast<T1>(pt.y / static_cast<double>(b));

    return pt;
}

template<typename T1, typename T2>
auto operator /=(Point<T1> &pt, T2 b) -> enableIfNotIntegral<T1,Point<T1>&>
{
    pt.x = numberCast<T1>(pt.x / b);
    pt.y = numberCast<T1>(pt.y / b);

    return pt;
}

template<typename T>
auto operator == (const Point<T> &pt1, const Point<T> &pt2) -> bool
{
    return pt1.x == pt2.x && pt1.y == pt2.y;
}

template<typename T>
auto operator != (const Point<T> &pt1, const Point<T> &pt2) -> bool
{
    return pt1.x != pt2.x || pt1.y != pt2.y;
}

template<typename T>
auto operator + (const Point<T> &pt1, const Point<T> &pt2) -> Point<T>
{
    return Point<T>(pt1.x + pt2.x, pt1.y + pt2.y);
}

template<typename T>
auto operator - (const Point<T> &pt1, const Point<T> &pt2) -> Point<T>
{
    return Point<T>(pt1.x - pt2.x, pt1.y - pt2.y);
}

template<typename T>
auto operator - (const Point<T> &pt1) -> Point<T>
{
    return Point<T>(-pt1.x, -pt1.y);
}

template<typename T1, typename T2>
auto operator * (const Point<T1> &pt, T2 b) -> Point<T1>
{
    return Point<T1>(numberCast<T1>(pt.x * b),
                     numberCast<T1>(pt.y * b));
}

template<typename T1, typename T2>
auto operator * (T1 a, const Point<T2> &b) -> Point<T2>
{
    return Point<T2>(numberCast<T2>(b.x * a),
                     numberCast<T2>(b.y * a));
}

template<typename T1, typename T2>
auto operator /(const Point<T1> &pt, T2 b) -> enableIfIntegral<T1,Point<T1>>
{
    return Point<T1>(numberCast<T1>(pt.x / static_cast<double>(b)),
                     numberCast<T1>(pt.y / static_cast<double>(b)));
}

template<typename T1, typename T2>
auto operator /(const Point<T1> &pt, T2 b) -> enableIfNotIntegral<T1,Point<T1>>
{
    return Point<T1>(numberCast<T1>(pt.x / b),
                     numberCast<T1>(pt.y / b));
}



/* ---------------------------------------------------------------------------------- */




using Point3i = Point3<int>;
using Point3d = Point3<double>;
using Point3f = Point3<float>;


template<typename T>
Point3<T>::Point3()
  : Entity(Type::point3d),
    x(consts::zero<T>), 
    y(consts::zero<T>), 
    z(consts::zero<T>)
{
}

template<typename T>
Point3<T>::Point3(T x, T y, T z)
  : Entity(Type::point3d),
    x(x), 
    y(y), 
    z(z)
{
}

template<typename T>
Point3<T>::Point3(const Point3<T> &point)
  : Entity(point),
    x(point.x),
    y(point.y),
    z(point.z)
{
}

template<typename T>
Point3<T>::Point3(Point3<T> &&point) TL_NOEXCEPT
  : Entity(std::forward<Entity>(point)),
    x(std::exchange(point.x, consts::zero<T>)),
    y(std::exchange(point.y, consts::zero<T>)),
    z(std::exchange(point.z, consts::zero<T>))
{
}

template<typename T>
Point3<T>::Point3(const std::array<T, 3> &v)
  : Entity(Type::point3d),
    x(v[0]), 
    y(v[1]),
    z(v[2])
{
}

template<typename T>
Point3<T>::Point3(const Vector<T, 3> &vector)
  : Entity(Type::point3d),
    x(vector[0]),
    y(vector[1]),
    z(vector[2])
{
}

template<typename T>
auto Point3<T>::operator = (const Point3<T> &point) -> Point3<T>&
{
    if (this != &point) {

        Entity::operator = (point);
        this->x = point.x;
        this->y = point.y;
        this->z = point.z;

    }

    return *this;
}

template<typename T>
auto Point3<T>::operator = (Point3 &&point) TL_NOEXCEPT -> Point3<T>&
{
    if (this != &point) {

        Entity::operator = (std::forward<Entity>(point));
        this->x = std::exchange(point.x, consts::zero<T>);
        this->y = std::exchange(point.y, consts::zero<T>);
        this->z = std::exchange(point.z, consts::zero<T>);

    }

    return *this;
}

template<typename T> template<typename T2>
Point3<T>::operator Point3<T2>() const
{
    Point3<T2> point;

    point.x = numberCast<T2>(this->x);
    point.y = numberCast<T2>(this->y);
    point.z = numberCast<T2>(this->z);

    return point;
}

template<typename T> template<typename T2>
Point3<T>::operator Point<T2>() const
{
    Point<T2> point;

    point.x = numberCast<T2>(this->x);
    point.y = numberCast<T2>(this->y);

    return point;
}

template<typename T>
auto Point3<T>::vector() const -> Vector<T, 3>
{
    return Vector<T, 3>{this->x, this->y, this->z};
}

template<typename T>
auto Point3<T>::dynVector() const -> Vector<T>
{
    return Vector<T>{this->x, this->y, this->z};
}



template<typename T>
auto operator += (Point3<T> &pt1, const Point3<T> &pt2) -> Point3<T> &
{
    pt1.x += pt2.x;
    pt1.y += pt2.y;
    pt1.z += pt2.z;

    return pt1;
}

template<typename T1, typename T2>
auto operator += (Point3<T1> &pt1, const Point3<T2> &pt2) -> Point3<T1> &
{
    pt1 += static_cast<Point3<T1>>(pt2);

    return pt1;
}

template<typename T>
auto operator -= (Point3<T> &pt1, const Point3<T> &pt2) -> Point3<T> &
{
    pt1.x -= pt2.x;
    pt1.y -= pt2.y;
    pt1.z -= pt2.z;

    return pt1;
}

template<typename T1, typename T2>
auto operator -= (Point3<T1> &pt1, const Point3<T2> &pt2) -> Point3<T1> &
{
    pt1 -= static_cast<Point3<T1>>(pt2);

    return pt1;
}

template<typename T1, typename T2>
auto operator *= (Point3<T1> &pt, T2 b) -> Point3<T1>&
{
    pt.x = numberCast<T1>(pt.x * b);
    pt.y = numberCast<T1>(pt.y * b);
    pt.z = numberCast<T1>(pt.z * b);

    return pt;
}

template<typename T1, typename T2>
auto operator /=(Point3<T1> &pt, T2 b) -> enableIfIntegral<T1,Point3<T1>&>
{
    pt.x = numberCast<T1>(pt.x / static_cast<double>(b));
    pt.y = numberCast<T1>(pt.y / static_cast<double>(b));
    pt.z = numberCast<T1>(pt.z / static_cast<double>(b));

    return pt;
}

template<typename T1, typename T2>
auto operator /=(Point3<T1> &pt, T2 b) -> enableIfNotIntegral<T1,Point3<T1>&>
{
    pt.x = static_cast<T1>(pt.x / static_cast<T1>(b));
    pt.y = static_cast<T1>(pt.y / static_cast<T1>(b));
    pt.z = static_cast<T1>(pt.z / static_cast<T1>(b));

    return pt;
}

template<typename T>
auto operator == (const Point3<T> &pt1,
                  const Point3<T> &pt2) -> bool
{
    return pt1.x == pt2.x &&
           pt1.y == pt2.y &&
           pt1.z == pt2.z;
}

template<typename T>
auto operator != (const Point3<T> &pt1,
                  const Point3<T> &pt2) -> bool
{
    return pt1.x != pt2.x ||
           pt1.y != pt2.y ||
           pt1.z != pt2.z;
}

template<typename T>
auto operator + (const Point3<T> &pt1,
                 const Point3<T> &pt2) -> Point3<T>
{
    return Point3<T>(pt1.x + pt2.x,
                     pt1.y + pt2.y,
                     pt1.z + pt2.z);
}

template<typename T>
auto operator - (const Point3<T> &pt1,
                 const Point3<T> &pt2) -> Point3<T>
{
    return Point3<T>(pt1.x - pt2.x,
                     pt1.y - pt2.y,
                     pt1.z - pt2.z);
}

template<typename T>
auto operator - (const Point3<T> &pt1) -> Point3<T>
{
    return Point3<T>(-pt1.x, -pt1.y, -pt1.z);
}

template<typename T1, typename T2>
auto operator * (const Point3<T1> &pt, T2 b) -> Point3<T1>
{
    return Point3<T1>(numberCast<T1>(pt.x * b),
                      numberCast<T1>(pt.y * b),
                      numberCast<T1>(pt.z * b));
}

template<typename T1, typename T2>
auto operator * (T1 a, const Point3<T2> &pt) -> Point3<T2>
{
    return Point3<T2>(numberCast<T2>(pt.x * a),
                      numberCast<T2>(pt.y * a),
                      numberCast<T2>(pt.z * a));
}

template<typename T1, typename T2>
auto operator /(const Point3<T1>& pt, T2 b) -> enableIfIntegral<T1,Point3<T1>>
{
    return Point3<T1>(numberCast<T1>(pt.x / static_cast<double>(b)),
                      numberCast<T1>(pt.y / static_cast<double>(b)),
                      numberCast<T1>(pt.z / static_cast<double>(b)));
}

template<typename T1, typename T2>
auto operator /(const Point3<T1>& pt, T2 b) -> enableIfNotIntegral<T1,Point3<T1>>
{
    return Point3<T1>(static_cast<T1>(pt.x / b),
                      static_cast<T1>(pt.y / b),
                      static_cast<T1>(pt.z / b));
}

/*! \} */ // end of geometry

} // End namespace tl
