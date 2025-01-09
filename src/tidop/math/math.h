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

#include "tidop/core/base/defs.h"

#include <cmath>
#include <type_traits>
#include <algorithm>

#include "tidop/core/base/common.h"

namespace tl
{


/*!
 * \defgroup math Mathematical module
 *
 * \{
 */


/*!
 * \defgroup Constants Constants
 *
 * \brief Definition of commonly used mathematical and geometric constants.
 * 
 * This module provides a comprehensive set of predefined constants, including:
 * - General mathematical constants (e.g., π, e, √2).
 * - Floating-point friendly templates for high precision.
 * - Conversion factors for angles between different units (degrees, radians, gradians).
 * \{
 */

namespace consts
{

/*!
 * \brief `zero<T>`: Represents the value 0 for the specified type `T`.
 * \tparam T Type of the value.
 */
template<typename T>
constexpr T zero = static_cast<T>(0); 

/*!
 * \brief `one<T>`: Represents the value 1 for the specified type `T`.
 * \tparam T Type of the value.
 */
template<typename T>
constexpr T one = static_cast<T>(1);

/*!
 * \brief `two<T>`: Represents the value 2 for the specified type `T`.
 * \tparam T Type of the value.
 */
template<typename T>
constexpr T two = static_cast<T>(2);

/*!
 * \brief `one_half<T>`: Represents the fraction 1/2 for the specified type `T` (only for floating-point types).
 * \tparam T Type of the value.
 */
template<typename T>
constexpr enableIfFloating<T,T> one_half = static_cast<T>(0.5L);



/*!
 * \brief `pi<T>`: The value of π (3.14159...).
 * \tparam T Type of the value.
 */
template<typename T>
constexpr enableIfFloating<T,T> pi = static_cast<T>(3.141592653589793238462643383279502884L);

/*!
 * \brief `half_pi<T>`: Half of π.
 * \tparam T Type of the value.
 */
template<typename T>
constexpr enableIfFloating<T,T> half_pi = static_cast<T>(1.570796326794896619231321691639751442L);

/*!
 * \brief `two_pi<T>`: Twice π.
 * \tparam T Type of the value.
 */
template<typename T>
constexpr enableIfFloating<T,T> two_pi = static_cast<T>(6.283185307179586476925286766559005768L);

/*!
 * \brief `inv_pi<T>`: The reciprocal of π (1/π).
 * \tparam T Type of the value.
 */
template<typename T>
constexpr enableIfFloating<T,T> inv_pi = static_cast<T>(1.L / 3.141592653589793238462643383279502884L);

/*!
 * \brief `sqrt2<T>`: The square root of 2.
 * \tparam T Type of the value.
 */
template<typename T>
constexpr enableIfFloating<T,T> sqrt2 = static_cast<T>(1.414213562373095048801688724209698079L);

/*!
 * \brief `sqrt3<T>`: The square root of 3.
 * \tparam T Type of the value.
 */
template<typename T>
constexpr enableIfFloating<T,T> sqrt3 = static_cast<T>(1.7320508075688772935274463415059L);

/*!
 * \brief `inv_sqrt2<T>`: The reciprocal of the square root of 2 (1/√2).
 * \tparam T Type of the value.
 */
template<typename T>
constexpr enableIfFloating<T,T> inv_sqrt2 = static_cast<T>(1.L / 1.414213562373095048801688724209698079L);

/*!
 * \brief `e<T> `: The base of the natural logarithm(e ≈ 2.718).
 * \tparam T Type of the value.
 */
template<typename T>
constexpr enableIfFloating<T,T> e = static_cast<T>(2.718281828459045235360287471352662498L);

/*!
 * \brief `log2e<T> `: The base-2 logarithm of e.
 * \tparam T Type of the value.
 */
template<typename T>
constexpr enableIfFloating<T,T> log2e = static_cast<T>(1.442695040888963407359924681001892137L);

/*!
 * \brief `log10e<T> `: The base-10 logarithm of e.
 * \tparam T Type of the value.
 */
template<typename T>
constexpr enableIfFloating<T,T> log10e = static_cast<T>(0.4342944819032518276511289189166050823L);

/*!
 * \brief `ln2<T> `: The natural logarithm of 2.
 * \tparam T Type of the value.
 */
template<typename T>
constexpr enableIfFloating<T,T> ln2 = static_cast<T>(0.6931471805599453094172321214581765681L);

/*!
 * \brief `ln10<T> `: The natural logarithm of 10.
 * \tparam T Type of the value.
 */
template<typename T>
constexpr enableIfFloating<T,T> ln10 = static_cast<T>(2.302585092994045684017991454684364208L);



/*! 
 * \defgroup angleConversion Angle conversion
 * \brief Provides constants and templates for converting between angle formats.
 * 
 * This module defines conversion factors and templates to facilitate angle conversions between degrees, radians, and gradians.
 * 
 * Angle constants for common geometric divisions:
 *
 * - `full_circle_deg`: 360 degrees.
 * - `full_circle_grad`: 400 gradians.
 * - `half_circle_deg`: 180 degrees.
 * - `half_circle_grad`: 200 gradians.
 * - `quarter_circle_deg`: 90 degrees.
 * - `quarter_circle_grad`: 100 gradians.
 * 
 * Conversion factors for degrees, minutes, and seconds:
 *
 * - `degrees_to_minutes`: 60 (1 degree = 60 arcminutes).
 * - `minutes_to_seconds`: 60 (1 arcminute = 60 arcseconds).
 * - `degrees_to_seconds`: 3600 (1 degree = 3600 arcseconds).
 * 
 * Angle conversion templates:
 *
 * - `rad_to_deg<T>`: Converts radians to degrees.
 * - `deg_to_rad<T>`: Converts degrees to radians.
 * - `deg_to_arcminute<T>`: Converts degrees to arcminutes.
 * - `arcminute_to_deg<T>`: Converts arcminutes to degrees.
 * - `deg_to_arcsecond<T>`: Converts degrees to arcseconds.
 * - `arcsecond_to_deg<T>`: Converts arcseconds to degrees.
 * - `rad_to_grad<T>`: Converts radians to gradians.
 * - `grad_to_rad<T>`: Converts gradians to radians.
 * - `deg_to_grad<T>`: Converts degrees to gradians.
 * - `grad_to_deg<T>`: Converts gradians to degrees.
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

/*!
 * \brief `rad_to_deg<T>`: Converts radians to degrees.
 * \tparam T Type of the value.
 */
template<typename T>
constexpr enableIfFloating<T,T> rad_to_deg = static_cast<T>(half_circle_deg) / pi<T>;

/*!
 * \brief `deg_to_rad<T>`: Converts degrees to radians.
 * \tparam T Type of the value.
 */
template<typename T>
constexpr enableIfFloating<T,T> deg_to_rad = pi<T> / static_cast<T>(half_circle_deg);

/*!
 * \brief `deg_to_arcminute<T>`: Converts degrees to arcminutes.
 * \tparam T Type of the value.
 */
template<typename T>
constexpr enableIfFloating<T,T> deg_to_arcminute = static_cast<T>(degrees_to_minutes);

/*!
 * \brief `arcminute_to_deg<T>`: Converts arcminutes to degrees.
 * \tparam T Type of the value.
 */
template<typename T>
constexpr enableIfFloating<T,T> arcminute_to_deg = one<T> / static_cast<T>(degrees_to_minutes);

/*!
 * \brief `deg_to_arcsecond<T>`: Converts degrees to arcseconds.
 * \tparam T Type of the value.
 */
template<typename T>
constexpr enableIfFloating<T,T> deg_to_arcsecond = static_cast<T>(degrees_to_seconds);

/*!
* \brief `arcsecond_to_deg<T>`: Converts arcseconds to degrees.
* \tparam T Type of the value.
*/
template<typename T>
constexpr enableIfFloating<T,T> arcsecond_to_deg = one<T> / static_cast<T>(degrees_to_seconds);

/*!
 * \brief `rad_to_grad<T>`: Converts radians to gradians.
 * \tparam T Type of the value.
 */
template<typename T>
constexpr enableIfFloating<T,T> rad_to_grad = static_cast<T>(half_circle_grad) / pi<T>;

/*!
 * \brief `grad_to_rad<T>`: Converts gradians to radians.
 * \tparam T Type of the value.
 */
template<typename T>
constexpr enableIfFloating<T,T> grad_to_rad = pi<T> / static_cast<T>(half_circle_grad);

/*!
 * \brief `deg_to_grad<T>`: Converts degrees to gradians.
 * \tparam T Type of the value.
 */
template<typename T>
constexpr enableIfFloating<T,T> deg_to_grad = static_cast<T>(half_circle_grad) / static_cast<T>(half_circle_deg);

/*!
* \brief `grad_to_deg<T>`: Converts gradians to degrees.
* \tparam T Type of the value.
*/
template<typename T>
constexpr enableIfFloating<T,T> grad_to_deg = static_cast<T>(0.9);

/*! \} */ 

}

/*! \} */

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
 * \brief Computes the module (magnitude) of a 2D vector for integral types.
 *
 * Uses a numerically stable algorithm to calculate the magnitude of the vector (a, b).
 *
 * \tparam T Integral type.
 * \param a X-coordinate of the vector.
 * \param b Y-coordinate of the vector.
 * \return The module of the vector as a double.
 */
template<typename T>
auto module(T a, T b) -> enableIfIntegral<T, double>
{
    if (a == 0 && b == 0) return 0.;

    auto result = std::minmax(std::abs(a), std::abs(b));
    double div = static_cast<double>(result.first) /
        static_cast<double>(result.second);
    return static_cast<double>(result.second) * sqrt(1. + div * div);
}

/*!
 * \brief Computes the module (magnitude) of a 2D vector for floating-point types.
 *
 * Uses a numerically stable algorithm to calculate the magnitude of the vector (a, b).
 *
 * \tparam T Floating-point type.
 * \param a X-coordinate of the vector.
 * \param b Y-coordinate of the vector.
 * \return The module of the vector as the same type as the input.
 */
template<typename T>
auto module(T a, T b) -> enableIfFloating<T, T>
{
    if (a == consts::zero<T> && b == consts::zero<T>) return consts::zero<T>;

    auto result = std::minmax(std::abs(a), std::abs(b));
    T div = static_cast<T>(result.first) / static_cast<T>(result.second);
    return result.second * std::sqrt(consts::one<T> +div * div);
}

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
template<typename T>
auto isNearlyEqual(T a, T b) -> enableIfFloating<T, bool>
{
    if (a == b) return true;

    T diff = std::abs(a - b);
    if (diff >= consts::one<T>) return false;

    T epsilon = std::numeric_limits<T>::epsilon() * T { 128 };
    T norm = std::min((std::abs(a) + std::abs(b)), std::numeric_limits<T>::max());

    return diff < std::max(std::numeric_limits<T>::min(), epsilon * norm);
}

/*!
 * \brief Checks if an integral value is zero.
 *
 * \tparam T Integral type.
 * \param value Value to check.
 * \return True if the value is zero, false otherwise.
 */
template<typename T>
auto isZero(T value) -> enableIfIntegral<T, bool>
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
template<typename T>
auto isZero(T value) -> enableIfFloating<T, bool>
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
template<typename T>
auto isNearlyZero(T value) -> enableIfFloating<T, bool>
{
    return isNearlyEqual(value, consts::zero<T>);
}


/*! \} */

} // End namespace tl

