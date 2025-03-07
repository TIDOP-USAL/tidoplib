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

#include "tidop/graphic/styles/symbol.h"


namespace tl
{


Symbol::Symbol()
  : mName(Name::cross),
    mAngle(0.),
    mColor(0),
    mOutlineColor(0),
    mScalingFactor(1.),
    mPriorityLevel(0)
{
    mOffset[0] = mOffset[1] = 0.;
}

Symbol::Symbol(const Symbol &symbol)
  : mName(symbol.mName),
    mAngle(symbol.mAngle),
    mColor(symbol.mColor),
    mOutlineColor(symbol.mOutlineColor),
    mScalingFactor(symbol.mScalingFactor),
    mOffset(symbol.mOffset),
    mPriorityLevel(symbol.mPriorityLevel)
{
}

Symbol::Symbol(Symbol&& symbol) TL_NOEXCEPT
  : mName(symbol.mName),
    mAngle(symbol.mAngle),
    mColor(std::move(symbol.mColor)),
    mOutlineColor(std::move(symbol.mOutlineColor)),
    mScalingFactor(symbol.mScalingFactor),
    mOffset(symbol.mOffset),
    mPriorityLevel(symbol.mPriorityLevel)
{
}

Symbol::~Symbol() = default;

auto Symbol::angle() const -> double
{
    return mAngle;
}

void Symbol::setAngle(double angle)
{
    mAngle = angle;
}

auto Symbol::name() const -> Name
{
    return mName;
}

void Symbol::setName(Name name)
{
    mName = name;
}

auto Symbol::color() const -> Color
{
    return mColor;
}

void Symbol::setColor(const Color &color)
{
    mColor = color;
}

auto Symbol::outlineColor() const -> Color
{
    return mOutlineColor;
}

void Symbol::setOutlineColor(const Color &outlinecolor)
{
    mOutlineColor = outlinecolor;
}

auto Symbol::scalingFactor() const -> double
{
    return mScalingFactor;
}

void Symbol::setScalingFactor(double scalingFactor)
{
    mScalingFactor = scalingFactor;
}

auto Symbol::offsetX() const -> double
{
    return mOffset[0];
}

auto Symbol::offsetY() const -> double
{
    return mOffset[1];
}

void Symbol::setOffset(double dx, double dy)
{
    mOffset[0] = dx;
    mOffset[1] = dy;
}

auto Symbol::priorityLevel() const -> uint32_t
{
    return mPriorityLevel;
}

void Symbol::setPriorityLevel(uint32_t priorityLevel)
{
    mPriorityLevel = priorityLevel;
}

auto Symbol::operator =(const Symbol& symbol) -> Symbol&
{
    if (this != &symbol) {
        mName = symbol.mName;
        mAngle = symbol.mAngle;
        mColor = symbol.mColor;
        mOutlineColor = symbol.mOutlineColor;
        mScalingFactor = symbol.mScalingFactor;
        mOffset = symbol.mOffset;
        mPriorityLevel = symbol.mPriorityLevel;
    }
    return *this;
}

auto Symbol::operator=(Symbol&& symbol) TL_NOEXCEPT -> Symbol&
{
    if (this != &symbol) {
        mName = symbol.mName;
        mAngle = symbol.mAngle;
        mColor = std::move(symbol.mColor);
        mOutlineColor = std::move(symbol.mOutlineColor);
        mScalingFactor = symbol.mScalingFactor;
        mOffset = symbol.mOffset;
        mPriorityLevel = symbol.mPriorityLevel;
    }
    return *this;
}


} // End namespace tl
