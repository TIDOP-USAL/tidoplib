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

#include "tidop/graphic/color/cmyk.h"
#include "tidop/core/base/type_conversions.h"

namespace tl
{

ColorCMYK::ColorCMYK()
  : mCyan(0.),
    mMagenta(0.),
    mYellow(0.),
    mKey(0.),
    mRangeMin(0.), /// TODO: tiene sentido pasar como parametro el rango minimo???
    mRangeMax(1.)
{

}

ColorCMYK::ColorCMYK(double cyan, double magenta, double yellow, double key)
  : mCyan(cyan),
    mMagenta(magenta),
    mYellow(yellow),
    mKey(key),
    mRangeMin(0.),
    mRangeMax(1.)
{
    this->adjustRangeCyan();
    this->adjustRangeMagenta();
    this->adjustRangeYellow();
    this->adjustRangeKey();
}

ColorCMYK::ColorCMYK(const ColorCMYK &color)
  : mCyan(color.mCyan),
    mMagenta(color.mMagenta),
    mYellow(color.mYellow),
    mKey(color.mKey),
    mRangeMin(color.mRangeMin),
    mRangeMax(color.mRangeMax)
{

}

ColorCMYK::ColorCMYK(ColorCMYK &&color) TL_NOEXCEPT
  : mCyan(std::exchange(color.mCyan, 0.)),
    mMagenta(std::exchange(color.mMagenta, 0.)),
    mYellow(std::exchange(color.mYellow, 0.)),
    mKey(std::exchange(color.mKey, 0.)),
    mRangeMin(std::exchange(color.mRangeMin, 0.)),
    mRangeMax(std::exchange(color.mRangeMax, 1.))
{
}

auto ColorCMYK::cyan() const -> double
{
    return mCyan;
}

void ColorCMYK::setCyan(double cyan)
{
    mCyan = cyan;
    this->adjustRangeCyan();
}

auto ColorCMYK::magenta() const -> double
{
    return mMagenta;
}

void ColorCMYK::setMagenta(double magenta)
{
    mMagenta = magenta;
    this->adjustRangeMagenta();
}

auto ColorCMYK::yellow() const -> double
{
    return mYellow;
}

void ColorCMYK::setYellow(double yellow)
{
    mYellow = yellow;
    this->adjustRangeYellow();
}

auto ColorCMYK::key() const -> double
{
    return mKey;
}

void ColorCMYK::setKey(double key)
{
  mKey = key;
  this->adjustRangeKey();
}

auto ColorCMYK::operator =(const ColorCMYK& color) -> ColorCMYK&
{
    if (this != &color) {
        this->mCyan = color.mCyan;
        this->mMagenta = color.mMagenta;
        this->mYellow = color.mYellow;
        this->mKey = color.mKey;
        this->mRangeMin = color.mRangeMin;
        this->mRangeMax = color.mRangeMax;
    }

    return *this;
}

auto ColorCMYK::operator=(ColorCMYK&& color) noexcept -> ColorCMYK&
{
    if (this != &color) {
        this->mCyan = std::exchange(color.mCyan, 0.);
        this->mMagenta = std::exchange(color.mMagenta, 0.);
        this->mYellow = std::exchange(color.mYellow, 0.);
        this->mKey = std::exchange(color.mKey, 0.);
        this->mRangeMin = std::exchange(color.mRangeMin, 0.);
        this->mRangeMax = std::exchange(color.mRangeMax, 1.);
    }

    return *this;
}

void ColorCMYK::setRange(double min, double max)
{
    mRangeMin = min;
    mRangeMax = max;
}

auto ColorCMYK::toColor() const -> Color
{
    ///TODO: Tener en cuenta que se puede modificar el rango a 0-100. Hay que normalizar en función de esto
    double aux = (1 - mKey) * 255;
    int red = roundToInteger((1 - mCyan) * aux);
    int green = roundToInteger((1 - mMagenta) * aux);
    int blue = roundToInteger((1 - mYellow) * aux);
    Color color((blue & 0xFF) |
                ((green << 8) & 0xFF00) |
                ((red << 16) & 0xFF0000));
    return color;
}

void ColorCMYK::fromColor(const Color &color)
{
    ///TODO: Tener en cuenta que se puede modificar el rango a 0-100. Hay que normalizar en función de esto
    double rgb[3] = {color.red() / 255., color.green() / 255., color.blue() / 255.};
    double max = *std::max_element(rgb, rgb + 3);
    mKey = 1. - max;
    if (mKey == 1.) {
        mCyan = mMagenta = mYellow = 0.;
    } else {
        mCyan = 1. - rgb[0] / max;
        mMagenta = 1. - rgb[1] / max;
        mYellow = 1. - rgb[2] / max;
    }
}

void ColorCMYK::adjustRangeCyan()
{
    if (mCyan < mRangeMin) mCyan = mRangeMin;
    else if (mCyan > mRangeMax) mCyan = mRangeMax;
}

void ColorCMYK::adjustRangeMagenta()
{
    if (mMagenta < mRangeMin) mMagenta = mRangeMin;
    else if (mMagenta > mRangeMax) mMagenta = mRangeMax;
}

void ColorCMYK::adjustRangeYellow()
{
    if (mYellow < mRangeMin) mYellow = mRangeMin;
    else if (mYellow > mRangeMax) mYellow = mRangeMax;
}

void ColorCMYK::adjustRangeKey()
{
    if (mKey < mRangeMin) mKey = mRangeMin;
    else if (mKey > mRangeMax) mKey = mRangeMax;
}


} // End namespace tl
