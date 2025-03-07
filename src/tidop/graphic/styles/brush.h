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
 * \brief Brush style class
 */
class TL_EXPORT Brush
{
public:

    enum class Name : uint8_t
    {
        solid,             /*!< Solid (default value when no id is provided) */
        null,              /*!< Null brush (invisible) */
        horizontal_hatch,  /*!< ------ */
        vertical_hatch,    /*!< |||||| */
        fdiagonal_hatch,   /*!< \\\\\\ */
        bdiagonal_hatch,   /*!< ////// */
        cross_hatch,       /*!< ++++++ */
        diagcross_hatch    /*!< xxxxxx */
    };

private:

    /*!
     * \brief Foreground color
     * \see Color
     */
    Color mForeColor;

    /*!
     * \brief Background color
     * \see Color
     */
    Color mBackColor;

    /*!
     * \brief Brush name
     */
    Name mName;

    /*!
     * \brief Rotation angle in decimal sexagesimal degrees
     * \see angleConversion
     */
    double mAngle;

    /*!
     * \brief Scaling factor
     */
    double mScalingFactor;

    /*!
     * \brief Spacing between symbols
     */
    std::array<double, 2> mSpacing;

    /*!
     * \brief mPriorityLevel
     */
    uint32_t mPriorityLevel;

public:

    /*!
     * \brief Default constructor
     */
    Brush();

    /*!
     * \brief Copy constructor
     * \param[in] brush Brush object being copied
     */
    Brush(const Brush &brush);

    /*!
     * \brief Move constructor
     * \param[in] brush Brush object being copied
     */
    Brush(Brush &&brush) TL_NOEXCEPT;

    /*!
     * \brief Destructor
     */
    ~Brush();

    /*!
     * \brief Returns the foreground color
     * \return Foreground color
     * \see Color
     */
    auto foregroundColor() const -> Color;

    /*!
     * \brief Sets the foreground color
     * \param[in] foregroundColor Foreground color
     * \see Color
     */
    void setForegroundColor(const Color &foregroundColor);

    /*!
     * \brief Returns the background color
     * \return Background color
     * \see Color
     */
    auto backgroundColor() const -> Color;

    /*!
     * \brief Sets the background color
     * \param[in] backgroundColor Background color
     * \see Color
     */
    void setBackgroundColor(const Color& backgroundColor);

    /*!
     * \brief Returns the name or ID of the brush
     * \return Name or ID of the brush
     */
    auto name() const -> Name;

    /*!
     * \brief Sets the name or ID of the brush
     * \param[in] name Name or ID of the brush
     */
    void setName(Name name);

    /*!
     * \brief Returns the rotation angle
     * \return Rotation angle in decimal sexagesimal degrees
     * \see angleConversion
     */
    auto angle() const -> double;

    /*!
     * \brief Sets the rotation angle
     * \param[in] angle Rotation angle in decimal sexagesimal degrees
     * \see angleConversion
     */
    void setAngle(double angle);

    /*!
     * \brief Returns the scaling factor
     * \return Scaling factor
     */
    auto scalingFactor() const -> double;

    /*!
     * \brief Sets the scaling factor
     * \param[in] scalingFactor Scaling factor
     */
    void setScalingFactor(double scalingFactor);

    /*!
     * \brief Returns the spacing in the X direction
     * \return Spacing in X
     */
    auto spacingX() const -> double;

    /*!
     * \brief Returns the spacing in the Y direction
     * \return Spacing in Y
     */
    auto spacingY() const -> double;

    /*!
     * \brief Sets the spacing in the X and Y directions
     * \param[in] spacingX Spacing in X
     * \param[in] spacingY Spacing in Y
     */
    void setSpacing(double spacingX, double spacingY);

    /*!
     * \brief Returns the priority level
     * \return Priority level
     */
    auto priorityLevel() const -> uint32_t;

    /*!
     * \brief Sets the priority level
     * \param priorityLevel Priority level
     */
    void setPriorityLevel(uint32_t priorityLevel);

    /*!
     * \brief Assignment operator
     * \param brush Brush style
     * \return Reference to the brush style
     */
    auto operator =(const Brush& brush) -> Brush&;

    /*!
     * \brief Assignment move operator
     * \param brush Brush style
     * \return Reference to the brush style
     */
    auto operator =(Brush&& brush) TL_NOEXCEPT -> Brush&;

};


/*! \} */ 

} // End namespace tl
