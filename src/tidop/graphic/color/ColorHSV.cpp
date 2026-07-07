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

#include "tidop/graphic/color/ColorHSV.h"
#include "tidop/core/base/TypeConversions.h"

#include <array>
#include <utility>

namespace tl
{

auto ColorHSV::toColor() const -> Color
{
    double norm_value = mValue / 100.;
    double norm_saturation = mSaturation / 100.;
    double chroma = norm_value * norm_saturation;
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

    double m = norm_value - chroma;

    int red = std::clamp(roundToInteger((_rgb[0] + m) * 255), 0, 255);
    int green = std::clamp(roundToInteger((_rgb[1] + m) * 255), 0, 255);
    int blue = std::clamp(roundToInteger((_rgb[2] + m) * 255), 0, 255);

    Color color(static_cast<Color::Name>(blue | (green << 8) | (red << 16)));
    return color;
}

auto ColorHSV::fromColor(const Color &color) -> ColorHSV
{
    double r = color.red() / 255.0;
    double g = color.green() / 255.0;
    double b = color.blue() / 255.0;

    auto [min, max] = std::minmax({r, g, b});

    double delta = max - min;

    double hue = 0.;

    if (delta > 0.) {
        if (max == r)
            hue = 360. / 6. * (fmod(((g - b) / delta), 6.));
        else if (max == g)
            hue = 360. / 6. * (((b - r) / delta) + 2.);
        else
            hue = 360. / 6. * (((r - g) / delta) + 4.);
    }

    if (hue < 0.) hue += 360.;

    double value = max * 100.;

    double saturation = 0.;
    if (max != 0)
        saturation = (delta / max) * 100.;

    return ColorHSV(hue, saturation, value);
}

} // namespace tl
