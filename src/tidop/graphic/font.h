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

#include "tidop/core/base/Defs.h"
#include "tidop/core/base/flags.h"

namespace tl
{

/*! \addtogroup Graphic
 *  \{
 */

/*!
 * \class Font
 * \brief Represents a text font with styling options.
 *
 * The `Font` class defines the typographic attributes used when rendering text.
 * This includes the font family name, size, style flags (bold, italic), and decorations
 * such as underline or strikethrough. Font styles are stored using bitwise combinable flags.
 *
 * \see Font::Style, Label, GraphicStyle
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
        bold = 1 << 0,  /*!< Bold weight */
        italic = 1 << 1   /*!< Italic slant */
    };

private:

    std::string mName;
    int mSize;
    EnumFlags<Style> mStyle;
    bool mUnderline;
    bool mStrikethrough;

public:

    /*!
     * \brief Default constructor
     */
    Font();

    /*!
     * \brief Copy constructor
     * \param[in] font Font to copy.
     */
    Font(const Font &font);

    /*!
     * \brief Move constructor
     * \param[in] font Font to move.
     */
    Font(Font &&font) TL_NOEXCEPT;

    /*!
     * \brief Constructs a font with custom parameters.
     * \param[in] name Font family name.
     * \param[in] size Font size in points.
     * \param[in] style Font style flags.
     * \param[in] underline Whether the font is underlined.
     * \param[in] strikethrough Whether the font is struck through.
     * \see Style
     */
    explicit Font(std::string name, 
                  int size = 10, 
                  Style style = Style::normal, 
                  bool underline = false, 
                  bool strikethrough = false);

    /*!
     * \brief Destructor.
     */
    ~Font() = default;

    /*!
     * \brief Returns the font family name.
     * \return Font name.
     */
    auto name() const -> std::string;

    /*!
     * \brief Sets the font family name.
     * \param[in] name Font name.
     */
    void setName(const std::string &name);

    /*!
     * \brief Returns the font size (in points).
     * \return Font size.
     */
    auto size() const -> int;

    /*!
     * \brief Sets the font size (in points).
     * \param[in] size Font size.
     */
    void setSize(int size);

    /*!
     * \brief Checks if the font is bold.
     * \return True if bold.
     */
    auto isBold() const -> bool;

    /*!
     * \brief Enables or disables bold style.
     * \param[in] active True to enable bold.
     */
    void setBold(bool active);

    /*!
     * \brief Checks if the font is italic.
     * \return True if italic.
     */
    auto isItalic() const -> bool;

    /*!
     * \brief Enables or disables italic style.
     * \param[in] active True to enable italic.
     */
    void setItalic(bool active);

    /*!
     * \brief Checks if the font is underlined.
     * \return True if underlined.
     */
    auto isUnderline() const -> bool;

    /*!
     * \brief Enables or disables underline.
     * \param[in] active True to enable underline.
     */
    void setUnderline(bool active);

    /*!
     * \brief Checks if the font has strikethrough.
     * \return True if strikethrough is enabled.
     */
    auto isStrikethrough() const -> bool;

    /*!
     * \brief Enables or disables strikethrough.
     * \param[in] active True to enable strikethrough.
     */
    void setStrikethrough(bool active);

    /*!
     * \brief Sets the font style.
     * \param[in] style Bitwise combination of Style flags.
     */
    void setStyle(Style style);

    /*!
     * \brief Copy assignment operator.
     * \param[in] font Font to assign.
     * \return Reference to this object.
     */
    auto operator =(const Font& font) -> Font&;

    /*!
     * \brief Move assignment operator.
     * \param[in] font Font to move.
     * \return Reference to this object.
     */
    auto operator =(Font&& font) TL_NOEXCEPT -> Font&;


};
ALLOW_BITWISE_FLAG_OPERATIONS(Font::Style)

/*! \} */ // end of GraphicEntities

} // End namespace tl

