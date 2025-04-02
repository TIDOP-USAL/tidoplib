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

/*!
 * \brief Specifies the storage order for matrices.
 *
 * Defines whether matrices are stored in row-major or column-major order,
 * compatible with LAPACK conventions.
 */
enum class Order
{
    row_major = LAPACK_ROW_MAJOR, /*!< Row-major storage order. */
    col_major = LAPACK_COL_MAJOR  /*!< Column-major storage order. */
};

/*!
 * \brief Specifies whether a matrix is upper or lower triangular.
 *
 * Used in LAPACK functions that require information about triangular matrices.
 */
enum class TriangularForm : char
{
    upper = 'U', /*!< Upper triangular matrix. */
    lower = 'L'  /*!< Lower triangular matrix. */
};

/*!
 * \brief Specifies matrix transposition modes.
 *
 * Defines whether a matrix is transposed or conjugate transposed before operations.
 */
enum class Transpose : char
{
    no_trans = 'N', /*!< No transposition. */
    transpose = 'T', /*!< Transpose operation. */
    conjugate_transpose = 'C' /*!< Conjugate transpose operation. */
};


/*!
 * \brief LAPACK function mappings for different floating-point types.
 *
 * Defines a set of LAPACK functions specialized for `float` and `double` types.
 * This allows type-safe LAPACK function calls.
 *
 * \tparam T Floating-point type (`float` or `double`).
 */
template <typename T>
struct LapackTraits;

/*!
 * \brief Specialization of LapackTraits for `float` type.
 *
 * Provides LAPACK function mappings for single-precision floating point operations.
 */
template <>
struct LapackTraits<float>
{
    static constexpr auto getrf = LAPACKE_sgetrf; /*!< LU decomposition. */
    static constexpr auto getrs = LAPACKE_sgetrs; /*!< Solves a system using LU decomposition. */
    static constexpr auto potrf = LAPACKE_spotrf; /*!< Cholesky decomposition. */
    static constexpr auto potrs = LAPACKE_spotrs; /*!< Solves a system using Cholesky decomposition. */
    static constexpr auto geqrf = LAPACKE_sgeqrf; /*!< QR decomposition. */
    static constexpr auto orgqr = LAPACKE_sorgqr; /*!< Generates an orthogonal matrix from QR decomposition. */
    static constexpr auto gels = LAPACKE_sgels;   /*!< Solves linear least squares problems. */
    static constexpr auto gesvd = LAPACKE_sgesvd; /*!< Singular value decomposition (SVD). */
    static constexpr auto syev = LAPACKE_ssyev;   /*!< Computes eigenvalues and eigenvectors of a symmetric matrix. */
    static constexpr auto geev = LAPACKE_sgeev;   /*!< Computes eigenvalues and eigenvectors of a general matrix. */
};

/*!
 * \brief Specialization of LapackTraits for `double` type.
 *
 * Provides LAPACK function mappings for double-precision floating point operations.
 */
template <>
struct LapackTraits<double>
{
    static constexpr auto getrf = LAPACKE_dgetrf; /*!< LU decomposition. */
    static constexpr auto getrs = LAPACKE_dgetrs; /*!< Solves a system using LU decomposition. */
    static constexpr auto potrf = LAPACKE_dpotrf; /*!< Cholesky decomposition. */
    static constexpr auto potrs = LAPACKE_dpotrs; /*!< Solves a system using Cholesky decomposition. */
    static constexpr auto geqrf = LAPACKE_dgeqrf; /*!< QR decomposition. */
    static constexpr auto orgqr = LAPACKE_dorgqr; /*!< Generates an orthogonal matrix from QR decomposition. */
    static constexpr auto gels = LAPACKE_dgels;   /*!< Solves linear least squares problems. */
    static constexpr auto gesvd = LAPACKE_dgesvd; /*!< Singular value decomposition (SVD). */
    static constexpr auto syev = LAPACKE_dsyev;   /*!< Computes eigenvalues and eigenvectors of a symmetric matrix. */
    static constexpr auto geev = LAPACKE_dgeev;   /*!< Computes eigenvalues and eigenvectors of a general matrix. */
};



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
 * \brief QR decomposition for floating point types.
 *
 * This function computes the QR decomposition of a matrix \( A \), factorizing it as:
 * \f[
 * A = QR
 * \f]
 * where \( Q \) is an orthogonal (or unitary) matrix, and \( R \) is an upper triangular matrix.
 * It uses the LAPACK function `LAPACKE_sgeqrf` or `LAPACKE_dgeqrf`, depending on the data type (`float` or `double`).
 *
 * \tparam T The data type (must be `float` or `double`).
 * \param[in] order The storage order of the matrix (row-major or column-major).
 * \param[in] m The number of rows in the matrix \( A \).
 * \param[in] n The number of columns in the matrix \( A \).
 * \param[in,out] a Pointer to the matrix data. On exit, contains the factors of \( Q \) and \( R \).
 * \param[in] lda Leading dimension of the matrix.
 * \param[out] tau Pointer to an array of length \f$ \min(m, n) \f$, storing scalar factors of elementary reflectors.
 * \throws std::invalid_argument If any input argument has an illegal value.
 */
