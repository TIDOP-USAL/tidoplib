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

#ifndef TL_MATH_UTILS_BLAS_H
#define TL_MATH_UTILS_BLAS_H

#include "config_tl.h"

#include "tidop/core/defs.h"
#include "tidop/math/math.h"

#ifdef TL_HAVE_OPENBLAS
#include <cblas.h>

namespace tl
{

namespace math
{


/*! \addtogroup Math
 *  \{
 */

namespace blas
{

/* Multiplicación matrices */

/* void cblas_sgemm(OPENBLAS_CONST enum CBLAS_ORDER Order, OPENBLAS_CONST enum CBLAS_TRANSPOSE TransA, OPENBLAS_CONST enum CBLAS_TRANSPOSE TransB, OPENBLAS_CONST blasint M, OPENBLAS_CONST blasint N, OPENBLAS_CONST blasint K,
		 OPENBLAS_CONST float alpha, OPENBLAS_CONST float *A, OPENBLAS_CONST blasint lda, OPENBLAS_CONST float *B, OPENBLAS_CONST blasint ldb, OPENBLAS_CONST float beta, float *C, OPENBLAS_CONST blasint ldc);
		 
void cblas_dgemm(OPENBLAS_CONST enum CBLAS_ORDER Order, OPENBLAS_CONST enum CBLAS_TRANSPOSE TransA, OPENBLAS_CONST enum CBLAS_TRANSPOSE TransB, OPENBLAS_CONST blasint M, OPENBLAS_CONST blasint N, OPENBLAS_CONST blasint K,
		 OPENBLAS_CONST double alpha, OPENBLAS_CONST double *A, OPENBLAS_CONST blasint lda, OPENBLAS_CONST double *B, OPENBLAS_CONST blasint ldb, OPENBLAS_CONST double beta, double *C, OPENBLAS_CONST blasint ldc); */

template<typename T> inline
typename std::enable_if<
  std::is_same<float, typename std::remove_cv<T>::type>::value, void>::type
gemm(int m, int n, int k,
     const T *a, const T *b, T *c)
{
  T alpha = 1.f;
  T beta = 0.f;
  blasint lda = k;
  blasint ldb = n;
  blasint ldc = n;
  cblas_sgemm(CBLAS_ORDER::CblasRowMajor,
              CBLAS_TRANSPOSE::CblasNoTrans,
              CBLAS_TRANSPOSE::CblasNoTrans,
              m, n, k, alpha, a, lda, b, ldb,
              beta, c, ldc);
}

template<typename T> inline
typename std::enable_if<
  std::is_same<double, typename std::remove_cv<T>::type>::value, void>::type
gemm(int m, int n, int k,
     const T *a, const T *b, T *c)
{
  T alpha = 1.;
  T beta = 0.;
  blasint lda = k;
  blasint ldb = n;
  blasint ldc = n;

  cblas_dgemm(CBLAS_ORDER::CblasRowMajor,
              CBLAS_TRANSPOSE::CblasNoTrans, 
              CBLAS_TRANSPOSE::CblasNoTrans, 
              m, n, k, alpha, a, lda, b, ldb,
              beta, c, ldc);
}

} // End namespace blas

/*! \} */ // end of Math

} // End namespace math

} // End namespace tl

#endif // TL_HAVE_OPENBLAS

#endif // TL_MATH_UTILS_BLAS_H

