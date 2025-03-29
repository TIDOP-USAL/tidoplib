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

enum class Order
{
    row_major = LAPACK_ROW_MAJOR,
    col_major = LAPACK_COL_MAJOR
};

enum class TriangularForm : char
{
    upper = 'U',
    lower = 'L'
};


/* Factorización LU */

enum class LUTransposeMode : char
{
    NoTrans = 'N',
    Transpose = 'T',
    ConjugateTranspose = 'C'
};

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
auto getrf(Order order, lapack_int rows, lapack_int cols, T* a, lapack_int lda, lapack_int* ipiv) -> enableIfFloat<T, lapack_int>
{
    return LAPACKE_sgetrf(static_cast<int>(order), rows, cols, a, lda, ipiv);
}

template<typename T>
auto getrf(Order order, lapack_int rows, lapack_int cols, T* a, lapack_int lda, lapack_int* ipiv) -> enableIfDouble<T, lapack_int>
{
    return LAPACKE_dgetrf(static_cast<int>(order), rows, cols, a, lda, ipiv);
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
auto getrs(Order order, LUTransposeMode trans, lapack_int rows, lapack_int nrhs, T* a, lapack_int lda,
           lapack_int* ipiv, T* b, lapack_int ldb) -> enableIfFloat<T, lapack_int>
{
    return LAPACKE_sgetrs(static_cast<int>(order), static_cast<char>(trans), rows, nrhs, a, lda, ipiv, b, ldb);
}

template<typename T> 
auto getrs(Order order, LUTransposeMode trans, lapack_int rows, lapack_int nrhs, T* a, lapack_int lda,
           lapack_int* ipiv, T* b, lapack_int ldb) -> enableIfDouble<T, lapack_int>
{
    return LAPACKE_dgetrs(static_cast<int>(order), static_cast<char>(trans), rows, nrhs, a, lda, ipiv, b, ldb);
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
auto potrf(Order order, TriangularForm form, lapack_int rows, T* a, lapack_int lda) -> enableIfFloat<T, lapack_int>
{
    return LAPACKE_spotrf(static_cast<int>(order), static_cast<char>(form), rows, a, lda);
}

template<typename T> 
auto potrf(Order order, TriangularForm form, lapack_int rows, T* a, lapack_int lda) -> enableIfDouble<T, lapack_int>
{
    return LAPACKE_dpotrf(static_cast<int>(order), static_cast<char>(form), rows, a, lda);
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
 * \return Information about the success or failure of the operation.
 */
template<typename T>
auto geqrf(Order order, lapack_int m, lapack_int n, T *a, lapack_int lda, T *tau) -> enableIfFloat<T, lapack_int>
{
    return LAPACKE_sgeqrf(static_cast<int>(order), m, n, a, lda, tau);
}

template<typename T> 
auto geqrf(Order order, lapack_int m, lapack_int n, T *a, lapack_int lda, T *tau) -> enableIfDouble<T, lapack_int>
{
    return LAPACKE_dgeqrf(static_cast<int>(order), m, n, a, lda, tau);
}

template<typename T>
auto orgqr(Order order, lapack_int m, lapack_int n, lapack_int k, T *a, lapack_int lda, T *tau) -> enableIfFloat<T, lapack_int>
{
    return LAPACKE_sorgqr(static_cast<int>(order), m, n, k, a, lda, tau);
}

template<typename T>
auto orgqr(Order order, lapack_int m, lapack_int n, lapack_int k, T *a, lapack_int lda, T *tau) -> enableIfDouble<T, lapack_int>
{
    return LAPACKE_dorgqr(static_cast<int>(order), m, n, k, a, lda, tau);
}


/* SVD (Singular value decomposition) */

/*!
 * \brief Options for returning U and V^T matrices in SVD
 */
enum class SVDMode : char
{
    full = 'A',      ///< Returns full U (m x m) and V^T (n x n)
    reduced = 'S',   ///< Returns reduced U (m x min(m,n)) and V^T (min(m,n) x n)
    overwrite = 'O', ///< Overwrites A with U or V^T
    none = 'N'       ///< Does not compute U or V^T
};

/**
 * @brief Computes Singular Value Decomposition (SVD) using LAPACK
 *
 * \tparam T Data type (float or double)
 * \param order Matrix storage order (row_major or col_major)
 * \param jobu Specifies how the U matrix is stored
 *        - SVDMode::full -> U will be (m x m)
 *        - SVDMode::reduced -> U will be (m x min(m,n))
 *        - SVDMode::overwrite -> U overwrites A
 *        - SVDMode::none -> U is not computed
 * \param jobvt Specifies how the V^T matrix is stored
 *        - SVDMode::full -> V^T will be (n x n)
 *        - SVDMode::reduced -> V^T will be (min(m,n) x n)
 *        - SVDMode::overwrite -> V^T overwrites A
 *        - SVDMode::none -> V^T is not computed
 * \param m Number of rows in A
 * \param n Number of columns in A
 * \param a Pointer to matrix A of size (m x n)
 * \param lda Leading dimension of A (>= max(1, m))
 * \param s Vector where singular values will be stored (min(m, n))
 * \param u Pointer to U matrix (according to jobu)
 * \param ldu Leading dimension of U (>= 1 if U is used, >= m if jobu != none)
 * \param vt Pointer to V^T matrix (according to jobvt)
 * \param ldvt Leading dimension of V^T (>= 1 if V^T is used, >= n if jobvt != none)
 * \param superb Vector of size (min(m,n) - 1) for additional information
 * \return 0 if successful, > 0 if convergence failed
 */
template<typename T>
auto gesvd(Order order, SVDMode jobu, SVDMode jobvt,
           size_t m, size_t n, T *a, size_t lda,
           T *s, T *u, size_t ldu, T *vt,
           size_t ldvt, T *superb) -> enableIfFloat<T, size_t>
{
    lapack_int info = LAPACKE_sgesvd(static_cast<int>(order),
                                     static_cast<char>(jobu), 
                                     static_cast<char>(jobvt), 
                                     static_cast<lapack_int>(m), 
                                     static_cast<lapack_int>(n),
                                     a, static_cast<lapack_int>(lda), 
                                     s, u, static_cast<lapack_int>(ldu),
                                     vt, static_cast<lapack_int>(ldvt), superb);

    return static_cast<size_t>(info);
}

template<typename T>
auto gesvd(Order order, SVDMode jobu, SVDMode jobvt,
           size_t m, size_t n, T *a, size_t lda,
           T *s, T *u, size_t ldu, T *vt,
           size_t ldvt, T *superb) -> enableIfDouble<T, size_t>
{
    lapack_int info = LAPACKE_dgesvd(static_cast<int>(order),
                                     static_cast<char>(jobu),
                                     static_cast<char>(jobvt),
                                     static_cast<lapack_int>(m),
                                     static_cast<lapack_int>(n),
                                     a, static_cast<lapack_int>(lda),
                                     s, u, static_cast<lapack_int>(ldu),
                                     vt, static_cast<lapack_int>(ldvt), superb);

    return static_cast<size_t>(info);
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
