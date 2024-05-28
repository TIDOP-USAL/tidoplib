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


#include "tidop/math/math.h"

#ifdef TL_HAVE_OPENBLAS
#include <cblas.h>

namespace tl
{


/*! \addtogroup Math
 *  \{
 */

namespace blas
{

/* Multiplicación matrices */

template<typename T>
auto gemm(size_t m, size_t n, size_t k,
          const T* a, const T* b, T* c) -> enableIfFloat<T, void>
{
    T alpha = 1.f;
    T beta = 0.f;
    blasint lda = static_cast<blasint>(k);
    blasint ldb = static_cast<blasint>(n);
    blasint ldc = static_cast<blasint>(n);

    cblas_sgemm(CBLAS_ORDER::CblasRowMajor,
                CBLAS_TRANSPOSE::CblasNoTrans,
                CBLAS_TRANSPOSE::CblasNoTrans,
                static_cast<blasint>(m),
                static_cast<blasint>(n),
                static_cast<blasint>(k),
                alpha, a, lda, b, ldb,
                beta, c, ldc);
}

template<typename T>
auto gemm(size_t m, size_t n, size_t k,
          const T* a, const T* b, T* c) -> enableIfDouble<T, void>
{
    T alpha = 1.;
    T beta = 0.;
    blasint lda = static_cast<blasint>(k);
    blasint ldb = static_cast<blasint>(n);
    blasint ldc = static_cast<blasint>(n);

    cblas_dgemm(CBLAS_ORDER::CblasRowMajor,
                CBLAS_TRANSPOSE::CblasNoTrans,
                CBLAS_TRANSPOSE::CblasNoTrans,
                static_cast<blasint>(m),
                static_cast<blasint>(n),
                static_cast<blasint>(k),
                alpha, a, lda, b, ldb,
                beta, c, ldc);
}

} // End namespace blas

/*! \} */ // end of Math

} // End namespace tl

#endif // TL_HAVE_OPENBLAS

