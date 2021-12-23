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
 * License along with Foobar. If not, see <http://www.gnu.org/licenses/>. *
 *                                                                        *
 * @license LGPL-3.0 <https://www.gnu.org/licenses/lgpl-3.0.html>         *
 *                                                                        *
 **************************************************************************/

#ifndef TL_CORE_MATH_H
#define TL_CORE_MATH_H

#include "config_tl.h"
#include "tidop/core/defs.h"

#include <cmath>
#include <type_traits>
#include <algorithm>

namespace tl
{



namespace math
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
constexpr typename std::enable_if<std::is_floating_point<T>::value, T>::type half = static_cast<T>(0.5L);

template<typename T>
constexpr typename std::enable_if<std::is_floating_point<T>::value, T>::type pi = static_cast<T>(3.141592653589793238462643383279502884L);
template<typename T>
constexpr typename std::enable_if<std::is_floating_point<T>::value, T>::type half_pi = static_cast<T>(1.570796326794896619231321691639751442L);
template<typename T>
constexpr typename std::enable_if<std::is_floating_point<T>::value, T>::type two_pi = static_cast<T>(6.283185307179586476925286766559005768L);
template<typename T>
constexpr typename std::enable_if<std::is_floating_point<T>::value, T>::type inv_pi = static_cast<T>(1.L / 3.141592653589793238462643383279502884L);
template<typename T>
constexpr typename std::enable_if<std::is_floating_point<T>::value, T>::type sqrt2 = static_cast<T>(1.414213562373095048801688724209698079L);
template<typename T>
constexpr typename std::enable_if<std::is_floating_point<T>::value, T>::type sqrt3 = static_cast<T>(1.7320508075688772935274463415059L);
template<typename T>
constexpr typename std::enable_if<std::is_floating_point<T>::value, T>::type inv_sqrt2 = static_cast<T>(1.L / 1.414213562373095048801688724209698079L);
template<typename T>
constexpr typename std::enable_if<std::is_floating_point<T>::value, T>::type e = static_cast<T>(2.718281828459045235360287471352662498L);
template<typename T>
constexpr typename std::enable_if<std::is_floating_point<T>::value, T>::type log2e = static_cast<T>(1.442695040888963407359924681001892137L);
template<typename T>
constexpr typename std::enable_if<std::is_floating_point<T>::value, T>::type log10e = static_cast<T>(0.4342944819032518276511289189166050823L);
template<typename T>
constexpr typename std::enable_if<std::is_floating_point<T>::value, T>::type ln2 = static_cast<T>(0.6931471805599453094172321214581765681L);
template<typename T>
constexpr typename std::enable_if<std::is_floating_point<T>::value, T>::type ln10 = static_cast<T>(2.302585092994045684017991454684364208L);



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
constexpr typename std::enable_if<std::is_floating_point<T>::value, T>::type rad_to_deg = static_cast<T>(180) / pi<T>;
template<typename T>
constexpr typename std::enable_if<std::is_floating_point<T>::value, T>::type deg_to_rad = pi<T> / static_cast<T>(180);
template<typename T>
constexpr typename std::enable_if<std::is_floating_point<T>::value, T>::type rad_to_grad = static_cast<T>(200) / pi<T>;
template<typename T>
constexpr typename std::enable_if<std::is_floating_point<T>::value, T>::type grad_to_rad = pi<T> / static_cast<T>(200);
template<typename T>
constexpr typename std::enable_if<std::is_floating_point<T>::value, T>::type deg_to_grad = static_cast<T>(200) / static_cast<T>(180);
template<typename T>
constexpr typename std::enable_if<std::is_floating_point<T>::value, T>::type grad_to_deg = static_cast<T>(0.9);


}

/*!
  * \brief "clamping" a value between a pair of boundary values
  * If value compares less than min, returns min; otherwise if max 
  * compares less than value, returns max; otherwise returns value.
  * \param[int] value
  * \param[int] min 
  * \param[int] max
  */
template<typename T> inline
T clamp(const T &value, const T &min, const T &max)
{
  return std::max(min, std::min(max, value));
}

/*!
 * \brief 2D Vector module
 * \param[in] v Vector
 */
template<typename T> inline
typename std::enable_if<
    std::is_integral<T>::value,
    double>::type
module(T a, T b)
{
  auto result = std::minmax(abs(a), abs(b));
  double div = static_cast<double>(result.first) /
               static_cast<double>(result.second);
  return static_cast<double>(result.second) * sqrt(1. + div * div);
}

template<typename T> inline
typename std::enable_if<
    std::is_floating_point<T>::value,T>::type
module(T a, T b)
{
  auto result = std::minmax(abs(a), abs(b));
  T div = static_cast<T>(result.first) / static_cast<T>(result.second);
  return result.second * sqrt(static_cast<T>(1) + div * div);
}

/*! \} */ // end of math

} // Fin namespace math

} // End namespace tl

#endif // TL_CORE_MATH_H
