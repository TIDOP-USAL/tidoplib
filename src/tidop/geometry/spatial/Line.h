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
 * License along with Foobar. If not, see <http://www.gnu.org/licenses/>. *
 *                                                                        *
 * @license LGPL-3.0 <https://www.gnu.org/licenses/lgpl-3.0.html>         *
 *                                                                        *
 **************************************************************************/

#pragma once

#include "tidop/config.h"
#include "tidop/core/base/Concepts.h"
#include "tidop/math/algebra/vector/Vector.h"

namespace tl
{

/*! \addtogroup Geometry
 *  \{
 */
 
template <Floating T, size_t Dim = 3>
class Line
{
	
    static_assert(Dim == 2 || Dim == 3, "Only 2 or 3 dimensions allowed");


private:

    using PointType = Point<T, typename tag_for_dim<Dim>::type>;
    PointType origin;
    Vector<T, Dim> direction;

public:

    constexpr Line(const PointType &_origin, const Vector<T, Dim>& _direction)
      : origin(_origin), 
        direction(_direction) 
    {
    }

    constexpr Line() = default;
    constexpr Line(const Line &line) = default;
    constexpr Line(Line &&line) noexcept = default;
    ~Line() = default;

    constexpr auto operator=(const Line &line) -> Line & = default;
    constexpr auto operator=(Line &&line) noexcept -> Line & = default;

    [[nodiscard]]
    constexpr auto evaluate(T lambda) const -> Vector<T, Dim>
    {
        return origin + direction * lambda;
    }

    [[nodiscard]]
    constexpr auto distance(const PointType &point) const -> T
    {
        Vector<T, Dim> diff = point - origin;
        T directionModule = direction.module();

        if constexpr (Dim == 2) {
            T cross = diff.x() * direction.y() - diff.y() * direction.x();
            return tl::abs(cross) / directionModule;
        } else {
            T numModule = diff.cross(direction).module();
            return numModule / directionModule;
        }
    }

    [[nodiscard]]
    constexpr auto getOrigin() const -> PointType { return origin; }
    [[nodiscard]]
    constexpr auto getDirection() const -> Vector<T, Dim> { return direction; }

};

}