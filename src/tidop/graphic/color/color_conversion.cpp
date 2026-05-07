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

#include "tidop/graphic/color/color_conversion.h"
#include "tidop/core/base/type_conversions.h"

#include <algorithm>
#include <array>
#include <sstream>
#include <string>

namespace tl
{

void adjustRangeRGBA(int *red, int *green, int *blue, int *alpha = nullptr)
{
    if (*red < 0) *red = 0;
    else if (*red > 255) *red = 255;
    if (*green < 0) *green = 0;
    else if (*green > 255) *green = 255;
    if (*blue < 0) *blue = 0;
    else if (*blue > 255) *blue = 255;
    if (alpha) {
        if (*alpha < 0) *alpha = 0;
        else if (*alpha > 255) *alpha = 255;
    }
}

void adjustRangeCMYK(double *cyan, double *magenta, double *yellow, double *key)
{
    if (*cyan < 0.) *cyan = 0.;
    else if (*cyan > 1.) *cyan = 1.;
    if (*magenta < 0.) *magenta = 0.;
    else if (*magenta > 1) *magenta = 1.;
    if (*yellow < 0.) *yellow = 0.;
    else if (*yellow > 1.) *yellow = 1.;
    if (*key < 0.) *key = 0.;
    else if (*key > 1.) *key = 1.;
}

void adjustRangeHSV(double *hue, double *saturation, double *value)
{
    if (*hue < 0) *hue = 0.;
    else if (*hue >= 360.) *hue = 359.;
    if (*saturation < 0.) *saturation = 0.;
    else if (*saturation > 100.) *saturation = 100.;
    if (*value < 0.) *value = 0.;
    else if (*value > 100.) *value = 100.;
}

void adjustRangeHSL(double *hue, double *saturation, double *lightness)
{
    if (*hue < 0) *hue = 0.;
    if (*saturation < 0.) *saturation = 0.;
    if (*lightness < 0.) *lightness = 0.;
    if (*hue >= 360.) *hue = 359.;
    if (*saturation > 100.) *saturation = 100.;
    if (*lightness > 100.) *lightness = 100.;
}


int rgbToInt(int red, int green, int blue)
{
    adjustRangeRGBA(&red, &green, &blue);
    return (blue & 0xFF) | ((green << 8) & 0xFF00) | ((red << 16) & 0xFF0000);
}

int rgbaToInt(int red, int green, int blue, int alpha)
{
    adjustRangeRGBA(&red, &green, &blue, &alpha);
    return (blue & 0xFF) | ((green << 8) & 0xFF00) | ((red << 16) & 0xFF0000) | ((alpha << 24) & 0xFF000000);
}

int hexToInt(const std::string &colorhex)
{
    ///TODO: error inprevisto en test
    //mColor = static_cast<uint32_t>(stringToInteger(color, TL::Base::HEXADECIMAL));
    return std::stoi(colorhex, nullptr, 16);
}

std::string intToHex(int color)
{
    std::stringstream stream;
    stream << std::hex << color;
    return std::string(stream.str());
}

void rgbToCmyk(int red, int green, int blue, double *cyan, double *magenta, double *yellow, double *key)
{
    adjustRangeRGBA(&red, &green, &blue);
    double rgb[3] = {red / 255., green / 255., blue / 255.};
    double max = *std::max_element(rgb, rgb + 3);
    *key = 1. - max;
    if (*key == 1.) {
        *cyan = *magenta = *yellow = 0.;
    } else {
        *cyan = 1. - rgb[0] / max;
        *magenta = 1. - rgb[1] / max;
        *yellow = 1. - rgb[2] / max;
    }
}


void cmykToRgb(double cyan, double magenta, double yellow, double key, int *red, int *green, int *blue)
{
    adjustRangeCMYK(&cyan, &magenta, &yellow, &key);
    double aux = (1 - key) * 255;
    *red = roundToInteger((1 - cyan) * aux);
    *green = roundToInteger((1 - magenta) * aux);
    *blue = roundToInteger((1 - yellow) * aux);
}


void rgbToHSL(int red, int green, int blue, double *hue, double *saturation, double *lightness)
{
    adjustRangeRGBA(&red, &green, &blue);
    double rgb[3] = {red / 255., green / 255., blue / 255.};

    double max = *std::max_element(rgb, rgb + 3);
    double min = *std::min_element(rgb, rgb + 3);
    double delta = max - min;

    if (delta > 0.) {
        if (max == rgb[0])
            *hue = 60. * (fmod(((rgb[1] - rgb[2]) / delta), 6.));
        else if (max == rgb[1])
            *hue = 60. * (((rgb[2] - rgb[0]) / delta) + 2.);
        else
            *hue = 60. * (((rgb[0] - rgb[1]) / delta) + 4.);
    } else {
        *hue = 0.;
    }

    if (*hue < 0.) *hue += 360.;

    *lightness = (max + min) / 2;

    if (delta == 0)
        *saturation = 0.;
    else
        *saturation = delta / (1 - std::abs(2 * *lightness - 1));

    *saturation *= 100;
    *lightness *= 100;
}


void hslToRgb(double hue, double saturation, double lightness, int *red, int *green, int *blue)
{
    adjustRangeHSL(&hue, &saturation, &lightness);

    lightness /= 100.;
    saturation /= 100.;
    double chroma = (1 - std::abs(2 * lightness - 1)) * saturation;
    double h = hue / 60.;
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

    double m = lightness - chroma / 2;
    *red = roundToInteger((_rgb[0] + m) * 255);
    *green = roundToInteger((_rgb[1] + m) * 255);
    *blue = roundToInteger((_rgb[2] + m) * 255);
}


void rgbToHSV(int red, int green, int blue, double *hue, double *saturation, double *value)
{
    adjustRangeRGBA(&red, &green, &blue);
    double rgb[3] = {red / 255., green / 255., blue / 255.};
    double max = *std::max_element(rgb, rgb + 3);
    double min = *std::min_element(rgb, rgb + 3);
    double delta = max - min;

    if (delta > 0.) {
        if (max == rgb[0])
            *hue = 60. * (fmod(((rgb[1] - rgb[2]) / delta), 6.));
        else if (max == rgb[1])
            *hue = 60. * (((rgb[2] - rgb[0]) / delta) + 2.);
        else
            *hue = 60. * (((rgb[0] - rgb[1]) / delta) + 4.);
    } else {
        *hue = 0.;
    }

    if (*hue < 0.) *hue += 360.;

    *value = max * 100.;

    if (max == 0)
        *saturation = 0.;
    else
        *saturation = (delta / max) * 100;
}


void hsvToRgb(double hue, double saturation, double value, int *red, int *green, int *blue)
{
    adjustRangeHSV(&hue, &saturation, &value);

    value /= 100.;
    saturation /= 100.;
    double chroma = value * saturation;
    double h = hue / 60.;
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

    double m = value - chroma;

    *red = roundToInteger((_rgb[0] + m) * 255);
    *green = roundToInteger((_rgb[1] + m) * 255);
    *blue = roundToInteger((_rgb[2] + m) * 255);
}


int rgbToLuminance(int red, int green, int blue)
{
    //TODO: permitir diferentes tipos de conversión
    // Poynton	       0.2125	0.7154	0.0721
    // sRGB proposal   0.2126	0.7152	0.0722
    // W3              0.2126	0.7152	0.0722
    return roundToInteger(0.2126 * red + 0.7152 * green + 0.0722 * blue);
}


void chromaticityCoordinates(int red, int green, int blue, double *r, double *g, double *b)
{
    adjustRangeRGBA(&red, &green, &blue);
    double sum = static_cast<double>(red)
        + static_cast<double>(green)
        + static_cast<double>(blue);
    *r = red / sum;
    *g = green / sum;
    *b = blue / sum;
}


//void ColorConverter::convert(const ColorRGB &rgb, ColorRGBA &rgba)
//{
//    rgba.fromColor(rgb.toColor());
//}
//
//void ColorConverter::convert(const ColorRGB &rgb, ColorCMYK &cmyk)
//{
//    cmyk.fromColor(rgb.toColor());
//}
//
//void ColorConverter::convert(const ColorRGB &rgb, ColorHSL &hsl)
//{
//    hsl.fromColor(rgb.toColor());
//}
//
//void ColorConverter::convert(const ColorRGB &rgb, ColorHSV &hsv)
//{
//    hsv.fromColor(rgb.toColor());
//}
//
//void ColorConverter::convert(const ColorRGBA &rgba, ColorRGB &rgb)
//{
//    rgb.fromColor(rgba.toColor());
//}
//
//void ColorConverter::convert(const ColorRGBA& rgba, ColorCMYK& cmyk)
//{
//    cmyk.fromColor(rgba.toColor());
//}
//
//void ColorConverter::convert(const ColorRGBA& rgba, ColorHSL& hsl)
//{
//    hsl.fromColor(rgba.toColor());
//}
//
//void ColorConverter::convert(const ColorRGBA& rgba, ColorHSV& hsv)
//{
//    hsv.fromColor(rgba.toColor());
//}
//
//void ColorConverter::convert(const ColorCMYK& cmyk, ColorRGB& rgb)
//{
//    rgb.fromColor(cmyk.toColor());
//}
//
//void ColorConverter::convert(const ColorCMYK& cmyk, ColorRGBA& rgba)
//{
//    rgba.fromColor(cmyk.toColor());
//}
//
//void ColorConverter::convert(const ColorCMYK& cmyk, ColorHSL& hsl)
//{
//    hsl.fromColor(cmyk.toColor());
//}
//
//void ColorConverter::convert(const ColorCMYK& cmyk, ColorHSV& hsv)
//{
//    hsv.fromColor(cmyk.toColor());
//}
//
//void ColorConverter::convert(const ColorHSL& hsl, ColorRGB& rgb)
//{
//    rgb.fromColor(hsl.toColor());
//}
//
//void ColorConverter::convert(const ColorHSL& hsl, ColorRGBA& rgba)
//{
//    rgba.fromColor(hsl.toColor());
//}
//
//void ColorConverter::convert(const ColorHSL& hsl, ColorCMYK& cmyk)
//{
//    cmyk.fromColor(hsl.toColor());
//}
//
//void ColorConverter::convert(const ColorHSL& hsl, ColorHSV& hsv)
//{
//    hsv.fromColor(hsl.toColor());
//}
//
//void ColorConverter::convert(const ColorHSV& hsv, ColorRGB& rgb)
//{
//    rgb.fromColor(hsv.toColor());
//}
//
//void ColorConverter::convert(const ColorHSV& hsv, ColorRGBA& rgba)
//{
//    rgba.fromColor(hsv.toColor());
//}
//
//void ColorConverter::convert(const ColorHSV& hsv, ColorCMYK& cmyk)
//{
//    cmyk.fromColor(hsv.toColor());
//}
//
//void ColorConverter::convert(const ColorHSV& hsv, ColorHSL& hsl)
//{
//    hsl.fromColor(hsv.toColor());
//}

} // End namespace tl
