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

/*! \file Property.h
 * \brief Typed property implementation with string conversion.
 *
 * This file defines the `Property` class template, a concrete implementation
 * of `PropertyBase` that stores a value of type `T` and provides serialization
 * to/from strings using `PropertySerializer<T>`.
 *
 * \ingroup Core
 * \see tl::PropertyBase, tl::PropertySerializer
 */

#pragma once


#include "tidop/config.h"

#include <string>

#include "tidop/core/base/Type.h"
#include "tidop/core/base/Exception.h"
#include "tidop/core/base/properties/PropertyBase.h"
#include "tidop/core/base/properties/PropertySerializer.h"

namespace tl
{

class PropertyBase;

/*! \addtogroup Base
 *  \{
 */

/*!
 * \class Property
 * \brief Typed property storing a value of type `T`.
 *
 * This class extends `PropertyBase` and implements functionality for converting
 * the property value to and from strings using the `PropertySerializer` trait.
 * It stores a value of type `T` and provides accessors and mutators, along with
 * cloning and type information.
 *
 * \tparam T The type of the property's value.
 *
 * ### Example
 * \code
 * Property<int> intProp(42);
 * std::string str = intProp.toString();           // "42"
 * intProp.fromString("100");
 * int value = intProp.value();                    // 100
 *
 * Property<double> doubleProp(3.14);
 * auto clone = doubleProp.clone();                // clone as PropertyBase
 * \endcode
 *
 * \see PropertyBase, PropertySerializer
 */
template <typename T>
class Property final
  : public PropertyBase 
{

private:

    T mValue;

public:

    /*!
     * \brief Default constructor.
     * Value-initializes the stored value (zero for arithmetic types).
     */
    Property() = default;

    /*!
     * \brief Constructs a property with an initial value.
     * \param[in] value The initial value.
     */
    explicit Property(const T &value) 
      : mValue(value) {}

    /*!
     * \brief Returns the stored value.
     * \return A copy of the property's value.
     */
    auto value() const noexcept -> T
    {
        return mValue;
    }

    /*!
     * \brief Sets the property's value.
     * \param[in] value The new value.
     */
    void setValue(const T &value)  
    {
        mValue = value;
    }

    /*!
     * \brief Converts the property's value to a string.
     * \return A string representation of the property's value.
     * \throws `tl::Exception` if the conversion fails.
     */
    auto toString() const -> std::string override 
    {
        try {
            return PropertySerializer<T>::toString(mValue);
        } catch (...) {
            TL_THROW_EXCEPTION_WITH_NESTED("Error converting property to string");
        }
    }

    /*!
     * \brief Sets the property's value from a string.
     * \param[in] value A string representation of the value to set.
     * \throws `tl::Exception` if the string cannot be converted to the property's type.
     */
    void fromString(const std::string &value) override
    {
        try {
            PropertySerializer<T>::fromString(value, mValue);
        } catch(...) {
            TL_THROW_EXCEPTION_WITH_NESTED("Failed to parse property from string: {}", value);
        }
    }

    /*!
     * \brief Returns the name of the property's type.
     * \return The type name as a string (e.g., `"int"`, `"double"`).
     */
	auto typeName() const noexcept -> std::string override
    {
        return PropertySerializer<T>::typeName();
    }

    /*!
     * \brief Creates a deep copy of this property.
     * \return A `std::unique_ptr` to the cloned property.
     */
    [[nodiscard]]
    auto clone() const -> std::unique_ptr<PropertyBase> final override
    {
        return std::make_unique<Property<T>>(mValue);
    }
};

/*! \} */

} // namespace tl
