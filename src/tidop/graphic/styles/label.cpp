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

#include "tidop/graphic/styles/label.h"


namespace tl
{



Label::Label()
  : mAngle(0.),
    mForegroundColor(0),
    mBackgroundColor(0),
    mOutlineColor(0),
    mShadowColor(0),
    mStretch(100.),
    mPlacement(Placement::point),
    mAnchorPosition(AnchorPosition::vertical_baseline | AnchorPosition::horizontal_left),
    mPerpendicularOffset(0),
    mStrikeout(false),
    mPriorityLevel(0)
{
    mOffset[0] = mOffset[1] = 0.;
}

Label::Label(const Label &label)
  : mFont(label.mFont),
    mLicenseText(label.mLicenseText),
    mAngle(label.mAngle),
    mForegroundColor(label.mForegroundColor),
    mBackgroundColor(label.mBackgroundColor),
    mOutlineColor(label.mOutlineColor),
    mShadowColor(label.mShadowColor),
    mStretch(label.mStretch),
    mPlacement(label.mPlacement),
    mAnchorPosition(label.mAnchorPosition),
    mOffset(label.mOffset),
    mPerpendicularOffset(label.mPerpendicularOffset),
    mStrikeout(label.mStrikeout),
    mPriorityLevel(label.mPriorityLevel)
{
}

Label::Label(Label &&label) TL_NOEXCEPT
  : mFont(label.mFont),
    mLicenseText(label.mLicenseText),
    mAngle(label.mAngle),
    mForegroundColor(std::move(label.mForegroundColor)),
    mBackgroundColor(std::move(label.mBackgroundColor)),
    mOutlineColor(std::move(label.mOutlineColor)),
    mShadowColor(std::move(label.mShadowColor)),
    mStretch(label.mStretch),
    mPlacement(label.mPlacement),
    mAnchorPosition(label.mAnchorPosition),
    mOffset(label.mOffset),
    mPerpendicularOffset(label.mPerpendicularOffset),
    mStrikeout(label.mStrikeout),
    mPriorityLevel(label.mPriorityLevel)
{
}

Label::~Label() = default;

auto Label::text() const -> std::string
{
    return mLicenseText;
}

void Label::setText(const std::string &text)
{
    mLicenseText = text;
}

auto Label::angle() const -> double
{
    return mAngle;
}

void Label::setAngle(double angle)
{
    mAngle = angle;
}

auto Label::foregroundColor() const -> Color
{
    return mForegroundColor;
}

void Label::setForegroundColor(const Color &color)
{
    mForegroundColor = color;
}

auto Label::backgroundColor() const -> Color
{
    return mBackgroundColor;
}

void Label::setBackgroundColor(const Color &color)
{
    mBackgroundColor = color;
}

auto Label::outlineColor() const -> Color
{
    return mOutlineColor;
}

void Label::setOutlineColor(const Color &color)
{
    mOutlineColor = color;
}

auto Label::shadowColor() const -> Color
{
    return mShadowColor;
}

void Label::setShadowColor(const Color &color)
{
    mShadowColor = color;
}

auto Label::stretch() const -> double
{
    return mStretch;
}

void Label::setStretch(double stretch)
{
    mStretch = stretch;
}

auto Label::placement() const -> Label::Placement
{
    return mPlacement;
}

void Label::setPlacement(Placement placement)
{
    mPlacement = placement;
}

auto Label::anchorPosition() const -> Label::AnchorPosition
{
    return mAnchorPosition;
}

void Label::setAnchorPosition(AnchorPosition anchorPosition)
{
    mAnchorPosition = anchorPosition;
}

auto Label::offsetX() const -> double
{
    return mOffset[0];
}

auto Label::offsetY() const -> double
{
    return mOffset[1];
}

void Label::setOffset(double dx, double dy)
{
    mOffset[0] = dx;
    mOffset[1] = dy;
}

auto Label::perpendicularOffset() const -> int
{
    return mPerpendicularOffset;
}

void Label::setPerpendicularOffset(int perpendicularOffset)
{
    mPerpendicularOffset = perpendicularOffset;
}

auto Label::operator =(const Label& label) -> Label&
{
    if (this != &label) {
        mFont = label.mFont;
        mLicenseText = label.mLicenseText;
        mAngle = label.mAngle;
        mForegroundColor = label.mForegroundColor;
        mBackgroundColor = label.mBackgroundColor;
        mOutlineColor = label.mOutlineColor;
        mShadowColor = label.mShadowColor;
        mStretch = label.mStretch;
        mPlacement = label.mPlacement;
        mAnchorPosition = label.mAnchorPosition;
        mPerpendicularOffset = label.mPerpendicularOffset;
        mStrikeout = label.mStrikeout;
        mPriorityLevel = label.mPriorityLevel;
    }
    return *this;
}

auto Label::operator=(Label&& label) TL_NOEXCEPT -> Label&
{
    if (this != &label) {
        mFont = label.mFont;
        mLicenseText = label.mLicenseText;
        mAngle = label.mAngle;
        mForegroundColor = std::move(label.mForegroundColor);
        mBackgroundColor = std::move(label.mBackgroundColor);
        mOutlineColor = std::move(label.mOutlineColor);
        mShadowColor = std::move(label.mShadowColor);
        mStretch = label.mStretch;
        mPlacement = label.mPlacement;
        mAnchorPosition = label.mAnchorPosition;
        mPerpendicularOffset = label.mPerpendicularOffset;
        mStrikeout = label.mStrikeout;
        mPriorityLevel = label.mPriorityLevel;
    }
    return *this;
}

void Label::setFont(const Font &font)
{
    mFont = font;
}

auto Label::font() const -> Font
{
    return mFont;
}



} // End namespace tl
