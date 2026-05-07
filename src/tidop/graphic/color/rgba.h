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
#include "tidop/graphic/color/rgb.h"

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

    int mAlpha;

public:

    /*!
     * \brief Default constructor.
     * Initializes an RGBA color with default values.
     */
    ColorRGBA();

    /*!
     * \brief Constructor with RGBA components.
     * \param[in] red Red component
     * \param[in] green Green component
     * \param[in] blue Blue component
     * \param[in] alpha Alpha (transparency) component (default: 255)
     */
    ColorRGBA(int red, int green, int blue, int alpha = 255);

    /*!
     * \brief Copy constructor.
     * \param[in] color Object to copy from
     */
    ColorRGBA(const ColorRGBA &color);

    /*!
     * \brief Move constructor.
     * \param[in] color Object to move
     */
    ColorRGBA(ColorRGBA &&color) TL_NOEXCEPT;

    /*!
     * \brief Returns the alpha component.
     * \return Alpha (transparency) value
     */
    auto alpha() const -> int;

    /*!
     * \brief Sets the alpha component.
     * \param[in] alpha New alpha (transparency) value
     */
    void setAlpha(int alpha);

    /*!
     * \brief Converts the RGBA model to a Color object.
     * \return Converted Color object
     */
    auto toColor() const -> Color override;

    /*!
     * \brief Initializes the RGBA model from a Color object.
     * \param[in] color Color object to initialize from
     */
    void fromColor(const Color &color) override;

    /*!
     * \brief Assignment operator.
     * \param[in] color ColorRGBA object to assign
     * \return Reference to this ColorRGBA object
     */
    auto operator =(const ColorRGBA &color) -> ColorRGBA&;

    /*!
     * \brief Move assignment operator.
     * \param[in] color ColorRGBA object to assign
     * \return Reference to this ColorRGBA object
     */
    auto operator =(ColorRGBA &&color) TL_NOEXCEPT -> ColorRGBA&;

protected:

    void adjustRangeAlpha();

};

/*! \} */

} // End namespace tl
