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
#include <iostream>

#include "tidop/core/base/defs.h"
#include "tidop/core/app/message.h"


namespace tl
{

/*! \addtogroup Console
 *  \{
 */

/*! 
 * \brief Abstract base class for validators.
 * 
 * This class defines the interface for validator classes. It ensures that any subclass 
 * implements the `print` function to display validation results.
 */
class TL_EXPORT Validator
{

public:

    Validator() = default;
    virtual ~Validator() = default;

    /*!
     * \brief Prints the validation result.
     *
     * This function must be implemented by derived classes to provide custom
     * validation output.
     */
    virtual void print() const = 0;
};

/*!
 * \brief Template class for validating specific data types.
 *
 * This class extends `Validator` and implements validation logic for a specific data type `T`.
 * Derived classes must implement the `validate` function, which defines the validation logic.
 *
 * \tparam T The type of value to validate.
 */
template <typename T>
class ValidatorBase
  : public Validator
{
public:

    ValidatorBase() = default;
    ~ValidatorBase() override = default;

    /*!
     * \brief Validates a given value.
     *
     * Derived classes must implement this function to provide validation logic
     * for the given value of type `T`.
     *
     * \param value The value to validate.
     * \return `true` if the value is valid, `false` otherwise.
     */
    virtual bool validate(T value) = 0;
};



/*!
 * \class RangeValidator
 * \brief Validator to ensure a value is within a specified range.
 *
 * This class is used internally in Command::parse() to check if a parsed argument
 * is within the valid range. It supports any arithmetic type (e.g., int, float).
 * 
 * \tparam T The type of the value to be validated. Must be an arithmetic type.
 *
 * Example:
 * \code
 * auto validator = RangeValidator<int>::create(0, 100);
 * if (validator->validate(50)) {
 *     std::cout << "Value is valid!" << std::endl;
 * } else {
 *     std::cout << "Value is out of range!" << std::endl;
 * }
 * \endcode
 */
template <typename T>
class RangeValidator final
  : public ValidatorBase<T>
{

public:

    /*!
     * \brief Default constructor.
     *
     * Initializes the range to the lowest and highest possible values for the type T.
     *
     * \pre T must be an arithmetic type.
     */
    RangeValidator()
      : mMin(std::numeric_limits<T>::lowest()),
        mMax(std::numeric_limits<T>().max())
    {
        static_assert(std::is_arithmetic<T>::value, "Only arithmetic types are allowed");
    }

    /*!
     * \brief Constructs a RangeValidator with a specified range.
     *
     * \param[in] min Minimum value of the range.
     * \param[in] max Maximum value of the range.
     *
     * \pre T must be an arithmetic type.
     */
    RangeValidator(T min, T max)
      : mMin(min),
        mMax(max)
    {
        static_assert(std::is_arithmetic<T>::value, "Only arithmetic types are allowed");
    }

    /*!
     * \brief Validates if a value is within the range.
     *
     * \param[in] value The value to validate.
     * \return True if the value is within the range, otherwise false.
     *
     * \post If the value is invalid, an error message is printed.
     */
    auto validate(T value) -> bool override
    {
        bool valid = (value >= mMin && value <= mMax);
        
        if(!valid) {
            Message::error("The value '{}' is out of valid range [{}-{}]", value, mMin, mMax);
        }

        return valid;
    }

    /*!
     * \brief Sets the range for validation.
     *
     * \param[in] min Minimum value of the range.
     * \param[in] max Maximum value of the range.
     */
    void setRange(T min, T max)
    {
        mMin = min;
        mMax = max;
    }

    /*!
     * \brief Retrieves the minimum value of the range.
     * \return The minimum value.
     */
    auto min() const -> T
    {
        return mMin;
    }

    /*!
     * \brief Retrieves the maximum value of the range.
     * \return The maximum value.
     */
    auto max() const -> T
    {
        return mMax;
    }

    /*!
     * \brief Prints the range to the console.
     *
     * Displays the range in the format `[min - max]`.
     */
    void print() const override
    {
        std::cout << "Valid range [" << mMin << " - " << mMax << "]";
    }

    /*!
     * \brief Creates a shared instance of RangeValidator.
     *
     * \param[in] min Minimum value of the range.
     * \param[in] max Maximum value of the range.
     * \return A shared pointer to the created RangeValidator instance.
     */
    static auto create(T min, T max) -> std::shared_ptr<RangeValidator<T>>
    {
        return std::make_shared<RangeValidator<T>>(min, max);
    }

private:

    T mMin;
    T mMax;

};



/*!
 * \class ValuesValidator
 * \brief Validator to ensure a value is among a predefined set of valid values.
 *
 * This class is used internally in Command::parse() to verify that a parsed argument
 * matches one of the specified valid values. It supports any type that can be compared using `operator==`.
 * 
 * \tparam T The type of the value to be validated.
 *
 * Example:
 * \code
 * // Create a validator for specific string values
 * auto validator = ValuesValidator<std::string>::create({"XML", "YML", "TXT", "BIN"});
 * if (validator->validate("XML")) {
 *     std::cout << "Value is valid!" << std::endl;
 * } else {
 *     std::cout << "Value is invalid!" << std::endl;
 * }
 * \endcode
 */
template <typename T>
class ValuesValidator final
  : public ValidatorBase<T>
{

public:

    /*!
     * \brief Default constructor.
     *
     * Creates a `ValuesValidator` with no predefined valid values.
     */
    ValuesValidator()
    {
    }

    /*!
     * \brief Constructs a `ValuesValidator` with a specified set of valid values.
     *
     * \param[in] values A vector containing the valid values.
     */
    ValuesValidator(std::vector<T> values)
      : values(values)
    {
    }

    /*!
     * \brief Validates if a value is in the list of valid values.
     *
     * \param[in] value The value to validate.
     * \return True if the value is in the list of valid values, otherwise false.
     *
     * \post If the value is invalid, an error message is printed, showing the allowed values.
     */
    auto validate(T value) -> bool override
    {
        bool valid = false;

        for(auto &_value : values) {
            if(value == _value) {
                valid = true;
                break;
            }
        }

        if(!valid) {
            std::ostringstream stream;
            stream << "Invalid value: '" << value << "'. Valid values are: ";
            for (const auto &values : values)
                stream << values << " ";
            
            Message::error(stream.str());
        }

        return valid;
    }

    /*!
     * \brief Sets the valid values.
     *
     * \param[in] values A vector containing the valid values.
     */
    void setValues(const std::vector<T> &values)
    {
        this->values = values;
    }

    /*!
     * \brief Prints the valid values to the console.
     *
     * Displays the allowable values in the format `[ value1 value2 ... valueN ]`.
     */
    void print() const override
    {
        std::cout << "Allowable values: [ ";
        for (const auto &values : this->values)
            std::cout << values << " ";
        std::cout << "]";
    }

    /*!
     * \brief Creates a shared instance of `ValuesValidator`.
     *
     * \param[in] values A vector containing the valid values.
     * \return A shared pointer to the created `ValuesValidator` instance.
     */
    static auto create(std::vector<T> values) -> std::shared_ptr<ValuesValidator<T>>
    {
        return std::make_shared<ValuesValidator<T>>(values);
    }

private:

    std::vector<T> values;

};


/*! \} */


} // End namespace tl
