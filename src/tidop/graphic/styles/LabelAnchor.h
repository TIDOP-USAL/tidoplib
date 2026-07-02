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

#include <array>

#include "tidop/core/base/Defs.h"
#include "tidop/graphic/styles/Font.h"
#include "tidop/graphic/Color.h"

namespace tl
{


/*! \addtogroup Styles
 *  \{
 */



class TL_EXPORT LabelAnchor
{

public:

    enum class Horizontal : uint8_t
    {
        left,
        center,
        right
    };

    enum class Vertical : uint8_t
    {
        baseline,
        center,
        top,
        bottom
    };

private:

    Horizontal mHorizontal = Horizontal::left;
    Vertical mVertical = Vertical::baseline;

public:

    constexpr LabelAnchor() = default;
    
    constexpr LabelAnchor(Horizontal h, Vertical v) noexcept
      : mHorizontal(h),
        mVertical(v)
    {}

    [[nodiscard]] 
    constexpr auto horizontal() const noexcept -> Horizontal { return mHorizontal; }
    constexpr void setHorizontal(Horizontal h) noexcept { mHorizontal = h; }

    [[nodiscard]] 
    constexpr auto vertical() const noexcept -> Vertical { return mVertical; }
    constexpr void setVertical(Vertical v) noexcept { mVertical = v; }

    auto operator<=>(const LabelAnchor &) const = default;
};


/*! \} */ 

} // namespace tl
