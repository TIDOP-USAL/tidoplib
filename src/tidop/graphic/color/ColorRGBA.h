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
#include "tidop/graphic/color/ColorRGB.h"

namespace tl
{

/*! \addtogroup Color
 *  \{
 */


/*!
 * \brief RGB color model with alpha channel.
 * The RGB color model is based on the additive synthesis of the three
 * primary colors: red, green, and blue. This class extends the model
 * by including an alpha (transparency) channel.
 */
class TL_EXPORT ColorRGBA
  : public ColorRGB
{

protected:

    uint8_t mAlpha = 255;

public:

    /*!
     * \brief Default constructor.
     * Initializes an RGBA color with default values.
     */
    constexpr ColorRGBA() = default;

    /*!
     * \brief Constructor with RGBA components.
     * \param[in] red Red component
     * \param[in] green Green component
     * \param[in] blue Blue component
     * \param[in] alpha Alpha (transparency) component (default: 255)
     */
    constexpr ColorRGBA(int red, int green, int blue, int alpha = 255);

    /*!
     * \brief Copy constructor.
     * \param[in] color Object to copy from
     */
    constexpr ColorRGBA(const ColorRGBA &color) = default;

    /*!
     * \brief Move constructor.
     * \param[in] color Object to move
     */
    constexpr ColorRGBA(ColorRGBA &&color) noexcept = default;

    ~ColorRGBA() = default;

    /*!
     * \brief Assignment operator.
     * \param[in] color ColorRGBA object to assign
     * \return Reference to this ColorRGBA object
     */
    constexpr auto operator =(const ColorRGBA &color) -> ColorRGBA & = default;

    /*!
     * \brief Move assignment operator.
     * \param[in] color ColorRGBA object to assign
     * \return Reference to this ColorRGBA object
     */
    constexpr auto operator =(ColorRGBA &&color) noexcept ->ColorRGBA & = default;

    /*!
     * \brief Returns the alpha component.
     * \return Alpha (transparency) value
     */
    [[nodiscard]]
    constexpr auto alpha() const noexcept -> int;

    /*!
     * \brief Sets the alpha component.
     * \param[in] alpha New alpha (transparency) value
     */
    constexpr void setAlpha(int alpha);

    /*!
     * \brief Converts the RGBA model to a Color object.
     * \return Converted Color object
     */
    [[nodiscard]]
    constexpr auto toColor() const -> Color;

    /*!
     * \brief Initializes the RGBA model from a Color object.
     * \param[in] color Color object to initialize from
     */
    static constexpr auto fromColor(const Color &color) -> ColorRGBA;

};



constexpr ColorRGBA::ColorRGBA(int red, int green, int blue, int alpha)
  : ColorRGB(red, green, blue),
    mAlpha(adjustRange(alpha))
{
}

constexpr auto ColorRGBA::alpha() const noexcept -> int
{
    return static_cast<int>(mAlpha);
}

constexpr void ColorRGBA::setAlpha(int alpha)
{
    mAlpha = adjustRange(alpha);
}

constexpr auto ColorRGBA::toColor() const -> Color
{
    Color color(static_cast<int>(mRed), 
                static_cast<int>(mGreen),
                static_cast<int>(mBlue));
    color.setAlpha(static_cast<int>(mAlpha));
    return color;
}

constexpr auto ColorRGBA::fromColor(const Color &color) -> ColorRGBA
{
    ColorRGBA rgba(color.red(), color.green(), color.blue(), color.alpha());
    return rgba;
}


/*! \} */

} // End namespace tl
