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
 * \file Property.h
 * \brief Type-safe property system with dynamic type conversion
 *
 * This module provides a generic property system for storing and retrieving typed values
 * with automatic conversion capabilities. Properties can be dynamically converted between
 * compatible types and serialized to/from strings.
 *
 * ### Classes
 *
 * - \ref PropertyBase - Abstract base class for all property types
 * - \ref Property<T> - Generic property implementation for type T
 * - \ref Property<Size<T>> - Specialized property for Size types
 * - \ref Properties - Container managing multiple named properties
 *
 * ### Features
 *
 * - Type-safe property storage with RTTI support
 * - Automatic type conversion between compatible types
 * - String serialization/deserialization
 * - Support for built-in types (int, float, bool, etc.)
 * - Support for custom types (Path, Size)
 * - Container-style interface (iterators, find, etc.)
 * - Loss-of-data warnings on narrowing conversions
 *
 * ### Example Usage
 *
 * \code{.cpp}
 * #include "tidop/core/base/Property.h"
 *
 * tl::Properties config;
 * config.setProperty("width", 1920);
 * config.setProperty("height", 1080);
 * config.setProperty("enabled", true);
 * config.setProperty("name", "Desktop");
 *
 * int w = config.getProperty<int>("width");
 * std::string name = config.getProperty<std::string>("name");
 * std::string configStr = config.getPropertyAsString("enabled");
 * \endcode
 *
 * \see tl::PropertyBase, tl::Property, tl::Properties
 */

#pragma once

#include "tidop/config.h"

#include <type_traits>
#include <stdexcept>
#include <unordered_map>
#include <iostream>

#include "tidop/core/base/type.h"
#include "tidop/core/base/Exception.h"
#include "tidop/core/base/StringUtils.h"
#include "tidop/core/base/TypeConversions.h"

namespace tl
{

class PropertyBase;

namespace internal
{

template<typename T>
class PropertyValue
{

public:

    PropertyValue(/* args */) {}

    T value(const PropertyBase *arg);
};

}

/*! \addtogroup Base
 *  \{
 */

/*!
 * \brief Base class for all property types.
 *
 * This class provides a common interface for properties, allowing them to be converted to and from strings.
 * It also stores the type of the property.
 */
class PropertyBase
{

public:

    using SharedPtr = std::shared_ptr<PropertyBase>;

private:

    Type mType;

public:

    /*!
     * \brief Constructor.
     * \param type The type of the property.
     */
    PropertyBase(Type type)
        : mType(type)
    {
    }

    virtual ~PropertyBase() = default;

    /*!
     * \brief Converts the property's value to a string.
     * \return A string representation of the property's value.
     */
    virtual auto toString() const -> std::string = 0;

    /*!
     * \brief Sets the property's value from a string.
     * \param[in] value A string representation of the value to set.
     * \exception Exception If the string cannot be converted to the property's type.
     */
    virtual void fromString(const std::string &value) = 0;

    /*!
     * \brief Gets the type of the property.
     * \return The type of the property as a `Type` enum value.
     */
	auto type() const TL_NOEXCEPT -> Type
    {
        return mType;
    }

