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
 * \brief Specifies whether to compute eigenvectors in eigenvalue decomposition.
 */
enum class EigenVectors : char
{
    compute = 'V', ///< Compute eigenvectors.
    none = 'N'     ///< Do not compute eigenvectors.
};

/*!
 * \brief Computes the eigenvalues and optionally the eigenvectors of a symmetric matrix.
 *
 * This function is a wrapper for the LAPACKE_ssyev (float) and LAPACKE_dsyev (double) functions.
 * It always assumes row-major storage order.
 *
 * \tparam T The floating-point type (`float` or `double`).
 * \param[in] order Matrix storage order (row-major or column-major).
 * \param[in] jobz Specifies whether to compute eigenvectors (`EigenVectors::compute` or `EigenVectors::none`).
 * \param[in] uplo Specifies which triangle of the matrix is stored (`TriangularForm::upper` or `TriangularForm::lower`).
 * \param[in] n The order of the matrix (number of rows and columns).
 * \param[in,out] a Pointer to the matrix data. The content is overwritten.
 * \param[in] lda Leading dimension of the matrix (must be \(\geq n\)).
 * \param[out] w Pointer to the array where computed eigenvalues will be stored (size `n`).
 * \throws tl::Exception if an argument is invalid or if the algorithm fails to converge.
 *
 * \note If \p jobz is `EigenVectors::compute`, \p a will contain the eigenvectors on exit.
 *       If \p jobz is `EigenVectors::none`, \p a is overwritten but does not contain eigenvectors.
 */
template<typename T>
void syev(Order order, EigenVectors jobz, TriangularForm uplo, size_t n, T *a, size_t lda, T *w)
{
    lapack_int info = LapackTraits<T>::syev(static_cast<int>(order),
                                            static_cast<char>(jobz),
                                            static_cast<char>(uplo),
                                            static_cast<lapack_int>(n), a,
                                            static_cast<lapack_int>(lda), w);

    if (info < 0) {
        TL_THROW_EXCEPTION("syev: Argument {} has an illegal value", -info);
    }

    if (info > 0) {
        TL_THROW_EXCEPTION("syev: The algorithm failed to converge. {} off-diagonal elements did not converge to zero.", info);
    }
}

/*!
 * \brief Computes the eigenvalues and eigenvectors of a general (possibly non-symmetric) matrix.
 *
 * This function is a wrapper for the LAPACKE_sgeev (float) and LAPACKE_dgeev (double) functions.
 * It always assumes row-major storage order.
 *
 * \tparam T The floating-point type (`float` or `double`).
 * \param[in] order Matrix storage order (row-major or column-major).
 * \param[in] jobvl Specifies whether to compute left eigenvectors (`EigenVectors::compute` or `EigenVectors::none`).
 * \param[in] jobvr Specifies whether to compute right eigenvectors (`EigenVectors::compute` or `EigenVectors::none`).
 * \param[in] n The order of the matrix (number of rows and columns).
 * \param[in,out] a Pointer to the matrix data. The content is overwritten.
 * \param[in] lda Leading dimension of the matrix (must be \(\geq n\)).
 * \param[out] wr Pointer to the array where the real parts of the computed eigenvalues will be stored (size `n`).
 * \param[out] wi Pointer to the array where the imaginary parts of the computed eigenvalues will be stored (size `n`).
 * \param[out] vl Pointer to the array where the left eigenvectors will be stored (if computed).
 * \param[in] ldvl Leading dimension of `vl` (must be \(\geq 1\) if computed, otherwise ignored).
 * \param[out] vr Pointer to the array where the right eigenvectors will be stored (if computed).
 * \param[in] ldvr Leading dimension of `vr` (must be \(\geq 1\) if computed, otherwise ignored).
 * \throws tl::Exception if an argument is invalid or if the algorithm fails to compute all eigenvalues.
 */
template<typename T>
void geev(Order order, EigenVectors jobvl, EigenVectors jobvr, size_t n, T *a,
          size_t lda, T *wr, T *wi, T *vl, size_t ldvl, T *vr, size_t ldvr)
{
    lapack_int info = LapackTraits<T>::geev(static_cast<int>(order),
                                            static_cast<char>(jobvl),
                                            static_cast<char>(jobvr),
                                            static_cast<lapack_int>(n),
                                            a, static_cast<lapack_int>(lda),
                                            wr, wi, vl,
                                            static_cast<lapack_int>(ldvl),
                                            vr, static_cast<lapack_int>(ldvr));
    if (info < 0) {
        TL_THROW_EXCEPTION("geev: Argument {} has an illegal value.", -info);
    }

    if (info > 0) {
        TL_THROW_EXCEPTION("geev: The QR algorithm failed to compute all eigenvalues. "
                           "Only elements 1 to {} in WR and WI are valid.", info);
    }
}

} // End namespace lapack

/*! \} */

} // End namespace tl

/// \cond
#endif // TL_HAVE_OPENBLAS
/// \endcond
