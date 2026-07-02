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

#include "tidop/config.h"
#include "tidop/math/algebra/vector/Vector.h"
#include "tidop/graphic/styles/Font.h"
#include "tidop/graphic/Color.h"
#include "tidop/graphic/styles/LabelAnchor.h"

namespace tl
{


/*! \addtogroup Styles
 *  \{
 */


/*!
 * \class Label
 * \brief Represents the text style and placement options for map or geometry labels.
 *
 * The `Label` class defines how text is rendered and positioned relative to a geometric feature.
 * It supports font styling, color customization (foreground, background, outline, shadow),
 * alignment, rotation, offsetting, and various placement strategies for polylines or points.
 *
 * Labels are typically used to annotate map features, drawings, or geometries in visualization systems.
 *
 * \see Font, Color, Label::Placement, Label::AnchorPosition
 */
class TL_EXPORT Label
{
public:

    /*!
     * \enum Placement
     * \brief Defines how text is positioned along or around geometric features.
     *
     * Used to determine label placement for polylines and point features.
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

private:

    Font mFont;
    std::string mText;
    double mAngle = 0.0;
    Color mForegroundColor;
    Color mBackgroundColor;
    Color mOutlineColor;
    Color mShadowColor;
    double mStretch = 100.;
    Placement mPlacement = Placement::point;
    LabelAnchor mAnchor;
    Vector2d mOffset = {0.0, 0.0};
    int mPerpendicularOffset = 0;
    bool mStrikeout = false;

public:

    Label() = default;
    Label(const Label &label) = default;
    Label(Label &&label) noexcept = default;
    ~Label() = default;

    /*!
     * \brief Assignment operator
     * \param label The label style
     * \return Reference to the label style
     */
    auto operator =(const Label &label) -> Label & = default;

    /*!
     * \brief Assignment move operator
     * \param label The label style
     * \return Reference to the label style
     */
    auto operator =(Label &&label) noexcept -> Label & = default;

    /*!
     * \brief Get the label text
     * \return The label text
     */
    [[nodiscard]]
    auto text() const noexcept -> std::string;

    /*!
     * \brief Set the label text
     * \param[in] text The label text
     */
    void setText(std::string text) noexcept;

    /*!
     * \brief Get the rotation angle
     * \return The rotation angle in decimal sexagesimal degrees
     * \see angleConversion
     */
    [[nodiscard]]
    constexpr auto angle() const noexcept -> double;

    /*!
     * \brief Set the rotation angle
     * \param[in] angle The rotation angle in decimal sexagesimal degrees
     * \see angleConversion
     */
    constexpr void setAngle(double angle) noexcept;

    /*!
     * \brief Get the foreground color
     * \return The foreground color
     * \see Color
     */
    [[nodiscard]]
    constexpr auto foregroundColor() const noexcept -> Color;

    /*!
     * \brief Set the foreground color
     * \param[in] color The foreground color
     * \see Color
     */
    constexpr void setForegroundColor(Color color) noexcept;

    /*!
     * \brief Get the background color
     * \return The background color
     * \see Color
     */
    [[nodiscard]]
    constexpr auto backgroundColor() const noexcept -> Color;

    /*!
     * \brief Set the background color
     * \param[in] color The background color
     * \see Color
     */
    constexpr void setBackgroundColor(Color color) noexcept;

    /*!
     * \brief Get the outline color
     * \return The outline color
     * \see Color
     */
    [[nodiscard]]
    constexpr auto outlineColor() const noexcept -> Color;

    /*!
     * \brief Set the outline color
     * \param[in] color The outline color
     * \see Color
     */
    constexpr void setOutlineColor(Color color) noexcept;

    /*!
     * \brief Get the shadow color
     * \return The shadow color
     * \see Color
     */
    [[nodiscard]]
    constexpr auto shadowColor() const noexcept -> Color;

    /*!
     * \brief Set the shadow color
     * \param[in] color The shadow color
     * \see Color
     */
    constexpr void setShadowColor(Color color) noexcept;

    /*!
     * \brief Gets the horizontal stretch factor of the text.
     * \return Stretch factor in percentage (e.g. 100 = no stretch, 200 = 2× wider).
     */
    [[nodiscard]]
    constexpr auto stretchFactor() const noexcept -> double;

    /*!
     * \brief Set the stretch factor
     * \param[in] stretch The stretch factor
     */
    constexpr void setStretchFactor(double stretch) noexcept;

