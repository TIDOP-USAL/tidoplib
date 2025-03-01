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

#include "tidop/geometry/shapes/shape.h"
#include "tidop/geometry/entities/point.h"
#include "tidop/geometry/rect.h"

namespace tl
{

/*! \addtogroup Shapes
 *  \{
 */


/*!
 * \brief Ellipse class
 *
 * Represents an ellipse defined by a center, semi-major axis, semi-minor 
 * axis, and optional rotation.
 */
template<typename T>
class Ellipse
  : public Shape
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



template<typename T>
Ellipse<T>::Ellipse()
  : center(Point<T>()),
    a(static_cast<T>(1)),
    b(static_cast<T>(1))
{
}

template<typename T>
Ellipse<T>::Ellipse(const Point<T> &center, T a, T b)
  : center(center),
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
  : center(ellipse.center),
    a(ellipse.a),
    b(ellipse.b)
{
}

template<typename T>
Ellipse<T>::Ellipse(Ellipse<T> &&ellipse) TL_NOEXCEPT
  : center(std::move(ellipse.center)),
    a(ellipse.a),
    b(ellipse.b)
{
}

template<typename T>
auto Ellipse<T>::operator = (const Ellipse &ellipse) -> Ellipse<T> &
{
    if (this != &ellipse) {
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


/*! \} */ 

}
