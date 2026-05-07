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

#include "tidop/core/base/defs.h"
#include "tidop/graphic/color.h"

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

#ifdef TL_WARNING_DEPRECATED_METHOD
    /*!
     * \brief Pen names
     */
    enum class TL_DEPRECATED(PredefinedPattern, "4.0") Name : uint8_t
    {
        solid,             /*!< Solid (the default value when no ID is provided) */
        null,              /*!< Null pen (invisible) */
        dash,              /*!<  */
        short_dash,        /*!<  */
        long_dash,         /*!<  */
        dot_line,          /*!< Dotted line */
        dash_dot_line,     /*!< Dash-dot line */
        dash_dot_dot_line, /*!< Dash-dot-dot line */
        alternate_line     /*!< Alternate line */
    };
#endif // TL_WARNING_DEPRECATED_METHOD

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
    uint8_t mWidth;
    std::string mPattern;
    PredefinedPattern mPredefinedPattern;
    Cap mCap;
    Join mJoin;
    int32_t mPerpendicularOffset;
    uint32_t mPriorityLevel;

public:

    /*!
     * \brief Default constructor
     */
    Pen();

    /*!
     * \brief Copy constructor
     * \param[in] pen Pen style class being copied
     */
    Pen(const Pen &pen);

    /*!
     * \brief Move constructor
     */
    Pen(Pen &&pen) TL_NOEXCEPT;

    /*!
     * \brief Destructor
     */
    ~Pen();

    /*!
     * \brief Returns the pen color.
     * \return Pen color.
     */
    auto color() const -> Color;

    /*!
     * \brief Sets the pen color.
     * \param[in] color Pen color.
     */
    void setColor(const Color &color);

    /*!
     * \brief Returns the pen width in pixels or drawing units.
     * \return Pen width.
     */
    auto width() const -> uint8_t;

    /*!
     * \brief Sets the pen width.
     * \param[in] width Pen width.
     */
    void setWidth(uint8_t width);

    /*!
     * \brief Returns the custom line pattern string (e.g., for dash arrays).
     * \return Pattern string.
     */
    auto pattern() const -> std::string;

    /*!
     * \brief Sets a custom line pattern string.
     * \param[in] pattern Pattern string.
     */
    void setPattern(const std::string& pattern);

    /*!
     * \brief Returns the predefined pattern type.
     * \return Predefined line pattern.
     */
    auto predefinedPattern() const->PredefinedPattern;

    /*!
     * \brief Sets the predefined pattern type.
     * \param[in] pattern Predefined line pattern.
     */
    void setPredefinedPattern(PredefinedPattern pattern);

    /*!
     * \brief Returns the cap style for line ends.
     * \return Cap style.
     */
    auto cap() const -> Cap;

    /*!
     * \brief Sets the cap style for line ends.
     * \param[in] cap Cap style.
     */
    void setCap(Cap pencap);

    /*!
     * \brief Returns the join style between line segments.
     * \return Join style.
     */
    auto join() const -> Join;

    /*!
     * \brief Sets the join style between line segments.
     * \param[in] join Join style.
     */
    void setJoin(Join join);

    /*!
     * \brief Returns the perpendicular offset from the centerline.
     * \return Offset in drawing units (negative = left).
     */
    auto perpendicularOffset() const -> int32_t;

    /*!
     * \brief Sets the perpendicular offset from the centerline.
     * \param[in] offset Offset value (negative = left).
     */
    void setPerpendicularOffset(int32_t perpendicularoffset);

    /*!
     * \brief Returns the rendering priority level.
     * \return Priority level.
     */
    auto priorityLevel() const -> uint32_t;

    /*!
     * \brief Sets the rendering priority level.
     * \param[in] level Priority level.
     */
    void setPriorityLevel(uint32_t priorityLevel);

    /*!
     * \brief Assignment operator
     * \param[in] stylePen Pen style
     * \return Reference to the pen style
     */
    auto operator =(const Pen& stylePen) -> Pen&;

    /*!
     * \brief Assignment move operator
     * \param[in] stylePen Pen style
     * \return Reference to the pen style
     */
    auto operator =(Pen &&stylePen) TL_NOEXCEPT -> Pen &;

#ifdef TL_WARNING_DEPRECATED_METHOD
    /*!
     * \brief Get the pen name or ID
     * \return Pen name or ID
     * \deprecated Use `predefinedPattern()` instead.
     */
    TL_DEPRECATED("predefinedPattern()", "4.0")
    auto name() const->Name;

    /*!
     * \brief Set the pen name or ID
     * \param[in] name Pen name or ID
     * \deprecated Use `setPredefinedPattern()` instead.
     */
    TL_DEPRECATED("setPredefinedPattern()", "4.0")
    void setName(Name name);
#endif // TL_WARNING_DEPRECATED_METHOD

};

/*! \} */

} // End namespace tl
