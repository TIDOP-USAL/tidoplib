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
 * \brief Computes the LU factorization of a matrix.
 *
 * This function performs the LU factorization of a matrix using the LAPACK functions `LAPACKE_sgetrf` or `LAPACKE_dgetrf`,
 * depending on the data type (`float` or `double`). The factorization produces a permutation matrix \( P \),
 * a lower triangular matrix \( L \), and an upper triangular matrix \( U \), such that \( PA = LU \).
 *
 * If the matrix is singular (i.e., at least one diagonal element of \( U \) is exactly zero),
 * the function will return `true`. Otherwise, it returns `false`.
 *
 * If an input argument has an illegal value, an exception of type `tl::Exception` is thrown.
 *
 * \tparam T The data type (must be `float` or `double`).
 * \param[in] order The storage order of the matrix (row-major or column-major).
 * \param[in] rows The number of rows in the matrix.
 * \param[in] cols The number of columns in the matrix.
 * \param[in,out] a Pointer to the matrix data. On entry, it contains the matrix to be factorized.
 *                  On exit, it contains the L and U factors from the decomposition.
 * \param[in] lda The leading dimension of the matrix `a`.
 * \param[out] ipiv Pointer to an array of pivot indices of size `min(rows, cols)`.
 *                  The indices indicate row swaps applied during factorization.
 * \return `true` if the matrix is singular (i.e., U has a zero diagonal element), `false` otherwise.
 * \throws tl::Exception if an input argument has an illegal value.
 */
template<typename T>
auto getrf(Order order, size_t rows, size_t cols, T* a, size_t lda, int * ipiv) -> bool
{
    lapack_int info = LapackTraits<T>::getrf(static_cast<int>(order),
                                             static_cast<lapack_int>(rows),
                                             static_cast<lapack_int>(cols),
                                             a, static_cast<lapack_int>(lda), ipiv);

    if (info < 0) {
        TL_THROW_EXCEPTION("getrf: Argument {} has an illegal value", info);
    }

    return (info > 0);
}

/*!
 * \brief Solves a system of linear equations using LU factorization for floating point types.
 *
 * This function solves a system of linear equations \( AX = B \) using the LU factorization
 * previously computed by `getrf`. It calls the LAPACK function `LAPACKE_sgetrs` or `LAPACKE_dgetrs`,
 * depending on the data type (`float` or `double`).
 *
 * \tparam T The data type (must be `float` or `double`).
 * \param[in] order The storage order of the matrix (row-major or column-major).
 * \param[in] transpose Specifies whether to solve with \( A \), \( A^T \) (transpose), or \( A^H \) (conjugate transpose).
 * \param[in] rows The number of rows in the coefficient matrix \( A \).
 * \param[in] nrhs The number of right-hand sides, i.e., the number of columns in \( B \).
 * \param[in] a Pointer to the LU-factorized matrix \( A \) obtained from `getrf`.
 * \param[in] lda The leading dimension of matrix \( A \).
 * \param[in] ipiv Pointer to the pivot indices of size `rows`, as returned by `getrf`.
 * \param[in,out] b Pointer to the right-hand side matrix \( B \).
 *                  On entry, it contains the right-hand side matrix.
 *                  On exit, it contains the solution matrix \( X \).
 * \param[in] ldb The leading dimension of the right-hand side matrix \( B \).
 * \throws tl::Exception if an input argument has an illegal value.
 */
template<typename T>
void getrs(Order order, Transpose transpose, size_t rows, size_t nrhs, const T* a, size_t lda,
           const lapack_int* ipiv, T* b, size_t ldb)
{
    lapack_int info = LapackTraits<T>::getrs(static_cast<int>(order),
                                             static_cast<char>(transpose),
                                             static_cast<lapack_int>(rows),
                                             static_cast<lapack_int>(nrhs),
                                             a, static_cast<lapack_int>(lda),
                                             ipiv,
                                             b, static_cast<lapack_int>(ldb));

    if (info < 0) {
        TL_THROW_EXCEPTION("getrs: Argument {} has an illegal value", info);
    }
}

} // End namespace lapack

/*! \} */

} // End namespace tl

/// \cond
#endif // TL_HAVE_OPENBLAS
/// \endcond
