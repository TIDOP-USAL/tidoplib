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

#include "tidop/core/base/property.h"


namespace tl
{

Properties::Properties(std::string name)
    : mName(std::move(name))
{
}

auto Properties::name() const TL_NOEXCEPT -> std::string
{
    return mName;
}

void Properties::setName(const std::string &name)
{
    mName = name;
}

auto Properties::getPropertyAsString(const std::string &key) const -> std::string
{
    auto it = mProperties.find(key);
    if (it == mProperties.end()) {
        throw std::out_of_range("Property not found: " + key);
    }
    return it->second->toString();
}

auto Properties::hasProperty(const std::string &key) const -> bool {
    return mProperties.find(key) != mProperties.end();
}

auto Properties::getPropertyType(const std::string &key) const -> Type
{
    auto it = mProperties.find(key);
    if (it == mProperties.end()) {
        throw std::out_of_range("Property not found: " + key);
    }
    return it->second->type();
}

} // End namespace tl

