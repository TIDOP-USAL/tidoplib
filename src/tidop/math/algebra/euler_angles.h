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

#ifndef TL_MATH_EULER_ANGLES_H
#define TL_MATH_EULER_ANGLES_H

#include <vector>
#include <array>

#include "tidop/math/math.h"
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

/*! \defgroup rotations Rotations
 *  \{
 */


/*!
 * \brief Ángulos de Euler
 */
template<typename T>
class EulerAngles
  : public RotationBase<T>
{

public:

  enum class Axes
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

public:

  EulerAngles();
  EulerAngles(double x, double y, double z, Axes axes);
  EulerAngles(const EulerAngles<T> &eulerAngles);
  EulerAngles(EulerAngles<T> &&eulerAngles) TL_NOEXCEPT;
  ~EulerAngles() override = default;

  EulerAngles &operator = (const EulerAngles<T> &eulerAngles);
  EulerAngles &operator = (EulerAngles<T> &&eulerAngles) TL_NOEXCEPT;

public:

  double x;
  double y;
  double z;
  Axes axes;

};

template<typename T>
EulerAngles<T>::EulerAngles()
  : RotationBase<T>(Rotation::Type::euler_angles),
    x{0},
    y{0},
    z{0},
    axes(Axes::xyz)
{
}

template<typename T>
EulerAngles<T>::EulerAngles(double x, 
                            double y, 
                            double z, 
                            Axes axes)
  : RotationBase<T>(Rotation::Type::euler_angles),
    x(x),
    y(y),
    z(z),
    axes(axes)
{
}

template<typename T>
EulerAngles<T>::EulerAngles(const EulerAngles<T> &eulerAngles)
  : RotationBase<T>(Rotation::Type::euler_angles),
    x(eulerAngles.x),
    y(eulerAngles.y),
    z(eulerAngles.z),
    axes(eulerAngles.axes)
{
}

template<typename T>
EulerAngles<T>::EulerAngles(EulerAngles<T> &&eulerAngles) TL_NOEXCEPT
  : RotationBase<T>(Rotation::Type::euler_angles),
    x(std::exchange(eulerAngles.x, 0)),
    y(std::exchange(eulerAngles.y, 0)),
    z(std::exchange(eulerAngles.z, 0)),
    axes(std::exchange(eulerAngles.axes, Axes::xyz))
{
}

template<typename T>
EulerAngles<T> &EulerAngles<T>::operator = (const EulerAngles &eulerAngles)
{
  if (this != &eulerAngles) {
    x = eulerAngles.x;
    y = eulerAngles.y;
    z = eulerAngles.z;
    axes = eulerAngles.axes;
  }
  return *this;
}

template<typename T>
EulerAngles<T> &EulerAngles<T>::operator = (EulerAngles<T> &&eulerAngles) TL_NOEXCEPT
{
  if (this != &eulerAngles) {
    x = std::exchange(eulerAngles.x, 0);
    y = std::exchange(eulerAngles.y, 0);
    z = std::exchange(eulerAngles.z, 0);
    axes = std::exchange(eulerAngles.axes, Axes::xyz);
  }
  return *this;
}

/* Operaciones unarias */


template <typename T>
EulerAngles<T> operator - (const EulerAngles<T> &eulerAngles)
{
  return EulerAngles<T>(-eulerAngles.x,
                        -eulerAngles.y, 
                        -eulerAngles.z,
                        eulerAngles.axes);
}


/*! \} */ // end of rotation

/*! \} */ // end of algebra

/*! \} */ // end of math

} // Fin namespace math

} // End namespace tl

#endif // TL_MATH_EULER_ANGLES_H
