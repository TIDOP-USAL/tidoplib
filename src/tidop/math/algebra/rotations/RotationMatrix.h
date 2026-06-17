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
  : public RotationBase<RotationMatrix<T>>,
    public Matrix<T, 3, 3>
{

    static_assert(Floating<T>, "Integral type not supported");

public:

    using value_type = T;

public:

    /*!
     * \brief Default constructor
     * Initializes the rotation matrix to the identity matrix.
     */
    constexpr RotationMatrix();

    /*!
     * \brief Copy constructor
     * \param[in] rot The rotation matrix object to copy.
     */
    constexpr RotationMatrix(const RotationMatrix<T> &rot) = default;

    /*!
     * \brief Move constructor
     * \param[in] rot The rotation matrix object to move.
     */
    constexpr RotationMatrix(RotationMatrix<T> &&rot) noexcept = default;

    /*!
     * \brief Constructor from a matrix
     * \param[in] rot A 3x3 matrix to initialize the rotation matrix.
     */
    RotationMatrix(Matrix<T, 3, 3> rot);

    /*!
     * \brief Destructor
     */
    ~RotationMatrix() = default;

    /*!
     * \brief Assignment operator
     * \param[in] rot The rotation matrix object to copy.
     * \return A reference to the current rotation matrix.
     */
    constexpr auto operator=(const RotationMatrix<T> &rot) -> RotationMatrix & = default;

    /*!
     * \brief Move assignment operator
     * \param[in] rot The rotation matrix object to move.
     * \return A reference to the current rotation matrix.
     */
    constexpr auto operator=(RotationMatrix &&rot) noexcept -> RotationMatrix & = default;

};


template <typename T>
constexpr RotationMatrix<T>::RotationMatrix()
  : Matrix<T, 3, 3>()
{
}

template <typename T>
RotationMatrix<T>::RotationMatrix(Matrix<T, 3, 3> rot)
  : Matrix<T, 3, 3>(std::move(rot))
{

}


/*! \} */

} // End namespace tl

