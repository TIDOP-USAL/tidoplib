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

#include "tidop/config.h"

#include <vector>
#include <string>
#include <numeric>
#include <sstream>
#include <iomanip>

#include "tidop/core/base/defs.h"
#include "tidop/core/base/exception.h"
#include "tidop/core/base/common.h"
#include "tidop/core/base/string_utils.h"
#include "tidop/core/base/type.h"

namespace tl
{

/*!
 * \addtogroup Base
 *
 * \{
 */


/*!
 * \brief Performs a cast from one numeric type to another, rounding if necessary.
 * \tparam T1 The target numeric type.
 * \tparam T2 The source numeric type.
 * \param[in] number The number to cast.
 * \return The number cast to type T1
 */
template<typename T1, typename T2>
auto numberCast(T2 number) -> enableIfIntegral<T1, T1>
{
    return static_cast<T1>(std::round(number));
}

/// \cond

template<typename T1, typename T2>
auto numberCast(T2 number) -> enableIfFloating<T1, T1>
{
    return static_cast<T1>(number);
}

template<typename T1, typename T2>
auto numberCast(T2 /*b*/) -> enableIfNotArithmetic<T1, T1>
{
    //En linux me sale siempre el error aunque no se llame a la función.
    //TL_COMPILER_WARNING("Invalid conversion. It isn't an arithmetic type.")
    throw Exception("Invalid conversion. It isn't an arithmetic type.", __FILE__, __LINE__, TL_FUNCTION);
    return T1{0};
}

/// \endcond

/*!
 * \brief Converts a string to a numeric or boolean type, with range validation.
 *
 * This function converts an input string to the specified numeric type \c T.
 * It ensures that the value falls within the valid range for \c T.
 * Specialized behavior is implemented for `int8_t`, `uint8_t`, and `bool`.
 *
 * \tparam T The target type to convert the string into. Supported types include numeric types and `bool`.
 * \param[in] str The input string representing the value.
 * \return The converted value as the specified type \c T.
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

template <typename T>
auto toStringWithPrecision(T value, int precision) -> std::string
{
    std::ostringstream out;
    out << std::fixed << std::setprecision(precision) << value;
    return out.str();
}



/*!
 * \enum Base
 * \brief Represents numeric bases for string-to-integer conversion.
 */
enum class Base : int8_t
{
    octal = 8,        /*!< Octal base. */
    decimal = 10,     /*!< Decimal base. */
    hexadecimal = 16  /*!< Hexadecimal base. */
};


/*!
 * \brief Converts a string to an integer, considering its base.
 * \param[in] text The input string representing a number.
 * \param[in] base The numeric base (e.g., octal, decimal, hexadecimal).
 * \return The integer value represented by the string.
 * \see Base
 */
TL_EXPORT int stringToInteger(const std::string &text, Base base = Base::decimal);

/*!
 * \brief Rounds a floating-point number and converts it to an integer.
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

