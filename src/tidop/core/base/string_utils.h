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

namespace tl
{


/*!
 * \addtogroup StringUtils
 *
 * \{
 */

/*!
 * \brief Compares two strings case-insensitively.
 * \param[in] source The first string to compare.
 * \param[in] compare The second string to compare.
 * \return True if the strings are equal ignoring case, false otherwise.
 */
#if CPP_VERSION >= 17
TL_EXPORT bool compareInsensitiveCase(std::string_view source,
                                      std::string_view compare);
#else
TL_EXPORT bool compareInsensitiveCase(const std::string &source,
                                      const std::string &compare);
#endif

// https://stackoverflow.com/a/217605

/*!
 * \brief Trims the leading whitespace characters from a string (in place).
 * \param[in,out] s The string to trim.
 */
inline void ltrim(std::string &s)
{
    s.erase(s.begin(), std::find_if(s.begin(), s.end(), [](unsigned char ch) {
        return !std::isspace(ch);
        }));
}

/*!
 * \brief Trims the trailing whitespace characters from a string (in place).
 * \param[in,out] s The string to trim.
 */
inline void rtrim(std::string &s)
{
    s.erase(std::find_if(s.rbegin(), s.rend(), [](unsigned char ch) {
        return !std::isspace(ch);
        }).base(), s.end());
}

/*!
 * \brief Trims both leading and trailing whitespace characters from a string (in place).
 * \param[in,out] s The string to trim.
 */
inline void trim(std::string &s)
{
    rtrim(s);
    ltrim(s);
}

/*!
 * \brief Trims the leading whitespace characters from a string (returns a new string).
 * \param[in] s The string to trim.
 * \return A new string with leading whitespace removed.
 */
inline auto ltrim_copy(std::string s) -> std::string
{
    ltrim(s);
    return s;
}

/*!
 * \brief Trims the trailing whitespace characters from a string (returns a new string).
 * \param[in] s The string to trim.
 * \return A new string with trailing whitespace removed.
 */
inline auto rtrim_copy(std::string s) -> std::string
{
    rtrim(s);
    return s;
}

/*!
 * \brief Trims both leading and trailing whitespace characters from a string (returns a new string).
 * \param[in] s The string to trim.
 * \return A new string with both leading and trailing whitespace removed.
 */
inline auto trim_copy(std::string s) -> std::string
{
    trim(s);
    return s;
}

/*!
 * \brief Replaces a substring with another in a string.
 * \param[in,out] str The original string.
 * \param[in] str_old The substring to be replaced.
 * \param[in] str_new The new substring to replace the old one.
 * \return None. The original string is modified.
 *
 * ### Example Usage
 * \code{.cpp}
 * std::string str = "Hello world";
 * replaceString(str, " ", "_");
 * // str will be "Hello_world"
 * \endcode
 */
TL_EXPORT void replaceString(std::string *str, const std::string &str_old, const std::string &str_new);

/// \cond
#ifdef TL_OS_WINDOWS
/// \endcond

/*!
 * \brief Converts a string to a wide string.
 * \param[in] string The original string.
 * \return The converted wide string.
 */
auto stringToWString(const std::string &string) -> std::wstring;

/*!
 * \brief Converts a wide string to a regular string.
 * \param[in] wideString The original wide string.
 * \return The converted regular string.
 */
auto wstringToString(const std::wstring &wideString) -> std::string;

/// \cond
#endif // TL_OS_WINDOWS
/// \endcond

/*! \} */


} // End namespace tl

