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
 * \brief Circle class
 *
 * Represents a circle with a center and a radius.
 */
template<typename T>
class Circle
  : public Shape
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
    auto rect() const -> Rect<T>;

    /*!
     * \brief Equality comparison operator
     * \param[in] other The circle to compare with
     * \return True if circles are equal, false otherwise
     */
    bool operator==(const Circle &other) const;

};




template<typename T>
Circle<T>::Circle()
  : center(Point<T>()),
    radius(static_cast<T>(1))
{
}

template<typename T>
Circle<T>::Circle(const Point<T> &center, T radius)
  : center(center),
    radius(radius)
{
}

template<typename T>
Circle<T>::Circle(const Circle<T> &circle)
  : center(circle.center),
    radius(circle.radius)
{
}

template<typename T>
Circle<T>::Circle(Circle<T> &&circle) TL_NOEXCEPT
  : center(std::move(circle.center)),
    radius(circle.radius)
{
}

template<typename T>
auto Circle<T>::operator = (const Circle &circle) -> Circle<T> &
{
    if (this != &circle) {
        this->center = circle.center;
        this->radius = circle.radius;
    }

    return *this;
}

template<typename T>
auto Circle<T>::operator = (Circle &&circle) TL_NOEXCEPT -> Circle<T> &
{
    if (this != &circle) {
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


/*! \} */ 

}
