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

#include "tidop/config.h"
#include "tidop/math/algebra/vector/Vector.h"
#include "tidop/graphic/Color.h"

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

private:

    Shape mShape = Shape::cross;
    double mAngle = 0.0;
    Color mColor;
    Color mOutlineColor;
    double mScalingFactor = 1.0;
    Vector2d mOffset = {0., 0.};

public:

    /*!
     * \brief Default constructor
     */
    constexpr Symbol() = default;

    /*!
     * \brief Copy constructor
     * \param[in] symbol Symbol style object to copy
     */
    constexpr Symbol(const Symbol &symbol) = default;

    /*!
     * \brief Move constructor
     * \param[in] symbol Symbol style object to move
     */
    constexpr Symbol(Symbol &&symbol) noexcept = default;

    /*!
     * \brief Destructor
     */
    ~Symbol() = default;

    /*!
     * \brief Assignment operator
     * \param symbol Symbol style
     * \return Reference to the symbol style
     */
    constexpr auto operator =(const Symbol &symbol) -> Symbol & = default;

    /*!
     * \brief Assignment operator
     * \param symbol Symbol style
     * \return Reference to the symbol style
     */
    constexpr auto operator =(Symbol &&symbol) noexcept -> Symbol & = default;

    /*!
     * \brief Gets the rotation angle of the symbol.
     * \return Rotation angle in decimal degrees.
     */
    [[nodiscard]] 
    constexpr auto angle() const noexcept -> double;

    /*!
     * \brief Sets the rotation angle of the symbol.
     * \param[in] angle Rotation angle in decimal degrees.
     */
    constexpr void setAngle(double angle) noexcept;

    /*!
     * \brief Returns the symbol shape.
     * \return Symbol shape.
     */
    [[nodiscard]]
    constexpr auto shape() const noexcept -> Shape;

    /*!
     * \brief Sets the symbol shape.
     * \param[in] shape The shape to use for rendering the symbol.
     */
    constexpr void setShape(Shape shape) noexcept;

    /*!
     * \brief Gets the fill color of the symbol.
     * \return Symbol fill color.
     */
    [[nodiscard]]
    constexpr auto color() const noexcept -> Color;

    /*!
     * \brief Set the color
     * \param[in] color Color
     * \see Color
     */
    constexpr void setColor(Color color) noexcept;

    /*!
     * \brief Gets the outline color of the symbol.
     * \return Outline color.
     */
    [[nodiscard]]
    constexpr auto outlineColor() const noexcept -> Color;

    /*!
     * \brief Set the outline color
     * \param[in] outlinecolor Outline color
     * \see Color
     */
    constexpr void setOutlineColor(Color outlinecolor) noexcept;

    /*!
     * \brief Gets the scale factor applied to the symbol.
     * \return Scaling factor (1.0 = no scaling).
     */
    [[nodiscard]]
    constexpr auto scalingFactor() const noexcept -> double;

    /*!
     * \brief Sets the scale factor of the symbol.
     * \param[in] scalingFactor Factor to scale the symbol size.
     */
    constexpr void setScalingFactor(double scalingFactor) noexcept;

    [[nodiscard]]
    constexpr auto offset() const noexcept -> Vector2d;

    /*!
     * \brief Sets the offset from the insertion point.
     * \param[in] dx Horizontal offset.
     * \param[in] dy Vertical offset.
     */
    constexpr void setOffset(double dx, double dy) noexcept;

    constexpr void setOffset(Vector2d offset) noexcept;

};


constexpr auto Symbol::angle() const noexcept -> double
{
    return mAngle;
}

constexpr void Symbol::setAngle(double angle) noexcept
{
    mAngle = angle;
}

constexpr auto Symbol::shape() const noexcept -> Shape
{
    return mShape;
}

constexpr void Symbol::setShape(Shape shape) noexcept
{
    mShape = shape;
}

constexpr auto Symbol::color() const noexcept -> Color
{
    return mColor;
}

constexpr void Symbol::setColor(Color color) noexcept
{
    mColor = std::move(color);
}

constexpr auto Symbol::outlineColor() const noexcept -> Color
{
    return mOutlineColor;
}

constexpr void Symbol::setOutlineColor(Color outlineColor) noexcept
{
    mOutlineColor = std::move(outlineColor);
}

constexpr auto Symbol::scalingFactor() const noexcept -> double
{
    return mScalingFactor;
}

constexpr void Symbol::setScalingFactor(double scalingFactor) noexcept
{
    mScalingFactor = scalingFactor;
}

constexpr auto Symbol::offset() const noexcept -> Vector2d
{
    return mOffset;
}

constexpr void Symbol::setOffset(double dx, double dy) noexcept
{
    mOffset[0] = dx;
    mOffset[1] = dy;
}

constexpr void Symbol::setOffset(Vector2d offset) noexcept
{
    mOffset = std::move(offset);
}


/*! \} */ 

} // End namespace tl
