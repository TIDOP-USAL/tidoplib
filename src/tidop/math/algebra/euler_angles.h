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

#ifndef TL_MATH_EULER_ANGLES_H
#define TL_MATH_EULER_ANGLES_H

#include "config_tl.h"

#include <vector>
#include <array>

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
  EulerAngles(double omega, double phi, double kappa, Axes axes);
  EulerAngles(const EulerAngles<T> &eulerAngles);
  ~EulerAngles() override;

  /*!
   * \brief Operador de asignación
   * \param[in] eulerAngles Objeto que se copia
   */
  EulerAngles &operator = (const EulerAngles<T> &eulerAngles);

public:

  double omega;
  double phi;
  double kappa;
  Axes axes;

};

template<typename T>
EulerAngles<T>::EulerAngles()
  : RotationBase<T>(Rotation::Type::euler_angles),
    omega{0},
    phi{0},
    kappa{0},
    axes(Axes::xyz)
{
}

template<typename T>
EulerAngles<T>::EulerAngles(double omega, 
                            double phi, 
                            double kappa, 
                            Axes axes)
  : RotationBase<T>(Rotation::Type::euler_angles),
    omega(omega),
    phi(phi),
    kappa(kappa),
    axes(axes)
{
}

template<typename T>
EulerAngles<T>::EulerAngles(const EulerAngles<T> &eulerAngles)
  : RotationBase<T>(Rotation::Type::euler_angles),
    omega(eulerAngles.omega),
    phi(eulerAngles.phi),
    kappa(eulerAngles.kappa),
    axes(eulerAngles.axes)
{
}

template<typename T>
EulerAngles<T>::~EulerAngles()
{}

template<typename T>
EulerAngles<T> &EulerAngles<T>::operator = (const EulerAngles &eulerAngles)
{
  if (this != &eulerAngles) {
    omega = eulerAngles.omega;
    phi = eulerAngles.phi;
    kappa = eulerAngles.kappa;
    axes = eulerAngles.axes;
  }
  return *this;
}

/*! \} */ // end of Algebra

/*! \} */ // end of Math

} // Fin namespace math

} // End namespace tl

#endif // TL_MATH_EULER_ANGLES_H
