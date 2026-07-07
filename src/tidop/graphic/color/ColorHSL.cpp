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

#include "tidop/graphic/color/ColorHSL.h"
#include "tidop/core/base/TypeConversions.h"

#include <algorithm>
#include <array>
#include <utility>

namespace tl
{

auto ColorHSL::toColor() const -> Color
{
    double lightness_norm = mLightness / 100.;
    double saturation_norm = mSaturation / 100.;
    double chroma = (1 - std::abs(2 * lightness_norm - 1)) * saturation_norm;
    double h = mHue / (360. / 6.);
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

    Color color(red, green, blue);
    return color;
}

auto ColorHSL::fromColor(const Color &color) -> ColorHSL
{
    double r = color.red() / 255.0;
    double g = color.green() / 255.0;
    double b = color.blue() / 255.0;

    auto [min, max] = std::minmax({r, g, b});

    double delta = max - min;

    double hue = 0.0;
    double saturation = 0.0;
    double lightness = 0.0;

    if (delta > 0.) {
        if (max == r)
            hue = 60. * (fmod(((g - b) / delta), 6.));
        else if (max == g)
            hue = 60. * (((b - r) / delta) + 2.);
        else
            hue = 60. * (((r - g) / delta) + 4.);
    } else {
        hue = 0.;
    }

    if (hue < 0.) hue += 360.;

    lightness = (max + min) / 2;

    if (delta == 0)
        saturation = 0.;
    else
        saturation = delta / (1 - std::abs(2 * lightness - 1));

    saturation *= 100.;
    lightness *= 100.;

    return ColorHSL(hue, saturation, lightness);    
}

} // namespace tl
