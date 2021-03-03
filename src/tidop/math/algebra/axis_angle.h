/************************************************************************
 *                                                                      *
 * Copyright (C) 2020 by Tidop Research Group                           *
 *                                                                      *
 * This file is part of TidopLib                                        *
 *                                                                      *
 * TidopLib is free software: you can redistribute it and/or modify     *
 * it under the terms of the GNU General Public License as published by *
 * the Free Software Foundation, either version 3 of the License, or    *
 * (at your option) any later version.                                  *
 *                                                                      *
 * TidopLib is distributed in the hope that it will be useful,          *
 * but WITHOUT ANY WARRANTY; without even the implied warranty of       *
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the        *
 * GNU General Public License for more details.                         *
 *                                                                      *
 * You should have received a copy of the GNU General Public License    *
 * along with Foobar.  If not, see <http://www.gnu.org/licenses/>.      *
 *                                                                      *
 * @license GPL-3.0+ <http://spdx.org/licenses/GPL-3.0+>                *
 *                                                                      *
 ************************************************************************/

#ifndef TL_MATH_AXIS_ANGLE_H
#define TL_MATH_AXIS_ANGLE_H

#include "config_tl.h"

#include <vector>
#include <array>

#include "tidop/math/algebra/vector.h"
#include "tidop/math/algebra/rotations.h"

namespace tl
{

namespace math
{

/*! \addtogroup Math
 *  \{
 */


 /*! \addtogroup Algebra
  *
  * Algebra
  *
  *  \{
  */

/*!
 * \brief Notación axial-angular
 */
template<typename T>
class AxisAngle
  : public RotationBase<T>
{

public:

  AxisAngle();
  AxisAngle(T angle, const Vector<T, 3> &axis);
  ~AxisAngle() override;

public:

  T angle;
  Vector<T, 3> axis;
};

template<typename T>
AxisAngle<T>::AxisAngle()
  : RotationBase<T>(Rotation::Type::axis_angle),
    angle(0)
{
  axis.at(0) = 1;
  axis.at(1) = 0;
  axis.at(2) = 0;
}

template<typename T>
AxisAngle<T>::AxisAngle(T angle, const Vector<T, 3> &axis)
  : RotationBase<T>(Rotation::Type::axis_angle),
    angle(angle),
    axis(axis)
{
  TL_TODO("normalizar vector")
}

template<typename T>
AxisAngle<T>::~AxisAngle()
{

}

/*! \} */ // end of Algebra

/*! \} */ // end of Math

} // Fin namespace math

} // End namespace tl

#endif // TL_MATH_AXIS_ANGLE_H
