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
    no_trans = 'N',             /*!< No transposition. */
    transpose = 'T',            /*!< Transpose operation. */
    conjugate_transpose = 'C'   /*!< Conjugate transpose operation. */
};

/*!
 * \brief Specifies whether a matrix has a unit diagonal or not.
 *
 * Used in LAPACK functions to indicate if the diagonal elements
 * of a triangular matrix are assumed to be 1.0.
 */
enum class Diagonal : char
{
    unit = 'U',    /*!< Matrix is assumed to be unit triangular (diagonal is 1.0). */
    non_unit = 'N' /*!< Matrix is not assumed to be unit triangular. */
};

} // End namespace lapack

/*! \} */

} // End namespace tl

/// \cond
#endif // TL_HAVE_OPENBLAS
/// \endcond
