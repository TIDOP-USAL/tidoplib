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

#include "tidop/core/defs.h"

#include <cmath>
#include <type_traits>
#include <algorithm>

#include "tidop/core/common.h"

namespace tl
{


/*!
 * \defgroup math Mathematical module
 *
 * \{
 */


/* Definition of constants */

namespace consts
{

template<typename T>
constexpr T zero = static_cast<T>(0);
template<typename T>
constexpr T one = static_cast<T>(1);
template<typename T>
constexpr T two = static_cast<T>(2);
template<typename T>
constexpr enableIfFloating<T,T> one_half = static_cast<T>(0.5L);

template<typename T>
constexpr enableIfFloating<T,T> pi = static_cast<T>(3.141592653589793238462643383279502884L);
template<typename T>
constexpr enableIfFloating<T,T> half_pi = static_cast<T>(1.570796326794896619231321691639751442L);
template<typename T>
constexpr enableIfFloating<T,T> two_pi = static_cast<T>(6.283185307179586476925286766559005768L);
template<typename T>
constexpr enableIfFloating<T,T> inv_pi = static_cast<T>(1.L / 3.141592653589793238462643383279502884L);
template<typename T>
constexpr enableIfFloating<T,T> sqrt2 = static_cast<T>(1.414213562373095048801688724209698079L);
template<typename T>
constexpr enableIfFloating<T,T> sqrt3 = static_cast<T>(1.7320508075688772935274463415059L);
template<typename T>
constexpr enableIfFloating<T,T> inv_sqrt2 = static_cast<T>(1.L / 1.414213562373095048801688724209698079L);
template<typename T>
constexpr enableIfFloating<T,T> e = static_cast<T>(2.718281828459045235360287471352662498L);
template<typename T>
constexpr enableIfFloating<T,T> log2e = static_cast<T>(1.442695040888963407359924681001892137L);
template<typename T>
constexpr enableIfFloating<T,T> log10e = static_cast<T>(0.4342944819032518276511289189166050823L);
template<typename T>
constexpr enableIfFloating<T,T> ln2 = static_cast<T>(0.6931471805599453094172321214581765681L);
template<typename T>
constexpr enableIfFloating<T,T> ln10 = static_cast<T>(2.302585092994045684017991454684364208L);



/*! \defgroup angleConversion Angle conversion
 *
 * Conversions between different angle formats:
 * - Degrees
 * - Gradians or gons
 * - Radians
 * \{
 */

 /* Angle conversion constants */

constexpr auto full_circle_deg = 360;
constexpr auto full_circle_grad = 400;
constexpr auto half_circle_deg = 180;
constexpr auto half_circle_grad = 200;
constexpr auto quarter_circle_deg = 90;
constexpr auto quarter_circle_grad = 100;
constexpr auto degrees_to_minutes = 60;
constexpr auto minutes_to_seconds = 60;
constexpr auto degrees_to_seconds = 3600;

template<typename T>
constexpr enableIfFloating<T,T> rad_to_deg = static_cast<T>(half_circle_deg) / pi<T>;
template<typename T>
constexpr enableIfFloating<T,T> deg_to_rad = pi<T> / static_cast<T>(half_circle_deg);
template<typename T>
constexpr enableIfFloating<T,T> deg_to_arcminute = static_cast<T>(degrees_to_minutes);
template<typename T>
constexpr enableIfFloating<T,T> arcminute_to_deg = one<T> / static_cast<T>(degrees_to_minutes);
template<typename T>
constexpr enableIfFloating<T,T> deg_to_arcsecond = static_cast<T>(degrees_to_seconds);
template<typename T>
constexpr enableIfFloating<T,T> arcsecond_to_deg = one<T> / static_cast<T>(degrees_to_seconds);
template<typename T>
constexpr enableIfFloating<T,T> rad_to_grad = static_cast<T>(half_circle_grad) / pi<T>;
template<typename T>
constexpr enableIfFloating<T,T> grad_to_rad = pi<T> / static_cast<T>(half_circle_grad);
template<typename T>
constexpr enableIfFloating<T,T> deg_to_grad = static_cast<T>(half_circle_grad) / static_cast<T>(half_circle_deg);
template<typename T>
constexpr enableIfFloating<T,T> grad_to_deg = static_cast<T>(0.9);


}

/*!
  * \brief "clamping" a value between a pair of boundary values
  * If value compares less than min, returns min; otherwise if max 
  * compares less than value, returns max; otherwise returns value.
  * \param[int] value
  * \param[int] min 
  * \param[int] max
  */
template<typename T>
constexpr auto clamp(const T& value, const T& min, const T& max) -> T
{
#if (CPP_VERSION >= 17)
    return std::clamp(value, min, max);
#else
    return std::max(min, std::min(max, value));
#endif
}

/*!
 * \brief 2D Vector module
 */
template<typename T>
auto module(T a, T b) -> std::enable_if_t<
    std::is_integral<T>::value,
    double>
{
    if (a == 0 && b == 0) return 0.;

    auto result = std::minmax(std::abs(a), std::abs(b));
    double div = static_cast<double>(result.first) /
        static_cast<double>(result.second);
    return static_cast<double>(result.second) * sqrt(1. + div * div);
}

template<typename T>
auto module(T a, T b) -> std::enable_if_t<
    std::is_floating_point<T>::value, T>
{
    if (a == consts::zero<T> && b == consts::zero<T>) return consts::zero<T>;

    auto result = std::minmax(std::abs(a), std::abs(b));
    T div = static_cast<T>(result.first) / static_cast<T>(result.second);
    return result.second * std::sqrt(consts::one<T> +div * div);
}


template<typename T>
auto isNearlyEqual(T a, T b) -> std::enable_if_t<
    std::is_floating_point<T>::value,
    bool>
{
    if (a == b) return true;

    T diff = std::abs(a - b);
    if (diff >= consts::one<T>) return false;

    T epsilon = std::numeric_limits<T>::epsilon() * T { 128 };
    T norm = std::min((std::abs(a) + std::abs(b)), std::numeric_limits<T>::max());

    return diff < std::max(std::numeric_limits<T>::min(), epsilon * norm);
}


template<typename T>
auto isZero(T value) -> std::enable_if_t<
    std::is_integral<T>::value,
    bool>
{
    return value == consts::zero<T>;
}

template<typename T>
auto isZero(T value) -> std::enable_if_t<
    std::is_floating_point<T>::value,
    bool>
{
    T up = std::nextafter(consts::zero<T>, consts::one<T>);
    T down = std::nextafter(consts::zero<T>, -consts::one<T>);
    return (down <= value && value <= up);
}

template<typename T>
auto isNearlyZero(T value) -> std::enable_if_t<
    std::is_floating_point<T>::value,
    bool>
{
    return isNearlyEqual(value, consts::zero<T>);
}


/*! \} */ // end of math

} // End namespace tl

