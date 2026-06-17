/**************************************************************************
 *                                                                        *
 * Copyright (C) 2021 by Tidop Research Group                             *
 * Copyright (C) 2021 by Esteban Ruiz de OÃ±a Crespo                       *
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

#include <type_traits>

#include "tidop/math/algebra/rotations/EulerAnglesAxis.h"

namespace tl
{

/*! \addtogroup Rotations
 *  \{
 */

template<typename T> class Quaternion;
template<typename T> class RotationMatrix;
template<typename T> class AxisAngle;
template<typename T, Axes Order> class EulerAngles;

template<typename T>
struct orientation_traits;

template<typename T>
struct orientation_traits<Quaternion<T>>
{
    using value_type = T;
};

template<typename T>
struct orientation_traits<RotationMatrix<T>>
{
    using value_type = T;
};

template<typename T>
struct orientation_traits<AxisAngle<T>>
{
    using value_type = T;
};

template<typename T, Axes Order>
struct orientation_traits<EulerAngles<T, Order>>
{
    using value_type = T;
};


/*! \} */

} // End namespace tl


