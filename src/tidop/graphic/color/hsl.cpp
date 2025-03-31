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

#include "tidop/graphic/color/hsl.h"
#include "tidop/core/base/type_conversions.h"

#include <algorithm>
#include <array>
#include <utility>

namespace tl
{

ColorHSL::ColorHSL()
  : mHue(0.),
    mSaturation(0.),
    mLightness(0.),
    mRangeMinHue(0.),
    mRangeMaxHue(360.),
    mRangeMin(0.),
    mRangeMax(100.)
{

}

ColorHSL::ColorHSL(double hue, double saturation, double lightness)
  : mHue(hue),
    mSaturation(saturation),
    mLightness(lightness),
    mRangeMinHue(0.),
    mRangeMaxHue(360.),
    mRangeMin(0.),
    mRangeMax(100.)
{
    this->adjustRangeHue();
    this->adjustRangeSaturation();
    this->adjustRangeLightness();
}

ColorHSL::ColorHSL(const ColorHSL &color)
  : mHue(color.mHue),
    mSaturation(color.mSaturation),
    mLightness(color.mLightness),
    mRangeMinHue(color.mRangeMinHue),
    mRangeMaxHue(color.mRangeMaxHue),
    mRangeMin(color.mRangeMin),
    mRangeMax(color.mRangeMax)
{

}

ColorHSL::ColorHSL(ColorHSL&& color) TL_NOEXCEPT
  : mHue(std::exchange(color.mHue, 0.)),
    mSaturation(std::exchange(color.mSaturation, 0.)),
    mLightness(std::exchange(color.mLightness, 0.)),
    mRangeMinHue(std::exchange(color.mRangeMinHue, 0.)),
    mRangeMaxHue(std::exchange(color.mRangeMaxHue, 360.)),
    mRangeMin(std::exchange(color.mRangeMin, 0.)),
    mRangeMax(std::exchange(color.mRangeMax, 100.))
{
}

auto ColorHSL::hue() const -> double
{
    return mHue;
}

void ColorHSL::setHue(double hue)
{
    mHue = hue;
    this->adjustRangeHue();
}

auto ColorHSL::saturation() const -> double
{
    return mSaturation;
}

void ColorHSL::setSaturation(double saturation)
{
    mSaturation = saturation;
    this->adjustRangeSaturation();
}

auto ColorHSL::lightness() const -> double
{
    return mLightness;
}

void ColorHSL::setLightness(double lightness)
{
    mLightness = lightness;
    this->adjustRangeLightness();
}

auto ColorHSL::operator =(const ColorHSL& color) -> ColorHSL&
{
    if (this != &color) {
        this->mHue = color.mHue;
        this->mSaturation = color.mSaturation;
        this->mLightness = color.mLightness;
        this->mRangeMinHue = color.mRangeMinHue;
        this->mRangeMaxHue = color.mRangeMaxHue;
        this->mRangeMin = color.mRangeMin;
        this->mRangeMax = color.mRangeMax;
    }

    return *this;
}

auto ColorHSL::operator=(ColorHSL &&color) noexcept -> ColorHSL&
{
    if (this != &color) {
        this->mHue = std::exchange(color.mHue, 0.);
        this->mSaturation = std::exchange(color.mSaturation, 0.);
        this->mLightness = std::exchange(color.mLightness, 0.);
        this->mRangeMinHue = std::exchange(color.mRangeMinHue, 0.);
        this->mRangeMaxHue = std::exchange(color.mRangeMaxHue, 360.);
        this->mRangeMin = std::exchange(color.mRangeMin, 0.);
        this->mRangeMax = std::exchange(color.mRangeMax, 100.);
    }

    return *this;
}

auto ColorHSL::toColor() const -> Color
{
    double lightness_norm = mLightness / mRangeMax;
    double saturation_norm = mSaturation / mRangeMax;
    double chroma = (1 - std::abs(2 * lightness_norm - 1)) * saturation_norm;
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

    double m = lightness_norm - chroma / 2;
    int red = roundToInteger((_rgb[0] + m) * 255);
    int green = roundToInteger((_rgb[1] + m) * 255);
    int blue = roundToInteger((_rgb[2] + m) * 255);

    Color color((blue & 0xFF) |
                ((green << 8) & 0xFF00) |
                ((red << 16) & 0xFF0000));
    return color;
}

void ColorHSL::fromColor(const Color &color)
{
    double rgb[3] = {color.red() / 255., color.green() / 255., color.blue() / 255.};

    double max = *std::max_element(rgb, rgb + 3);
    double min = *std::min_element(rgb, rgb + 3);
    double delta = max - min;

    if (delta > 0.) {
        if (max == rgb[0])
            mHue = 60. * (fmod(((rgb[1] - rgb[2]) / delta), 6.));
        else if (max == rgb[1])
            mHue = 60. * (((rgb[2] - rgb[0]) / delta) + 2.);
        else
            mHue = 60. * (((rgb[0] - rgb[1]) / delta) + 4.);
    } else {
        mHue = 0.;
    }

    if (mHue < 0.) mHue += mRangeMaxHue;

    mLightness = (max + min) / 2;

    if (delta == 0)
        mSaturation = 0.;
    else
        mSaturation = delta / (1 - std::abs(2 * mLightness - 1));

    mSaturation *= mRangeMax;
    mLightness *= mRangeMax;
}

void ColorHSL::adjustRangeHue()
{
    if (mHue < mRangeMinHue) mHue = mRangeMinHue;
    else if (mHue >= mRangeMaxHue) mHue = mRangeMaxHue;
}

void ColorHSL::adjustRangeSaturation()
{
    if (mSaturation < mRangeMin) mSaturation = mRangeMin;
    else if (mSaturation > mRangeMax) mSaturation = mRangeMax;
}

void ColorHSL::adjustRangeLightness()
{
    if (mLightness < mRangeMin) mLightness = mRangeMin;
    else if (mLightness > mRangeMax) mLightness = mRangeMax;
}

} // End namespace tl
