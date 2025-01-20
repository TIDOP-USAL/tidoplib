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

/* Matrix multiplication */

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
auto gemm(size_t m, size_t n, size_t k,
          const T *a, const T *b, T *c) -> enableIfFloat<T, void>
{
    T alpha = 1.f;
    T beta = 0.f;
    blasint lda = static_cast<blasint>(k);
    blasint ldb = static_cast<blasint>(n);
    blasint ldc = static_cast<blasint>(n);

    cblas_sgemm(CBLAS_ORDER::CblasRowMajor,
                CBLAS_TRANSPOSE::CblasNoTrans,
                CBLAS_TRANSPOSE::CblasNoTrans,
                static_cast<blasint>(m),
                static_cast<blasint>(n),
                static_cast<blasint>(k),
                alpha, a, lda, b, ldb,
                beta, c, ldc);
}

template<typename T>
auto gemm(size_t m, size_t n, size_t k,
          const T *a, const T *b, T *c) -> enableIfDouble<T, void>
{
    T alpha = 1.;
    T beta = 0.;
    blasint lda = static_cast<blasint>(k);
    blasint ldb = static_cast<blasint>(n);
    blasint ldc = static_cast<blasint>(n);

    cblas_dgemm(CBLAS_ORDER::CblasRowMajor,
                CBLAS_TRANSPOSE::CblasNoTrans,
                CBLAS_TRANSPOSE::CblasNoTrans,
                static_cast<blasint>(m),
                static_cast<blasint>(n),
                static_cast<blasint>(k),
                alpha, a, lda, b, ldb,
                beta, c, ldc);
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
