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
#include <charconv>
#include <string_view>

#include "tidop/core/base/Exception.h"
#include "tidop/core/base/StringUtils.h"
#include "tidop/core/base/Type.h"
#include "tidop/core/base/Concepts.h"

namespace tl
{

/*!
 * \addtogroup Base
 *
 * \{
 */


/*!
 * \brief Converts a numeric value from one arithmetic type to another, with
 *        automatic rounding for integral target types.
 *
 * This function safely converts a numeric value of type `T2` to type `T1`.
 * For integral target types (`T1`), the source value is rounded to the nearest
 * integer using `std::round` before casting. For floating-point target types,
 * the value is converted directly without rounding.
 *
 * \tparam T1 Target arithmetic type (must satisfy `Arithmetic` concept).
 * \tparam T2 Source arithmetic type (must satisfy `Arithmetic` concept).
 *
 * \param[in] number The value to cast.
 *
 * \return The value cast to type `T1`.
 *
 * \note The function is `constexpr` and `noexcept`, and can be used in
 *       compile-time contexts when the inputs are constant expressions.
 *
 * ### Example
 * \code
 * int rounded = tl::numberCast<int>(3.7);          // Result: 4 (rounded)
 * int truncated = tl::numberCast<int>(3.2);        // Result: 3 (rounded)
 * float converted = tl::numberCast<float>(42);     // Result: 42.0f
 * double from_int = tl::numberCast<double>(100);   // Result: 100.0
 * \endcode
 *
 * \see convertStringTo
 */
template<Arithmetic T1, Arithmetic T2>
constexpr T1 numberCast(T2 number) noexcept
{
    if constexpr (std::is_integral_v<T1>) {
        return static_cast<T1>(std::round(number));
    } else {
        return static_cast<T1>(number);
    }
}


/*!
 * \brief Converts a string to an arithmetic type (excluding `bool`), with range validation.
 *
 * This function converts an input string to the specified numeric type `T`.
 * It ensures that the value falls within the valid range for `T`.
 * The function handles:
 * - Leading/trailing whitespace removal.
 * - Optional suffixes for floating-point (`f`, `F`) and integral (`u`, `U`, `l`, `L`) types.
 * - Exhaustive input validation (the entire string must be consumed).
 *
 * \tparam T The target arithmetic type (must satisfy `ArithmeticNoBool` concept).
 * \param[in] str The input string to convert.
 * \return The converted value as type `T`.
 *
 * \throws `tl::Exception` if the string is empty, contains invalid characters,
 *         or if the value is out of range for the target type.
 *
 * ### Example
 * \code
 * int value1 = convertStringTo<int>("42");                    // value1 = 42
 * uint8_t value2 = convertStringTo<uint8_t>("255");          // value2 = 255
 * float value3 = convertStringTo<float>("3.14f");            // value3 = 3.14f
 * long value4 = convertStringTo<long>("100L");               // value4 = 100L
 *
 * // Out-of-range example (throws)
 * try {
 *     uint8_t value5 = convertStringTo<uint8_t>("300");      // Throws: Value out of range
 * } catch (const Exception &e) {
 *     std::cerr << e.what() << std::endl;
 * }
 * \endcode
 *
 * \see numberCast, TypeTraits
 */
template <ArithmeticNoBool T>
auto convertStringTo(std::string_view str) -> T
{
    TL_ASSERT(!str.empty(), "Invalid argument for {}: empty string", TypeTraits<T>::name_type);

    while (!str.empty() && std::isspace(str.front())) str.remove_prefix(1);
    while (!str.empty() && std::isspace(str.back()))  str.remove_suffix(1);

    if constexpr (std::is_floating_point_v<T>) {
        if (!str.empty() && (str.back() == 'f' || str.back() == 'F')) {
            str.remove_suffix(1);
        }
    } else if constexpr (std::is_integral_v<T>) {
        while (!str.empty() && (str.back() == 'u' || str.back() == 'U' ||
            str.back() == 'l' || str.back() == 'L')) {
            str.remove_suffix(1);
        }
    }

    T value{};

    auto [ptr, ec] = std::from_chars(str.data(), str.data() + str.size(), value);

    if (ec == std::errc::result_out_of_range) {
        TL_THROW_EXCEPTION("Value out of range for {}: {}", TypeTraits<T>::name_type, str);
    }
    if (ec == std::errc::invalid_argument || ptr != str.data() + str.size()) {
        TL_THROW_EXCEPTION("Invalid argument for {}: {}", TypeTraits<T>::name_type, str);
    }

    return value;
}

/*!
 * \brief Converts a string to a `bool` value.
 *
 * This overload handles the conversion of a string to `bool`.
 * It accepts the strings `"true"`, `"1"`, `"false"`, and `"0"`,
 * ignoring leading/trailing whitespace.
 *
 * \param[in] str The input string to convert.
 * \return `true` if the string is `"true"` or `"1"`; `false` if `"false"` or `"0"`.
 *
 * \throws `tl::Exception` if the string does not match any of the accepted values.
 *
 * ### Example
 * \code
 * bool b1 = convertStringTo<bool>("true");   // b1 = true
 * bool b2 = convertStringTo<bool>("0");      // b2 = false
 * bool b3 = convertStringTo<bool>("1");      // b3 = true
 * bool b4 = convertStringTo<bool>("TRUE");   // Throws (case-sensitive)
 * \endcode
 */
template <typename T>
    requires std::same_as<T, bool>
auto convertStringTo(std::string_view str) -> bool
{
    while (!str.empty() && std::isspace(str.front())) str.remove_prefix(1);
    while (!str.empty() && std::isspace(str.back()))  str.remove_suffix(1);

    if (str == "true" || str == "1") return true;
    if (str == "false" || str == "0") return false;

    TL_THROW_EXCEPTION("Invalid value for bool: {}", str);
}


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
template <Floating T>
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
template<Floating T>
constexpr auto roundToInteger(T n) 
{
    return static_cast<int>(round(n));
}

/*! \} */


} // End namespace tl

