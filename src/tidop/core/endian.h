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

#include "tidop/core/defs.h"
#include "tidop/core/path.h"
#include "tidop/core/common.h"

namespace tl
{

/*! \addtogroup core
 *  \{
 */

enum class endianness
{
#ifdef _WIN32
    little_endian,
    big_endian,
    native = little_endian
#else
    little_endian = __ORDER_LITTLE_ENDIAN__,
    big_endian = __ORDER_BIG_ENDIAN__,
    native = __BYTE_ORDER__
#endif
};


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


template<typename T>
void read(std::fstream *stream, T &value, bool littleEndian = true)
{
    stream->read(reinterpret_cast<char *>(&value), sizeof(T));
    if ((littleEndian && endianness::native == endianness::big_endian) ||
        (!littleEndian && endianness::native == endianness::little_endian))
        value = swapEndian(value);
}

template<typename T>
void read(std::ifstream *stream, T &value, bool littleEndian = true)
{
    stream->read(reinterpret_cast<char *>(&value), sizeof(T));
    if ((littleEndian && endianness::native == endianness::big_endian) ||
        (!littleEndian && endianness::native == endianness::little_endian))
        value = swapEndian(value);
}

template<typename T>
void write(std::fstream *stream, const T &value, bool littleEndian = true)
{
    T _value = value;
    if ((littleEndian && endianness::native == endianness::big_endian) ||
        (!littleEndian && endianness::native == endianness::little_endian))
        _value = swapEndian(_value);
    stream->write(reinterpret_cast<char *>(&_value), sizeof(T));
}

template<typename T>
void write(std::ofstream *stream, const T &value, bool littleEndian = true)
{
    T _value = value;
    if ((littleEndian && endianness::native == endianness::big_endian) ||
        (!littleEndian && endianness::native == endianness::little_endian))
        _value = swapEndian(_value);
    stream->write(reinterpret_cast<char *>(&_value), sizeof(T));
}

/*! \} */ // end of core

} // namespace tl

