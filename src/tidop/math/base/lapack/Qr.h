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

} // End namespace lapack

/*! \} */

} // End namespace tl

/// \cond
#endif // TL_HAVE_OPENBLAS
/// \endcond