    /*!
     * \brief Gets the name of the property's type.
     * \return The name of the property's type as a string.
     */
	virtual auto typeName() const -> std::string = 0;
	
};


/*!
 * \brief A templated class representing a specific property with a defined type.
 *
 * This class extends PropertyBase and implements functionality for converting the property
 * value to and from strings, as well as accessing and modifying the property's value.
 *
 * \tparam T The type of the property's value.
 */
template <typename T>
class Property 
  : public PropertyBase 
{

private:

    T mValue;

public:

    Property()
      : PropertyBase(TypeTraits<T>::id_type)
    {
    }

    /*!
     * \brief Constructor.
     * \param[in] value The initial value of the property.
     */
    explicit Property(const T &value) 
      : PropertyBase(TypeTraits<T>::id_type),
        mValue(value) {}

    /*!
     * \brief Gets the property's value.
     * \return The value of the property.
     */
    auto value() const TL_NOEXCEPT -> T
    {
        return mValue;
    }

    /*!
     * \brief Sets the property's value.
     * \param[in] value The new value to set.
     */
    void setValue(const T &value)  
    {
        mValue = value;
    }

    /*!
     * \brief Converts the property's value to a string.
     * \return A string representation of the property's value.
     * \exception Exception If the value cannot be converted to a string.
     */
    auto toString() const -> std::string override 
    {
        try {
            internal::PropertyValue<std::string> property_value;
            return property_value.value(this);
        } catch (...) {
            TL_THROW_EXCEPTION_WITH_NESTED("Catched exception");
        }
    }

    /*!
     * \brief Sets the property's value from a string.
     * \param[in] value A string representation of the value to set.
     * \exception Exception If the string cannot be converted to the property's type.
     */
    void fromString(const std::string &value) override
    {
        try {
            mValue = convertStringTo<T>(value);
        } catch(...) {
            TL_THROW_EXCEPTION_WITH_NESTED("Catched exception");
        }
    }

    /*!
     * \brief Gets the name of the property's type.
     * \return The name of the property's type as a string.
     */
	auto typeName() const TL_NOEXCEPT -> std::string override
    {
        return TypeTraits<T>::name_type;
    }
};

template<>
inline void Property<std::string>::fromString(const std::string &value)
{
    mValue = value;
}

template<>
inline void Property<Path>::fromString(const std::string &value)
{
    mValue = Path(value);
}




template <typename T>
class Property<Size<T>>
    : public PropertyBase
{
private:

    Size<T> mValue;

public:

    explicit Property(const Size<T> &value)
        : PropertyBase(TypeTraits<Size<T>>::id_type), mValue(value) {
    }

    auto value() const TL_NOEXCEPT -> Size<T>
    {
        return mValue;
    }

    void setValue(const Size<T> &value)
    {
        mValue = value;
    }

    auto toString() const -> std::string override
    {
        try {
            return std::to_string(mValue.width) + "x" + std::to_string(mValue.height);
        } catch (...) {
            TL_THROW_EXCEPTION_WITH_NESTED("Error converting Size<T> to string");
        }
    }

    void fromString(const std::string &value) override
    {
        try {
            size_t pos = value.find('x');
            if (pos == std::string::npos) {
                TL_THROW_EXCEPTION("Invalid Size format. Expected 'width x height' (e.g., '1920x1080')");
            }

            std::string widthStr = value.substr(0, pos);
            std::string heightStr = value.substr(pos + 1);

            T width = convertStringTo<T>(widthStr);
            T height = convertStringTo<T>(heightStr);

            if (width <= 0 || height <= 0) {
                TL_THROW_EXCEPTION("Size dimensions must be positive: {}x{}", width, height);
            }

            mValue = Size<T>(width, height);
        } catch (...) {
            TL_THROW_EXCEPTION_WITH_NESTED("Failed to convert string to Size<T>");
        }
    }

    auto typeName() const TL_NOEXCEPT -> std::string override
    {
        return TypeTraits<Size<T>>::name_type;
    }
};





template <typename T>
inline auto formatValue(const T &value) -> std::string
{
    return std::to_string(value);
}

// Specialization for std::string
template <>
inline auto formatValue<std::string>(const std::string &value) -> std::string
{
    return tl::format("\"{}\"", value);
}

// Specialization for bool
template <>
inline auto formatValue<bool>(const bool &value) -> std::string
{
    return value ? "true" : "false";
}

// Specialization for const char*
template <>
inline auto formatValue<const char *>(const char *const &value) -> std::string
{
    return tl::format("\"{}\"", value);
}

template <typename Key, typename Value>
auto mapToString(const std::map<Key, Value> &m) -> std::string
{
    if (m.empty()) return "{}";

    std::string result = "{";
    bool first = true;

    for (const auto p : m) {
        auto key = p.first;
        auto value = p.second;
        if (!first) result += ",";

        std::string key_str = formatValue<Key>(key);
        std::string value_str = formatValue<Value>(value);

        result += tl::format("{}:{}", key_str, value_str);
        first = false;
    }

    result += "}";
    return result;
}


template <typename Key, typename Value>
auto stringToMap(const std::string &str) -> std::map<Key, Value>
{
    std::map<Key, Value> result;

    if (str.empty() || str == "{}") return result;

    std::string trimmed = str;
    if (trimmed.front() == '{') trimmed = trimmed.substr(1);
    if (trimmed.back() == '}') trimmed.pop_back();

    std::istringstream ss(trimmed);
    std::string pair;

    while (std::getline(ss, pair, ',')) {
        // Trim whitespace
        pair.erase(0, pair.find_first_not_of(" \t"));
        pair.erase(pair.find_last_not_of(" \t") + 1);

        size_t sep = pair.find(':');
        if (sep == std::string::npos) {
            TL_THROW_EXCEPTION("Invalid map format. Expected 'key:value'");
        }

        std::string key_str = pair.substr(0, sep);
        std::string value_str = pair.substr(sep + 1);

        // Trim whitespace
        key_str.erase(0, key_str.find_first_not_of(" \t"));
        key_str.erase(key_str.find_last_not_of(" \t") + 1);
        value_str.erase(0, value_str.find_first_not_of(" \t"));
        value_str.erase(value_str.find_last_not_of(" \t") + 1);

        // Remove quotes si están presentes
        if (!key_str.empty() && key_str.front() == '"' && key_str.back() == '"') {
            key_str = key_str.substr(1, key_str.length() - 2);
        }
        if (!value_str.empty() && value_str.front() == '"' && value_str.back() == '"') {
            value_str = value_str.substr(1, value_str.length() - 2);
        }

        Property<Key> key_property;
        key_property.fromString(key_str);
        Key k = key_property.value();
        Property<Value> value_property;
        value_property.fromString(value_str);
        Value v = value_property.value();
        result[k] = v;
    }

    return result;
}

template <typename Key, typename Value>
class Property<std::map<Key, Value>> 
  : public PropertyBase
{

private:

    std::map<Key, Value> mValue;

public:

    explicit Property(const std::map<Key, Value> &value)
      : PropertyBase(TypeTraits<std::map<Key, Value>>::id_type),
        mValue(value) {
    }

    /*!
     * \brief Gets the map value.
     * \return The stored map
     */
    auto value() const TL_NOEXCEPT -> std::map<Key, Value>
    {
        return mValue;
    }

    /*!
     * \brief Sets the map value.
     * \param[in] value The new map value
     */
    void setValue(const std::map<Key, Value> &value)
    {
        mValue = value;
    }

    /*!
     * \brief Converts map to string representation.
     * 
     * Format: "key1:value1,key2:value2,..."
     * Empty map: "{}"
     * 
     * \return String representation
     */
    auto toString() const -> std::string override
    {
        try {
            return mapToString(mValue);
        } catch (...) {
            TL_THROW_EXCEPTION_WITH_NESTED("Failed to convert std::map to string");
        }
    }

    /*!
     * \brief Converts string to map.
     * 
     * Expected format: "key1:value1,key2:value2,..."
     * 
     * \param[in] value String representation of map
     * \exception Exception If format is invalid
     */
    void fromString(const std::string &value) override
    {
        try {
            mValue = stringToMap<Key, Value>(value);
        } catch (...) {
            TL_THROW_EXCEPTION_WITH_NESTED("Failed to convert string to std::map");
        }
    }

   auto typeName() const TL_NOEXCEPT -> std::string override
   {
        return tl::format("std::map<{},{}>",
                          TypeTraits<Key>::name_type,
                          TypeTraits<Value>::name_type);
   }

};

/// \cond

namespace internal
{


template<typename T>
auto PropertyValue<T>::value(const PropertyBase *arg) -> T
{
    T value{};

    try {

        TL_ASSERT(arg, "Property pointer is null");

        auto type = arg->type();
        auto return_type = TypeTraits<T>::id_type;

        if (type != return_type) {
            TL_ASSERT(type != Type::type_string, "Conversion from \"{}\" to \"std::string\" is not allowed", arg->typeName());
            if (type < Type::type_string && return_type < Type::type_string && return_type < type) {
                std::string type_name = TypeTraits<T>::name_type;
                Message::warning("Conversion from \"{}\" to \"{}\", possible loss of data", arg->typeName(), type_name);
            }
        }

        switch (type) {
        case Type::type_unknown:
            TL_THROW_EXCEPTION("Unknown Property type");
        case Type::type_bool:
            value = numberCast<T>(dynamic_cast<const Property<bool> *>(arg)->value());
            break;
        case Type::type_int8:
            value = numberCast<T>(dynamic_cast<const Property<signed char> *>(arg)->value());
            break;
        case Type::type_uint8:
            value = numberCast<T>(dynamic_cast<const Property<unsigned char> *>(arg)->value());
            break;
        case Type::type_int16:
            value = numberCast<T>(dynamic_cast<const Property<short> *>(arg)->value());
            break;
        case Type::type_uint16:
            value = numberCast<T>(dynamic_cast<const Property<unsigned short>*>(arg)->value());
            break;
        case Type::type_int32:
            value = numberCast<T>(dynamic_cast<const Property<int>*>(arg)->value());
            break;
        case Type::type_uint32:
            value = numberCast<T>(dynamic_cast<const Property<unsigned int>*>(arg)->value());
            break;
        case Type::type_int64:
            value = numberCast<T>(dynamic_cast<const Property<long long>*>(arg)->value());
            break;
        case Type::type_uint64:
            value = numberCast<T>(dynamic_cast<const Property<unsigned long long>*>(arg)->value());
            break;
        case Type::type_float32:
            value = numberCast<T>(dynamic_cast<const Property<float>*>(arg)->value());
            break;
        case Type::type_float64:
            value = numberCast<T>(dynamic_cast<const Property<double>*>(arg)->value());
            break;
        default:
            break;
        }

    } catch (...) {
        TL_THROW_EXCEPTION_WITH_NESTED("Catched exception");
    }

    return value;
}

template<>
inline auto PropertyValue<std::string>::value(const PropertyBase *arg) -> std::string
{
    std::string value;

    try {

        TL_ASSERT(arg, "Property pointer is null");

        const auto type = arg->type();

        switch (type) {
        case Type::type_unknown:
            TL_THROW_EXCEPTION("Unknown Property type");
        case Type::type_string:
            value = dynamic_cast<const Property<std::string>*>(arg)->value();
            break;
        case Type::type_path:
            value = dynamic_cast<const Property<tl::Path>*>(arg)->value().toString();
            break;
        case Type::type_bool:
            value = dynamic_cast<const Property<bool> *>(arg)->value() ? "true" : "false";
            break;
        case Type::type_int8:
            value = std::to_string(dynamic_cast<const Property<int8_t> *>(arg)->value());
            break;
        case Type::type_uint8:
            value = std::to_string(dynamic_cast<const Property<uint8_t> *>(arg)->value());
            break;
        case Type::type_int16:
            value = std::to_string(dynamic_cast<const Property<int16_t> *>(arg)->value());
            break;
        case Type::type_uint16:
            value = std::to_string(dynamic_cast<const Property<uint16_t>*>(arg)->value());
            break;
        case Type::type_int32:
            value = std::to_string(dynamic_cast<const Property<int>*>(arg)->value());
            break;
        case Type::type_uint32:
            value = std::to_string(dynamic_cast<const Property<unsigned int>*>(arg)->value());
            break;
        case Type::type_int64:
            value = std::to_string(dynamic_cast<const Property<int64_t>*>(arg)->value());
            break;
        case Type::type_uint64:
            value = std::to_string(dynamic_cast<const Property<uint64_t>*>(arg)->value());
            break;
        case Type::type_float32:
            value = std::to_string(dynamic_cast<const Property<float>*>(arg)->value());
            break;
        case Type::type_float64:
            value = std::to_string(dynamic_cast<const Property<double>*>(arg)->value());
            break;
        default:
            TL_THROW_EXCEPTION("Conversion from \"{}\" to \"std::string\" is not allowed", arg->typeName());
        }

    } catch (...) {
        TL_THROW_EXCEPTION_WITH_NESTED("Catched exception");
    }

    return value;
}

template<>
inline auto PropertyValue<tl::Path>::value(const PropertyBase *arg) -> tl::Path
{
    tl::Path value;

    try {

        TL_ASSERT(arg, "Property pointer is null");

        const auto type = arg->type();

        switch (type) {
        case Type::type_unknown:
            TL_THROW_EXCEPTION("Unknown Property type");
        case Type::type_string:
            value = tl::Path(dynamic_cast<const Property<std::string>*>(arg)->value());
            break;
        case Type::type_path:
            value = dynamic_cast<const Property<tl::Path>*>(arg)->value();
            break;
        default:
            TL_THROW_EXCEPTION("Conversion from \"{}\" to \"tl::Path\" is not allowed", arg->typeName());
        }

    } catch (...) {
        TL_THROW_EXCEPTION_WITH_NESTED("Catched exception");
    }

    return value;
}

template<typename T>
struct PropertyValue<Size<T>>
{
    auto value(const PropertyBase *arg) -> Size<T>
    {
        Size<T> value{};

        try {

            TL_ASSERT(arg, "Property pointer is null");

            const auto type = arg->type();
            auto return_type = TypeTraits<Size<T>>::id_type;

            if (type != return_type) {
                TL_THROW_EXCEPTION("Cannot convert from type {}<{}> to Size<{}>", arg->typeName(), TypeTraits<Size<T>>::name_type, TypeTraits<T>::name_type);
            }

            switch (type) {
            case Type::type_size:
                value = dynamic_cast<const Property<Size<T>>*>(arg)->value();
                break;
            default:
                TL_THROW_EXCEPTION("Unsupported type for Size conversion");
            }
        } catch (...) {
            TL_THROW_EXCEPTION_WITH_NESTED("Catched exception");
        }

        return value;
    }
};

} // namespace internal 

/// \endcond

/*!
 * \class Properties
 * \brief Manages a collection of properties using a key-value storage system.
 *
 * This class allows storing, retrieving, and modifying properties dynamically.
 * Each property is associated with a string key and can hold different types of values.
 * \example Core/properties.cpp
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

    Properties(std::string name);

    auto name() const TL_NOEXCEPT -> std::string;

    void setName(const std::string &name);

    /*!
     * \brief Adds or updates a property with a given value.
     * \tparam T The type of the property.
     * \param[in] key The key associated with the property.
     * \param[in] value The value of the property.
     */
    template <typename T>
    void setProperty(const std::string &key, T value);

