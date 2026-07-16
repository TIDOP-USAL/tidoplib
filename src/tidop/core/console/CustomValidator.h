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
 * \file CustomValidator.h
 * \brief Provides a generic validator based on a user‑supplied predicate.
 * 
 * This file defines the \ref CustomValidator class, which allows creating
 * custom validation logic for any type `T` by providing a predicate function
 * and a descriptive message. It is particularly useful for ad‑hoc validation
 * rules that do not fit into predefined validators like range or values.
 *
 * \ingroup Validators
 */

#pragma once


#include "tidop/config.h"

#include <memory>
#include <functional>

#include "tidop/core/app/Message.h"
#include "tidop/core/console/Validator.h"
#include "tidop/core/base/Format.h"

namespace tl
{

/*! \addtogroup Validators
 *  \{
 */

/*!
 * \brief Generic validator that applies a custom predicate to the value.
 *
 * This validator evaluates a given value using a user‑provided predicate
 * (`std::function<bool(T)>`). If the predicate returns `false`, an error
 * message is emitted via \ref Message::error and the validation fails.
 *
 * The description string is used in the error message and in the
 * \ref toString() output, making it easy to identify the validation rule
 * in help or log messages.
 *
 * \tparam T The type of the value to validate (e.g., `int`, `double`,
 *           `std::string`, `tl::Path`).
 *
 * \ingroup Validators
 */
template <typename T>
class CustomValidator final
  : public ValidatorBase<T>
{

public:

    /*!
     * \brief Predicate type: a callable that takes a value of type `T`
     *        and returns `true` if the value is valid.
     */
    using Predicate = std::function<bool(T)>;

private:

    Predicate mPredicate;
    std::string mDescription;

public:

    /*!
     * \brief Constructs a custom validator.
     * \param[in] predicate The validation predicate. It should return `true`
     *                  when the value is valid, `false` otherwise.
     * \param[in] description A short description of the validation rule
     *                    (e.g., "must be positive", "even number").
     */
    CustomValidator(Predicate predicate, std::string description)
      : mPredicate(std::move(predicate)), 
        mDescription(std::move(description))
    {
    }

    /*!
     * \brief Validates the given value using the stored predicate.
     * \param[in] value The value to be checked.
     * \return `true` if the predicate returns `true`; otherwise, `false`
     *         and an error message is logged.
     */
    auto validate(T value) -> bool override
    {
        if (!mPredicate(value)) {
            Message::error("Value '{}' failed custom validation: {}", value, mDescription);
            return false;
        }

        return true;
    }

    /*!
     * \brief Returns a textual description of the validation rule.
     * \return A string in the format "Custom rule: <description>".
     */
    [[nodiscard]]
    auto toString() const -> std::string override
    {
        return tl::format("Custom rule: {}", mDescription);
    }

    /*!
     * \brief Factory method to create a shared pointer to a CustomValidator.
     * \param[in] predicate The validation predicate.
     * \param[in] description The description of the rule.
     * \return std::shared_ptr<CustomValidator<T>>
     */
    static auto create(Predicate predicate, std::string description) -> std::shared_ptr<CustomValidator<T>>
    {
        return std::make_shared<CustomValidator<T>>(std::move(predicate), std::move(description));
    }

};


/*! \} */


} // End namespace tl
