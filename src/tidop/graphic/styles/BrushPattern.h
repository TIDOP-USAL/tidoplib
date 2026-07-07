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

/*! \file BrushPattern.h
 * \brief Hatch pattern definition for brush fills.
 *
 * This file defines the `BrushPattern` class, which represents a hatch pattern
 * used by the `Brush` class to fill shapes with repeating line patterns. It
 * includes predefined hatch types, rotation angle, scaling factor, and spacing
 * controls.
 *
 * \ingroup Graphics
 * \see tl::Brush, tl::Color
 */

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

/*!
 * \class BrushPattern
 * \brief Hatch pattern used for brush fills.
 *
 * The `BrushPattern` class defines the appearance of a hatch fill, including
 * the type of pattern, rotation angle, scaling factor, and spacing between
 * lines. It is used by the `Brush` class when `Brush::Style::hatch` is set.
 *
 * ### Example
 * \code
 * BrushPattern pattern(BrushPattern::HatchType::cross, 45.0, 1.5);
 * pattern.setSpacing(2.0, 2.0);
 * Brush brush(Color::Blue, Color::White, pattern);
 * \endcode
 *
 * \see Brush, Color
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

    /*!
     * \brief Default constructor.
     * Creates a horizontal hatch pattern with no rotation, unit scaling,
     * and zero spacing.
     */
    constexpr BrushPattern() = default;

    /*!
     * \brief Copy constructor.
     * \param[in] brushPattern Pattern to copy.
     */
    constexpr BrushPattern(const BrushPattern &brushPattern) = default;

    /*!
     * \brief Move constructor.
     * \param[in] brushPattern Pattern to move.
     */
    constexpr BrushPattern(BrushPattern &&brushPattern) noexcept = default;

    /*!
     * \brief Constructs a hatch pattern with a specific type, angle, and scale.
     * \param[in] type  Hatch type.
     * \param[in] angle Rotation angle in degrees (default: 0.0).
     * \param[in] scale Scaling factor (default: 1.0).
     */
    constexpr BrushPattern(HatchType type, double angle = 0.0, double scale = 1.0)
      : mHatchType(type),
        mAngle(angle), 
        mScalingFactor(scale)
    {
    }

    /*!
     * \brief Copy assignment operator.
     * \param[in] brushPattern Pattern to copy.
     * \return Reference to this object.
     */
    constexpr auto operator =(const BrushPattern &brushPattern) -> BrushPattern & = default;

    /*!
     * \brief Move assignment operator.
     * \param[in] brush Pattern to move.
     * \return Reference to this object.
     */
    constexpr auto operator =(BrushPattern &&brush) noexcept -> BrushPattern & = default;

    /*!
     * \brief Returns the hatch type.
     * \return The current hatch type.
     */
    [[nodiscard]] 
    constexpr auto hatchType() const noexcept -> HatchType { return mHatchType; }

    /*!
     * \brief Sets the hatch type.
     * \param[in] type New hatch type.
     */
    constexpr void setHatchType(HatchType type) noexcept { mHatchType = type; }

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
     * \brief Returns the line spacing.
     * \return Vector containing spacing in X and Y directions.
     */
    [[nodiscard]]
    constexpr auto spacing() const noexcept -> const Vector2d &;

    /*!
     * \brief Sets the line spacing.
     * \param[in] spacing Vector containing spacing in X and Y.
     */
    constexpr void setSpacing(Vector2d spacing) noexcept;

    /*!
     * \brief Sets the line spacing using individual coordinates.
     * \param[in] spacingX Spacing in X direction.
     * \param[in] spacingY Spacing in Y direction.
     */
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
