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

/*! \file GraphicStyle.h
 * \brief Aggregated visual style for graphic entities.
 *
 * This file defines the `GraphicStyle` class, which groups together the four
 * visual components used for rendering: `Pen` (stroke), `Brush` (fill),
 * `Symbol` (point marker), and `Label` (text). Each component is optional,
 * so a style can be partially defined.
 *
 * \ingroup Graphics
 * \see tl::Pen, tl::Brush, tl::Symbol, tl::Label
 */

#pragma once

#include <memory>
#include <optional>

#include "tidop/core/base/Defs.h"
#include "tidop/graphic/styles/Label.h"
#include "tidop/graphic/styles/Pen.h"
#include "tidop/graphic/styles/Symbol.h"
#include "tidop/graphic/styles/Brush.h"

namespace tl
{


/*! \addtogroup Styles
 *  \{
 */


/*!
 * \class GraphicStyle
 * \brief Aggregates graphical style components for rendering.
 *
 * `GraphicStyle` defines the complete visual styling for an entity, layer,
 * or dataset. It groups four visual components:
 * - `Pen`: controls stroke (line) appearance.
 * - `Brush`: controls area (fill) appearance.
 * - `Symbol`: defines point marker style.
 * - `Label`: defines text rendering style.
 *
 * A style can be partially defined (e.g., only a label), and missing
 * components are simply ignored during rendering.
 *
 * ### Example
 * \code
 * GraphicStyle style;
 * style.setPen(Pen(Color::Blue, 2.0));
 * style.setBrush(Brush(Color::Red, Color::White, Brush::Style::solid));
 * style.setSymbol(Symbol(Symbol::Type::Circle, 8.0));
 * style.setLabel(Label("Name", Font("Arial", 12), Color::Black));
 * \endcode
 *
 * \see Pen, Brush, Symbol, Label
 */
class TL_EXPORT GraphicStyle
{

private:

    std::optional<Pen> mPen;
    std::optional<Brush> mBrush;
    std::optional<Symbol> mSymbol;
    std::optional<Label> mLabel;

public:

    /*!
     * \brief Default constructor.
     * Creates an empty style with no components set.
     */
    GraphicStyle() = default;

    /*!
     * \brief Copy constructor.
     * \param[in] graphicStyle The style to copy.
     */
    GraphicStyle(const GraphicStyle &graphicStyle) = default;

    /*!
     * \brief Move constructor.
     * \param[in] graphicStyle The style to move.
     */
    GraphicStyle(GraphicStyle &&graphicStyle) noexcept = default;

    /*!
     * \brief Destructor
     */
    ~GraphicStyle() = default;

    /*!
     * \brief Copy assignment operator.
     * \param[in] graphicStyle The style to copy.
     * \return Reference to this object.
     */
    auto operator =(const GraphicStyle &graphicStyle) -> GraphicStyle & = default;

    /*!
     * \brief Move assignment operator.
     * \param[in] graphicStyle The style to move.
     * \return Reference to this object.
     */
    auto operator =(GraphicStyle &&graphicStyle) noexcept -> GraphicStyle & = default;

    /*!
     * \brief Returns the current pen style.
     * \return Pointer to the `Pen` object, or `nullptr` if not set.
     */
    [[nodiscard]]
    auto pen() const -> const Pen *;

    /*!
     * \brief Sets the pen style.
     * \param[in] pen The new pen style.
     */
    void setPen(Pen pen);

    /*!
     * \brief Returns the current brush style.
     * \return Pointer to the `Brush` object, or `nullptr` if not set.
     */
    [[nodiscard]]
    auto brush() const -> const Brush *;

    /*!
     * \brief Sets the brush style.
     * \param[in] brush The new brush style.
     */
    void setBrush(Brush brush);

    /*!
     * \brief Returns the current symbol style.
     * \return Pointer to the `Symbol` object, or `nullptr` if not set.
     */
    [[nodiscard]]
    auto symbol() const -> const Symbol*;

    /*!
     * \brief Sets the symbol style.
     * \param[in] symbol Shared pointer to the symbol style.
     */
    void setSymbol(Symbol symbol);

    /*!
     * \brief Returns the current label style.
     * \return Pointer to the `Label` object, or `nullptr` if not set.
     */
    [[nodiscard]]
    auto label() const -> const Label*;

    /*!
     * \brief Sets the label style.
     * \param[in] label The new label style.
     */
    void setLabel(Label label);
};


/*! \} */ 

} // namespace tl
