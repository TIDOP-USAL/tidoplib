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

/*! \file Font.h
 * \brief Font representation for text rendering.
 *
 * This file defines the `Font` class, which encapsulates font properties
 * such as family name, size, style (bold, italic), underline, and strikethrough.
 * It is used by the `Label` class to control text rendering.
 *
 * \ingroup Graphics
 * \see tl::Label, tl::Color
 */

#pragma once

#include <string>

#include "tidop/core/base/Defs.h"
#include "tidop/core/base/Flags.h"

namespace tl
{

/*! \addtogroup Graphic
 *  \{
 */

/*!
 * \class Font
 * \brief Font style and properties for text rendering.
 *
 * The `Font` class represents a typeface with specific attributes including
 * family name, size in points, and style flags (bold, italic, underline,
 * strikethrough). It is used as part of the `Label` class to define how text
 * is rendered on a canvas.
 *
 * ### Example
 * \code
 * Font font("Times New Roman", 14, Font::Style::bold | Font::Style::italic);
 * font.setUnderline(true);
 * Label label("Hello", font, Color::Black);
 * \endcode
 *
 * \see Label, Color
 */
class TL_EXPORT Font
{

public:

    /*!
     * \enum Style
     * \brief Font style flags.
     *
     * These flags define the stylistic appearance of the font.
     * Flags can be combined using bitwise operators.
     *
     * \code
     * Font::Style style = Font::Style::bold | Font::Style::italic;
     * \endcode
     */
    enum class Style : uint8_t
    {
        normal = 0 << 0,  /*!< Regular style (default) */
        bold = 1 << 0,    /*!< Bold weight */
        italic = 1 << 1   /*!< Italic slant */
    };

private:

    std::string mName = "Arial";
    int mSize = 10;
    EnumFlags<Style> mStyle;
    bool mUnderline = false;
    bool mStrikethrough = false;

public:

    /*!
     * \brief Default constructor
     */
    constexpr Font() = default;

    /*!
     * \brief Copy constructor
     * \param[in] font Font to copy.
     */
    constexpr Font(const Font &font) = default;

    /*!
     * \brief Move constructor
     * \param[in] font Font to move.
     */
    constexpr Font(Font &&font) noexcept = default;

    /*!
     * \brief Constructs a font with custom parameters.
     * \param[in] name Font family name.
     * \param[in] size Font size in points.
     * \param[in] style Font style flags.
     * \param[in] underline Whether the font is underlined.
     * \param[in] strikethrough Whether the font is struck through.
     * \see Style
     */
    explicit constexpr Font(std::string name,
                            int size = 10, 
                            Style style = Style::normal, 
                            bool underline = false, 
                            bool strikethrough = false);

    /*!
     * \brief Copy assignment operator.
     * \param[in] font Font to assign.
     * \return Reference to this object.
     */
    constexpr auto operator =(const Font &font) -> Font & = default;

    /*!
     * \brief Move assignment operator.
     * \param[in] font Font to move.
     * \return Reference to this object.
     */
    constexpr auto operator =(Font &&font) noexcept -> Font & = default;

    /*!
     * \brief Returns the font family name.
     * \return Font name.
     */
    [[nodiscard]]
    constexpr auto name() const noexcept -> std::string;

    /*!
     * \brief Sets the font family name.
     * \param[in] name Font name.
     */
    void setName(const std::string &name) noexcept;

    /*!
     * \brief Returns the font size (in points).
     * \return Font size.
     */
    [[nodiscard]]
    constexpr auto size() const noexcept -> int;

    /*!
     * \brief Sets the font size (in points).
     * \param[in] size Font size.
     */
    constexpr void setSize(int size) noexcept;

    /*!
     * \brief Checks if the font is bold.
     * \return `true` if bold is enabled; `false` otherwise.
     */
    [[nodiscard]]
    constexpr auto isBold() const noexcept -> bool;

    /*!
     * \brief Enables or disables bold style.
     * \param[in] active `true` to enable bold; `false` to disable.
     */
    constexpr void setBold(bool active) noexcept;

    /*!
     * \brief Checks if the font is italic.
     * \return `true` if italic is enabled; `false` otherwise.
     */
    [[nodiscard]] 
    constexpr auto isItalic() const noexcept -> bool;

    /*!
     * \brief Enables or disables italic style.
     * \param[in] active `true` to enable italic; `false` to disable.
     */
    constexpr void setItalic(bool active) noexcept;

    /*!
     * \brief Checks if the font is underlined.
     * \return `true` if underline is enabled; `false` otherwise.
     */
    [[nodiscard]] 
    constexpr auto isUnderline() const noexcept -> bool;

    /*!
     * \brief Enables or disables underline.
     * \param[in] active `true` to enable underline; `false` to disable.
     */
    constexpr void setUnderline(bool active) noexcept;

    /*!
     * \brief Checks if the font has strikethrough.
     * \return `true` if strikethrough is enabled; `false` otherwise.
     */
    [[nodiscard]] 
    constexpr auto isStrikethrough() const noexcept -> bool;

    /*!
     * \brief Enables or disables strikethrough.
     * \param[in] active `true` to enable strikethrough; `false` to disable.
     */
    constexpr void setStrikethrough(bool active) noexcept;

    /*!
     * \brief Sets the font style.
     * \param[in] style Bitwise combination of `Style` flags.
     */
    constexpr void setStyle(Style style) noexcept;


};
ALLOW_BITWISE_FLAG_OPERATIONS(Font::Style)


constexpr Font::Font(std::string name,
                     int size,
                     Style style,
                     bool underline,
                     bool strikethrough)
  : mName(std::move(name)),
    mSize(size),
    mStyle(style),
    mUnderline(underline),
    mStrikethrough(strikethrough)
{
}

constexpr auto Font::name() const noexcept -> std::string
{
    return mName;
}

inline void Font::setName(const std::string &name) noexcept
{
    mName = name;
}

constexpr auto Font::size() const noexcept -> int
{
    return mSize;
}

constexpr void Font::setSize(int size) noexcept
{
    mSize = size;
}

constexpr auto Font::isBold() const noexcept -> bool
{
    return mStyle.isEnabled(Style::bold);
}

constexpr void Font::setBold(bool active) noexcept
{
    mStyle.activeFlag(Style::bold, active);
}

constexpr auto Font::isItalic() const noexcept -> bool
{
    return mStyle.isEnabled(Style::italic);
}

constexpr void Font::setItalic(bool active) noexcept
{
    mStyle.activeFlag(Style::italic, active);
}

constexpr auto Font::isUnderline() const noexcept -> bool
{
    return mUnderline;
}

constexpr void Font::setUnderline(bool active) noexcept
{
    mUnderline = active;
}

constexpr auto Font::isStrikethrough() const noexcept -> bool
{
    return mStrikethrough;
}

constexpr void Font::setStrikethrough(bool active) noexcept
{
    mStrikethrough = active;
}

constexpr void Font::setStyle(Style style) noexcept
{
    mStyle = style;
}

/*! \} */

} // namespace tl

