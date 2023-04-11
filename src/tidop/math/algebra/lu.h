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

#ifndef TL_MATH_LU_DECOMPOSITION_H
#define TL_MATH_LU_DECOMPOSITION_H

#include <algorithm>

#include "tidop/math/math.h"
#include "tidop/core/messages.h"
//#include "tidop/math/algebra/matrix.h"
#include "tidop/math/algebra/vector.h"
#include "tidop/math/lapack.h"


namespace tl
{

namespace math
{

/*! \addtogroup math
 *  \{
 */

/*! \addtogroup algebra
 *  \{
 */


/*!
 * \brief Factorización o descomposición LU
 *
 * Sea A una matriz no singular (si lo fuera, entonces la descomposición podría no ser única)
 *
 * \f[ A=LU \f]
 *
 * donde L y U son matrices inferiores y superiores triangulares respectivamente.
 *
 */
template<typename T>
class LuDecomposition;


template<
  template<typename, size_t, size_t> 
  class Matrix_t, typename T, size_t _rows, size_t _cols
>
class LuDecomposition<Matrix_t<T, _rows, _cols>>
{

public:

  LuDecomposition(const Matrix_t<T, _rows, _cols> &a);
  ~LuDecomposition();

  Vector<T, _rows> solve(const Vector<T, _rows> &b);
  Matrix_t<T, _rows, _cols> solve(const Matrix_t<T, _rows, _cols> &b);
  Matrix_t<T, _rows, _cols> lu() const;

  T determinant() const;
  //Matrix_t<T, _rows, _cols> inverse();

private:

  void decompose();
  tl::math::Vector<T, _rows> findMaxElementsByRows();
#ifdef TL_HAVE_OPENBLAS
  void lapackeDecompose();
#endif // TL_HAVE_OPENBLAS

private:

