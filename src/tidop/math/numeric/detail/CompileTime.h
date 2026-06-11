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
#include <type_traits>

namespace tl
{

namespace detail
{

template<typename T>
[[nodiscard]] 
constexpr auto abs_compile_time(T val) noexcept -> T
{
    return val < T(0) ? -val : val;
}

template<typename T>
[[nodiscard]] constexpr auto sqrt_compile_time(T x) noexcept
{
	using float_type = std::conditional_t<std::is_integral_v<T>, double, T>;
	
    if (x < 0) {
        return float_type(0);
    }
    if (x == 0 || x == 1) return static_cast<float_type>(x);

    float_type current = static_cast<float_type>(x) / float_type(2);
    float_type previous = float_type(0);

    // Newton-Raphson
    for (int i = 0; i < 100; ++i) {
        previous = current;
        current = static_cast<float_type>(0.5) * (current + static_cast<float_type>(x) / current);

        float_type diff = current - previous;
        if (abs_compile_time(diff) < static_cast<float_type>(1e-15)) {
            break;
        }
    }
	
    return current;
}

template<typename T>
[[nodiscard]]
constexpr auto hypot_compile_time(T a, T b) noexcept
{
    using float_type = std::conditional_t<std::is_integral_v<T>, double, T>;

    float_type fa = static_cast<float_type>(a);
    float_type fb = static_cast<float_type>(b);

    fa = abs_compile_time(fa);
    fb = abs_compile_time(fb);

    auto [min_val, max_val] = std::minmax(fa, fb);

    if (max_val == float_type(0)) {
        return float_type(0);
    }

    float_type r = min_val / max_val;

    return max_val * sqrt_compile_time(float_type(1) + r * r);
}

} // namespace detail

} // namespace tl