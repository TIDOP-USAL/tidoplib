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
 * \file string_utils.h
 * \brief String manipulation utilities
 *
 * This module provides utility functions for common string operations including
 * case-insensitive comparison, whitespace trimming and substring replacement.
 *
 * ### Functions
 *
 * - \ref compareInsensitiveCase - Case-insensitive string comparison
 * - \ref ltrim, \ref rtrim, \ref trim - Whitespace trimming functions
 * - \ref ltrim_copy, \ref rtrim_copy, \ref trim_copy - Non-mutating trim variants
 * - \ref replaceString - Substring replacement
 *
 */
 
#pragma once

#include "tidop/config.h"

#include <vector>
#include <string>
#include <numeric>
#include <sstream>

#include "tidop/core/base/defs.h"

namespace tl
{


/*!
 * \addtogroup StringUtils
 *
 * \{
 */

/*!
 * \brief Compares two strings case-insensitively.
 *
 * Performs lexicographic comparison of two strings, ignoring case differences.
 * This function has multiple implementations depending on available features:
 * - If Boost is available, uses boost::iequals for optimal performance
 * - Otherwise uses std::equal with a case-insensitive comparator (C++14+)
 * - Fallback implementation for older standards
 *
 * ### Example
 *
 * \code{.cpp}
 * bool result = tl::compareInsensitiveCase("Hello", "HELLO");  // true
 * bool result2 = tl::compareInsensitiveCase("abc", "def");     // false
 * \endcode
 *
 * \param[in] source The first string to compare
 * \param[in] compare The second string to compare
 * \return True if the strings are equal ignoring case; false otherwise
 *
 * \note In C++17+, parameters are std::string_view for efficiency
 */
#if TL_CPP_VERSION>= 17
TL_EXPORT bool compareInsensitiveCase(std::string_view source,
                                      std::string_view compare);
#else
TL_EXPORT bool compareInsensitiveCase(const std::string &source,
                                      const std::string &compare);
#endif

// https://stackoverflow.com/a/217605


/*!
 * \brief Trims leading whitespace characters from a string (in-place).
 *
 * Removes all leading whitespace characters (spaces, tabs, newlines, etc.)
 * from the beginning of the string. The operation modifies the string in-place.
 *
 * ### Example
 *
 * \code{.cpp}
 * std::string text = "  hello world";
 * tl::ltrim(text);  // Result: "hello world"
 * \endcode
 *
 * \param[in,out] s The string to trim
 *
 * \see ltrim_copy, rtrim, trim
 */
inline void ltrim(std::string &s)
{
    s.erase(s.begin(), std::find_if(s.begin(), s.end(), [](unsigned char ch) {
        return !std::isspace(ch);
        }));
}

/*!
 * \brief Trims trailing whitespace characters from a string (in-place).
 *
 * Removes all trailing whitespace characters (spaces, tabs, newlines, etc.)
 * from the end of the string. The operation modifies the string in-place.
 *
 * ### Example
 *
 * \code{.cpp}
 * std::string text = "hello world  ";
 * tl::rtrim(text);  // Result: "hello world"
 * \endcode
 *
 * \param[in,out] s The string to trim
 *
 * \see rtrim_copy, ltrim, trim
 */
inline void rtrim(std::string &s)
{
    s.erase(std::find_if(s.rbegin(), s.rend(), [](unsigned char ch) {
        return !std::isspace(ch);
        }).base(), s.end());
}

/*!
 * \brief Trims both leading and trailing whitespace characters from a string (in-place).
 *
 * Removes all leading and trailing whitespace characters from both ends of the string.
 * The operation modifies the string in-place by calling rtrim() followed by ltrim().
 *
 * ### Example
 *
 * \code{.cpp}
 * std::string text = "  hello world  ";
 * tl::trim(text);  // Result: "hello world"
 * \endcode
 *
 * \param[in,out] s The string to trim
 *
 * \see trim_copy, ltrim, rtrim
 */
inline void trim(std::string &s)
{
    rtrim(s);
    ltrim(s);
}

/*!
 * \brief Trims leading whitespace characters from a string (returns a new string).
 *
 * Returns a new string with leading whitespace removed. The original string
 * is not modified. This function is useful when you need to preserve the
 * original value.
 *
 * ### Example
 *
 * \code{.cpp}
 * std::string original = "  hello world";
 * std::string result = tl::ltrim_copy(original);
 * // original unchanged: "  hello world"
 * // result: "hello world"
 * \endcode
 *
 * \param[in] s The string to trim
 * \return A new string with leading whitespace removed
 *
 * \see ltrim, rtrim_copy, trim_copy
 */
inline auto ltrim_copy(std::string s) -> std::string
{
    ltrim(s);
    return s;
}

/*!
 * \brief Trims trailing whitespace characters from a string (returns a new string).
 *
 * Returns a new string with trailing whitespace removed. The original string
 * is not modified. This function is useful when you need to preserve the
 * original value.
 *
 * ### Example
 *
 * \code{.cpp}
 * std::string original = "hello world  ";
 * std::string result = tl::rtrim_copy(original);
 * // original unchanged: "hello world  "
 * // result: "hello world"
 * \endcode
 *
 * \param[in] s The string to trim
 * \return A new string with trailing whitespace removed
 *
 * \see rtrim, ltrim_copy, trim_copy
 */
inline auto rtrim_copy(std::string s) -> std::string
{
    rtrim(s);
    return s;
}

/*!
 * \brief Trims both leading and trailing whitespace characters from a string (returns a new string).
 *
 * Returns a new string with both leading and trailing whitespace removed.
 * The original string is not modified. This function is useful when you need
 * to preserve the original value.
 *
 * ### Example
 *
 * \code{.cpp}
 * std::string original = "  hello world  ";
 * std::string result = tl::trim_copy(original);
 * // original unchanged: "  hello world  "
 * // result: "hello world"
 * \endcode
 *
 * \param[in] s The string to trim
 * \return A new string with both leading and trailing whitespace removed
 *
 * \see trim, ltrim_copy, rtrim_copy
 */
inline auto trim_copy(std::string s) -> std::string
{
    trim(s);
    return s;
}

/*!
 * \brief Replaces all occurrences of a substring with another string.
 *
 * Finds all occurrences of the substring `str_old` in the string pointed to by `str`
 * and replaces them with `str_new`. The replacement is performed in-place and modifies
 * the original string. The search continues from where the previous replacement ended.
 *
 * ### Example
 *
 * \code{.cpp}
 * std::string str = "Hello world, Hello universe";
 * tl::replaceString(&str, "Hello", "Hi");
 * // Result: "Hi world, Hi universe"
 * \endcode
 *
 *
 * \param[in,out] str Pointer to the string to modify
 * \param[in] str_old The substring to be replaced
 * \param[in] str_new The replacement substring
 *
 * \see trim, ltrim, rtrim
 */
TL_EXPORT void replaceString(std::string *str, const std::string &str_old, const std::string &str_new);


/*! \} */


} // End namespace tl

