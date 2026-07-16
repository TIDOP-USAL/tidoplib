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
 * \file RegexValidator.h
 * \brief Validator that checks if a string matches a regular expression pattern.
 *
 * This file defines the \ref RegexValidator class, which validates a string
 * against a regular expression pattern (ECMAScript syntax). It is commonly
 * used to enforce specific formats for command‑line arguments, such as file
 * extensions, numeric patterns, or email addresses.
 *
 * \ingroup Validators
 */

#pragma once


#include "tidop/config.h"

#include <string>
#include <memory>
#include <regex>

#include "tidop/core/app/Message.h"
#include "tidop/core/console/Validator.h"
#include "tidop/core/base/Format.h"

namespace tl
{

/*! \addtogroup Validators
 *  \{
 */

/*!
 * \brief Validator that checks if a string matches a regular expression.
 *
 * This validator uses `std::regex_match` to determine if the input string
 * conforms to the specified pattern. The pattern is compiled at construction
 * time, and any regex syntax error will throw a `std::regex_error`.
 *
 * The validator stores a human‑readable description of the expected format,
 * which is used in error messages and in the \ref toString() output.
 */
class RegexValidator final
  : public ValidatorBase<std::string>
{
private:

    std::string mPatternStr;
    std::regex mPattern;
    std::string mDescription;

public:

    /*!
     * \brief Constructs a RegexValidator.
     * \param[in] pattern The regular expression pattern (ECMAScript syntax).
     * \param[in] formatDescription A short description of the expected format
     *                          (e.g., "positive integer", "email address").
     * \throws std::regex_error if the pattern is invalid.
     */
    RegexValidator(std::string pattern, std::string formatDescription)
      : mPatternStr(pattern), 
        mPattern(pattern), 
        mDescription(std::move(formatDescription))
    {
    }

    /*!
     * \brief Validates the given string against the regex pattern.
     * \param[in] value The string to check.
     * \return `true` if the string matches the pattern; otherwise `false`
     *         and an error message is logged.
     */
    auto validate(std::string value) -> bool override
    {
        if (!std::regex_match(value, mPattern)) {
            Message::error("Value '{}' does not match the required format: {}", value, mDescription);
            return false;
        }
        return true;
    }

    /*!
     * \brief Returns a textual description of the validation rule.
     * \return A string like "Format required: positive integer".
     */
    [[nodiscard]]
    auto toString() const -> std::string override
    {
        return tl::format("Format required: {}", mDescription);
    }

    /*!
     * \brief Factory method for creating a shared pointer to a RegexValidator.
     * \param[in] pattern The regular expression pattern.
     * \param[in] formatDescription The description of the expected format.
     * \return std::shared_ptr<RegexValidator>
     */
    static auto create(std::string pattern, std::string formatDescription) -> std::shared_ptr<RegexValidator>
    {
        return std::make_shared<RegexValidator>(pattern, formatDescription);
    }

};


/*! \} */


} // End namespace tl
