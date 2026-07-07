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

/*! \file LabelAnchor.h
 * \brief Anchor position for label placement.
 *
 * This file defines the `LabelAnchor` class, which specifies the anchor point
 * of a label relative to its geometric feature. It combines horizontal and
 * vertical alignment options.
 *
 * \ingroup Graphics
 * \see tl::Label
 */

#pragma once

#include <array>

#include "tidop/core/base/Defs.h"
#include "tidop/graphic/styles/Font.h"
#include "tidop/graphic/Color.h"

namespace tl
{


/*! \addtogroup Styles
 *  \{
 */


/*!
 * \class LabelAnchor
 * \brief Anchor position for label placement.
 *
 * The `LabelAnchor` class defines where a label is anchored relative to its
 * associated geometry point. It combines a horizontal alignment (left, center,
 * right) and a vertical alignment (baseline, center, top, bottom).
 *
 * ### Example
 * \code
 * LabelAnchor anchor(LabelAnchor::Horizontal::center, LabelAnchor::Vertical::top);
 * label.setAnchorPosition(anchor);
 * \endcode
 *
 * \see Label
 */
class TL_EXPORT LabelAnchor
{

public:

    /*!
     * \enum Horizontal
     * \brief Horizontal alignment of the label.
     */
    enum class Horizontal : uint8_t
    {
        left,   /*!< Anchor at the left edge of the label. */
        center, /*!< Anchor at the horizontal center of the label. */
        right   /*!< Anchor at the right edge of the label. */
    };

    /*!
     * \enum Vertical
     * \brief Vertical alignment of the label.
     */
    enum class Vertical : uint8_t
    {
        baseline, /*!< Anchor at the text baseline. */
        center,   /*!< Anchor at the vertical center of the label. */
        top,      /*!< Anchor at the top edge of the label. */
        bottom    /*!< Anchor at the bottom edge of the label. */
    };

private:

    Horizontal mHorizontal = Horizontal::left;
    Vertical mVertical = Vertical::baseline;

public:

    /*!
     * \brief Default constructor.
     * Creates an anchor with horizontal left and vertical baseline.
     */
    constexpr LabelAnchor() = default;

    /*!
     * \brief Constructs an anchor with explicit horizontal and vertical alignments.
     * \param[in] h Horizontal alignment.
     * \param[in] v Vertical alignment.
     */
    constexpr LabelAnchor(Horizontal h, Vertical v) noexcept
        : mHorizontal(h),
        mVertical(v)
    {
    }

    /*!
     * \brief Returns the horizontal alignment.
     * \return The horizontal alignment.
     */
    [[nodiscard]]
    constexpr auto horizontal() const noexcept -> Horizontal { return mHorizontal; }

    /*!
     * \brief Sets the horizontal alignment.
     * \param[in] h The new horizontal alignment.
     */
    constexpr void setHorizontal(Horizontal h) noexcept { mHorizontal = h; }

    /*!
     * \brief Returns the vertical alignment.
     * \return The vertical alignment.
     */
    [[nodiscard]]
    constexpr auto vertical() const noexcept -> Vertical { return mVertical; }

    /*!
     * \brief Sets the vertical alignment.
     * \param[in] v The new vertical alignment.
     */
    constexpr void setVertical(Vertical v) noexcept { mVertical = v; }

    auto operator<=>(const LabelAnchor &) const = default;
};


/*! \} */ 

} // namespace tl
