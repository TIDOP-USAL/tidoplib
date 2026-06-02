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

namespace tl
{


/*!
 * \brief Checks if two floating-point numbers are nearly equal.
 *
 * This function compares two floating-point numbers with an epsilon margin of error.
 *
 * \tparam T Floating-point type.
 * \param a First value.
 * \param b Second value.
 * \return True if the values are nearly equal, false otherwise.
 */
template<Floating T>
[[nodiscard]]
constexpr auto isNearlyEqual(T a, T b, T epsilon = std::numeric_limits<T>::epsilon() * 128) -> bool
{
    if (a == b) return true;

    T diff = std::abs(a - b);
    if (diff >= consts::one<T>) return false;

    T norm = std::min((std::abs(a) + std::abs(b)), std::numeric_limits<T>::max());

    return diff <= std::max(std::numeric_limits<T>::min(), epsilon * norm);
}

template<Integral T>
[[nodiscard]] 
constexpr auto isNearlyEqual(T a, T b) -> bool
{
    return a == b;
}

/*!
 * \brief Checks if an integral value is zero.
 *
 * \tparam T Integral type.
 * \param value Value to check.
 * \return True if the value is zero, false otherwise.
 */
template<Integral T>
[[nodiscard]] 
constexpr auto isZero(T value) -> bool
{
    return value == consts::zero<T>;
}

/*!
 * \brief Checks if a floating-point value is zero.
 *
 * Accounts for numerical precision and treats values very close to zero as zero.
 *
 * \tparam T Floating-point type.
 * \param value Value to check.
 * \return True if the value is zero or nearly zero, false otherwise.
 */
template<Floating T>
[[nodiscard]] 
constexpr auto isZero(T value) -> bool
{
    T up = std::nextafter(consts::zero<T>, consts::one<T>);
    T down = std::nextafter(consts::zero<T>, -consts::one<T>);
    return (down <= value && value <= up);
}

/*!
 * \brief Checks if a floating-point value is nearly zero.
 *
 * Compares the value against zero with an epsilon margin of error.
 *
 * \tparam T Floating-point type.
 * \param value Value to check.
 * \return True if the value is nearly zero, false otherwise.
 */
template<Floating T>
[[nodiscard]] 
constexpr auto isNearlyZero(T value) -> bool
{
    return isNearlyEqual(value, consts::zero<T>);
}

template<Integral T>
[[nodiscard]] 
constexpr auto isNearlyZero(T value) -> bool
{
    return isZero(value);
}


} // namespace tl