  Matrix_t<T, _rows, _cols> LU;
#ifndef TL_HAVE_OPENBLAS 
  Vector<size_t, _rows> mPivotIndex;
#else
  lapack_int *mPivotIndex;
#endif
  T d;
  size_t mRows;
};


template<
  template<typename, size_t, size_t> 
  class Matrix_t, typename T, size_t _rows, size_t _cols
>
LuDecomposition<Matrix_t<T, _rows, _cols>>::LuDecomposition(const Matrix_t<T, _rows, _cols> &a)
  : LU(a),
#ifndef TL_HAVE_OPENBLAS 
    mPivotIndex(a.rows()),
#else
    mPivotIndex(new lapack_int[a.rows()]),
#endif
    mRows(a.rows())
{
  static_assert(std::is_floating_point<T>::value, "Integral type not supported");

#ifdef TL_HAVE_OPENBLAS
  this->lapackeDecompose();
#else
  this->decompose();
#endif // TL_HAVE_OPENBLAS

}

template<
  template<typename, size_t, size_t> 
  class Matrix_t, typename T, size_t _rows, size_t _cols
>
LuDecomposition<Matrix_t<T, _rows, _cols>>::~LuDecomposition()
{
#ifdef TL_HAVE_OPENBLAS 
  if (mPivotIndex){
    delete[] mPivotIndex;
  }
#endif
}

template<
  template<typename, size_t, size_t>
  class Matrix_t, typename T, size_t _rows, size_t _cols
>
Vector<T, _rows> LuDecomposition<Matrix_t<T, _rows, _cols>>::solve(const Vector<T, _rows> &b)
{
  TL_ASSERT(b.size() == mRows, "LuDecomposition::solve bad sizes");

  Vector<T, _rows> x(b);

#ifdef TL_HAVE_OPENBLAS 

  lapack_int nrhs = 1;
  lapack_int lda = mRows;
  lapack_int ldb = 1;

  lapack_int info = lapack::getrs(mRows, nrhs, LU.data(), lda, mPivotIndex, x.data(), ldb);

#else

  T sum;
  size_t ii = 0;
  size_t j;
  size_t ip;
  for (size_t i = 0; i < mRows; i++) {

    ip = mPivotIndex[i];
    sum = x[ip];
    x[ip] = x[i];

    if (ii != 0) {
      for (j = ii - 1; j < i; j++) {
        sum -= LU[i][j] * x[j]; 
      }
    } else if (sum != math::consts::zero<T>) {
      ii = i + 1;
    }
      
    x[i] = sum;
  }

  for (int i = static_cast<int>(mRows) - consts::one<int>; i >= 0; i--) {

    sum = x[i];
    T luii = LU[i][i];
    
    for (j = static_cast<size_t>(i) + consts::one<size_t>; j < mRows; j++)
      sum -= LU[i][j] * x[j];

    x[i] = sum / luii;
  }

#endif

  return x;
}

template<
  template<typename, size_t, size_t>
  class Matrix_t, typename T, size_t _rows, size_t _cols
>
Matrix_t<T, _rows, _cols> LuDecomposition<Matrix_t<T, _rows, _cols>>::solve(const Matrix_t<T, _rows, _cols> &b) ///Por ahora solo funciona con matrizes dinamicas
{
  TL_ASSERT(b.rows() == mRows, "LuDecomposition::solve bad sizes");
  
  Matrix_t<T, _rows, _cols> x(b);

#ifdef TL_HAVE_OPENBLAS    

  lapack_int info;
  lapack_int nrhs = b.cols();
  lapack_int lda = mRows;
  lapack_int ldb = b.cols();

  info = lapack::getrs(mRows, nrhs, LU.data(), lda, mPivotIndex, x.data(), ldb);

#else

  Vector<T, _rows> xx(mRows);

  for (size_t j = 0; j < x.cols(); j++) {

    for (size_t i = 0; i < mRows; i++) {
      xx[i] = b[i][j];
    }

    xx = this->solve(xx);

    for (size_t i = 0; i < mRows; i++) { 
      x[i][j] = xx[i]; 
    }
  }

#endif

  return x;
}

template<
  template<typename, size_t, size_t>
  class Matrix_t, typename T, size_t _rows, size_t _cols
>
void LuDecomposition<Matrix_t<T, _rows, _cols>>::decompose()
{

  T big;
  T temp;

  this->d = consts::one<T>;

  Vector<T, _rows> max_elements = findMaxElementsByRows();

  size_t pivot_row = 0;

  for (size_t k = 0; k < mRows; k++) {

    big = consts::zero<T>;
    pivot_row = k;

    for (size_t i = k; i < mRows; i++) {
      temp = std::abs(LU[i][k]) / max_elements[i];
      if (temp > big) {
        big = temp;
        pivot_row = i;
      }
    }

    if (k != pivot_row) {
      LU.swapRows(pivot_row, k);
      this->d = -this->d;
      max_elements[pivot_row] = max_elements[k];
    }

    mPivotIndex[k] = pivot_row;

    //if (LU[k][k] == consts::zero<T>)
    if (isNearlyZero(LU[k][k]))
      LU[k][k] = std::numeric_limits<T>().min();
    
    T llkk = LU[k][k];

    for (size_t i = k + 1; i < mRows; i++) {
    
      temp = LU[i][k] /= llkk;
      
      for (size_t j = k + 1; j < mRows; j++)
        LU[i][j] -= temp * LU[k][j];
    }
  }
}

template<
  template<typename, size_t, size_t>
  class Matrix_t, typename T, size_t _rows, size_t _cols
>
tl::math::Vector<T, _rows> LuDecomposition<Matrix_t<T, _rows, _cols>>::findMaxElementsByRows()
{
  Vector<T, _rows> max_elements(mRows, 0);
  T element;
  T max;

  for (size_t r = 0; r < mRows; r++) {

    max = consts::zero<T>;

    for (size_t c = 0; c < mRows; c++) {
      if ((element = std::abs(LU[r][c])) > max) {
        max = element;
      }
    }

    TL_ASSERT(max != consts::zero<T>, "Singular matrix");

    max_elements[r] = max;
  }

  return max_elements;
}

#ifdef TL_HAVE_OPENBLAS

template<
  template<typename, size_t, size_t> 
  class Matrix_t, typename T, size_t _rows, size_t _cols
>
inline void LuDecomposition<Matrix_t<T, _rows, _cols>>::lapackeDecompose()
{
  lapack_int info;
  lapack_int lda = mRows;
  
  info = lapack::getrf(mRows, mRows, LU.data(), lda, mPivotIndex);

  TL_ASSERT(info == 0, "The algorithm computing LU failed to converge.");

}

#endif // TL_HAVE_OPENBLAS

template<
  template<typename, size_t, size_t> 
  class Matrix_t, typename T, size_t _rows, size_t _cols
> 
inline Matrix_t<T, _rows, _cols> LuDecomposition<Matrix_t<T, _rows, _cols>>::lu() const
{
  return LU;
}

template<
  template<typename, size_t, size_t> 
  class Matrix_t, typename T, size_t _rows, size_t _cols
> 
T LuDecomposition<Matrix_t<T, _rows, _cols>>::determinant() const
{
  T det = this->d;

	for (size_t i = 0; i < mRows; i++) 
    det *= LU[i][i];

	return det;
}

//template<
//  template<typename, size_t, size_t> 
//  class Matrix_t, typename T, size_t _rows, size_t _cols
//> 
//Matrix_t<T, _rows, _cols> LuDecomposition<Matrix_t<T, _rows, _cols>>::inverse()
//{
//  Matrix_t<T, _rows, _cols> matrix = Matrix_t<T, _rows, _cols>::identity(mRows, mRows);
//
//  Matrix_t<T, _rows, _cols> inv = this->solve(matrix);
//  return inv;
//}


/*! \} */ // end of algebra

/*! \} */ // end of math

} // Fin namespace math


} // End namespace tl


#endif // TL_MATH_LU_DECOMPOSITION_H
