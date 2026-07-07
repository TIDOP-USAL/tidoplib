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

/*! \file Label.h
 * \brief Text label style and placement for graphic entities.
 *
 * This file defines the `Label` class, which encapsulates the text content,
 * font, colors, and placement options for rendering labels on geometric features.
 * It is used by `GraphicStyle` to annotate points, polylines, and polygons.
 *
 * \ingroup Graphics
 * \see tl::Font, tl::Color, tl::LabelAnchor, tl::GraphicStyle
 */

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
 * The `Label` class defines how text is rendered and positioned relative to a
 * geometric feature. It supports font styling, color customization (foreground,
 * background, outline, shadow), alignment, rotation, offsetting, and various
 * placement strategies for polylines or points.
 *
 * Labels are typically used to annotate map features, drawings, or geometries
 * in visualization systems.
 *
 * ### Example
 * \code
 * Font font("Arial", 12, Font::Style::bold);
 * Label label(font, Color::Black);
 * label.setText("Hello World");
 * label.setPlacement(Label::Placement::middle);
 * label.setOffset(5.0, 10.0);
 * \endcode
 *
 * \see Font, Color, LabelAnchor, GraphicStyle
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
        point,                /*!< Label attached to a point or to the first vertex of a polyline. */
        last_vertex,          /*!< Label attached to the last vertex of a polyline. */
        stretched,            /*!< Text stretched along a polyline with equal spacing between characters. */
        middle,               /*!< Single label placed at the middle of a polyline (based on total length). */
        word_per_segment,     /*!< One word per line segment in a polyline. */
        horizontal,           /*!< Each word placed horizontally in its segment, anchored at the segment center. */
        stretched_to_segment  /*!< Each word stretched to fit its segment and placed along it, anchored at segment start. */
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

    /*!
     * \brief Default constructor.
     * Creates an empty label with default settings.
     */
    Label() = default;

    /*!
     * \brief Constructs a label with a font and foreground color.
     * \param[in] font            Font to use.
     * \param[in] foregroundColor Text color (default: black).
     */
    Label(Font font, Color foregroundColor = {0, 0, 0})
        : mFont(std::move(font)),
        mForegroundColor(foregroundColor)
    {
    }

    /*!
     * \brief Copy constructor.
     * \param[in] label Label to copy.
     */
    Label(const Label &label) = default;

    /*!
     * \brief Move constructor.
     * \param[in] label Label to move.
     */
    Label(Label &&label) noexcept = default;

    /*! \brief Destructor. */
    ~Label() = default;

    /*!
     * \brief Copy assignment operator.
     * \param[in] label Label to copy.
     * \return Reference to this object.
     */
    auto operator =(const Label &label) -> Label & = default;

    /*!
     * \brief Move assignment operator.
     * \param[in] label Label to move.
     * \return Reference to this object.
     */
    auto operator =(Label &&label) noexcept -> Label & = default;

    /*!
     * \brief Returns the label text.
     * \return The text string.
     */
    [[nodiscard]]
    auto text() const noexcept -> std::string;

    /*!
     * \brief Sets the label text.
     * \param[in] text The text string.
     */
    void setText(std::string text) noexcept;

    /*!
     * \brief Returns the font used for rendering.
     * \return The font.
     */
    [[nodiscard]]
    constexpr auto font() const noexcept -> Font;

    /*!
     * \brief Sets the font.
     * \param[in] font The new font.
     */
    void setFont(Font font) noexcept;

    /*!
     * \brief Returns the foreground (text) color.
     * \return The foreground color.
     */
    [[nodiscard]]
    constexpr auto foregroundColor() const noexcept -> Color;

    /*!
     * \brief Sets the foreground color.
     * \param[in] color The new foreground color.
     */
    constexpr void setForegroundColor(Color color) noexcept;

    /*!
     * \brief Returns the background color.
     * \return The background color.
     */
    [[nodiscard]]
    constexpr auto backgroundColor() const noexcept -> Color;

    /*!
     * \brief Sets the background color.
     * \param[in] color The new background color.
     */
    constexpr void setBackgroundColor(Color color) noexcept;

    /*!
     * \brief Returns the outline color.
     * \return The outline color.
     */
    [[nodiscard]]
    constexpr auto outlineColor() const noexcept -> Color;

    /*!
     * \brief Sets the outline color.
     * \param[in] color The new outline color.
     */
    constexpr void setOutlineColor(Color color) noexcept;

    /*!
     * \brief Returns the shadow color.
     * \return The shadow color.
     */
    [[nodiscard]]
    constexpr auto shadowColor() const noexcept -> Color;

    /*!
     * \brief Sets the shadow color.
     * \param[in] color The new shadow color.
     */
    constexpr void setShadowColor(Color color) noexcept;

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
     * \brief Returns the horizontal stretch factor.
     * \return Stretch factor in percentage (100 = no stretch).
     */
    [[nodiscard]]
    constexpr auto stretchFactor() const noexcept -> double;

    /*!
     * \brief Sets the horizontal stretch factor.
     * \param[in] stretch Stretch factor in percentage (100 = no stretch).
     */
    constexpr void setStretchFactor(double stretch) noexcept;

    /*!
     * \brief Returns the placement mode.
     * \return The placement mode.
     */
    [[nodiscard]]
    constexpr auto placement() const noexcept -> Placement;

    /*!
     * \brief Sets the placement mode.
     * \param[in] placement The new placement mode.
     */
    constexpr void setPlacement(Placement placement) noexcept;

    /*!
     * \brief Returns the anchor position.
     * \return The anchor position.
     */
    [[nodiscard]]
    constexpr auto anchorPosition() const noexcept -> LabelAnchor;

    /*!
     * \brief Sets the anchor position.
     * \param[in] anchor The new anchor position.
     */
    constexpr void setAnchorPosition(LabelAnchor anchor) noexcept;

    /*!
     * \brief Returns the offset from the anchor point.
     * \return The offset vector.
     */
    [[nodiscard]]
    constexpr auto offset() const noexcept -> Vector2d;

    /*!
     * \brief Sets the offset from the anchor point.
     * \param[in] dx X-offset.
     * \param[in] dy Y-offset.
     */
    constexpr void setOffset(double dx, double dy) noexcept;

    /*!
     * \brief Returns the perpendicular offset from the geometry.
     * \return The perpendicular offset in drawing units.
     */
    [[nodiscard]]
    constexpr auto perpendicularOffset() const noexcept -> int;

    /*!
     * \brief Sets the perpendicular offset from the geometry.
     * \param[in] offset The perpendicular offset.
     */
    constexpr void setPerpendicularOffset(int offset) noexcept;

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

} // namespace tl
