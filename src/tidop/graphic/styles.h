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

#include <string>
#include <array>
#include <memory>

#include "tidop/core/defs.h"
#include "tidop/core/flags.h"
#include "tidop/graphic/color.h"
#include "tidop/graphic/font.h"

namespace tl
{


/*! \defgroup GraphicEntities Entidades gráficas
 *  Puntos, lineas, ...
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


/*!
 * \brief Brush style class
 */
class TL_EXPORT Brush
{
public:

    enum class Name : uint8_t
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

private:

    /*!
     * \brief Foreground color
     * \see Color
     */
    Color mForeColor;

    /*!
     * \brief Background color
     * \see Color
     */
    Color mBackColor;

    /*!
     * \brief Brush name
     */
    Name mName;

    /*!
     * \brief Rotation angle in decimal sexagesimal degrees
     * \see angleConversion
     */
    double mAngle;

    /*!
     * \brief Scaling factor
     */
    double mScalingFactor;

    /*!
     * \brief Spacing between symbols
     */
    std::array<double, 2> mSpacing;

    /*!
     * \brief mPriorityLevel
     */
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
     * \brief Returns the foreground color
     * \return Foreground color
     * \see Color
     */
    auto foregroundColor() const -> Color;

    /*!
     * \brief Sets the foreground color
     * \param[in] foregroundColor Foreground color
     * \see Color
     */
    void setForegroundColor(const Color &foregroundColor);

    /*!
     * \brief Returns the background color
     * \return Background color
     * \see Color
     */
    auto backgroundColor() const -> Color;

    /*!
     * \brief Sets the background color
     * \param[in] backgroundColor Background color
     * \see Color
     */
    void setBackgroundColor(const Color& backgroundColor);

    /*!
     * \brief Returns the name or ID of the brush
     * \return Name or ID of the brush
     */
    auto name() const -> Name;

    /*!
     * \brief Sets the name or ID of the brush
     * \param[in] name Name or ID of the brush
     */
    void setName(Name name);

    /*!
     * \brief Returns the rotation angle
     * \return Rotation angle in decimal sexagesimal degrees
     * \see angleConversion
     */
    auto angle() const -> double;

    /*!
     * \brief Sets the rotation angle
     * \param[in] angle Rotation angle in decimal sexagesimal degrees
     * \see angleConversion
     */
    void setAngle(double angle);

    /*!
     * \brief Returns the scaling factor
     * \return Scaling factor
     */
    auto scalingFactor() const -> double;

    /*!
     * \brief Sets the scaling factor
     * \param[in] scalingFactor Scaling factor
     */
    void setScalingFactor(double scalingFactor);

    /*!
     * \brief Returns the spacing in the X direction
     * \return Spacing in X
     */
    auto spacingX() const -> double;

    /*!
     * \brief Returns the spacing in the Y direction
     * \return Spacing in Y
     */
    auto spacingY() const -> double;

    /*!
     * \brief Sets the spacing in the X and Y directions
     * \param[in] spacingX Spacing in X
     * \param[in] spacingY Spacing in Y
     */
    void setSpacing(double spacingX, double spacingY);

    /*!
     * \brief Returns the priority level
     * \return Priority level
     */
    auto priorityLevel() const -> uint32_t;

    /*!
     * \brief Sets the priority level
     * \param priorityLevel Priority level
     */
    void setPriorityLevel(uint32_t priorityLevel);

    /*!
     * \brief Assignment operator
     * \param brush Brush style
     * \return Reference to the brush style
     */
    auto operator =(const Brush& brush) -> Brush&;

    /*!
     * \brief Assignment move operator
     * \param brush Brush style
     * \return Reference to the brush style
     */
    auto operator =(Brush&& brush) TL_NOEXCEPT -> Brush&;

};


/*!
 * \brief Symbol style class
 */
class TL_EXPORT Symbol
{

public:

    enum class Name
    {
        cross,             /*!< + */
        diagonal_cross,    /*!< x */
        circle,            /*!< Circle */
        circle_filled,     /*!< Filled circle */
        square,            /*!< Square */
        square_filled,     /*!< Filled square */
        triangle,          /*!< Triangle */
        triangle_filled,   /*!< Filled triangle */
        star,              /*!< Star */
        star_filled,       /*!< Filled star */
        vertical_bar       /*!< | */
    };

private:

    /*!
     * \brief Name or identifier of the symbol
     */
    Name mName;

    /*!
     * \brief Rotation angle in decimal sexagesimal degrees
     * \see angleConversion
     */
    double mAngle;

    /*!
     * \brief Color
     * \see Color
     */
    Color mColor;

    /*!
     * \brief Outline color
     * \see Color
     */
    Color mOutlineColor;

    /*!
     * \brief Scaling factor
     */
    double mScalingFactor;

    /*!
     * \brief X and Y offset of the symbol insertion point.
     */
    std::array<double, 2> mOffset;

    /*!
     * \brief mPriorityLevel
     */
    uint32_t mPriorityLevel;

public:

    /*!
     * \brief Default constructor
     */
    Symbol();

    /*!
     * \brief Copy constructor
     * \param[in] symbol Symbol style object to copy
     */
    Symbol(const Symbol &symbol);

