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

#include <limits>
#include <numeric>

#include "tidop/geometry/entities/entity.h"
#include "tidop/geometry/entities/point.h"
#include "tidop/geometry/rect.h"

namespace tl
{

/*! \addtogroup geometry
 *  \{
 */

 /*!
  * \brief Shape interface
  *
  */
class Shape
{

public:

    Shape() = default;
    virtual ~Shape() = default;

    virtual double area() const = 0;

};


/*!
 * \brief Circle class
 *
 */
template<typename T>
class Circle
  : public Entity,
    public Shape
{

public:

    typedef T value_type;

public:

    /*!
     * \brief Circle center
     */
    Point<T> center;

    /*!
     * \brief Circle radius
     */
    T radius;

public:

    /*!
     * \brief Default constructor
     */
    Circle();

    /*!
     * \brief Constructor
     * \param[in] center Circle center
     * \param[in] radius Circle radius
     */
    Circle(const Point<T> &center, T radius);

    /*!
     * \brief Copy constructor
     * \param[in] circle Circle object to copy
     */
    Circle(const Circle<T> &circle);

    /*!
     * \brief Move constructor
     * \param[in] circle Circle object to move
     */
    Circle(Circle<T> &&circle) TL_NOEXCEPT;

    ~Circle() override = default;

    /*!
     * \brief Copy assignment operator
     * \param[in] circle Circle object to copy
     */
    auto operator = (const Circle<T> &circle) -> Circle<T> &;

    /*!
     * \brief Move assignment operator
     * \param[in] circle Circle object to move
     */
    auto operator = (Circle<T> &&circle) TL_NOEXCEPT -> Circle<T> &;

    /*!
     * \brief Type conversion
     */
    template<typename T2> operator Circle<T2>() const;

    /*!
     * \brief Circle area
     * \return Area
     */
    auto area() const -> double override;

    /*!
     * \brief Circumference (Distance around the perimeter of a circle)
     * \return length
     */
    auto length() const -> double;

    auto rect() const -> Rect<T>;

    bool operator==(const Circle &other) const;

};

typedef Circle<int> CircleI;
typedef Circle<double> CircleD;
typedef Circle<float> CircleF;





/*!
 * \brief Ellipse class
 *
 */
template<typename T>
class Ellipse
    : public Entity,
      public Shape
{

public:

    typedef T value_type;

public:

    /*!
     * \brief Center
     */
    Point<T> center;

    /*!
     * \brief Semi-major axis
     */
    T a;

    /*!
     * \brief Semi-minor axis
     */
    T b;

    TL_TODO("ángulo")

public:

    /*!
     * \brief Default constructor
     */
    Ellipse();

    /*!
     * \brief Constructor
     * \param[in] center Ellipse center
     * \param[in] a Semi-major axis
     * \param[in] b Semi-minor axis
     */
    Ellipse(const Point<T> &center, T a, T b);

    /*!
     * \brief Copy constructor
     * \param[in] ellipse Ellipse object to copy
     */
    Ellipse(const Ellipse<T> &ellipse);

    /*!
     * \brief Move constructor
     * \param[in] ellipse Ellipse object to move
     */
    Ellipse(Ellipse<T> &&ellipse) TL_NOEXCEPT;

    ~Ellipse() override = default;

    /*!
     * \brief Copy assignment operator
     * \param[in] ellipse Ellipse object to copy
     */
    auto operator = (const Ellipse<T> &ellipse) -> Ellipse<T> &;

    /*!
     * \brief Move assignment operator
     * \param[in] ellipse Ellipse object to move
     */
    auto operator = (Ellipse<T> &&ellipse) TL_NOEXCEPT -> Ellipse<T> &;

    /*!
     * \brief Type conversion
     */
    template<typename T2> operator Ellipse<T2>() const;

    /*!
     * \brief Ellipse area
     * \return Area
     */
    auto area() const -> double override;

    /*!
     * \brief Circumference
     * \return length
     */
    auto length() const -> double;

    auto rect() const -> Rect<T>;

    auto isInner(const Point<T> &point) const -> bool;

    auto operator==(const Ellipse &other) const -> bool;
};

typedef Ellipse<int> EllipseI;
typedef Ellipse<double> EllipseD;
typedef Ellipse<float> EllipseF;



/*!
 * \brief Triangle class
 *
 */
template<typename T>
class Triangle
  : public Entity,
    public Shape
{

public:

    typedef T value_type;

protected:

    Point<T> mPt1;
    Point<T> mPt2;
    Point<T> mPt3;

public:

    Triangle();
    Triangle(const Point<T> &pt1, const Point<T> &pt2, const Point<T> &pt3);
    Triangle(const Triangle<T> &triangle);
    Triangle(Triangle<T> &&triangle) TL_NOEXCEPT;

    ~Triangle() override = default;

    auto operator = (const Triangle<T> &circle) -> Triangle<T> &;
    auto operator = (Triangle<T> &&triangle) TL_NOEXCEPT -> Triangle<T> &;

    auto area() const -> double override;

    auto operator==(const Triangle &other) const -> bool;
};




/*!
 * \brief Square class
 *
 */
template<typename T>
class Square
  : public Entity,
    public Shape
{

public:

    /*!
     * \brief type
     */
    typedef T value_type;

public:

    Square();

    /*!
     * \brief Area
     * \return Area
     */
    auto area() const -> double override;

};


/*!
 * \brief Clase rectangulo
 *
 */
template<typename T>
class Rectangle
  : public Entity,
    public Shape
{

public:

    typedef T value_type;

public:

    Rectangle();

    auto area() const -> double override;

};





template<typename T> inline
Circle<T>::Circle()
  : Entity(Entity::Type::circle),
    center(Point<T>()),
    radius(static_cast<T>(1))
{
}

template<typename T> inline
Circle<T>::Circle(const Point<T> &center, T radius)
  : Entity(Entity::Type::circle),
    center(center),
    radius(radius)
{
}

template<typename T> inline
Circle<T>::Circle(const Circle<T> &circle)
  : Entity(Entity::Type::circle),
    center(circle.center),
    radius(circle.radius)
{
}

template<typename T> inline
Circle<T>::Circle(Circle<T> &&circle) TL_NOEXCEPT
  : Entity(std::forward<Entity>(circle)),
    center(std::move(circle.center)),
    radius(circle.radius)
{
}

template<typename T> inline
auto Circle<T>::operator = (const Circle &circle) -> Circle<T> &
{
    if (this != &circle) {
        Entity::operator = (circle);
        this->center = circle.center;
        this->radius = circle.radius;
    }

    return *this;
}

template<typename T> inline
auto Circle<T>::operator = (Circle &&circle) TL_NOEXCEPT -> Circle<T> &
{
    if (this != &circle) {
        Entity::operator = (std::forward<Entity>(circle));
        this->center = std::move(circle.center);
        this->radius = circle.radius;
    }

    return *this;
}

template<typename T> template<typename T2> inline
Circle<T>::operator Circle<T2>() const
{
    return Circle<T2>(this->center, numberCast<T2>(this->radius));
}

template<typename T> inline
auto Circle<T>::area() const -> double
{
    return consts::pi<double> * radius * radius;
}

template<typename T> inline
auto Circle<T>::length() const -> double
{
    return consts::two_pi<double> * radius;
}

template<typename T> inline 
auto Circle<T>::rect() const -> Rect<T>
{
    return Rect<T>(Point<T>(center.x - radius, center.y - radius), radius * 2., radius * 2.);
}

template<typename T> inline 
bool Circle<T>::operator==(const Circle &other) const
{
    return center == other.center && radius == other.radius;
}




template<typename T> inline
Ellipse<T>::Ellipse()
  : Entity(Entity::Type::ellipse),
    center(Point<T>()),
    a(static_cast<T>(1)),
    b(static_cast<T>(1))
{
}

template<typename T> inline
Ellipse<T>::Ellipse(const Point<T> &center, T a, T b)
  : Entity(Entity::Type::ellipse),
    center(center),
    a(a),
    b(b)
{
    if (b > a) {
        T aux = b;
        b = a;
        a = aux;
    }
}

template<typename T> inline
Ellipse<T>::Ellipse(const Ellipse<T> &ellipse)
  : Entity(ellipse),
    center(ellipse.center),
    a(ellipse.a),
    b(ellipse.b)
{
}

template<typename T> inline
Ellipse<T>::Ellipse(Ellipse<T> &&ellipse) TL_NOEXCEPT
  : Entity(std::forward<Entity>(ellipse)),
    center(std::move(ellipse.center)),
    a(ellipse.a),
    b(ellipse.b)
{
}

template<typename T> inline
auto Ellipse<T>::operator = (const Ellipse &ellipse) -> Ellipse<T> &
{
    if (this != &ellipse) {
        Entity::operator = (ellipse);
        this->center = ellipse.center;
        this->a = ellipse.a;
        this->b = ellipse.b;
    }

    return *this;
}

template<typename T> inline
auto Ellipse<T>::operator = (Ellipse &&ellipse) TL_NOEXCEPT -> Ellipse<T> &
{
    if (this != &ellipse) {
        Entity::operator = (std::forward<Entity>(ellipse));
        this->center = std::move(ellipse.center);
        this->a = ellipse.a;
        this->b = ellipse.b;
    }

    return *this;
}

template<typename T> template<typename T2> inline
Ellipse<T>::operator Ellipse<T2>() const
{
    return Ellipse<T2>(this->center,
                       numberCast<T2>(this->a),
                       numberCast<T2>(this->b));
}

template<typename T> inline
auto Ellipse<T>::area() const -> double
{
    return consts::pi<double> * a * b;
}

template<typename T> inline
auto Ellipse<T>::length() const -> double
{
    return consts::pi<double> * (3 * (a + b) - sqrt((3 * a + b)*(a + 3 * b)));
}

template<typename T> inline 
auto Ellipse<T>::rect() const -> Rect<T>
{
    return Rect<T>(tl::Point<T>(center.x - a, center.y - b), a * 2., b * 2.);
}

template<typename T>
inline auto Ellipse<T>::isInner(const Point<T> &point) const -> bool
{
    if (!this->rect().contains(point)) return false;
    
    T x = (point.x - this->center.x);
    T y = (point.y - this->center.y);
    T check = (x * x) / (this->a * this->a) + (y * y) / (this->b * this->b);
    if (check <= 1) return true; // the point is inner
    //else if (check == 1) return true; // the point is on the boundary of the region
    else return false;
}

template<typename T>
inline auto Ellipse<T>::operator==(const Ellipse &other) const -> bool
{
    return center == other.center && a == other.a && b == other.b;
}



template<typename T> inline
Triangle<T>::Triangle()
  : Entity(Entity::Type::triangle),
    mPt1(),
    mPt2(),
    mPt3()
{
}

template<typename T> inline 
Triangle<T>::Triangle(const Point<T> &pt1, 
                      const Point<T> &pt2, 
                      const Point<T> &pt3)
  : Entity(Entity::Type::triangle),
    mPt1(pt1),
    mPt2(pt2),
    mPt3(pt3)
{
}

template<typename T> inline
Triangle<T>::Triangle(const Triangle<T> &triangle)
  : Entity(triangle),
    mPt1(triangle.pt1),
    mPt2(triangle.pt2),
    mPt3(triangle.pt3)
{
}

template<typename T> inline
Triangle<T>::Triangle(Triangle<T> &&triangle) TL_NOEXCEPT
  : Entity(std::forward<Entity>(triangle)),
    mPt1(std::move(triangle.mPt1)),
    mPt2(std::move(triangle.mPt2)),
    mPt3(std::move(triangle.mPt3))
{
}

template<typename T> inline
auto Triangle<T>::operator = (const Triangle &triangle) -> Triangle<T> &
{
    if (this != &triangle) {
        Entity::operator = (triangle);
        this->mPt1 = triangle.mPt1;
        this->mPt2 = triangle.mPt2;
        this->mPt3 = triangle.mPt3;
    }

    return *this;
}

template<typename T> inline
auto Triangle<T>::operator = (Triangle &&triangle) TL_NOEXCEPT -> Triangle<T> &
{
    if (this != &triangle) {
        Entity::operator = (std::forward<Entity>(triangle));
        this->mPt1 = std::move(triangle.mPt1);
        this->mPt2 = std::move(triangle.mPt2);
        this->mPt3 = std::move(triangle.mPt3);
    }

    return *this;
}

template<typename T> inline
auto Triangle<T>::area() const -> double
{
    return 0;
}

template<typename T> inline
auto Triangle<T>::operator==(const Triangle &other) const -> bool
{
    return mPt1 == other.mPt1 && mPt2 == other.mPt2 && mPt3 == other.mPt3;
}





template<typename T> inline
Square<T>::Square()
{
}


template<typename T> inline
auto Square<T>::area() const -> double
{
    return 0;
}





template<typename T> inline
Rectangle<T>::Rectangle()
{
}


template<typename T> inline
auto Rectangle<T>::area() const -> double
{
    return 0;
}


/*! \} */ // end of geometry

} // End namespace tl
