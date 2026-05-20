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

/*!
 * \file TypeConversions.h
 * \brief Type conversion utilities for numeric and string conversions
 *
 * This module provides utilities for converting between different types, including
 * numeric type casting, string-to-numeric conversions with validation, and precision
 * control for floating-point to string conversion.
 *
 * ### Functions
 *
 * - \ref numberCast - Numeric type casting with rounding for integral types
 * - \ref convertStringTo - String to numeric/boolean conversion with range validation
 * - \ref toStringWithPrecision - Floating-point to string with precision control
 * - \ref roundToInteger - Floating-point rounding to integer
 * - \ref stringToInteger - String to integer with configurable base (octal, decimal, hex)
 *
 * ### Features
 *
 * - Type-safe numeric conversions
 * - Range validation for all numeric types
 * - Support for octal, decimal, and hexadecimal bases
 * - Precise floating-point to string conversion
 * - Proper error handling with detailed exception messages
 * - Specializations for small integer types (int8_t, uint8_t)
 * - Support for 64-bit integer types (int64_t, uint64_t)
 * - Boolean string parsing ("true"/"false" or "1"/"0")
 *
 * \see numberCast, convertStringTo, stringToInteger, Base
 */
 
#pragma once

#include "tidop/config.h"

#include <vector>
#include <string>
#include <numeric>
#include <sstream>
#include <iomanip>

#include "tidop/core/base/Exception.h"
#include "tidop/core/base/StringUtils.h"
#include "tidop/core/base/type.h"

