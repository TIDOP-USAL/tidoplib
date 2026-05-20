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
 * \file argument.h
 * \brief Command-line argument representation and management
 *
 * This module provides a type-safe system for defining and managing command-line arguments.
 * It supports metadata (name, short name, description), type information, validation, and
 * automatic type conversion from string input.
 *
 * ### Classes
 *
 * - \ref Argument - Abstract base class for command-line arguments
 * - \ref Argument_<T> - Template implementation for typed arguments
 *
 * ### Features
 *
 * - Type-safe argument definition with template specialization
 * - Configurable required/optional arguments with default values
 * - Custom validation through Validator interface
 * - Automatic string-to-type conversion
 * - Support for all fundamental types and Path objects
 * - Both long and short argument names
 * - Comprehensive error handling and validation
 *
 * \see tl::Argument, tl::Argument_, tl::Validator
 */
 
#pragma once


#include "tidop/config.h"

#include <string>
#include <memory>

#include "tidop/core/base/TextEncoding.h"
#include "tidop/core/base/Path.h"
#include "tidop/core/base/type.h"
#include "tidop/core/base/TypeConversions.h"
#include "tidop/core/base/Exception.h"
#include "tidop/core/base/macros/SmartPtr.h"
#include "tidop/core/console/validator.h"


namespace tl
{

/*! \addtogroup Console
 *  \{
 */

template <typename T> class Argument_;


/*!
 * \class Argument
 * \brief Abstract base class representing a command-line argument
 *
 * The `Argument` class is an abstraction for command-line arguments. It allows
 * you to define arguments with a name, short name, description, type, and an optional validator.
 * This class supports both mandatory and optional arguments, with the ability to validate
 * and parse their values from strings.
 *
 * ### Features
 *
 * - **Metadata**: Name, short name, description, and type for each argument.
 * - **Validation**: Validate argument values using custom validators.
 * - **Dynamic Typing**: Supports various argument types such as integers, floating-point numbers, strings, and more.
 * - **Extensibility**: Derived classes can implement specific behavior, including mandatory checks, type names, and value parsing.
 *
 * ### %Argument Types
 *
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
 * \see Argument_<T>, Validator
 */
class TL_EXPORT Argument
{

public:

    //using SharedPtr = std::shared_ptr<Argument>;
    GENERATE_SHARED_PTR(Argument)

private:

    std::string mName;
    std::string mDescription;
    char mShortName;
    Type mType;
    std::shared_ptr<Validator> mValidator;

public:

    /*!
     * \brief Constructs an a with name, description, and type.
     * \param[in] name Name of the argument.
     * \param[in] description Description of the argument, explaining its purpose.
     * \param[in] type Type of the argument, as defined in \ref Argument::Type.
     * \see Argument::Type
     */
    Argument(std::string name, std::string description, Type type);

    /*!
     * \brief Constructs an argument with a short name, description, and type.
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
     * \brief Copy assignment operator
     *
     * Assigns a copy of the argument's state to this instance.
     *
     * \return Reference to this object
     */
    auto operator = (const Argument &argument) -> Argument &;

    /*!
     * \brief Move assignment operator
     *
     * Moves the argument's state to this instance.
     *
     * \return Reference to this object
     */
    auto operator = (Argument &&argument) TL_NOEXCEPT -> Argument &;

    /*!
     * \brief Returns the argument name
     * \return Name of the argument
     */
    auto name() const -> std::string;

    /*!
     * \brief Sets the argument name
     * \param[in] name New name for the argument
     */
    void setName(const std::string &name);

    /*!
     * \brief Returns the argument description
     * \return Description explaining the argument's purpose
     */
    auto description() const -> std::string;

    /*!
     * \brief Sets the argument description
     * \param[in] description Description of the argument
     */
    void setDescription(const std::string &description);

    /*!
     * \brief Returns the short name (single character)
     * \return The single-character abbreviation, or '\\0' if not set
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
     *
     * \return Human-readable type name (e.g., "int", "float", "std::string")
     *
     * Must be implemented by subclasses.
     */
    virtual auto typeName() const -> std::string = 0;

    /*!
     * \brief Checks if this argument is required (mandatory)
     *
     * \return `true` if the argument must be provided by the user, `false` if optional
     *
     * Must be implemented by subclasses.
     */
    virtual auto isRequired() const -> bool = 0;

    /*!
     * \brief Parses the argument value from a string
     *
     * This method is called to set the argument's value from command-line input.
     * The string is converted to the appropriate type using type-specific logic.
     *
     * \param[in] value String representation of the argument value
     *
     * \exception May throw if the string cannot be parsed as the target type
     *
     * Must be implemented by subclasses.
     *
     * \see isValid
     */
    virtual void fromString(const std::string &value) = 0;

