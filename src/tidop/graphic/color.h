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
 
 // Añadir generación de color aleatorio. util para pruebas

#pragma once

#include <string>
#include <typeinfo>
#include <functional>

#include "tidop/core/defs.h"
#include "tidop/core/exception.h"

namespace tl
{

/*! \addtogroup GraphicEntities
 *  \{
 */

/* ---------------------------------------------------------------------------------- */
/*                                Conversión de color                                 */
/* ---------------------------------------------------------------------------------- */



/*! \defgroup colorConversion Color conversion
 *  
 * Conversions between the most common color modules
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


/*! \} */ // end of colorConversion


//TL_EXPORT bool operator == (const Color &color1, const Color &color2);
//TL_EXPORT bool operator != (const Color &color1, const Color &color2);




/*!
 * \brief Color space class
 *
 */
class TL_EXPORT ColorSpace
{
public:

    ColorSpace();
    ~ColorSpace();

private:

};




class ColorModel;

/*!
 * \brief Class Color
 */
class TL_EXPORT Color
{

public:

    /*!
     * html color names
     * http://www.w3schools.com/colors/colors_names.asp
     */
    enum class Name
    {
        //  Color                     Hex	         (R,G,B)	      (H,S,V)	          (H,S,L)
        // ----------------------------------------------------------------------------------
    
