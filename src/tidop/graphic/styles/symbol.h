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

#include "tidop/core/base/Defs.h"
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

    /*!
     * \enum Shape
     * \brief Symbol shapes used to represent points.
     *
     * These shapes are rendered at the symbol insertion point, optionally rotated or scaled.
     */
    enum class Shape
    {
        cross,             /*!< + */
        diagonal_cross,    /*!< x */
        circle,            /*!< Hollow circle */
        circle_filled,     /*!< Filled circle */
        square,            /*!< Hollow square */
        square_filled,     /*!< Filled square */
        triangle,          /*!< Hollow triangle */
        triangle_filled,   /*!< Filled triangle */
        star,              /*!< Hollow star */
        star_filled,       /*!< Filled star */
        vertical_bar       /*!< | */
    };

#ifdef TL_WARNING_DEPRECATED_METHOD
    enum class TL_DEPRECATED("Shape", "4.0") Name
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
#endif // TL_WARNING_DEPRECATED_METHOD

private:

    Shape mShape;
    double mAngle;
    Color mColor;
    Color mOutlineColor;
    double mScalingFactor;
    std::array<double, 2> mOffset;
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
     * \brief Gets the rotation angle of the symbol.
     * \return Rotation angle in decimal degrees.
     */
    auto angle() const -> double;

    /*!
     * \brief Sets the rotation angle of the symbol.
     * \param[in] angle Rotation angle in decimal degrees.
     */
    void setAngle(double angle);

    /*!
     * \brief Returns the symbol shape.
     * \return Symbol shape.
     */
    auto shape() const -> Shape;

    /*!
     * \brief Sets the symbol shape.
     * \param[in] shape The shape to use for rendering the symbol.
     */
    void setShape(Shape shape);

    /*!
     * \brief Gets the fill color of the symbol.
     * \return Symbol fill color.
     */
    auto color() const -> Color;

    /*!
     * \brief Set the color
     * \param[in] color Color
     * \see Color
     */
    void setColor(const Color &color);

    /*!
     * \brief Gets the outline color of the symbol.
     * \return Outline color.
     */
    auto outlineColor() const -> Color;

    /*!
     * \brief Set the outline color
     * \param[in] outlinecolor Outline color
     * \see Color
     */
    void setOutlineColor(const Color &outlinecolor);

    /*!
     * \brief Gets the scale factor applied to the symbol.
     * \return Scaling factor (1.0 = no scaling).
     */
    auto scalingFactor() const -> double;

    /*!
     * \brief Sets the scale factor of the symbol.
     * \param[in] scalingFactor Factor to scale the symbol size.
     */
    void setScalingFactor(double scalingFactor);

    /*!
     * \brief Gets the offset in X direction from the insertion point.
     * \return X offset in drawing units.
     */
    auto offsetX() const -> double;

    /*!
     * \brief Gets the offset in Y direction from the insertion point.
     * \return Y offset in drawing units.
     */
    auto offsetY() const -> double;

    /*!
     * \brief Sets the offset from the insertion point.
     * \param[in] dx Horizontal offset.
     * \param[in] dy Vertical offset.
     */
    void setOffset(double dx, double dy);

    /*!
     * \brief Gets the rendering priority level.
     * \return Priority level.
     */
    auto priorityLevel() const -> uint32_t;

    /*!
     * \brief Sets the rendering priority level.
     * \param[in] priorityLevel Rendering priority.
     */
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

#ifdef TL_WARNING_DEPRECATED_METHOD
    /*!
     * \brief Get the name or id of the symbol
     * \return Name or id of the symbol
     * \deprecated Use `shape()` instead.
     */
    TL_DEPRECATED("shape()", "4.0")
    auto name() const->Name;

    /*!
     * \brief Set the name or id of the symbol
     * \param[in] name Name or id of the symbol
     * \deprecated Use `setShape()` instead.
     */
    TL_DEPRECATED("setShape()", "4.0")
    void setName(Name name);
#endif // TL_WARNING_DEPRECATED_METHOD
};


/*! \} */ 

} // End namespace tl
