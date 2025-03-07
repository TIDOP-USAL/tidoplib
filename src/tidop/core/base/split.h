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

#include "tidop/core/base/type_conversions.h"

namespace tl
{

/*!
 * \addtogroup StringUtils
 *
 * \{
 */

/*!
 * \brief Splits a string into a vector of substrings based on a specified delimiter.
 *
 * This function can split a string into substrings, either returning a vector of `std::string`
 * or a vector of a numeric type (e.g., `int`, `float`, etc.), depending on the template type `T`.
 * It uses a specified separator character to split the string. The default separator is a comma (`,`) if not specified.
 *
 * \tparam T The type of elements in the resulting vector. It can either be `std::string` or any arithmetic type (excluding `bool`).
 *
 * \param[in] string The input string to split.
 * \param[in] separator The character used to split the string (default is `','`).
 *
 * \return A vector containing the substrings, either as `std::string` or as the specified numeric type `T`.
 *
 * ### Example Usage
 * \code{.cpp}
 * std::string str = "10,20,30,40";
 * auto result = split<int>(str, ',');
 * // result = {10, 20, 30, 40}
 * \endcode
 */
template <typename T>
auto split(const std::string& string,
           char separator = ',') -> std::enable_if_t<std::is_same<T, std::string>::value, std::vector<T>>
{
    std::vector<T> out;

    std::stringstream ss(string);
    std::string  item{};
    while (std::getline(ss, item, separator)) {
        out.push_back(item);
    }

    return out;
}

/// \cond

template <typename T>
auto split(const std::string& string,
           char separator = ',') -> std::enable_if_t<std::is_arithmetic<T>::value && 
                                                     !std::is_same<T, bool>::value,
	                                                 std::vector<T>>
{
    std::vector<T> out;

    std::stringstream ss(string);
    std::string item{};
    while (std::getline(ss, item, separator)) {
        out.push_back(convertStringTo<T>(item));
    }

    return out;
}

/// \endcond

/*! \} */


} // End namespace tl

