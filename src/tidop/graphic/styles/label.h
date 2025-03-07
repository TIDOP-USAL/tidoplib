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

#include "tidop/core/base/defs.h"
#include "tidop/graphic/font.h"
#include "tidop/graphic/color.h"

namespace tl
{


/*! \addtogroup Styles
 *  \{
 */


/*!
 * \brief Text style class
 */
class TL_EXPORT Label
{
public:

    /*!
     * \brief How the text is drawn relative to the feature's geometry
     */
    enum class Placement : uint8_t
    {
        point,                /*!< A simple label is attached to a point or to the first vertex of a polyline. */
        last_vertex,          /*!< Text is attached to the last vertex of a polyline. A PEN tool can be combined with this LABEL tool to draw the polyline as a leader to the label. */
        stretched,            /*!< Stretch the text string along a polyline, with an equal spacing between each character. */
        middle,               /*!< Place text as a single label at the middle of a polyline (based on total line length). */
        word_per_segment,     /*!< One word per line segment in a polyline. */
        horizontal,           /*!< Every word of text attached to polyline is placed horizontally in its segment, anchor point is a center of segment. */
        stretched_to_segment  /*!< Every word of text attached to polyline is stretched to fit the segment of polyline and placed along that segment. The anchor point is a start of a segment. */
    };

    /*!
     * \brief Position of the label relative to the insertion point
     */
    enum class AnchorPosition : uint8_t
    {
        vertical_baseline = 1 << 0,   /*!<  */
        vertical_center = 1 << 1,     /*!<  */
        vertical_top = 1 << 2,        /*!<  */
        vertical_bottom = 1 << 3,     /*!<  */
        horizontal_left = 1 << 4,     /*!<  */
        horizontal_center = 1 << 5,   /*!<  */
        horizontal_right = 1 << 6     /*!<  */
    };

private:

    /*!
     * \brief Font
     */
    Font mFont;

    /*!
     * \brief Label text
     */
    std::string mLicenseText;

    /*!
     * \brief Rotation angle in decimal sexagesimal degrees
     * \see angleConversion
     */
    double mAngle;

    /*!
     * \brief Foreground color
     * \see Color
     */
    Color mForegroundColor;

    /*!
     * \brief Background color
     * \see Color
     */
    Color mBackgroundColor;

    /*!
     * \brief Outline color
     * \see Color
     */
    Color mOutlineColor;

    /*!
     * \brief Shadow color
     * \see Color
     */
    Color mShadowColor;

    /*!
     * \brief Scaling in percentage
     */
    double mStretch;

    /*!
     * \brief Label placement mode in entities
     * \see Placement
     */
    Placement mPlacement;

    /*!
     * \brief Anchor position of the label
     * \see AnchorPosition
     */
    AnchorPosition mAnchorPosition;

    /*!
     * \brief X and Y offset of the label insertion point
     */
    std::array<double, 2> mOffset;

    /*!
     * \brief Perpendicular distance between the label and the line along which it is placed
     */
    int mPerpendicularOffset;

    /*!
     * \brief Strikethrough
     */
    bool mStrikeout;

    /*!
     * \brief mPriorityLevel
     */
    uint32_t mPriorityLevel;

public:

    Label();
    Label(const Label &label);
    Label(Label &&label) TL_NOEXCEPT;
    ~Label();

    /*!
     * \brief Get the label text
     * \return The label text
     */
    auto text() const -> std::string;

    /*!
     * \brief Set the label text
     * \param[in] text The label text
     */
    void setText(const std::string &text);

    /*!
     * \brief Get the rotation angle
     * \return The rotation angle in decimal sexagesimal degrees
     * \see angleConversion
     */
    auto angle() const -> double;

    /*!
     * \brief Set the rotation angle
     * \param[in] angle The rotation angle in decimal sexagesimal degrees
     * \see angleConversion
     */
    void setAngle(double angle);

    /*!
     * \brief Get the foreground color
     * \return The foreground color
     * \see Color
     */
    auto foregroundColor() const -> Color;

    /*!
     * \brief Set the foreground color
     * \param[in] color The foreground color
     * \see Color
     */
    void setForegroundColor(const Color &color);

    /*!
     * \brief Get the background color
     * \return The background color
     * \see Color
     */
    auto backgroundColor() const -> Color;

    /*!
     * \brief Set the background color
     * \param[in] color The background color
     * \see Color
     */
    void setBackgroundColor(const Color &color);

    /*!
     * \brief Get the outline color
     * \return The outline color
     * \see Color
     */
    auto outlineColor() const -> Color;

    /*!
     * \brief Set the outline color
     * \param[in] color The outline color
     * \see Color
     */
    void setOutlineColor(const Color &color);

    /*!
     * \brief Get the shadow color
     * \return The shadow color
     * \see Color
     */
    auto shadowColor() const -> Color;

    /*!
     * \brief Set the shadow color
     * \param[in] color The shadow color
     * \see Color
     */
    void setShadowColor(const Color &color);

    /*!
     * \brief Get the stretch factor
     * \return The stretch factor
     */
    auto stretch() const -> double;

    /*!
     * \brief Set the stretch factor
     * \param[in] stretch The stretch factor
     */
    void setStretch(double stretch);

    /*!
     * \brief Get the label placement mode
     * \return The label placement mode
     * \see Placement
     */
    auto placement() const -> Placement;

    /*!
     * \brief Set the label placement mode
     * \param[in] placement The label placement mode
     * \see Placement
     */
    void setPlacement(Placement placement);

    /*!
     * \brief Get the label anchor position
     * \return The label anchor position
     * \see AnchorPosition
     */
    auto anchorPosition() const -> AnchorPosition;

    /*!
     * \brief Set the label anchor position
     * \param[in] anchorPosition The label anchor position
     * \see AnchorPosition
     */
    void setAnchorPosition(AnchorPosition anchorPosition);

    /*!
     * \brief Get the X offset of the label insertion point
     * \return The X offset of the label insertion point
     */
    auto offsetX() const -> double;

    /*!
     * \brief Get the Y offset of the label insertion point
     * \return The Y offset of the label insertion point
     */
    auto offsetY() const -> double;

    /*!
     * \brief Set the offset of the label insertion point
     * \param[in] dx The X offset of the label insertion point
     * \param[in] dy The Y offset of the label insertion point
     */
    void setOffset(double dx, double dy);

    /*!
     * \brief Assignment operator
     * \param label The label style
     * \return Reference to the label style
     */
    auto operator =(const Label& label) -> Label&;

    /*!
     * \brief Assignment move operator
     * \param label The label style
     * \return Reference to the label style
     */
    auto operator =(Label &&label) TL_NOEXCEPT -> Label &;

    void setFont(const Font &font);
    auto font() const -> Font;
    auto perpendicularOffset() const -> int;
    void setPerpendicularOffset(int perpendicularOffset);



};
ALLOW_BITWISE_FLAG_OPERATIONS(Label::AnchorPosition)



/*! \} */ 

} // End namespace tl
