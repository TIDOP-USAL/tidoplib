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

#include <numeric>

#include "tidop/geometry/entities/entity.h"
#include "tidop/geometry/entities/point.h"
#include "tidop/geometry/rect.h"

namespace tl
{

/*! \addtogroup GeometricEntities
 *  \{
 */


/*!
 * \brief Shape interface
 *
 * Base class for geometric shapes, enforcing area computation.
 */
class TL_EXPORT Shape
{

public:

    /*!
     * \brief Default constructor
     */
    Shape() = default;

    /*!
     * \brief Default virtual destructor
     */
    virtual ~Shape() = default;

    /*!
     * \brief Compute the area of the shape
     * \return The computed area
     */
    virtual auto area() const -> double = 0;

};


/*!
 * \brief Circle class
 *
 * Represents a circle with a center and a radius.
 */
template<typename T>
class Circle
  : public Entity,
    public Shape
{

public:

    using value_type = T;

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
     * \brief Constructs a circle with a given center and radius
     * \param[in] center The center point of the circle
     * \param[in] radius The radius of the circle
     */
    explicit Circle(const Point<T> &center, T radius);

    /*!
     * \brief Copy constructor
     * \param[in] circle The circle to copy
     */
    Circle(const Circle<T> &circle);

    /*!
     * \brief Move constructor
     * \param[in] circle The circle to move
     */
    Circle(Circle<T> &&circle) TL_NOEXCEPT;

    /*!
     * \brief Destructor
     */
    ~Circle() override = default;

    /*!
     * \brief Copy assignment operator
     * \param[in] circle The circle to copy
     * \return Reference to the assigned object
     */
    auto operator=(const Circle<T> &circle) -> Circle<T> &;

    /*!
     * \brief Move assignment operator
     * \param[in] circle The circle to move
     * \return Reference to the assigned object
     */
    auto operator=(Circle<T> &&circle) TL_NOEXCEPT -> Circle<T> &;

    /*!
     * \brief Converts the circle to a different type
     * \tparam T2 The target type for conversion
     * \return A new circle with the converted type
     */
    template<typename T2> operator Circle<T2>() const;

    /*!
     * \brief Computes the area of the circle
     * \return The computed area
     */
    auto area() const -> double override;

    /*!
     * \brief Computes the circumference (perimeter) of the circle
     * \return The computed circumference
     */
    auto length() const -> double;

    /*!
     * \brief Computes the bounding rectangle of the circle
     * \return A rectangle that bounds the circle
     */
    auto rect() const->Rect<T>;

    /*!
     * \brief Equality comparison operator
     * \param[in] other The circle to compare with
     * \return True if circles are equal, false otherwise
     */
    bool operator==(const Circle &other) const;

};

typedef Circle<int> CircleI;
typedef Circle<double> CircleD;
typedef Circle<float> CircleF;





/*!
 * \brief Ellipse class
 *
 * Represents an ellipse defined by a center, semi-major axis, semi-minor axis, and optional rotation.
 */
template<typename T>
class Ellipse
    : public Entity,
      public Shape
{

public:

    using value_type = T;

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

    /*!
     * \brief Destructor
     */
    ~Ellipse() override = default;

    /*!
     * \brief Copy assignment operator
     * \param[in] ellipse Ellipse object to copy
     * \return Reference to the assigned object
     */
    auto operator=(const Ellipse<T> &ellipse) -> Ellipse<T> &;

    /*!
     * \brief Move assignment operator
     * \param[in] ellipse Ellipse object to move
     * \return Reference to the assigned object
     */
    auto operator=(Ellipse<T> &&ellipse) TL_NOEXCEPT -> Ellipse<T> &;

    /*!
     * \brief Converts the ellipse to a different type
     * \tparam T2 The target type for conversion
     * \return A new ellipse with the converted type
     */
    template<typename T2> operator Ellipse<T2>() const;

    /*!
     * \brief Computes the area of the ellipse
     * \return The computed area
     */
    auto area() const -> double override;

    /*!
     * \brief Computes the approximate circumference of the ellipse
     * \return The computed circumference
     */
    auto length() const -> double;

    /*!
     * \brief Computes the bounding rectangle of the ellipse (axis-aligned)
     * \return A rectangle that bounds the ellipse
     */
    auto rect() const -> Rect<T>;

    /*!
     * \brief Checks if a point is inside the ellipse
     * \param[in] point The point to check
     * \return True if the point is inside, false otherwise
     */
    auto isInner(const Point<T> &point) const -> bool;

    /*!
     * \brief Equality comparison operator
     * \param[in] other The ellipse to compare with
     * \return True if ellipses are equal, false otherwise
     */
    auto operator==(const Ellipse &other) const -> bool;
};

typedef Ellipse<int> EllipseI;
typedef Ellipse<double> EllipseD;
typedef Ellipse<float> EllipseF;



/*!
 * \brief Triangle class
 *
 * Represents a 2D triangle defined by three points.
 */
template<typename T>
class Triangle
  : public Entity,
    public Shape
{

public:

    using value_type = T;

protected:

    Point<T> mPt1;
    Point<T> mPt2;
    Point<T> mPt3;

public:

    /*!
     * \brief Default constructor
     */
    Triangle();

    /*!
     * \brief Constructor with three points
     * \param[in] pt1 First vertex
     * \param[in] pt2 Second vertex
     * \param[in] pt3 Third vertex
     */
    explicit Triangle(const Point<T> &pt1, const Point<T> &pt2, const Point<T> &pt3);

    /*!
     * \brief Copy constructor
     * \param[in] triangle Triangle object to copy
     */
    Triangle(const Triangle<T> &triangle);

    /*!
     * \brief Move constructor
     * \param[in] triangle Triangle object to move
     */
    Triangle(Triangle<T> &&triangle) TL_NOEXCEPT;

    /*!
     * \brief Destructor
     */
    ~Triangle() override = default;

    /*!
     * \brief Copy assignment operator
     * \param[in] triangle Triangle object to copy
     * \return Reference to the assigned object
     */
    auto operator=(const Triangle<T> &triangle)->Triangle<T> &;

    /*!
     * \brief Move assignment operator
     * \param[in] triangle Triangle object to move
     * \return Reference to the assigned object
     */
    auto operator=(Triangle<T> &&triangle) TL_NOEXCEPT->Triangle<T> &;

    /*!
     * \brief Computes the area of the triangle using the determinant formula
     * \return The computed area
     */
    auto area() const -> double override;

    /*!
     * \brief Computes the perimeter of the triangle
     * \return The sum of the three side lengths
     */
    auto perimeter() const -> double;

    /*!
     * \brief Checks if the triangle is valid (not degenerate)
     * \return True if the triangle is valid, false otherwise
     */
    auto isValid() const -> bool;

    /*!
     * \brief Computes the centroid (barycenter) of the triangle
     * \return The centroid as a `Point<T>`
     */
    auto centroid() const -> Point<T>;

    /*!
     * \brief Equality comparison operator
     * \param[in] other The triangle to compare with
     * \return True if triangles are equal, false otherwise
     */
    auto operator==(const Triangle &other) const -> bool;

    auto pt1() const -> Point<T> { return mPt1; }
    auto pt2() const -> Point<T> { return mPt2; }
    auto pt3() const -> Point<T> { return mPt3; }
};




/*!
 * \brief Square class
 *
 * The `Square` class represents a square, a 2D geometric shape where all sides are of equal length.
 * It inherits from `Entity` and `Shape`, providing common geometric properties and the ability to calculate its area.
 */
template<typename T>
class Square
  : public Entity,
    public Shape
{

public:

    using value_type = T;

private:

    T side;

public:

    /*!
     * \brief Default constructor
     *
     * Creates a square with side length 0.
     */
    Square();

    /*!
     * \brief Constructor with parameter
     * \param[in] side The length of one side of the square
     *
     * Creates a square with the specified side length.
     */
    Square(T side);

    /*!
     * \brief Get the area of the square
     * \return The area of the square
     *
     * The area of a square is simply the side length squared.
     */
    auto area() const -> double override;

    /*!
     * \brief Set the side length
     * \param[in] side The length of the square's side
     */
    void setSide(T side);

    /*!
     * \brief Get the side length
     * \return The length of the square's side
     */
    auto getSide() const -> T;
};


/*!
 * \brief Rectangle class
 *
 * The `Rectangle` class represents a rectangle, a 2D geometric shape with opposite sides of equal length.
 * It inherits from `Entity` and `Shape`, providing common geometric properties and the ability to calculate its area.
 */
template<typename T>
class Rectangle
  : public Entity,
    public Shape
{

public:

    using value_type = T;

private:

    T width;  ///< The width of the rectangle
    T height; ///< The height of the rectangle

public:

    /*!
     * \brief Default constructor
     *
     * Creates a rectangle with width and height equal to 0.
     */
    Rectangle();

    /*!
     * \brief Constructor with parameters
     * \param[in] width The width of the rectangle
     * \param[in] height The height of the rectangle
     *
     * Creates a rectangle with the specified width and height.
     */
    Rectangle(T width, T height);

    /*!
     * \brief Get the area of the rectangle
     * \return The area of the rectangle
     *
     * The area of a rectangle is calculated by multiplying its width by its height.
     */
    auto area() const -> double override;

    /*!
     * \brief Set the width of the rectangle
     * \param[in] width The width of the rectangle
     */
    void setWidth(T width);

    /*!
     * \brief Get the width of the rectangle
     * \return The width of the rectangle
     */
    auto getWidth() const->T;

    /*!
     * \brief Set the height of the rectangle
     * \param[in] height The height of the rectangle
     */
    void setHeight(T height);

    /*!
     * \brief Get the height of the rectangle
     * \return The height of the rectangle
     */
    auto getHeight() const->T;
};





template<typename T>
Circle<T>::Circle()
  : Entity(Type::circle),
    center(Point<T>()),
    radius(static_cast<T>(1))
{
}

template<typename T>
Circle<T>::Circle(const Point<T> &center, T radius)
  : Entity(Type::circle),
    center(center),
    radius(radius)
{
}

template<typename T>
Circle<T>::Circle(const Circle<T> &circle)
  : Entity(Type::circle),
    center(circle.center),
    radius(circle.radius)
{
}

template<typename T>
Circle<T>::Circle(Circle<T> &&circle) TL_NOEXCEPT
  : Entity(std::forward<Entity>(circle)),
    center(std::move(circle.center)),
    radius(circle.radius)
{
}

template<typename T>
auto Circle<T>::operator = (const Circle &circle) -> Circle<T> &
{
    if (this != &circle) {
        Entity::operator = (circle);
        this->center = circle.center;
        this->radius = circle.radius;
    }

    return *this;
}

template<typename T>
auto Circle<T>::operator = (Circle &&circle) TL_NOEXCEPT -> Circle<T> &
{
    if (this != &circle) {
        Entity::operator = (std::forward<Entity>(circle));
        this->center = std::move(circle.center);
        this->radius = circle.radius;
    }

    return *this;
}

template<typename T> template<typename T2>
Circle<T>::operator Circle<T2>() const
{
    return Circle<T2>(this->center, numberCast<T2>(this->radius));
}

template<typename T>
auto Circle<T>::area() const -> double
{
    return consts::pi<double> * radius * radius;
}

template<typename T>
auto Circle<T>::length() const -> double
{
    return consts::two_pi<double> * radius;
}

template<typename T>
auto Circle<T>::rect() const -> Rect<T>
{
    return Rect<T>(Point<T>(center.x - radius, center.y - radius), radius * 2., radius * 2.);
}

template<typename T>
bool Circle<T>::operator==(const Circle &other) const
{
    return center == other.center && radius == other.radius;
}




template<typename T>
Ellipse<T>::Ellipse()
  : Entity(Type::ellipse),
    center(Point<T>()),
    a(static_cast<T>(1)),
    b(static_cast<T>(1))
{
}

template<typename T>
Ellipse<T>::Ellipse(const Point<T> &center, T a, T b)
  : Entity(Type::ellipse),
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

template<typename T>
Ellipse<T>::Ellipse(const Ellipse<T> &ellipse)
  : Entity(ellipse),
    center(ellipse.center),
    a(ellipse.a),
    b(ellipse.b)
{
}

template<typename T>
Ellipse<T>::Ellipse(Ellipse<T> &&ellipse) TL_NOEXCEPT
  : Entity(std::forward<Entity>(ellipse)),
    center(std::move(ellipse.center)),
    a(ellipse.a),
    b(ellipse.b)
{
}

template<typename T>
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

template<typename T>
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

template<typename T> template<typename T2>
Ellipse<T>::operator Ellipse<T2>() const
{
    return Ellipse<T2>(this->center,
                       numberCast<T2>(this->a),
                       numberCast<T2>(this->b));
}

template<typename T>
auto Ellipse<T>::area() const -> double
{
    return consts::pi<double> * a * b;
}

template<typename T>
auto Ellipse<T>::length() const -> double
{
    return consts::pi<double> * (3 * (a + b) - sqrt((3 * a + b)*(a + 3 * b)));
}

template<typename T>
auto Ellipse<T>::rect() const -> Rect<T>
{
    return Rect<T>(tl::Point<T>(center.x - a, center.y - b), a * 2., b * 2.);
}

template<typename T>
auto Ellipse<T>::isInner(const Point<T> &point) const -> bool
{
    if (!this->rect().contains(point)) return false;
    
    T x = (point.x - this->center.x);
    T y = (point.y - this->center.y);
    T check = (x * x) / (this->a * this->a) + (y * y) / (this->b * this->b);
    //if (check <= 1) return true; // the point is inner
    //else if (check == 1) return true; // the point is on the boundary of the region
    //else return false;

    return check <= 1;
}

template<typename T>
auto Ellipse<T>::operator==(const Ellipse &other) const -> bool
{
    return center == other.center && a == other.a && b == other.b;
}



template<typename T>
Triangle<T>::Triangle()
  : Entity(Type::triangle),
    mPt1(),
    mPt2(),
    mPt3()
{
}

template<typename T>
Triangle<T>::Triangle(const Point<T> &pt1, 
                      const Point<T> &pt2, 
                      const Point<T> &pt3)
  : Entity(Type::triangle),
    mPt1(pt1),
    mPt2(pt2),
    mPt3(pt3)
{
}

template<typename T>
Triangle<T>::Triangle(const Triangle<T> &triangle)
  : Entity(triangle),
    mPt1(triangle.mPt1),
    mPt2(triangle.mPt2),
    mPt3(triangle.mPt3)
{
}

template<typename T>
Triangle<T>::Triangle(Triangle<T> &&triangle) TL_NOEXCEPT
  : Entity(std::forward<Entity>(triangle)),
    mPt1(std::move(triangle.mPt1)),
    mPt2(std::move(triangle.mPt2)),
    mPt3(std::move(triangle.mPt3))
{
}

template<typename T>
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

template<typename T>
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

template<typename T>
auto Triangle<T>::area() const -> double
{
    return 0.5 * std::abs(
        mPt1.x * (mPt2.y - mPt3.y) +
        mPt2.x * (mPt3.y - mPt1.y) +
        mPt3.x * (mPt1.y - mPt2.y));
}

template<typename T>
auto Triangle<T>::perimeter() const -> double
{
    double side1 = mPt1.distanceTo(mPt2);
    double side2 = mPt2.distanceTo(mPt3);
    double side3 = mPt3.distanceTo(mPt1);
    return side1 + side2 + side3;
}

template<typename T>
auto Triangle<T>::isValid() const -> bool
{
    double side1 = mPt1.distanceTo(mPt2);
    double side2 = mPt2.distanceTo(mPt3);
    double side3 = mPt3.distanceTo(mPt1);
    return (side1 + side2 > side3) &&
           (side2 + side3 > side1) &&
           (side3 + side1 > side2);
}

template<typename T>
auto Triangle<T>::centroid() const -> Point<T>
{
    return Point<T>((mPt1.x + mPt2.x + mPt3.x) / 3.0, (mPt1.y + mPt2.y + mPt3.y) / 3.0);
}

template<typename T>
auto Triangle<T>::operator==(const Triangle &other) const -> bool
{
    return mPt1 == other.mPt1 && mPt2 == other.mPt2 && mPt3 == other.mPt3;
}





template<typename T>
Square<T>::Square()
  : Entity(Type::square),
    side(0)
{
}

template<typename T>
Square<T>::Square(T side)
  : Entity(Type::square),
    side(side)
{
}

template<typename T>
auto Square<T>::area() const -> double
{
    return static_cast<double>(side * side);
}

template<typename T>
void Square<T>::setSide(T side)
{
    this->side = side;
}

template<typename T>
auto Square<T>::getSide() const -> T 
{
    return side;
}





template<typename T>
Rectangle<T>::Rectangle()
  : Entity(Type::rectangle),
    width(0), 
    height(0)
{
}


template<typename T>
Rectangle<T>::Rectangle(T width, T height)
  : Entity(Type::rectangle), 
    width(width),
    height(height)
{
}

template<typename T>
auto Rectangle<T>::area() const -> double
{
    return static_cast<double>(width * height);
}

template<typename T>
void Rectangle<T>::setWidth(T width)
{
    this->width = width;
}

template<typename T>
auto Rectangle<T>::getWidth() const -> T
{
    return width;
}

template<typename T>
void Rectangle<T>::setHeight(T height)
{
    this->height = height;
}

template<typename T>
auto Rectangle<T>::getHeight() const -> T
{
    return height;
}


/*! \} */ // end of geometry

} // End namespace tl
