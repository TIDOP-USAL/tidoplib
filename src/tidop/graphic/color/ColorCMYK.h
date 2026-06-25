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

#include <utility>
#include <algorithm>

#include "tidop/config.h"
#include "tidop/graphic/color/Color.h"
#include "tidop/core/base/TypeConversions.h"

namespace tl
{

/*! \addtogroup Color
 *  \{
 */


/*!
 * \brief CMYK color model.
 * The CMYK model (Cyan, Magenta, Yellow, and Key/Black) is a subtractive color model.
 * CMYK component values range by default between 0 and 1, representing percentages.
 */
class TL_EXPORT ColorCMYK 
{

protected:

    double mCyan = 0.0;       /*!< Cyan component */
    double mMagenta = 0.0;    /*!< Magenta component */
    double mYellow = 0.0;     /*!< Yellow component */
    double mKey = 0.0;        /*!< Black (Key) component */

public:

    /*!
     * \brief Default constructor.
     * Initializes a CMYK color with default values.
     */
    constexpr ColorCMYK() = default;

    /*!
     * \brief Constructor with CMYK components.
     * \param[in] cyan Cyan component
     * \param[in] magenta Magenta component
     * \param[in] yellow Yellow component
     * \param[in] key Black (Key) component
     */
    constexpr ColorCMYK(double cyan, double magenta, double yellow, double key)
      : mCyan(adjustRange(cyan)),
        mMagenta(adjustRange(magenta)),
        mYellow(adjustRange(yellow)),
        mKey(adjustRange(key))
    {
    }

    /*!
     * \brief Copy constructor.
     * \param[in] color ColorCMYK object to copy
     */
    constexpr ColorCMYK(const ColorCMYK &color) = default;;

    /*!
     * \brief Move constructor.
     * \param[in] color ColorCMYK object to move
     */
    constexpr ColorCMYK(ColorCMYK &&color) noexcept = default;

    ~ColorCMYK() = default;

    /*!
     * \brief Assignment operator.
     * \param[in] color ColorCMYK object to assign
     * \return Reference to this ColorCMYK object
     */
    constexpr auto operator =(const ColorCMYK &color) -> ColorCMYK & = default;

    /*!
     * \brief Move assignment operator.
     * \param[in] color ColorCMYK object to assign
     * \return Reference to this ColorCMYK object
     */
    constexpr auto operator =(ColorCMYK &&color) noexcept ->ColorCMYK & = default;

    /*!
     * \brief Returns the cyan component.
     * \return Cyan value
     */
    [[nodiscard]]
    constexpr auto cyan() const noexcept -> double;

    /*!
     * \brief Sets the cyan component.
     * \param[in] cyan New cyan value
     */
    constexpr void setCyan(double cyan);

    /*!
     * \brief Returns the magenta component.
     * \return Magenta value
     */
    [[nodiscard]]
    constexpr auto magenta() const noexcept -> double;

    /*!
     * \brief Sets the magenta component.
     * \param[in] magenta New magenta value
     */
    constexpr void setMagenta(double magenta);

    /*!
     * \brief Returns the yellow component.
     * \return Yellow value
     */
    [[nodiscard]]
    constexpr auto yellow() const noexcept -> double;

    /*!
     * \brief Sets the yellow component.
     * \param[in] yellow New yellow value
     */
    constexpr void setYellow(double yellow);

    /*!
     * \brief Returns the key (black) component.
     * \return Key value
     */
    [[nodiscard]]
    constexpr auto key() const noexcept -> double;

    /*!
     * \brief Sets the key (black) component.
     * \param[in] key New key value
     */
    constexpr void setKey(double key);

    /*!
     * \brief Converts the CMYK model to a Color object.
     * \return Converted Color object
     */
    [[nodiscard]]
    constexpr auto toColor() const -> Color;

    /*!
     * \brief Initializes the CMYK model from a Color object.
     * \param[in] color Color object to initialize from
     */
    [[nodiscard]]
    static constexpr auto fromColor(const Color &color) -> ColorCMYK;

protected:

    static constexpr auto adjustRange(double value) -> double;

};


constexpr auto ColorCMYK::cyan() const noexcept -> double
{
    return mCyan;
}

constexpr void ColorCMYK::setCyan(double cyan)
{
    mCyan = ColorCMYK::adjustRange(cyan);
}

constexpr auto ColorCMYK::magenta() const noexcept -> double
{
    return mMagenta;
}

constexpr void ColorCMYK::setMagenta(double magenta)
{
    mMagenta = ColorCMYK::adjustRange(magenta);
}

constexpr auto ColorCMYK::yellow() const noexcept -> double
{
    return mYellow;
}

constexpr void ColorCMYK::setYellow(double yellow)
{
    mYellow = ColorCMYK::adjustRange(yellow);
}

constexpr auto ColorCMYK::key() const noexcept -> double
{
    return mKey;
}

constexpr void ColorCMYK::setKey(double key)
{
    mKey = adjustRange(key);
}

constexpr auto ColorCMYK::toColor() const -> Color
{
    double aux = (1 - mKey) * 255;
    int red = roundToInteger((1 - mCyan) * aux);
    int green = roundToInteger((1 - mMagenta) * aux);
    int blue = roundToInteger((1 - mYellow) * aux);

    Color color(red, green, blue);
    return color;
}

constexpr auto ColorCMYK::fromColor(const Color &color) -> ColorCMYK
{
    double r = color.red() / 255.0;
    double g = color.green() / 255.0;
    double b = color.blue() / 255.0;

    double max = std::max({r, g, b});

    double key = 1. - max;
    if (key == 1.) {
        return ColorCMYK(0., 0., 0., 1.);
    } else {
        double cyan = 1. - r / max;
        double magenta = 1. - g / max;
        double yellow = 1. - b / max;
        return ColorCMYK(cyan, magenta, yellow, key);
    }
}

constexpr auto ColorCMYK::adjustRange(double value) -> double
{
    return std::clamp(value, 0.0, 1.0);
}

/*! \} */

} // End namespace tl
