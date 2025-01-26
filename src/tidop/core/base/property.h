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

#include <type_traits>
#include <stdexcept>
#include <unordered_map>


namespace tl
{


/*! \addtogroup Base
 *  \{
 */


class PropertyBase
{

public:

    using SharedPtr = std::shared_ptr<PropertyBase>;

private:

    Type mType;

public:

    PropertyBase(Type type)
        : mType(type)
    {
    }
    virtual ~PropertyBase() = default;

    virtual auto toString() const -> std::string = 0;
    virtual void fromString(const std::string &value) = 0;
    virtual auto clone() const -> PropertyBase* = 0;
	auto type() const -> Type
    {
        return mType;
    }
	virtual auto typeName() const -> std::string = 0;
	
};



template <typename T>
class Property 
  : public PropertyBase 
{

private:

    T mValue; ///< Valor almacenado con su tipo original.

public:

    explicit Property(const T &value) 
      : PropertyBase(TypeTraits<T>::id_type),
        mValue(value) {}

    auto value() const -> T {
        return mValue;
    }

    void setValue(const T &value) {
        mValue = value;
    }

    auto toString() const -> std::string override 
    {
        return toStringImpl(mValue);
    }

    void fromString(const std::string &value) override
    {
        try {
            mValue = convertStringTo<T>(value);
        } catch(...) {

        }
    }

    auto clone() const -> PropertyBase* override 
    {
        return new Property<T>(mValue);
    }

	auto typeName() const -> std::string override
    {
        return TypeTraits<T>::name_type;
    }

private:

    std::string toStringImpl(const bool &value) const
    {
        return value ? "true" : "false";
    }

    std::string toStringImpl(const std::string &value) const
    {
        return value;
    }

    std::string toStringImpl(const tl::Path &value) const
    {
        return value.toString();
    }

    template <typename U>
    std::string toStringImpl(const U &value) const 
    {
        return std::to_string(value);
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


/// \cond

namespace internal
{


template<typename T>
class PropertyValue
{

public:

    PropertyValue(/* args */) {}

    T value(const PropertyBase::SharedPtr &arg);
};

template<typename T>
auto PropertyValue<T>::value(const PropertyBase::SharedPtr &arg) -> T
{
    T value{};

    try {

        TL_ASSERT(arg, "Property pointer is null");

        auto type = arg->type();
        auto return_type = TypeTraits<T>::property_type;

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
            value = numberCast<T>(std::dynamic_pointer_cast<Property<bool>>(arg)->value());
            break;
        case Type::type_int8:
            value = numberCast<T>(std::dynamic_pointer_cast<Property<char>>(arg)->value());
            break;
        case Type::type_uint8:
            value = numberCast<T>(std::dynamic_pointer_cast<Property<unsigned char>>(arg)->value());
            break;
        case Type::type_int16:
            value = numberCast<T>(std::dynamic_pointer_cast<Property<short>>(arg)->value());
            break;
        case Type::type_uint16:
            value = numberCast<T>(std::dynamic_pointer_cast<Property<unsigned short>>(arg)->value());
            break;
        case Type::type_int32:
            value = numberCast<T>(std::dynamic_pointer_cast<Property<int>>(arg)->value());
            break;
        case Type::type_uint32:
            value = numberCast<T>(std::dynamic_pointer_cast<Property<unsigned int>>(arg)->value());
            break;
        case Type::type_float32:
            value = numberCast<T>(std::dynamic_pointer_cast<Property<float>>(arg)->value());
            break;
        case Type::type_float64:
            value = numberCast<T>(std::dynamic_pointer_cast<Property<double>>(arg)->value());
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
inline auto PropertyValue<std::string>::value(const PropertyBase::SharedPtr &arg) -> std::string
{
    std::string value;

    try {

        TL_ASSERT(arg, "Property pointer is null");

        const auto type = arg->type();

        switch (type) {
        case Type::type_unknown:
            TL_THROW_EXCEPTION("Unknown Property type");
        case Type::type_string:
            value = std::dynamic_pointer_cast<Property<std::string>>(arg)->value();
            break;
        case Type::type_path:
            value = std::dynamic_pointer_cast<Property<tl::Path>>(arg)->value().toString();
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
inline auto PropertyValue<tl::Path>::value(const PropertyBase::SharedPtr &arg) -> tl::Path
{
    tl::Path value;

    try {

        TL_ASSERT(arg, "Property pointer is null");

        const auto type = arg->type();

        switch (type) {
        case Type::type_unknown:
            TL_THROW_EXCEPTION("Unknown Property type");
        case Type::type_string:
            value = tl::Path(std::dynamic_pointer_cast<Property<std::string>>(arg)->value());
            break;
        case Type::type_path:
            value = std::dynamic_pointer_cast<Property<tl::Path>>(arg)->value();
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
    void setProperty(const std::string &key, const T &value) {
        mProperties[key] = std::shared_ptr<Property<T>>(value);
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
            _value = property_value.value(it->second);
        } catch (std::exception &e) {
            std::throw_with_nested(std::bad_cast());
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
    auto getPropertyAsString(const std::string &key) const -> std::string {
        auto it = mProperties.find(key);
        if (it == mProperties.end()) {
            throw std::out_of_range("Property not found: " + key);
        }
        return it->second->toString();
    }

    /*!
     * \brief Establece una propiedad a partir de una cadena de texto.
     * \param[in] key Clave de la propiedad.
     * \param[in] value Cadena con el valor a asignar.
     * \throws std::out_of_range Si la clave no existe.
     */
    void setPropertyFromString(const std::string &key, const std::string &value) {
        auto it = mProperties.find(key);
        if (it == mProperties.end()) {
            throw std::out_of_range("Property not found: " + key);
        }
        it->second->fromString(value);
    }

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
    auto getPropertyType(const std::string &key) const -> Type {
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


/*! \} */


} // End namespace tl
