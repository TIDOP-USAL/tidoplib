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

/*! \file Symbol.h
 * \brief Point symbol style for marker rendering.
 *
 * This file defines the `Symbol` class, which encapsulates the visual style
 * for point markers. It includes shape, color, outline color, rotation,
 * scaling, and offset from the insertion point.
 *
 * \ingroup Graphics
 * \see tl::GraphicStyle, tl::Color
 */

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
 * \class Symbol
 * \brief Point symbol style for marker rendering.
 *
 * The `Symbol` class defines how a point feature is rendered on a canvas.
 * It includes a predefined shape (e.g., circle, square, cross), fill color,
 * outline color, rotation angle, scaling factor, and offset from the
 * insertion point.
 *
 * ### Example
 * \code
 * Symbol sym(Symbol::Shape::circle_filled, Color::Red);
 * sym.setOutlineColor(Color::Black);
 * sym.setAngle(45.0);
 * sym.setScalingFactor(1.5);
 * sym.setOffset(2.0, -3.0);
 * \endcode
 *
 * \see GraphicStyle, Color
 */
class TL_EXPORT Symbol
{

public:

    /*!
     * \enum Shape
     * \brief Symbol shapes used to represent points.
     *
     * These shapes are rendered at the symbol insertion point, optionally
     * rotated or scaled.
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
    Color mColor;
    Color mOutlineColor;    
    double mAngle = 0.0;
    double mScalingFactor = 1.0;
    Vector2d mOffset = {0., 0.};

public:

     /*!
      * \brief Default constructor.
      * Creates a black cross symbol with no rotation or scaling.
      */
    constexpr Symbol() = default;

    /*!
     * \brief Constructs a symbol with a shape and color.
     * \param[in] shape Symbol shape.
     * \param[in] color Fill color (default: black).
     */
    constexpr Symbol(Shape shape, Color color = Color(Color::Name::black))
      : mShape(shape),
        mColor(color)
    {
    }

    /*!
     * \brief Copy constructor.
     * \param[in] symbol Symbol to copy.
     */
    constexpr Symbol(const Symbol &symbol) = default;

    /*!
     * \brief Move constructor.
     * \param[in] symbol Symbol to move.
     */
    constexpr Symbol(Symbol &&symbol) noexcept = default;

    /*! \brief Destructor. */
    ~Symbol() = default;

    /*!
     * \brief Copy assignment operator.
     * \param[in] symbol Symbol to copy.
     * \return Reference to this object.
     */
    constexpr auto operator =(const Symbol &symbol) -> Symbol & = default;

    /*!
     * \brief Move assignment operator.
     * \param[in] symbol Symbol to move.
     * \return Reference to this object.
     */
    constexpr auto operator =(Symbol &&symbol) noexcept -> Symbol & = default;

    /*!
     * \brief Returns the symbol shape.
     * \return The current shape.
     */
    [[nodiscard]]
    constexpr auto shape() const noexcept -> Shape;

    /*!
     * \brief Sets the symbol shape.
     * \param[in] shape The new shape.
     */
    constexpr void setShape(Shape shape) noexcept;

    /*!
     * \brief Returns the fill color.
     * \return The fill color.
     */
    [[nodiscard]]
    constexpr auto color() const noexcept -> Color;

    /*!
     * \brief Sets the fill color.
     * \param[in] color The new fill color.
     */
    constexpr void setColor(Color color) noexcept;

    /*!
     * \brief Returns the outline color.
     * \return The outline color.
     */
    [[nodiscard]]
    constexpr auto outlineColor() const noexcept -> Color;

    /*!
     * \brief Sets the outline color.
     * \param[in] outlineColor The new outline color.
     */
    constexpr void setOutlineColor(Color outlineColor) noexcept;

    /*!
     * \brief Returns the rotation angle.
     * \return Angle in degrees.
     */
    [[nodiscard]]
    constexpr auto angle() const noexcept -> double;

    /*!
     * \brief Sets the rotation angle.
     * \param[in] angle Angle in degrees.
     */
    constexpr void setAngle(double angle) noexcept;

    /*!
     * \brief Returns the scaling factor.
     * \return Scaling factor (1.0 = no scaling).
     */
    [[nodiscard]]
    constexpr auto scalingFactor() const noexcept -> double;

    /*!
     * \brief Sets the scaling factor.
     * \param[in] scalingFactor New scaling factor.
     */
    constexpr void setScalingFactor(double scalingFactor) noexcept;

    /*!
     * \brief Returns the offset from the insertion point.
     * \return Offset vector.
     */
    [[nodiscard]]
    constexpr auto offset() const noexcept -> Vector2d;

    /*!
     * \brief Sets the offset from the insertion point.
     * \param[in] dx Horizontal offset.
     * \param[in] dy Vertical offset.
     */
    constexpr void setOffset(double dx, double dy) noexcept;

    /*!
     * \brief Sets the offset from the insertion point.
     * \param[in] offset Offset vector.
     */
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

} // namespace tl
