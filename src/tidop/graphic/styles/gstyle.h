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

#include <memory>

#include "tidop/core/base/defs.h"
#include "tidop/graphic/styles/label.h"
#include "tidop/graphic/styles/pen.h"
#include "tidop/graphic/styles/symbol.h"
#include "tidop/graphic/styles/brush.h"

namespace tl
{


/*! \addtogroup Styles
 *  \{
 */


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


/*! \} */ 

} // End namespace tl
