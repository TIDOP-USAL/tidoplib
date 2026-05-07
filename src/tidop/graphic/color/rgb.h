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
 * \brief RGB color model.
 * The RGB color model is based on the additive synthesis of the three
 * primary colors: red, green, and blue.
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
     * \brief Default constructor.
     * Initializes an RGB color with default values.
     */
    ColorRGB();

    /*!
     * \brief Constructor with RGB components.
     * \param[in] red Red component
     * \param[in] green Green component
     * \param[in] blue Blue component
     */
    ColorRGB(int red, int green, int blue);
    
    /*!
     * \brief Copy constructor.
     * \param[in] color Object to copy from
     */
    ColorRGB(const ColorRGB &color);

    /*!
     * \brief Move constructor.
     * \param[in] color Object to move
     */
    ColorRGB(ColorRGB &&color) TL_NOEXCEPT;

    /*!
     * \brief Destructor.
     */
    ~ColorRGB() override;
    
    /*!
     * \brief Returns the red component.
     * \return Red component value
     */
    auto red() const -> int;

    /*!
     * \brief Sets the red component.
     * \param[in] red New red component value
     */
    void setRed(int red);

    /*!
     * \brief Returns the green component.
     * \return Green component value
     */
    auto green() const -> int;

    /*!
     * \brief Sets the green component.
     * \param[in] green New green component value
     */
    void setGreen(int green);

    /*!
     * \brief Returns the blue component.
     * \return Blue component value
     */
    auto blue() const -> int;

    /*!
     * \brief Sets the blue component.
     * \param[in] blue New blue component value
     */
    void setBlue(int blue);

    /*!
     * \brief Assignment operator.
     * \param[in] color ColorRGB object to assign
     * \return Reference to this ColorRGB object
     */
    auto operator =(const ColorRGB& color) -> ColorRGB&;

    /*!
     * \brief Move assignment operator.
     * \param[in] color ColorRGB object to assign
     * \return Reference to this ColorRGB object
     */
    auto operator =(ColorRGB &&color) TL_NOEXCEPT -> ColorRGB&;
    
    /*!
     * \brief Converts the RGB model to a Color object.
     * \return Converted Color object
     */
    auto toColor() const -> Color override;

    /*!
     * \brief Initializes the RGB model from a Color object.
     * \param[in] color Color object to initialize from
     */
    void fromColor(const Color &color) override;

protected:

    void adjustRangeRed();
    void adjustRangeGreen();
    void adjustRangeBlue();

};

/*! \} */

} // End namespace tl
