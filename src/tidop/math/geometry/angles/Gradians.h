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

/*! \addtogroup AngleConversion
 *  \{
 */


/*!
 * \brief Class representing an angle in gradians.
 *
 * The `Gradians` class provides functionality for handling angles in gradians,
 * allowing for normalization and conversion to and from degrees, minutes, and seconds.
 *
 * \tparam T The type used for the angle value, typically a floating-point type like `float` or `double`.
 */
template<Floating T>
class Gradians
  : public AngleBase<Gradians<T>, T>
{

public:

    using value_type = T;
    using AngleBase<Gradians<T>, T>::AngleBase;

public:

    /*!
     * \brief Default constructor.
     *
     * Constructs a `Gradians` object with a default angle value of zero.
     */
    constexpr Gradians();

    /*!
     * \brief Constructor with an initial angle value.
     * \param[in] value The initial angle value in gradians.
     */
    constexpr Gradians(T value);

    /*!
     * \brief Destructor.
     */
    ~Gradians() = default;

    /*!
     * \brief Normalize the angle to the range \f$[-200, 200]\f$.
     *
     * Adjusts the angle value to ensure it is within the standard range for gradians.
     */
    constexpr void normalize();

    /*!
     * \brief Normalize the angle to the range \f$[0, 400]\f$.
     *
     * Adjusts the angle value to be positive and within a full circle in gradians.
     */
    constexpr void normalizePositive();

    /*!
     * \brief Get the degree component of the angle.
     * \return The degree component, as an integer.
     */
    [[nodiscard]] 
    constexpr auto degrees() const -> int
    {
        return static_cast<int>(this->value());
    }

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
     * \param minutes The minute value to set.
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



/* Gradians implementation */

template<Floating T>
constexpr Gradians<T>::Gradians()
    : AngleBase<Gradians<T>, T>()
{}

template<Floating T>
constexpr Gradians<T>::Gradians(T value)
    : AngleBase<Gradians<T>, T>(value) 
{}

template<Floating T>
constexpr void Gradians<T>::normalize()
{
    //T val = std::fmod(this->value(), static_cast<T>(400));
    //if (val > 200) val -= 400;
    //else if (val <= -200) val += 400;
    //this->setValue(val);
    this->setValue(wrap(this->value(), T(-200), T(200)));
}

template<Floating T>
constexpr void Gradians<T>::normalizePositive()
{
    //T val = std::fmod(this->value(), static_cast<T>(400));
    //if (val < 0) val += 400;
    //this->setValue(val);
    this->setValue(wrap(this->value(), T(0), T(400)));
}

template<Floating T>
constexpr void Gradians<T>::setDegrees(int degrees)
{
    T value = detail::isNegative(degrees) *
              (static_cast<T>(std::abs(degrees)) +
              ((this->seconds() / consts::quarter_circle_grad) + this->minutes()) /
              consts::quarter_circle_grad);
    this->setValue(value);
}

template<Floating T>
[[nodiscard]]
constexpr auto Gradians<T>::minutes() const -> int
{
    return static_cast<int>(std::abs(this->value() - this->degrees()) * consts::quarter_circle_grad);
}

template<Floating T>
constexpr void Gradians<T>::setMinutes(int minutes)
{
    int degrees = this->degrees();
    T value = detail::isNegative(degrees) *
              (static_cast<T>(std::abs(degrees)) +
              ((this->seconds() / static_cast<T>(consts::quarter_circle_grad)) +
              static_cast<T>(minutes)) / static_cast<T>(consts::quarter_circle_grad));
    this->setValue(value);
}

template<Floating T>
[[nodiscard]]
constexpr auto Gradians<T>::seconds() const -> T
{
    double min = std::abs(this->value() - this->degrees()) * static_cast<T>(consts::quarter_circle_grad);
    return (min - this->minutes()) * static_cast<T>(consts::quarter_circle_grad);
}

template<Floating T>
constexpr void Gradians<T>::setSeconds(T seconds)
{
    int degrees = this->degrees();
    T value = detail::isNegative(degrees) *
              (static_cast<T>(std::abs(this->degrees())) +
              ((seconds / consts::quarter_circle_grad) +
              static_cast<T>(this->minutes())) / consts::quarter_circle_grad);
    this->setValue(value);
}



namespace literals
{

constexpr auto operator"" _grad(long double val)
{
    return Gradians<double>(static_cast<double>(val));
}

constexpr auto operator"" _grad(unsigned long long val)
{
    return Gradians<double>(static_cast<double>(val));
}

}

/*! \} */

} // namespace tl
