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

#include <mutex>
#include <fstream>

#include "tidop/core/base/defs.h"
#include "tidop/core/base/path.h"
#include "tidop/core/base/common.h"

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
 * \return `endianness::little_endian` or `endianness::big_endian`.
 */
inline endianness getNativeEndianness()
{
    uint16_t test = 0x1;
    uint8_t *byte = reinterpret_cast<uint8_t *>(&test);
    return (*byte == 0x1) ? endianness::little_endian : endianness::big_endian;
}

/*!
 * \brief Swaps the endianness of an arithmetic value.
 * \tparam T The type of the value, must be arithmetic (e.g., int, float).
 * \param[in] val The value whose endianness is to be swapped.
 * \return The value with swapped byte order.
 *
 * This function converts a value from one byte order to its opposite.
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
 * \brief Reads a value of type T from a stream, adjusting for endianness.
 * \tparam T The type of the value to read.
 * \param[in] stream Pointer to the input file stream.
 * \param[out] value Reference to the value to store the result.
 * \param[in] littleEndian Indicates whether the value in the file is in little-endian format.
 *
 * Reads a value of type T from a file stream and optionally converts its byte order
 * to match the native system endianness.
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
 * \brief Writes a value of type T to a stream, adjusting for endianness.
 * \tparam T The type of the value to write.
 * \param[in] stream Pointer to the output file stream.
 * \param[in] value The value to write to the file.
 * \param[in] littleEndian Indicates whether the value should be written in little-endian format.
 *
 * Converts the byte order of a value to the specified endianness and writes it to the file stream.
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
 * \param[in] buffer Pointer to the memory buffer.
 * \param[out] value Reference to the value to store the result.
 * \param[in] littleEndian Indicates whether the buffer is in little-endian format.
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
 * \param[out] buffer Pointer to the memory buffer.
 * \param[in] value The value to write to the buffer.
 * \param[in] littleEndian Indicates whether the value should be written in little-endian format.
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