    /*!
     * \brief Retrieves a property as type T.
     * \tparam T The expected type of the property.
     * \param[in] key The key associated with the property.
     * \return The property value as type T.
     * \throws std::out_of_range If the key does not exist.
     * \throws std::bad_cast If the type does not match the stored value.
     */
    template <typename T>
    auto getProperty(const std::string &key) const -> T;

    /*!
     * \brief Retrieves a property as a string.
     * \param[in] key The key associated with the property.
     * \return The property value as a string.
     * \throws std::out_of_range If the key does not exist.
     */
    auto getPropertyAsString(const std::string &key) const -> std::string;

    /*!
     * \brief Checks whether a property exists.
     * \param[in] key The key associated with the property.
     * \return True if the property exists; otherwise, false.
     */
    auto hasProperty(const std::string &key) const -> bool;

    /*!
     * \brief Retrieves the type of a property.
     * \param[in] key The key associated with the property.
     * \return The type of the property.
     * \throws std::out_of_range If the key does not exist.
     */
    auto getPropertyType(const std::string &key) const -> Type;

    /*!
     * \brief Returns an iterator to the beginning of the properties.
     * \return Iterator pointing to the beginning of the properties.
     */
    auto begin() TL_NOEXCEPT -> iterator;

    /*!
     * \brief Returns an iterator to the end of the properties.
     * \return Iterator pointing to the end of the properties.
     */
    auto end() TL_NOEXCEPT -> iterator;

