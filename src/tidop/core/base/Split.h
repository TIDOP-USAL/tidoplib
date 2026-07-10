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
 * \file Split.h
 * \brief String splitting utilities with type conversion
 *
 * This module provides generic string splitting functionality with automatic
 * type conversion. Strings can be split into substrings (as std::string) or
 * directly into numeric types (int, float, double, etc.). The template-based
 * implementation handles type selection and conversion automatically.
 *
 * ### Functions
 *
 * - \ref split - Generic string splitting function with type conversion
 *
 * ### Features
 *
 * - Split strings into std::string or numeric types
 * - Configurable delimiter character
 * - Automatic type conversion using type conversion utilities
 * - Specializations for string and arithmetic types
 * - Exception handling for conversion errors
 * - Efficient stringstream-based parsing
 *
 * ### Example Usage - String Splitting
 *
 * \code{.cpp}
 * #include "tidop/core/base/split.h"
 *
 * std::string csv = "apple,banana,cherry";
 * auto fruits = tl::split<std::string>(csv, ',');
 * // Result: {"apple", "banana", "cherry"}
 * \endcode
 *
 * ### Example Usage - Numeric Conversion
 *
 * \code{.cpp}
 * std::string numbers = "10.5,20.3,30.7,40.1";
 * auto values = tl::split<float>(numbers, ',');
 * // Result: {10.5f, 20.3f, 30.7f, 40.1f}
 *
 * std::string integers = "100,200,300,400";
 * auto ints = tl::split<int>(integers, ',');
 * // Result: {100, 200, 300, 400}
 * \endcode
 *
 * \see tl::split
 */
 
#pragma once

#include "tidop/config.h"

#include <vector>
#include <string>
#include <numeric>
#include <sstream>

#include "tidop/core/base/Concepts.h"
#include "tidop/core/base/TypeConversions.h"

namespace tl
{

/*!
 * \addtogroup StringUtils
 *
 * \{
 */


enum class EmptyTokenPolicy
{
    keep,
    skip,
    error
};


template <typename T, EmptyTokenPolicy Policy = EmptyTokenPolicy::keep>
    requires std::same_as<T, std::string> || 
             ArithmeticNoBool<T>
auto split(std::string_view str, char separator = ',') -> std::vector<T>
{
    std::vector<T> out;
    out.reserve(10);

    for (const auto &&subrange : str | std::ranges::views::split(separator)) {

        std::string_view item{subrange.data(), subrange.size()};

        if (item.empty()) {
            if constexpr (Policy == EmptyTokenPolicy::skip) {
                continue;
            } else if constexpr (Policy == EmptyTokenPolicy::error) {
                TL_THROW_EXCEPTION("Empty token detected in split with strict policy");
            }
        }

        if constexpr (std::same_as<T, std::string>) {
            out.emplace_back(item);
        } else {
            out.push_back(convertStringTo<T>(item));
        }
    }

    return out;
}

/*! \} */


} // End namespace tl