        black =	                  0x000000,   // (0,0,0)        (0°,0%,0%)	      (0°,0%,0%)
        navy =	                  0x000080,   // (0,0,128)	    (240°,100%,50%)	  (240°,100%,25%)
        dark_blue =	              0x00008B,
        medium_blue =	            0x0000CD,
        blue =	                  0x0000FF,   // (0,0,255)      (240°,100%,100%)	(240°,100%,50%)
        dark_green =	            0x006400,
        green =	                  0x008000,   // (0,128,0)	    (120°,100%,50%)	  (120°,100%,25%)
        teal =	                  0x008080,   // (0,128,128)	  (180°,100%,50%)	  (180°,100%,25%)
        dark_cyan =	              0x008B8B,
        deep_sky_blue =	          0x00BFFF,
        dark_turquoise =	        0x00CED1,
        medium_spring_green =	    0x00FA9A,
        lime =	                  0x00FF00,   // (0,255,0)	    (120°,100%,100%)	(120°,100%,50%)
        spring_green =	          0x00FF7F,
        aqua =	                  0x00FFFF,
        cyan =	                  0x00FFFF,   // (0,255,255)	  (180°,100%,100%)	(180°,100%,50%)
        midnigh_tblue =	          0x191970,
        dodger_blue =	            0x1E90FF,
        light_sea_green =	        0x20B2AA,
        forest_green =	          0x228B22,
        sea_green =	              0x2E8B57,
        dark_slate_gray =	        0x2F4F4F,
        lime_green =	            0x32CD32,
        medium_sea_green =	      0x3CB371,
        turquoise =	              0x40E0D0,
        royal_blue =	            0x4169E1,
        steel_blue =	            0x4682B4,
        dark_slate_blue =	        0x483D8B,
        medium_turquoise =	      0x48D1CC,
        indigo = 	                0x4B0082,
        dark_olive_green =	      0x556B2F,
        cadet_blue =	            0x5F9EA0,
        cornflower_blue =	        0x6495ED,
        rebecca_purple =   	      0x663399,
        medium_aqua_marine =	    0x66CDAA,
        dim_gray =	              0x696969,
        slate_blue =	            0x6A5ACD,
        olive_drab =	            0x6B8E23,
        slate_gray =	            0x708090,
        light_slate_gray =	      0x778899,
        medium_slate_blue =	      0x7B68EE,
        lawn_green =	            0x7CFC00,
        chartreuse =	            0x7FFF00,
        aquamarine =	            0x7FFFD4,
        maroon =	                0x800000,   // (128,0,0)	    (0°,100%,50%)   	(0°,100%,25%)
        purple =	                0x800080,   // (128,0,128)	  (300°,100%,50%)	  (300°,100%,25%)
        olive =	                  0x808000,   // (128,128,0)	  (60°,100%,50%)	  (60°,100%,25%)
        gray =	                  0x808080,   // (128,128,128)	(0°,0%,50%)	      (0°,0%,50%)
        sky_blue =	              0x87CEEB,
        light_sky_blue =	        0x87CEFA,
        blue_violet =	            0x8A2BE2,
        dark_red =	              0x8B0000,
        dark_magenta =	          0x8B008B,
        saddle_brown =	          0x8B4513,
        dark_sea_green =	        0x8FBC8F,
        light_green =	            0x90EE90,
        medium_purple =	          0x9370DB,
        dark_violet =	            0x9400D3,
        pale_green =	            0x98FB98,
        dark_orchid =	            0x9932CC,
        yellow_green =	          0x9ACD32,
        sienna =	                0xA0522D,
        brown =   	              0xA52A2A,
        dark_gray =	              0xA9A9A9,
        light_blue =	            0xADD8E6,
        green_yellow =	          0xADFF2F,
        pale_turquoise =	        0xAFEEEE,
        light_steel_blue =	      0xB0C4DE,
        powder_blue =	            0xB0E0E6,
        fire_brick =	            0xB22222,
        dark_golden_rod =	        0xB8860B,
        medium_orchid =	          0xBA55D3,
        rosy_brown =	            0xBC8F8F,
        dark_khaki =	            0xBDB76B,
        silver =	                0xC0C0C0,   // (192,192,192)	(0°,0%,75%)	      (0°,0%,75%)
        medium_violet_red =	      0xC71585,
        indian_red = 	            0xCD5C5C,
        peru =	                  0xCD853F,
        chocolate =	              0xD2691E,
        tan =	                    0xD2B48C,
        light_gray =	            0xD3D3D3,
        thistle =	                0xD8BFD8,
        orchid =	                0xDA70D6,
        golden_rod =	            0xDAA520,
        pale_violet_red =	        0xDB7093,
        crimson =	                0xDC143C,
        gainsboro =	              0xDCDCDC,
        plum =	                  0xDDA0DD,
        burly_wood =	            0xDEB887,
        light_cyan =	            0xE0FFFF,
        lavender =	              0xE6E6FA,
        dark_salmon =	            0xE9967A,
        violet =	                0xEE82EE,
        pale_golden_rod =	        0xEEE8AA,
        light_coral =	            0xF08080,
        khaki =	                  0xF0E68C,
        alice_blue =	            0xF0F8FF,
        honey_dew =	              0xF0FFF0,
        azure =	                  0xF0FFFF,
        sandy_brown =	            0xF4A460,
        wheat =	                  0xF5DEB3,
        beige =	                  0xF5F5DC,
        white_smoke = 	          0xF5F5F5,
        mint_cream =	            0xF5FFFA,
        ghost_white =	            0xF8F8FF,
        salmon =	                0xFA8072,
        antique_white =	          0xFAEBD7,
        linen =	                  0xFAF0E6,
        light_golden_rod_yellow =	0xFAFAD2,
        old_lace =	              0xFDF5E6,
        red =	                    0xFF0000,   // (255,0,0)	    (0°,100%,100%)	  (0°,100%,50%)
        fuchsia =	                0xFF00FF,
        magenta =	                0xFF00FF,   // (255,0,255)	  (300°,100%,100%)	(300°,100%,50%)
        deep_pink =	              0xFF1493,
        orange_red =	            0xFF4500,
        tomato =	                0xFF6347,
        hot_pink =	              0xFF69B4,
        coral =	                  0xFF7F50,
        dark_orange =	            0xFF8C00,
        light_salmon =	          0xFFA07A,
        orange =	                0xFFA500,
        light_pink =	            0xFFB6C1,
        pink =	                  0xFFC0CB,
        gold =	                  0xFFD700,
        peach_puff =	            0xFFDAB9,
        navajo_white =	          0xFFDEAD,
        moccasin =	              0xFFE4B5,
        bisque =	                0xFFE4C4,
        misty_rose =	            0xFFE4E1,
        blanched_almond =	        0xFFEBCD,
        papaya_whip =	            0xFFEFD5,
        lavender_blush =	        0xFFF0F5,
        sea_shell =	              0xFFF5EE,
        cornsilk =	              0xFFF8DC,
        lemon_chiffon =	          0xFFFACD,
        floral_white =	          0xFFFAF0,
        snow =	                  0xFFFAFA,
        yellow =	                0xFFFF00,   // (255,255,0)	  (60°,100%,100%)	  (60°,100%,50%)
        light_yellow =	          0xFFFFE0,
        ivory =	                  0xFFFFF0,
        white = 	                0xFFFFFF    // (255,255,255)	(0°,0%,100%)	    (0°,0%,100%)
    };


public:

