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

#include "tidop/graphic/styles/GraphicStyle.h"


namespace tl
{

auto GraphicStyle::pen() const -> const Pen*
{
    return mPen ? &(*mPen) : nullptr;
}

void GraphicStyle::setPen(Pen pen)
{
    mPen = std::move(pen);
}

auto GraphicStyle::brush() const -> const Brush*
{
    return mBrush ? &(*mBrush) : nullptr;
}

void GraphicStyle::setBrush(Brush brush)
{
    mBrush = std::move(brush);
}

auto GraphicStyle::symbol() const -> const Symbol*
{
    return mSymbol ? &(*mSymbol) : nullptr;
}

void GraphicStyle::setSymbol(Symbol symbol)
{
    mSymbol = std::move(symbol) ;
}

auto GraphicStyle::label() const -> const Label*
{
    return mLabel ? &(*mLabel) : nullptr;
}

void GraphicStyle::setLabel(Label label)
{
    mLabel = std::move(label);
}

} // namespace tl
