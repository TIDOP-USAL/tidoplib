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

#include "tidop/graphic/model/RegisterValue.h"
#include "tidop/core/base/Meta.h"

namespace tl
{

RegisterValue::RegisterValue(const TableField &field)
  : mField(field)
{
}

RegisterValue::RegisterValue(const TableField &field,
                             std::string value)
  : mField(field),
    mValue(std::move(value))
{
}

auto RegisterValue::value() const -> std::string
{
    return mValue;
}

void RegisterValue::setValue(const std::string &value)
{
    TL_TODO("Usar TableField para comprobar que el valor introducido es correcto")
    mValue = value;
}

} // namespace tl
