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

#include "tidop/geometry/shapes/Shape.h"
#include "tidop/geometry/primitives/Point.h"
#include "tidop/graphic/rect.h"
#include "tidop/math/numeric/Arithmetic.h"

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
    constexpr Ellipse();

    /*!
     * \brief Constructor
     * \param[in] center Ellipse center
     * \param[in] a Semi-major axis
     * \param[in] b Semi-minor axis
     */
    constexpr Ellipse(const Point<T> &center, T a, T b);

    /*!
     * \brief Copy constructor
     * \param[in] ellipse Ellipse object to copy
     */
    constexpr Ellipse(const Ellipse<T> &ellipse) = default;

    /*!
     * \brief Move constructor
     * \param[in] ellipse Ellipse object to move
     */
    constexpr Ellipse(Ellipse<T> &&ellipse) noexcept = default;

    /*!
     * \brief Destructor
     */
    ~Ellipse() override = default;

    /*!
     * \brief Copy assignment operator
     * \param[in] ellipse Ellipse object to copy
     * \return Reference to the assigned object
     */
    constexpr auto operator=(const Ellipse<T> &ellipse) -> Ellipse<T> & = default;

    /*!
     * \brief Move assignment operator
     * \param[in] ellipse Ellipse object to move
     * \return Reference to the assigned object
     */
    constexpr auto operator=(Ellipse<T> &&ellipse) TL_NOEXCEPT -> Ellipse<T> & = default;

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
    [[nodiscard]]
    constexpr auto area() const noexcept -> double override;

    /*!
     * \brief Computes the approximate circumference of the ellipse
     * \return The computed circumference
     */
    [[nodiscard]]
    constexpr auto length() const noexcept -> double;

    /*!
     * \brief Computes the bounding rectangle of the ellipse (axis-aligned)
     * \return A rectangle that bounds the ellipse
     */
    [[nodiscard]]
    constexpr auto rect() const -> Rect<T>;

    /*!
     * \brief Checks if a point is inside the ellipse
     * \param[in] point The point to check
     * \return True if the point is inside, false otherwise
     */
    [[nodiscard]]
    constexpr auto isInner(const Point<T> &point) const -> bool;

    /*!
     * \brief Equality comparison operator
     * \param[in] other The ellipse to compare with
     * \return True if ellipses are equal, false otherwise
     */
    [[nodiscard]]
    constexpr auto operator==(const Ellipse &other) const -> bool;
};



template<typename T>
constexpr Ellipse<T>::Ellipse()
  : center(Point<T>()),
    a(static_cast<T>(1)),
    b(static_cast<T>(1))
{
}

template<typename T>
constexpr Ellipse<T>::Ellipse(const Point<T> &center, T a, T b)
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

//template<typename T>
//Ellipse<T>::Ellipse(const Ellipse<T> &ellipse)
//  : center(ellipse.center),
//    a(ellipse.a),
//    b(ellipse.b)
//{
//}
//
//template<typename T>
//Ellipse<T>::Ellipse(Ellipse<T> &&ellipse) TL_NOEXCEPT
//  : center(std::move(ellipse.center)),
//    a(ellipse.a),
//    b(ellipse.b)
//{
//}

//template<typename T>
//auto Ellipse<T>::operator = (const Ellipse &ellipse) -> Ellipse<T> &
//{
//    if (this != &ellipse) {
//        this->center = ellipse.center;
//        this->a = ellipse.a;
//        this->b = ellipse.b;
//    }
//
//    return *this;
//}
//
//template<typename T>
//auto Ellipse<T>::operator = (Ellipse &&ellipse) TL_NOEXCEPT -> Ellipse<T> &
//{
//    if (this != &ellipse) {
//        this->center = std::move(ellipse.center);
//        this->a = ellipse.a;
//        this->b = ellipse.b;
//    }
//
//    return *this;
//}

template<typename T> template<typename T2>
Ellipse<T>::operator Ellipse<T2>() const
{
    return Ellipse<T2>(static_cast<Point<T2>>(this->center),
                       numberCast<T2>(this->a),
                       numberCast<T2>(this->b));
}

template<typename T>
constexpr auto Ellipse<T>::area() const noexcept -> double
{
    return consts::pi<double> * a * b;
}

template<typename T>
constexpr auto Ellipse<T>::length() const noexcept -> double
{
    return consts::pi<double> * (3 * (a + b) - tl::sqrt((3 * a + b)*(a + 3 * b)));
}

template<typename T>
constexpr auto Ellipse<T>::rect() const -> Rect<T>
{
    return Rect<T>(tl::Point<T>(center.x() - a, center.y() - b), a * 2., b * 2.);
}

template<typename T>
constexpr auto Ellipse<T>::isInner(const Point<T> &point) const -> bool
{
    if (!this->rect().contains(point)) return false;
    
    auto v = point - center;

    T check = (v.x() * v.x()) / (this->a * this->a) + (v.y() * v.y()) / (this->b * this->b);

    return check <= 1;
}

template<typename T>
constexpr auto Ellipse<T>::operator==(const Ellipse &other) const -> bool
{
    return center == other.center && a == other.a && b == other.b;
}


/*! \} */ 

}
