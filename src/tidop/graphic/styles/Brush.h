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
 * The `Brush` class encapsulates the style used to fill areas in vector graphics (e.g. polygons).
 * It includes properties such as foreground and background colors, hatch patterns, spacing,
 * orientation, scaling, and a drawing priority level.
 *
 * The brush style determines how a filled region is visually rendered — for example,
 * with solid color, no fill, or hatch patterns such as diagonal or crosshatch lines.
 *
 * ### Example Usage
 * \code
 * BrushPattern pattern(BrushPattern::HatchType::cross, 0.0, 2.0);
 * brush = Brush(Color(Color::Name::green),
 *               Color(Color::Name::dark_green),
 *               pattern);
 * \endcode
 *
 * \see Color, GraphicStyle
 */
class TL_EXPORT Brush
{

public:

    enum class Style : uint8_t
    {
        solid,
        null,
        hatch
    };

private:

    Color mForeColor;
    Color mBackColor;
    Style mStyle = Style::solid;
    std::optional<BrushPattern> mPattern = std::nullopt;

public:

    /*!
     * \brief Default constructor
     */
    constexpr Brush() = default;

    /*!
     * \brief Copy constructor
     * \param[in] brush Brush object being copied
     */
    constexpr Brush(const Brush &brush) = default;

    /*!
     * \brief Move constructor
     * \param[in] brush Brush object being copied
     */
    constexpr Brush(Brush &&brush) noexcept = default;

    constexpr Brush(Color foreground,
                    Color background,
                    Style style = Style::solid)
      : mForeColor(std::move(foreground)),
        mBackColor(std::move(background))
    {
        setStyle(style);
    }

    constexpr Brush(Color foreground,
                    Color background,
                    BrushPattern pattern)
      : mForeColor(std::move(foreground)),
        mBackColor(std::move(background)),
        mStyle(Style::hatch),
        mPattern(pattern)
    { }

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
     * \brief Gets the foreground color of the brush pattern.
     * \return Foreground color used for the hatch lines or solid fill.
     * \see Color
     */
    [[nodiscard]] 
    constexpr auto foregroundColor() const noexcept -> Color;

    /*!
     * \brief Sets the foreground color of the brush pattern.
     * \param[in] foregroundColor Color for the lines or fill.
     * \see Color 
     */
    constexpr void setForegroundColor(Color foregroundColor) noexcept;

    /*!
     * \brief Gets the background color behind the pattern.
     * \return Background color.
     * \see Color
     */
    [[nodiscard]]
    constexpr auto backgroundColor() const noexcept -> Color;

    /*!
     * \brief Sets the background color behind the pattern.
     * \param[in] backgroundColor Color to appear behind the hatch lines or fill.
     * \see Color
     */
    constexpr void setBackgroundColor(Color backgroundColor) noexcept;

    [[nodiscard]] 
    constexpr auto style() const noexcept -> Style { return mStyle; }

    constexpr void setStyle(Style style) noexcept
    {
        mStyle = style;
        if (mStyle == Style::hatch && !mPattern.has_value()) {
            mPattern = BrushPattern();
        }
    }

    [[nodiscard]] constexpr auto pattern() const noexcept -> const std::optional<BrushPattern> & { return mPattern; }
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