    /*!
     * \brief Move constructor
     * \param[in] symbol Symbol style object to move
     */
    Symbol(Symbol &&symbol) TL_NOEXCEPT;

    /*!
     * \brief Destructor
     */
    ~Symbol();

    /*!
     * \brief Get the rotation angle
     * \return Rotation angle in decimal sexagesimal degrees
     * \see angleConversion
     */
    auto angle() const -> double;

    /*!
     * \brief Set the rotation angle
     * \param[in] angle Rotation angle in decimal sexagesimal degrees
     * \see angleConversion
     */
    void setAngle(double angle);

    /*!
     * \brief Get the name or id of the symbol
     * \return Name or id of the symbol
     */
    auto name() const -> Name;

    /*!
     * \brief Set the name or id of the symbol
     * \param[in] name Name or id of the symbol
     */
    void setName(Name name);

    /*!
     * \brief Get the color
     * \return Color
     * \see Color
     */
    auto color() const -> Color;

    /*!
     * \brief Set the color
     * \param[in] color Color
     * \see Color
     */
    void setColor(const Color &color);

    /*!
     * \brief Get the outline color
     * \return Outline color
     * \see Color
     */
    auto outlineColor() const -> Color;

    /*!
     * \brief Set the outline color
     * \param[in] outlinecolor Outline color
     * \see Color
     */
    void setOutlineColor(const Color &outlinecolor);

    /*!
     * \brief Get the scaling factor
     * \return Scaling factor
     */
    auto scalingFactor() const -> double;

    /*!
     * \brief Set the scaling factor
     * \param[in] scalingFactor Scaling factor
     */
    void setScalingFactor(double scalingFactor);

    /*!
     * \brief Get the X offset of the symbol insertion point.
     */
    auto offsetX() const -> double;

    /*!
     * \brief Get the Y offset of the symbol insertion point.
     */
    auto offsetY() const -> double;

    /*!
     * \brief Set the offset of the symbol insertion point
     * \param[in] dx X offset of the symbol insertion point.
     * \param[in] dy Y offset of the symbol insertion point.
     */
    void setOffset(double dx, double dy);

    auto priorityLevel() const -> uint32_t;

    void setPriorityLevel(uint32_t priorityLevel);

    /*!
     * \brief Assignment operator
     * \param symbol Symbol style
     * \return Reference to the symbol style
     */
    auto operator =(const Symbol& symbol) -> Symbol&;

    /*!
     * \brief Assignment operator
     * \param symbol Symbol style
     * \return Reference to the symbol style
     */
    auto operator =(Symbol&& symbol) TL_NOEXCEPT -> Symbol&;
};


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
    std::string mText;

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






/*!
 * \brief Class for styles
 *
 * Styles for an entity, a layer, or a file
 */
class TL_EXPORT GraphicStyle
{

private:

    /*!
     * \brief Pen style
     */
    std::shared_ptr<Pen> mPen;

    /*!
     * \brief Brush style
     */
    std::shared_ptr<Brush> mBrush;

    /*!
     * \brief Symbol style
     */
    std::shared_ptr<Symbol> mSymbol;

    /*!
     * \brief Label style
     */
    std::shared_ptr<Label> mLabel;

public:

    /*!
     * \brief Default constructor
     */
    GraphicStyle();

    /*!
     * \brief Copy constructor
     */
    GraphicStyle(const GraphicStyle &graphicStyle);
    GraphicStyle(GraphicStyle&& graphicStyle) noexcept;

    /*!
     * \brief Destructor
     */
    virtual ~GraphicStyle() = default;

    /*!
     * \brief Get the pen style
     * \return Pointer to the pen style
     */
    auto pen() const -> Pen*;

    /*!
     * \brief Set the pen style
     * \param[in] pen Pointer to the pen style
     */
    void setPen(const std::shared_ptr<Pen> &pen);

    /*!
     * \brief Get the brush style
     * \return Pointer to the brush style
     */
    auto brush() const -> Brush*;

    /*!
     * \brief Set the brush style
     * \param[in] brush Pointer to the brush style
     */
    void setBrush(const std::shared_ptr<Brush> &brush);

    /*!
     * \brief Get the symbol style
     * \return Pointer to the symbol style
     */
    auto symbol() const -> Symbol*;

    /*!
     * \brief Set the symbol style
     * \param[in] symbol Pointer to the symbol style
     */
    void setSymbol(const std::shared_ptr<Symbol> &symbol);

    /*!
     * \brief Get the label style
     * \return Pointer to the label style
     */
    auto label() const -> Label*;

    /*!
     * \brief Set the label style
     * \param[in] label Pointer to the label style
     */
    void setLabel(const std::shared_ptr<Label> &label);

    /*!
     * \brief Assignment operator
     * \param[in] graphicStyle The graphic style
     */
    auto operator =(const GraphicStyle& graphicStyle) -> GraphicStyle&;

    /*!
     * \brief Assignment copy operator
     * \param[in] graphicStyle The graphic style
     */
    auto operator =(GraphicStyle &&graphicStyle) TL_NOEXCEPT -> GraphicStyle &;

};


/*! \} */ // Fin GraphicEntities

} // Fin namespace tl
