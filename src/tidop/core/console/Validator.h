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
 * \file Validator.h
 * \brief Defines the base interfaces for value validators.
 * 
 * This file provides the abstract base class \ref Validator and the templated
 * \ref ValidatorBase class, which serve as the foundation for implementing
 * concrete validators used to validate command-line arguments or other inputs.
 * 
 * \ingroup Validators
 */

#pragma once

#include "tidop/config.h"

namespace tl
{

/*! \addtogroup Validators
 *  \{
 */

/*! 
 * \brief Abstract base class for validators.
 * 
 * This class defines the common interface that all validators must implement.
 * The primary responsibility is to provide a textual description of the
 * validation rule via \ref toString(), which is typically used for generating
 * help messages or error reports.
 */
class TL_EXPORT Validator
{

public:

    Validator() = default;
    virtual ~Validator() = default;

    /*!
     * \brief Returns a string representation of the validation rule.
     * \return A description of the validation criteria
     */
    [[nodiscard]] 
    virtual auto toString() const -> std::string = 0;
};



/*!
 * \brief Templated base class for validators of a specific type.
 *
 * Extends \ref Validator to add type‑specific validation logic. Concrete
 * validators for a particular type `T` should derive from this class and
 * implement the \ref validate() method.
 *
 * \tparam T The type of the value to be validated (e.g., `int`, `double`,
 *           `std::string`, `tl::Path`).
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
     * This pure virtual function must be implemented by derived classes to
     * perform the actual validation logic.
     *
     * \param[in] value The value to be checked.
     * \return `true` if the value meets the validation criteria, `false` otherwise.
     */
    virtual bool validate(T value) = 0;
};

/*! \} */

} // namespace tl
