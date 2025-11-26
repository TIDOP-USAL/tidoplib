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
 * \file endian.h
 * \brief Byte order (endianness) detection and conversion utilities
 *
 * This module provides utilities for handling byte order conversions and endianness detection.
 * It supports reading and writing values with automatic byte-order conversion for both
 * file streams and memory buffers.
 *
 * ### Features
 *
 * - Detect native system byte order (little-endian or big-endian)
 * - Swap byte order of arithmetic values
 * - Read/write from file streams with automatic endianness conversion
 * - Read/write from memory buffers with automatic endianness conversion
 * - Cross-platform support (Windows/Linux)
 *
 * ### Example Usage
 *
 * \code{.cpp}
 * // Detect native endianness
 * auto native = getNativeEndianness();
 * 
 * // Read from file with conversion
 * int32_t value;
 * std::ifstream file("data.bin", std::ios::binary);
 * read(&file, value, true);  // File is in little-endian format
 * \endcode
 *
 * \see tl::endianness, tl::getNativeEndianness
 */

#pragma once

#include "tidop/config.h"

#include <array>
#include <cstring>
#include <mutex>
#include <fstream>

#include "tidop/core/base/path.h"
#include "tidop/core/base/type.h"

namespace tl
{

/*! \addtogroup Base
 *  \{
 */

/*!
 * \enum endianness
 * \brief Represents byte order (endianness) of the system.
 *
 * This enumeration provides constants to identify the byte order of the system.
 * - `little_endian`: Little-endian byte order.
 * - `big_endian`: Big-endian byte order.
 * - `native`: Native byte order of the current platform.
 */
enum class endianness
{
/// \cond
#ifdef _WIN32
/// \endcond
    little_endian, /*!< Little-endian byte order. */
    big_endian,    /*!< Big-endian byte order. */
    native = little_endian /*!< Native byte order (always little-endian on Windows). */
/// \cond
#else
    little_endian = __ORDER_LITTLE_ENDIAN__,
    big_endian = __ORDER_BIG_ENDIAN__,
    native = __BYTE_ORDER__
#endif
/// \endcond
};


/*!
 * \brief Detects the native byte order of the current system.
 * 
 * Determines whether the system uses little-endian or big-endian byte order
 * by examining how a test value is stored in memory.
 * 
 * \return `endianness::little_endian` if the system is little-endian,
 *         `endianness::big_endian` if the system is big-endian.
 *
 * ### Example Usage
 * \code{.cpp}
 * auto order = getNativeEndianness();
 * if (order == endianness::little_endian) {
 *     Message::info("System is little-endian");
 * }
 * \endcode
 */
inline endianness getNativeEndianness()
{
    uint16_t test = 0x1;
    uint8_t *byte = reinterpret_cast<uint8_t *>(&test);
    return (*byte == 0x1) ? endianness::little_endian : endianness::big_endian;
}

/*!
 * \brief Swaps the endianness of an arithmetic value.
 * 
 * Converts a value from one byte order to its opposite by reversing
 * the order of its bytes in memory.
 * 
 * \tparam T The type of the value, must be arithmetic (e.g., int, float, double).
 * \param[in] val The value whose endianness is to be swapped.
 * \return The value with swapped byte order.
 *
 * ### Example Usage
 * \code{.cpp}
 * uint32_t value = 0x12345678;
 * uint32_t swapped = swapEndian(value);  // Result: 0x78563412
 * \endcode
 *
 * \note This function only works with arithmetic types
 */
template <typename T>
auto swapEndian(T val) -> enableIfArithmetic<T, T>
{
    union U
    {
        T val;
        std::array<uint8_t, sizeof(T)> raw;
    } src, dst;

    src.val = val;
    std::reverse_copy(src.raw.begin(), src.raw.end(), dst.raw.begin());
    return dst.val;
}

/*!
 * \brief Reads a value of type T from a file stream, adjusting for endianness.
 * 
 * Reads a value of type T from a file stream and automatically converts its byte order
 * from the specified format to the native system endianness if necessary.
 * 
 * \tparam T The type of the value to read (must be arithmetic).
 * \param[in] stream Pointer to the input file stream.
 * \param[out] value Reference to the value to store the result.
 * \param[in] littleEndian If true, assumes the file value is in little-endian format.
 *                         If false, assumes big-endian format. Defaults to true.
 *
 * ### Example Usage
 * \code{.cpp}
 * std::ifstream file("data.bin", std::ios::binary);
 * int32_t value;
 * read(&file, value, true);  // File is little-endian
 * file.close();
 * \endcode
 *
 * ### Notes
 * - Stream position advances by sizeof(T) bytes
 * - Conversion is automatic based on native system endianness
 */
template<typename T>
void read(std::fstream *stream, T &value, bool littleEndian = true)
{
    stream->read(reinterpret_cast<char *>(&value), sizeof(T));
    if ((littleEndian && endianness::native == endianness::big_endian) ||
        (!littleEndian && endianness::native == endianness::little_endian))
        value = swapEndian(value);
}

/*!
 * \copydoc read(std::fstream *, T &, bool)
 */
template<typename T>
void read(std::ifstream *stream, T &value, bool littleEndian = true)
{
    stream->read(reinterpret_cast<char *>(&value), sizeof(T));
    if ((littleEndian && endianness::native == endianness::big_endian) ||
        (!littleEndian && endianness::native == endianness::little_endian))
        value = swapEndian(value);
}

/*!
 * \brief Writes a value of type T to a file stream, adjusting for endianness.
 * 
 * Converts a value to the specified byte order and writes it to the file stream.
 * 
 * \tparam T The type of the value to write (must be arithmetic).
 * \param[in] stream Pointer to the output file stream.
 * \param[in] value The value to write to the file.
 * \param[in] littleEndian If true, writes the value in little-endian format.
 *                         If false, writes in big-endian format. Defaults to true.
 *
 * ### Example Usage
 * \code{.cpp}
 * std::ofstream file("data.bin", std::ios::binary);
 * int32_t value = 12345;
 * write(&file, value, true);  // Write as little-endian
 * file.close();
 * \endcode
 *
 * ### Notes
 * - Conversion is automatic based on native system endianness
 * - Stream position advances by sizeof(T) bytes
 */
template<typename T>
void write(std::fstream *stream, const T &value, bool littleEndian = true)
{
    T _value = value;
    if ((littleEndian && endianness::native == endianness::big_endian) ||
        (!littleEndian && endianness::native == endianness::little_endian))
        _value = swapEndian(_value);
    stream->write(reinterpret_cast<char *>(&_value), sizeof(T));
}

/*!
 * \copydoc write(std::fstream *, const T &, bool)
 */
template<typename T>
void write(std::ofstream *stream, const T &value, bool littleEndian = true)
{
    T _value = value;
    if ((littleEndian && endianness::native == endianness::big_endian) ||
        (!littleEndian && endianness::native == endianness::little_endian))
        _value = swapEndian(_value);
    stream->write(reinterpret_cast<char *>(&_value), sizeof(T));
}

/*!
 * \brief Reads a value of type T from a memory buffer, adjusting for endianness.
 * 
 * Copies a value of type T from a memory buffer and automatically converts its byte order
 * from the specified format to the native system endianness if necessary.
 * 
 * \tparam T The type of the value to read (must be arithmetic).
 * \param[in] buffer Pointer to the memory buffer containing the data.
 * \param[out] value Reference to the value to store the result.
 * \param[in] littleEndian If true, assumes the buffer value is in little-endian format.
 *                         If false, assumes big-endian format. Defaults to true.
 *
 * ### Example Usage
 * \code{.cpp}
 * uint8_t buffer[4] = {0x12, 0x34, 0x56, 0x78};
 * int32_t value;
 * readFromBuffer(buffer, value, true);  // Buffer is little-endian
 * \endcode
 *
 * ### Notes
 * - Does not modify buffer pointer or stream position
 * - Safe for use with arbitrary memory locations
 */
template <typename T>
void readFromBuffer(const uint8_t *buffer, T &value, bool littleEndian = true)
{
    std::memcpy(&value, buffer, sizeof(T));
    if ((littleEndian && endianness::native == endianness::big_endian) ||
        (!littleEndian && endianness::native == endianness::little_endian))
        value = swapEndian(value);
}

/*!
 * \brief Writes a value of type T to a memory buffer, adjusting for endianness.
 * 
 * Converts a value to the specified byte order and copies it to a memory buffer.
 * 
 * \tparam T The type of the value to write (must be arithmetic).
 * \param[out] buffer Pointer to the memory buffer where data will be written.
 * \param[in] value The value to write to the buffer.
 * \param[in] littleEndian If true, writes the value in little-endian format.
 *                         If false, writes in big-endian format. Defaults to true.
 *
 * ### Example Usage
 * \code{.cpp}
 * uint8_t buffer[4];
 * int32_t value = 0x12345678;
 * writeToBuffer(buffer, value, true);  // Write as little-endian
 * // buffer now contains: {0x78, 0x56, 0x34, 0x12}
 * \endcode
 *
 * ### Notes
 * - Caller is responsible for ensuring buffer has at least sizeof(T) bytes
 * - Does not modify original value
 */
template <typename T>
void writeToBuffer(uint8_t *buffer, const T &value, bool littleEndian = true)
{
    T tempValue = value;
    if ((littleEndian && endianness::native == endianness::big_endian) ||
        (!littleEndian && endianness::native == endianness::little_endian))
        tempValue = swapEndian(tempValue);
    std::memcpy(buffer, &tempValue, sizeof(T));
}

/*! \} */

} // namespace tl

