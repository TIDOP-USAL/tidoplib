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

#include <string>
#include <vector>

#include "tidop/core/defs.h"
#include "tidop/core/utils.h"
#include "tidop/core/path.h"
#include "tidop/core/console/validator.h"

namespace tl
{

/*! \addtogroup core
 *  \{
 */

 /*! \addtogroup Console
  *  \{
  */


/*!
 * \brief The Argument class
 */
class TL_EXPORT Argument
{

public:

    enum class Type
    {
        arg_unknown,
        arg_bool,
        arg_int8,
        arg_uint8,
        arg_int16,
        arg_uint16,
        arg_int32,
        arg_uin32,
        arg_float32,
        arg_float64,
        arg_string,
        arg_char = arg_int8,
        arg_uchar = arg_uint8,
        arg_short = arg_int16,
        arg_ushort = arg_uint16,
        arg_int = arg_int32,
        arg_uint = arg_uin32,
        arg_float = arg_float32,
        arg_double = arg_float64
    };

    using SharedPtr = std::shared_ptr<Argument>;

public:

    /*!
     * \brief Constructora argumento
     * \param[in] name Nombre del argumento
     * \param[in] description Descripción del argumento
     */
    Argument(std::string name, std::string description, Type type);

    /*!
     * \brief Constructora argumento
     * \param[in] shortName Nombre corto del argumento
     * \param[in] description Descripción del argumento
     */
    Argument(const char &shortName, std::string description, Type type);

    /*!
     * \brief Constructora argumento
     * \param[in] name Nombre del argumento
     * \param[in] shortName Nombre corto del argumento
     * \param[in] description Descripción del argumento
     */
    Argument(std::string name, const char &shortName, std::string description, Type type);

    /*!
     * \brief Constructora de copia
     * \param[in] argument Objeto que se copia
     */
    Argument(const Argument &argument);

    /*!
     * \brief Constructora de movimiento
     * \param[in] argument Objeto que se mueve
     */
    Argument(Argument &&argument) TL_NOEXCEPT;

    /*!
     * \brief Destructora
     */
    virtual ~Argument() = default;

    auto operator = (const Argument &argument) -> Argument &;
    auto operator = (Argument &&argument) TL_NOEXCEPT -> Argument &;

    /*!
     * \brief Devuelve el nombre del argumento
     * \return Nombre del argumento
     */
    auto name() const -> std::string;

    /*!
     * \brief Establece el nombre del argumento
     * \param[in] name Nombre del argumento
     */
    void setName(const std::string &name);

    /*!
     * \brief Devuelve la descripción del argumento
     * \return Descripción del argumento
     */
    virtual auto description() const -> std::string;

    /*!
     * \brief Establece la descripción del argumento
     * \param[in] description Descripción del argumento
     */
    void setDescription(const std::string &description);

    /*!
     * \brief Devuelve el nombre corto del argumento
     * \return Nombre corto
     */
    auto shortName() const -> char;

    /*!
     * \brief Establece el nombre corto del argumento
     * \param[in] shortName Nombre corto
     */
    void setShortName(const char &shortName);

    auto type() const -> Type;

    auto validator() const -> std::shared_ptr<Validator>;
    void setValidator(const std::shared_ptr<Validator> &validator);

    /*!
     * \brief Devuelve una cadena de texto con el tipo del argumento
     * \return
     */
    virtual auto typeName() const -> std::string = 0;

    /*!
     * \brief Comprueba si el argumento es obligatorio
     * \return true si es obligatorio
     */
    virtual auto isRequired() const -> bool = 0;

    /*!
     * \brief Establece el valor del argumento a partir de una cadena de texto
     * \param[in] value Valor del argumento como cadena de texto
     */
    virtual void fromString(const std::string &value) = 0;

    /*!
     * \brief Comprueba si el valor pasado al argumento es valido
     * \return
     */
    virtual bool isValid() = 0;

private:

    /*!
     * \brief Nombre del argumento
     */
    std::string mName;

    /*!
     * \brief Descripción del argumento
     */
    std::string mDescription;

    /*!
     * \brief Nombre corto del argumento (Opcional)
     * Es un único caracter
     */
    char mShortName;

    Type mType;

