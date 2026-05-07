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
 * \file format.h
 * \brief String formatting utilities with support for fmt and std::format
 *
 * This module provides a unified interface for string formatting that transparently
 * uses either the {fmt} library (if available) or the standard library's std::format.
 * This abstraction allows consistent formatting across different C++ standards.
 *
 * ### Features
 *
 * - Cross-version compatibility (supports both fmt and std::format)
 * - Type-safe formatting with compile-time format string validation
 * - Variadic template support for any number of arguments
 * - Automatic selection of best available formatting library
 *
 * ### Functions
 *
 * - \ref format - Format a string with arguments
 *
 * ### Example Usage
 *
 * \code{.cpp}
 * #include "tidop/core/base/format.h"
 *
 * int main() {
 *     std::string name = "World";
 *     int count = 42;
 *     
 *     // Simple formatting
 *     std::string msg = tl::format("Hello, {}!", name);
 *     
 *     // Multiple arguments
 *     std::string result = tl::format("Count: {}, Name: {}", count, name);
 *     
 *     // With format specifiers
 *     double pi = 3.14159;
 *     std::string precise = tl::format("Pi = {:.2f}", pi);
 *     
 *     return 0;
 * }
 * \endcode
 *
 * ### Backend Selection
 *
 * If `TL_HAVE_FMT` is defined (fmt library available), uses `fmt::format`.
 * Otherwise falls back to `std::format` (C++20 and later).
 *
 * \see tl::format
 */

#pragma once

#include "tidop/config.h"

#ifdef TL_HAVE_FMT
#include <fmt/format.h>
#else
#include <format>
#endif

namespace tl
{


/*! \addtogroup Base
 *  \{
 */

/*!
 * \brief Format a message string with arguments using printf-style or format-style syntax.
 *
 * Provides a type-safe string formatting function that supports both positional
 * and named arguments. The format string is validated at compile-time when using
 * fmt or C++20 std::format.
 *
 * ### Format String Syntax
 *
 * - `{}` - Default formatting
 * - `{0}` - Positional argument (first argument)
 * - `{1}` - Positional argument (second argument)
 * - `{:.2f}` - Format specifiers (e.g., precision for floats)
 * - `{:>10}` - Alignment and width
 *
 * ### Supported Types
 *
 * - Integral types: int, unsigned int, int64_t, uint64_t, etc.
 * - Floating-point: float, double
 * - Character/String: char, char*, const char*, std::string, std::string_view
 * - Standard containers: vector, map, array, etc. (via fmt/std::format extensions)
 * - Custom types: if they have an operator<< overload or format specialization
 *
 * \tparam Args The types of the arguments to format
 * \param[in] s Format string with placeholders for arguments
 * \param[in] args Arguments to substitute into the format string
 * \return Formatted string as std::string
 *
 * ### Example Usage
 *
 * \code{.cpp}
 * // Simple formatting
 * std::string msg = tl::format("Hello, {}!", "World");
 * // Result: "Hello, World!"
 *
 * // Multiple arguments
 * int x = 10, y = 20;
 * std::string coords = tl::format("Position: ({}, {})", x, y);
 * // Result: "Position: (10, 20)"
 *
 * // With format specifiers
 * double pi = 3.14159265;
 * std::string precise = tl::format("Pi ≈ {:.2f}", pi);
 * // Result: "Pi ≈ 3.14"
 *
 * // Alignment and width
 * std::string padded = tl::format("{:>10}", 42);
 * // Result: "        42"
 *
 * \endcode
 *
 * ### Exception Safety
 *
 * May throw std::format_error if the format string is invalid
 * (e.g., mismatched braces, invalid format specifiers).
 *
 * ### Notes
 *
 * - Format string validation: compile-time for fmt and C++20, runtime for others
 * - Uses perfect forwarding for optimal performance
 * - Compatible with both fmt namespace and std namespace
 *
 * \see FORMAT_NAMESPACE
 */
template<typename... Args>
auto format(FORMAT_NAMESPACE format_string<Args...> s, Args&&... args) -> std::string
{
    return FORMAT_NAMESPACE vformat(s.get(), FORMAT_NAMESPACE make_format_args(args...));
}


/*! \} */


} // End namespace tl