    /*!
     * \brief Get the label placement mode
     * \return The label placement mode
     * \see Placement
     */
    [[nodiscard]]
    constexpr auto placement() const noexcept -> Placement;

    /*!
     * \brief Set the label placement mode
     * \param[in] placement The label placement mode
     * \see Placement
     */
    constexpr void setPlacement(Placement placement) noexcept;

    /*!
     * \brief Get the label anchor position
     * \return The label anchor position
     * \see LabelAnchor
     */
    [[nodiscard]]
    constexpr auto anchorPosition() const noexcept -> LabelAnchor;

    /*!
     * \brief Set the label anchor position
     * \param[in] anchorPosition The label anchor position
     * \see LabelAnchor
     */
    constexpr void setAnchorPosition(LabelAnchor anchor) noexcept;

    [[nodiscard]]
    constexpr auto offset() const noexcept -> Vector2d;

    /*!
     * \brief Set the offset of the label insertion point
     * \param[in] dx The X offset of the label insertion point
     * \param[in] dy The Y offset of the label insertion point
     */
    constexpr void setOffset(double dx, double dy) noexcept;


    /*!
     * \brief Sets the font of the label text.
     * \param[in] font Font to be used.
     */
    void setFont(Font font) noexcept;

    /*!
     * \brief Gets the font used to render the label text.
     * \return Font definition.
     */
    [[nodiscard]]
    constexpr auto font() const noexcept -> Font;

    /*!
     * \brief Gets the perpendicular offset from the geometry.
     *
     * Useful to move the label away from the feature when drawing along a line.
     * \return Perpendicular offset in pixels or drawing units.
     */
    [[nodiscard]]
    constexpr auto perpendicularOffset() const noexcept -> int;

    /*!
     * \brief Sets the perpendicular offset from the geometry.
     * \param[in] offset Offset value.
     */
    constexpr void setPerpendicularOffset(int perpendicularOffset) noexcept;

};



inline auto Label::text() const noexcept -> std::string
{
    return mText;
}

inline void Label::setText(std::string text) noexcept
{
    mText = std::move(text);
}

constexpr auto Label::angle() const noexcept -> double
{
    return mAngle;
}

constexpr void Label::setAngle(double angle) noexcept
{
    mAngle = angle;
}

constexpr auto Label::foregroundColor() const noexcept -> Color
{
    return mForegroundColor;
}

constexpr void Label::setForegroundColor(Color color) noexcept
{
    mForegroundColor = std::move(color);
}

constexpr auto Label::backgroundColor() const noexcept -> Color
{
    return mBackgroundColor;
}

constexpr void Label::setBackgroundColor(Color color) noexcept
{
    mBackgroundColor = std::move(color);
}

constexpr auto Label::outlineColor() const noexcept -> Color
{
    return mOutlineColor;
}

constexpr void Label::setOutlineColor(Color color) noexcept
{
    mOutlineColor = std::move(color);
}

constexpr auto Label::shadowColor() const noexcept -> Color
{
    return mShadowColor;
}

constexpr void Label::setShadowColor(Color color) noexcept
{
    mShadowColor = std::move(color);
}

constexpr auto Label::stretchFactor() const noexcept -> double
{
    return mStretch;
}

constexpr void Label::setStretchFactor(double stretch) noexcept
{
    mStretch = stretch;
}

constexpr auto Label::placement() const noexcept -> Label::Placement
{
    return mPlacement;
}

constexpr void Label::setPlacement(Placement placement) noexcept
{
    mPlacement = placement;
}

constexpr auto Label::anchorPosition() const noexcept -> LabelAnchor
{
    return mAnchor;
}

constexpr void Label::setAnchorPosition(LabelAnchor anchor) noexcept
{
    mAnchor = anchor;
}

constexpr auto Label::offset() const noexcept -> Vector2d
{
    return mOffset;
}

constexpr void Label::setOffset(double dx, double dy) noexcept
{
    mOffset[0] = dx;
    mOffset[1] = dy;
}

inline void Label::setFont(Font font) noexcept
{
    mFont = std::move(font);
}

constexpr auto Label::font() const noexcept -> Font
{
    return mFont;
}

constexpr auto Label::perpendicularOffset() const noexcept -> int
{
    return mPerpendicularOffset;
}

constexpr void Label::setPerpendicularOffset(int perpendicularOffset) noexcept
{
    mPerpendicularOffset = perpendicularOffset;
}

/*! \} */ 

} // End namespace tl
