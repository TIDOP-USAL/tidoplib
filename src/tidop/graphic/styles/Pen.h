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

#include "tidop/config.h"
#include "tidop/graphic/Color.h"

namespace tl
{


/*! \addtogroup Styles
 *  \{
 */


 /*!
  * \class Pen
  * \brief Defines the line style used for drawing outlines or strokes.
  *
  * The `Pen` class encapsulates the visual styling properties for drawing linear features such as borders,
  * paths, or contours. This includes color, width, predefined or custom line patterns, line caps,
  * joins, perpendicular offset, and rendering priority.
  *
  * It is typically used in vector drawing, mapping, and styling of geometries.
  *
  * \see Color, Pen::PredefinedPattern, Pen::Cap, Pen::Join
  */
class TL_EXPORT Pen
{
public:

    /*!
     * \enum PredefinedPattern
     * \brief Predefined stroke patterns.
     */
    enum class PredefinedPattern : uint8_t
    {
        solid,              /*!< Solid line (default) */
        null,               /*!< No line (invisible) */
        dash,               /*!< Dashed line */
        short_dash,         /*!< Short dashes */
        long_dash,          /*!< Long dashes */
        dot_line,           /*!< Dotted line */
        dash_dot_line,      /*!< Dash-dot pattern */
        dash_dot_dot_line,  /*!< Dash-dot-dot pattern */
        alternate_line      /*!< Alternate line */
    };

    /*!
     * \enum Cap
     * \brief Defines the shape of the line endpoints.
     */
    enum class Cap : uint8_t
    {
        butt,           /*!< The ends of the line do not extend beyond the endpoints. This is the default value. */
        round,          /*!< End lines with a circle whose diameter is equal to the width of the line. */
        projective      /*!< Similar to Butt, but the ends of the line extend halfway past the endpoints.*/
    };

    /*!
     * \enum Join
     * \brief Defines the shape of the connection between line segments.
     */
    enum class Join : uint8_t
    {
        miter,      /*!< Extend the outer edge of the lines until they touch. This is the default value */
        rounded,    /*!< Join lines with an arc whose center is at the junction point and whose diameter is equal to the width of the line */
        bevel       /*!< Beveled join */
    };

private:

    Color mColor;
    uint8_t mWidth = 1;
    std::string mPattern;
    PredefinedPattern mPredefinedPattern = PredefinedPattern::solid;
    Cap mCap = Cap::butt;
    Join mJoin = Join::bevel;
    int32_t mPerpendicularOffset = 0;

public:

    /*!
     * \brief Default constructor
     */
    constexpr Pen() = default;

    /*!
     * \brief Copy constructor
     * \param[in] pen Pen style class being copied
     */
    constexpr Pen(const Pen &pen) = default;

    /*!
     * \brief Move constructor
     */
    constexpr Pen(Pen &&pen) noexcept = default;

    /*!
     * \brief Destructor
     */
    ~Pen() = default;

    /*!
     * \brief Assignment operator
     * \param[in] stylePen Pen style
     * \return Reference to the pen style
     */
    constexpr auto operator =(const Pen &stylePen) -> Pen& = default;

    /*!
     * \brief Assignment move operator
     * \param[in] stylePen Pen style
     * \return Reference to the pen style
     */
    constexpr auto operator =(Pen &&stylePen) noexcept -> Pen& = default;

    /*!
     * \brief Returns the pen color.
     * \return Pen color.
     */
    [[nodiscard]]
    constexpr auto color() const noexcept -> Color;

    /*!
     * \brief Sets the pen color.
     * \param[in] color Pen color.
     */
    constexpr void setColor(Color color) noexcept;

    /*!
     * \brief Returns the pen width in pixels or drawing units.
     * \return Pen width.
     */
    [[nodiscard]]
    constexpr auto width() const noexcept -> uint8_t;

    /*!
     * \brief Sets the pen width.
     * \param[in] width Pen width.
     */
    constexpr void setWidth(uint8_t width) noexcept;

    /*!
     * \brief Returns the custom line pattern string (e.g., for dash arrays).
     * \return Pattern string.
     */
    [[nodiscard]]
    constexpr auto pattern() const noexcept -> std::string;

    /*!
     * \brief Sets a custom line pattern string.
     * \param[in] pattern Pattern string.
     */
    void setPattern(std::string pattern) noexcept;

    /*!
     * \brief Returns the predefined pattern type.
     * \return Predefined line pattern.
     */
    [[nodiscard]]
    constexpr auto predefinedPattern() const noexcept -> PredefinedPattern;

    /*!
     * \brief Sets the predefined pattern type.
     * \param[in] pattern Predefined line pattern.
     */
    constexpr void setPredefinedPattern(PredefinedPattern pattern) noexcept;

    /*!
     * \brief Returns the cap style for line ends.
     * \return Cap style.
     */
    [[nodiscard]]
    constexpr auto cap() const noexcept  -> Cap;

    /*!
     * \brief Sets the cap style for line ends.
     * \param[in] cap Cap style.
     */
    constexpr void setCap(Cap pencap) noexcept;

    /*!
     * \brief Returns the join style between line segments.
     * \return Join style.
     */
    [[nodiscard]]
    constexpr auto join() const noexcept -> Join;

    /*!
     * \brief Sets the join style between line segments.
     * \param[in] join Join style.
     */
    constexpr void setJoin(Join join) noexcept;

    /*!
     * \brief Returns the perpendicular offset from the centerline.
     * \return Offset in drawing units (negative = left).
     */
    [[nodiscard]]
    constexpr auto perpendicularOffset() const noexcept -> int32_t;

    /*!
     * \brief Sets the perpendicular offset from the centerline.
     * \param[in] offset Offset value (negative = left).
     */
    constexpr void setPerpendicularOffset(int32_t perpendicularoffset) noexcept;

};



constexpr auto Pen::color() const noexcept -> Color
{
    return mColor;
}

constexpr void Pen::setColor(Color color) noexcept
{
    mColor = std::move(color);
}

constexpr auto Pen::width() const noexcept -> uint8_t
{
    return mWidth;
}

constexpr void Pen::setWidth(uint8_t width) noexcept
{
    mWidth = width;
}

constexpr auto Pen::pattern() const noexcept -> std::string
{
    return mPattern;
}

inline void Pen::setPattern(std::string pattern) noexcept
{
    mPattern = std::move(pattern);
}

constexpr auto Pen::predefinedPattern() const noexcept -> PredefinedPattern
{
    return mPredefinedPattern;
}

constexpr void Pen::setPredefinedPattern(PredefinedPattern pattern) noexcept
{
    mPredefinedPattern = pattern;
}

constexpr auto Pen::cap() const noexcept -> Cap
{
    return mCap;
}

constexpr void Pen::setCap(Cap cap) noexcept
{
    mCap = cap;
}

constexpr auto Pen::join() const noexcept -> Join
{
    return mJoin;
}

constexpr void Pen::setJoin(Join join) noexcept
{
    mJoin = join;
}

constexpr auto Pen::perpendicularOffset() const noexcept -> int32_t
{
    return mPerpendicularOffset;
}

constexpr void Pen::setPerpendicularOffset(int32_t perpendicularoffset) noexcept
{
    mPerpendicularOffset = perpendicularoffset;
}


/*! \} */

} // namespace tl
