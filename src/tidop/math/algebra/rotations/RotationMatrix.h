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

#include <utility>

#include "tidop/math/algebra/matrix/Matrix.h"
#include "tidop/math/algebra/rotations/Rotations.h"

namespace tl
{

/*! \addtogroup Rotations
 *  \{
 */

/*!
 * \brief Rotation Matrix
 *
 * This class represents a 3x3 rotation matrix used to describe rotations in 3D space.
 * A rotation matrix is an orthogonal matrix with determinant 1. It can be used to
 * perform linear transformations such as rotation of vectors in 3D space.
 * It can be directly used to rotate points and vectors in space, and it is often
 * used in combination with other rotation representations such as quaternions and Euler angles.
 */
template <typename T>
class RotationMatrix
  : public OrientationBase<RotationMatrix<T>>,
    public Matrix<T, 3, 3>
{

public:

    using value_type = T;

public:

    /*!
     * \brief Default constructor
     * Initializes the rotation matrix to the identity matrix.
     */
    RotationMatrix();

    /*!
     * \brief Copy constructor
     * \param[in] rot The rotation matrix object to copy.
     */
    RotationMatrix(const RotationMatrix<T> &rot);

    /*!
     * \brief Move constructor
     * \param[in] rot The rotation matrix object to move.
     */
    RotationMatrix(RotationMatrix<T> &&rot) noexcept;

    /*!
     * \brief Constructor from a matrix
     * \param[in] rot A 3x3 matrix to initialize the rotation matrix.
     */
    RotationMatrix(const Matrix<T, 3, 3> &rot);

    /*!
     * \brief Destructor
     */
    ~RotationMatrix() override = default;

    /*!
     * \brief Assignment operator
     * \param[in] rot The rotation matrix object to copy.
     * \return A reference to the current rotation matrix.
     */
    auto operator=(const RotationMatrix<T> &rot) -> RotationMatrix &;

    /*!
     * \brief Move assignment operator
     * \param[in] rot The rotation matrix object to move.
     * \return A reference to the current rotation matrix.
     */
    auto operator=(RotationMatrix &&rot) noexcept -> RotationMatrix &;

};


template <typename T>
RotationMatrix<T>::RotationMatrix()
  : OrientationBase<RotationMatrix<T>>(Orientation::Type::rotation_matrix),
    Matrix<T, 3, 3>()
{
}

template <typename T>
RotationMatrix<T>::RotationMatrix(const RotationMatrix<T> &rot)
  : OrientationBase<RotationMatrix<T>>(Orientation::Type::rotation_matrix),
    Matrix<T, 3, 3>(rot)
{
}

template <typename T>
RotationMatrix<T>::RotationMatrix(RotationMatrix<T> &&rot) noexcept
  : OrientationBase<RotationMatrix<T>>(Orientation::Type::rotation_matrix),
    Matrix<T, 3, 3>(std::move(rot))
{
}

template <typename T>
RotationMatrix<T>::RotationMatrix(const Matrix<T, 3, 3> &rot)
  : OrientationBase<RotationMatrix<T>>(Orientation::Type::rotation_matrix),
    Matrix<T, 3, 3>(rot)
{

}

template <typename T>
auto RotationMatrix<T>::operator = (const RotationMatrix<T> &rot) -> RotationMatrix<T> &
{
    if (this != &rot) {
        Matrix<T, 3, 3>::operator = (rot);
    }

    return *this;
}

template <typename T>
auto RotationMatrix<T>::operator = (RotationMatrix &&rot) noexcept -> RotationMatrix<T> &
{
    if (this != &rot) {
        Matrix<T, 3, 3>::operator = (std::move(rot));
    }

    return *this;
}

/*! \} */

} // End namespace tl

