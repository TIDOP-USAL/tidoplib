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

#include <tidop/config.h>

namespace tl
{

/*!
 * \defgroup type_traits Template Type Utilities
 * \ingroup core
 * \brief Utility aliases for constraining templates using SFINAE based on C++ type traits.
 *
 * This group provides convenient aliases like enableIfFloating and enableIfIntegral
 * to conditionally enable or disable template functions and classes, making generic
 * programming more robust and readable.
 * \{
 */


/*!
 * \brief Enables the template only if the type T is an integral type (e.g., int, short, unsigned long).
 *
 * This alias facilitates Substitution Failure Is Not An Error (SFINAE) by yielding the type R
 * if T satisfies std::is_integral. Otherwise, it causes a substitution failure, effectively
 * disabling the template instantiation for non-integral types.
 *
 * \tparam T The type being checked for integrality.
 * \tparam R The result type if the condition is true (defaults to void).
 */
template<typename T, class R = void>
using enableIfIntegral = std::enable_if_t<std::is_integral<T>::value, R>;

/*!
 * \brief Enables the template only if the type T is a floating-point type (e.g., float, double).
 *
 * This alias is used for SFINAE, yielding the type R if T satisfies std::is_floating_point.
 * Otherwise, it causes a substitution failure, preventing instantiation for non-floating types.
 *
 * \tparam T The type being checked for floating-point status.
 * \tparam R The result type if the condition is true (defaults to void).
 */
template<typename T, class R = void>
using enableIfFloating = std::enable_if_t<std::is_floating_point<T>::value, R>;

/*!
 * \brief Enables the template only if the type T is an arithmetic type (integral or floating-point).
 *
 * Used for SFINAE to ensure T is either integral or floating-point (satisfies std::is_arithmetic).
 * Yields R on success; otherwise, it causes a substitution failure.
 *
 * \tparam T The type being checked for arithmetic status.
 * \tparam R The result type if the condition is true (defaults to void).
 */
template<typename T, class R = void>
using enableIfArithmetic = std::enable_if_t<std::is_arithmetic<T>::value, R>;

/*!
 * \brief Enables the template only if the type T is an unsigned integral type (e.g., unsigned int, unsigned short).
 *
 * This alias facilitates Substitution Failure Is Not An Error (SFINAE) by yielding the type R
 * if T satisfies std::is_unsigned. Otherwise, it causes a substitution failure, effectively
 * disabling the template instantiation for signed, floating-point, or non-arithmetic types.
 *
 * \tparam T The type being checked for unsigned status.
 * \tparam R The result type if the condition is true (defaults to void).
 */
template<typename T, class R = void>
using enableIfUnsigned = std::enable_if_t<std::is_unsigned<T>::value, R>;

/*!
 * \brief Enables the template only if the type T is exactly 'float' (ignoring const/volatile qualifiers).
 *
 * This is a highly specific SFINAE constraint, checking for exact type equality with float
 * after removing const/volatile qualifiers (std::remove_cv_t).
 *
 * \tparam T The type being checked for equality with float.
 * \tparam R The result type if the condition is true (defaults to void).
 */
template<typename T, class R = void>
using enableIfFloat = std::enable_if_t<std::is_same<float, std::remove_cv_t<T>>::value, R>;

/*!
 * \brief Enables the template only if the type T is exactly 'double' (ignoring const/volatile qualifiers).
 *
 * A specific SFINAE constraint, checking for exact type equality with double
 * after removing const/volatile qualifiers (std::remove_cv_t).
 *
 * \tparam T The type being checked for equality with double.
 * \tparam R The result type if the condition is true (defaults to void).
 */
template<typename T, class R = void>
using enableIfDouble = std::enable_if_t<std::is_same<double, std::remove_cv_t<T>>::value, R>;

/*!
 * \brief Enables the template only if the type T is exactly 'bool' (ignoring const/volatile qualifiers).
 *
 * A specific SFINAE constraint, checking for exact type equality with bool
 * after removing const/volatile qualifiers (std::remove_cv_t).
 *
 * \tparam T The type being checked for equality with bool.
 * \tparam R The result type if the condition is true (defaults to void).
 */
template<typename T, class R = void>
using enableIfBool = std::enable_if_t<std::is_same<bool, std::remove_cv_t<T>>::value, R>;

/*!
 * \brief Enables the template only if the type T is NOT an integral type.
 *
 * This is the inverse of enableIfIntegral, yielding R only if T fails std::is_integral.
 *
 * \tparam T The type being checked for non-integrality.
 * \tparam R The result type if the condition is true (defaults to void).
 */
template<typename T, class R = void>
using enableIfNotIntegral = std::enable_if_t<!std::is_integral<T>::value, R>;

/*!
 * \brief Enables the template only if the type T is NOT a floating-point type.
 *
 * This is the inverse of enableIfFloating, yielding R only if T fails std::is_floating_point.
 *
 * \tparam T The type being checked for non-floating-point status.
 * \tparam R The result type if the condition is true (defaults to void).
 */
template <typename T, class R = void>
using enableIfNotFloating = std::enable_if_t<!std::is_floating_point<T>::value, R>;

/*!
 * \brief Enables the template only if the type T is NOT an arithmetic type.
 *
 * This is the inverse of enableIfArithmetic, yielding R only if T fails std::is_arithmetic.
 *
 * \tparam T The type being checked for non-arithmetic status.
 * \tparam R The result type if the condition is true (defaults to void).
 */
template<typename T, class R = void>
using enableIfNotArithmetic = std::enable_if_t<!std::is_arithmetic<T>::value, R>;

/*!
 * \brief Enables the template only if the type T is NOT an unsigned integral type.
 *
 * This is the inverse of enableIfUnsigned, yielding R only if T fails std::is_unsigned
 * (i.e., T is a signed integral, floating-point, or non-arithmetic type).
 *
 * \tparam T The type being checked for non-unsigned status.
 * \tparam R The result type if the condition is true (defaults to void).
 */
template<typename T, class R = void>
using enableIfNotUnsigned = std::enable_if_t<!std::is_unsigned<T>::value, R>;

/*!
 * \brief Trait alias to extract the value type of an iterator.
 *
 * Simplifies accessing the typename std::iterator_traits<It>::value_type.
 *
 * \tparam It The iterator type.
 */
template<typename It>
using iteratorValueType = typename std::iterator_traits<It>::value_type;

/*! \} */

}