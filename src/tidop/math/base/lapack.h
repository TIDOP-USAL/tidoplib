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

#include "tidop/core/base/defs.h"
#include "tidop/math/math.h"

#include <lapacke.h>

namespace tl
{


/*! \addtogroup MathBase
 *  \{
 */

namespace lapack
{


/* Factorización LU */

/*!
 * \brief LU factorization for floating point types.
 *
 * This function performs the LU factorization of a matrix using the LAPACKE_sgetrf function,
 * which is optimized for floating point types (`float` or `double`).
 *
 * \tparam T The data type (must be `float` or `double`).
 * \param[in] rows The number of rows in the matrix.
 * \param[in] cols The number of columns in the matrix.
 * \param[in,out] a Pointer to the matrix data.
 * \param[in] lda Leading dimension of the matrix.
 * \param[out] ipiv Pointer to the pivot indices.
 * \return Information about the success or failure of the operation.
 */
template<typename T>
auto getrf(lapack_int rows, lapack_int cols, T* a, lapack_int lda, lapack_int* ipiv) -> enableIfFloat<T, lapack_int>
{
    lapack_int info = LAPACKE_sgetrf(LAPACK_ROW_MAJOR, rows, cols, a, lda, ipiv);
    return info;
}

template<typename T>
auto getrf(lapack_int rows, lapack_int cols, T* a, lapack_int lda, lapack_int* ipiv) -> enableIfDouble<T, lapack_int>
{
    lapack_int info = LAPACKE_dgetrf(LAPACK_ROW_MAJOR, rows, cols, a, lda, ipiv);
    return info;
}

/* Solving linear equations using LU factorization */

/*!
 * \brief Solves a system of linear equations using LU factorization for floating point types.
 *
 * This function solves a system of linear equations using the LAPACKE_sgetrs function,
 * which is optimized for floating point types (`float` or `double`).
 *
 * \tparam T The data type (must be `float` or `double`).
 * \param[in] rows The number of rows of the matrix.
 * \param[in] nrhs The number of right-hand sides (columns of the matrix B).
 * \param[in] a Pointer to the matrix data.
 * \param[in] lda Leading dimension of the matrix.
 * \param[in] ipiv Pointer to the pivot indices.
 * \param[in,out] b Pointer to the right-hand side matrix.
 * \param[in] ldb Leading dimension of the right-hand side matrix.
 * \return Information about the success or failure of the operation.
 */
template<typename T> 
auto getrs(lapack_int rows, lapack_int nrhs, T* a, lapack_int lda, 
           lapack_int* ipiv, T* b, lapack_int ldb) -> enableIfFloat<T, lapack_int>
{
    lapack_int info = LAPACKE_sgetrs(LAPACK_ROW_MAJOR, 'N', rows, nrhs, a, lda, ipiv, b, ldb);
    return info;
}

template<typename T> 
auto getrs(lapack_int rows, lapack_int nrhs, T* a, lapack_int lda, 
           lapack_int* ipiv, T* b, lapack_int ldb) -> enableIfDouble<T, lapack_int>
{
    lapack_int info = LAPACKE_dgetrs(LAPACK_ROW_MAJOR, 'N', rows, nrhs, a, lda, ipiv, b, ldb);
    return info;
}


/* Factorización Cholesky */

/*!
 * \brief Cholesky factorization for floating point types.
 *
 * This function performs the Cholesky factorization of a matrix using the LAPACKE_spotrf function,
 * which is optimized for floating point types (`float` or `double`).
 *
 * \tparam T The data type (must be `float` or `double`).
 * \param[in] rows The number of rows and columns of the matrix (must be square).
 * \param[in,out] a Pointer to the matrix data.
 * \param[in] lda Leading dimension of the matrix.
 * \return Information about the success or failure of the operation.
 */
template<typename T>
auto potrf(lapack_int rows, T* a, lapack_int lda) -> enableIfFloat<T, lapack_int>
{
    lapack_int info = LAPACKE_spotrf(LAPACK_ROW_MAJOR, 'L', rows, a, lda);
    return info;
}

template<typename T> 
auto potrf(lapack_int rows, T* a, lapack_int lda) -> enableIfDouble<T, lapack_int>
{
    lapack_int info = LAPACKE_dpotrf(LAPACK_ROW_MAJOR, 'L', rows, a, lda);
    return info;
}


/* Descomposición QR */

/*!
 * \brief QR decomposition for floating point types.
 *
 * This function performs the QR decomposition of a matrix using the LAPACKE_sgeqrf function,
 * which is optimized for floating point types (`float` or `double`).
 *
 * \tparam T The data type (must be `float` or `double`).
 * \param[in] rows The number of rows in the matrix.
 * \param[in] cols The number of columns in the matrix.
 * \param[in,out] a Pointer to the matrix data.
 * \param[in] lda Leading dimension of the matrix.
 * \return Information about the success or failure of the operation.
 */
template<typename T>
auto geqrf(lapack_int rows, lapack_int cols, T *a, lapack_int lda) -> enableIfFloat<T, lapack_int>
{
    std::vector<double> tau(std::min(rows, cols)); 

    lapack_int info = LAPACKE_sgeqrf(LAPACK_ROW_MAJOR, rows, cols, a, lda, tau.data());
    return info;
}

template<typename T> 
auto geqrf(lapack_int rows, lapack_int cols, T *a, lapack_int lda) -> enableIfDouble<T, lapack_int>
{
    std::vector<double> tau(std::min(rows, cols)); 

    lapack_int info = LAPACKE_dgeqrf(LAPACK_ROW_MAJOR, rows, cols, a, lda, tau.data());
    return info;
}




/* SVD (Singular value decomposition) */

/*!
 * \brief Singular Value Decomposition (SVD) for floating point types.
 *
 * This function performs the Singular Value Decomposition of a matrix using the LAPACKE_sgesvd function,
 * which is optimized for floating point types (`float` or `double`).
 *
 * \tparam T The data type (must be `float` or `double`).
 * \param[in] rows The number of rows in the matrix.
 * \param[in] cols The number of columns in the matrix.
 * \param[in,out] a Pointer to the matrix data.
 * \param[in] lda Leading dimension of the matrix.
 * \param[out] s Pointer to the singular values.
 * \param[out] u Pointer to the left singular vectors.
 * \param[in] ldu Leading dimension of the matrix U.
 * \param[out] v Pointer to the right singular vectors.
 * \param[in] ldvt Leading dimension of the matrix VT.
 * \param[in] superb Pointer to the array that holds the superdiagonal elements of the bidiagonal form.
 * \return Information about the success or failure of the operation.
 */
template<typename T> 
auto gesvd(lapack_int rows, lapack_int cols, T* a, lapack_int lda, T* s, T* u,
           lapack_int ldu, T* v, lapack_int ldvt, T* superb) -> enableIfFloat<T, lapack_int>
{
    lapack_int info = LAPACKE_sgesvd(LAPACK_ROW_MAJOR, 'A', 'A', rows, cols, a, lda, s, u, ldu, v, ldvt, superb);
    return info;
}

template<typename T> 
auto gesvd(lapack_int rows, lapack_int cols, T* a, lapack_int lda, T* s, T* u,
           lapack_int ldu, T* v, lapack_int ldvt, T* superb) -> enableIfDouble<T, lapack_int>
{
    lapack_int info = LAPACKE_dgesvd(LAPACK_ROW_MAJOR, 'A', 'A', rows, cols, a, lda, s, u, ldu, v, ldvt, superb);
    return info;
}


} // End namespace lapack

/*! \} */

} // End namespace tl

/// \cond
#endif // TL_HAVE_OPENBLAS
/// \endcond
