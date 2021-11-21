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
 * License along with Foobar. If not, see <http://www.gnu.org/licenses/>. *
 *                                                                        *
 * @license LGPL-3.0 <https://www.gnu.org/licenses/lgpl-3.0.html>         *
 *                                                                        *
 **************************************************************************/

#ifndef TL_MATH_CHOLESKY_DECOMPOSITION_H
#define TL_MATH_CHOLESKY_DECOMPOSITION_H

#include <algorithm>

#ifdef HAVE_OPENBLAS
#include <lapacke.h>
#endif // HAVE_OPENBLAS

#include "tidop/math/math.h"
#include "tidop/core/messages.h"
#include "tidop/math/algebra/vector.h"
#include "tidop/math/algebra/matrix.h"

namespace tl
{

namespace math
{


/*! \addtogroup Math
 *  \{
 */


/*! \addtogroup Algebra
 *  \{
 */



#ifdef HAVE_OPENBLAS

template<typename T> inline
typename std::enable_if<
  std::is_same<float, typename std::remove_cv<T>::type>::value, int>::type
  lapackePOTRF(lapack_int rows, T *a, lapack_int lda)
{
  lapack_int info = LAPACKE_spotrf(LAPACK_ROW_MAJOR, 'L', rows, a, lda);
  return info;
}

template<typename T> inline
typename std::enable_if<
  std::is_same<double, typename std::remove_cv<T>::type>::value, int>::type
  lapackePOTRF(lapack_int rows, T *a, lapack_int lda)
{
  lapack_int info = LAPACKE_dpotrf(LAPACK_ROW_MAJOR, 'L', rows, a, lda);
  return info;
}

#endif // HAVE_OPENBLAS

/*!
 * \brief Factorización Cholesky
 * 
 */
template<typename T>
class CholeskyDecomposition;

template<
  template<typename, size_t, size_t> 
  class Matrix_t, typename T, size_t _rows, size_t _cols
>
class CholeskyDecomposition<Matrix_t<T, _rows, _cols>>
{

public:

  CholeskyDecomposition(const Matrix_t<T, _rows, _cols> &a);

  Vector<T, _rows> solve(const Vector<T, _rows> &b);
  Matrix<T, _rows, _cols> l() const;

private:

  void decompose();
#ifdef HAVE_OPENBLAS
  void lapackeDecompose();
#endif // HAVE_OPENBLAS

protected:

  Matrix<T, _rows, _cols> L;
  size_t mRows;
};


template<
  template<typename, size_t, size_t> 
  class Matrix_t, typename T, size_t _rows, size_t _cols
>
CholeskyDecomposition<Matrix_t<T, _rows, _cols>>::CholeskyDecomposition(const Matrix_t<T, _rows, _cols> &a)
  : L(a),
    mRows(a.rows())
{
  static_assert(std::is_floating_point<T>::value, "Integral type not supported");

#ifdef HAVE_OPENBLAS
  this->lapackeDecompose();
#else
  this->decompose();
#endif // HAVE_OPENBLAS

  for (size_t i = 0; i < mRows; i++) {
    for (size_t j = 0; j < i; j++) {
      L[j][i] = consts::zero<T>;
    }
  }
}

template<
  template<typename, size_t, size_t> 
  class Matrix_t, typename T, size_t _rows, size_t _cols
>
inline Vector<T, _rows> CholeskyDecomposition<Matrix_t<T, _rows, _cols>>::solve(const Vector<T, _rows> &b)
{
  /// Resolver con matrices...
  TL_ASSERT(b.size() == mRows, "bad lengths in Cholesky");

  T sum;
  Vector<T, _rows> x(b);

  for (size_t r = 0; r < mRows; r++) {

    sum = b[r];

    for (size_t k = r; k > 0; k--) {
      sum -= L[r][k-1] * x[k-1];
    }

    x[r] = sum / L[r][r];

  }

  for (size_t i = mRows; i > 0; i--) {

    sum = x[i-1];

    for (size_t k = i; k < mRows; k++) {
      sum -= L[k][i-1] * x[k];
    }

    x[i-1] = sum / L[i-1][i-1];

  }

  return x;
}

template<
  template<typename, size_t, size_t> 
  class Matrix_t, typename T, size_t _rows, size_t _cols
>
inline void CholeskyDecomposition<Matrix_t<T, _rows, _cols>>::decompose()
{

  for (size_t i = 0; i < mRows; i++) {

    T lii = L[i][i];

    for (size_t j = i; j < mRows; j++) {

      T sum = L[i][j];

      for (size_t k = i; k > 0; k--) {
        sum -= L[i][k-1] * L[j][k-1];
      }

      if (i == j) {
        TL_ASSERT(sum > 0.0, "Cholesky failed");
        L[i][i] = sqrt(sum);
      } else {
        L[j][i] = sum / lii;
      }

    }
  }
}

#ifdef HAVE_OPENBLAS

template<
  template<typename, size_t, size_t>
class Matrix_t, typename T, size_t _rows, size_t _cols
>
inline void CholeskyDecomposition<Matrix_t<T, _rows, _cols>>::lapackeDecompose()
{
  lapack_int info;

  info = lapackePOTRF(L.rows(), L.data(), L.cols());

  TL_ASSERT(info >= 0, "Cholesky Decomposition failed");

}

#endif // HAVE_OPENBLAS

template<
  template<typename, size_t, size_t> 
  class Matrix_t, typename T, size_t _rows, size_t _cols
>
inline Matrix<T, _rows, _cols> CholeskyDecomposition<Matrix_t<T, _rows, _cols>>::l() const
{
  return L;
}

/*! \} */ // end of Algebra

/*! \} */ // end of Math

} // Fin namespace math


} // End namespace tl


#endif // TL_MATH_CHOLESKY_DECOMPOSITION_H
