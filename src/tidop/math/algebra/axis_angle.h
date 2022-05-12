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

#ifndef TL_MATH_AXIS_ANGLE_H
#define TL_MATH_AXIS_ANGLE_H

#include "tidop/math/math.h"

#include <vector>
#include <array>

#include "tidop/math/algebra/vector.h"
#include "tidop/math/algebra/rotations.h"

namespace tl
{

namespace math
{

/*! \addtogroup math
 *  \{
 */


 /*! \addtogroup algebra
  *  \{
  */

/*! \addtogroup rotations
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
  ~AxisAngle() override = default;

  T angle() const;
  void setAngle(T angle);
  Vector<T, 3> axis() const;
  T axis(size_t i) const;
  void setAxis(const Vector<T, 3> &axis);

private:

  T mAngle;
  Vector<T, 3> mAxis;
};


/* AxisAngle implementation */

template<typename T>
AxisAngle<T>::AxisAngle()
  : RotationBase<T>(Rotation::Type::axis_angle),
    mAngle(0),
    mAxis{1,0,0}
{
  static_assert(std::is_floating_point<T>::value, "Integral type not supported");
}

template<typename T>
AxisAngle<T>::AxisAngle(T angle, const Vector<T, 3> &axis)
  : RotationBase<T>(Rotation::Type::axis_angle),
    mAngle(angle),
    mAxis(axis)
{
  static_assert(std::is_floating_point<T>::value, "Integral type not supported");

  mAxis.normalize();
}

template<typename T>
inline T AxisAngle<T>::angle() const
{
  return mAngle;
}

template<typename T>
inline void AxisAngle<T>::setAngle(T angle)
{
  mAngle = angle;
}

template<typename T>
inline Vector<T, 3> AxisAngle<T>::axis() const
{
  return mAxis;
}

template<typename T>
inline T AxisAngle<T>::axis(size_t i) const
{
  TL_ASSERT((0 <= i) && (i < 3), "");
  return mAxis.at(i);
}

template<typename T>
inline void AxisAngle<T>::setAxis(const Vector<T, 3> &axis)
{
  mAxis = axis;
  mAxis.normalize();
}

/*! \} */ // end of rotations

/*! \} */ // end of algebra

/*! \} */ // end of math

} // Fin namespace math

} // End namespace tl

#endif // TL_MATH_AXIS_ANGLE_H