    /*!
     * \brief Default constructor
     */
    Color();
    
    /*!
     * \brief Copy constructor
     * \param[in] color Color object
     */
    Color(const Color &color);

    /*!
     * \brief Move constructor
     * \param[in] color Color object
     */
    Color(Color &&color) TL_NOEXCEPT;

    /*!
     * \brief Constructor
     * \param[in] color Color as an integer
     */
    explicit Color(uint32_t color);
    
    /*!
     * \brief Constructor
     * \param[in] color Color as a string (hexadecimal)
     */
    explicit Color(const std::string &color);
    
    /*!
     * \brief Constructor
     * \param[in] color Name of the color
     * \see Name
     */
    explicit Color(const Name &color);

    /*!
     * \brief Constructor
     * \param[in] colorModel Color model
     * \see Name
     */
    explicit Color(const ColorModel &colorModel);
    
    ~Color();
    
    /*!
     * \brief Returns the blue component
     * \return Blue component
     */
    auto blue() const -> int;
    
    /*!
     * \brief Returns the green component
     * \return Green component
     */
    auto green() const -> int;
    
    /*!
     * \brief Returns the red component
     * \return Red component
     */
    auto red() const -> int;
    
    /*!
     * \brief Returns the opacity
     * \return Opacity
     */
    auto opacity() const -> uint8_t;
    
    void setOpacity(uint8_t opacity);
    
    /*!
     * \brief Luminance value of a color as luminance
     * \return Luminance value
     */
    auto luminance() const -> int;
    
    /*!
     * \brief Converts a color to hexadecimal
     * \return String with the hexadecimal value of the color
     */
    auto toHex() const -> std::string;
    
    /*!
     * \brief Generates a random color
     */
    static auto randomColor() -> Color;
    
    operator unsigned int() const { return mColor; }
    operator int() const { return static_cast<int>(mColor); }
    
    /*!
     * \brief Assignment operator
     * \param color Color
     * \return Reference to the color
     */
    auto operator =(const Color& color) -> Color&;

    /*!
     * \brief Assignment move operator
     * \param color Color
     * \return Reference to the color
     */
    auto operator =(Color &&color) TL_NOEXCEPT -> Color&;

private:

    /*!
     * \brief Color
     */
    uint32_t mColor;
    uint8_t mOpacity{255};
    bool mHasOpacity{false};

};

TL_EXPORT bool operator == (const Color &color1, const Color &color2);
TL_EXPORT bool operator != (const Color &color1, const Color &color2);




/*!
 * \brief Interfaz modelo de color
 */
class TL_EXPORT ColorModel
{

public:

    ColorModel();

    virtual ~ColorModel();

    /*!
     * \brief toColor
     * \return
     */
    virtual auto toColor() const -> Color = 0;