template<typename T>
void geqrf(Order order, size_t m, size_t n, T *a, size_t lda, T *tau)
{
    lapack_int info = LapackTraits<T>::geqrf(static_cast<int>(order),
                                             static_cast<lapack_int>(m),
                                             static_cast<lapack_int>(n),
                                             a, static_cast<lapack_int>(lda), tau);

    if (info < 0) {
        TL_THROW_EXCEPTION("geqrf: Argument {} has an illegal value", info);
    }
}



/*!
 * \brief Generates an orthogonal matrix from QR decomposition.
 *
 * This function constructs the orthogonal (or unitary) matrix \( Q \) from the QR decomposition
 * computed by `geqrf`, using the LAPACK function `LAPACKE_sorgqr` or `LAPACKE_dorgqr`.
 *
 * \tparam T The data type (must be `float` or `double`).
 * \param[in] order The storage order of the matrix (row-major or column-major).
 * \param[in] m The number of rows in the matrix \( Q \).
 * \param[in] n The number of columns in the matrix \( Q \).
 * \param[in] k The number of elementary reflectors used to construct \( Q \).
 * \param[in,out] a Pointer to the matrix data. On entry, contains the output of `geqrf`. On exit, contains the matrix \( Q \).
 * \param[in] lda Leading dimension of the matrix.
 * \param[in] tau Pointer to the array of scalar factors from `geqrf`.
 * \throws std::invalid_argument If any input argument has an illegal value.
 */
template<typename T>
void orgqr(Order order, size_t m, size_t n, size_t k, T *a, size_t lda, T *tau)
{
    lapack_int info = LapackTraits<T>::orgqr(static_cast<int>(order), 
                                             static_cast<lapack_int>(m),
                                             static_cast<lapack_int>(n),
                                             static_cast<lapack_int>(k),
                                             a, static_cast<lapack_int>(lda), tau);

    if (info > 0)
        TL_THROW_EXCEPTION("orgqr: Argument {} has an illegal value", info);
}



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

//gelsd: Computes the minimum norm solution to a linear least squares problem using the singular value decomposition of A and a divide and conquer method.


/*!
 * \brief Options for returning U and V^T matrices in Singular Value Decomposition (SVD).
 *
 * This enumeration defines the options for computing the left singular vectors (U) and
 * the right singular vectors (V^T) in the Singular Value Decomposition (SVD) algorithm.
 */
enum class SVDMode : char
{
    full = 'A',      ///< Returns full U (m x m) and V^T (n x n)
    reduced = 'S',   ///< Returns reduced U (m x min(m,n)) and V^T (min(m,n) x n)
    overwrite = 'O', ///< Overwrites A with U or V^T
    none = 'N'       ///< Does not compute U or V^T
};

