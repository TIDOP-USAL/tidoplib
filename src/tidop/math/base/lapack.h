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

/* Eigen Decomposition */

/*!
 * \brief Computes the eigenvalues and optionally the eigenvectors of a symmetric matrix.
 *
 * This function is a wrapper for the LAPACKE_ssyev (float) and LAPACKE_dsyev (double) functions.
 * It always assumes row-major storage order.
 *
 * \tparam T The floating-point type (float or double).
 * \param jobz Specifies whether to compute eigenvectors ('V' for compute, 'N' for no computation).
 * \param uplo Specifies whether the upper ('U') or lower ('L') triangle of the matrix is stored.
 * \param n The order of the matrix (number of rows and columns).
 * \param a Pointer to the matrix data.
 * \param lda Leading dimension of the matrix.
 * \param w Pointer to the array where computed eigenvalues will be stored.
 * \return lapack_int LAPACK status code (0 if successful, <0 for illegal values, >0 for convergence failure).
 */
template<typename T>
auto syev(char jobz, char uplo, lapack_int n, T *a, lapack_int lda, T *w) -> enableIfFloat<T, lapack_int>
{
    lapack_int info = LAPACKE_ssyev(LAPACK_ROW_MAJOR, jobz, uplo, n, a, lda, w);
    return info;
}

template<typename T>
auto syev(char jobz, char uplo, lapack_int n, T *a, lapack_int lda, T *w) -> enableIfDouble<T, lapack_int>
{
    lapack_int info = LAPACKE_dsyev(LAPACK_ROW_MAJOR, jobz, uplo, n, a, lda, w);
    return info;
}

/*!
 * \brief Computes the eigenvalues and eigenvectors of a general (possibly non-symmetric) matrix.
 *
 * This function is a wrapper for the LAPACKE_sgeev (float) and LAPACKE_dgeev (double) functions.
 * It always assumes row-major storage order.
 *
 * \tparam T The floating-point type (float or double).
 * \param jobvl Specifies whether to compute left eigenvectors ('V' for compute, 'N' for no computation).
 * \param jobvr Specifies whether to compute right eigenvectors ('V' for compute, 'N' for no computation).
 * \param n The order of the matrix (number of rows and columns).
 * \param a Pointer to the matrix data.
 * \param lda Leading dimension of the matrix.
 * \param wr Pointer to the array where computed real parts of eigenvalues will be stored.
 * \param wi Pointer to the array where computed imaginary parts of eigenvalues will be stored.
 * \param vl Pointer to the array where left eigenvectors will be stored (if computed).
 * \param ldvl Leading dimension of vl.
 * \param vr Pointer to the array where right eigenvectors will be stored (if computed).
 * \param ldvr Leading dimension of vr.
 * \return lapack_int LAPACK status code (0 if successful, <0 for illegal values, >0 for convergence failure).
 */
template<typename T>
auto geev(char jobvl, char jobvr, 
          lapack_int n, T *a, 
          lapack_int lda, T *wr, 
          T *wi, T *vl, 
          lapack_int ldvl, T *vr, 
          lapack_int ldvr) -> enableIfFloat<T, lapack_int>
{
    lapack_int info = LAPACKE_sgeev(LAPACK_ROW_MAJOR, jobvl, jobvr, n, a, lda, wr, wi, vl, ldvl, vr, ldvr);
    return info;
}

template<typename T>
auto geev(char jobvl, char jobvr, 
          lapack_int n, T *a, 
          lapack_int lda, T *wr, 
          T *wi, T *vl, 
          lapack_int ldvl, T *vr, 
          lapack_int ldvr) -> enableIfDouble<T, lapack_int>
{
    lapack_int info = LAPACKE_dgeev(LAPACK_ROW_MAJOR, jobvl, jobvr, 
                                    n, a, lda, wr, wi, vl, ldvl, vr, ldvr);
    return info;
}

} // End namespace lapack

/*! \} */

} // End namespace tl

/// \cond
#endif // TL_HAVE_OPENBLAS
/// \endcond
