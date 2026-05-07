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
 * \file text_encoding.h
 * \brief Character encoding conversion utilities
 *
 * This module provides utilities for converting strings between different character encodings:
 * UTF-8, UTF-16/UTF-32 (wide strings), and platform-specific local encodings.
 *
 * ### Functions
 *
 * **UTF-8 Conversions:**
 * - \ref fromUtf8 - Converts UTF-8 string to wide string
 * - \ref toUtf8 - Converts wide string to UTF-8
 *
 * **Local Encoding Conversions (Platform-Specific):**
 * - \ref fromLocalEncoding - Converts from local encoding to wide string
 * - \ref fromLocal8Bit - Converts from local 8-bit encoding to wide string
 * - \ref toLocal8Bit - Converts wide string to local 8-bit encoding
 *
 * ### Encoding Mappings
 *
 * **Windows (TL_OS_WINDOWS):**
 * - Local encoding: CP_ACP (ANSI Code Page)
 * - UTF-8: CP_UTF8
 * - Wide strings: UTF-16
 *
 * **Unix/Linux:**
 * - Local encoding and UTF-8 are treated as the same
 * - Wide strings: UTF-32 (wchar_t)
 *
 * ### Features
 *
 * - Platform-aware encoding conversions
 * - UTF-8 support across all platforms
 * - Proper handling of null terminators
 * - C++17 and C++20 optimizations
 * - Error handling with exception support
 *
 * \see fromUtf8, toUtf8, fromLocalEncoding, toLocal8Bit
 */
 
#pragma once

#include "tidop/config.h"

#include <string>

#include "tidop/core/base/exception.h"


namespace tl
{

/*!
 * \addtogroup Base
 *
 * \{
 */

/*!
 * \brief Converts a string from the local encoding to a wide string.
 *
 * Converts a string from the platform's local 8-bit encoding to a wide string (UTF-16 on Windows, UTF-32 on Unix).
 *
 *
 * ### Example
 *
 * \code{.cpp}
 * #ifdef TL_OS_WINDOWS
 * std::string ansi = "Hola";  // Windows ANSI code page
 * std::wstring wide = tl::fromLocalEncoding(ansi);
 * #endif
 * \endcode
 *
 * \param[in] local The input string in local encoding
 * \return Wide string representation
 * \see toLocal8Bit, fromUtf8
 */
auto fromLocalEncoding(const std::string& local) -> std::wstring;

/*!
 * \brief Converts a wide string to UTF-8 encoding.
 *
 * Converts a wide string (UTF-16 on Windows, UTF-32 on Unix) to a UTF-8 encoded string.
 * This is useful for cross-platform string handling and file I/O operations.
 *
 * ### Example
 *
 * \code{.cpp}
 * std::wstring wide = L"Hola";
 * std::string utf8 = tl::toUtf8(wide);
 * // Result: "Hola" (UTF-8 encoded)
 * \endcode
 *
 * \param[in] wstr The wide string input
 * \return UTF-8 encoded string
 *
 * \see fromUtf8, toLocal8Bit
 */
auto toUtf8(const std::wstring& wstr) -> std::string;

/*!
 * \brief Converts a UTF-8 string to a wide string.
 *
 * Converts a UTF-8 encoded string to a wide string (UTF-16 on Windows, UTF-32 on Unix).
 * This is useful for cross-platform string handling and Windows API calls.
 *
 * ### Example
 *
 * \code{.cpp}
 * std::string utf8 = "Hola";
 * std::wstring wide = tl::fromUtf8(utf8);
 * // Result: L"Hola" (UTF-16 on Windows)
 * \endcode
 *
 * \param[in] utf8str The UTF-8 encoded input string
 * \return Wide string representation
 *
 * \see toUtf8, fromLocalEncoding
 */
auto fromUtf8(const std::string& utf8str) -> std::wstring;

/*!
 * \brief Converts a wide string to local 8-bit encoding.
 *
 * Converts a wide string (UTF-16 on Windows, UTF-32 on Unix) to the platform's local 8-bit encoding.
 *
 * **Platform-Specific Behavior:**
 * - **Windows**: Uses CP_ACP (ANSI Code Page) for local encoding
 * - **Unix/Linux**: Treats local encoding as UTF-8
 *
 * ### Example
 *
 * \code{.cpp}
 * #ifdef TL_OS_WINDOWS
 * std::wstring wide = L"Hola";
 * std::string ansi = tl::toLocal8Bit(wide);
 * // Result: "Hola" (Windows ANSI code page)
 * #endif
 * \endcode
 *
 * \param[in] wstr The wide string input
 * \return String in local encoding
 *
 * \see fromLocal8Bit, toUtf8
 */
auto toLocal8Bit(const std::wstring& wstr) -> std::string;

/*!
 * \brief Converts a string from local 8-bit encoding to a wide string.
 *
 * Converts a string from the platform's local 8-bit encoding to a wide string.
 * Similar to fromLocalEncoding() but with explicit error handling.
 *
 * **Platform-Specific Behavior:**
 * - **Windows**: Uses CP_ACP (ANSI Code Page) for local encoding
 * - **Unix/Linux**: Treats local encoding as UTF-8
 *
 * ### Example
 *
 * \code{.cpp}
 * #ifdef TL_OS_WINDOWS
 * std::string ansi = "Hola";
 * std::wstring wide = tl::fromLocal8Bit(ansi);
 * #endif
 * \endcode
 *
 * \param[in] str The input string in local encoding
 * \return Wide string representation
 *
 * \see toLocal8Bit, fromUtf8
 */
auto fromLocal8Bit(const std::string& str) -> std::wstring;

/*! \} */


} // End namespace tl

