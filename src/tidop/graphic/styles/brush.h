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
 * Brush brush;
 * brush.setPattern(Brush::Pattern::cross_hatch);
 * brush.setForegroundColor(Color(Color::Name::black));
 * brush.setBackgroundColor(Color(Color::Name::white));
 * brush.setSpacing(5.0, 5.0);
 * \endcode
 *
 * \see Color, GraphicStyle
 */
class TL_EXPORT Brush
{

public:

    /*!
     * \enum Pattern
     * \brief Predefined brush fill patterns.
     *
     * | Enum Value         | Visual Pattern | Description                         |
     * |--------------------|----------------|-------------------------------------|
     * | `solid`            | ██████         | Solid fill (default).              |
     * | `null`             |                | Transparent fill (no pattern).     |
     * | `horizontal_hatch` | ──────         | Horizontal lines.                  |
     * | `vertical_hatch`   | ||||||         | Vertical lines.                    |
     * | `fdiagonal_hatch`  | \\\\\\\\        | Forward diagonals (`/`).           |
     * | `bdiagonal_hatch`  | //////          | Backward diagonals (`\`).          |
     * | `cross_hatch`      | ++++++          | Horizontal and vertical lines.     |
     * | `diagcross_hatch`  | xxxxxx          | Diagonal crosshatch.               |
     */
    enum class Pattern : uint8_t
    {
        solid,             /*!< Solid (default value when no id is provided) */
        null,              /*!< Null brush (invisible) */
        horizontal_hatch,  /*!< ────── */
        vertical_hatch,    /*!< |||||| */
        fdiagonal_hatch,   /*!< \\\\\\ */
        bdiagonal_hatch,   /*!< ////// */
        cross_hatch,       /*!< ++++++ */
        diagcross_hatch    /*!< xxxxxx */
    };
#ifdef TL_WARNING_DEPRECATED_METHOD
    enum class TL_DEPRECATED("Pattern", "4.0") Name : uint8_t
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
#endif // TL_WARNING_DEPRECATED_METHOD

private:

    Color mForeColor;
    Color mBackColor;
    Pattern mPattern;
    double mAngle;
    double mScalingFactor;
    std::array<double, 2> mSpacing;
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
     * \brief Gets the foreground color of the brush pattern.
     * \return Foreground color used for the hatch lines or solid fill.
     * \see Color
     */
    auto foregroundColor() const -> Color;

    /*!
     * \brief Sets the foreground color of the brush pattern.
     * \param[in] foregroundColor Color for the lines or fill.
     * \see Color 
     */
    void setForegroundColor(const Color &foregroundColor);

    /*!
     * \brief Gets the background color behind the pattern.
     * \return Background color.
     * \see Color
     */
    auto backgroundColor() const -> Color;

    /*!
     * \brief Sets the background color behind the pattern.
     * \param[in] backgroundColor Color to appear behind the hatch lines or fill.
     * \see Color
     */
    void setBackgroundColor(const Color &backgroundColor);

    /*!
     * \brief Gets the brush pattern.
     * \return Fill pattern.
     */
    auto pattern() const -> Pattern;

    /*!
     * \brief Sets the brush pattern.
     * \param[in] pattern Fill pattern to use.
     */
    void setPattern(Pattern pattern);

    /*!
     * \brief Gets the rotation angle of the brush pattern.
     * \return Angle in decimal degrees.
     *
     * This affects the orientation of hatch lines or patterns.
     */
    auto angle() const -> double;

    /*!
     * \brief Sets the rotation angle of the brush pattern.
     * \param[in] angle Angle in decimal degrees.
     */
    void setAngle(double angle);

    /*!
     * \brief Gets the scale applied to the pattern.
     * \return Scaling factor (1.0 = no scaling).
     *
     * Useful to zoom in or out the density of the brush pattern.
     */
    auto scalingFactor() const -> double;

    /*!
     * \brief Sets the scale of the pattern.
     * \param[in] scalingFactor Scaling factor to apply.
     */
    void setScalingFactor(double scalingFactor);

    /*!
     * \brief Gets the horizontal spacing of hatch lines.
     * \return Spacing in X direction (in pixels or units).
     */
    auto spacingX() const -> double;

    /*!
     * \brief Gets the vertical spacing of hatch lines.
     * \return Spacing in Y direction (in pixels or units).
     */
    auto spacingY() const -> double;

    /*!
     * \brief Sets the spacing of the pattern in both X and Y directions.
     * \param[in] spacingX Horizontal spacing.
     * \param[in] spacingY Vertical spacing.
     */
    void setSpacing(double spacingX, double spacingY);

    /*!
     * \brief Gets the drawing priority level of the brush.
     * \return Priority level (higher values may draw on top of lower ones).
     */
    auto priorityLevel() const -> uint32_t;

    /*!
     * \brief Sets the drawing priority level.
     * \param[in] priorityLevel Priority for rendering order.
     *
     * This value can be used to determine rendering precedence when multiple layers overlap.
     */
    void setPriorityLevel(uint32_t priorityLevel);

    /*!
     * \brief Copy assignment operator.
     * \param[in] brush Brush to copy.
     * \return Reference to the assigned object.
     */
    auto operator =(const Brush& brush) -> Brush&;

    /*!
     * \brief Move assignment operator.
     * \param[in] brush Brush to move.
     * \return Reference to the assigned object.
     */
    auto operator =(Brush&& brush) TL_NOEXCEPT -> Brush&;

#ifdef TL_WARNING_DEPRECATED_METHOD
    /*!
     * \brief Returns the name or ID of the brush
     * \return Name or ID of the brush
     * \deprecated This method is deprecated (v4.0), use pattern() instead.
     */
    TL_DEPRECATED("pattern()", "4.0")
    auto name() const -> Name;
    /*!
     * \brief Sets the brush pattern.
     * \param[in] pattern Fill pattern to use.
     * \deprecated This method is deprecated (v4.0), use setPattern() instead.
     */
    TL_DEPRECATED("setPattern(Pattern pattern)", "4.0")
    void setName(Name name);
#endif // TL_WARNING_DEPRECATED_METHOD
};


/*! \} */ 

} // End namespace tl
