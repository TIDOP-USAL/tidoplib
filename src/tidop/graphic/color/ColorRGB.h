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

#include "tidop/config.h"
#include "tidop/graphic/color/Color.h"

namespace tl
{

/*! \addtogroup Color
 *  \{
 */

/*!
 * \brief RGB color model.
 * The RGB color model is based on the additive synthesis of the three
 * primary colors: red, green, and blue.
 */
class TL_EXPORT ColorRGB 
{

protected:

    uint8_t mRed = 0;
    uint8_t mGreen = 0;
    uint8_t mBlue = 0;

public:

    /*!
     * \brief Default constructor.
     * Initializes an RGB color with default values.
     */
    constexpr ColorRGB() = default;

    /*!
     * \brief Constructor with RGB components.
     * \param[in] red Red component
     * \param[in] green Green component
     * \param[in] blue Blue component
     */
    constexpr ColorRGB(int red, int green, int blue);
    
    /*!
     * \brief Copy constructor.
     * \param[in] color Object to copy from
     */
    constexpr ColorRGB(const ColorRGB &color) = default;

    /*!
     * \brief Move constructor.
     * \param[in] color Object to move
     */
    constexpr ColorRGB(ColorRGB &&color) noexcept = default;

    /*!
     * \brief Destructor.
     */
    ~ColorRGB() = default;
    
    /*!
     * \brief Assignment operator.
     * \param[in] color ColorRGB object to assign
     * \return Reference to this ColorRGB object
     */
    constexpr auto operator =(const ColorRGB &color) -> ColorRGB & = default;

    /*!
     * \brief Move assignment operator.
     * \param[in] color ColorRGB object to assign
     * \return Reference to this ColorRGB object
     */
    constexpr auto operator =(ColorRGB &&color) noexcept -> ColorRGB & = default;

    /*!
     * \brief Returns the red component.
     * \return Red component value
     */
    [[nodiscard]]
    constexpr auto red() const noexcept -> int;

    /*!
     * \brief Sets the red component.
     * \param[in] red New red component value
     */
    constexpr void setRed(int red);

    /*!
     * \brief Returns the green component.
     * \return Green component value
     */
    [[nodiscard]]
    constexpr auto green() const noexcept -> int;

    /*!
     * \brief Sets the green component.
     * \param[in] green New green component value
     */
    constexpr void setGreen(int green);

    /*!
     * \brief Returns the blue component.
     * \return Blue component value
     */
    [[nodiscard]]
    constexpr auto blue() const noexcept -> int;

    /*!
     * \brief Sets the blue component.
     * \param[in] blue New blue component value
     */
    constexpr void setBlue(int blue);
    
    /*!
     * \brief Converts the RGB model to a Color object.
     * \return Converted Color object
     */ 
    [[nodiscard]]
    constexpr auto toColor() const -> Color;

    /*!
     * \brief Initializes the RGB model from a Color object.
     * \param[in] color Color object to initialize from
     */
    [[nodiscard]]
    static constexpr auto fromColor(const Color &color) -> ColorRGB;

protected:

    [[nodiscard]]
    constexpr auto adjustRange(int value) const -> uint8_t;

};




constexpr ColorRGB::ColorRGB(int red, int green, int blue)
  : mRed(adjustRange(red)),
    mGreen(adjustRange(green)),
    mBlue(adjustRange(blue))
{
}

constexpr auto ColorRGB::red() const noexcept -> int
{
    return mRed;
}

constexpr void ColorRGB::setRed(int red)
{
    mRed = adjustRange(red);
}

constexpr auto ColorRGB::green() const noexcept -> int
{
    return mGreen;
}

constexpr void ColorRGB::setGreen(int green)
{
    mGreen = adjustRange(green);
}

constexpr auto ColorRGB::blue() const noexcept -> int
{
    return mBlue;
}

constexpr void ColorRGB::setBlue(int blue)
{
    mBlue = adjustRange(blue);
}

constexpr auto ColorRGB::toColor() const -> Color
{
    Color color(static_cast<int>(mRed),
                static_cast<int>(mGreen),
                static_cast<int>(mBlue));
    return color;
}

constexpr auto ColorRGB::fromColor(const Color &color) -> ColorRGB
{
    return ColorRGB(color.red(), color.green(), color.blue());
}

constexpr auto ColorRGB::adjustRange(int value) const -> uint8_t
{
    return static_cast<uint8_t>(std::clamp(value, 0, 255));
}

/*! \} */

} // End namespace tl
