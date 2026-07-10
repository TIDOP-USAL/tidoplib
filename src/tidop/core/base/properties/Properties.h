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

/*! \file Properties.h
 * \brief Dynamic property container with type-safe access.
 *
 * This file defines the `Properties` class, which manages a collection of
 * named properties of arbitrary types. Properties are stored as `PropertyBase`
 * objects and can be accessed with type safety using `getProperty<T>()`.
 *
 * \ingroup Core
 * \see tl::PropertyBase, tl::Property
 */

#pragma once

#include "tidop/config.h"

#include <unordered_map>
#include <string>
#include <memory>
#include <iostream>

#include "tidop/core/base/Exception.h"
#include "tidop/core/base/properties/Property.h"

namespace tl
{

/*! \addtogroup Base
 *  \{
 */

/*!
 * \class Properties
 * \brief Manages a collection of typed properties.
 *
 * This class stores a collection of named properties, each with a type known
 * at runtime. Properties can be added, retrieved, and modified dynamically.
 * The class provides type-safe access via `getProperty<T>()` and iteration
 * over the property map.
 *
 * ### Example
 * \code
 * Properties config("Settings");
 * config.setProperty("width", 1920);
 * config.setProperty("height", 1080);
 * config.setProperty("title", std::string("My App"));
 * config.setProperty("fullscreen", true);
 *
 * int width = config.getProperty<int>("width");
 * bool fs = config.getProperty<bool>("fullscreen");
 * config.print();
 * \endcode
 *
 * \see PropertyBase, Property
 */
class Properties 
{

public:

    /*!
     * \brief Iterator type for traversing and modifying properties.
     */
    using iterator = std::unordered_map<std::string, std::shared_ptr<PropertyBase>>::iterator;

    /*!
     * \brief Constant iterator type for read-only traversal of properties.
     */
    using const_iterator = std::unordered_map<std::string, std::shared_ptr<PropertyBase>>::const_iterator;

private:

    std::string mName;
    std::unordered_map<std::string, std::shared_ptr<PropertyBase>> mProperties; 

public:

    /*!
     * \brief Default constructor.
     */
    Properties() = default;

    /*!
     * \brief Constructs a property set with a name.
     * \param[in] name The name of the property set.
     */
    explicit Properties(std::string name);

    /*!
     * \brief Copy constructor.
     * Performs a deep copy of all properties.
     */
    Properties(const Properties &other);

    /*!
     * \brief Move constructor.
     */
    Properties(Properties &&) noexcept = default;

    /*! \brief Destructor. */
    ~Properties() = default;

    /*!
     * \brief Move assignment operator.
     */
    auto operator=(Properties &&) noexcept -> Properties & = default;

    /*!
     * \brief Copy assignment operator.
     * Performs a deep copy of all properties.
     */
    auto operator=(const Properties &other) -> Properties &;

    /*!
     * \brief Returns the name of the property set.
     * \return The name.
     */
    auto name() const noexcept -> std::string;

    /*!
     * \brief Sets the name of the property set.
     * \param[in] name The new name.
     */
    void setName(const std::string &name);

    /*!
     * \brief Adds or updates a property with the given key and value.
     * \tparam T The type of the property.
     * \param[in] key   The property key.
     * \param[in] value The property value.
     *
     * \note If the key already exists, the property is overwritten.
     * \note For string literals (`const char*`), a `std::string` property is created.
     */
    template <typename T>
    void setProperty(const std::string &key, T value);

    /*!
     * \brief Retrieves a property as type `T`.
     * \tparam T The expected type of the property.
     * \param[in] key The property key.
     * \return The property value as type `T`.
     * \throws `tl::Exception` if the key does not exist or if the type mismatch occurs.
     *
     * ### Example
     * \code
     * int width = config.getProperty<int>("width");
     * bool fullscreen = config.getProperty<bool>("fullscreen");
     * \endcode
     */
    template <typename T>
    auto getProperty(const std::string &key) const -> T;