namespace tl
{

/*!
 * \addtogroup Base
 *
 * \{
 */


/*!
 * \brief Performs a cast from one numeric type to another, with automatic rounding for integral types.
 *
 * This function safely converts a numeric value from type T2 to type T1. For integral target types,
 * the source value is rounded to the nearest integer. For floating-point target types, the value
 * is converted directly without rounding.
 *
 * ### Template Specializations
 *
 * - **Integral T1**: Rounds the source value before casting
 * - **Floating-point T1**: Direct cast without rounding
 * - **Non-arithmetic T1**: Throws an exception
 *
 * ### Parameters
 *
 * - `number` - The value to cast
 *
 * ### Returns
 *
 * The value cast to type T1
 *
 * ### Example
 *
 * \code{.cpp}
 * int rounded = tl::numberCast<int>(3.7);          // Result: 4 (rounded)
 * float converted = tl::numberCast<float>(42);     // Result: 42.0f
 * \endcode
 *
 * \tparam T1 The target numeric type
 * \tparam T2 The source numeric type
 * \param[in] number The number to cast
 * \return The number cast to type T1
 *
 * \exception Exception If T1 is not an arithmetic type
 *
 * \see convertStringTo
 */
template<typename T1, typename T2>
    requires std::is_arithmetic_v<T1> &&std::is_arithmetic_v<T2>
constexpr T1 numberCast(T2 number) noexcept
{
    if constexpr (std::is_integral_v<T1>) {
        return static_cast<T1>(std::round(number));
    } else {
        return static_cast<T1>(number);
    }
}


/*!
 * \brief Converts a string to a numeric or boolean type, with range validation.
 *
 * This function converts an input string to the specified numeric type T.
 * It ensures that the value falls within the valid range for T using std::numeric_limits.
 * Specialized implementations are provided for `int8_t`, `uint8_t`, `int64_t`, `uint64_t`, and `bool`.
 *
 * ### Supported Types
 *
 * - Integral types: `int`, `long`, `long long`, `short`, `int8_t`, `uint8_t`, `int64_t`, `uint64_t`
 * - Floating-point types: `float`, `double`, `long double`
 * - Boolean: `bool` (accepts "true", "1", "false", "0")
 *
 * \tparam T The target type to convert the string into. Supported types include numeric types and `bool`.
 * \param[in] str The input string to convert
 * \return The converted value as type T
 *
 * \note For `bool`, the function accepts "true", "1" (evaluating to \c true) and "false", "0" (evaluating to \c false).
 * \exception Exception Throws an exception if the input string is invalid or if the value is out of range.
 * 
 * ### Example Usage
 * \code{.cpp}
 * // Converting integers
 * int value1 = convertStringTo<int>("42");       // value1 = 42
 * uint8_t value2 = convertStringTo<uint8_t>("255"); // value2 = 255
 *
 * // Converting floating-point numbers
 * float value3 = convertStringTo<float>("3.14");   // value3 = 3.14f
 *
 * // Converting booleans
 * bool value4 = convertStringTo<bool>("true");     // value4 = true
 * bool value5 = convertStringTo<bool>("0");        // value5 = false
 *
 * // Handling out-of-range values
 * try {
 *     uint8_t value6 = convertStringTo<uint8_t>("300"); // Throws an exception: Value out of range
 * } catch (const Exception &e) {
 *     std::cerr << e.what() << std::endl;
 * }
 * \endcode
 */
template <typename T>
auto convertStringTo(const std::string &str) -> std::enable_if_t<std::is_arithmetic<T>::value && !std::is_same<T, bool>::value, T>
{
    using Limits = std::numeric_limits<T>;
    try {
        long double value = std::stold(str);
        if (value < Limits::lowest() || value > Limits::max())
            throw std::out_of_range("Value out of range");
        return static_cast<T>(value);
    } catch (const std::out_of_range &) {
        TL_THROW_EXCEPTION("Value out of range for {}: {}", TypeTraits<T>::name_type, str);
    } catch (const std::invalid_argument &) {
        TL_THROW_EXCEPTION("Invalid argument for {}: {}", TypeTraits<T>::name_type, str);
    }
}

/// \cond

template <>
inline auto convertStringTo<int8_t>(const std::string &str) -> int8_t
{
    using Limits = std::numeric_limits<int8_t>;
    try {
        int value = std::stoi(str);
        if (value < Limits::min() || value > Limits::max())
            throw std::out_of_range("Index out of range");
        return static_cast<int8_t>(value);
    } catch (const std::out_of_range &) {
        TL_THROW_EXCEPTION("Value out of range for int8_t: {}", str);
    } catch (const std::invalid_argument &) {
        TL_THROW_EXCEPTION("Invalid argument for int8_t: {}", str);
    }
}

template <>
inline auto convertStringTo<uint8_t>(const std::string &str) -> uint8_t
{
    try {
        int value = std::stoi(str);
        if (value < 0 || value > std::numeric_limits<uint8_t>::max())
            throw std::out_of_range("Index out of range");
        return static_cast<uint8_t>(value);
    } catch (const std::out_of_range &) {
        TL_THROW_EXCEPTION("Value out of range for uint8_t: {}", str);
    } catch (const std::invalid_argument &) {
        TL_THROW_EXCEPTION("Invalid argument for uint8_t: {}", str);
    }
}

template <>
inline auto convertStringTo<int64_t>(const std::string &str) -> int64_t
{
    try {
        return std::stoll(str);
    } catch (const std::out_of_range &) {
        TL_THROW_EXCEPTION("Value out of range for int64_t: {}", str);
    } catch (const std::invalid_argument &) {
        TL_THROW_EXCEPTION("Invalid argument for int64_t: {}", str);
    }
}

template <>
inline auto convertStringTo<uint64_t>(const std::string &str) -> uint64_t
{
    try {
        if (str[0] == '-') {
            throw std::out_of_range("Index out of range");
        }
        return std::stoull(str);
    } catch (const std::out_of_range &) {
        TL_THROW_EXCEPTION("Value out of range for uint64_t: {}", str);
    } catch (const std::invalid_argument &) {
        TL_THROW_EXCEPTION("Invalid argument for uint64_t: {}", str);
    }
}

template <typename T>
auto convertStringTo(const std::string &str) -> enableIfBool<T,T>
{
    if (str == "true" || str == "1") {
        return true;
    } else if (str == "false" || str == "0") {
        return false;
    }

    TL_THROW_EXCEPTION("Invalid value for bool");
}


template <typename T>
auto convertStringTo(const std::string &/*str*/) -> enableIfNotArithmetic<T,T>
{
    //En linux me sale siempre el error aunque no se llame a la función.
    //TL_COMPILER_WARNING("Invalid conversion. It isn't an arithmetic type.")
    throw Exception("Invalid conversion. It isn't an arithmetic type.", __FILE__, __LINE__, TL_FUNCTION);
    return T{0};
}

/// \endcond


/*!
 * \brief Converts a floating-point number to a string with specified precision.
 *
 * This function provides a convenient way to convert floating-point values to strings
 * with a specified number of decimal places. Uses fixed-point notation with the
 * specified precision.
 *
 * ### Example
 *
 * \code{.cpp}
 * std::string str1 = tl::toStringWithPrecision(3.14159, 2);   // "3.14"
 * std::string str2 = tl::toStringWithPrecision(2.71828, 3);   // "2.718"
 * std::string str3 = tl::toStringWithPrecision(1.5, 4);       // "1.5000"
 * \endcode
 *
 * \tparam T The floating-point type
 * \param[in] value The value to convert
 * \param[in] precision The number of decimal places
 * \return String representation with specified precision
 *
 * \see convertStringTo
 */
template <typename T>
auto toStringWithPrecision(T value, int precision) -> std::string
{
    std::ostringstream out;
    out << std::fixed << std::setprecision(precision) << value;
    return out.str();
}



/*!
 * \enum Base
 * \brief Numeric bases for string-to-integer conversion.
 *
 * Enumeration specifying the base (radix) for converting strings to integers.
 * Commonly used bases are supported: octal (8), decimal (10), and hexadecimal (16).
 *
 * ### Values
 *
 * - `octal` - Base 8 (digits 0-7)
 * - `decimal` - Base 10 (digits 0-9)
 * - `hexadecimal` - Base 16 (digits 0-9, A-F)
 *
 * \see stringToInteger
 */
enum class Base : int8_t
{
    octal = 8,        /*!< Octal base (radix 8). */
    decimal = 10,     /*!< Decimal base (radix 10). */
    hexadecimal = 16  /*!< Hexadecimal base (radix 16). */
};


/*!
 * \brief Converts a string to an integer, considering its numeric base.
 *
 * This function converts a string representation of an integer to an `int` value,
 * using the specified base (octal, decimal, or hexadecimal). The string format
 * should follow the conventions of the specified base.
 *
 * ### Supported Formats
 *
 * - **Decimal**: Normal digits (0-9)
 * - **Octal**: Digits 0-7
 * - **Hexadecimal**: Digits 0-9 and letters A-F (case-insensitive)
 *
 * ### Example
 *
 * \code{.cpp}
 * int dec = tl::stringToInteger("42");                              // 42
 * int hex = tl::stringToInteger("FF", tl::Base::hexadecimal);       // 255
 * int oct = tl::stringToInteger("77", tl::Base::octal);             // 63
 * int neg = tl::stringToInteger("-10", tl::Base::decimal);          // -10
 * \endcode
 *
 * \param[in] text The input string representing a number.
 * \param[in] base The numeric base (e.g., octal, decimal, hexadecimal).
 * \return The integer value represented by the string.
 * \see convertStringTo, Base
 */
TL_EXPORT int stringToInteger(const std::string &text, Base base = Base::decimal);

/*!
 * \brief Rounds a floating-point number and converts it to an integer.
 *
 * This function rounds a floating-point value to the nearest integer and performs
 * a safe conversion to `int`. This avoids compiler warnings (e.g., C4244 in MSVC)
 * about implicit floating-point to integer conversions and ensures proper rounding
 * semantics.
 *
 * ### Example
 *
 * \code{.cpp}
 * int val1 = tl::roundToInteger(3.4);    // 3
 * int val2 = tl::roundToInteger(3.5);    // 4 (banker's rounding or standard rounding)
 * int val3 = tl::roundToInteger(3.7);    // 4
 * \endcode
 *
 * \tparam T The floating-point type.
 * \param[in] n The number to round.
 * \return The rounded integer value.
 * \details This avoids warnings (e.g., C4244) about implicit conversions and ensures proper rounding.
 */
template<typename T>
constexpr auto roundToInteger(T n) 
{
    return static_cast<int>(round(n));
}

/*! \} */


} // End namespace tl

