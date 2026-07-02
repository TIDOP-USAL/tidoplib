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
#include "tidop/math/algebra/vector/Vector.h"
#include "tidop/graphic/Color.h"

namespace tl
{


/*! \addtogroup Styles
 *  \{
 */

class TL_EXPORT BrushPattern
{

public:

    /*!
     * \enum HatchType
     * \brief Predefined brush fill patterns.
     *
     * | Enum Value         | Visual Pattern | Description                         |
     * |--------------------|----------------|-------------------------------------|
     * | `horizontal` | ──────         | Horizontal lines.                  |
     * | `vertical`   | ||||||         | Vertical lines.                    |
     * | `fdiagonal`  | \\\\\\\\        | Forward diagonals (`/`).           |
     * | `bdiagonal`  | //////          | Backward diagonals (`\`).          |
     * | `cross`      | ++++++          | Horizontal and vertical lines.     |
     * | `diagcross`  | xxxxxx          | Diagonal crosshatch.               |
     */
    enum class HatchType : uint8_t
    {
        horizontal,  /*!< ────── */
        vertical,    /*!< |||||| */
        fdiagonal,   /*!< \\\\\\ */
        bdiagonal,   /*!< ////// */
        cross,       /*!< ++++++ */
        diagcross    /*!< xxxxxx */
    };

private:

    HatchType mHatchType = HatchType::horizontal;
    double mAngle = 0.0;
    double mScalingFactor = 1.0;
    tl::Vector2d mSpacing = {0.0, 0.0};

public:

    constexpr BrushPattern() = default;
    constexpr BrushPattern(const BrushPattern &brushPattern) = default;
    constexpr BrushPattern(BrushPattern &&brushPattern) noexcept = default;
    constexpr BrushPattern(HatchType type, double angle = 0.0, double scale = 1.0)
      : mHatchType(type),
        mAngle(angle), 
        mScalingFactor(scale)
    {
    }

    constexpr auto operator =(const BrushPattern &brushPattern) -> BrushPattern & = default;
    constexpr auto operator =(BrushPattern &&brush) noexcept -> BrushPattern & = default;

    [[nodiscard]] 
    constexpr auto hatchType() const noexcept -> HatchType { return mHatchType; }
    constexpr void setHatchType(HatchType type) noexcept { mHatchType = type; }

    /*!
     * \brief Gets the rotation angle of the brush pattern.
     * \return Angle in decimal degrees.
     *
     * This affects the orientation of hatch lines or patterns.
     */
    [[nodiscard]]
    constexpr auto angle() const noexcept -> double;

    /*!
     * \brief Sets the rotation angle of the brush pattern.
     * \param[in] angle Angle in decimal degrees.
     */
    constexpr void setAngle(double angle) noexcept;

    /*!
     * \brief Gets the scale applied to the pattern.
     * \return Scaling factor (1.0 = no scaling).
     *
     * Useful to zoom in or out the density of the brush pattern.
     */
    [[nodiscard]]
    constexpr auto scalingFactor() const noexcept -> double;

    /*!
     * \brief Sets the scale of the pattern.
     * \param[in] scalingFactor Scaling factor to apply.
     */
    constexpr void setScalingFactor(double scalingFactor) noexcept;

    [[nodiscard]]
    constexpr auto spacing() const noexcept -> const Vector2d &;

    /*!
     * \brief Sets the spacing of the pattern in both X and Y directions.
     * \param[in] spacingX Horizontal spacing.
     * \param[in] spacingY Vertical spacing.
     */
    constexpr void setSpacing(Vector2d spacing) noexcept;
    constexpr void setSpacing(double spacingX, double spacingY) noexcept;

    auto operator<=>(const BrushPattern &) const = default;
};


constexpr auto BrushPattern::angle() const noexcept -> double
{
    return mAngle;
}

constexpr void BrushPattern::setAngle(double angle) noexcept
{
    mAngle = angle;
}

constexpr auto BrushPattern::scalingFactor() const noexcept -> double
{
    return mScalingFactor;
}

constexpr void BrushPattern::setScalingFactor(double scalingFactor) noexcept
{
    mScalingFactor = scalingFactor;
}

constexpr auto BrushPattern::spacing() const noexcept -> const Vector2d &
{
    return mSpacing;
}

constexpr void BrushPattern::setSpacing(Vector2d spacing) noexcept
{
    mSpacing = std::move(spacing);
}

constexpr void BrushPattern::setSpacing(double spacingX, double spacingY) noexcept
{
    mSpacing[0] = spacingX;
    mSpacing[1] = spacingY;
}


/*! \} */ 

} // namespace tl
