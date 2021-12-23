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

#ifndef TL_MATH_ROTATION_MATRIX_H
#define TL_MATH_ROTATION_MATRIX_H

#include "config_tl.h"

#include <vector>
#include <array>

#include "tidop/math/algebra/matrix.h"
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
 * \brief Matriz de rotación
 */
template <typename T>
class RotationMatrix 
  : public RotationBase<T>,
    public Matrix<T, 3, 3>
{

public:
  
  RotationMatrix();
  RotationMatrix(const RotationMatrix<T> &rot);
  RotationMatrix(const Matrix<T, 3, 3> &rot);
  ~RotationMatrix() override = default;


private:

};


template <typename T> inline
RotationMatrix<T>::RotationMatrix()
  : RotationBase<T>(Rotation::Type::rotation_matrix),
    Matrix<T, 3, 3>()
{
}

template <typename T> inline
RotationMatrix<T>::RotationMatrix(const RotationMatrix<T> &rot)
  : RotationBase<T>(Rotation::Type::rotation_matrix),
    Matrix<T, 3, 3>(rot)
{
}

template <typename T> inline
RotationMatrix<T>::RotationMatrix(const Matrix<T, 3, 3> &rot)
  : RotationBase<T>(Rotation::Type::rotation_matrix),
    Matrix<T, 3, 3>(rot)
{
}

/*! \} */ // end of rotation

/*! \} */ // end of algebra

/*! \} */ // end of math

} // Fin namespace math

} // End namespace tl

#endif // TL_MATH_ROTATION_MATRIX_H
