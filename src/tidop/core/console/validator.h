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

#include "tidop/core/defs.h"
#include "tidop/core/utils.h"
#include "tidop/core/path.h"
#include "tidop/core/msg/message.h"


namespace tl
{

/*! \addtogroup core
 *  \{
 */

/*! \addtogroup Console
 *  \{
 */

class Validator
{

public:

    Validator() = default;
    virtual ~Validator() = default;

    virtual void print() const = 0;
};

template <typename T>
class ValidatorBase
  : public Validator
{
public:

    ValidatorBase() = default;
    virtual ~ValidatorBase() = default;

    virtual bool validate(T value) = 0;
};


/*!
 * \brief Validator to check if the value passed to an argument is within the expected range.
 * 
 * This class is used internally in Command::parse() to check if the parsed argument is valid.
 *
 * <h4>Example:</h4>
 * \code
 * // Set a validator to an argument
 * arg_format->setValidator(RangeValidator<std::string>::create(0, 100));
 * \endcode
 */
template <typename T>
class RangeValidator
    : public ValidatorBase<T>
{

public:

    RangeValidator()
      : mMin(std::numeric_limits<T>::lowest()),
        mMax(std::numeric_limits<T>().max())
    {
        static_assert(std::is_arithmetic<T>::value, "Only arithmetic types are allowed");
    }

    /*!
     * \brief constructor
     * \param[in] min Minimum value of the range
     * \param[in] max Maximum value of the range
     */
    RangeValidator(T min, T max)
      : mMin(min),
        mMax(max)
    {
        static_assert(std::is_arithmetic<T>::value, "Only arithmetic types are allowed");
    }

    /*!
     * \brief Check if the value is valid
     * \param[in] value Value to check
     * \return True if the passed value is within the range
     */
    bool validate(T value) override
    {
        bool valid = (value >= mMin && value <= mMax);
        
        if(!valid) {
            Message::error("The value '{}' is out of valid range [{}-{}]", value, mMin, mMax);
        }

        return valid;
    }

    /*!
     * \brief Minimum value of the range
     * \param[in] min Minimum value of the range
     * \param[in] max Maximum value of the range
     */
    void setRange(T min, T max)
    {
        mMin = min;
        mMax = max;
    }

    /*!
     * \brief Minimum value of the range
     */
    T min() const
    {
        return mMin;
    }

    /*!
     * \brief Maximum value of the range
     */
    T max() const
    {
        return mMax;
    }

    void print() const
    {
        std::cout << "Valid range [" << mMin << " - " << mMax << "]";
    }

    static auto create(T min, T max) -> std::shared_ptr<RangeValidator<T>>
    {
        return std::make_shared<RangeValidator<T>>(min, max);
    }

private:

    T mMin;
    T mMax;

};


/*!
 * \brief Validator to check if the value passed to an argument is within the expected values.
 * 
 * This class is used internally in Command::parse() to check if the parsed argument is valid.
 * <h4>Example:</h4>
 * \code
 * // Set a validator to an argument
 * arg_format->setValidator(ValuesValidator<std::string>::({"XML", "YML", "TXT", "BIN"}));
 * \endcode
 */
template <typename T>
class ValuesValidator
    : public ValidatorBase<T>
{

public:

    ValuesValidator()
    {
    }

    /*!
     * \brief Constructor
     * \param[in] values Valid values
     */
    ValuesValidator(std::vector<T> values)
      : values(values)
    {
    }

    /*!
     * \brief Check if the value is valid
     * \param[in] value Value to check
     * \return True if the passed value is within the expected values
     */
    bool validate(T value) override
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
     * \brief Set valid values
     * \param[in] values Valid values
     */
    void setValues(const std::vector<T> &values)
    {
        this->values = values;
    }

    void print() const
    {
        std::cout << "Allowable values: [ ";
        for (const auto &values : this->values)
            std::cout << values << " ";
        std::cout << "]";
    }

    static auto create(std::vector<T> values) -> std::shared_ptr<ValuesValidator<T>>
    {
        return std::make_shared<ValuesValidator<T>>(values);
    }

private:

    std::vector<T> values;

};


/*! \} */ // end of Console

/*! \} */ // end of core


} // End namespace tl
