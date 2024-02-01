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

#include <vector>
#include <array>

#include "tidop/math/math.h"
#include "tidop/math/algebra/rotations.h"
#include "tidop/math/algebra/vector.h"


namespace tl
{

/*! \addtogroup math
 *  \{
 */


/*! \addtogroup algebra
 *  \{
 */

/*! \defgroup rotations Rotations
 *  \{
 */

enum Axes
{
    //Euler angles
    zxz,
    xyx,
    yzy,
    zyz,
    xzx,
    yxy,
    //TaitBryan angles
    xyz,
    yzx,
    zxy,
    xzy,
    zyx,
    yxz
};

/*!
 * \brief Ángulos de Euler
 */
template<typename T, int _axes = Axes::xyz>
class EulerAngles
    : public OrientationBase<EulerAngles<T, _axes>>
{

public:

    double x;
    double y;
    double z;
    Axes axes;

public:

    EulerAngles();
    EulerAngles(double x, double y, double z);
    EulerAngles(const Vector<T, 3> &angles);
    EulerAngles(const EulerAngles<T, _axes> &eulerAngles);
    EulerAngles(EulerAngles<T, _axes> &&eulerAngles) TL_NOEXCEPT;
    ~EulerAngles() override = default;

    auto operator = (const EulerAngles<T, _axes> &eulerAngles) -> EulerAngles&;
    auto operator = (EulerAngles<T, _axes> &&eulerAngles) TL_NOEXCEPT -> EulerAngles&;

    /* Unary arithmetic operators */

    auto operator+() -> EulerAngles<T, _axes>;
    auto operator-() -> EulerAngles<T, _axes>;
};


template<typename T, int _axes>
EulerAngles<T, _axes>::EulerAngles()
  : OrientationBase<EulerAngles<T, _axes>>(Orientation::Type::euler_angles),
    x{0},
    y{0},
    z{0},
    axes(static_cast<Axes>(_axes))
{
}

template<typename T, int _axes>
EulerAngles<T, _axes>::EulerAngles(double x, double y, double z)
  : OrientationBase<EulerAngles<T, _axes>>(Orientation::Type::euler_angles),
    x(x),
    y(y),
    z(z),
    axes(static_cast<Axes>(_axes))
{
}

template<typename T, int _axes>
inline EulerAngles<T, _axes>::EulerAngles(const Vector<T, 3> &angles)
  : OrientationBase<EulerAngles<T, _axes>>(Orientation::Type::euler_angles),
    x(angles[0]),
    y(angles[1]),
    z(angles[2]),
    axes(static_cast<Axes>(_axes))
{
}

template<typename T, int _axes>
EulerAngles<T, _axes>::EulerAngles(const EulerAngles<T, _axes> &eulerAngles)
  : OrientationBase<EulerAngles<T, _axes>>(Orientation::Type::euler_angles),
    x(eulerAngles.x),
    y(eulerAngles.y),
    z(eulerAngles.z),
    axes(static_cast<Axes>(_axes))
{
}

template<typename T, int _axes>
EulerAngles<T, _axes>::EulerAngles(EulerAngles<T, _axes> &&eulerAngles) TL_NOEXCEPT
  : OrientationBase<EulerAngles<T, _axes>>(Orientation::Type::euler_angles),
    x(std::exchange(eulerAngles.x, 0)),
    y(std::exchange(eulerAngles.y, 0)),
    z(std::exchange(eulerAngles.z, 0)),
    axes(static_cast<Axes>(_axes))
{
}

template<typename T, int _axes>
auto EulerAngles<T, _axes>::operator = (const EulerAngles<T, _axes> &eulerAngles) -> EulerAngles<T, _axes>&
{
    if (this != &eulerAngles) {
        x = eulerAngles.x;
        y = eulerAngles.y;
        z = eulerAngles.z;
    }
    return *this;
}

template<typename T, int _axes>
auto EulerAngles<T, _axes>::operator = (EulerAngles<T, _axes> &&eulerAngles) TL_NOEXCEPT -> EulerAngles<T, _axes>&
{
    if (this != &eulerAngles) {
        x = std::exchange(eulerAngles.x, 0);
        y = std::exchange(eulerAngles.y, 0);
        z = std::exchange(eulerAngles.z, 0);
    }

    return *this;
}

/* Operaciones unarias */

template<typename T, int _axes>
inline auto EulerAngles<T, _axes>::operator+() -> EulerAngles<T, _axes>
{
    return *this;
}

template <typename T, int _axes>
inline auto EulerAngles<T, _axes>::operator-() -> EulerAngles<T, _axes>
{
    return EulerAngles<T, _axes>(-this->x,-this->y,-this->z);
}


/*! \} */ // end of rotation

/*! \} */ // end of algebra

/*! \} */ // end of math

} // End namespace tl

