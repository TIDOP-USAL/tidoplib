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
 * \brief Estilo de pluma
 */
class TL_EXPORT Pen
{
public:

    /*!
     * \brief Pen names
     */
    enum class Name : uint8_t
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

    /*!
     * \brief End point shapes of lines.
     */
    enum class Cap : uint8_t
    {
        butt,           /*!< The ends of the line do not extend beyond the endpoints. This is the default value. */
        round,          /*!< End lines with a circle whose diameter is equal to the width of the line. */
        projective      /*!< Similar to Butt, but the ends of the line extend halfway past the endpoints.*/
    };

    /*!
     * \brief Join point shape (vertex) of lines.
     */
    enum class Join : uint8_t
    {
        miter,      /*!< Extend the outer edge of the lines until they touch. This is the default value */
        rounded,    /*!< Join lines with an arc whose center is at the junction point and whose diameter is equal to the width of the line */
        bevel       /*!< Beveled join */
    };
private:

    /*!
     * \brief Pen color
     * \see Color
     */
    Color mColor;

    /*!
     * \brief Pen width
     */
    uint8_t mWidth;

    /*!
     * \brief Pattern
     */
    std::string mPattern;

    /*!
     * \brief Pen name or id
     * \see Name
     */
    Name mName;

    /*!
     * \brief End point shape of lines
     * \see Cap
     */
    Cap mCap;

    /*!
     * \brief Join point shape (vertex) of lines
     * \see Join
     */
    Join mJoin;

    /*!
     * \brief Offset from the center of the line.
     * If negative, it is drawn to the left.
     */
    int32_t mPerpendicularOffset;

    /*!
     * \brief Priority level
     */
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
     * \brief Get the pen color
     * \return Pen color
     * \see Color
     */
    auto color() const -> Color;

    /*!
     * \brief Set the pen color
     * \param[in] color Pen color
     * \see Color
     */
    void setColor(const Color &color);

    /*!
     * \brief Get the pen width
     * \return Pen width
     */
    auto width() const -> uint8_t;

    /*!
     * \brief Set the pen width
     * \param[in] width Pen width
     */
    void setWidth(uint8_t width);

    /*!
     * \brief Get the pen pattern
     * \return Pen pattern
     */
    auto pattern() const -> std::string;

    /*!
     * \brief Set the pen pattern
     * \param[in] pattern Pen pattern
     */
    void setPattern(const std::string& pattern);

    /*!
     * \brief Get the pen name or ID
     * \return Pen name or ID
     */
    auto name() const -> Name;

    /*!
     * \brief Set the pen name or ID
     * \param[in] name Pen name or ID
     */
    void setName(Name name);

    /*!
     * \brief Get the shape of line endpoints
     * \return Shape of line endpoints
     */
    auto cap() const -> Cap;

    /*!
     * \brief Set the shape of line endpoints
     * \param[in] pencap Shape of line endpoints
     */
    void setCap(Cap pencap);

    /*!
     * \brief Get the shape of line junction points (vertices)
     * \return Shape of line junction points (vertices)
     */
    auto join() const -> Join;

    /*!
     * \brief Set the shape of line junction points (vertices)
     * \param[in] join Shape of line junction points (vertices)
     */
    void setJoin(Join join);

    /*!
     * \brief Get the perpendicular offset from the center of the line
     * \return Perpendicular offset from the center of the line
     */
    auto perpendicularOffset() const -> int32_t;

    /*!
     * \brief Set the perpendicular offset from the center of the line
     * If negative, it draws to the left
     * \param[in] perpendicularoffset Perpendicular offset from the center of the line
     */
    void setPerpendicularOffset(int32_t perpendicularoffset);

    /*!
     * \brief Get the priority level
     * \return Priority level
     */
    auto priorityLevel() const -> uint32_t;

    /*!
     * \brief Set the priority level
     * \param priorityLevel Priority level
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
};

/*! \} */

} // End namespace tl
