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

#include "tidop/math/base/lapack/Traits.h"

#include <cstddef>

namespace tl
{

/*! \addtogroup MathBase
 *  \{
 */

namespace lapack
{

/*!
 * \brief Solves a system of linear equations with a triangular matrix.
 * This function solves the system A * X = B, A**T * X = B, or A**H * X = B,
 * where A is a triangular matrix.
 * \tparam T The data type (float or double).
 * \param order Storage order (row-major or column-major).
 * \param form Whether A is upper ('U') or lower ('L') triangular.
 * \param transpose Specifies the form of the system (NoTrans, Trans, ConjTrans).
 * \param diag Whether A has unit diagonal ('U' for unit, 'N' for non-unit).
 * \param n Order of the matrix A.
 * \param nrhs Number of right-hand sides (columns of B).
 * \param a Pointer to the triangular matrix A.
 * \param lda Leading dimension of matrix A.
 * \param b Pointer to the right-hand side matrix B. On exit, overwritten by solution X.
 * \param ldb Leading dimension of matrix B.
 * \return true if the matrix is singular (diagonal element is zero), false otherwise.
 */
template<typename T>
auto trtrs(Order order, TriangularForm form, Transpose transpose, Diagonal diag,
           size_t n, size_t nrhs, const T *a, size_t lda,
           T *b, size_t ldb) -> bool
{
    lapack_int info = LapackTraits<T>::trtrs(static_cast<int>(order),
                                             static_cast<char>(form),
                                             static_cast<char>(transpose),
                                             static_cast<char>(diag),
                                             static_cast<lapack_int>(n),
                                             static_cast<lapack_int>(nrhs),
                                             a, static_cast<lapack_int>(lda),
                                             b, static_cast<lapack_int>(ldb));

    if (info < 0) {
        TL_THROW_EXCEPTION("trtrs: Argument {} has an illegal value", -info);
    }

    return (info > 0);
}

} // End namespace lapack

/*! \} */

} // End namespace tl

/// \cond
#endif // TL_HAVE_OPENBLAS
/// \endcond
