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

#include <string>

#include "tidop/core/base/defs.h"
#include "tidop/core/base/exception.h"


namespace tl
{

/*!
 * \addtogroup Base
 *
 * \{
 */

/*!
 * \brief Converts a string from the local encoding (e.g., Windows ANSI code page) to a wide string (UTF-16/UTF-32).
 * 
 * \param local The input string encoded using the local 8-bit encoding.
 * \return std::wstring The resulting wide string.
 */
auto fromLocalEncoding(const std::string& local) -> std::wstring;

/*!
 * \brief Converts a wide string (UTF-16/UTF-32) to a UTF-8 encoded string.
 * 
 * \param wstr The wide string input.
 * \return std::string The UTF-8 encoded result.
 */
auto toUtf8(const std::wstring& wstr) -> std::string;

/*!
 * \brief Converts a UTF-8 encoded string to a wide string (UTF-16/UTF-32).
 * 
 * \param utf8str The UTF-8 encoded input string.
 * \return std::wstring The resulting wide string.
 */
auto fromUtf8(const std::string& utf8str) -> std::wstring;

/*!
 * \brief Converts a wide string (UTF-16/UTF-32) to the local 8-bit encoding.
 * 
 * \param wstr The wide string input.
 * \return std::string The resulting string in local encoding.
 */
auto toLocal8Bit(const std::wstring& wstr) -> std::string;

/*!
 * \brief Converts a string from the local 8-bit encoding to a wide string (UTF-16/UTF-32).
 * 
 * \param str The input string in local encoding.
 * \return std::wstring The resulting wide string.
 */
auto fromLocal8Bit(const std::string& str) -> std::wstring;

/*! \} */


} // End namespace tl

