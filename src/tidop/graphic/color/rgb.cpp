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

#include "tidop/graphic/color/rgb.h"

namespace tl
{


ColorModel::ColorModel() = default;
ColorModel::~ColorModel() = default;

ColorRGB::ColorRGB()
  : mRed(0),
    mGreen(0),
    mBlue(0),
    mRangeMin(0),
    mRangeMax(255)
{}

ColorRGB::ColorRGB(int red, int green, int blue)
  : mRed(red),
    mGreen(green),
    mBlue(blue),
    mRangeMin(0),
    mRangeMax(255)
{
    this->adjustRangeRed();
    this->adjustRangeGreen();
    this->adjustRangeBlue();
}

ColorRGB::ColorRGB(const ColorRGB &color)
  :  mRed(color.mRed),
     mGreen(color.mGreen),
     mBlue(color.mBlue),
     mRangeMin(color.mRangeMin),
     mRangeMax(color.mRangeMax)
{
}

ColorRGB::ColorRGB(ColorRGB&& color) TL_NOEXCEPT
  : mRed(std::exchange(color.mRed, 0)),
    mGreen(std::exchange(color.mGreen, 0)),
    mBlue(std::exchange(color.mBlue, 0)),
    mRangeMin(std::exchange(color.mRangeMin, 0)),
    mRangeMax(std::exchange(color.mRangeMax, 255))
{
}

ColorRGB::~ColorRGB() = default;

auto ColorRGB::red() const -> int
{
    return mRed;
}

void ColorRGB::setRed(int red)
{
    mRed = red;
    this->adjustRangeRed();
}

auto ColorRGB::green() const -> int
{
    return mGreen;
}

void ColorRGB::setGreen(int green)
{
    mGreen = green;
    this->adjustRangeGreen();
}

auto ColorRGB::blue() const -> int
{
    return mBlue;
}

void ColorRGB::setBlue(int blue)
{
    mBlue = blue;
    this->adjustRangeBlue();
}

auto ColorRGB::operator =(const ColorRGB &color) -> ColorRGB &
{
    if (this != &color) {
        this->mRed = color.mRed;
        this->mGreen = color.mGreen;
        this->mBlue = color.mBlue;
        this->mRangeMin = color.mRangeMin;
        this->mRangeMax = color.mRangeMax;
    }

    return *this;
}

auto ColorRGB::operator=(ColorRGB&& color) TL_NOEXCEPT -> ColorRGB&
{
    if (this != &color) {
        this->mRed = std::exchange(color.mRed, 0);
        this->mGreen = std::exchange(color.mGreen, 0);
        this->mBlue = std::exchange(color.mBlue, 0);
        this->mRangeMin = std::exchange(color.mRangeMin, 0);
        this->mRangeMax = std::exchange(color.mRangeMax, 255);
    }

    return *this;
}

auto ColorRGB::toColor() const -> Color
{
    Color color((mBlue & 0xFF) |
                ((mGreen << 8) & 0xFF00) |
                ((mRed << 16) & 0xFF0000));
    return color;
}

void ColorRGB::fromColor(const Color &color)
{
    mRed = color.red();
    mGreen = color.green();
    mBlue = color.blue();
}

void ColorRGB::adjustRangeRed()
{
    if (mRed < mRangeMin) mRed = mRangeMin;
    else if (mRed > mRangeMax) mRed = mRangeMax;
}

void ColorRGB::adjustRangeGreen()
{
    if (mGreen < mRangeMin) mGreen = mRangeMin;
    else if (mGreen > mRangeMax) mGreen = mRangeMax;
}

void ColorRGB::adjustRangeBlue()
{
    if (mBlue < mRangeMin) mBlue = mRangeMin;
    else if (mBlue > mRangeMax) mBlue = mRangeMax;
}


} // End namespace tl
