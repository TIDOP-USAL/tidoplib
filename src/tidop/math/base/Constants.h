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

namespace tl
{

/*!
 * \addtogroup MathTools
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
template<Floating T>
constexpr T one_half = static_cast<T>(0.5L);



/*!
 * \brief `pi<T>`: The value of π (3.14159...).
 * \tparam T Type of the value.
 */
template<Floating T>
constexpr T pi = static_cast<T>(3.141592653589793238462643383279502884L);

/*!
 * \brief `half_pi<T>`: Half of π.
 * \tparam T Type of the value.
 */
template<Floating T>
constexpr T half_pi = static_cast<T>(1.570796326794896619231321691639751442L);

/*!
 * \brief `two_pi<T>`: Twice π.
 * \tparam T Type of the value.
 */
template<Floating T>
constexpr T two_pi = static_cast<T>(6.283185307179586476925286766559005768L);

/*!
 * \brief `inv_pi<T>`: The reciprocal of π (1/π).
 * \tparam T Type of the value.
 */
template<Floating T>
constexpr T inv_pi = static_cast<T>(1.L / 3.141592653589793238462643383279502884L);

/*!
 * \brief `sqrt2<T>`: The square root of 2.
 * \tparam T Type of the value.
 */
template<Floating T>
constexpr T sqrt2 = static_cast<T>(1.414213562373095048801688724209698079L);

/*!
 * \brief `sqrt3<T>`: The square root of 3.
 * \tparam T Type of the value.
 */
template<Floating T>
constexpr T sqrt3 = static_cast<T>(1.7320508075688772935274463415059L);

/*!
 * \brief `inv_sqrt2<T>`: The reciprocal of the square root of 2 (1/√2).
 * \tparam T Type of the value.
 */
template<Floating T>
constexpr T inv_sqrt2 = static_cast<T>(1.L / 1.414213562373095048801688724209698079L);

/*!
 * \brief `e<T> `: The base of the natural logarithm(e ≈ 2.718).
 * \tparam T Type of the value.
 */
template<Floating T>
constexpr T e = static_cast<T>(2.718281828459045235360287471352662498L);

/*!
 * \brief `log2e<T> `: The base-2 logarithm of e.
 * \tparam T Type of the value.
 */
template<Floating T>
constexpr T log2e = static_cast<T>(1.442695040888963407359924681001892137L);

/*!
 * \brief `log10e<T> `: The base-10 logarithm of e.
 * \tparam T Type of the value.
 */
template<Floating T>
constexpr T log10e = static_cast<T>(0.4342944819032518276511289189166050823L);

/*!
 * \brief `ln2<T> `: The natural logarithm of 2.
 * \tparam T Type of the value.
 */
template<Floating T>
constexpr T ln2 = static_cast<T>(0.6931471805599453094172321214581765681L);

/*!
 * \brief `ln10<T> `: The natural logarithm of 10.
 * \tparam T Type of the value.
 */
template<Floating T>
constexpr T ln10 = static_cast<T>(2.302585092994045684017991454684364208L);



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
template<Floating T>
constexpr T rad_to_deg = static_cast<T>(half_circle_deg) / pi<T>;

/*!
 * \brief `deg_to_rad<T>`: Converts degrees to radians.
 * \tparam T Type of the value.
 */
template<Floating T>
constexpr T deg_to_rad = pi<T> / static_cast<T>(half_circle_deg);

/*!
 * \brief `deg_to_arcminute<T>`: Converts degrees to arcminutes.
 * \tparam T Type of the value.
 */
template<Floating T>
constexpr T deg_to_arcminute = static_cast<T>(degrees_to_minutes);

/*!
 * \brief `arcminute_to_deg<T>`: Converts arcminutes to degrees.
 * \tparam T Type of the value.
 */
template<Floating T>
constexpr T arcminute_to_deg = one<T> / static_cast<T>(degrees_to_minutes);

/*!
 * \brief `deg_to_arcsecond<T>`: Converts degrees to arcseconds.
 * \tparam T Type of the value.
 */
template<Floating T>
constexpr T deg_to_arcsecond = static_cast<T>(degrees_to_seconds);

/*!
* \brief `arcsecond_to_deg<T>`: Converts arcseconds to degrees.
* \tparam T Type of the value.
*/
template<Floating T>
constexpr T arcsecond_to_deg = one<T> / static_cast<T>(degrees_to_seconds);

/*!
 * \brief `rad_to_grad<T>`: Converts radians to gradians.
 * \tparam T Type of the value.
 */
template<Floating T>
constexpr T rad_to_grad = static_cast<T>(half_circle_grad) / pi<T>;

/*!
 * \brief `grad_to_rad<T>`: Converts gradians to radians.
 * \tparam T Type of the value.
 */
template<Floating T>
constexpr T grad_to_rad = pi<T> / static_cast<T>(half_circle_grad);

/*!
 * \brief `deg_to_grad<T>`: Converts degrees to gradians.
 * \tparam T Type of the value.
 */
template<Floating T>
constexpr T deg_to_grad = static_cast<T>(half_circle_grad) / static_cast<T>(half_circle_deg);

/*!
* \brief `grad_to_deg<T>`: Converts gradians to degrees.
* \tparam T Type of the value.
*/
template<Floating T>
constexpr T grad_to_deg = static_cast<T>(0.9);

}


} // namespace tl