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

#include <algorithm>

#include "tidop/config.h"
#include "tidop/graphic/color/Color.h"

namespace tl
{

/*! \addtogroup Color
 *  \{
 */


/*!
 * \brief HSV color model
 * The HSV (Hue, Saturation, Value) color space is based on how humans perceive colors. It is also known as HSB (Brightness).
 * The model represents color in terms of three components: hue (H), saturation (S), and value (V).
 * Hue represents the color type, saturation describes the intensity or purity of the color, 
 * and value represents the brightness or darkness of the color.
 * The HSV model is widely used in various applications like graphics software and image processing 
 * because it is more intuitive for human perception than the RGB model.
 */
class TL_EXPORT ColorHSV
{

private:

    double mHue = 0.;           ///< Hue value (angle in degrees, 0 to 360)
    double mSaturation = 0.;    ///< Saturation value (0 to 1, where 0 is grayscale and 1 is full color)
    double mValue = 0.;         ///< Value/brightness (0 to 1, where 0 is black and 1 is full brightness)

public:

    /*!
     * \brief Default constructor
     * Initializes a default color with hue, saturation, and value all set to 0.
     */
    constexpr ColorHSV() = default;

    /*!
     * \brief Parameterized constructor
     * Initializes the color with the given hue, saturation, and value.
     * \param[in] hue The hue value (angle in degrees from 0 to 360).
     * \param[in] saturation The saturation value (0 to 1).
     * \param[in] value The brightness value (0 to 1).
     */
    constexpr ColorHSV(double hue, double saturation, double value);

    /*!
     * \brief Copy constructor
     * Creates a copy of the given ColorHSV object.
     * \param[in] color The ColorHSV object to copy.
     */
    constexpr ColorHSV(const ColorHSV &color) = default;

    /*!
     * \brief Move constructor
     * Moves the given ColorHSV object to this instance, transferring ownership of resources.
     * \param[in] color The ColorHSV object to move.
     */
    constexpr ColorHSV(ColorHSV &&color) noexcept = default;

    /*!
     * \brief Destructor.
     */
    ~ColorHSV() = default;

    /*!
     * \brief Assignment operator
     * Assigns the values from another ColorHSV object to this one.
     * \param[in] color The ColorHSV object to assign.
     * \return Reference to the current ColorHSV object.
     */
    constexpr auto operator =(const ColorHSV &color) -> ColorHSV & = default;

    /*!
     * \brief Assignment move operator
     * Moves the values from another ColorHSV object to this one, transferring ownership of resources.
     * \param[in] color The ColorHSV object to move.
     * \return Reference to the current ColorHSV object.
     */
    constexpr auto operator =(ColorHSV &&color) noexcept -> ColorHSV & = default;

    /*!
     * \brief Retrieves the hue component
     * \return The hue value (angle in degrees from 0 to 360).
     */
    [[nodiscard]]
    constexpr auto hue() const noexcept -> double;

    /*!
     * \brief Sets the hue component
     * \param[in] hue The hue value to set (angle in degrees from 0 to 360).
     */
    constexpr void setHue(double hue);

    /*!
     * \brief Retrieves the saturation component
     * \return The saturation value (0 to 1).
     */
    [[nodiscard]]
    constexpr auto saturation() const noexcept -> double;

    /*!
     * \brief Sets the saturation component
     * \param[in] saturation The saturation value to set (0 to 1).
     */
    constexpr void setSaturation(double saturation);

    /*!
     * \brief Retrieves the value (brightness) component
     * \return The value (brightness) value (0 to 1).
     */
    [[nodiscard]]
    constexpr auto value() const noexcept -> double;

    /*!
     * \brief Sets the value (brightness) component
     * \param[in] value The value (brightness) to set (0 to 1).
     */
    constexpr void setValue(double value);

    /*!
     * \brief Converts the HSV color to a Color object in another color model
     * \return A Color object corresponding to the HSV values.
     */
    [[nodiscard]]
    auto toColor() const -> Color;

    /*!
     * \brief Converts a Color object to HSV values
     * \param[in] color The Color object to convert.
     */
    [[nodiscard]]
    static auto fromColor(const Color &color) -> ColorHSV;

};


constexpr ColorHSV::ColorHSV(double hue, double saturation, double value)
  : mHue(std::clamp(hue, 0., 360.)),
    mSaturation(std::clamp(saturation, 0., 100.)),
    mValue(std::clamp(value, 0., 100.))
{
}

constexpr auto ColorHSV::hue() const noexcept -> double
{
    return mHue;
}

constexpr void ColorHSV::setHue(double hue)
{
    mHue = std::clamp(hue, 0., 360.);
}

constexpr auto ColorHSV::saturation() const noexcept -> double
{
    return mSaturation;
}

constexpr void ColorHSV::setSaturation(double saturation)
{
    mSaturation = std::clamp(saturation, 0., 100.);
}

constexpr auto ColorHSV::value() const noexcept -> double
{
    return mValue;
}

constexpr void ColorHSV::setValue(double value)
{
    mValue = std::clamp(value, 0., 100.);
}

/*! \} */

} // End namespace tl
