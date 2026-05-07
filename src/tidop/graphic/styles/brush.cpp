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

#include "tidop/graphic/styles/brush.h"


namespace tl
{


Brush::Brush()
  : mForeColor(0),
    mBackColor(0),
    mPattern(Pattern::solid),
    mAngle(0.),
    mScalingFactor(1.),
    mPriorityLevel(0)
{
    mSpacing[0] = mSpacing[1] = 0.;
}

Brush::Brush(const Brush &brush)
  : mForeColor(brush.mForeColor),
    mBackColor(brush.mBackColor),
    mPattern(brush.mPattern),
    mAngle(brush.mAngle),
    mScalingFactor(brush.mScalingFactor),
    mSpacing(brush.mSpacing),
    mPriorityLevel(brush.mPriorityLevel)
{
}

Brush::Brush(Brush&& brush) TL_NOEXCEPT
  : mForeColor(std::move(brush.mForeColor)),
    mBackColor(std::move(brush.mBackColor)),
    mPattern(brush.mPattern),
    mAngle(brush.mAngle),
    mScalingFactor(brush.mScalingFactor),
    mSpacing(brush.mSpacing),
    mPriorityLevel(brush.mPriorityLevel)
{
}

Brush::~Brush() = default;

auto Brush::foregroundColor() const -> Color
{
    return mForeColor;
}

void Brush::setForegroundColor(const Color& foregroundColor)
{
    mForeColor = foregroundColor;
}

auto Brush::backgroundColor() const -> Color
{
    return mBackColor;
}

void Brush::setBackgroundColor(const Color& backgroundColor)
{
    mBackColor = backgroundColor;
}

auto Brush::pattern() const -> Pattern
{
    return mPattern;
}

void Brush::setPattern(Pattern pattern)
{
    mPattern = pattern;
}

auto Brush::angle() const -> double
{
    return mAngle;
}

void Brush::setAngle(double angle)
{
    mAngle = angle;
}

auto Brush::scalingFactor() const -> double
{
    return mScalingFactor;
}

void Brush::setScalingFactor(double scalingFactor)
{
    mScalingFactor = scalingFactor;
}

auto Brush::spacingX() const -> double
{
    return mSpacing[0];
}

auto Brush::spacingY() const -> double
{
    return mSpacing[1];
}

void Brush::setSpacing(double spacingX, double spacingY)
{
    mSpacing[0] = spacingX;
    mSpacing[1] = spacingY;
}

auto Brush::priorityLevel() const -> uint32_t
{
    return mPriorityLevel;
}

void Brush::setPriorityLevel(uint32_t priorityLevel)
{
    mPriorityLevel = priorityLevel;
}

auto Brush::operator =(const Brush& brush) -> Brush&
{
    if (this != &brush) {
        mForeColor = brush.mForeColor;
        mBackColor = brush.mBackColor;
        mPattern = brush.mPattern;
        mAngle = brush.mAngle;
        mScalingFactor = brush.mScalingFactor;
        mSpacing = brush.mSpacing;
        mPriorityLevel = brush.mPriorityLevel;
    }
    return *this;
}

#ifdef TL_WARNING_DEPRECATED_METHOD
auto Brush::name() const -> Name
{
    return static_cast<Brush::Name>(static_cast<std::underlying_type<Brush::Pattern>::type>(mPattern));
}

void Brush::setName(Name name)
{
    mPattern = static_cast<Brush::Pattern>(static_cast<std::underlying_type<Brush::Name>::type>(name));
}
#endif // TL_WARNING_DEPRECATED_METHOD

auto Brush::operator=(Brush&& brush) TL_NOEXCEPT -> Brush&
{
    if (this != &brush) {
        mForeColor = std::move(brush.mForeColor);
        mBackColor = std::move(brush.mBackColor);
        mPattern = brush.mPattern;
        mAngle = brush.mAngle;
        mScalingFactor = brush.mScalingFactor;
        mSpacing = brush.mSpacing;
        mPriorityLevel = brush.mPriorityLevel;
    }
    return *this;
}


} // End namespace tl
