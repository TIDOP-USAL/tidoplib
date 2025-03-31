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

#include "tidop/graphic/color/hsv.h"
#include "tidop/core/base/type_conversions.h"

#include <array>
#include <utility>

namespace tl
{

ColorHSV::ColorHSV()
  : mHue(0.),
    mSaturation(0.),
    mValue(0.),
    mRangeMinHue(0.),
    mRangeMaxHue(360.),
    mRangeMin(0.),
    mRangeMax(100.)
{

}

ColorHSV::ColorHSV(double hue, double saturation, double value)
  : mHue(hue),
    mSaturation(saturation),
    mValue(value),
    mRangeMinHue(0.),
    mRangeMaxHue(360.),
    mRangeMin(0.),
    mRangeMax(100.)
{
    this->adjustRangeHue();
    this->adjustRangeSaturation();
    this->adjustRangeValue();
}

ColorHSV::ColorHSV(const ColorHSV &color)
  : mHue(color.mHue),
    mSaturation(color.mSaturation),
    mValue(color.mValue),
    mRangeMinHue(color.mRangeMinHue),
    mRangeMaxHue(color.mRangeMaxHue),
    mRangeMin(color.mRangeMin),
    mRangeMax(color.mRangeMax)
{

}

ColorHSV::ColorHSV(ColorHSV &&color) TL_NOEXCEPT
  : mHue(std::exchange(color.mHue, 0.)),
    mSaturation(std::exchange(color.mSaturation, 0.)),
    mValue(std::exchange(color.mValue, 0.)),
    mRangeMinHue(std::exchange(color.mRangeMinHue, 0.)),
    mRangeMaxHue(std::exchange(color.mRangeMaxHue, 360.)),
    mRangeMin(std::exchange(color.mRangeMin, 0.)),
    mRangeMax(std::exchange(color.mRangeMax, 100.))
{
}

auto ColorHSV::hue() const -> double
{
    return mHue;
}

void ColorHSV::setHue(double hue)
{
    mHue = hue;
    this->adjustRangeHue();
}

auto ColorHSV::saturation() const -> double
{
    return mSaturation;
}

void ColorHSV::setSaturation(double saturation)
{
    mSaturation = saturation;
    this->adjustRangeSaturation();
}

auto ColorHSV::value() const -> double
{
    return mValue;
}

void ColorHSV::setValue(double value)
{
    mValue = value;
    this->adjustRangeValue();
}

auto ColorHSV::operator =(const ColorHSV& color) -> ColorHSV&
{
    if (this != &color) {
        this->mHue = color.mHue;
        this->mSaturation = color.mSaturation;
        this->mValue = color.mValue;
        this->mRangeMinHue = color.mRangeMinHue;
        this->mRangeMaxHue = color.mRangeMaxHue;
        this->mRangeMin = color.mRangeMin;
        this->mRangeMax = color.mRangeMax;
    }

    return *this;
}

auto ColorHSV::operator=(ColorHSV&& color) TL_NOEXCEPT -> ColorHSV&
{
    if (this != &color) {
        this->mHue = std::exchange(color.mHue, 0.);
        this->mSaturation = std::exchange(color.mSaturation, 0.);
        this->mValue = std::exchange(color.mValue, 0.);
        this->mRangeMinHue = std::exchange(color.mRangeMinHue, 0.);
        this->mRangeMaxHue = std::exchange(color.mRangeMaxHue, 360.);
        this->mRangeMin = std::exchange(color.mRangeMin, 0.);
        this->mRangeMax = std::exchange(color.mRangeMax, 100.);
    }

    return *this;
}

auto ColorHSV::toColor() const -> Color
{
    double norm_value = mValue / mRangeMax;
    double norm_saturation = mSaturation / mRangeMax;
    double chroma = norm_value * norm_saturation;
    double h = mHue / (mRangeMaxHue / 6.);
    double x = chroma * (1 - fabs(fmod(h, 2) - 1));

    std::array<double, 3> _rgb = {0., 0., 0.};

    if (h >= 0 && h < 1) {
        _rgb[0] = chroma;
        _rgb[1] = x;
    } else if (h >= 1 && h < 2) {
        _rgb[0] = x;
        _rgb[1] = chroma;
    } else if (h >= 2 && h < 3) {
        _rgb[1] = chroma;
        _rgb[2] = x;
    } else if (h >= 3 && h < 4) {
        _rgb[1] = x;
        _rgb[2] = chroma;
    } else if (h >= 4 && h < 5) {
        _rgb[0] = x;
        _rgb[2] = chroma;
    } else {
        _rgb[0] = chroma;
        _rgb[2] = x;
    }

    double m = norm_value - chroma;

    int red = roundToInteger((_rgb[0] + m) * 255);
    int green = roundToInteger((_rgb[1] + m) * 255);
    int blue = roundToInteger((_rgb[2] + m) * 255);
    Color color((blue & 0xFF) |
                ((green << 8) & 0xFF00) |
                ((red << 16) & 0xFF0000));
    return color;
}

void ColorHSV::fromColor(const Color &color)
{
    double rgb[3] = {color.red() / 255., color.green() / 255., color.blue() / 255.};
    double max = *std::max_element(rgb, rgb + 3);
    double min = *std::min_element(rgb, rgb + 3);
    double delta = max - min;

    if (delta > 0.) {
        if (max == rgb[0])
            mHue = mRangeMaxHue / 6. * (fmod(((rgb[1] - rgb[2]) / delta), 6.));
        else if (max == rgb[1])
            mHue = mRangeMaxHue / 6. * (((rgb[2] - rgb[0]) / delta) + 2.);
        else
            mHue = mRangeMaxHue / 6. * (((rgb[0] - rgb[1]) / delta) + 4.);
    } else {
        mHue = 0.;
    }

    if (mHue < 0.) mHue += mRangeMaxHue;

    mValue = max * mRangeMax;

    if (max == 0)
        mSaturation = 0.;
    else
        mSaturation = (delta / max) * mRangeMax;
}

void ColorHSV::adjustRangeHue()
{
    if (mHue < mRangeMinHue) mHue = mRangeMinHue;
    else if (mHue >= mRangeMaxHue) mHue = mRangeMaxHue;
}

void ColorHSV::adjustRangeSaturation()
{
    if (mSaturation < mRangeMin) mSaturation = mRangeMin;
    else if (mSaturation > mRangeMax) mSaturation = mRangeMax;
}

void ColorHSV::adjustRangeValue()
{
    if (mValue < mRangeMin) mValue = mRangeMin;
    else if (mValue > mRangeMax) mValue = mRangeMax;
}

} // End namespace tl