/*!
 * \brief Computes Singular Value Decomposition (SVD) using LAPACK.
 *
 * Given an m × n matrix \( A \), this function computes its singular value decomposition:
 * \f[
 * A = U \Sigma V^T
 * \f]
 * where:
 * - \( U \) is an m × m or m × min(m,n) orthogonal matrix (left singular vectors).
 * - \( \Sigma \) is a diagonal matrix of singular values of size min(m,n).
 * - \( V^T \) is an n × n or min(m,n) × n orthogonal matrix (right singular vectors).
 *
 * \tparam T Floating point type (`float` or `double`).
 * \param[in] order The storage order of the matrix (row-major or column-major).
 * \param[in] jobu Specifies how the U matrix is stored:
 *        - `SVDMode::full` → Compute full U (m × m).
 *        - `SVDMode::reduced` → Compute reduced U (m × min(m,n)).
 *        - `SVDMode::overwrite` → Overwrite A with U.
 *        - `SVDMode::none` → Do not compute U.
 * \param[in] jobvt Specifies how the V^T matrix is stored:
 *        - `SVDMode::full` → Compute full V^T (n × n).
 *        - `SVDMode::reduced` → Compute reduced V^T (min(m,n) × n).
 *        - `SVDMode::overwrite` → Overwrite A with V^T.
 *        - `SVDMode::none` → Do not compute V^T.
 * \param[in] m Number of rows of matrix \( A \).
 * \param[in] n Number of columns of matrix \( A \).
 * \param[in,out] a Pointer to matrix \( A \) of size (m × n). Overwritten with U or V^T if `jobu` or `jobvt` is `overwrite`.
 * \param[in] lda Leading dimension of \( A \) (must be \(\geq \max(1, m)\)).
 * \param[out] s Pointer to an array of size min(m,n) where singular values will be stored.
 * \param[out] u Pointer to the U matrix, depending on `jobu`. If `SVDMode::none`, this can be `nullptr`.
 * \param[in] ldu Leading dimension of \( U \) (must be \(\geq 1\) if `jobu != SVDMode::none`, and \(\geq m\) otherwise).
 * \param[out] vt Pointer to the V^T matrix, depending on `jobvt`. If `SVDMode::none`, this can be `nullptr`.
 * \param[in] ldvt Leading dimension of \( V^T \) (must be \(\geq 1\) if `jobvt != SVDMode::none`, and \(\geq n\) otherwise).
 * \param[out] superb Pointer to an array of size min(m,n)-1, used for intermediate computations.
 * \throws tl::Exception if an argument is invalid or if the algorithm fails to converge.
 */
template<typename T>
void gesvd(Order order, SVDMode jobu, SVDMode jobvt,
           size_t m, size_t n, T *a, size_t lda,
           T *s, T *u, size_t ldu, T *vt,
           size_t ldvt, T *superb)
{
    lapack_int info = LapackTraits<T>::gesvd(static_cast<int>(order),
                                             static_cast<char>(jobu), 
                                             static_cast<char>(jobvt), 
                                             static_cast<lapack_int>(m), 
                                             static_cast<lapack_int>(n),
                                             a, static_cast<lapack_int>(lda), 
                                             s, u, static_cast<lapack_int>(ldu),
                                             vt, static_cast<lapack_int>(ldvt), superb);

    if (info < 0) {
        TL_THROW_EXCEPTION("gesvd: Argument {} has an illegal value", -info);
    }

    if (info > 0) {
        TL_THROW_EXCEPTION("gesvd: The algorithm computing SVD failed to converge. {} superdiagonals did not converge to zero.", info);
    }
}

// gesdd: Computes the singular value decomposition of a general rectangular matrix using a divide and conquer method.


/* Eigen Decomposition */

/*!
 * \brief Specifies whether to compute eigenvectors in eigenvalue decomposition.
 */
enum class EigenVectors : char
{
    compute = 'V', ///< Compute eigenvectors.
    none = 'N'     ///< Do not compute eigenvectors.
};


