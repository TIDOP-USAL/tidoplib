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
 * \file ValuesValidator.h
 * \brief Validator that checks if a value belongs to a predefined set.
 *
 * This file defines the \ref ValuesValidator class, which ensures that a value
 * matches one of the allowed values in a list. It is commonly used to validate
 * command‑line arguments that accept only a fixed set of options (e.g., format
 * names, mode flags).
 *
 * \ingroup Validators
 */

#pragma once

#include "tidop/config.h"

#include <vector>
#include <memory>
#include <algorithm>

#include "tidop/core/app/Message.h"
#include "tidop/core/console/Validator.h"
#include "tidop/core/base/Format.h"

namespace tl
{

/*! \addtogroup Validators
 *  \{
 */

/*!
 * \brief Validator that verifies a value is among a set of allowed values.
 *
 * This validator stores a vector of allowed values of type `T`. The validation
 * succeeds if the input value is equal (using `operator==`) to at least one
 * element in the set.
 *
 * The allowed values can be set at construction time, via the constructor, or
 * later using \ref setValues(). If validation fails, an error message is emitted
 * via \ref Message::error, listing all allowed values.
 *
 * \tparam T The type of the value to validate. Must support `operator==`.
 *
 */
template <typename T>
class ValuesValidator final
  : public ValidatorBase<T>
{

public:

    /*!
     * \brief Default constructor.
     *
     * Creates an empty validator with no allowed values. Any validation will
     * fail unless values are set via \ref setValues() before calling \ref validate().
     */
    ValuesValidator()
    {
    }

    /*!
     * \brief Constructor with an initial list of allowed values.
     * \param[in] values A vector containing the valid values.
     */
    ValuesValidator(std::vector<T> values)
      : mValues(values)
    {
    }

    /*!
     * \brief Validates the given value against the allowed set.
     * \param[in] value The value to be checked.
     * \return `true` if the value is in the allowed list; otherwise `false`
     *         and an error message is printed.
     */
    auto validate(T value) -> bool override
    {
        bool valid = std::ranges::any_of(mValues, [value](const auto &v) {
            return v == value;
            });

        if(!valid) {
            std::string valid_str;
            for (size_t i = 0; i < mValues.size(); ++i) {
                valid_str += tl::format("{}{}", mValues[i], (i + 1 < mValues.size() ? ", " : ""));
            }
            Message::error("Invalid value: '{}'. Valid values are: [ {} ]", value, valid_str);
        }

        return valid;
    }

    /*!
     * \brief Replaces the set of allowed values.
     * \param[in] values A vector containing the new valid values.
     */
    void setValues(const std::vector<T> &values)
    {
        this->mValues = values;
    }

    /*!
     * \brief Returns a string description of the validation rule.
     * \return A string like "Allowed values: [ XML, YML, TXT, BIN ]".
     */
    [[nodiscard]]
    auto toString() const -> std::string override
    {
        std::string valid_str;
        for (size_t i = 0; i < mValues.size(); ++i) {
            valid_str += tl::format("{}{}", mValues[i], (i + 1 < mValues.size() ? ", " : ""));
        }

        return tl::format("Allowed values: [ {} ]", valid_str);
    }

    /*!
     * \brief Factory method for creating a shared pointer to a ValuesValidator.
     * \param[in] values A vector containing the allowed values.
     * \return std::shared_ptr<ValuesValidator<T>>
     */
    static auto create(std::vector<T> values) -> std::shared_ptr<ValuesValidator<T>>
    {
        return std::make_shared<ValuesValidator<T>>(values);
    }

private:

    std::vector<T> mValues;

};


/*! \} */


} // End namespace tl
