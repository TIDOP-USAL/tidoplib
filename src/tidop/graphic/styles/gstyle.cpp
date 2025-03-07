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

#include "tidop/graphic/styles/gstyle.h"


namespace tl
{


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
