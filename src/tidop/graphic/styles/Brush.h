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

/*! \file Brush.h
 * \brief Brush style for filling graphical shapes.
 *
 * This file defines the `Brush` class, which encapsulates the fill style used
 * for rendering filled shapes such as polygons. It supports solid fills, no fill,
 * and hatch patterns with configurable foreground and background colors.
 *
 * \ingroup Graphics
 * \see tl::Pen, tl::GraphicStyle, tl::BrushPattern
 */

#pragma once

#include <array>
#include <optional>

#include "tidop/config.h"
#include "tidop/graphic/Color.h"
#include "tidop/graphic/styles/BrushPattern.h"

namespace tl
{


/*! \addtogroup Styles
 *  \{
 */


/*!
 * \class Brush
 * \brief Represents a brush style used to fill graphical shapes.
 *
 * The `Brush` class encapsulates the style used to fill areas in vector graphics
 * (e.g., polygons). It includes properties such as foreground and background colors,
 * hatch patterns, and a drawing style.
 *
 * The brush style determines how a filled region is visually rendered — for example,
 * with solid color, no fill, or hatch patterns such as diagonal or crosshatch lines.
 *
 * ### Example
 * \code
 * Brush solidBrush(Color::Red, Color::Red, Brush::Style::solid);
 * Brush hatchBrush(Color::Blue, Color::White, BrushPattern(BrushPattern::HatchType::cross, 0.0, 2.0));
 * \endcode
 *
 * \see Color, GraphicStyle, BrushPattern
 */
class TL_EXPORT Brush
{

public:

    /*!
     * \enum Style
     * \brief Brush fill styles.
     */
    enum class Style : uint8_t
    {
        solid,   /*!< Solid fill using the foreground color. */
        no_fill, /*!< No fill (transparent). */
        hatch    /*!< Hatch pattern using foreground and background colors. */
    };

private:

    Color mForeColor;
    Color mBackColor;
    Style mStyle = Style::solid;
    std::optional<BrushPattern> mPattern = std::nullopt;

public:

    /*!
     * \brief Default constructor.
     * Creates a solid brush with default (black) colors.
     */
    constexpr Brush() = default;

    /*!
     * \brief Copy constructor.
     * \param[in] brush Brush object to copy.
     */
    constexpr Brush(const Brush &brush) = default;

    /*!
     * \brief Move constructor.
     * \param[in] brush Brush object to move.
     */
    constexpr Brush(Brush &&brush) noexcept = default;

    /*!
     * \brief Constructs a brush with a style.
     * \param[in] foreground Foreground color.
     * \param[in] background Background color.
     * \param[in] style      Brush style (default: `Style::solid`).
     */
    constexpr Brush(Color foreground,
                    Color background,
                    Style style = Style::solid)
      : mForeColor(std::move(foreground)),
        mBackColor(std::move(background))
    {
        setStyle(style);
    }

    /*!
     * \brief Constructs a hatch brush.
     * \param[in] foreground Foreground color (hatch lines).
     * \param[in] background Background color (behind the hatch).
     * \param[in] pattern    Hatch pattern.
     */
    constexpr Brush(Color foreground,
                    Color background,
                    BrushPattern pattern)
      : mForeColor(std::move(foreground)),
        mBackColor(std::move(background)),
        mStyle(Style::hatch),
        mPattern(pattern)
    {
    }

    /*!
     * \brief Copy assignment operator.
     * \param[in] brush Brush to copy.
     * \return Reference to the assigned object.
     */
    constexpr auto operator =(const Brush &brush) -> Brush & = default;

    /*!
     * \brief Move assignment operator.
     * \param[in] brush Brush to move.
     * \return Reference to the assigned object.
     */
    constexpr auto operator =(Brush &&brush) noexcept -> Brush & = default;

    /*!
     * \brief Returns the foreground color.
     * \return Foreground color (used for solid fill or hatch lines).
     */
    [[nodiscard]]
    constexpr auto foregroundColor() const noexcept -> Color;

    /*!
     * \brief Sets the foreground color.
     * \param[in] foregroundColor New foreground color.
     */
    constexpr void setForegroundColor(Color foregroundColor) noexcept;

    /*!
     * \brief Returns the background color.
     * \return Background color (used behind hatch patterns).
     */
    [[nodiscard]]
    constexpr auto backgroundColor() const noexcept -> Color;

    /*!
     * \brief Sets the background color.
     * \param[in] backgroundColor New background color.
     */
    constexpr void setBackgroundColor(Color backgroundColor) noexcept;

    /*!
     * \brief Returns the current brush style.
     * \return The brush style.
     */
    [[nodiscard]]
    constexpr auto style() const noexcept -> Style { return mStyle; }

    /*!
     * \brief Sets the brush style.
     * \param[in] style New brush style.
     * \note If the style is set to `hatch` and no pattern exists, a default
     *       `BrushPattern` is created.
     */
    constexpr void setStyle(Style style) noexcept
    {
        mStyle = style;
        if (mStyle == Style::hatch && !mPattern.has_value()) {
            mPattern = BrushPattern();
        }
    }

    /*!
     * \brief Returns the hatch pattern.
     * \return Optional reference to the current pattern (empty if not set).
     */
    [[nodiscard]] constexpr auto pattern() const noexcept -> const std::optional<BrushPattern> & { return mPattern; }

    /*!
     * \brief Sets the hatch pattern and switches the style to `hatch`.
     * \param[in] pattern New hatch pattern.
     */
    constexpr void setPattern(BrushPattern pattern) noexcept
    {
        mPattern = pattern;
        mStyle = Style::hatch;
    }

    auto operator<=>(const Brush &) const = default;

};

constexpr auto Brush::foregroundColor() const noexcept -> Color
{
    return mForeColor;
}

constexpr void Brush::setForegroundColor(Color foregroundColor) noexcept
{
    mForeColor = std::move(foregroundColor);
}

constexpr auto Brush::backgroundColor() const noexcept -> Color
{
    return mBackColor;
}

constexpr void Brush::setBackgroundColor(Color backgroundColor) noexcept
{
    mBackColor = std::move(backgroundColor);
}


/*! \} */ 

} // namespace tl