/*!
 * \brief Computes the eigenvalues and optionally the eigenvectors of a symmetric matrix.
 *
 * This function is a wrapper for the LAPACKE_ssyev (float) and LAPACKE_dsyev (double) functions.
 * It always assumes row-major storage order.
 *
 * \tparam T The floating-point type (`float` or `double`).
 * \param[in] order Matrix storage order (row-major or column-major).
 * \param[in] jobz Specifies whether to compute eigenvectors (`EigenVectors::compute` or `EigenVectors::none`).
 * \param[in] uplo Specifies which triangle of the matrix is stored (`TriangularForm::upper` or `TriangularForm::lower`).
 * \param[in] n The order of the matrix (number of rows and columns).
 * \param[in,out] a Pointer to the matrix data. The content is overwritten.
 * \param[in] lda Leading dimension of the matrix (must be \(\geq n\)).
 * \param[out] w Pointer to the array where computed eigenvalues will be stored (size `n`).
 * \throws tl::Exception if an argument is invalid or if the algorithm fails to converge.
 * 
 * \note If \p jobz is `EigenVectors::compute`, \p a will contain the eigenvectors on exit.
 *       If \p jobz is `EigenVectors::none`, \p a is overwritten but does not contain eigenvectors.
 */
template<typename T>
void syev(Order order, EigenVectors jobz, TriangularForm uplo, size_t n, T *a, size_t lda, T *w)
{
    lapack_int info = LapackTraits<T>::syev(static_cast<int>(order), 
                                            static_cast<char>(jobz), 
                                            static_cast<char>(uplo), 
                                            static_cast<lapack_int>(n), a, 
                                            static_cast<lapack_int>(lda), w);
    
    if (info < 0) {
        TL_THROW_EXCEPTION("syev: Argument {} has an illegal value", -info);
    }

    if (info > 0) {
        TL_THROW_EXCEPTION("syev: The algorithm failed to converge. {} off-diagonal elements did not converge to zero.", info);
    }
}


/*!
 * \brief Computes the eigenvalues and eigenvectors of a general (possibly non-symmetric) matrix.
 *
 * This function is a wrapper for the LAPACKE_sgeev (float) and LAPACKE_dgeev (double) functions.
 * It always assumes row-major storage order.
 *
 * \tparam T The floating-point type (`float` or `double`).
 * \param[in] order Matrix storage order (row-major or column-major).
 * \param[in] jobvl Specifies whether to compute left eigenvectors (`EigenVectors::compute` or `EigenVectors::none`).
 * \param[in] jobvr Specifies whether to compute right eigenvectors (`EigenVectors::compute` or `EigenVectors::none`).
 * \param[in] n The order of the matrix (number of rows and columns).
 * \param[in,out] a Pointer to the matrix data. The content is overwritten.
 * \param[in] lda Leading dimension of the matrix (must be \(\geq n\)).
 * \param[out] wr Pointer to the array where the real parts of the computed eigenvalues will be stored (size `n`).
 * \param[out] wi Pointer to the array where the imaginary parts of the computed eigenvalues will be stored (size `n`).
 * \param[out] vl Pointer to the array where the left eigenvectors will be stored (if computed).
 * \param[in] ldvl Leading dimension of `vl` (must be \(\geq 1\) if computed, otherwise ignored).
 * \param[out] vr Pointer to the array where the right eigenvectors will be stored (if computed).
 * \param[in] ldvr Leading dimension of `vr` (must be \(\geq 1\) if computed, otherwise ignored).
 * \throws tl::Exception if an argument is invalid or if the algorithm fails to compute all eigenvalues.
 */
template<typename T>
void geev(Order order, EigenVectors jobvl, EigenVectors jobvr, size_t n, T *a,
          size_t lda, T *wr, T *wi, T *vl, size_t ldvl, T *vr, size_t ldvr)
{
    lapack_int info = LapackTraits<T>::geev(static_cast<int>(order),
                                            static_cast<char>(jobvl),
                                            static_cast<char>(jobvr), 
                                            static_cast<lapack_int>(n), 
                                            a, static_cast<lapack_int>(lda), 
                                            wr, wi, vl, 
                                            static_cast<lapack_int>(ldvl), 
                                            vr, static_cast<lapack_int>(ldvr));
    if (info < 0) {
        TL_THROW_EXCEPTION("geev: Argument {} has an illegal value.", -info);
    }

    if (info > 0) {
        TL_THROW_EXCEPTION("geev: The QR algorithm failed to compute all eigenvalues. "
                           "Only elements 1 to {} in WR and WI are valid.", info);
    }
}


} // End namespace lapack

/*! \} */

} // End namespace tl

/// \cond
#endif // TL_HAVE_OPENBLAS
/// \endcond
