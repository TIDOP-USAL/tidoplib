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

/// \cond
#ifdef TL_HAVE_OPENBLAS
/// \endcond

#include "tidop/math/base/lapack/Traits.h"

#include <cstddef>

namespace tl
{

/*! \addtogroup MathBase
 *  \{
 */

namespace lapack
{

/*!
 * \brief Solves a linear least squares problem.
 *
 * This function finds the least squares solution to an overdetermined system of linear equations:
 * \f[
 * Ax = B
 * \f]
 * where \( A \) is an \( m \times n \) matrix and \( B \) is an \( m \times nrhs \) matrix.
 * It uses the LAPACK function `LAPACKE_sgels` or `LAPACKE_dgels`, depending on the data type (`float` or `double`).
 *
 * \tparam T The data type (must be `float` or `double`).
 * \param[in] order The storage order of the matrix (row-major or column-major).
 * \param[in] transpose Specifies whether \( A \) is transposed or not before solving.
 * \param[in] m The number of rows in the matrix \( A \).
 * \param[in] n The number of columns in the matrix \( A \).
 * \param[in] nrhs The number of right-hand sides (columns of \( B \)).
 * \param[in,out] a Pointer to the matrix data \( A \). On exit, contains details of the QR or LQ factorization.
 * \param[in] lda Leading dimension of the matrix \( A \).
 * \param[in,out] b Pointer to the right-hand side matrix \( B \). On exit, contains the solution matrix \( X \).
 * \param[in] ldb Leading dimension of the matrix \( B \).
 * \throws tl::Exception if an argument is invalid or if the matrix \( A \) does not have full rank.
 */
template<typename T>
void gels(Order order, Transpose transpose, size_t m, size_t n, size_t nrhs, T *a, size_t lda, T *b, size_t ldb)
{
    lapack_int info = LapackTraits<T>::gels(static_cast<int>(order),
                                            static_cast<char>(transpose),
                                            static_cast<lapack_int>(m),
                                            static_cast<lapack_int>(n),
                                            static_cast<lapack_int>(nrhs),
                                            a, static_cast<lapack_int>(lda),
                                            b, static_cast<lapack_int>(ldb));

    if (info < 0) {
        TL_THROW_EXCEPTION("gels: Argument {} has an illegal value", -info);
    }

    if (info > 0) {
        TL_THROW_EXCEPTION("gels: The A({},{}) element of the triangular factor of A is zero, meaning A does not have full rank. The least squares solution could not be computed.", info, info);
    }
}

} // End namespace lapack

/*! \} */

} // End namespace tl

/// \cond
#endif // TL_HAVE_OPENBLAS
/// \endcond
