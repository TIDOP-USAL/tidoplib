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

#include <string>

#include "tidop/config.h"
#include "tidop/graphic/model/TableField.h"

namespace tl
{

/*! \addtogroup Graphic
 *  \{
 */


/*!
 * \class RegisterValue
 * \brief Represents a single value in a table record.
 *
 * A `RegisterValue` holds a string-formatted value associated with a specific `TableField`.
 * The actual value may be stored in string form, regardless of its declared type.
 */
class TL_EXPORT RegisterValue
{

public:

    /*!
     * \brief Constructor with field reference.
     * \param[in] field Associated table field.
     */
    RegisterValue(const TableField &field);

    /*!
     * \brief Constructor with initial value.
     * \param[in] field Associated table field.
     * \param[in] value String-formatted value.
     */
    RegisterValue(const TableField &field,
                  std::string value);

    ~RegisterValue() = default;

    /*!
     * \brief Returns the string value.
     */
    auto value() const -> std::string;

    /*!
     * \brief Sets the string value.
     * \param[in] value New value to assign.
     */
    void setValue(const std::string &value);

private:

    TableField mField;
    std::string mValue;
};


/*! \} */

} // namespace tl