    std::shared_ptr<Validator> mValidator;
};



/* ---------------------------------------------------------------------------------- */



template<typename T>
struct ArgTraits
{
    static constexpr auto property_type = Argument::Type::arg_unknown;
    static constexpr auto type_name = "unknown";
};

template<>
struct ArgTraits<bool>
{
    using value_type = float;
    static constexpr auto property_type = Argument::Type::arg_bool;
    static constexpr auto type_name = "bool";
};

template<>
struct ArgTraits<float>
{
    using value_type = float;
    static constexpr auto property_type = Argument::Type::arg_float;
    static constexpr auto type_name = "float";
};

template<>
struct ArgTraits<double>
{
    using value_type = double;
    static constexpr auto property_type = Argument::Type::arg_double;
    static constexpr auto type_name = "double";
};

template<>
struct ArgTraits<char>
{
    using value_type = char;
    static constexpr auto property_type = Argument::Type::arg_char;
    static constexpr auto type_name = "char";
};

template<>
struct ArgTraits<unsigned char>
{
    using value_type = unsigned char;
    static constexpr auto property_type = Argument::Type::arg_uchar;
    static constexpr auto type_name = "uchar";
};

template<>
struct ArgTraits<short>
{
    using value_type = short;
    static constexpr auto property_type = Argument::Type::arg_short;
    static constexpr auto type_name = "short";
};

template<>
struct ArgTraits<unsigned short>
{
    using value_type = unsigned short;
    static constexpr auto property_type = Argument::Type::arg_ushort;
    static constexpr auto type_name = "ushort";
};

template<>
struct ArgTraits<int>
{
    using value_type = int;
    static constexpr auto property_type = Argument::Type::arg_int;
    static constexpr auto type_name = "int";
};

template<>
struct ArgTraits<unsigned int>
{
    using value_type = unsigned int;
    static constexpr auto property_type = Argument::Type::arg_uint;
    static constexpr auto type_name = "uint";
};

template<>
struct ArgTraits<std::string>
{
    using value_type = std::string;
    static constexpr auto property_type = Argument::Type::arg_string;
    static constexpr auto type_name = "std::string";
};



/* ---------------------------------------------------------------------------------- */




/*!
 * \brief Template para gestionar diferentes tipos de argumentos
 */
template <typename T>
class Argument_
    : public Argument
{

public:

    /*!
     * \brief Constructora argumento
     * \param[in] name Nombre del argumento
     * \param[in] description Descripción del argumento
     */
    Argument_(const std::string &name,
              const std::string &description);

    /*!
     * \brief Constructora argumento opcional
     * \param[in] name Nombre del argumento
     * \param[in] description Descripción del argumento
     * \param[in] default Valor por defecto del argumento.
     */
    Argument_(const std::string &name,
              const std::string &description,
              T value);

    /*!
     * \brief Constructora argumento
     * \param[in] shortName Nombre corto del argumento
     * \param[in] description Descripción del argumento
     */
    Argument_(const char &shortName,
              const std::string &description);

    /*!
     * \brief Constructora argumento opcional
     * \param[in] shortName Nombre corto del argumento
     * \param[in] description Descripción del argumento
     * \param[in] value Valor por defecto del argumento.
     */
    Argument_(const char &shortName,
              const std::string &description,
              T value);

    /*!
     * \brief Constructora argumento
     * \param[in] name Nombre del argumento
     * \param[in] shortName Nombre corto del argumento
     * \param[in] description Descripción del argumento
     */
    Argument_(const std::string &name,
              const char &shortName,
              const std::string &description);

    /*!
     * \brief Constructora argumento
     * \param[in] name Nombre del argumento
     * \param[in] shortName Nombre corto del argumento
     * \param[in] description Descripción del argumento
     * \param[in] value Valor por defecto del argumento.
     */
    Argument_(const std::string &name,
              const char &shortName,
              const std::string &description,
              T value);

    /*!
     * \brief Destructora
     */
    ~Argument_() override = default;

    TL_DISABLE_COPY(Argument_)
    TL_DISABLE_MOVE(Argument_)

    /*!
     * \brief Devuelve una cadena de texto con el tipo del argumento
     * \return
     */
    auto typeName() const -> std::string override;

    /*!
     * \brief Comprueba si el argumento es obligatorio
     * \return true si es obligatorio
     */
    auto isRequired() const -> bool override ;

    /*!
     * \brief Convierte el valor del argumento a cadena de texto
     * \return Cadena de texto con el valor del argumento
     */
     //std::string toString() const override;

     /*!
      * \brief Establece el valor del argumento a partir de una cadena de texto
      * \param[in] value Valor del argumento como cadena de texto
      */
    void fromString(const std::string &value) override;

    /*!
     * \brief Valor del argumento
     * \return Valor del argumento
     */
    auto value() const -> T;

    /*!
     * \brief Establece el valor del argumento
     * \param[in] value Valor del argumento
     */
    virtual void setValue(const T &value);

    auto isValid() -> bool override;

protected:

    //void setValid(bool valid);

private:

    T mValue;
    bool optional;
    bool bValid;
};


/* Definición de unos alias para los tipos mas frecuentes */

using ArgumentInteger = Argument_<int>;
using ArgumentDouble = Argument_<double>;
using ArgumentFloat = Argument_<float>;
using ArgumentBoolean = Argument_<bool>;
using ArgumentString = Argument_<std::string>;
using ArgumentCharRequired = Argument_<char>;



/* Implementación */

template<typename T> inline
Argument_<T>::Argument_(const std::string &name,
                        const std::string &description)
  : Argument(name, description, ArgTraits<T>::property_type),
    mValue(T()),
    optional(false),
    bValid(true)
{
}

template<typename T> inline
Argument_<T>::Argument_(const std::string &name,
                        const std::string &description,
                        T value)
  : Argument(name, description, ArgTraits<T>::property_type),
    mValue(value),
    optional(true),
    bValid(true)
{
}

template<typename T> inline
Argument_<T>::Argument_(const char &shortName,
                        const std::string &description)
  : Argument(shortName, description, ArgTraits<T>::property_type),
    optional(false),
    bValid(true)
{
}

template<typename T> inline
Argument_<T>::Argument_(const char &shortName,
                        const std::string &description,
                        T value)
  : Argument(shortName, description, ArgTraits<T>::property_type),
    mValue(value),
    optional(true),
    bValid(true)
{
}

template<typename T> inline
Argument_<T>::Argument_(const std::string &name,
                        const char &shortName,
                        const std::string &description)
  : Argument(name, shortName, description, ArgTraits<T>::property_type),
    optional(false),
    bValid(true)
{
}

template<typename T> inline
Argument_<T>::Argument_(const std::string &name,
                        const char &shortName,
                        const std::string &description,
                        T value)
  : Argument(name, shortName, description, ArgTraits<T>::property_type),
    mValue(value),
    optional(true),
    bValid(true)
{
}

template<typename T> inline
auto Argument_<T>::typeName() const -> std::string
{
    return ArgTraits<T>::type_name;
}

template<typename T> inline
auto Argument_<T>::isRequired() const -> bool
{
    return !optional;
}

template<typename T> inline
void Argument_<T>::fromString(const std::string &value)
{

    try {
        mValue = convertStringTo<T>(value);
    } catch(...) {
        bValid = false;
    }
}


template<> inline
void Argument_<std::string>::fromString(const std::string &value)
{
    mValue = value;
    bValid = true;
}

template<> inline
void Argument_<Path>::fromString(const std::string &value)
{
    mValue = value;
    bValid = true;
}

template<typename T> inline
auto Argument_<T>::value() const -> T
{
    return mValue;
}

template<typename T> inline
void Argument_<T>::setValue(const T &value)
{
    mValue = value;
    bValid = true;
}

template<typename T> inline
auto Argument_<T>::isValid() -> bool
{
    if(validator() != nullptr) {
        return std::dynamic_pointer_cast<ValidatorBase<T>>(validator())->validate(mValue);
    } else {
        return bValid;
    }
}



template<typename T, typename... Arg>
auto makeArgument(Arg&&... arg) -> std::shared_ptr<Argument_<T>>
{
    return std::make_shared<Argument_<T>>(std::forward<Arg>(arg)...);
}

/*! \} */ // end of Console

/*! \} */ // end of core


} // End namespace tl
