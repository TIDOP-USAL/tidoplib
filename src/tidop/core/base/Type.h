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

/*! \file Type.h
 *  \brief Type information, type traits, and SFINAE utilities
 *
 *  This file provides:
 *  - Runtime type identification (enum Type, TypeTraits)
 *  - Compile-time type checking utilities (enableIf*, iteratorValueType)
 *  - Helper functions for type manipulation
 *
 *  ### Single Include Strategy
 *
 *  All type-related utilities are consolidated here for convenience.
 *  Include this file once to access both SFINAE helpers and runtime type info:
 *
 *  \code{.cpp}
 *  #include "tidop/core/base/type.h"
 *
 *  // Use SFINAE helpers
 *  template<typename T>
 *  void process(T value, enableIfIntegral<T>* = nullptr) { }
 *
 *  // Use runtime type traits
 *  Type runtime_type = getTypeId<int>();
 *  \endcode
 */
 
#pragma once

#include "tidop/config.h"
#include "tidop/core/base/Path.h"

#include <type_traits>
#include <iterator>
#include <stdexcept>
#include <map>

namespace tl
{

template<typename T>
class Size;

/*!
 * \addtogroup type_traits
 * \{
 */

// ============================================================================
// SFINAE Type Traits - Compile-time Type Checking
// ============================================================================

/*!
 * \brief Enables the template only if the type T is an integral type (e.g., int, short, unsigned long).
 *
 * This alias facilitates Substitution Failure Is Not An Error (SFINAE) by yielding the type R
 * if T satisfies std::is_integral. Otherwise, it causes a substitution failure, effectively
 * disabling the template instantiation for non-integral types.
 *
 * \tparam T The type being checked for integrality.
 * \tparam R The result type if the condition is true (defaults to void).
 *
 * ### Example
 * \code{.cpp}
 * template<typename T>
 * void process(T value, enableIfIntegral<T>* = nullptr) {
 *     // Only enabled for integral types: int, short, long, etc.
 * }
 * \endcode
 */
template<typename T, class R = void>
using enableIfIntegral = std::enable_if_t<std::is_integral<T>::value, R>;

/*!
 * \brief Enables the template only if the type T is a floating-point type (e.g., float, double).
 *
 * This alias is used for SFINAE, yielding the type R if T satisfies std::is_floating_point.
 * Otherwise, it causes a substitution failure, preventing instantiation for non-floating types.
 *
 * \tparam T The type being checked for floating-point status.
 * \tparam R The result type if the condition is true (defaults to void).
 */
template<typename T, class R = void>
using enableIfFloating = std::enable_if_t<std::is_floating_point<T>::value, R>;

/*!
 * \brief Enables the template only if the type T is an arithmetic type (integral or floating-point).
 *
 * Used for SFINAE to ensure T is either integral or floating-point (satisfies std::is_arithmetic).
 * Yields R on success; otherwise, it causes a substitution failure.
 *
 * \tparam T The type being checked for arithmetic status.
 * \tparam R The result type if the condition is true (defaults to void).
 */
template<typename T, class R = void>
using enableIfArithmetic = std::enable_if_t<std::is_arithmetic<T>::value, R>;

/*!
 * \brief Enables the template only if the type T is an unsigned integral type (e.g., unsigned int, unsigned short).
 *
 * This alias facilitates Substitution Failure Is Not An Error (SFINAE) by yielding the type R
 * if T satisfies std::is_unsigned. Otherwise, it causes a substitution failure, effectively
 * disabling the template instantiation for signed, floating-point, or non-arithmetic types.
 *
 * \tparam T The type being checked for unsigned status.
 * \tparam R The result type if the condition is true (defaults to void).
 */
template<typename T, class R = void>
using enableIfUnsigned = std::enable_if_t<std::is_unsigned<T>::value, R>;

/*!
 * \brief Enables the template only if the type T is exactly 'float'.
 *
 * This is a highly specific SFINAE constraint, checking for exact type equality with float
 * after removing const/volatile qualifiers.
 *
 * \tparam T The type being checked for equality with float.
 * \tparam R The result type if the condition is true (defaults to void).
 */
template<typename T, class R = void>
using enableIfFloat = std::enable_if_t<std::is_same<float, std::remove_cv_t<T>>::value, R>;

/*!
 * \brief Enables the template only if the type T is exactly 'double'.
 *
 * A specific SFINAE constraint, checking for exact type equality with double
 * after removing const/volatile qualifiers.
 *
 * \tparam T The type being checked for equality with double.
 * \tparam R The result type if the condition is true (defaults to void).
 */
template<typename T, class R = void>
using enableIfDouble = std::enable_if_t<std::is_same<double, std::remove_cv_t<T>>::value, R>;

/*!
 * \brief Enables the template only if the type T is exactly 'bool'.
 *
 * A specific SFINAE constraint, checking for exact type equality with bool
 * after removing const/volatile qualifiers.
 *
 * \tparam T The type being checked for equality with bool.
 * \tparam R The result type if the condition is true (defaults to void).
 */
template<typename T, class R = void>
using enableIfBool = std::enable_if_t<std::is_same<bool, std::remove_cv_t<T>>::value, R>;

/*!
 * \brief Enables the template only if the type T is NOT an integral type.
 *
 * This is the inverse of enableIfIntegral, yielding R only if T fails std::is_integral.
 *
 * \tparam T The type being checked for non-integrality.
 * \tparam R The result type if the condition is true (defaults to void).
 */
template<typename T, class R = void>
using enableIfNotIntegral = std::enable_if_t<!std::is_integral<T>::value, R>;

/*!
 * \brief Enables the template only if the type T is NOT a floating-point type.
 *
 * This is the inverse of enableIfFloating, yielding R only if T fails std::is_floating_point.
 *
 * \tparam T The type being checked for non-floating-point status.
 * \tparam R The result type if the condition is true (defaults to void).
 */
template <typename T, class R = void>
using enableIfNotFloating = std::enable_if_t<!std::is_floating_point<T>::value, R>;

/*!
 * \brief Enables the template only if the type T is NOT an arithmetic type.
 *
 * This is the inverse of enableIfArithmetic, yielding R only if T fails std::is_arithmetic.
 *
 * \tparam T The type being checked for non-arithmetic status.
 * \tparam R The result type if the condition is true (defaults to void).
 */
template<typename T, class R = void>
using enableIfNotArithmetic = std::enable_if_t<!std::is_arithmetic<T>::value, R>;

/*!
 * \brief Enables the template only if the type T is NOT an unsigned integral type.
 *
 * This is the inverse of enableIfUnsigned, yielding R only if T fails std::is_unsigned
 * (i.e., T is a signed integral, floating-point, or non-arithmetic type).
 *
 * \tparam T The type being checked for non-unsigned status.
 * \tparam R The result type if the condition is true (defaults to void).
 */
template<typename T, class R = void>
using enableIfNotUnsigned = std::enable_if_t<!std::is_unsigned<T>::value, R>;

/*!
 * \brief Trait alias to extract the value type of an iterator.
 *
 * Simplifies accessing the typename std::iterator_traits<It>::value_type.
 *
 * ### Example
 * \code{.cpp}
 * template<typename It>
 * void process(It begin, It end) {
 *     using ValueType = iteratorValueType<It>;
 *     // ValueType is now the element type
 * }
 * \endcode
 *
 * \tparam It The iterator type.
 */
template<typename It>
using iteratorValueType = typename std::iterator_traits<It>::value_type;



// ============================================================================
// Runtime Type Enumeration
// ============================================================================

/*!
 * \brief Enumeration of supported runtime types.
 *
 * Provides a runtime representation of C++ types for dynamic type identification
 * and type-agnostic operations. Each enum value corresponds to a C++ fundamental
 * or commonly used type.
 *
 * ### Type Categories
 *
 * - **Boolean:** type_bool
 * - **Signed Integers:** type_int8, type_int16, type_int32, type_int64
 * - **Unsigned Integers:** type_uint8, type_uint16, type_uint32, type_uint64
 * - **Floating-Point:** type_float32, type_float64
 * - **String Types:** type_string, type_wstring
 * - **Special:** type_path, type_size
 *
 * ### Aliases
 *
 * Convenient aliases are provided for standard C++ types:
 * - `type_int` = `type_int32`
 * - `type_uint` = `type_uint32`
 * - `type_float` = `type_float32`
 * - `type_double` = `type_float64`
 *
 * ### Usage
 *
 * \code{.cpp}
 * Type t = getTypeId<int>();     // Returns Type::type_int32
 * if (t == Type::type_int32) { }
 * \endcode
 *
 * \see TypeTraits, getTypeId
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
    type_size,
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


// ============================================================================
// Runtime Type Traits - Type Information at Runtime
// ============================================================================

/*!
 * \brief Traits for type information at runtime.
 *
 * Specializations of this template provide compile-time mappings between C++ types
 * and their runtime Type enumeration, along with type names for debugging and
 * serialization.
 *
 * \tparam T The type to get traits for.
 *
 * ### Example
 * \code{.cpp}
 * static_assert(TypeTraits<int>::id_type == Type::type_int32);
 * std::cout << TypeTraits<int>::name_type;  // Prints "int"
 * \endcode
 */
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
    using value_type = unsigned long long;
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

template<typename T>
struct TypeTraits<Size<T>>
{
    using value_type = Size<T>;
    static constexpr auto id_type = Type::type_size;
    static constexpr auto name_type = "Size";
};

template <typename Key, typename Value>
struct TypeTraits<std::map<Key, Value>>
{
    using value_type = std::map<Key, Value>;
    static constexpr Type id_type = Type::type_map;
    static constexpr auto name_type = "std::map";
};

// ============================================================================
// Helper Functions
// ============================================================================

/*!
 * \brief Get the runtime Type identifier for a compile-time type.
 *
 * Returns the Type enumeration value corresponding to the template parameter T.
 * This is useful for runtime type dispatch and debugging.
 *
 * ### Example
 * \code{.cpp}
 * Type t = getTypeId<int>();           // Returns Type::type_int32
 * Type f = getTypeId<float>();         // Returns Type::type_float32
 * Type s = getTypeId<std::string>();   // Returns Type::type_string
 * \endcode
 *
 * \tparam T The type to get the identifier for.
 * \return The corresponding Type enumeration value.
 *
 * \see TypeTraits, Type
 */
template<typename T>
constexpr Type getTypeId()
{
    return TypeTraits<T>::id_type;
}

/*! \} */


} // End namespace tl
