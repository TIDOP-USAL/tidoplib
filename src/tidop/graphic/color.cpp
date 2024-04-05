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

#include "color.h"

#include <algorithm>
#include <array>
#include <thread>
#include <random>
#include <string>

#include "tidop/core/exception.h"
#include "tidop/core/utils.h"

#ifdef TL_HAVE_OPENCV
#include "opencv2/imgproc/imgproc.hpp"
#endif // TL_HAVE_OPENCV

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



/* ---------------------------------------------------------------------------------- */
/*                                Conversión de color                                 */
/* ---------------------------------------------------------------------------------- */


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


//bool operator == (const Color &color1, const Color &color2)
//{
//  return static_cast<int>(color1) == static_cast<int>(color2);
//}

//bool operator != (const Color &color1, const Color &color2)
//{
//  return static_cast<int>(color1) != static_cast<int>(color2);
//}



ColorSpace::ColorSpace()
{
}

ColorSpace::~ColorSpace()
{
}





Color::Color()
  : mColor(0)
{
}

Color::Color(const Color &color)
  : mColor(color.mColor),
    mOpacity(color.mOpacity),
    mHasOpacity(color.mHasOpacity)
{

}

Color::Color(Color &&color) TL_NOEXCEPT
  : mColor(std::exchange(color.mColor, 0)),
    mOpacity(std::exchange(color.mOpacity, 255)),
    mHasOpacity(std::exchange(color.mHasOpacity, false))
{
}

Color::Color(uint32_t color)
  : mColor(color)
{

}

Color::Color(const std::string &color)
{
    mColor = std::stoi(color, nullptr, 16);
}

Color::Color(const Color::Name &color)
  : mColor(static_cast<uint32_t>(color))
{
}

Color::Color(const ColorModel &colorModel)
{
    *this = colorModel.toColor();
}

Color::~Color() = default;

auto Color::blue() const -> int
{
    return mColor & 0xFF;
}

auto Color::green() const -> int
{
    return (mColor & 0xFF00) >> 8;
}

auto Color::red() const -> int
{
    return (mColor & 0xFF0000) >> 16;
}

auto Color::opacity() const -> uint8_t
{
    return mOpacity/*(mColor & 0xFF000000) >> 24*/;
}

void Color::setOpacity(uint8_t opacity)
{
    mOpacity = opacity;
}

auto Color::luminance() const -> int
{
    return roundToInteger(0.2126 * red() + 0.7152 * green() + 0.0722 * blue());
}

auto Color::toHex() const -> std::string
{
    std::stringstream stream;
    stream << std::hex << mColor;
    return std::string(stream.str());
}

auto Color::randomColor() -> Color
{
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(0, 16777216);
    return Color(dis(gen));
}

auto Color::operator =(const Color &color) -> Color &
{
    if (this != &color) {
        mColor = color.mColor;
        mOpacity = color.mOpacity;
        mHasOpacity = color.mHasOpacity;
    }

    return *this;
}

auto Color::operator=(Color &&color) TL_NOEXCEPT -> Color&
{
    if (this != &color) {
        mColor = std::exchange(color.mColor, 0);
        mOpacity = std::exchange(color.mOpacity, 255);
        mHasOpacity = std::exchange(color.mHasOpacity, false);
    }

    return *this;
}

auto operator ==(const Color& color1, const Color& color2) -> bool
{
    return static_cast<int>(color1) == static_cast<int>(color2);
}

auto operator !=(const Color& color1, const Color& color2) -> bool
{
    return static_cast<int>(color1) != static_cast<int>(color2);
}





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


/* ---------------------------------------------------------------------------------- */


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
