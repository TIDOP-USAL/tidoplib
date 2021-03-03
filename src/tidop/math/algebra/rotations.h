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

#ifndef TL_MATH_ROTATIONS_H
#define TL_MATH_ROTATIONS_H

#include "config_tl.h"
#include "tidop/core/defs.h"

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
 * \brief Interfaz Rotation
 */
class TL_EXPORT Rotation
{

public:

  /*!
   * \brief Tipos de rotaciones
   */
  enum class Type
  {
    axis_angle,      /*!< Axial-angular */
    euler_angles,    /*!< Angulos Euler/TaitBryan */
    rotation_matrix, /*!< Matriz de rotación */
    quaternion       /*!< Quaterniones */
  };

public:

  Rotation() {}
  virtual ~Rotation() = default;

  /*!
   * \brief Tipo de rotacion
   * \see Rotation::Type
   */
  virtual Type rotationType() const = 0;

};


/*!
 * \brief Clase base para rotaciones
 */
template<typename T>
class RotationBase
  : public Rotation
{

public:
  
  RotationBase(Type rotationType);
  ~RotationBase() override = default;

  Type rotationType() const override;

protected:

  /*!
   * \brief Tipo de rotación
   * \see Rotation::Type
   */
  Type mRotationType;

};



/// Implementación RotationBase


template<typename T> inline
RotationBase<T>::RotationBase(Type rotationType)
  : mRotationType(rotationType)
{
}

template<typename Point_t>
Rotation::Type RotationBase<Point_t>::rotationType() const
{
  return mRotationType;
}


/*! \} */ // end of Algebra

/*! \} */ // end of Math


} // Fin namespace math

} // End namespace tl

#endif // TL_MATH_ROTATIONS_H