    /*!
     * \brief Retrieves a property as a string.
     * \param[in] key The property key.
     * \return The property value as a string.
     * \throws `tl::Exception` if the key does not exist.
     */
    auto getPropertyAsString(const std::string &key) const -> std::string;

    /*!
     * \brief Checks whether a property exists.
     * \param[in] key The property key.
     * \return `true` if the property exists; `false` otherwise.
     */
    auto hasProperty(const std::string &key) const -> bool;

    /*!
     * \brief Returns an iterator to the beginning of the properties.
     * \return Iterator pointing to the first property.
     */
    auto begin() noexcept -> iterator;

    /*!
     * \brief Returns an iterator to the end of the properties.
     * \return Iterator pointing to the end of the properties.
     */
    auto end() noexcept -> iterator;

    /*!
     * \brief Returns a constant iterator to the beginning of the properties.
     * \return Constant iterator pointing to the beginning of the properties.
     */
    auto begin() const noexcept -> const_iterator;

    /*!
     * \brief Returns a constant iterator to the end of the properties.
     * \return Constant iterator pointing to the end of the properties.
     */
    auto end() const noexcept -> const_iterator;

    /*!
     * \brief Returns a constant iterator to the beginning of the properties (explicit const version).
     * \return Constant iterator pointing to the beginning of the properties.
     */
    auto cbegin() const noexcept -> const_iterator;
    
    /*!
     * \brief Returns a constant iterator to the end of the properties (explicit const version).
     * \return Constant iterator pointing to the end of the properties.
     */
    auto cend() const noexcept -> const_iterator;

    /*!
     * \brief Removes all stored properties.
     */
    void clear();

    /*!
     * \brief Prints all properties to standard output.
     *
     * Outputs all properties in a human-readable format to the console.
     * Format: "Properties: <name>\n  <key>   <value>\n  ..."
     *
     * ### Example Output
     * \code
     * Properties: Config
     *   width   1920
     *   height  1080
     *   enabled true
     * \endcode
     *
     * \see getPropertyAsString()
     */
    void print() const;
};


/*! \} */


inline auto Properties::name() const noexcept -> std::string
{
    return mName;
}

inline void Properties::setName(const std::string &name)
{
    mName = name;
}

template<typename T>
inline void Properties::setProperty(const std::string &key, T value)
{
    mProperties[key] = std::make_unique<Property<std::decay_t<T>>>(value);
}

template <>
inline void Properties::setProperty(const std::string &key, const char *value)
{
    mProperties[key] = std::make_unique<Property<std::string>>(std::string(value));
}

template<typename T>
inline auto Properties::getProperty(const std::string &key) const -> T
{
    T _value{};

    try {

        auto it = mProperties.find(key);
        if (it == mProperties.end()) {
            TL_THROW_EXCEPTION("Property '{}' not found in registry", key);
        }

        const auto *prop = dynamic_cast<const Property<T>*>(it->second.get());
        if (!prop) {
            TL_THROW_EXCEPTION(
                "Type mismatch for property '{}'. Requested type does not match stored property type '{}'",
                key, it->second->typeName()
            );
        }

        return prop->value();

    } catch (...) {
        TL_THROW_EXCEPTION_WITH_NESTED("Failed to get property '{}'", key);
    }

    return _value;
}

inline auto Properties::begin() noexcept -> iterator
{ 
    return mProperties.begin();
}

inline auto Properties::end() noexcept -> iterator 
{ 
    return mProperties.end(); 
}

inline auto Properties::begin() const noexcept -> const_iterator
{ 
    return mProperties.begin();
}

inline auto Properties::end() const noexcept -> const_iterator
{ 
    return mProperties.end();
}

inline auto Properties::cbegin() const noexcept -> const_iterator 
{ 
    return mProperties.cbegin();
}

inline auto Properties::cend() const noexcept -> const_iterator 
{ 
    return mProperties.cend();
}

inline void Properties::clear()
{
    mProperties.clear();
}

} // namespace tl
