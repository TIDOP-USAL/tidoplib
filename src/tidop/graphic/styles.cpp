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

#include "tidop/graphic/styles.h"


namespace tl
{


Pen::Pen()
  : mColor(0),
    mWidth(1),
    mName(Name::solid),
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
    mName(pen.mName),
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
    mName(pen.mName),
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

auto Pen::name() const -> Name
{
    return mName;
}

void Pen::setName(Name name)
{
    mName = name;
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
        mName = stylePen.mName;
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
        mName = stylePen.mName;
        mCap = stylePen.mCap;
        mJoin = stylePen.mJoin;
        mPerpendicularOffset = stylePen.mPerpendicularOffset;
        mPriorityLevel = stylePen.mPriorityLevel;
    }
    return *this;
}


/* ---------------------------------------------------------------------------------- */


Brush::Brush()
  : mForeColor(0),
    mBackColor(0),
    mName(Name::solid),
    mAngle(0.),
    mScalingFactor(1.),
    mPriorityLevel(0)
{
    mSpacing[0] = mSpacing[1] = 0.;
}

Brush::Brush(const Brush &brush)
  : mForeColor(brush.mForeColor),
    mBackColor(brush.mBackColor),
    mName(brush.mName),
    mAngle(brush.mAngle),
    mScalingFactor(brush.mScalingFactor),
    mSpacing(brush.mSpacing),
    mPriorityLevel(brush.mPriorityLevel)
{
}

Brush::Brush(Brush&& brush) TL_NOEXCEPT
  : mForeColor(std::move(brush.mForeColor)),
    mBackColor(std::move(brush.mBackColor)),
    mName(brush.mName),
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

auto Brush::name() const -> Name
{
    return mName;
}

void Brush::setName(Name name)
{
    mName = name;
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
        mName = brush.mName;
        mAngle = brush.mAngle;
        mScalingFactor = brush.mScalingFactor;
        mSpacing = brush.mSpacing;
        mPriorityLevel = brush.mPriorityLevel;
    }
    return *this;
}

auto Brush::operator=(Brush&& brush) TL_NOEXCEPT -> Brush&
{
    if (this != &brush) {
        mForeColor = std::move(brush.mForeColor);
        mBackColor = std::move(brush.mBackColor);
        mName = brush.mName;
        mAngle = brush.mAngle;
        mScalingFactor = brush.mScalingFactor;
        mSpacing = brush.mSpacing;
        mPriorityLevel = brush.mPriorityLevel;
    }
    return *this;
}





/* ---------------------------------------------------------------------------------- */




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



/* ---------------------------------------------------------------------------------- */



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
    mText(label.mText),
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
    mText(label.mText),
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
    return mText;
}

void Label::setText(const std::string &text)
{
    mText = text;
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
        mText = label.mText;
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
        mText = label.mText;
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



/* ---------------------------------------------------------------------------------- */


GraphicStyle::GraphicStyle()
  : mPen(std::make_shared<Pen>()),
    mBrush(nullptr/*std::make_shared<Brush>()*/),
    mSymbol(std::make_shared<Symbol>()),
    mLabel(std::make_shared<Label>())
{
}

GraphicStyle::GraphicStyle(const GraphicStyle &graphicStyle)
  : mPen(graphicStyle.mPen),
    mBrush(graphicStyle.mBrush),
    mSymbol(graphicStyle.mSymbol),
    mLabel(graphicStyle.mLabel)
{
}

GraphicStyle::GraphicStyle(GraphicStyle &&graphicStyle) TL_NOEXCEPT
  : mPen(std::move(graphicStyle.mPen)),
    mBrush(std::move(graphicStyle.mBrush)),
    mSymbol(std::move(graphicStyle.mSymbol)),
    mLabel(std::move(graphicStyle.mLabel))
{
}

auto GraphicStyle::pen() const -> Pen*
{
    return mPen.get();
}

void GraphicStyle::setPen(const std::shared_ptr<Pen> &pen)
{
    mPen = pen;
}

auto GraphicStyle::brush() const -> Brush*
{
    return mBrush.get();
}

void GraphicStyle::setBrush(const std::shared_ptr<Brush> &brush)
{
    mBrush = brush;
}

auto GraphicStyle::symbol() const -> Symbol*
{
    return mSymbol.get();
}

void GraphicStyle::setSymbol(const std::shared_ptr<Symbol> &symbol)
{
    mSymbol = symbol;
}

auto GraphicStyle::label() const -> Label*
{
    return mLabel.get();
}

void GraphicStyle::setLabel(const std::shared_ptr<Label> &label)
{
    mLabel = label;
}

auto GraphicStyle::operator =(const GraphicStyle& graphicStyle) -> GraphicStyle&
{
    if (this != &graphicStyle) {
        mPen = graphicStyle.mPen;
        mBrush = graphicStyle.mBrush;
        mSymbol = graphicStyle.mSymbol;
        mLabel = graphicStyle.mLabel;
    }
    return *this;
}

auto GraphicStyle::operator =(GraphicStyle &&graphicStyle) TL_NOEXCEPT -> GraphicStyle &
{
    if (this != &graphicStyle) {
        mPen = std::move(graphicStyle.mPen);
        mBrush = std::move(graphicStyle.mBrush);
        mSymbol = std::move(graphicStyle.mSymbol);
        mLabel = std::move(graphicStyle.mLabel);
    }
    return *this;
}

} // End namespace tl
