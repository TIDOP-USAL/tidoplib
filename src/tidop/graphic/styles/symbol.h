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

#include <array>

#include "tidop/core/base/defs.h"
#include "tidop/graphic/color.h"

namespace tl
{


/*! \addtogroup Styles
 *  \{
 */


/*!
 * \brief Symbol style class
 */
class TL_EXPORT Symbol
{

public:

    enum class Name
    {
        cross,             /*!< + */
        diagonal_cross,    /*!< x */
        circle,            /*!< Circle */
        circle_filled,     /*!< Filled circle */
        square,            /*!< Square */
        square_filled,     /*!< Filled square */
        triangle,          /*!< Triangle */
        triangle_filled,   /*!< Filled triangle */
        star,              /*!< Star */
        star_filled,       /*!< Filled star */
        vertical_bar       /*!< | */
    };

private:

    /*!
     * \brief Name or identifier of the symbol
     */
    Name mName;

    /*!
     * \brief Rotation angle in decimal sexagesimal degrees
     * \see angleConversion
     */
    double mAngle;

    /*!
     * \brief Color
     * \see Color
     */
    Color mColor;

    /*!
     * \brief Outline color
     * \see Color
     */
    Color mOutlineColor;

    /*!
     * \brief Scaling factor
     */
    double mScalingFactor;

    /*!
     * \brief X and Y offset of the symbol insertion point.
     */
    std::array<double, 2> mOffset;

    /*!
     * \brief mPriorityLevel
     */
    uint32_t mPriorityLevel;

public:

    /*!
     * \brief Default constructor
     */
    Symbol();

    /*!
     * \brief Copy constructor
     * \param[in] symbol Symbol style object to copy
     */
    Symbol(const Symbol &symbol);

    /*!
     * \brief Move constructor
     * \param[in] symbol Symbol style object to move
     */
    Symbol(Symbol &&symbol) TL_NOEXCEPT;

    /*!
     * \brief Destructor
     */
    ~Symbol();

    /*!
     * \brief Get the rotation angle
     * \return Rotation angle in decimal sexagesimal degrees
     * \see angleConversion
     */
    auto angle() const -> double;

    /*!
     * \brief Set the rotation angle
     * \param[in] angle Rotation angle in decimal sexagesimal degrees
     * \see angleConversion
     */
    void setAngle(double angle);

    /*!
     * \brief Get the name or id of the symbol
     * \return Name or id of the symbol
     */
    auto name() const -> Name;

    /*!
     * \brief Set the name or id of the symbol
     * \param[in] name Name or id of the symbol
     */
    void setName(Name name);

    /*!
     * \brief Get the color
     * \return Color
     * \see Color
     */
    auto color() const -> Color;

    /*!
     * \brief Set the color
     * \param[in] color Color
     * \see Color
     */
    void setColor(const Color &color);

    /*!
     * \brief Get the outline color
     * \return Outline color
     * \see Color
     */
    auto outlineColor() const -> Color;

    /*!
     * \brief Set the outline color
     * \param[in] outlinecolor Outline color
     * \see Color
     */
    void setOutlineColor(const Color &outlinecolor);

    /*!
     * \brief Get the scaling factor
     * \return Scaling factor
     */
    auto scalingFactor() const -> double;

    /*!
     * \brief Set the scaling factor
     * \param[in] scalingFactor Scaling factor
     */
    void setScalingFactor(double scalingFactor);

    /*!
     * \brief Get the X offset of the symbol insertion point.
     */
    auto offsetX() const -> double;

    /*!
     * \brief Get the Y offset of the symbol insertion point.
     */
    auto offsetY() const -> double;

    /*!
     * \brief Set the offset of the symbol insertion point
     * \param[in] dx X offset of the symbol insertion point.
     * \param[in] dy Y offset of the symbol insertion point.
     */
    void setOffset(double dx, double dy);

    auto priorityLevel() const -> uint32_t;

    void setPriorityLevel(uint32_t priorityLevel);

    /*!
     * \brief Assignment operator
     * \param symbol Symbol style
     * \return Reference to the symbol style
     */
    auto operator =(const Symbol& symbol) -> Symbol&;

    /*!
     * \brief Assignment operator
     * \param symbol Symbol style
     * \return Reference to the symbol style
     */
    auto operator =(Symbol&& symbol) TL_NOEXCEPT -> Symbol&;
};


/*! \} */ 

} // End namespace tl
