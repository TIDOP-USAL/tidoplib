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

template <typename T> class Argument_;


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
        arg_int64,
        arg_uint64,
        arg_float32,
        arg_float64,
        arg_string,
        arg_path,
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

private:

    std::string mName;
    std::string mDescription;
    char mShortName;
    Type mType;
    std::shared_ptr<Validator> mValidator;

public:

    /*!
     * \brief Constructor
     * \param[in] name Name of the argument
     * \param[in] description Description of the argument
     * \param[in] type Type of the argument
     * \see Type
     */
    Argument(std::string name, std::string description, Type type);

    /*!
     * \brief Constructor
     * \param[in] shortName Short name of the argument
     * \param[in] description Description of the argument
     * \param[in] type Type of the argument
     */
    Argument(const char &shortName, std::string description, Type type);

    /*!
     * \brief Constructor
     * \param[in] name Name of the argument
     * \param[in] shortName Short name of the argument
     * \param[in] description Description of the argument
     * \param[in] type Type of the argument
     */
    Argument(std::string name, const char &shortName, std::string description, Type type);

    /*!
     * \brief Copy constructor
     */
    Argument(const Argument &argument);

    /*!
     * \brief Move constructor
     */
    Argument(Argument &&argument) TL_NOEXCEPT;

    virtual ~Argument() = default;

    /*!
     * \brief Assignment operator
     */
    auto operator = (const Argument &argument) -> Argument &;

    /*!
     * \brief Move assignment operator
     */
    auto operator = (Argument &&argument) TL_NOEXCEPT -> Argument &;

    /*!
     * \brief Returns the name of the argument
     * \return Name of the argument
     */
    auto name() const -> std::string;

    /*!
     * \brief Sets the name of the argument
     * \param[in] name Name of the argument
     */
    void setName(const std::string &name);

    /*!
     * \brief Returns the description of the argument
     * \return Description of the argument
     */
    virtual auto description() const -> std::string;

    /*!
     * \brief Sets the description of the argument
     * \param[in] description Description of the argument
     */
    void setDescription(const std::string &description);

    /*!
     * \brief Returns the short name of the argument
     * \return Short name of the argument
     */
    auto shortName() const -> char;

    /*!
     * \brief Sets the short name of the argument
     * \param[in] shortName Short name of the argument
     */
    void setShortName(const char &shortName);

    /*!
     * \brief Returns the type of the argument
     * \return Type of the argument
     * \see Type 
     */
    auto type() const -> Type;

    /*!
     * \brief Returns the validator of the argument
     * \see Validator 
     */
    auto validator() const -> std::shared_ptr<Validator>;

    /*!
     * \brief Sets the validator of the argument
     * \param[in] validator Validator
     */
    void setValidator(const std::shared_ptr<Validator> &validator);

    /*!
     * \brief Returns a string of text with the type of the argument
     * \return
     */
    virtual auto typeName() const -> std::string = 0;

    /*!
     * \brief Check if the argument is mandatory
     * \return true if mandatory
     */
    virtual auto isRequired() const -> bool = 0;

    /*!
     * \brief Sets the value of the argument from a text string
     * \param[in] value Argument value as a text string
     */
    virtual void fromString(const std::string &value) = 0;

    /*!
     * \brief Checks if the value passed to the argument is valid
     * \return
     */
    virtual bool isValid() = 0;

    /*!
     * \brief Construct an argument
     * 
     * Example of use:
     * \code
     * auto input = Argument::make<std::string>("input", "Input data");
     * auto option = Argument::make<std::string>("option", 'o', "Option 1", false);
     * \endcode
     * 
     * \return
     */
    template<typename T, typename... Arg>
    static auto make(Arg&&... arg) -> std::shared_ptr<Argument_<T>>
    {
        return std::make_shared<Argument_<T>>(std::forward<Arg>(arg)...);
    }

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
struct ArgTraits<long long>
{
    using value_type = long long;
    static constexpr auto property_type = Argument::Type::arg_int64;
    static constexpr auto type_name = "int64";
};

template<>
struct ArgTraits<unsigned long long>
{
    using value_type = unsigned long;
    static constexpr auto property_type = Argument::Type::arg_uint64;
    static constexpr auto type_name = "uint64";
};

template<>
struct ArgTraits<std::string>
{
    using value_type = std::string;
    static constexpr auto property_type = Argument::Type::arg_string;
    static constexpr auto type_name = "std::string";
};

template<>
struct ArgTraits<tl::Path>
{
    using value_type = tl::Path;
    static constexpr auto property_type = Argument::Type::arg_path;
    static constexpr auto type_name = "tl::Path";
};


/* ---------------------------------------------------------------------------------- */




/*!
 * \brief Template class to manage different types of arguments
 */
template <typename T>
class Argument_
    : public Argument
{

public:

    /*!
     * \brief Constructor
     * \param[in] name Name of the argument
     * \param[in] description Description of the argument
     */
    Argument_(const std::string &name,
              const std::string &description);

    /*!
     * \brief Constructor
     * \param[in] name Name of the argument
     * \param[in] description Description of the argument
     * \param[in] value Value of the argument
     */
    Argument_(const std::string &name,
              const std::string &description,
              T value);

    /*!
     * \brief Constructor
     * \param[in] shortName Short name of the argument
     * \param[in] description Description of the argument
     */
    Argument_(const char &shortName,
              const std::string &description);

    /*!
     * \brief Constructor
     * \param[in] shortName Short name of the argument
     * \param[in] description Description of the argument
     * \param[in] value Value of the argument
     */
    Argument_(const char &shortName,
              const std::string &description,
              T value);

    /*!
     * \brief Constructor
     * \param[in] name Name of the argument
     * \param[in] shortName Short name of the argument
     * \param[in] description Description of the argument
     */
    Argument_(const std::string &name,
              const char &shortName,
              const std::string &description);

    /*!
     * \brief Constructor
     * \param[in] name Name of the argument
     * \param[in] shortName Short name of the argument
     * \param[in] description Description of the argument
     * \param[in] value Value of the argument
     */
    Argument_(const std::string &name,
              const char &shortName,
              const std::string &description,
              T value);

    ~Argument_() override = default;

    TL_DISABLE_COPY(Argument_)
    TL_DISABLE_MOVE(Argument_)

public:

    /*!
     * \brief Sets the value of the argument
     * \param[in] value Value of the argument
     */
    virtual void setValue(const T &value);

// Argument interface

public:

    auto typeName() const -> std::string override;
    auto isRequired() const -> bool override;
    void fromString(const std::string &value) override;
    auto value() const -> T;
    auto isValid() -> bool override;

private:

    T mValue;
    bool optional;
    bool bValid;
};


/* Definition of aliases for the most frequent types */

using ArgumentInteger = Argument_<int>;
using ArgumentDouble = Argument_<double>;
using ArgumentFloat = Argument_<float>;
using ArgumentBoolean = Argument_<bool>;
using ArgumentString = Argument_<std::string>;
using ArgumentCharRequired = Argument_<char>;



/* Argument_ Implementation */

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
    mValue = Path::fromLocal8Bit(value);
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
    }

    return bValid;
}



/*! \} */ // end of Console

/*! \} */ // end of core


} // End namespace tl