    /*!
     * \brief fromColor
     * \param color
     */
    virtual void fromColor(const Color &color) = 0;
};



/*!
 * \brief RGB color model
 * The RGB color model is based on the additive synthesis of the three
 * primary colors, namely red, green, and blue.
 */
class TL_EXPORT ColorRGB 
  : public ColorModel
{

protected:

    int mRed;
    int mGreen;
    int mBlue;
    int mRangeMin;
    int mRangeMax;

public:

    /*!
     * \brief Default constructor for RGB color
     */
    ColorRGB();

    /*!
     * \brief Constructor for ColorRGB
     * \param[in] red Red component
     * \param[in] green Green component
     * \param[in] blue Blue component
     */
    ColorRGB(int red, int green, int blue);
    
    /*!
     * \brief Copy constructor for ColorRGB
     * \param[in] color Object to copy from
     */
    ColorRGB(const ColorRGB &color);

    /*!
     * \brief Move constructor for ColorRGB
     * \param[in] color Object to move
     */
    ColorRGB(ColorRGB &&color) TL_NOEXCEPT;

    ~ColorRGB() override;
    
    /*!
     * \brief Red component
     * \return Red component
     */
    auto red() const -> int;
    
    /*!
     * \brief Sets the red component
     * \param[in] red Red component
     */
    void setRed(int red);
    
    /*!
     * \brief Green component
     * \return Green component
     */
    auto green() const -> int;
    
    /*!
     * \brief Sets the green component
     * \param[in] green Green component
     */
    void setGreen(int green);
    
    /*!
     * \brief Blue component
     * \return Blue component
     */
    auto blue() const -> int;
    
    /*!
     * \brief Sets the blue component
     * \param[in] blue Blue component
     */
    void setBlue(int blue);
    
    auto operator =(const ColorRGB& color) -> ColorRGB&;
    auto operator =(ColorRGB &&color) TL_NOEXCEPT -> ColorRGB&;
    
    auto toColor() const -> Color override;
    void fromColor(const Color &color) override;

protected:

    void adjustRangeRed();
    void adjustRangeGreen();
    void adjustRangeBlue();

};


/*!
 * \brief RGB color model + alpha channel
 * The RGB color model is based on the additive synthesis of the three
 * primary colors, namely red, green, and blue.
 */
class TL_EXPORT ColorRGBA
    : public ColorRGB
{

protected:

    int mAlpha;

public:

    /*!
     * \brief Default constructor for RGBA color
     */
    ColorRGBA();

    /*!
     * \brief Constructor for ColorRGBA
     * \param[in] red Red component
     * \param[in] green Green component
     * \param[in] blue Blue component
     * \param[in] alpha Alpha channel
     */
    ColorRGBA(int red, int green, int blue, int alpha = 255);

    /*!
     * \brief Copy constructor for ColorRGBA
     * \param[in] color Object to copy from
     */
    ColorRGBA(const ColorRGBA &color);

    /*!
     * \brief Move constructor for ColorRGBA
     * \param[in] color Object to move
     */
    ColorRGBA(ColorRGBA &&color) TL_NOEXCEPT;

    /*!
     * \brief Returns the alpha channel
     * \return Alpha channel
     */
    auto alpha() const -> int;

    /*!
     * \brief Sets the alpha channel
     * \param[in] alpha Alpha channel
     */
    void setAlpha(int alpha);

    auto toColor() const -> Color override;
    void fromColor(const Color &color) override;

    auto operator =(const ColorRGBA &color) -> ColorRGBA&;
    auto operator =(ColorRGBA &&color) TL_NOEXCEPT -> ColorRGBA&;

protected:

    void adjustRangeAlpha();

};


/*!
 * \brief Modelo de color CMYK
 * El modelo CMYK (Cyan, Magenta, Yellow y Key) es un modelo de color sustractivo.
 * Los valores de las componentes CMYK varían por defecto entre 0 y 1 y representan 
 * porcentajes.
 */
class TL_EXPORT ColorCMYK 
  : public ColorModel
{

protected:

    double mCyan;
    double mMagenta;
    double mYellow;
    double mKey;
    double mRangeMin;
    double mRangeMax;

public:

    /*!
     * \brief Default constructor for CMYK Color
     */
    ColorCMYK();
    
    /*!
     * \brief ColorCMYK constructor
     * \param[in] cyan Cyan color
     * \param[in] magenta Magenta color
     * \param[in] yellow Yellow color
     * \param[in] key Black color
     */
    ColorCMYK(double cyan, double magenta, double yellow, double key);
    
    /*!
     * \brief Copy constructor
     * \param[in] color ColorCMYK object to copy
     */
    ColorCMYK(const ColorCMYK &color);

    /*!
     * \brief Move constructor
     * \param[in] color ColorCMYK object to move
     */
    ColorCMYK(ColorCMYK &&color) TL_NOEXCEPT;

    /*!
     * \brief cyan
     * \return cyan
     */
    auto cyan() const -> double;
    
    /*!
     * \brief Sets Cyan
     * \param cyan
     */
    void setCyan(double cyan);
    
    /*!
     * \brief magenta
     * \return
     */
    auto magenta() const -> double;
    
    /*!
     * \brief Sets magenta
     * \param magenta
     */
    void setMagenta(double magenta);
    
    /*!
     * \brief yellow
     * \return
     */
    auto yellow() const -> double;
    
    /*!
     * \brief Sets Yellow
     * \param yellow
     */
    void setYellow(double yellow);
    
    /*!
     * \brief key
     * \return
     */
    auto key() const -> double;
    
    /*!
     * \brief Sets Key
     * \param key
     */
    void setKey(double key);
    
    /*!
     * \brief Assignment operator
     * \param[in] color Assigned object
     * \return Reference to the object
     */
    auto operator =(const ColorCMYK &color) -> ColorCMYK&;

    /*!
     * \brief Move assignment operator
     * \param[in] color Assigned object
     * \return Reference to the object
     */
    auto operator =(ColorCMYK &&color) TL_NOEXCEPT -> ColorCMYK &;

    /*!
     * \brief Sets the minimum and maximum range
     * \param[in] min Minimum value (default 0.)
     * \param[in] max Maximum value (default 1.)
     */
    void setRange(double min, double max);

    auto toColor() const -> Color override;
    void fromColor(const Color &color) override;

protected:

    void adjustRangeCyan();
    void adjustRangeMagenta();
    void adjustRangeYellow();
    void adjustRangeKey();


};



/*!
 * \brief HSV color model
 * The HSV (Hue, Saturation, Value) color space is based on how humans perceive colors. It is also known as HSB (Brightness).
 */
class TL_EXPORT ColorHSV
  : public ColorModel
{

private:

    double mHue;
    double mSaturation;
    double mValue;
    double mRangeMinHue;
    double mRangeMaxHue;
    double mRangeMin;
    double mRangeMax;

public:

    /*!
     * \brief Default constructor
     */
    ColorHSV();

    /*!
     * \brief Copy constructor
     * \param[in] hue Hue is the degree to which a stimulus can be described as similar or different from stimuli such as red, yellow, and blue.
     * \param[in] saturation Saturation
     * \param[in] value Value
     */
    ColorHSV(double hue, double saturation, double value);

    /*!
     * \brief Copy constructor
     * \param[in] color ColorHSV object to copy
     */
    ColorHSV(const ColorHSV &color);

    /*!
     * \brief Move constructor
     * \param[in] color ColorHSV object to move
     */
    ColorHSV(ColorHSV &&color) TL_NOEXCEPT;

    /*!
     * \brief hue
     * \return
     */
    auto hue() const -> double;

    /*!
     * \brief Sets Hue
     * \param hue
     */
    void setHue(double hue);

    /*!
     * \brief saturation
     * \return
     */
    auto saturation() const -> double;

    /*!
     * \brief Sets saturation
     * \param saturation
     */
    void setSaturation(double saturation);

    /*!
     * \brief value
     * \return
     */
    auto value() const -> double;

    /*!
     * \brief Sets value
     * \param value
     */
    void setValue(double value);

    /*!
     * \brief Assignment operator
     * \param[in] color Assigned object
     * \return Reference to the object
     */
    auto operator =(const ColorHSV &color) -> ColorHSV&;

    /*!
     * \brief Assignment move operator
     * \param[in] color Assigned object
     * \return Reference to the object
     */
    auto operator =(ColorHSV &&color) TL_NOEXCEPT -> ColorHSV&;

    auto toColor() const -> Color override;
    void fromColor(const Color &color) override;

protected:

    void adjustRangeHue();
    void adjustRangeSaturation();
    void adjustRangeValue();

};



/*!
 * \brief The ColorHSL class
 */
class TL_EXPORT ColorHSL
    : public ColorModel
{

public:

    /*!
     * \brief Default constructor
     */
    ColorHSL();

    /*!
     * \brief Copy constructor
     * \param[in] hue Hue is the degree to which a stimulus can be described as similar or different from stimuli such as red, yellow, and blue.
     * \param[in] saturation Saturation
     * \param[in] lightness Lightness
     */
    ColorHSL(double hue, double saturation, double lightness);

    /*!
     * \brief Copy constructor
     * \param[in] color ColorHSL object to copy
     */
    ColorHSL(const ColorHSL &color);

    /*!
     * \brief Move constructor
     * \param[in] color ColorHSL object to move
     */
    ColorHSL(ColorHSL &&color) TL_NOEXCEPT;

    auto hue() const -> double;
    void setHue(double hue);

    auto saturation() const -> double;
    void setSaturation(double saturation);

    auto lightness() const -> double;
    void setLightness(double lightness);

    auto operator =(const ColorHSL &color) -> ColorHSL&;
    auto operator =(ColorHSL &&color) TL_NOEXCEPT -> ColorHSL &;

    auto toColor() const -> Color override;
    void fromColor(const Color &color) override;

protected:

    void adjustRangeHue();
    void adjustRangeSaturation();
    void adjustRangeLightness();

private:

    /*!
     * \brief Matiz del color
     * Se representa como un grado de ángulo cuyos valores posibles van de 0 a 360°
     */
    double mHue;

    /*!
     * \brief Saturación, tono o tinte de un color
     * Se representa como la distancia al eje de brillo negro-blanco. Los valores
     * posibles van del 0 al 100%.
     */
    double mSaturation;

    /*!
     * \brief Luminosidad
     */
    double mLightness;

    double mRangeMinHue;
    double mRangeMaxHue;

    double mRangeMin;
    double mRangeMax;

};



//enum class ColorModel
//{
//  RGB,
//  CMYK,
//  HSV
//};


//class TransformColorModel
//{

//protected:

//  ColorModel mIn;
//  ColorModel mOut;
//  std::funtion mTrf;

//public:

//  TransformColorModel(ColorModel in, ColorModel out);

//  void aply(const IColorModel &colorIn, IColorModel &out);
//};



/*!
 * \brief Class that defines a conversion between two color models
 */
template <typename T1, typename T2>
class TrfColorModel
{

public:

    TrfColorModel();

    void aply();

private:

    /*!
     * \brief Transformation applied
     */
    std::function<void(T1, T2)> mTrf;

};

template<typename T1, typename T2>
TrfColorModel<T1, T2>::TrfColorModel()
{
    mTrf = [](T1 &in, T2 &out) {
        out.fromColor(in.toColor());
    };
}

template<typename T1, typename T2>
void TrfColorModel<T1, T2>::aply()
{

}

/*! \} */ // end of GraphicEntities

} // End namespace TL
