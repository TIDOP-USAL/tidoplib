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

#include "tidop/core/base/defs.h"
#include "tidop/core/base/flags.h"

namespace tl
{

/*! \addtogroup Graphic
 *  \{
 */

 /*!
  * \brief Font class 
  */
class TL_EXPORT Font
{

public:

    /*!
     * \brief Font style
     */
    enum class Style : uint8_t
    {
        normal = 0 << 0,
        bold = 1 << 0,
        italic = 1 << 1
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
     * \param[in] font Font
     */
    Font(const Font &font);

    /*!
     * \brief Move constructor
     * \param[in] font Font
     */
    Font(Font &&font) TL_NOEXCEPT;

    /*!
     * \brief Constructor
     * \param[in] name Font name
     * \param[in] size Font size
     * \param[in] style Font style
     * \param[in] underline Underline. Default is off
     * \param[in] strikethrough Strikethrough. Default is off
     * \see Style
     */
    explicit Font(std::string name, 
                  int size = 10, 
                  Style style = Style::normal, 
                  bool underline = false, 
                  bool strikethrough = false);

    ~Font() = default;

    /*!
     * \brief Returns the font name
     * \return Font name
     */
    auto name() const -> std::string;

    /*!
     * \brief Sets the font name
     * \param[in] name Font name
     */
    void setName(const std::string &name);

    /*!
     * \brief Returns the font size
     * \return Font size
     */
    auto size() const -> int;

    /*!
     * \brief Sets the font size
     * \param[in] size Font size
     */
    void setSize(int size);

    /*!
     * \brief Bold font
     * \return True if the font is bold
     */
    auto isBold() const -> bool;

    /*!
     * \brief Bold font
     * \param[in] active Activate option
     */
    void setBold(bool active);

    /*!
     * \brief Italic font
     * \return True if the font is italic
     */
    auto isItalic() const -> bool;

    /*!
     * \brief Italic font
     * \param[in] active Activate option
     */
    void setItalic(bool active);

    /*!
     * \brief Underlined font
     * \return True if the font is underlined
     */
    auto isUnderline() const -> bool;

    /*!
     * \brief Underlined font
     * \param[in] active Activate option
     */
    void setUnderline(bool active);

    /*!
     * \brief Strikethrough font
     * \return True if the font is strikethrough
     */
    auto isStrikethrough() const -> bool;

    /*!
     * \brief Strikethrough font
     * \param[in] active Activate option
     */
    void setStrikethrough(bool active);

    /*!
     * \brief Set the style
     * \param[in] style
     */
    void setStyle(Style style);

    /*!
     * \brief Assignment operator
     * \param[in] font Font object being assigned
     * \return
     */
    auto operator =(const Font& font) -> Font&;

    /*!
     * \brief Move assignment operator
     * \param[in] font Font object being moved
     * \return
     */
    auto operator =(Font&& font) TL_NOEXCEPT -> Font&;


};
ALLOW_BITWISE_FLAG_OPERATIONS(Font::Style)

/*! \} */ // end of GraphicEntities

} // End namespace TL

