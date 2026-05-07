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

#include <string>

#include "tidop/core/base/defs.h"


namespace tl
{

/*! \addtogroup Color
 *  \{
 */


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
 * \brief Class representing a color with various utility functions.
 */
class TL_EXPORT Color
{

public:

    /*!
     * \brief Enum containing HTML color names and their respective hex values.
     * \see http://www.w3schools.com/colors/colors_names.asp
     */
    enum class Name
    {
        //  Color                     Hex	         (R,G,B)	      (H,S,V)	          (H,S,L)
        // ----------------------------------------------------------------------------------
    
        black =	                  0x000000,   // (0,0,0)        (0°,0%,0%)	      (0°,0%,0%)
        navy =	                  0x000080,   // (0,0,128)	    (240°,100%,50%)	  (240°,100%,25%)
        dark_blue =	              0x00008B,
        medium_blue =	          0x0000CD,
        blue =	                  0x0000FF,   // (0,0,255)      (240°,100%,100%)	(240°,100%,50%)
        dark_green =	          0x006400,
        green =	                  0x008000,   // (0,128,0)	    (120°,100%,50%)	  (120°,100%,25%)
        teal =	                  0x008080,   // (0,128,128)    (180°,100%,50%)	  (180°,100%,25%)
        dark_cyan =	              0x008B8B,
        deep_sky_blue =	          0x00BFFF,
        dark_turquoise =	      0x00CED1,
        medium_spring_green =	  0x00FA9A,
        lime =	                  0x00FF00,   // (0,255,0)	    (120°,100%,100%)   (120°,100%,50%)
        spring_green =	          0x00FF7F,
        aqua =	                  0x00FFFF,
        cyan =	                  0x00FFFF,   // (0,255,255)	(180°,100%,100%)   (180°,100%,50%)
        midnigh_tblue =	          0x191970,
        dodger_blue =	          0x1E90FF,
        light_sea_green =	      0x20B2AA,
        forest_green =	          0x228B22,
        sea_green =	              0x2E8B57,
        dark_slate_gray =	      0x2F4F4F,
        lime_green =	          0x32CD32,
        medium_sea_green =	      0x3CB371,
        turquoise =	              0x40E0D0,
        royal_blue =	          0x4169E1,
        steel_blue =	          0x4682B4,
        dark_slate_blue =	      0x483D8B,
        medium_turquoise =	      0x48D1CC,
        indigo = 	              0x4B0082,
        dark_olive_green =	      0x556B2F,
        cadet_blue =	          0x5F9EA0,
        cornflower_blue =	      0x6495ED,
        rebecca_purple =   	      0x663399,
        medium_aqua_marine =	  0x66CDAA,
        dim_gray =	              0x696969,
        slate_blue =	          0x6A5ACD,
        olive_drab =	          0x6B8E23,
        slate_gray =	          0x708090,
        light_slate_gray =	      0x778899,
        medium_slate_blue =	      0x7B68EE,
        lawn_green =	          0x7CFC00,
        chartreuse =	          0x7FFF00,
        aquamarine =	          0x7FFFD4,
        maroon =	              0x800000,   // (128,0,0)	    (0°,100%,50%)   	(0°,100%,25%)
        purple =	              0x800080,   // (128,0,128)	  (300°,100%,50%)	  (300°,100%,25%)
        olive =	                  0x808000,   // (128,128,0)	  (60°,100%,50%)	  (60°,100%,25%)
        gray =	                  0x808080,   // (128,128,128)	(0°,0%,50%)	      (0°,0%,50%)
        sky_blue =	              0x87CEEB,
        light_sky_blue =	      0x87CEFA,
        blue_violet =	          0x8A2BE2,
        dark_red =	              0x8B0000,
        dark_magenta =	          0x8B008B,
        saddle_brown =	          0x8B4513,
        dark_sea_green =	      0x8FBC8F,
        light_green =	          0x90EE90,
        medium_purple =	          0x9370DB,
        dark_violet =	          0x9400D3,
        pale_green =	          0x98FB98,
        dark_orchid =	          0x9932CC,
        yellow_green =	          0x9ACD32,
        sienna =	              0xA0522D,
        brown =   	              0xA52A2A,
        dark_gray =	              0xA9A9A9,
        light_blue =	          0xADD8E6,
        green_yellow =	          0xADFF2F,
        pale_turquoise =	      0xAFEEEE,
        light_steel_blue =	      0xB0C4DE,
        powder_blue =	          0xB0E0E6,
        fire_brick =	          0xB22222,
        dark_golden_rod =	      0xB8860B,
        medium_orchid =	          0xBA55D3,
        rosy_brown =	          0xBC8F8F,
        dark_khaki =	          0xBDB76B,
        silver =	              0xC0C0C0,   // (192,192,192)	(0°,0%,75%)	      (0°,0%,75%)
        medium_violet_red =	      0xC71585,
        indian_red = 	          0xCD5C5C,
        peru =	                  0xCD853F,
        chocolate =	              0xD2691E,
        tan =	                  0xD2B48C,
        light_gray =	          0xD3D3D3,
        thistle =	              0xD8BFD8,
        orchid =	              0xDA70D6,
        golden_rod =	          0xDAA520,
        pale_violet_red =	      0xDB7093,
        crimson =	              0xDC143C,
        gainsboro =	              0xDCDCDC,
        plum =	                  0xDDA0DD,
        burly_wood =	          0xDEB887,
        light_cyan =	          0xE0FFFF,
        lavender =	              0xE6E6FA,
        dark_salmon =	          0xE9967A,
        violet =	              0xEE82EE,
        pale_golden_rod =	      0xEEE8AA,
        light_coral =	          0xF08080,
        khaki =	                  0xF0E68C,
        alice_blue =	          0xF0F8FF,
        honey_dew =	              0xF0FFF0,
        azure =	                  0xF0FFFF,
        sandy_brown =	          0xF4A460,
        wheat =	                  0xF5DEB3,
        beige =	                  0xF5F5DC,
        white_smoke = 	          0xF5F5F5,
        mint_cream =	          0xF5FFFA,
        ghost_white =	          0xF8F8FF,
        salmon =	              0xFA8072,
        antique_white =	          0xFAEBD7,
        linen =	                  0xFAF0E6,
        light_golden_rod_yellow = 0xFAFAD2,
        old_lace =	              0xFDF5E6,
        red =	                  0xFF0000,   // (255,0,0)	    (0°,100%,100%)	  (0°,100%,50%)
        fuchsia =	              0xFF00FF,
        magenta =	              0xFF00FF,   // (255,0,255)	  (300°,100%,100%)	(300°,100%,50%)
        deep_pink =	              0xFF1493,
        orange_red =	          0xFF4500,
        tomato =	              0xFF6347,
        hot_pink =	              0xFF69B4,
        coral =	                  0xFF7F50,
        dark_orange =	          0xFF8C00,
        light_salmon =	          0xFFA07A,
        orange =	              0xFFA500,
        light_pink =	          0xFFB6C1,
        pink =	                  0xFFC0CB,
        gold =	                  0xFFD700,
        peach_puff =	          0xFFDAB9,
        navajo_white =	          0xFFDEAD,
        moccasin =	              0xFFE4B5,
        bisque =	              0xFFE4C4,
        misty_rose =	          0xFFE4E1,
        blanched_almond =	      0xFFEBCD,
        papaya_whip =	          0xFFEFD5,
        lavender_blush =	      0xFFF0F5,
        sea_shell =	              0xFFF5EE,
        cornsilk =	              0xFFF8DC,
        lemon_chiffon =	          0xFFFACD,
        floral_white =	          0xFFFAF0,
        snow =	                  0xFFFAFA,
        yellow =	              0xFFFF00,   // (255,255,0)	  (60°,100%,100%)	  (60°,100%,50%)
        light_yellow =	          0xFFFFE0,
        ivory =	                  0xFFFFF0,
        white = 	              0xFFFFFF    // (255,255,255)	(0°,0%,100%)	    (0°,0%,100%)
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
     * \see ColorModel
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

    /*!
     * \brief Sets the opacity
     * \param[in] opacity New opacity value
     */
    void setOpacity(uint8_t opacity);
    
    /*!
     * \brief Computes the luminance value of a color
     * \return Luminance value
     */
    auto luminance() const -> int;
    
    /*!
     * \brief Converts the color to a hexadecimal string
     * \return Hexadecimal string representation of the color
     */
    auto toHex() const -> std::string;
    
    /*!
     * \brief Generates a random color
     * \return Randomly generated Color object
     */
    static auto randomColor() -> Color;
    
    operator unsigned int() const { return mColor; }
    operator int() const { return static_cast<int>(mColor); }
    
    /*!
     * \brief Assignment operator
     * \param[in] color Color to assign
     * \return Reference to this Color object
     */
    auto operator =(const Color &color) -> Color &;

    /*!
     * \brief Move assignment operator
     * \param[in] color Color to assign
     * \return Reference to this Color object
     */
    auto operator =(Color &&color) TL_NOEXCEPT -> Color &;

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




/*! \} */

} // End namespace tl
