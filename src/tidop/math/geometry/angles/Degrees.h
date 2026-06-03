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

#include "tidop/core/base/Concepts.h"
#include "tidop/math/base/Constants.h"
#include "tidop/math/geometry/angles/AngleBase.h"
#include "tidop/math/geometry/angles/detail/Utils.h"
#include "tidop/math/numeric/Arithmetic.h"

namespace tl
{


/*!
 * \brief Class representing an angle in degrees.
 *
 * The `Degrees` class provides functionality for handling angles in degrees,
 * including methods for normalization and setting/getting the degree, minute,
 * and second components.
 *
 * \tparam T The type used for the angle value, typically a floating-point type like `float` or `double`.
 */
template<Floating T>
class Degrees
  : public AngleBase<Degrees<T>, T>
{

public:

    using value_type = T;
    using AngleBase<Degrees<T>, T>::AngleBase;

public:

    /*!
     * \brief Default constructor.
     *
     * Constructs a `Degrees` object with a default angle value of zero.
     */
    constexpr Degrees();

    /*!
     * \brief Constructor with an initial angle value.
     * \param[in] value The initial angle value in degrees.
     */
    constexpr Degrees(T value);

    /*!
     * \brief Constructor with degrees, minutes, and seconds components.
     * \param[in] degrees The degree component.
     * \param[in] minutes The minute component.
     * \param[in] seconds The second component.
     */
    constexpr Degrees(int degrees, int minutes, T seconds);

    /*!
     * \brief Destructor.
     */
    ~Degrees() = default;

    /*!
     * \brief Normalize the angle to the range \f$[-180, 180]\f$.
     *
     * Adjusts the angle value to ensure it is within the standard range for degrees.
     */
    constexpr void normalize();

    /*!
     * \brief Normalize the angle to the range \f$[0, 360]\f$.
     *
     * Adjusts the angle value to be positive and within a full circle in degrees.
     */
    constexpr void normalizePositive();

    /*!
     * \brief Get the degree component of the angle.
     * \return The degree component, as an integer.
     */
    [[nodiscard]] 
    constexpr auto degrees() const -> int;

    /*!
     * \brief Set the degree component of the angle.
     * \param[in] degrees The degree value to set.
     */
    constexpr void setDegrees(int degrees);

    /*!
     * \brief Get the minute component of the angle.
     * \return The minute component, as an integer.
     */
    [[nodiscard]] 
    constexpr auto minutes() const -> int;

    /*!
     * \brief Set the minute component of the angle.
     * \param[in] minutes The minute value to set.
     */
    constexpr void setMinutes(int minutes);

    /*!
     * \brief Get the second component of the angle.
     * \return The second component, as a value of type `T`.
     */
    [[nodiscard]] 
    constexpr auto seconds() const -> T;

    /*!
     * \brief Set the second component of the angle.
     * \param seconds The second value to set.
     */
    constexpr void setSeconds(T seconds);

};



/* Degrees implementation */

template<Floating T>
constexpr Degrees<T>::Degrees()
  : AngleBase<Degrees<T>, T>()
{
}

template<Floating T>
constexpr Degrees<T>::Degrees(T value)
  : AngleBase<Degrees<T>, T>(value)
{
}

template<Floating T>
constexpr Degrees<T>::Degrees(int degrees, int minutes, T seconds)
  : AngleBase<Degrees<T>, T>()
{
    this->setValue(detail::isNegative(degrees) *
        (std::abs(degrees) +
            minutes / static_cast<T>(consts::degrees_to_minutes) +
            seconds / static_cast<T>(consts::degrees_to_seconds)));
}

template<Floating T>
constexpr void Degrees<T>::normalize()
{
    this->setValue(wrap(this->value(), T(-180), T(180)));
}

template<Floating T>
constexpr void Degrees<T>::normalizePositive()
{
    this->setValue(wrap(this->value(), T(0), T(360)));
}

template<Floating T>
constexpr auto Degrees<T>::degrees() const -> int
{
    return static_cast<int>(this->value());
}

template<Floating T>
constexpr void Degrees<T>::setDegrees(int degrees)
{
    *this = Degrees(degrees, minutes(), seconds());
}

template<Floating T>
constexpr auto Degrees<T>::minutes() const -> int
{
    int seconds = static_cast<int>(std::round(this->value() * consts::degrees_to_seconds));
    seconds = std::abs(seconds % consts::degrees_to_seconds);
    return seconds / consts::minutes_to_seconds;
}

template<Floating T>
constexpr void Degrees<T>::setMinutes(int minutes)
{
    T value = detail::isNegative(this->degrees()) * (std::abs(this->degrees()) +
        minutes / static_cast<T>(consts::degrees_to_minutes) +
        this->seconds() / static_cast<T>(consts::degrees_to_seconds));

    this->setValue(value);
}

template<Floating T>
constexpr auto Degrees<T>::seconds() const -> T
{
    double min = fabs(this->value() - this->degrees()) * consts::degrees_to_minutes;
    return fabs(min - this->minutes()) * consts::minutes_to_seconds;
}

template<Floating T>
constexpr void Degrees<T>::setSeconds(T seconds)
{
    T value = detail::isNegative(this->degrees()) *
        (std::abs(this->degrees()) +
            std::abs(this->value() - this->degrees()) +
            seconds / static_cast<T>(consts::degrees_to_seconds));
    this->setValue(value);
}

namespace literals
{

constexpr auto operator"" _deg(long double val)
{
    return Degrees<double>(static_cast<double>(val));
}

constexpr auto operator"" _deg(unsigned long long val)
{
    return Degrees<double>(static_cast<double>(val));
}

}


/*! \} */


} // namespace tl
