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

#include "tidop/graphic/color/rgba.h"

namespace tl
{


ColorRGBA::ColorRGBA()
  : ColorRGB(),
    mAlpha(0)
{

}

ColorRGBA::ColorRGBA(int red, int green, int blue, int alpha)
  : ColorRGB(red, green, blue),
    mAlpha(alpha)
{
    this->adjustRangeAlpha();
}

ColorRGBA::ColorRGBA(const ColorRGBA &color)
  : ColorRGB(color),
    mAlpha(color.mAlpha)
{
}

ColorRGBA::ColorRGBA(ColorRGBA &&color) TL_NOEXCEPT
  : ColorRGB(std::forward<ColorRGB>(color)),
    mAlpha(std::exchange(color.mAlpha, 255))
{
}

auto ColorRGBA::alpha() const -> int
{
    return mAlpha;
}

void ColorRGBA::setAlpha(int alpha)
{
    mAlpha = alpha;
    this->adjustRangeAlpha();
}

auto ColorRGBA::toColor() const -> Color
{
    Color color((mBlue & 0xFF) |
        ((mGreen << 8) & 0xFF00) |
        ((mRed << 16) & 0xFF0000) |
        ((mAlpha << 24) & 0xFF000000));
    return color;
}

void ColorRGBA::fromColor(const Color &color)
{
    ColorRGB::fromColor(color);
    mAlpha = color.opacity();
}

auto ColorRGBA::operator =(const ColorRGBA& color) -> ColorRGBA&
{
    if (this != &color) {
        this->mRed = color.mRed;
        this->mGreen = color.mGreen;
        this->mBlue = color.mBlue;
        this->mAlpha = color.mAlpha;
        this->mRangeMin = color.mRangeMin;
        this->mRangeMax = color.mRangeMax;
    }
    return *this;
}

auto ColorRGBA::operator=(ColorRGBA &&color) TL_NOEXCEPT -> ColorRGBA&
{
    if (this != &color) {
        this->mRed = std::exchange(color.mRed, 0);
        this->mGreen = std::exchange(color.mGreen, 0);
        this->mBlue = std::exchange(color.mBlue, 0);
        this->mAlpha = std::exchange(color.mAlpha, 255);
        this->mRangeMin = std::exchange(color.mRangeMin, 0);
        this->mRangeMax = std::exchange(color.mRangeMax, 255);
    }
    return *this;
}

void ColorRGBA::adjustRangeAlpha()
{
    if (mAlpha < mRangeMin) mAlpha = mRangeMin;
    else if (mAlpha > mRangeMax) mAlpha = mRangeMax;
}

} // End namespace tl
