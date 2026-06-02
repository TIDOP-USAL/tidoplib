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
 * \brief Cholesky factorization for floating point types.
 *
 * This function computes the Cholesky factorization of a symmetric positive-definite matrix \( A \),
 * using the LAPACK function `LAPACKE_spotrf` or `LAPACKE_dpotrf`, depending on the data type (`float` or `double`).
 * The result is stored in either the upper or lower triangular part of the matrix.
 *
 * \tparam T The data type (must be `float` or `double`).
 * \param[in] order The storage order of the matrix (row-major or column-major).
 * \param[in] form Specifies whether the upper or lower triangular part of \( A \) is used.
 * \param[in] rows The number of rows and columns of the matrix (must be square).
 * \param[in,out] a Pointer to the matrix data. On exit, it contains the Cholesky factor \( U \) or \( L \).
 * \param[in] lda Leading dimension of the matrix.
 * \throws tl::Exception if an argument is invalid or if the matrix is not positive definite.
 */
template<typename T>
void potrf(Order order, TriangularForm form, size_t rows, T* a, size_t lda)
{
    lapack_int info = LapackTraits<T>::potrf(static_cast<int>(order), static_cast<char>(form),
                                             static_cast<lapack_int>(rows), a, static_cast<lapack_int>(lda));

    if (info < 0) {
        TL_THROW_EXCEPTION("potrf: Argument {} has an illegal value", -info);
    }

    if (info > 0) {
        TL_THROW_EXCEPTION("potrf: The leading minor of order {} is not positive definite", info);
    }
}


/*!
 * \brief Solves a system of linear equations using Cholesky factorization for floating point types.
 *
 * Given a symmetric positive-definite matrix \( A \) and a right-hand side matrix \( B \),
 * this function solves \( AX = B \) using the Cholesky factorization \( A = U^T U \) (upper form)
 * or \( A = LL^T \) (lower form) computed by `potrf`.
 *
 * \tparam T The data type (must be `float` or `double`).
 * \param[in] order The storage order of the matrix (row-major or column-major).
 * \param[in] form Specifies whether the upper or lower triangular part of \( A \) is used.
 * \param[in] n The order of the coefficient matrix \( A \) (number of rows and columns, must be square).
 * \param[in] nrhs The number of right-hand sides, i.e., the number of columns in \( B \).
 * \param[in] a Pointer to the Cholesky factorized matrix \( A \) (upper or lower triangular form).
 * \param[in] lda The leading dimension of matrix \( A \).
 * \param[in,out] b Pointer to the right-hand side matrix \( B \).
 *                  On entry, it contains the right-hand side matrix.
 *                  On exit, it contains the solution matrix \( X \).
 * \param[in] ldb The leading dimension of the right-hand side matrix \( B \).
 * \throws tl::Exception if an input argument has an illegal value.
 */
template<typename T>
void potrs(Order order, TriangularForm form, size_t n, size_t nrhs,
           const T *a, size_t lda, T *b, size_t ldb)
{
    lapack_int info = LapackTraits<T>::potrs(static_cast<int>(order),
                                             static_cast<char>(form),
                                             static_cast<lapack_int>(n),
                                             static_cast<lapack_int>(nrhs),
                                             a, static_cast<lapack_int>(lda),
                                             b, static_cast<lapack_int>(ldb));

    if (info < 0) {
        TL_THROW_EXCEPTION("potrs: Argument {} has an illegal value", info);
    }
}


/*!
 * \brief Computes the inverse of a symmetric positive-definite matrix using the Cholesky factorization.
 *
 * This function computes the inverse of a symmetric positive-definite matrix \( A \)
 * using the Cholesky factorization \( A = U^T U \) or \( A = L L^T \) computed by `potrf`.
 *
 * \tparam T The data type (must be `float` or `double`).
 * \param[in] order The storage order of the matrix (row-major or column-major).
 * \param[in] form Specifies whether the factor is upper (U) or lower (L).
 * \param[in] n The order of the matrix \( A \).
 * \param[in,out] a Pointer to the Cholesky factor. On exit, it contains the inverse matrix.
 * \param[in] lda The leading dimension of matrix \( A \).
 * \throws tl::Exception if an argument is invalid or if the factor is singular.
 */
template<typename T>
void potri(Order order, TriangularForm form, size_t n, T *a, size_t lda)
{
    lapack_int info = LapackTraits<T>::potri(static_cast<int>(order),
                                             static_cast<char>(form),
                                             static_cast<lapack_int>(n),
                                             a,
                                             static_cast<lapack_int>(lda));

    if (info < 0) {
        TL_THROW_EXCEPTION("potri: Argument {} has an illegal value", -info);
    }

    if (info > 0) {
        TL_THROW_EXCEPTION("potri: Element ({}, {}) of the factor is zero; the matrix is singular", info, info);
    }
}

} // End namespace lapack

/*! \} */

} // End namespace tl

/// \cond
#endif // TL_HAVE_OPENBLAS
/// \endcond
