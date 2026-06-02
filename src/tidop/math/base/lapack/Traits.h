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

#include "tidop/math/base/lapack/Types.h"

namespace tl
{

/*! \addtogroup MathBase
 *  \{
 */

namespace lapack
{

/*!
 * \brief LAPACK function mappings for different floating-point types.
 *
 * Defines a set of LAPACK functions specialized for `float` and `double` types.
 * This allows type-safe LAPACK function calls.
 *
 * \tparam T Floating-point type (`float` or `double`).
 */
template <typename T>
struct LapackTraits;

/*!
 * \brief Specialization of LapackTraits for `float` type.
 *
 * Provides LAPACK function mappings for single-precision floating point operations.
 */
template <>
struct LapackTraits<float>
{
    static constexpr auto getrf = LAPACKE_sgetrf; /*!< LU decomposition. */
    static constexpr auto getrs = LAPACKE_sgetrs; /*!< Solves a system using LU decomposition. */
    static constexpr auto potrf = LAPACKE_spotrf; /*!< Cholesky decomposition. */
    static constexpr auto potrs = LAPACKE_spotrs; /*!< Solves a system using Cholesky decomposition. */
    static constexpr auto potri = LAPACKE_spotri; /*!< Inversion using Cholesky factor. */
    static constexpr auto geqrf = LAPACKE_sgeqrf; /*!< QR decomposition. */
    static constexpr auto orgqr = LAPACKE_sorgqr; /*!< Generates an orthogonal matrix from QR decomposition. */
    static constexpr auto gels = LAPACKE_sgels;   /*!< Solves linear least squares problems. */
    static constexpr auto gesvd = LAPACKE_sgesvd; /*!< Singular value decomposition (SVD). */
    static constexpr auto syev = LAPACKE_ssyev;   /*!< Computes eigenvalues and eigenvectors of a symmetric matrix. */
    static constexpr auto geev = LAPACKE_sgeev;   /*!< Computes eigenvalues and eigenvectors of a general matrix. */
    static constexpr auto trtrs = LAPACKE_strtrs;
};

/*!
 * \brief Specialization of LapackTraits for `double` type.
 *
 * Provides LAPACK function mappings for double-precision floating point operations.
 */
template <>
struct LapackTraits<double>
{
    static constexpr auto getrf = LAPACKE_dgetrf; /*!< LU decomposition. */
    static constexpr auto getrs = LAPACKE_dgetrs; /*!< Solves a system using LU decomposition. */
    static constexpr auto potrf = LAPACKE_dpotrf; /*!< Cholesky decomposition. */
    static constexpr auto potrs = LAPACKE_dpotrs; /*!< Solves a system using Cholesky decomposition. */
    static constexpr auto potri = LAPACKE_dpotri; /*!< Inversion using Cholesky factor. */
    static constexpr auto geqrf = LAPACKE_dgeqrf; /*!< QR decomposition. */
    static constexpr auto orgqr = LAPACKE_dorgqr; /*!< Generates an orthogonal matrix from QR decomposition. */
    static constexpr auto gels = LAPACKE_dgels;   /*!< Solves linear least squares problems. */
    static constexpr auto gesvd = LAPACKE_dgesvd; /*!< Singular value decomposition (SVD). */
    static constexpr auto syev = LAPACKE_dsyev;   /*!< Computes eigenvalues and eigenvectors of a symmetric matrix. */
    static constexpr auto geev = LAPACKE_dgeev;   /*!< Computes eigenvalues and eigenvectors of a general matrix. */
    static constexpr auto trtrs = LAPACKE_dtrtrs;
};

} // End namespace lapack

/*! \} */

} // End namespace tl

/// \cond
#endif // TL_HAVE_OPENBLAS
/// \endcond
