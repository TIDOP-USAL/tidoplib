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
#include "tidop/graphic/color/color_model.h"

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
  : public ColorModel
{

private:

    double mHue;         ///< Hue value (angle in degrees, 0 to 360)
    double mSaturation;  ///< Saturation value (0 to 1, where 0 is grayscale and 1 is full color)
    double mValue;       ///< Value/brightness (0 to 1, where 0 is black and 1 is full brightness)
    double mRangeMinHue; ///< Minimum range for Hue (0 to 360 degrees)
    double mRangeMaxHue; ///< Maximum range for Hue (0 to 360 degrees)
    double mRangeMin;    ///< Minimum range for saturation and value (0)
    double mRangeMax;    ///< Maximum range for saturation and value (1)

public:

    /*!
     * \brief Default constructor
     * Initializes a default color with hue, saturation, and value all set to 0.
     */
    ColorHSV();

    /*!
     * \brief Parameterized constructor
     * Initializes the color with the given hue, saturation, and value.
     * \param[in] hue The hue value (angle in degrees from 0 to 360).
     * \param[in] saturation The saturation value (0 to 1).
     * \param[in] value The brightness value (0 to 1).
     */
    ColorHSV(double hue, double saturation, double value);

    /*!
     * \brief Copy constructor
     * Creates a copy of the given ColorHSV object.
     * \param[in] color The ColorHSV object to copy.
     */
    ColorHSV(const ColorHSV &color);

    /*!
     * \brief Move constructor
     * Moves the given ColorHSV object to this instance, transferring ownership of resources.
     * \param[in] color The ColorHSV object to move.
     */
    ColorHSV(ColorHSV &&color) TL_NOEXCEPT;

    /*!
     * \brief Retrieves the hue component
     * \return The hue value (angle in degrees from 0 to 360).
     */
    auto hue() const -> double;

    /*!
     * \brief Sets the hue component
     * \param[in] hue The hue value to set (angle in degrees from 0 to 360).
     */
    void setHue(double hue);

    /*!
     * \brief Retrieves the saturation component
     * \return The saturation value (0 to 1).
     */
    auto saturation() const -> double;

    /*!
     * \brief Sets the saturation component
     * \param[in] saturation The saturation value to set (0 to 1).
     */
    void setSaturation(double saturation);

    /*!
     * \brief Retrieves the value (brightness) component
     * \return The value (brightness) value (0 to 1).
     */
    auto value() const -> double;

    /*!
     * \brief Sets the value (brightness) component
     * \param[in] value The value (brightness) to set (0 to 1).
     */
    void setValue(double value);

    /*!
     * \brief Assignment operator
     * Assigns the values from another ColorHSV object to this one.
     * \param[in] color The ColorHSV object to assign.
     * \return Reference to the current ColorHSV object.
     */
    auto operator =(const ColorHSV &color) -> ColorHSV &;

    /*!
     * \brief Assignment move operator
     * Moves the values from another ColorHSV object to this one, transferring ownership of resources.
     * \param[in] color The ColorHSV object to move.
     * \return Reference to the current ColorHSV object.
     */
    auto operator =(ColorHSV &&color) TL_NOEXCEPT -> ColorHSV &;

    /*!
     * \brief Converts the HSV color to a Color object in another color model
     * \return A Color object corresponding to the HSV values.
     */
    auto toColor() const->Color override;

    /*!
     * \brief Converts a Color object to HSV values
     * \param[in] color The Color object to convert.
     */
    void fromColor(const Color &color) override;

protected:

    void adjustRangeHue();
    void adjustRangeSaturation();
    void adjustRangeValue();

};

/*! \} */

} // End namespace tl
