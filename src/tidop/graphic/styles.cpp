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
    mPattern(""),
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

Pen::~Pen()
{
}

Color Pen::color() const
{
    return mColor;
}

void Pen::setColor(const Color &color)
{
    mColor = color;
}

uint8_t Pen::width() const
{
    return mWidth;
}

void Pen::setWidth(uint8_t width)
{
    mWidth = width;
}

std::string Pen::pattern() const
{
    return mPattern;
}

void Pen::setPattern(std::string pattern)
{
    mPattern = pattern;
}

Pen::Name Pen::name() const
{
    return mName;
}

void Pen::setName(Name name)
{
    mName = name;
}

Pen::Cap Pen::cap() const
{
    return mCap;
}

void Pen::setCap(Cap cap)
{
    mCap = cap;
}

Pen::Join Pen::join() const
{
    return mJoin;
}

void Pen::setJoin(Join join)
{
    mJoin = join;
}

int32_t Pen::perpendicularOffset() const
{
    return mPerpendicularOffset;
}

void Pen::setPerpendicularOffset(int32_t perpendicularoffset)
{
    mPerpendicularOffset = perpendicularoffset;
}

uint32_t Pen::priorityLevel() const
{
    return mPriorityLevel;
}

void Pen::setPriorityLevel(uint32_t priorityLevel)
{
    mPriorityLevel = priorityLevel;
}

Pen &Pen::operator = (const Pen &stylePen)
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

Brush::~Brush()
{
}

Color Brush::foregroundColor() const
{
    return mForeColor;
}

void Brush::setForegroundColor(Color foregroundColor)
{
    mForeColor = foregroundColor;
}

Color Brush::backgroundColor() const
{
    return mBackColor;
}

void Brush::setBackgroundColor(Color backgroundColor)
{
    mBackColor = backgroundColor;
}

Brush::Name Brush::name() const
{
    return mName;
}

void Brush::setName(Name name)
{
    mName = name;
}

double Brush::angle() const
{
    return mAngle;
}

void Brush::setAngle(double angle)
{
    mAngle = angle;
}

double Brush::scalingFactor() const
{
    return mScalingFactor;
}

void Brush::setScalingFactor(double scalingFactor)
{
    mScalingFactor = scalingFactor;
}

double Brush::spacingX() const
{
    return mSpacing[0];
}

double Brush::spacingY() const
{
    return mSpacing[1];
}

void Brush::setSpacing(double spacingX, double spacingY)
{
    mSpacing[0] = spacingX;
    mSpacing[1] = spacingY;
}

uint32_t Brush::priorityLevel() const
{
    return mPriorityLevel;
}

void Brush::setPriorityLevel(uint32_t priorityLevel)
{
    mPriorityLevel = priorityLevel;
}

Brush &Brush::operator = (const Brush &brush)
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

Symbol::~Symbol()
{
}

double Symbol::angle() const
{
    return mAngle;
}

void Symbol::setAngle(double angle)
{
    mAngle = angle;
}

Symbol::Name Symbol::name() const
{
    return mName;
}

void Symbol::setName(Name name)
{
    mName = name;
}

Color Symbol::color() const
{
    return mColor;
}

void Symbol::setColor(const Color &color)
{
    mColor = color;
}

Color Symbol::outlineColor() const
{
    return mOutlineColor;
}

void Symbol::setOutlineColor(const Color &outlinecolor)
{
    mOutlineColor = outlinecolor;
}

double Symbol::scalingFactor() const
{
    return mScalingFactor;
}

void Symbol::setScalingFactor(double scalingFactor)
{
    mScalingFactor = scalingFactor;
}

double Symbol::offsetX() const
{
    return mOffset[0];
}

double Symbol::offsetY() const
{
    return mOffset[1];
}

void Symbol::setOffset(double dx, double dy)
{
    mOffset[0] = dx;
    mOffset[1] = dy;
}

uint32_t Symbol::priorityLevel() const
{
    return mPriorityLevel;
}

void Symbol::setPriorityLevel(uint32_t priorityLevel)
{
    mPriorityLevel = priorityLevel;
}

