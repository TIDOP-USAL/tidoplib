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

#ifndef TL_MATH_ROTATIONS_H
#define TL_MATH_ROTATIONS_H

#include "tidop/core/defs.h"

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
 * \brief Rotation Interface
 */
class TL_EXPORT Rotation
{

public:

  /*!
   * \brief Rotacion types
   */
  enum class Type
  {
    axis_angle,      /*!< Axial-angular */
    euler_angles,    /*!< Euler/TaitBryan angles*/
    rotation_matrix, /*!< Rotation matriz */
    quaternion       /*!< Quaternions */
  };

public:

  Rotation() = default;
  virtual ~Rotation() = default;

  /*!
   * \brief Rotation type
   * \see Rotation::Type
   */
  virtual Type rotationType() const = 0;

};


/*!
 * \brief Base class for rotations
 */
template<typename T>
class RotationBase
  : public Rotation
{

public:
  
  RotationBase(Type rotationType);
  RotationBase(const RotationBase &rotation);
  ~RotationBase() override = default;

  Type rotationType() const override;

private:

  Type mRotationType;

};



/* RotationBase implementation */


template<typename T> inline
RotationBase<T>::RotationBase(Type rotationType)
  : mRotationType(rotationType)
{
}

template<typename T> inline
RotationBase<T>::RotationBase(const RotationBase &rotation)
  : mRotationType(rotation.mRotationType)
{
}

template<typename Point_t>
Rotation::Type RotationBase<Point_t>::rotationType() const
{
  return mRotationType;
}

/*! \} */ // end of rotation

/*! \} */ // end of algebra

/*! \} */ // end of math


} // Fin namespace math

} // End namespace tl

#endif // TL_MATH_ROTATIONS_H
