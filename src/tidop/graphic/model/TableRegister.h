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

#include <vector>
#include <string>

#include "tidop/config.h"
#include "tidop/graphic/model/RegisterValue.h"

namespace tl
{

/*! \addtogroup Graphic
 *  \{
 */


/*!
 * \class TableRegister
 * \brief Represents a single record (row) in a attributes table.
 *
 * A `TableRegister` stores one value per field, forming a complete table row.
 * Values are internally managed as `RegisterValue` objects.
 */
class TL_EXPORT TableRegister
{

protected:

    std::vector<RegisterValue> mRegisterValues;

public:

    TableRegister() = default;

    /*!
     * \brief Constructs a new record with the given table schema.
     * \param[in] fields Table fields defining the register structure.
     */
    TableRegister(const std::vector<TableField> &fields);

    /*!
     * \brief Returns the value at the given index.
     * \param[in] idx Field index.
     * \return Value as string.
     */
    auto value(size_t idx) const -> std::string;

    /*!
     * \brief Sets the value at the given index.
     * \param[in] idx Field index.
     * \param[in] field Value as string.
     */
    void setValue(size_t idx, const std::string &field);

    /*!
     * \brief Returns the number of fields in the register.
     */
    auto size() const -> size_t;

};

/*! \} */

} // namespace tl
