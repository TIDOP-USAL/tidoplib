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
#include <string>

#include "tidop/core/base/defs.h"
#include "tidop/core/base/exception.h"
#include "tidop/core/base/common.h"
#include "tidop/core/base/string_utils.h"


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
 * \brief Converts a string to a numeric type or boolean.
 * \tparam T The target type.
 * \param[in] str The input string.
 * \return The numeric value represented by the string.
 */
template <typename T>
auto convertStringTo(const std::string &str) -> std::enable_if_t<std::is_arithmetic<T>::value && !std::is_same<T, bool>::value,T>
{
    T value{};

    std::istringstream ss(str);
    ss >> value;

    return value;
} 

/// \cond

template <typename T>
auto convertStringTo(const std::string &str) -> enableIfBool<T,T>
{
    T value = (compareInsensitiveCase(str, "true") || str == "1");

    return value;
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

