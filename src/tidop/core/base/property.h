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
#include "tidop/core/base/type.h"
#include "tidop/core/base/exception.h"
#include "tidop/core/base/string_utils.h"
#include "tidop/core/base/type_conversions.h"

#include <type_traits>
#include <stdexcept>
#include <unordered_map>


namespace tl
{

namespace internal
{
template<typename T> class PropertyValue;
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
	auto type() const -> Type
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
    auto value() const -> T 
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
	auto typeName() const -> std::string override
    {
        return TypeTraits<T>::name_type;
    }
};

template<>
void Property<std::string>::fromString(const std::string &value)
{
    mValue = value;
}

template<>
void Property<Path>::fromString(const std::string &value)
{
    mValue = Path(value);
}



template <typename Key, typename Value>
std::string mapToString(const std::map<Key, Value> &map)
{
    std::ostringstream oss;
    bool first = true;

    for (const auto p/*&[key, value]*/ : map) {
        auto key = p.first;
        auto value = p.second;
        if (!first) {
            oss << ",";
        }
        first = false;

        oss << key << ":" << value;
    }

    return oss.str();
}

template <typename Key, typename Value>
std::map<Key, Value> stringToMap(const std::string &str)
{
    std::map<Key, Value> result;
    std::istringstream ss(str);
    std::string pair;

    while (std::getline(ss, pair, ',')) {
        auto separator = pair.find(':');
        if (separator == std::string::npos) {
            throw Exception("Invalid format for std::map");
        }

        std::string keyStr = pair.substr(0, separator);
        std::string valueStr = pair.substr(separator + 1);

        Key key = convertStringTo<Key>(keyStr);
        Value value = convertStringTo<Value>(valueStr);

        result.emplace(key, value);
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

    auto value() const -> std::map<Key, Value>
    {
        return mValue;
    }

    void setValue(const std::map<Key, Value> &value)
    {
        mValue = value;
    }

    auto toString() const -> std::string override
    {
        return mapToString(mValue);
    }

    void fromString(const std::string &value) override
    {
        try {
            mValue = stringToMap<Key, Value>(value);
        } catch (...) {
            TL_THROW_EXCEPTION_WITH_NESTED("Failed to convert string to std::map");
        }
    }

    auto typeName() const -> std::string override
    {
        return TypeTraits<std::map<Key, Value>>::name_type;
    }

};

/// \cond

namespace internal
{


template<typename T>
class PropertyValue
{

public:

    PropertyValue(/* args */) {}

    T value(const PropertyBase *arg);
};

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

} // namespace internal 

/// \endcond


class Properties 
{

private:

    std::unordered_map<std::string, std::shared_ptr<PropertyBase>> mProperties; 

public:
    Properties() = default;

    /*!
     * \brief Añade o actualiza una propiedad con un valor de tipo T.
     * \tparam T Tipo de la propiedad.
     * \param[in] key Clave de la propiedad.
     * \param[in] value Valor de la propiedad.
     */
    template <typename T>
    void setProperty(const std::string &key, T value) 
    {
        //if constexpr (std::is_same_v<T, const char *>) {
        //    mProperties[key] = std::make_shared<Property<std::string>>(std::string(value));
        //} else {
        //    mProperties[key] = std::make_shared<Property<typename std::decay<T>::type>>(value);
        //}
        mProperties[key] = std::make_shared<Property<std::decay<T>::type>>(value);
    }

    /*!
     * \brief Obtiene una propiedad como tipo T.
     * \tparam T Tipo esperado de la propiedad.
     * \param[in] key Clave de la propiedad.
     * \return Valor de la propiedad como tipo T.
     * \throws std::out_of_range Si la clave no existe.
     * \throws std::bad_cast Si el tipo no coincide con el almacenado.
     */
    template <typename T>
    auto getProperty(const std::string &key) const -> T
    {
        //auto it = mProperties.find(key);
        //if (it == mProperties.end()) {
        //    throw std::out_of_range("Property not found: " + key);
        //}

        //auto *prop = dynamic_cast<Property<T> *>(it->second.get());
        //if (!prop) {
        //    throw std::bad_cast();
        //}

        //return prop->value();

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

    /*!
     * \brief Obtiene una propiedad como una cadena de texto.
     * \param[in] key Clave de la propiedad.
     * \return Valor de la propiedad como cadena.
     * \throws std::out_of_range Si la clave no existe.
     */
    auto getPropertyAsString(const std::string &key) const -> std::string
    {
        auto it = mProperties.find(key);
        if (it == mProperties.end()) {
            throw std::out_of_range("Property not found: " + key);
        }
        return it->second->toString();
    }

    ///*!
    // * \brief Establece una propiedad a partir de una cadena de texto.
    // * \param[in] key Clave de la propiedad.
    // * \param[in] value Cadena con el valor a asignar.
    // * \throws std::out_of_range Si la clave no existe.
    // */
    //void setPropertyFromString(const std::string &key, const std::string &value)
    //{
    //    //auto it = mProperties.find(key);
    //    //if (it == mProperties.end()) {
    //    //    throw std::out_of_range("Property not found: " + key);
    //    //}
    //    //it->second->fromString(value);
    //    mProperties[key] = std::make_shared<Property<T>>(value);
    //}

    /*!
     * \brief Verifica si una propiedad existe.
     * \param[in] key Clave de la propiedad.
     * \return true si la propiedad existe; de lo contrario, false.
     */
    auto hasProperty(const std::string &key) const -> bool {
        return mProperties.find(key) != mProperties.end();
    }

    /*!
     * \brief Devuelve el tipo de una propiedad.
     * \param[in] key Clave de la propiedad.
     * \return Tipo de la propiedad.
     * \throws std::out_of_range Si la clave no existe.
     */
    auto getPropertyType(const std::string &key) const -> Type 
    {
        auto it = mProperties.find(key);
        if (it == mProperties.end()) {
            throw std::out_of_range("Property not found: " + key);
        }
        return it->second->type();
    }

    /*!
     * \brief Limpia todas las propiedades almacenadas.
     */
    void clear() {
        mProperties.clear();
    }
};

template <>
void Properties::setProperty(const std::string &key, const char *value)
{
    mProperties[key] = std::make_shared<Property<std::string>>(std::string(value));
}

/*! \} */


} // End namespace tl
