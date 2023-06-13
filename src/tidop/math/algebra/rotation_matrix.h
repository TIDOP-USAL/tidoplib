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

#include "tidop/math/algebra/matrix.h"
#include "tidop/math/algebra/rotations.h"

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


/*!
 * \brief Matriz de rotación
 */
template <typename T>
class RotationMatrix
  : public OrientationBase<T>,
    public Matrix<T, 3, 3>
{

public:

    RotationMatrix();
    RotationMatrix(const RotationMatrix<T> &rot);
    RotationMatrix(RotationMatrix<T> &&rot) TL_NOEXCEPT;
    RotationMatrix(const Matrix<T, 3, 3> &rot);
    ~RotationMatrix() override = default;

    RotationMatrix &operator = (const RotationMatrix<T> &rot);
    RotationMatrix &operator = (RotationMatrix &&rot) TL_NOEXCEPT;

private:

};


template <typename T> inline
RotationMatrix<T>::RotationMatrix()
  : OrientationBase<T>(Orientation::Type::rotation_matrix),
    Matrix<T, 3, 3>()
{
}

template <typename T> inline
RotationMatrix<T>::RotationMatrix(const RotationMatrix<T> &rot)
  : OrientationBase<T>(Orientation::Type::rotation_matrix),
    Matrix<T, 3, 3>(rot)
{
}

template <typename T> inline
RotationMatrix<T>::RotationMatrix(RotationMatrix<T> &&rot) TL_NOEXCEPT
  : OrientationBase<T>(std::forward<OrientationBase<T>>(rot)),
    Matrix<T, 3, 3>(std::forward<Matrix<T, 3, 3>>(rot))
{
}

template <typename T> inline
RotationMatrix<T>::RotationMatrix(const Matrix<T, 3, 3> &rot)
  : OrientationBase<T>(Orientation::Type::rotation_matrix),
    Matrix<T, 3, 3>(rot)
{

}

template <typename T>
inline RotationMatrix<T> &RotationMatrix<T>::operator = (const RotationMatrix<T> &rot)
{
    if (this != &rot) {
        OrientationBase<T>::operator = (rot);
        Matrix<T, 3, 3>::operator = (rot);
    }

    return *this;
}

template <typename T>
inline RotationMatrix<T> &RotationMatrix<T>::operator = (RotationMatrix &&rot) TL_NOEXCEPT
{
    if (this != &rot) {
        OrientationBase<T>::operator = (std::forward<OrientationBase<T>>(rot));
        Matrix<T, 3, 3>::operator = (std::forward<Matrix<T, 3, 3>>(rot));
    }

    return *this;
}

/*! \} */ // end of rotation

/*! \} */ // end of algebra

/*! \} */ // end of math

} // End namespace tl

