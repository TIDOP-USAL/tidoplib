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

#include "tidop/core/defs.h"
#include "tidop/core/flags.h"

namespace tl
{

/*! \addtogroup GraphicEntities
 *  \{
 */

namespace graph
{

/*!
 * \brief Clase Fuente
 */
class TL_EXPORT Font
{

public:

    /*!
     * \brief Estilo de fuente
     */
    enum class Style : uint8_t
    {
        normal = 0 << 0,
        bold = 1 << 0,
        italic = 1 << 1
    };

public:

    /*!
     * \brief Constructora por defecto
     */
    Font();

    /*!
     * \brief Constructora de copia
     * \param[in] font Fuente
     */
    Font(const Font &font);

    /*!
     * \brief Constructora de movimiento
     * \param[in] font Fuente
     */
    Font(Font &&font) TL_NOEXCEPT;

    /*!
     * \brief Constructora
     * \param[in] name Nombre de la fuente
     * \param[in] size Tamaño de la fuente
     * \param[in] style Estilo de la fuente
     * \param[in] underline Subrayado. Por defecto desactivado
     * \param[in] strikethrough Tachado. Por defecto desactivado
     * \see Style
     */
    Font(const std::string name, int size = 10, Style style = Style::normal, bool underline = false, bool strikethrough = false);

    /*!
     * \brief Destructora
     */
    ~Font() {}

    /*!
     * \brief Devuelve el nombre de la fuente
     * \return Nombre de la fuente
     */
    std::string name() const;

    /*!
     * \brief Establece el nombre de la fuente
     * \param[in] name Nombre de la fuente
     */
    void setName(const std::string &name);

    /*!
     * \brief Devuelve el tamaño de la fuente
     * \return Tamaño de la fuente
     */
    int size() const;

    /*!
     * \brief Establece el tamaño de la fuente
     * \param[in] size Tamaño de la fuente
     */
    void setSize(int size);

    /*!
     * \brief Fuente en negrita
     * \return Verdadero si la fuente está en negrita
     */
    bool isBold() const;

    /*!
     * \brief Fuente en negrita
     * \param[in] active Activa opcion
     */
    void setBold(bool active);

    /*!
     * \brief Fuente en cursiva
     * \return Verdadero si la fuente está en cursiva
     */
    bool isItalic() const;

    /*!
     * \brief Fuente cursiva
     * \param[in] active Activa opcion
     */
    void setItalic(bool active);

    /*!
     * \brief Fuente con subrayado
     * \return Verdadero si la fuente subrayada
     */
    bool isUnderline() const;

    /*!
     * \brief Fuente con subrayado
     * \param[in] active Activa opcion
     */
    void setUnderline(bool active);

    /*!
     * \brief Fuente tachada
     * \return Verdadero si la fuente está tachada
     */
    bool isStrikethrough() const;

    /*!
     * \brief setStrikethrough
     * \param[in] active Activa opcion
     */
    void setStrikethrough(bool active);

    /*!
     * \brief setStyle
     * \param[in] style
     */
    void setStyle(Style style);

    /*!
     * \brief Operador de asignacion
     * \param[in] font Objeto Font que se asigna
     * \return
     */
    Font &operator = (const Font &font);

    /*!
     * \brief Operador de asignacion de movimiento
     * \param[in] font Objeto Font que se mueve
     * \return
     */
    Font &operator = (Font &&font) TL_NOEXCEPT;

private:

    /*!
     * \brief Nombre de fuente
     */
    std::string mName;

    /*!
     * \brief Tamaño de fuente
     */
    int mSize;

    /*!
     * \brief Estilo
     * \see Style
     */
    EnumFlags<Style> mStyle;

    /*!
     * \brief Subrayado de fuente
     */
    bool mUnderline;

    /*!
     * \brief Tachado de fuente
     */
    bool mStrikethrough;

};
ALLOW_BITWISE_FLAG_OPERATIONS(Font::Style)

} // End namespace graph

/*! \} */ // end of GraphicEntities

} // End namespace TL

