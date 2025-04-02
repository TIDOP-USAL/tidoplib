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


#include "tidop/math/math.h"

/// \cond
#ifdef TL_HAVE_OPENBLAS
/// \endcond

#include <cblas.h>

namespace tl
{


/*! \addtogroup MathBase
 *  \{
 */

namespace blas
{

/*!
 * \brief Storage order for matrices.
 *
 * Specifies whether matrices are stored in row-major or column-major order.
 */
enum class Order
{
    row_major = CBLAS_ORDER::CblasRowMajor, /**< Row-major storage order. */
    col_major = CBLAS_ORDER::CblasColMajor  /**< Column-major storage order. */
};

/*!
 * \brief Specifies the side on which a matrix is applied.
 *
 * Determines whether the matrix is on the left or right in operations like SYMM and TRMM.
 */
enum class Side
{
    left = CBLAS_SIDE::CblasLeft,  /*!< Matrix is on the left side of the operation. */
    right = CBLAS_SIDE::CblasRight /*!< Matrix is on the right side of the operation. */
};

/*!
 * \brief Specifies the triangular form of a matrix.
 *
 * Indicates whether a triangular matrix is upper or lower triangular.
 */
enum class TriangularForm
{
    upper = CBLAS_UPLO::CblasUpper, /*!< Upper triangular matrix. */
    lower = CBLAS_UPLO::CblasLower  /*!< Lower triangular matrix. */
};

/*!
 * \brief Specifies how a matrix should be transposed in an operation.
 *
 * Determines whether a matrix is transposed, conjugate transposed, or not transposed.
 */
enum class TransposeMode
{
    no_transpose = CBLAS_TRANSPOSE::CblasNoTrans, /**< No transposition. */
    transpose = CBLAS_TRANSPOSE::CblasTrans, /**< Transposition. */
    conjugate_transpose = CBLAS_TRANSPOSE::CblasConjTrans, /**< Conjugate transposition. */
    conjugate_no_transpose = CBLAS_TRANSPOSE::CblasConjNoTrans /**< Conjugate without transposition. */
};


/*!
 * \brief Traits class to map floating point types to BLAS functions.
 *
 * This structure provides static mappings to the appropriate BLAS functions for
 * single and double precision floating point types.
 *
 * \tparam T The floating point type (float or double).
 */
template <typename T>
struct BlasTraits;

/*!
 * \brief Specialization of BlasTraits for float type.
 */
template <>
struct BlasTraits<float>
{
    static constexpr auto gemm = cblas_sgemm; /**< Single precision GEMM function. */
    static constexpr auto gemv = cblas_sgemv; /**< Single precision GEMV function. */
    static constexpr auto symm = cblas_ssymm; /**< Single precision SYMM function. */
    static constexpr auto trmm = cblas_strmm; /**< Single precision TRMM function. */
};

/*!
 * \brief Specialization of BlasTraits for double type.
 */
template <>
struct BlasTraits<double>
{
    static constexpr auto gemm = cblas_dgemm; /**< Double precision GEMM function. */
    static constexpr auto gemv = cblas_dgemv; /**< Double precision GEMV function. */
    static constexpr auto symm = cblas_dsymm; /**< Double precision SYMM function. */
    static constexpr auto trmm = cblas_dtrmm; /**< Double precision TRMM function. */
};


/* Matrix multiplication */

/*!
 * \brief General Matrix-Matrix multiplication (GEMM) for floating point types.
 *
 * This function performs matrix-matrix multiplication using the General Matrix-Matrix (GEMM) operation.
 *
 * \tparam T The data type (must be `float` or `double`).
 * \param[in] order The storage order of matrices (Row-major or Column-major).
 * \param[in] atranspose Specifies whether to transpose matrix A before multiplication.
 * \param[in] btranspose Specifies whether to transpose matrix B before multiplication.
 * \param[in] m The number of rows of matrix A and matrix C.
 * \param[in] n The number of columns of matrix B and matrix C.
 * \param[in] k The number of columns of matrix A and rows of matrix B.
 * \param[in] alpha Scalar multiplier for the matrix product.
 * \param[in] a Pointer to matrix A of size `m x k` or `k x m` depending on `atranspose`.
 * \param[in] lda Leading dimension of matrix A.
 * \param[in] b Pointer to matrix B of size `k x n` or `n x k` depending on `btranspose`.
 * \param[in] ldb Leading dimension of matrix B.
 * \param[in] beta Scalar multiplier for matrix C.
 * \param[out] c Pointer to output matrix C of size `m x n`.
 * \param[in] ldc Leading dimension of matrix C.
 */
template<typename T>
void gemm(Order order, TransposeMode atranspose, TransposeMode btranspose, size_t m, size_t n, size_t k,
          T alpha, const T *a, size_t lda, const T *b, size_t ldb, T beta, T *c, size_t ldc)
{
    BlasTraits<T>::gemm(static_cast<CBLAS_ORDER>(order),
                        static_cast<CBLAS_TRANSPOSE>(atranspose),
                        static_cast<CBLAS_TRANSPOSE>(btranspose),
                        static_cast<blasint>(m),
                        static_cast<blasint>(n),
                        static_cast<blasint>(k),
                        alpha,
                        a, static_cast<blasint>(lda),
                        b, static_cast<blasint>(ldb),
                        beta,
                        c, static_cast<blasint>(ldc));
}

/*!
 * \brief Symmetric Matrix-Matrix multiplication (SYMM) for floating point types.
 *
 * This function performs symmetric matrix-matrix multiplication.
 *
 * \tparam T The data type (must be `float` or `double`).
 * \param[in] order The storage order of matrices.
 * \param[in] side Specifies whether A is on the left or right of B.
 * \param[in] form Specifies whether A is upper or lower triangular.
 * \param[in] m The number of rows of matrix B and matrix C.
 * \param[in] n The number of columns of matrix B and matrix C.
 * \param[in] alpha Scalar multiplier for the matrix product.
 * \param[in] a Pointer to symmetric matrix A.
 * \param[in] lda Leading dimension of matrix A.
 * \param[in] b Pointer to matrix B of size `m x n`.
 * \param[in] ldb Leading dimension of matrix B.
 * \param[in] beta Scalar multiplier for matrix C.
 * \param[out] c Pointer to output matrix C of size `m x n`.
 * \param[in] ldc Leading dimension of matrix C.
 */
template<typename T>
void symm(Order order, Side side, TriangularForm form, size_t m, size_t n,
          T alpha, const T *a, size_t lda, const T *b, size_t ldb,
          T beta, T *c, size_t ldc)
{
    BlasTraits<T>::symm(static_cast<CBLAS_ORDER>(order),
                        static_cast<CBLAS_SIDE>(side),
                        static_cast<CBLAS_UPLO>(form),
                        static_cast<blasint>(m),
                        static_cast<blasint>(n),
                        alpha,
                        a, static_cast<blasint>(lda),
                        b, static_cast<blasint>(ldb),
                        beta,
                        c, static_cast<blasint>(ldc));
}

/*!
 * \brief Triangular Matrix-Matrix multiplication (TRMM) for floating point types.
 *
 * This function performs triangular matrix-matrix multiplication.
 *
 * \tparam T The data type (must be `float` or `double`).
 * \param[in] order The storage order of matrices.
 * \param[in] side Specifies whether A is on the left or right of B.
 * \param[in] form Specifies whether A is upper or lower triangular.
 * \param[in] transpose Specifies whether to transpose matrix A before multiplication.
 * \param[in] m The number of rows of matrix B.
 * \param[in] n The number of columns of matrix B.
 * \param[in] alpha Scalar multiplier for the matrix product.
 * \param[in] a Pointer to triangular matrix A.
 * \param[in] lda Leading dimension of matrix A.
 * \param[in,out] b Pointer to matrix B of size `m x n`.
 * \param[in] ldb Leading dimension of matrix B.
 */
template<typename T>
void trmm(Order order, Side side, TriangularForm form, TransposeMode transpose,
    size_t m, size_t n, T alpha, const T *a, size_t lda, T *b, size_t ldb)
{
    BlasTraits<T>::trmm(static_cast<CBLAS_ORDER>(order),
                        static_cast<CBLAS_SIDE>(side),
                        static_cast<CBLAS_UPLO>(form),
                        static_cast<CBLAS_TRANSPOSE>(transpose),
                        CBLAS_DIAG::CblasNonUnit,
                        static_cast<blasint>(m),
                        static_cast<blasint>(n),
                        alpha,
                        a, static_cast<blasint>(lda),
                        b, static_cast<blasint>(ldb));
}

/* Matrix x Vector */

/*! 
 * \brief General Matrix-Vector multiplication (GEMV) for floating point types.
 *
 * This function performs matrix-vector multiplication using the General Matrix-Vector (GEMV) operation
 * for floating point types (`float` or `double`). It relies on BLAS routines for efficient computation.
 *
 * \tparam T The data type (must be `float` or `double`).
 * \param[in] order The storage order of matrix A (Row-major or Column-major).
 * \param[in] transpose Specifies whether to transpose matrix A before multiplication.
 * \param[in] m The number of rows of matrix A.
 * \param[in] n The number of columns of matrix A.
 * \param[in] alpha Scalar multiplier for the matrix-vector product.
 * \param[in] a Pointer to the matrix A of size `m x n` stored in column-major or row-major order.
 * \param[in] lda Leading dimension of matrix A, which is at least `m` if column-major, or `n` if row-major.
 * \param[in] x Pointer to the input vector of size `n`.
 * \param[in] incx Stride between consecutive elements in vector x.
 * \param[in] beta Scalar multiplier for the output vector y.
 * \param[out] y Pointer to the output vector of size `m`, where the result will be stored.
 * \param[in] incy Stride between consecutive elements in vector y.
 */
template<typename T>
void gemv(Order order, TransposeMode transpose, size_t m, size_t n, T alpha,
    const T *a, size_t lda, const T *x, size_t incx, T beta, T *y, size_t incy)
{
    BlasTraits<T>::gemv(static_cast<CBLAS_ORDER>(order),
                        static_cast<CBLAS_TRANSPOSE>(transpose),
                        static_cast<blasint>(m),
                        static_cast<blasint>(n),
                        alpha,
                        a, static_cast<blasint>(lda),
                        x, static_cast<blasint>(incx),
                        beta,
                        y, static_cast<blasint>(incy));
}

} // End namespace blas

/*! \} */

} // End namespace tl

/// \cond
#endif // TL_HAVE_OPENBLAS
/// \endcond
