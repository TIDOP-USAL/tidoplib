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

enum class Order
{
    row_major = CBLAS_ORDER::CblasRowMajor,
    col_major = CBLAS_ORDER::CblasColMajor
};

enum class Side
{
    left = CBLAS_SIDE::CblasLeft,
    right = CBLAS_SIDE::CblasRight
};

enum class TriangularForm
{
    upper = CBLAS_UPLO::CblasUpper,
    lower = CBLAS_UPLO::CblasLower
};


/* Matrix multiplication */

template<typename T>
auto gemm(Order order, bool atranspose, bool btranspose, unsigned int m, unsigned int n, unsigned int k,
    T alpha, const T *a, unsigned int lda, const T *b, unsigned int ldb, T beta, T *c, unsigned int ldc) -> enableIfFloat<T, void>
{
    cblas_sgemm(static_cast<CBLAS_ORDER>(order),
                atranspose ? CBLAS_TRANSPOSE::CblasTrans : CBLAS_TRANSPOSE::CblasNoTrans,
                btranspose ? CBLAS_TRANSPOSE::CblasTrans : CBLAS_TRANSPOSE::CblasNoTrans,
                static_cast<blasint>(m),
                static_cast<blasint>(n),
                static_cast<blasint>(k),
                alpha, a, lda, b, ldb,
                beta, c, ldc);
}

template<typename T>
auto gemm(Order order, bool atranspose, bool btranspose, unsigned int m, unsigned int n, unsigned int k,
    T alpha, const T *a, unsigned int lda, const T *b, unsigned int ldb, T beta, T *c, unsigned int ldc) -> enableIfDouble<T, void>
{
    cblas_dgemm(static_cast<CBLAS_ORDER>(order),
                atranspose ? CBLAS_TRANSPOSE::CblasTrans : CBLAS_TRANSPOSE::CblasNoTrans,
                btranspose ? CBLAS_TRANSPOSE::CblasTrans : CBLAS_TRANSPOSE::CblasNoTrans,
                static_cast<blasint>(m),
                static_cast<blasint>(n),
                static_cast<blasint>(k),
                alpha, a, static_cast<blasint>(lda),
                b, static_cast<blasint>(ldb),
                beta, c, static_cast<blasint>(ldc));
}

/*!
 * \brief Matrix-matrix multiplication (gemm) for floating point types.
 *
 * This function performs matrix multiplication using the General Matrix Multiply (GEMM) operation
 * for floating point types (`float` or `double`).
 *
 * \tparam T The data type (must be `float` or `double`).
 * \param[in] m The number of rows of matrix A and matrix C.
 * \param[in] n The number of columns of matrix B and matrix C.
 * \param[in] k The number of columns of matrix A and rows of matrix B.
 * \param[in] a Pointer to the matrix A of size `m x k`.
 * \param[in] b Pointer to the matrix B of size `k x n`.
 * \param[out] c Pointer to the matrix C of size `m x n`, where the result will be stored.
 */
template<typename T>
auto gemm(size_t m, size_t n, size_t k, const T *a, const T *b, T *c) -> enableIfFloating<T, void>
{
    T alpha = 1.;
    T beta = 0.;

    gemm(blas::Order::row_major, false, false, m, n, k, alpha, a, /*lda*/ k, b, /*ldb*/ n, beta, c, /*ldc*/ n);
}






template<typename T>
auto symm(Side side, size_t m, size_t n, const T *a, const T *b, T *c) -> enableIfFloat<T, void>
{
    T alpha = 1.0;
    T beta = 0.0;
    blasint lda = static_cast<blasint>(m);
    blasint ldb = static_cast<blasint>(n);
    blasint ldc = static_cast<blasint>(n);

    cblas_ssymm(CBLAS_ORDER::CblasRowMajor, 
                static_cast<CBLAS_SIDE>(side),
                CBLAS_UPLO::CblasUpper,
                static_cast<blasint>(m), 
                static_cast<blasint>(n), 
                alpha, a, lda, b, ldb, 
                beta, c, ldc);
}

