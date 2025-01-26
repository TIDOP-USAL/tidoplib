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
#include <memory>

#include "tidop/core/base/defs.h"
#include "tidop/core/base/path.h"
#include "tidop/core/base/type.h"
#include "tidop/core/base/type_conversions.h"
#include "tidop/core/console/validator.h"
#include "tidop/core/base/exception.h"

namespace tl
{

/*! \addtogroup Console
 *  \{
 */

template <typename T> class Argument_;


/*!
 * \class Argument
 * \brief Represents a command-line argument with associated metadata and validation.
 *
 * The `Argument` class is an abstraction for command-line arguments. It allows
 * you to define arguments with a name, short name, description, type, and an optional validator.
 * This class supports both mandatory and optional arguments, with the ability to validate
 * and parse their values from strings.
 *
 * ### Features
 * - **Metadata**: Name, short name, description, and type for each argument.
 * - **Validation**: Validate argument values using custom validators.
 * - **Dynamic Typing**: Supports various argument types such as integers, floating-point numbers, strings, and more.
 * - **Extensibility**: Derived classes can implement specific behavior, including mandatory checks, type names, and value parsing.
 *
 * ### %Argument Types
 * The `Type` enum defines the supported types for arguments:
 * - Boolean (`type_bool`)
 * - Integer types (`type_int8`, `type_uint8`, etc.)
 * - Floating-point types (`type_float32`, `type_float64`)
 * - String types (`type_string`, `type_path`)
 *
 * ### Example Usage
 * \code{.cpp}
 * auto input = Argument::make<std::string>("input", "Path to the input file");
 * auto option = Argument::make<int>("option", 'o', "Optional parameter", false);
 * \endcode
 *
 * ### Notes
 * This class is designed to be subclassed to provide specific argument behaviors. Subclasses
 * must implement the following pure virtual methods:
 * - `typeName()`: Returns a string representation of the argument's type.
 * - `isRequired()`: Indicates if the argument is mandatory.
 * - `fromString()`: Parses the argument's value from a string.
 * - `isValid()`: Validates the argument's value.
 *
 * ### See Also
 * - `Validator`: For defining custom validation logic.
 * - `Argument_<T>`: Template subclass for typed arguments.
 */
class TL_EXPORT Argument
{

public:

    using SharedPtr = std::shared_ptr<Argument>;

private:

    std::string mName;
    std::string mDescription;
    char mShortName;
    Type mType;
    std::shared_ptr<Validator> mValidator;

public:

    /*!
     * \brief Constructs an Argument with a name, description, and type.
     * \param[in] name Name of the argument.
     * \param[in] description Description of the argument, explaining its purpose.
     * \param[in] type Type of the argument, as defined in \ref Argument::Type.
     * \see Argument::Type
     */
    Argument(std::string name, std::string description, Type type);

    /*!
     * \brief Constructs an Argument with a short name, description, and type.
     * \param[in] shortName Short, single-character name for the argument.
     * \param[in] description Description of the argument, explaining its purpose.
     * \param[in] type Type of the argument, as defined in \ref Argument::Type.
     *
     * This constructor is useful for command-line arguments that are commonly referred
     * to using a single character, such as `-o`.
     */
    Argument(const char &shortName, std::string description, Type type);

