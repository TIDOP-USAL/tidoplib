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
 * \brief The ColorHSL class
 * This class represents the HSL (Hue, Saturation, Lightness) color model.
 * The HSL model describes colors in terms of three components: hue, saturation, and lightness.
 * - Hue represents the color type, expressed as a degree angle from 0 to 360.
 * - Saturation defines the intensity of the color, with 0 being a grayscale color and 1 being a fully saturated color.
 * - Lightness represents the brightness of the color, with 0 being black, 1 being white, and 0.5 being the pure color.
 * HSL is widely used in graphics and design software because it is more intuitive for human understanding compared to RGB.
 */
class TL_EXPORT ColorHSL
{

private:

    double mHue = 0.;
    double mSaturation = 0.;
    double mLightness = 0.;

public:

    /*!
     * \brief Default constructor
     * Initializes a default color with hue, saturation, and lightness all set to 0.
     */
    constexpr ColorHSL() = default;

    /*!
     * \brief Parameterized constructor
     * Initializes the color with the given hue, saturation, and lightness.
     * \param[in] hue The hue value (angle in degrees from 0 to 360).
     * \param[in] saturation The saturation value (0 to 1, where 0 is grayscale and 1 is fully saturated).
     * \param[in] lightness The lightness value (0 to 1, where 0 is black, 1 is white, and 0.5 is the pure color).
     */
    constexpr ColorHSL(double hue, double saturation, double lightness);

    /*!
     * \brief Copy constructor
     * Creates a copy of the given ColorHSL object.
     * \param[in] color The ColorHSL object to copy.
     */
    constexpr ColorHSL(const ColorHSL &color) = default;

    /*!
     * \brief Move constructor
     * Moves the given ColorHSL object to this instance, transferring ownership of resources.
     * \param[in] color The ColorHSL object to move.
     */
    constexpr ColorHSL(ColorHSL &&color) noexcept = default;

    /*!
     * \brief Assignment operator
     * Assigns the values from another ColorHSL object to this one.
     * \param[in] color The ColorHSL object to assign.
     * \return Reference to the current ColorHSL object.
     */
    constexpr auto operator =(const ColorHSL &color) -> ColorHSL & = default;

    /*!
     * \brief Assignment move operator
     * Moves the values from another ColorHSL object to this one, transferring ownership of resources.
     * \param[in] color The ColorHSL object to move.
     * \return Reference to the current ColorHSL object.
     */
    constexpr auto operator =(ColorHSL &&color) noexcept -> ColorHSL & = default;

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
     * \brief Retrieves the lightness component
     * \return The lightness value (0 to 1).
     */
    [[nodiscard]]
    constexpr auto lightness() const noexcept -> double;

    /*!
     * \brief Sets the lightness component
     * \param[in] lightness The lightness value to set (0 to 1).
     */
    constexpr void setLightness(double lightness);

    /*!
     * \brief Converts the HSL color to a Color object in another color model
     * \return A Color object corresponding to the HSL values.
     */
    [[nodiscard]]
    auto toColor() const -> Color;

    /*!
     * \brief Converts a Color object to HSL values
     * \param[in] color The Color object to convert.
     */
    [[nodiscard]]
    static auto fromColor(const Color &color) -> ColorHSL;

};



constexpr ColorHSL::ColorHSL(double hue, double saturation, double lightness)
  : mHue(std::clamp(hue, 0., 360.)),
    mSaturation(std::clamp(saturation, 0., 100.)),
    mLightness(std::clamp(lightness, 0., 100.))
{
}

constexpr auto ColorHSL::hue() const noexcept -> double
{
    return mHue;
}

constexpr void ColorHSL::setHue(double hue)
{
    mHue = std::clamp(hue, 0., 360.);
}

constexpr auto ColorHSL::saturation() const noexcept -> double
{
    return mSaturation;
}

constexpr void ColorHSL::setSaturation(double saturation)
{
    mSaturation = std::clamp(saturation, 0., 100.);
}

constexpr auto ColorHSL::lightness() const noexcept -> double
{
    return mLightness;
}

constexpr void ColorHSL::setLightness(double lightness)
{
    mLightness = std::clamp(lightness, 0., 100.);
}


/*! \} */

} // End namespace tl