Symbol &Symbol::operator = (const Symbol &symbol)
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


/* ---------------------------------------------------------------------------------- */


Label::Label()
  : mFont(),
    mText(""),
    mAngle(0.),
    mForegroundColor(0),
    mBackgroundColor(0),
    mOutlineColor(0),
    mShadowColor(0),
    mStretch(100.),
    mPlacement(Placement::p),
    mAnchorPosition(AnchorPosition::vertical_baseline | AnchorPosition::horizontal_left),
    mPerpendicularOffset(0),
    bStrikeout(false),
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
    bStrikeout(label.bStrikeout),
    mPriorityLevel(label.mPriorityLevel)
{
}

Label::~Label()
{
}

std::string Label::text() const
{
    return mText;
}

void Label::setText(const std::string &text)
{
    mText = text;
}

double Label::angle() const
{
    return mAngle;
}

void Label::setAngle(double angle)
{
    mAngle = angle;
}

Color Label::foregroundColor() const
{
    return mForegroundColor;
}

void Label::setForegroundColor(const Color &color)
{
    mForegroundColor = color;
}

Color Label::backgroundColor() const
{
    return mBackgroundColor;
}

void Label::setBackgroundColor(const Color &color)
{
    mBackgroundColor = color;
}

Color Label::outlineColor() const
{
    return mOutlineColor;
}

void Label::setOutlineColor(const Color &color)
{
    mOutlineColor = color;
}

Color Label::shadowColor() const
{
    return mShadowColor;
}

void Label::setShadowColor(const Color &color)
{
    mShadowColor = color;
}

double Label::stretch() const
{
    return mStretch;
}

void Label::setStretch(double stretch)
{
    mStretch = stretch;
}

Label::Placement Label::placement() const
{
    return mPlacement;
}

void Label::setPlacement(Placement placement)
{
    mPlacement = placement;
}

Label::AnchorPosition Label::anchorPosition() const
{
    return mAnchorPosition;
}

void Label::setAnchorPosition(AnchorPosition anchorPosition)
{
    mAnchorPosition = anchorPosition;
}

double Label::offsetX() const
{
    return mOffset[0];
}

double Label::offsetY() const
{
    return mOffset[1];
}

void Label::setOffset(double dx, double dy)
{
    mOffset[0] = dx;
    mOffset[1] = dy;
}

int Label::perpendicularOffset() const
{
    return mPerpendicularOffset;
}

void Label::setPerpendicularOffset(int perpendicularOffset)
{
    mPerpendicularOffset = perpendicularOffset;
}

Label &Label::operator = (const Label &label)
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
        bStrikeout = label.bStrikeout;
        mPriorityLevel = label.mPriorityLevel;
    }
    return *this;
}

void Label::setFont(const Font &font)
{
    mFont = font;
}

Font Label::font() const
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


Pen *GraphicStyle::pen() const
{
    return mPen.get();
}

void GraphicStyle::setPen(const std::shared_ptr<Pen> &pen)
{
    mPen = pen;
}

Brush *GraphicStyle::brush() const
{
    return mBrush.get();
}

void GraphicStyle::setBrush(const std::shared_ptr<Brush> &brush)
{
    mBrush = brush;
}

Symbol *GraphicStyle::symbol() const
{
    return mSymbol.get();
}

void GraphicStyle::setSymbol(const std::shared_ptr<Symbol> &symbol)
{
    mSymbol = symbol;
}

Label *GraphicStyle::label() const
{
    return mLabel.get();
}

void GraphicStyle::setLabel(const std::shared_ptr<Label> &label)
{
    mLabel = label;
}

GraphicStyle &GraphicStyle::operator = (const GraphicStyle &graphicStyle)
{
    if (this != &graphicStyle) {
        mPen = graphicStyle.mPen;
        mBrush = graphicStyle.mBrush;
        mSymbol = graphicStyle.mSymbol;
        mLabel = graphicStyle.mLabel;
    }
    return *this;
}


} // End namespace tl
