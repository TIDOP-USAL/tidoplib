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
#include "tidop/math/algebra/vector.h"

namespace tl
{

template <typename T>
class Line
{
private:
    Vector<T, 3> origin;
    Vector<T, 3> direction;

public:
    Line(const Vector<T, 3>& _origin, const Vector<T, 3>& _direction)
        : origin(_origin), direction(_direction) {
    }

    Line() = default;
    ~Line() = default;

public:
    Vector<T, 3> evaluate(T lambda)
    {
        return origin + direction * lambda;
    }

    T distance(const Vector<T, 3>& point)
    {
        T numModule = crossProduct(point - origin, direction).module();
        T directionModule = direction.module();
        return numModule / directionModule;
    }

public:
    Vector<T, 3> getOrigin() const { return origin; }
    Vector<T, 3> getDirection() const { return direction; }

};

}