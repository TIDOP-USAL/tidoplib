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
 * - \( &Sigma; \) is a diagonal matrix of singular values of size min(m,n).
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

} // End namespace lapack

/*! \} */

} // End namespace tl

/// \cond
#endif // TL_HAVE_OPENBLAS
/// \endcond
