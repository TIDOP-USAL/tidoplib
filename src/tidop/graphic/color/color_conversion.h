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

#pragma once

#include "tidop/core/base/defs.h"

#include <string>

#include "tidop/graphic/color/cmyk.h"
#include "tidop/graphic/color/rgb.h"
#include "tidop/graphic/color/rgba.h"
#include "tidop/graphic/color/hsv.h"
#include "tidop/graphic/color/hsl.h"

namespace tl
{


/*! \addtogroup ColorConversion
 * \{
 */


/*!
 * \brief Converts an integer color to RGB
 * \param[in] color Integer color
 * \param[out] red Red component
 * \param[out] green Green component
 * \param[out] blue Blue component
 */
TL_EXPORT void intToRGB(int color, int *red, int *green, int *blue);


/*!
 * \brief Converts an RGB color to an integer
 * \param[in] red Red component
 * \param[in] green Green component
 * \param[in] blue Blue component
 * \return Color as an integer
 */
TL_EXPORT int rgbToInt(int red, int green, int blue);


/*!
 * \brief Converts an RGB+alpha color to an integer
 * \param[in] red Red component
 * \param[in] green Green component
 * \param[in] blue Blue component
 * \param[in] alpha Alpha channel
 * \return Color as an integer
 */
TL_EXPORT int rgbaToInt(int red, int green, int blue, int alpha);


/*!
 * \brief Converts a hexadecimal color (as a string) to an integer
 * \param[in] colorhex Color in hexadecimal
 * \return Color as an integer
 */
TL_EXPORT int hexToInt(const std::string &colorhex);


/*!
 * \brief Converts an integer color to hexadecimal
 * \param[in] color Color as an integer
 * \return Color in hexadecimal
 */
TL_EXPORT std::string intToHex(int color);


/*!
 * \brief Converts an RGB color to CMYK
 * \param[in] red Red component
 * \param[in] green Green component
 * \param[in] blue Blue component
 * \param[out] cyan Cyan
 * \param[out] magenta Magenta
 * \param[out] yellow Yellow
 * \param[out] key Black
 */
TL_EXPORT void rgbToCmyk(int red, int green, int blue, double *cyan, double *magenta, double *yellow, double *key);


/*!
 * \brief Converts a CMYK color to RGB
 * \param[in] cyan Cyan
 * \param[in] magenta Magenta
 * \param[in] yellow Yellow
 * \param[in] key Black
 * \param[out] red Red component
 * \param[out] green Green component
 * \param[out] blue Blue component
 */
TL_EXPORT void cmykToRgb(double cyan, double magenta, double yellow, double key, int *red, int *green, int *blue);


/*!
 * \brief Converts an RGB color to HSL
 * \param[in] red Red component
 * \param[in] green Green component
 * \param[in] blue Blue component
 * \param[out] hue Hue
 * \param[out] saturation Saturation
 * \param[out] lightness Lightness
 */
TL_EXPORT void rgbToHSL(int red, int green, int blue, double *hue, double *saturation, double *lightness);


/*!
 * \brief Converts a color from HSL to RGB
 * \param[in] hue Hue
 * \param[in] saturation Saturation
 * \param[in] lightness Lightness
 * \param[out] red Red component
 * \param[out] green Green component
 * \param[out] blue Blue component
 */
TL_EXPORT void hslToRgb(double hue, double saturation, double lightness, int *red, int *green, int *blue);


/*!
 * \brief Convierte un color de RGB a HSV
 * \param[in] red Componente roja
 * \param[in] green Componente verde
 * \param[in] blue Componente azul
 * \param[out] hue Matiz
 * \param[out] saturation Saturación
 * \param[out] value Valor
 */
TL_EXPORT void rgbToHSV(int red, int green, int blue, double *hue, double *saturation, double *value);


/*!
 * \brief Converts an RGB color to HSV
 * \param[in] red Red component
 * \param[in] green Green component
 * \param[in] blue Blue component
 * \param[out] hue Hue
 * \param[out] saturation Saturation
 * \param[out] value Value
 */
TL_EXPORT void hsvToRgb(double hue, double saturation, double value, int *red, int *green, int *blue);


/*!
 * \brief Converts an RGB color to grayscale
 * \param[in] red Red component
 * \param[in] green Green component
 * \param[in] blue Blue component
 * \return Luminance
 */
TL_EXPORT int rgbToLuminance(int red, int green, int blue);


/*!
 * \brief Conversion to chromaticity coordinates
 *
 * When an image exhibits changes in intensity, dimmer light, etc., one solution
 * involves eliminating the effects of intensity in the image. This is achieved by
 * converting to a color space invariant to intensity, such as chromaticity coordinates.
 * Chromaticity specifies the hue and saturation of a color but not its luminance.
 * Conversion to chromaticity coordinates (division by Intensity)
 * (R, G, B) -> (R/(R+G+B), G/(R+G+B), B/(R+G+B))
 *
 * A change in intensity in the image is a scalar product
 * (R, G, B) -> s.(R, G, B) -> (s.R, s.G, s.B)
 *
 * Intensity cancels out and the new descriptor is invariant to intensity
 * The new descriptor is of dimension 2, it's a projection onto the plane:
 *
 * R +G + B = 1
 *
 * https://engineering.purdue.edu/~bouman/ece637/notes/pdf/ColorSpaces.pdf
 *
 * \param[in] red Red component
 * \param[in] green Green component
 * \param[in] blue Blue component
 * \param[out] r Red component in chromaticity coordinates
 * \param[out] g Green component in chromaticity coordinates
 * \param[out] b Blue component in chromaticity coordinates
 */
TL_EXPORT void chromaticityCoordinates(int red, int green, int blue, double *r, double *g, double *b);



template<typename Color1, typename Color2>
auto colorConvert(const Color1 &color1, Color2 &color2) ->
    std::enable_if_t<std::is_base_of_v<ColorModel, Color1> &&
                     std::is_base_of_v<ColorModel, Color2>>
{
    color2.fromColor(color1.toColor());
}

//class ColorConverter
//{
//
//private:
//
//    ColorConverter() = default;
//
//public:
//
//    static void convert(const ColorRGB &rgb, ColorRGBA &rgba);
//    static void convert(const ColorRGB &rgb, ColorCMYK &cmyk);
//    static void convert(const ColorRGB &rgb, ColorHSL &hsl);
//    static void convert(const ColorRGB &rgb, ColorHSV &hsv);
//
//    static void convert(const ColorRGBA &rgba, ColorRGB &rgb);
//    static void convert(const ColorRGBA &rgba, ColorCMYK &cmyk);
//    static void convert(const ColorRGBA &rgba, ColorHSL &hsl);
//    static void convert(const ColorRGBA &rgba, ColorHSV &hsv);
//
//    static void convert(const ColorCMYK &cmyk, ColorRGB &rgb);
//    static void convert(const ColorCMYK &cmyk, ColorRGBA &rgba);
//    static void convert(const ColorCMYK &cmyk, ColorHSL &hsl);
//    static void convert(const ColorCMYK &cmyk, ColorHSV &hsv);
//
//    static void convert(const ColorHSL &hsl, ColorRGB &rgb);
//    static void convert(const ColorHSL &hsl, ColorRGBA &rgba);
//    static void convert(const ColorHSL &hsl, ColorCMYK &cmyk);
//    static void convert(const ColorHSL &hsl, ColorHSV &hsv);
//
//    static void convert(const ColorHSV &hsv, ColorRGB &rgb);
//    static void convert(const ColorHSV &hsv, ColorRGBA &rgba);
//    static void convert(const ColorHSV &hsv, ColorCMYK &cmyk);
//    static void convert(const ColorHSV &hsv, ColorHSL &hsl);
//};



/*! \} */

} // End namespace tl
