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

#include "tidop/graphic/styles/pen.h"


namespace tl
{


Pen::Pen()
  : mColor(Color::Name::black),
    mWidth(1),
    mPredefinedPattern(PredefinedPattern::solid),
    mCap(Cap::butt),
    mJoin(Join::bevel),
    mPerpendicularOffset(0),
    mPriorityLevel(0)
{
}

Pen::Pen(const Pen &pen)
  : mColor(pen.mColor),
    mWidth(pen.mWidth),
    mPattern(pen.mPattern),
    mPredefinedPattern(pen.mPredefinedPattern),
    mCap(pen.mCap),
    mJoin(pen.mJoin),
    mPerpendicularOffset(pen.mPerpendicularOffset),
    mPriorityLevel(pen.mPriorityLevel)
{
}

Pen::Pen(Pen&& pen) TL_NOEXCEPT
  : mColor(std::move(pen.mColor)),
    mWidth(pen.mWidth),
    mPattern(std::move(pen.mPattern)),
    mPredefinedPattern(pen.mPredefinedPattern),
    mCap(pen.mCap),
    mJoin(pen.mJoin),
    mPerpendicularOffset(pen.mPerpendicularOffset),
    mPriorityLevel(pen.mPriorityLevel)
{

}

Pen::~Pen() = default;

auto Pen::color() const -> Color
{
    return mColor;
}

void Pen::setColor(const Color &color)
{
    mColor = color;
}

auto Pen::width() const -> uint8_t
{
    return mWidth;
}

void Pen::setWidth(uint8_t width)
{
    mWidth = width;
}

auto Pen::pattern() const -> std::string
{
    return mPattern;
}

void Pen::setPattern(const std::string &pattern)
{
    mPattern = pattern;
}

auto Pen::predefinedPattern() const -> PredefinedPattern
{
    return mPredefinedPattern;
}

void Pen::setPredefinedPattern(PredefinedPattern pattern)
{
    mPredefinedPattern = pattern;
}

auto Pen::cap() const -> Cap
{
    return mCap;
}

void Pen::setCap(Cap cap)
{
    mCap = cap;
}

auto Pen::join() const -> Join
{
    return mJoin;
}

void Pen::setJoin(Join join)
{
    mJoin = join;
}

auto Pen::perpendicularOffset() const -> int32_t
{
    return mPerpendicularOffset;
}

void Pen::setPerpendicularOffset(int32_t perpendicularoffset)
{
    mPerpendicularOffset = perpendicularoffset;
}

auto Pen::priorityLevel() const -> uint32_t
{
    return mPriorityLevel;
}

void Pen::setPriorityLevel(uint32_t priorityLevel)
{
    mPriorityLevel = priorityLevel;
}

auto Pen::operator =(const Pen& stylePen) -> Pen&
{
    if (this != &stylePen) {
        mColor = stylePen.mColor;
        mWidth = stylePen.mWidth;
        mPattern = stylePen.mPattern;
        mPredefinedPattern = stylePen.mPredefinedPattern;
        mCap = stylePen.mCap;
        mJoin = stylePen.mJoin;
        mPerpendicularOffset = stylePen.mPerpendicularOffset;
        mPriorityLevel = stylePen.mPriorityLevel;
    }
    return *this;
}

auto Pen::operator=(Pen &&stylePen) TL_NOEXCEPT -> Pen&
{
    if (this != &stylePen) {
        mColor = std::move(stylePen.mColor);
        mWidth = stylePen.mWidth;
        mPattern = std::move(stylePen.mPattern);
        mPredefinedPattern = stylePen.mPredefinedPattern;
        mCap = stylePen.mCap;
        mJoin = stylePen.mJoin;
        mPerpendicularOffset = stylePen.mPerpendicularOffset;
        mPriorityLevel = stylePen.mPriorityLevel;
    }
    return *this;
}

#ifdef TL_WARNING_DEPRECATED_METHOD
auto Pen::name() const -> Name
{
    return static_cast<Pen::Name>(static_cast<std::underlying_type<Pen::PredefinedPattern>::type>(mPredefinedPattern));
}

void Pen::setName(Name name)
{
    mPredefinedPattern = static_cast<Pen::PredefinedPattern>(static_cast<std::underlying_type<Pen::Name>::type>(name));
}
#endif // TL_WARNING_DEPRECATED_METHOD

} // End namespace tl