    /*!
     * \brief Returns a constant iterator to the beginning of the properties.
     * \return Constant iterator pointing to the beginning of the properties.
     */
    auto begin() const TL_NOEXCEPT -> const_iterator;

    /*!
     * \brief Returns a constant iterator to the end of the properties.
     * \return Constant iterator pointing to the end of the properties.
     */
    auto end() const TL_NOEXCEPT -> const_iterator;

    /*!
     * \brief Returns a constant iterator to the beginning of the properties (explicit const version).
     * \return Constant iterator pointing to the beginning of the properties.
     */
    auto cbegin() const TL_NOEXCEPT -> const_iterator;
    
    /*!
     * \brief Returns a constant iterator to the end of the properties (explicit const version).
     * \return Constant iterator pointing to the end of the properties.
     */
    auto cend() const TL_NOEXCEPT -> const_iterator;

    /*!
     * \brief Clears all stored properties.
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
    void print() const
    {
        std::cout << "Properties: " << mName << std::endl;
        for (const auto &property : mProperties) {
            std::cout << "  " << property.first << "   " << property.second->toString() << std::endl;
        }
    }
};



/*! \} */


template<typename T>
inline void Properties::setProperty(const std::string &key, T value)
{
    mProperties[key] = std::make_shared<Property<typename std::decay<T>::type>>(value);
}

template <>
inline void Properties::setProperty(const std::string &key, const char *value)
{
    mProperties[key] = std::make_shared<Property<std::string>>(std::string(value));
}

template<typename T>
inline auto Properties::getProperty(const std::string &key) const -> T
{
    T _value{};

    try {
        auto it = mProperties.find(key);
        if (it == mProperties.end()) {
            throw std::out_of_range("Property not found: " + key);
        }
        internal::PropertyValue<T> property_value;
        _value = property_value.value(it->second.get());
    } catch (std::exception &e) {
        std::throw_with_nested(e);
    } catch (...) {
        ///TODO: comprobar
        std::throw_with_nested(std::bad_cast());
    }

    return _value;
}

inline auto Properties::begin() TL_NOEXCEPT -> iterator
{ 
    return mProperties.begin();
}

inline auto Properties::end() TL_NOEXCEPT -> iterator 
{ 
    return mProperties.end(); 
}

inline auto Properties::begin() const TL_NOEXCEPT -> const_iterator
{ 
    return mProperties.begin();
}

inline auto Properties::end() const TL_NOEXCEPT -> const_iterator
{ 
    return mProperties.end();
}

inline auto Properties::cbegin() const TL_NOEXCEPT -> const_iterator 
{ 
    return mProperties.cbegin();
}

inline auto Properties::cend() const TL_NOEXCEPT -> const_iterator 
{ 
    return mProperties.cend();
}

inline void Properties::clear()
{
    mProperties.clear();
}

} // End namespace tl
