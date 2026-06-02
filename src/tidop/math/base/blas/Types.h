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
    no_transpose = CBLAS_TRANSPOSE::CblasNoTrans,              /*!< No transposition. */
    transpose = CBLAS_TRANSPOSE::CblasTrans,                   /*!< Transposition. */
    conjugate_transpose = CBLAS_TRANSPOSE::CblasConjTrans,     /*!< Conjugate transposition. */
    conjugate_no_transpose = CBLAS_TRANSPOSE::CblasConjNoTrans /*!< Conjugate without transposition. */
};

} // End namespace blas

/*! \} */

} // End namespace tl

/// \cond
#endif // TL_HAVE_OPENBLAS
/// \endcond
