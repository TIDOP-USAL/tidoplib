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
#include "tidop/math/geometry/angles/AngleBase.h"

namespace tl
{

/*! \addtogroup AngleConversion
 *  \{
 */


/*!
 * \brief Class representing an angle in radians.
 *
 * The `Radians` class inherits from `AngleBase` and provides specific
 * implementations for handling angles in radians.
 *
 * \tparam T The type used for the angle value (e.g., `double` or `float`).
 */
template<Floating T>
class Radians 
  : public AngleBase<Radians<T>, T>
{

public:

    using value_type = T;
    using AngleBase<Radians<T>, T>::AngleBase;

public:

    /*!
     * \brief Default constructor.
     *
     * Initializes the angle to zero radians.
     */
    constexpr Radians();

    /*!
     * \brief Constructor with an initial angle value.
     * \param[in] value The initial angle value in radians.
     */
    constexpr Radians(T value);

    /*!
     * \brief Destructor.
     */
    ~Radians() = default;

    /*!
     * \brief Normalize the angle to the range \f$[-\pi, \pi]\f$.
     *
     * Ensures that the angle value is within the standard range for radians.
     */
    void normalize();

    /*!
     * \brief Normalize the angle to the range \f$[0, 2\pi]\f$.
     *
     * Ensures that the angle value is positive and within the range of one full circle.
     */
    void normalizePositive();

};



/* Radians implementation */

template<Floating T>
constexpr Radians<T>::Radians()
  : AngleBase<Radians<T>, T>(AngleUnit::radians)
{
}

template<Floating T>
constexpr Radians<T>::Radians(T value)
  : AngleBase<Radians<T>, T>(AngleUnit::radians, value)
{
}

template<Floating T>
void Radians<T>::normalize()
{
    T value = this->value();

    if (value <= -consts::pi<T> || value > consts::pi<T>) {
        value = std::fmod(value + consts::pi<T>, consts::two_pi<T>);

        if (value <= consts::zero<T>)
            value += consts::pi<T>;
        else
            value -= consts::pi<T>;

        this->setValue(value);
    }
}

template<Floating T>
void Radians<T>::normalizePositive()
{
    T value = this->value();

    if (value < consts::zero<T> || value >= consts::two_pi<T>) {
        value = std::fmod(value, consts::two_pi<T>);

        if (value < consts::zero<T>)
            value += consts::two_pi<T>;

        this->setValue(value);
    }
}



namespace literals
{
// 180.0_rad
constexpr auto operator"" _rad(long double val)
{
    return Radians<double>(static_cast<double>(val));
}

// 180_rad
constexpr auto operator"" _rad(unsigned long long val)
{
    return Radians<double>(static_cast<double>(val));
}
}
  
/*! \} */

} // namespace tl
