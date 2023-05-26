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

    RangeValidator(T min, T max)
      : mMin(min),
        mMax(max)
    {
        static_assert(std::is_arithmetic<T>::value, "Only arithmetic types are allowed");
    }

    bool validate(T value) override
    {
        bool valid = (value > mMin && value < mMax);
        
        if(!valid) {
            std::string msg = "The value '";
            msg.append(std::to_string(value)).append("' is out of valid range [");
            msg.append(std::to_string(mMin)).append("-").append(std::to_string(mMin)).append("]");
            msgError(msg.c_str());
        }

        return valid;
    }

    void setRange(T min, T max)
    {
        mMin = min;
        mMax = max;
    }

    T min() const
    {
        return mMin;
    }

    T max() const
    {
        return mMax;
    }

    void print() const
    {
        std::cout << "Valid range [" << mMin << " - " << mMax << "]";
    }

private:

    T mMin;
    T mMax;

};


template <typename T>
class ValuesValidator
    : public ValidatorBase<T>
{

public:

    ValuesValidator()
    {
    }

    ValuesValidator(std::vector<T> values)
      : mValues(values)
    {
    }

    bool validate(T value) override
    {
        bool valid = false;

        for(auto &_value : mValues) {
            if(value == _value) {
                valid = true;
                break;
            }
        }

        if(!valid) {
            std::ostringstream stream;
            stream << "Invalid value: '" << value << "'. Valid values are: ";
            for (const auto &values : mValues)
                stream << values << " ";
            std::string str = stream.str();
            msgError(str.c_str());
        }

        return valid;
    }

    void setVallues(const std::vector<T> &values)
    {
        mValues = values;
    }

    void print() const
    {
        std::cout << "Allowable values: [ ";
        for (const auto &values : mValues)
            std::cout << values << " ";
        std::cout << "]";
    }

private:

    std::vector<T> mValues;

};


/*! \} */ // end of Console

/*! \} */ // end of core


} // End namespace tl
