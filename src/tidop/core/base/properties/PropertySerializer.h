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

/*! \file PropertySerializer.h
 * \brief Serialization traits for property values.
 *
 * This file defines the `PropertySerializer` struct template, which provides
 * string serialization and deserialization for property values of type `T`.
 * Specializations are provided for common types (`std::string`, `bool`,
 * arithmetic types, and `tl::Size<T>`), with extensibility for user-defined types.
 *
 * \ingroup Core
 * \see tl::Property, tl::TypeTraits, tl::convertStringTo
 */

#pragma once

#include "tidop/config.h"

#include <string>
#include <concepts>
#include <type_traits>

#include "tidop/core/base/Type.h"
#include "tidop/core/base/TypeConversions.h"

namespace tl
{

/*! \addtogroup Base
 *  \{
 */

/*!
 * \struct PropertySerializer
 * \brief Serialization trait for property values.
 *
 * This struct provides static methods to convert a value of type `T` to and
 * from a string representation. The default implementation handles:
 * - `std::string`: direct copy.
 * - `bool`: `"true"`/`"false"` or `"1"`/`"0"`.
 * - Arithmetic types: uses `std::to_string` and `convertStringTo<T>`.
 * - Types with `toString()` and `fromString()` methods: delegates to those.
 *
 * \tparam T The type to serialize.
 * \tparam Enable SFINAE parameter (default: `void`).
 *
 * ### Example
 * \code
 * // Using with int
 * int value = 42;
 * std::string str = PropertySerializer<int>::toString(value);      // "42"
 * PropertySerializer<int>::fromString("100", value);               // value = 100
 * \endcode
 *
 * \see Property, TypeTraits, convertStringTo
 */
template <typename T, typename Enable = void>
struct PropertySerializer
{
    /*!
     * \brief Converts a value to a string.
     * \param[in] val The value to convert.
     * \return String representation of the value.
     */
    static auto toString(const T &val) -> std::string
    {
        if constexpr (std::is_same_v<T, std::string>) {
            return val;
        //} else if constexpr (std::is_same_v<T, bool>) {
        //    return val ? "true" : "false";
        } else if constexpr (std::is_arithmetic_v<T>) {
            return std::to_string(val);
        } else {
            return val.toString();
        }
    }

    /*!
     * \brief Parses a string and sets the output value.
     * \param[in] str The string to parse.
     * \param[out] output The parsed value.
     * \throws `std::invalid_argument` if the string cannot be parsed.
     * \throws `tl::Exception` if conversion fails (for arithmetic types).
     */
    static void fromString(const std::string &str, T &output)
    {
        if constexpr (std::is_same_v<T, std::string>) {
            output = str;
        //} else if constexpr (std::is_same_v<T, bool>) {
        //    //TODO: esto se puede hacer con convertStringTo
        //    if (str == "true" || str == "1") output = true;
        //    else if (str == "false" || str == "0") output = false;
        //    else throw std::invalid_argument("Invalid bool");
        } else if constexpr (std::is_arithmetic_v<T>) {
            output = convertStringTo<T>(str); 
        } else {
            output.fromString(str);
        }
    }

    /*!
     * \brief Returns the type name as a string.
     * \return The type name (e.g., `"int"`, `"double"`).
     */
    static auto typeName() -> std::string
    {
        return TypeTraits<T>::name_type;
    }
};

/*!
 * \brief Specialization of `PropertySerializer` for `tl::Size<T>`.
 *
 * Serializes a `Size<T>` as `"widthxheight"` (e.g., `"1920x1080"`).
 */
template <typename T>
struct PropertySerializer<Size<T>>
{
    /*!
     * \brief Converts a `Size<T>` to a string.
     * \param[in] val The size value.
     * \return String in the format `"widthxheight"`.
     */
    static auto toString(const Size<T> &val) -> std::string
    {
        return std::to_string(val.width()) + "x" + std::to_string(val.height());
    }

    /*!
     * \brief Parses a string in the format `"widthxheight"` and sets the output.
     * \param[in] str The string to parse.
     * \param[out] output The parsed size.
     * \throws `std::invalid_argument` if the format is invalid.
     * \throws `tl::Exception` if numeric conversion fails.
     */
    static void fromString(const std::string &str, Size<T> &output)
    {
        size_t pos = str.find('x');
        if (pos == std::string::npos) throw std::invalid_argument("Invalid Size format");

        T w = convertStringTo<T>(str.substr(0, pos));
        T h = convertStringTo<T>(str.substr(pos + 1));
        output = Size<T>(w, h);
    }

    /*!
     * \brief Returns the type name.
     * \return `"tl::Size"`.
     */
    static auto typeName() -> std::string 
    { 
        return "tl::Size"; 
    }
};

/*! \} */

} // namespace tl
