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

namespace tl
{

/*! \addtogroup Graphic
 *  \{
 */

/*!
 * \class TableField
 * \brief Represents a field (column) definition in a table schema.
 *
 * A `TableField` stores metadata about a table column, including its name,
 * attributes type and maximum size. It defines the structure of a `TableRegister`.
 */
class TL_EXPORT TableField
{

public:

    /*!
     * \enum Type
     * \brief Supported field attributes types.
     */
    enum class Type
    {
        INT,       /*!< 32-bit integer */
        INT64,     /*!< 64-bit integer */
        DOUBLE,    /*!< Double-precision floating point */
        STRING     /*!< UTF-8 encoded string */
        //....
    };

private:

    std::string mName;
    Type mType;
    int mSize;

public:

    //TableField() = default;

    /*!
     * \brief Constructor
     * \param[in] name Field name
     * \param[in] type Field type
     * \param[in] size Maximum field size (in characters or bytes)
     */
    TableField(const std::string &name, Type type, int size);

    ~TableField();

    /*!
     * \brief Returns the field name
     */
    [[nodiscard]]
    auto name() const ->std::string;

    /*!
     * \brief Returns the field type
     */
    [[nodiscard]]
    auto type() const -> Type;

    /*!
     * \brief Returns the field size
     */
    [[nodiscard]]
    auto size() const -> int;

};

/*! \} */

} // namespace tl
