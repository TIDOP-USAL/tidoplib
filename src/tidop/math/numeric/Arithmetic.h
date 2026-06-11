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

#include <cmath>
#include <algorithm>

#include "tidop/core/base/Concepts.h"
#include "tidop/core/base/macros/Deprecation.h"
#include "tidop/math/numeric/detail/CompileTime.h"

namespace tl
{

/*!
 * \brief Clamps a value between a pair of boundary values.
 *
 * Ensures that the \p value lies within the range [\p min, \p max].
 * If \p value is less than \p min, returns \p min.
 * If \p value is greater than \p max, returns \p max.
 * Otherwise, returns \p value.
 *
 * \tparam T Type of the value and boundaries, which must be comparable.
 * \param value The value to be clamped.
 * \param min The lower boundary.
 * \param max The upper boundary.
 * \return The clamped value within the range [\p min, \p max].
 */
template<Arithmetic T>
[[nodiscard]]
TL_DEPRECATED("void std::clamp", "4.0")
constexpr auto clamp(const T &value, const T &min, const T &max) -> T
{
    return std::clamp(value, min, max);
}

/*!
 * \brief Computes the norm of a 2D vector
 *
 * \tparam T Arithmetic type.
 * \param a X-coordinate of the vector.
 * \param b Y-coordinate of the vector.
 * \return The norm of the vector.
 */
template<Arithmetic T>
[[nodiscard]]
constexpr auto module(T a, T b) noexcept
{
    if (std::is_constant_evaluated()) {
        return detail::hypot_compile_time(a, b);
    } else {
        return std::hypot(a, b);
    }
}


template<typename T>
[[nodiscard]]
constexpr auto sign(T val) noexcept -> int
{
    return (T(0) < val) - (val < T(0));
}


template<typename T>
[[nodiscard]]
constexpr auto square(T x) noexcept -> T
{ 
    return x * x;
}

template<typename T>
[[nodiscard]]
constexpr auto cube(T x) noexcept -> T
{ 
    return x * x * x;
}

template<typename T>
[[nodiscard]] 
constexpr auto abs(T val) noexcept -> T
{
    if (std::is_constant_evaluated()) {
        return detail::abs_compile_time(val);
    } else {
        return std::abs(val);
    }
}

template<typename T>
[[nodiscard]] constexpr auto sqrt(T x) noexcept
{
    if (std::is_constant_evaluated()) {
        return detail::sqrt_compile_time(x); // (Newton-Raphson)
    } else {
        return std::sqrt(x);
    }
}

/*!
 * \brief Envuelve un valor en el rango [min, max).
 * \tparam T Tipo flotante.
 * \param[in] value Valor a envolver.
 * \param[in] min Límite inferior (exclusivo).
 * \param[in] max Límite superior (inclusivo).
 */
template<Floating T>
[[nodiscard]]
constexpr auto wrap(T value, T min, T max) noexcept -> T
{
    const T range = max - min;
    T result = std::fmod(value - min, range);
    if (result < 0) result += range;
    return result + min;
}

} // namespace tl