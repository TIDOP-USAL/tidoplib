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

#include "tidop/math/base/blas/Types.h"

namespace tl
{

/*! \addtogroup MathBase
 *  \{
 */

namespace blas
{

/*!
 * \brief Traits class to map floating point types to BLAS functions.
 *
 * This structure provides static mappings to the appropriate BLAS functions for
 * single and double precision floating point types.
 *
 * \tparam T The floating point type (float or double).
 */
template <typename T>
struct BlasTraits;

/*!
 * \brief Specialization of BlasTraits for float type.
 */
template <>
struct BlasTraits<float>
{
    static constexpr auto gemm = cblas_sgemm; /**< Single precision GEMM function. */
    static constexpr auto gemv = cblas_sgemv; /**< Single precision GEMV function. */
    static constexpr auto symm = cblas_ssymm; /**< Single precision SYMM function. */
    static constexpr auto trmm = cblas_strmm; /**< Single precision TRMM function. */
};

/*!
 * \brief Specialization of BlasTraits for double type.
 */
template <>
struct BlasTraits<double>
{
    static constexpr auto gemm = cblas_dgemm; /**< Double precision GEMM function. */
    static constexpr auto gemv = cblas_dgemv; /**< Double precision GEMV function. */
    static constexpr auto symm = cblas_dsymm; /**< Double precision SYMM function. */
    static constexpr auto trmm = cblas_dtrmm; /**< Double precision TRMM function. */
};

} // End namespace blas

/*! \} */

} // End namespace tl

/// \cond
#endif // TL_HAVE_OPENBLAS
/// \endcond
