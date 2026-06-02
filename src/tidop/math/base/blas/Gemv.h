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