template<typename T>
auto symm(Side side, size_t m, size_t n, const T *a, const T *b, T *c) -> enableIfDouble<T, void>
{
    T alpha = 1.0;
    T beta = 0.0;
    blasint lda = static_cast<blasint>(m);
    blasint ldb = static_cast<blasint>(n);
    blasint ldc = static_cast<blasint>(n);

    cblas_dsymm(CBLAS_ORDER::CblasRowMajor,
                static_cast<CBLAS_SIDE>(side),
                CBLAS_UPLO::CblasUpper,
                static_cast<blasint>(m), 
                static_cast<blasint>(n), 
                alpha, a, lda, b, ldb, 
                beta, c, ldc);
}

template<typename T>
auto trmm(Side side, TriangularForm form, size_t m, size_t n, const T *a, T *b) -> enableIfFloat<T, void>
{
    T alpha = 1.0;
    blasint lda = static_cast<blasint>(m);
    blasint ldb = static_cast<blasint>(n);

    cblas_strmm(CBLAS_ORDER::CblasRowMajor,
                static_cast<CBLAS_SIDE>(side),
                static_cast<CBLAS_UPLO>(form),
                CBLAS_TRANSPOSE::CblasNoTrans,
                CBLAS_DIAG::CblasNonUnit, 
                static_cast<blasint>(m), 
                static_cast<blasint>(n), 
                alpha, a, lda, b, ldb);
}

template<typename T>
auto trmm(Side side, TriangularForm form, size_t m, size_t n, const T *a, T *b) -> enableIfDouble<T, void>
{
    T alpha = 1.0;
    blasint lda = static_cast<blasint>(m);
    blasint ldb = static_cast<blasint>(n);

    cblas_dtrmm(CBLAS_ORDER::CblasRowMajor,
                static_cast<CBLAS_SIDE>(side),
                static_cast<CBLAS_UPLO>(form),
                CBLAS_TRANSPOSE::CblasNoTrans,
                CBLAS_DIAG::CblasNonUnit,
                static_cast<blasint>(m), 
                static_cast<blasint>(n), 
                alpha, a, lda, b, ldb);
}


/* Matrix x Vector */

/*!
 * \brief Matrix-vector multiplication (gemv) for floating point types.
 *
 * This function performs matrix-vector multiplication using the General Matrix Vector (GEMV) operation
 * for floating point types (`float` or `double`).
 *
 * \tparam T The data type (must be `float` or `double`).
 * \param[in] m The number of rows of matrix A.
 * \param[in] n The number of columns of matrix A and the length of the vector.
 * \param[in] matrix Pointer to the matrix A of size `m x n`.
 * \param[in] vector Pointer to the vector B of size `n`.
 * \param[out] vector_out Pointer to the output vector C of size `m`, where the result will be stored.
 */
template<typename T>
auto gemv(size_t m, size_t n, const T *matrix, const T *vector, T *vector_out) -> enableIfFloat<T, void>
{
    T alpha = 1.;
    T beta = 0.;

    cblas_sgemv(CBLAS_ORDER::CblasRowMajor, 
                CBLAS_TRANSPOSE::CblasNoTrans, 
                static_cast<blasint>(m),
                static_cast<blasint>(n), 
                alpha, matrix,
                static_cast<blasint>(n), 
                vector, 1, 
                beta, vector_out, 1);
}

template<typename T>
auto gemv(size_t m, size_t n, const T *matrix, const T *vector, T *vector_out) -> enableIfDouble<T, void>
{
    T alpha = 1.;
    T beta = 0.;

    cblas_dgemv(CBLAS_ORDER::CblasRowMajor, 
                CBLAS_TRANSPOSE::CblasNoTrans,
                static_cast<blasint>(m),
                static_cast<blasint>(n), 
                alpha, matrix, 
                static_cast<blasint>(n),
                vector, 1, 
                beta, vector_out, 1);
}


} // End namespace blas

/*! \} */

} // End namespace tl

/// \cond
#endif // TL_HAVE_OPENBLAS
/// \endcond
