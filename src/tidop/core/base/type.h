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
#include "tidop/core/base/defs.h"
#include "tidop/core/base/path.h"

#include <type_traits>
#include <stdexcept>
#include <map>

namespace tl
{


/*! \addtogroup Base
 *  \{
 */

enum class Type
{
    type_unknown,                /*!< Represents an unknown or unspecified type. */
    type_bool,                   /*!< Represents a boolean value (`true` or `false`). */
    type_int8,                   /*!< Represents an 8-bit signed integer. */
    type_uint8,                  /*!< Represents an 8-bit unsigned integer. */
    type_int16,                  /*!< Represents a 16-bit signed integer. */
    type_uint16,                 /*!< Represents a 16-bit unsigned integer. */
    type_int32,                  /*!< Represents a 32-bit signed integer. */
    type_uint32,                 /*!< Represents a 32-bit unsigned integer. */
    type_int64,                  /*!< Represents a 64-bit signed integer. */
    type_uint64,                 /*!< Represents a 64-bit unsigned integer. */
    type_float32,                /*!< Represents a 32-bit floating-point number. */
    type_float64,                /*!< Represents a 64-bit floating-point number (double precision). */
    type_string,                 /*!< Represents a string. */
    type_wstring,                /*!< Represents a wide string. */
    type_path,                   /*!< Represents a file path or directory path. */
    type_map,                    /*!< Represents a map (associative container). */
    type_schar = type_int8,      /*!< Alias for `type_int8` (character as an 8-bit signed integer). */
    type_uchar = type_uint8,     /*!< Alias for `type_uint8` (character as an 8-bit unsigned integer). */
    type_short = type_int16,     /*!< Alias for `type_int16` (short integer). */
    type_ushort = type_uint16,   /*!< Alias for `type_uint16` (unsigned short integer). */
    type_int = type_int32,       /*!< Alias for `type_int32` (standard signed integer). */
    type_uint = type_uint32,     /*!< Alias for `type_uint32` (unsigned integer). */
    type_float = type_float32,   /*!< Alias for `type_float32` (standard single-precision floating-point number). */
    type_double = type_float64   /*!< Alias for `type_float64` (double-precision floating-point number). */
};



template<typename T>
struct TypeTraits
{
    using value_type = T;
    static constexpr auto id_type = Type::type_unknown;
    static constexpr auto name_type = "unknown";
};

template<>
struct TypeTraits<bool>
{
    using value_type = bool;
    static constexpr auto id_type = Type::type_bool;
    static constexpr auto name_type = "bool";
};

template<>
struct TypeTraits<float>
{
    using value_type = float;
    static constexpr auto id_type = Type::type_float;
    static constexpr auto name_type = "float";
};

template<>
struct TypeTraits<double>
{
    using value_type = double;
    static constexpr auto id_type = Type::type_double;
    static constexpr auto name_type = "double";
};

template<>
struct TypeTraits<signed char>
{
    using value_type = signed char;
    static constexpr auto id_type = Type::type_int8;
    static constexpr auto name_type = "int8";
};

template<>
struct TypeTraits<unsigned char>
{
    using value_type = unsigned char;
    static constexpr auto id_type = Type::type_uint8;
    static constexpr auto name_type = "uint8";
};

template<>
struct TypeTraits<short>
{
    using value_type = short;
    static constexpr auto id_type = Type::type_int16;
    static constexpr auto name_type = "int16";
};

template<>
struct TypeTraits<unsigned short>
{
    using value_type = unsigned short;
    static constexpr auto id_type = Type::type_uint16;
    static constexpr auto name_type = "uint16";
};

template<>
struct TypeTraits<int>
{
    using value_type = int;
    static constexpr auto id_type = Type::type_int;
    static constexpr auto name_type = "int";
};

template<>
struct TypeTraits<unsigned int>
{
    using value_type = unsigned int;
    static constexpr auto id_type = Type::type_uint;
    static constexpr auto name_type = "uint";
};

template<>
struct TypeTraits<long long>
{
    using value_type = long long;
    static constexpr auto id_type = Type::type_int64;
    static constexpr auto name_type = "int64";
};

template<>
struct TypeTraits<unsigned long long>
{
    using value_type = unsigned long;
    static constexpr auto id_type = Type::type_uint64;
    static constexpr auto name_type = "uint64";
};

template<>
struct TypeTraits<std::string>
{
    using value_type = std::string;
    static constexpr auto id_type = Type::type_string;
    static constexpr auto name_type = "std::string";
};

template<>
struct TypeTraits<std::wstring>
{
    using value_type = std::wstring;
    static constexpr auto id_type = Type::type_wstring;
    static constexpr auto name_type = "std::wstring";
};

template<>
struct TypeTraits<tl::Path>
{
    using value_type = tl::Path;
    static constexpr auto id_type = Type::type_path;
    static constexpr auto name_type = "tl::Path";
};

template <typename Key, typename Value>
struct TypeTraits<std::map<Key, Value>>
{
    using value_type = std::map<Key, Value>;
    static constexpr auto id_type = Type::type_map;
    static constexpr auto name_type = "std::map";
};

/*! \} */


} // End namespace tl
