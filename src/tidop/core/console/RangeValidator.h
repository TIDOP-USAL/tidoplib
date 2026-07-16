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
 * \file RangeValidator.h
 * \brief Validator that checks whether a numeric value falls within a specified range.
 *
 * This file defines the \ref RangeValidator class, which ensures that a value
 * of an arithmetic type is between a minimum and maximum (inclusive). It is
 * commonly used to validate command‑line arguments that expect a numeric value
 * within certain bounds.
 *
 * \ingroup Validators
 */

#pragma once


#include "tidop/config.h"

#include <string>
#include <memory>

#include "tidop/core/app/Message.h"
#include "tidop/core/console/Validator.h"
#include "tidop/core/base/Format.h"
#include "tidop/core/base/Exception.h"

namespace tl
{

/*! \addtogroup Validators
 *  \{
 */


/*!
 * \brief Validator that checks if a numeric value lies within a given range.
 *
 * This validator is templated on an arithmetic type `T` (e.g., `int`, `double`).
 * It ensures that a value is between `mMin` and `mMax` (inclusive). If the value
 * is outside the range, an error message is emitted via \ref Message::error.
 *
 * The default constructor sets the range to the full extent of the type
 * (from `lowest()` to `max()`). The range can be modified at any time using
 * \ref setRange().
 *
 * \tparam T The arithmetic type of the value to validate.
 */
template <typename T>
class RangeValidator final
  : public ValidatorBase<T>
{

    static_assert(std::is_arithmetic<T>::value, "Only arithmetic types are allowed");

public:

    /*!
     * \brief Default constructor.
     *
     * Initialises the range to the lowest and highest possible values for
     * the type `T` using `std::numeric_limits`.
     */
    RangeValidator()
      : mMin(std::numeric_limits<T>::lowest()),
        mMax(std::numeric_limits<T>().max())
    {
    }

    /*!
     * \brief Constructor with explicit range boundaries.
     * \param[in] min The minimum allowed value (inclusive).
     * \param[in] max The maximum allowed value (inclusive).
     */
    RangeValidator(T min, T max)
      : mMin(min),
        mMax(max)
    {
    }

    /*!
     * \brief Validates the given value.
     * \param[in] value The value to be checked.
     * \return `true` if `min <= value <= max`; otherwise `false` and an error
     *         message is printed.
     */
    [[nodiscard]]
    auto validate(T value) -> bool override
    {
        bool valid = (value >= mMin && value <= mMax);
        
        if (!valid) {
            Message::error("The value '{}' is out of valid range [{}-{}]", value, mMin, mMax);
        }

        return valid;
    }

    /*!
     * \brief Updates the allowed range.
     * \param min New minimum value.
     * \param max New maximum value.
     */
    void setRange(T min, T max)
    {
        mMin = min;
        mMax = max;
    }

    /*!
     * \brief Returns the current minimum value.
     * \return The lower bound.
     */
    [[nodiscard]]
    auto min() const noexcept -> T
    {
        return mMin;
    }

    /*!
     * \brief Returns the current maximum value.
     * \return The upper bound.
     */
    [[nodiscard]]
    auto max() const noexcept -> T
    {
        return mMax;
    }

    /*!
     * \brief Returns a string description of the validation rule.
     * \return A string like "Valid range [0 - 100]".
     */
    [[nodiscard]] 
    auto toString() const -> std::string override
    {
        return tl::format("Valid range [{} - {}]", mMin, mMax);
    }

    /*!
     * \brief Factory method for creating a shared pointer to a RangeValidator.
     * \param[in] min Lower bound of the range.
     * \param[in] max Upper bound of the range.
     * \return std::shared_ptr<RangeValidator<T>>
     */
    static auto create(T min, T max) -> std::shared_ptr<RangeValidator<T>>
    {
        return std::make_shared<RangeValidator<T>>(min, max);
    }

private:

    T mMin;
    T mMax;

};

/*! \} */

} // namespace tl