    /*!
     * \brief Constructs an Argument with a name, short name, description, and type.
     * \param[in] name Name of the argument.
     * \param[in] shortName Short, single-character name for the argument.
     * \param[in] description Description of the argument, explaining its purpose.
     * \param[in] type Type of the argument, as defined in \ref Argument::Type.
     *
     * This constructor allows defining both a full name and a shorthand for the argument.
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
     * \brief Creates a new argument instance.
     *
     * This static method allows the construction of an argument with the specified
     * type and parameters. It supports both required and optional arguments.
     *
     * - **Required arguments**: These do not have a default value and must always
     *   be provided by the user.
     * - **Optional arguments**: These include a default value, making them optional
     *   to specify during usage.
     *
     * \tparam T The type of the argument (e.g., `std::string`, `int`, `bool`).
     * \tparam Arg The variadic template for additional argument parameters.
     * \param[in] arg Parameters to initialize the argument (e.g., name, description,
     * short name, and default value for optional arguments).
     *
     * \return A shared pointer to the newly created `Argument_<T>` instance.
     *
     * Example usage:
     * \code{.cpp}
     * // Required argument (must be provided by the user)
     * auto input = Argument::make<std::string>("input", "Input data");
     *
     * // Optional argument (default value provided, user can override)
     * auto option = Argument::make<std::string>("option", 'o', "Option 1", "default_value");
     * \endcode
     *
     * In the first example, `input` is a required argument because no default value is provided.
     * In the second example, `option` is an optional argument because it has a default value (`"default_value"`).
     */
    template<typename T, typename... Arg>
    static auto make(Arg&&... arg) -> std::shared_ptr<Argument_<T>>
    {
        return std::make_shared<Argument_<T>>(std::forward<Arg>(arg)...);
    }

};




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
  : Argument(name, description, TypeTraits<T>::id_type),
    mValue(T()),
    optional(false),
    bValid(true)
{
}

template<typename T> inline
Argument_<T>::Argument_(const std::string &name,
                        const std::string &description,
                        T value)
  : Argument(name, description, TypeTraits<T>::id_type),
    mValue(value),
    optional(true),
    bValid(true)
{
}

template<typename T> inline
Argument_<T>::Argument_(const char &shortName,
                        const std::string &description)
  : Argument(shortName, description, TypeTraits<T>::id_type),
    optional(false),
    bValid(true)
{
}

template<typename T> inline
Argument_<T>::Argument_(const char &shortName,
                        const std::string &description,
                        T value)
  : Argument(shortName, description, TypeTraits<T>::id_type),
    mValue(value),
    optional(true),
    bValid(true)
{
}

template<typename T> inline
Argument_<T>::Argument_(const std::string &name,
                        const char &shortName,
                        const std::string &description)
  : Argument(name, shortName, description, TypeTraits<T>::id_type),
    optional(false),
    bValid(true)
{
}

template<typename T> inline
Argument_<T>::Argument_(const std::string &name,
                        const char &shortName,
                        const std::string &description,
                        T value)
  : Argument(name, shortName, description, TypeTraits<T>::id_type),
    mValue(value),
    optional(true),
    bValid(true)
{
}

template<typename T> inline
auto Argument_<T>::typeName() const -> std::string
{
    return TypeTraits<T>::name_type;
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
    //mValue = Path::fromLocal8Bit(value);
    mValue = Path(value);
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



/*! \} */



/// \cond

namespace internal
{


template<typename T>
class ArgValue
{

public:

    ArgValue(/* args */) {}

    T value(const Argument::SharedPtr &arg);
};

template<typename T>
auto ArgValue<T>::value(const Argument::SharedPtr &arg) -> T
{
    T value{};

    try {

        TL_ASSERT(arg, "Argument pointer is null");

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
            TL_THROW_EXCEPTION("Unknown argument type");
        case Type::type_bool:
            value = numberCast<T>(std::dynamic_pointer_cast<Argument_<bool>>(arg)->value());
            break;
        case Type::type_int8:
            value = numberCast<T>(std::dynamic_pointer_cast<Argument_<char>>(arg)->value());
            break;
        case Type::type_uint8:
            value = numberCast<T>(std::dynamic_pointer_cast<Argument_<unsigned char>>(arg)->value());
            break;
        case Type::type_int16:
            value = numberCast<T>(std::dynamic_pointer_cast<Argument_<short>>(arg)->value());
            break;
        case Type::type_uint16:
            value = numberCast<T>(std::dynamic_pointer_cast<Argument_<unsigned short>>(arg)->value());
            break;
        case Type::type_int32:
            value = numberCast<T>(std::dynamic_pointer_cast<Argument_<int>>(arg)->value());
            break;
        case Type::type_uint32:
            value = numberCast<T>(std::dynamic_pointer_cast<Argument_<unsigned int>>(arg)->value());
            break;
        case Type::type_float32:
            value = numberCast<T>(std::dynamic_pointer_cast<Argument_<float>>(arg)->value());
            break;
        case Type::type_float64:
            value = numberCast<T>(std::dynamic_pointer_cast<Argument_<double>>(arg)->value());
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
inline auto ArgValue<std::string>::value(const Argument::SharedPtr &arg) -> std::string
{
    std::string value;

    try {

        TL_ASSERT(arg, "Argument pointer is null");

        const auto type = arg->type();

        switch (type) {
        case Type::type_unknown:
            TL_THROW_EXCEPTION("Unknown argument type");
        case Type::type_string:
            value = std::dynamic_pointer_cast<Argument_<std::string>>(arg)->value();
            break;
        case Type::type_path:
            value = std::dynamic_pointer_cast<Argument_<tl::Path>>(arg)->value().toString();
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
inline auto ArgValue<tl::Path>::value(const Argument::SharedPtr &arg) -> tl::Path
{
    tl::Path value;

    try {

        TL_ASSERT(arg, "Argument pointer is null");

        const auto type = arg->type();

        switch (type) {
        case Type::type_unknown:
            TL_THROW_EXCEPTION("Unknown argument type");
        case Type::type_string:
            value = tl::Path(std::dynamic_pointer_cast<Argument_<std::string>>(arg)->value());
            break;
        case Type::type_path:
            value = std::dynamic_pointer_cast<Argument_<tl::Path>>(arg)->value();
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


} // End namespace tl
