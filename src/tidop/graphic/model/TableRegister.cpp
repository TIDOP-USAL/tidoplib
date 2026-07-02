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

#include "tidop/graphic/model/TableRegister.h"
#include "tidop/core/base/Meta.h"

namespace tl
{


TableRegister::TableRegister(const std::vector<TableField> &fields)
{
    size_t size = fields.size();

    for (size_t i = 0; i < size; i++) {
        mRegisterValues.push_back(RegisterValue(fields[i]));
    }
}

auto TableRegister::value(size_t idx) const -> std::string
{
    if (idx < mRegisterValues.size())
        return mRegisterValues[idx].value();

    return std::string{};
}

void TableRegister::setValue(size_t idx, const std::string &field)
{
    if (idx < mRegisterValues.size())
        mRegisterValues[idx].setValue(field);
}

auto TableRegister::size() const -> size_t
{
    return mRegisterValues.size();
}


} // namespace tl
