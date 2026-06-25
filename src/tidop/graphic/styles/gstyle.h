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

#include "tidop/core/base/Defs.h"
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
 * \class GraphicStyle
 * \brief Aggregates graphical style components for rendering.
 *
 * `GraphicStyle` defines the complete visual styling for an entity, layer, or dataset.
 * It groups four visual components:
 * - `Pen`: controls stroke (line) appearance.
 * - `Brush`: controls area (fill) appearance.
 * - `Symbol`: defines point marker style.
 * - `Label`: defines text rendering style.
 *
 * A style can be partially defined (e.g., only a label), and missing components are simply ignored during rendering.
 *
 * \see Pen, Brush, Symbol, Label
 */
class TL_EXPORT GraphicStyle
{

private:

    std::shared_ptr<Pen> mPen;
    std::shared_ptr<Brush> mBrush;
    std::shared_ptr<Symbol> mSymbol;
    std::shared_ptr<Label> mLabel;

public:

    /*!
     * \brief Default constructor
     */
    GraphicStyle();

    /*!
     * \brief Copy constructor.
     * \param[in] graphicStyle The style to copy.
     */
    GraphicStyle(const GraphicStyle &graphicStyle);

    /*!
     * \brief Move constructor.
     * \param[in] graphicStyle The style to move.
     */
    GraphicStyle(GraphicStyle&& graphicStyle) noexcept;

    /*!
     * \brief Destructor
     */
    virtual ~GraphicStyle() = default;

    /*!
     * \brief Returns the current pen style.
     * \return Shared pointer to the `Pen` object (may be null).
     */
    auto pen() const -> std::shared_ptr<Pen>;

    /*!
     * \brief Sets the pen style.
     * \param[in] pen Shared pointer to the pen style.
     */
    void setPen(const std::shared_ptr<Pen> &pen);

    /*!
     * \brief Returns the current brush style.
     * \return Shared pointer to the `Brush` object (may be null).
     */
    auto brush() const -> std::shared_ptr<Brush>;

    /*!
     * \brief Sets the brush style.
     * \param[in] brush Shared pointer to the brush style.
     */
    void setBrush(const std::shared_ptr<Brush> &brush);

    /*!
     * \brief Returns the current symbol style.
     * \return Shared pointer to the `Symbol` object (may be null).
     */
    auto symbol() const -> std::shared_ptr<Symbol>;

    /*!
     * \brief Sets the symbol style.
     * \param[in] symbol Shared pointer to the symbol style.
     */
    void setSymbol(const std::shared_ptr<Symbol> &symbol);

    /*!
     * \brief Returns the current label style.
     * \return Shared pointer to the `Label` object (may be null).
     */
    auto label() const -> std::shared_ptr<Label>;

    /*!
     * \brief Sets the label style.
     * \param[in] label Shared pointer to the label style.
     */
    void setLabel(const std::shared_ptr<Label> &label);

    /*!
     * \brief Copy assignment operator.
     * \param[in] graphicStyle The style to copy.
     * \return Reference to this object.
     */
    auto operator =(const GraphicStyle &graphicStyle) -> GraphicStyle&;

    /*!
     * \brief Move assignment operator.
     * \param[in] graphicStyle The style to move.
     * \return Reference to this object.
     */
    auto operator =(GraphicStyle &&graphicStyle) TL_NOEXCEPT -> GraphicStyle &;

};


/*! \} */ 

} // End namespace tl