    /*!
     * \brief Validates the current argument value
     *
     * Checks if the argument's current value is valid according to:
     * 1. The assigned validator (if any)
     * 2. Any type-specific validation rules
     *
     * \return `true` if the value is valid, `false` otherwise
     *
     * Must be implemented by subclasses.
     *
     * \see validator, setValidator
     */
    virtual bool isValid() = 0;

    /*!
     * \brief Factory method for creating typed argument instances
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
 * \brief Template specialization of Argument for type-specific handling
 *
 * This template class provides concrete implementation of Argument for a specific type T.
 * It manages the argument's value, handles string-to-type conversion, and validation.
 *
 * \tparam T The type of value this argument represents
 *
 * ### Supported Types
 *
 * - `bool` - Boolean values
 * - Integral types: `char`, `short`, `int`, `long`, `long long`, and unsigned variants
 * - Floating-point: `float`, `double`
 * - `std::string` - Text strings
 * - `Path` - File system paths (with platform-specific encoding handling)
 *
 * ### Required vs Optional
 *
 * - **Required**: Constructors without default value parameter
 * - **Optional**: Constructors with default value parameter
 *
 * ### Example
 *
 * \code{.cpp}
 * // Required int argument
 * auto count = std::make_shared<Argument_<int>>("count", "Number of items");
 *
 * // Optional string argument with default
 * auto name = std::make_shared<Argument_<std::string>>("name", "Name", "DefaultName");
 *
 * // Using the factory method
 * auto port = Argument::make<int>("port", 'p', "Port number", 8080);
 * \endcode
 */
template <typename T>
class Argument_
    : public Argument
{

public:

    /*!
     * \brief Constructor for required argument (long name only)
     *
     * \param[in] name Full name of the argument
     * \param[in] description Description of the argument's purpose
     */
    Argument_(const std::string &name,
              const std::string &description);

    /*!
     * \brief Constructor for optional argument with default value (long name only)
     *
     * \param[in] name Full name of the argument
     * \param[in] description Description of the argument's purpose
     * \param[in] value Default value for the argument
     */
    Argument_(const std::string &name,
              const std::string &description,
              T value);

    /*!
     * \brief Constructor for required argument (short name only)
     *
     * \param[in] shortName Single-character name for the argument
     * \param[in] description Description of the argument's purpose
     */
    Argument_(const char &shortName,
              const std::string &description);

    /*!
     * \brief Constructor for optional argument with default value (short name only)
     *
     * \param[in] shortName Single-character name for the argument
     * \param[in] description Description of the argument's purpose
     * \param[in] value Default value for the argument
     */
    Argument_(const char &shortName,
              const std::string &description,
              T value);

    /*!
     * \brief Constructor for required argument (both names)
     *
     * \param[in] name Full name of the argument
     * \param[in] shortName Single-character name for the argument
     * \param[in] description Description of the argument's purpose
     */
    Argument_(const std::string &name,
              const char &shortName,
              const std::string &description);

    /*!
     * \brief Constructor for optional argument with default value (both names)
     *
     * \param[in] name Full name of the argument
     * \param[in] shortName Single-character name for the argument
     * \param[in] description Description of the argument's purpose
     * \param[in] value Default value for the argument
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

    /*!
     * \brief Gets the current value of the argument
     *
     * \return The current value
     */
    auto value() const -> T;
	
// Argument interface

public:

    auto typeName() const -> std::string override;
    auto isRequired() const -> bool override;
    void fromString(const std::string &value) override;
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
using ArgumentChar = Argument_<char>;



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
#ifdef TL_OS_WINDOWS
    // Convert from local encoding (CP_ACP) to UTF-16
    std::wstring wide = fromLocalEncoding(value);

    // Convert from UTF-16 to UTF-8
    std::string utf8 = toUtf8(wide);

    mValue = Path(utf8);
#else
    // On Unix, std::string should already be in UTF-8
    mValue = Path(value);
#endif

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

/*!
 * \brief Helper class for extracting typed values from Argument instances
 *
 * This class provides type-safe value extraction with automatic type conversion
 * and validation. It handles conversions between compatible numeric types and
 * validates the source type is compatible with the target type.
 *
 * \tparam T The target type to extract the value as
 */
template<typename T>
class ArgValue
{

public:

    ArgValue() = default;

    /*!
     * \brief Extracts a value from an Argument and converts it to type T
     *
     * \param[in] arg The argument to extract from
     * \return The value converted to type T
     *
     * \exception Exception If the argument type is incompatible with T
     */															   
    auto value(const Argument::Ptr &arg) -> T;
};

template<typename T>
inline auto ArgValue<T>::value(const Argument::Ptr &arg) -> T
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
inline auto ArgValue<std::string>::value(const Argument::Ptr &arg) -> std::string
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
inline auto ArgValue<tl::Path>::value(const Argument::Ptr &arg) -> tl::Path
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
