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


#ifdef TL_HAVE_OPENBLAS

#include "tidop/core/defs.h"
#include "tidop/math/math.h"

#include <lapacke.h>

namespace tl
{


/*! \addtogroup Math
 *  \{
 */

namespace lapack
{


/* Factorización LU */

template<typename T>
auto getrf(lapack_int rows, lapack_int cols, T* a, lapack_int lda, lapack_int* ipiv) -> std::enable_if_t<
    std::is_same<float, std::remove_cv_t<T>>::value, lapack_int>
{
    lapack_int info = LAPACKE_sgetrf(LAPACK_ROW_MAJOR, rows, cols, a, lda, ipiv);
    return info;
}

template<typename T>
auto getrf(lapack_int rows, lapack_int cols, T* a, lapack_int lda, lapack_int* ipiv) -> std::enable_if_t<
    std::is_same<double, std::remove_cv_t<T>>::value, lapack_int>
{
    lapack_int info = LAPACKE_dgetrf(LAPACK_ROW_MAJOR, rows, cols, a, lda, ipiv);
    return info;
}

template<typename T> 
auto getrs(lapack_int rows, lapack_int nrhs, T* a, lapack_int lda, 
           lapack_int* ipiv, T* b, lapack_int ldb) -> std::enable_if_t<
    std::is_same<float, typename std::remove_cv<T>::type>::value, lapack_int>
{
    lapack_int info = LAPACKE_sgetrs(LAPACK_ROW_MAJOR, 'N', rows, nrhs, a, lda, ipiv, b, ldb);
    return info;
}

template<typename T> 
auto getrs(lapack_int rows, lapack_int nrhs, T* a, lapack_int lda, 
           lapack_int* ipiv, T* b, lapack_int ldb) -> std::enable_if_t<
    std::is_same<double, std::remove_cv_t<T>>::value, lapack_int>
{
    lapack_int info = LAPACKE_dgetrs(LAPACK_ROW_MAJOR, 'N', rows, nrhs, a, lda, ipiv, b, ldb);
    return info;
}


/* Factorización Cholesky */

template<typename T>
auto potrf(lapack_int rows, T* a, lapack_int lda) -> std::enable_if_t<
    std::is_same<float, std::remove_cv_t<T>>::value, lapack_int>
{
    lapack_int info = LAPACKE_spotrf(LAPACK_ROW_MAJOR, 'L', rows, a, lda);
    return info;
}

template<typename T> 
auto potrf(lapack_int rows, T* a, lapack_int lda) -> std::enable_if_t<
    std::is_same<double, std::remove_cv_t<T>>::value, lapack_int>
{
    lapack_int info = LAPACKE_dpotrf(LAPACK_ROW_MAJOR, 'L', rows, a, lda);
    return info;
}


/* SVD (Singular value decomposition) */

template<typename T> 
auto gesvd(lapack_int rows, lapack_int cols, T* a, lapack_int lda, T* s, T* u,
           lapack_int ldu, T* v, lapack_int ldvt, T* superb) -> std::enable_if_t<
    std::is_same<float, std::remove_cv_t<T>>::value, lapack_int>
{
    lapack_int info = LAPACKE_sgesvd(LAPACK_ROW_MAJOR, 'A', 'A', rows, cols, a, lda, s, u, ldu, v, ldvt, superb);
    return info;
}

template<typename T> 
auto gesvd(lapack_int rows, lapack_int cols, T* a, lapack_int lda, T* s, T* u,
           lapack_int ldu, T* v, lapack_int ldvt, T* superb) -> std::enable_if_t<
    std::is_same<double, std::remove_cv_t<T>>::value, lapack_int>
{
    lapack_int info = LAPACKE_dgesvd(LAPACK_ROW_MAJOR, 'A', 'A', rows, cols, a, lda, s, u, ldu, v, ldvt, superb);
    return info;
}


} // End namespace lapack

/*! \} */ // end of Math

} // End namespace tl

#endif // TL_HAVE_OPENBLAS

