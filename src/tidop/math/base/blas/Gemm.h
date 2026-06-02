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

#include "tidop/math/base/blas/Traits.h"

#include <cstddef>

namespace tl
{

/*! \addtogroup MathBase
 *  \{
 */

namespace blas
{

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

} // End namespace blas

/*! \} */

} // End namespace tl

/// \cond
#endif // TL_HAVE_OPENBLAS
/// \endcond
