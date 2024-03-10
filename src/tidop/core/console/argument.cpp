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

#include "tidop/core/console/argument.h"

namespace tl
{

Argument::Argument(std::string name,
                   std::string description, 
                   Type type)
  : mName(std::move(name)),
    mDescription(std::move(description)),
    mShortName(),
    mType(type)
{

}

Argument::Argument(const char &shortName,
                   std::string description, 
                   Type type)
  : mName(""),
    mDescription(std::move(description)),
    mShortName(shortName),
    mType(type)
{
}

Argument::Argument(std::string name,
                   const char &shortName,
                   std::string description, 
                   Type type)
  : mName(std::move(name)),
    mDescription(std::move(description)),
    mShortName(shortName),
    mType(type)
{
}

Argument::Argument(const Argument &argument)
  : mName(argument.mName),
    mDescription(argument.mDescription),
    mShortName(argument.mShortName),
    mType(argument.mType)
{
}

Argument::Argument(Argument &&argument) TL_NOEXCEPT
  : mName(std::move(argument.mName)),
    mDescription(std::move(argument.mDescription)),
    mShortName(argument.mShortName),
    mType(argument.mType)
{
}

auto Argument::operator=(const Argument &argument) -> Argument &
{
    if(this != &argument) {

        this->mName = argument.mName;
        this->mDescription = argument.mName;
        this->mShortName = argument.mShortName;
        this->mType = argument.mType;
    }

    return *this;
}

auto Argument::operator = (Argument &&argument) TL_NOEXCEPT -> Argument &
{
    if(this != &argument) {

        this->mName = std::move(argument.mName);
        this->mDescription = std::move(argument.mName);
        this->mShortName = argument.mShortName;
        this->mType = argument.mType;

    }

    return *this;
}

auto Argument::name() const -> std::string
{
    return mName;
}

void Argument::setName(const std::string &name)
{
    mName = name;
}

auto Argument::description() const -> std::string
{
    return mDescription;
}

void Argument::setDescription(const std::string &description)
{
    mDescription = description;
}

auto Argument::shortName() const -> char
{
    return mShortName;
}

void Argument::setShortName(const char &shortName)
{
    mShortName = shortName;
}

auto Argument::type() const -> Type
{
    return mType;
}

auto Argument::validator() const -> std::shared_ptr<Validator>
{
    return mValidator;
}

void Argument::setValidator(const std::shared_ptr<Validator> &validator)
{
    mValidator = validator;
}

} // End mamespace tl


