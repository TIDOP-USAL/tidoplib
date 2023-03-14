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

#ifndef TL_CORE_ENDIANNESS_H
#define TL_CORE_ENDIANNESS_H

#include "config_tl.h"

#include <string>
#include <memory>
#include <mutex>

#include "tidop/core/defs.h"
#include "tidop/core/path.h"

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
  little_endian = __ORDER_BIG_ENDIAN__,
  native = __BYTE_ORDER__
#endif
};


template <typename T>
typename std::enable_if<std::is_arithmetic<T>::value, T>::type
swapEndian(T val)
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

//template <> int8_t swapEndian<int8_t>(int8_t val) { return val; }
//template <> uint8_t swapEndian<uint8_t>(uint8_t val) { return val; }

/*! \} */ // end of core

} // namespace tl

#endif // TL_CORE_ENDIANNESS_H
