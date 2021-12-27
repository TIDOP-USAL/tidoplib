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

#ifndef TL_MATH_SVD_H
#define TL_MATH_SVD_H

#include "config_tl.h"

#include <algorithm>

#ifdef HAVE_OPENBLAS
#include <lapacke.h>
#endif // HAVE_OPENBLAS

#include "tidop/core/defs.h"
#include "tidop/core/messages.h"
#include "tidop/math/algebra/matrix.h"
#include "tidop/math/algebra/vector.h"

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

#ifdef HAVE_OPENBLAS

template<typename T> inline
typename std::enable_if<
    std::is_same<float, typename std::remove_cv<T>::type>::value, int>::type
lapackeGESVD(lapack_int rows, lapack_int cols, T *a, lapack_int lda, T *s, T *u, lapack_int ldu, T *v, lapack_int ldvt, T *superb)
{
  lapack_int info = LAPACKE_sgesvd(LAPACK_ROW_MAJOR, 'A', 'A', rows, cols, a, lda, s, u, ldu, v, ldvt, superb);
  return info;
}

template<typename T> inline
typename std::enable_if<
    std::is_same<double, typename std::remove_cv<T>::type>::value, int>::type
lapackeGESVD(lapack_int rows, lapack_int cols, T *a, lapack_int lda, T *s, T *u, lapack_int ldu, T *v, lapack_int ldvt, T *superb)
{
  lapack_int info = LAPACKE_dgesvd(LAPACK_ROW_MAJOR, 'A', 'A', rows, cols, a, lda, s, u, ldu, v, ldvt, superb);
  return info;
}

#endif // HAVE_OPENBLAS

/*!
 * \brief SVD (Singular value decomposition)
 *
 * En álgebra lineal, la descomposición en valores singulares de una matriz A
 * es una factorización de la misma con muchas aplicaciones, entre ellas
 * la resolución de sistemas lineales.
 *
 * Dada una matriz A de m×n, existen matrices ortogonales U (de orden m) y V (de orden n)
 * y una matriz diagonal Σ (de tamaño m×n) tales que:
 *
 * \f[ A = UΣV^T  \f]
 *
 * Esta factorización de A se llama descomposición en valores singulares de A.
 *
 *
 * http://www.ehu.eus/izaballa/Cursos/valores_singulares.pdf
 * https://www.researchgate.net/publication/263583897_La_descomposicion_en_valores_singulares_SVD_y_algunas_de_sus_aplicaciones
 */
template<typename T>
class SingularValueDecomposition;

template<
  template<typename, size_t, size_t> 
  class Matrix_t, typename T, size_t _rows, size_t _cols
>
class SingularValueDecomposition<Matrix_t<T, _rows, _cols>>
{

public:

  SingularValueDecomposition(const Matrix_t<T, _rows, _cols> &a);

  Vector<T, _cols> solve(const Vector<T, _rows> &b);

  Matrix<T, _rows, _cols> u() const;
  Matrix<T, _cols, _cols> v() const;
  Vector<T, _cols> w() const;

  int maxIterations() const;
  void setMaxIterations(int maxIterations);

private:

  void decompose();
  void reorder();
#ifdef HAVE_OPENBLAS
  void lapackeDecompose();
#endif // HAVE_OPENBLAS

private:

  Matrix<T, _rows, _cols> A;
  Matrix<T, _rows, _cols> U;
  Matrix<T, _cols, _cols> V;
  Vector<T, _cols> W;
  int mIterationMax;
  T eps;
  T tsh;
  size_t mRows;
  size_t mCols;
};


template<
  template<typename, size_t, size_t> 
  class Matrix_t, typename T, size_t _rows, size_t _cols
>
SingularValueDecomposition<Matrix_t<T, _rows, _cols>>::SingularValueDecomposition(const Matrix_t<T, _rows, _cols> &a)
  : A(a),
    mIterationMax(30),
    mRows(a.rows()),
    mCols(a.cols())
{
  static_assert(std::is_floating_point<T>::value, "Integral type not supported");

  U = Matrix<T, _rows, _cols>(mRows, mCols);
  V = Matrix<T, _cols, _cols>(mCols, mCols);
  W = Vector<T, _cols>(mCols);

#ifdef HAVE_OPENBLAS
  this->lapackeDecompose();
#else
  eps = std::numeric_limits<T>::epsilon();
  this->decompose();
  this->reorder();
  tsh = consts::half<T> * sqrt(mRows + mCols + consts::one<T>) * W[0] * eps;
#endif // HAVE_OPENBLAS

}


template<
  template<typename, size_t, size_t>
  class Matrix_t, typename T, size_t _rows, size_t _cols
>
Vector<T, _cols> SingularValueDecomposition<Matrix_t<T, _rows, _cols>>::solve(const Vector<T, _rows> &B)
{
  Vector<T, _cols> C(mCols);

  T s;
  Vector<T, _cols> tmp(mCols);

  for (size_t j = 0; j < mCols; j++) {
    s = consts::zero<T>;
    if (W[j] > tsh) {
      for (size_t i = 0; i < mRows; i++)
        s += U[i][j] * B[i];
      s /= W[j];
    }
    tmp[j] = s;
  }

  for (size_t j = 0; j < mCols; j++) {
    s = consts::zero<T>;
    for (size_t k = 0; k < mCols; k++)
      s += V[j][k] * tmp[k];
    C[j] = s;
  }

  return C;
}

template<
  template<typename, size_t, size_t> 
  class Matrix_t, typename T, size_t _rows, size_t _cols
>
inline void SingularValueDecomposition<Matrix_t<T, _rows, _cols>>::decompose()
{
  this->U = this->A;
  Vector<T, _cols> rv1(mCols);
  int i;
  int j;
  int k;
  int l;
  T anorm;
  T f;
  T g;
  T h;
  T s;
  T scale;
  g = scale = anorm = consts::zero<T>; //Householder reduction to bidiagonal form.

  //int one = 1;

  for (i = 0; i < mCols; i++) {
    l = i + 2;
    rv1[i] = scale * g;
    g = s = scale = 0.0;

    if (i < mRows) {
      for (k = i; k < mRows; k++) scale += std::abs(U[k][i]);
      if (scale != 0.) {
        for (k = i; k < mRows; k++) {
          U[k][i] /= scale;
          s += U[k][i] * U[k][i];
        }
        f = U[i][i];
        g = -std::copysign(sqrt(s), f);
        h = f * g - s;
        U[i][i] = f - g;
        for (j = l - consts::one<int>; j < mCols; j++) {
          for (s = 0.0, k = i; k < mRows; k++)
            s += U[k][i] * U[k][j];
          f = s / h;
          for (k = i; k < mRows; k++)
            U[k][j] += f * U[k][i];
        }
        for (k = i; k < mRows; k++)
          U[k][i] *= scale;
      }
    }

    W[i] = scale * g;
    g = s = scale = 0.0;
    if (i + consts::one<int> <= mRows && i + consts::one<int> != mCols) {
      for (k = l - consts::one<int>; k < mCols; k++) scale += fabs(U[i][k]);
      if (scale != 0.) {
        for (k = l - consts::one<int>; k < mCols; k++) {
          U[i][k] /= scale;
          s += U[i][k] * U[i][k];
        }
        f = U[i][l - consts::one<int>];
        g = -std::copysign(sqrt(s), f);
        h = f * g - s;
        U[i][l - consts::one<int>] = f - g;

        for (k = l - consts::one<int>; k < mCols; k++)
          rv1[k] = U[i][k] / h;

        for (j = l - consts::one<int>; j < mRows; j++) {
          for (s = 0.0, k = l - consts::one<int>; k < mCols; k++)
            s += U[j][k] * U[i][k];
          for (k = l - consts::one<int>; k < mCols; k++)
            U[j][k] += s * rv1[k];
        }

        for (k = l - consts::one<int>; k < mCols; k++)
          U[i][k] *= scale;
      }
    }
    anorm = std::max(anorm, std::abs(W[i]) + std::abs(rv1[i]));
  }

  for (i = static_cast<int>(mCols) - consts::one<int>; i >= 0; i--) { //Accumulation of right-hand transformations.
    if (i < mCols - consts::one<int>) {
      if (g != 0.) {
        for (j = l; j < mCols; j++) //Double division to avoid possible underflow.
          V[j][i] = (U[i][j] / U[i][l]) / g;
        for (j = l; j < mCols; j++) {
          for (s = 0.0, k = l; k < mCols; k++)
            s += U[i][k] * V[k][j];
          for (k = l; k < mCols; k++)
            V[k][j] += s * V[k][i];
        }
      }

      for (j = l; j < mCols; j++)
        V[i][j] = V[j][i] = consts::zero<T>;
    }

    V[i][i] = consts::one<T>;
    g = rv1[i];
    l = i;
  }

  for (i = static_cast<int>(std::min(mRows, mCols)) - consts::one<int>; i >= 0; i--) { //Accumulation of left-hand transformations.
    l = i + consts::one<int>;
    g = W[i];
    for (j = l; j < mCols; j++) this->U.at(i, j) = 0.0;
    if (g != consts::zero<T>) {
      g = consts::one<T> / g;
      for (j = l; j < mCols; j++) {
        for (s = consts::zero<T>, k = l; k < mRows; k++)
          s += U[k][i] * U[k][j];
        f = (s / U[i][i]) * g;
        for (k = i; k < mRows; k++)
          U[k][j] += f * U[k][i];
      }
      for (j = i; j < mRows; j++)
        U[j][i] *= g;
    } else {
      for (j = i; j < mRows; j++)
        U[j][i] = consts::zero<T>;
    }
    ++U[i][i];
  }

  bool flag;
  int its;
  size_t nm;
  T c;
  T x;
  T y;
  T z;
  for (k = static_cast<int>(mCols) - consts::one<int>; k >= 0; k--) { //Diagonalization of the bidiagonal form: Loop over
    for (its = 0; its < mIterationMax; its++) { // singular values, and over allowed iterations.
    
      flag = true;
      
      for (l = k; l >= 0; l--) { //Test for splitting.
        nm = static_cast<size_t>(l) - consts::one<int>; //Note that rv1[1] is always zero.
        if (l == 0 || abs(rv1[l]) <= eps * anorm) {
          flag = false;
          break;
        }
        if (abs(W[nm]) <= eps * anorm) break;
      }

      if (flag) {
        c = consts::zero<T>; //Cancellation of rv1[l], if l > 1.
        s = consts::one<T>;
        for (i = l; i < k + consts::one<int>; i++) {
          f = s * rv1[i];
          rv1[i] = c * rv1[i];
          if (abs(f) <= eps * anorm) break;
          g = W[i];
          h = math::module(f, g);
          W[i] = h;
          h = consts::one<T> / h;
          c = g * h;
          s = -f * h;
          for (j = 0; j < mRows; j++) {
            y = U[j][nm];
            z = U[j][i];
            U[j][nm] = y * c + z * s;
            U[j][i] = z * c - y * s;
          }
        }
      }
      z = W[k];
      if (l == k) { //Convergence.
        if (z < consts::zero<T>) { //Singular value is made nonnegative.
          W[k] = -z;
          for (j = 0; j < mCols; j++) 
            V[j][k] = -V[j][k];
        }
        break;
      }

      if (its == 29) throw std::runtime_error("no convergence in 30 iterations");
      x = W[l]; //Shift from bottom 2-by-2 minor.
      nm = static_cast<size_t>(k - consts::one<int>);
      y = W[nm];
      g = rv1[nm];
      h = rv1[k];
      f = ((y - z) * (y + z) + (g - h) * (g + h)) / (static_cast<T>(2) * h * y);
      g = math::module(f, consts::one<T>);
      f = ((x - z) * (x + z) + h * ((y / (f + std::copysign(g, f))) - h)) / x;
      c = s = consts::one<T>; //Next QR transformation:

      for (j = l; j <= nm; j++) {
        i = j + consts::one<int>;
        g = rv1[i];
        y = W[i];
        h = s * g;
        g = c * g;
        z = math::module(f, h);
        rv1[j] = z;
        c = f / z;
        s = h / z;
        f = x * c + g * s;
        g = g * c - x * s;
        h = y * s;
        y *= c;
        for (size_t jj = 0; jj < mCols; jj++) {
          x = V[jj][j];
          z = V[jj][i];
          V[jj][j] = x * c + z * s;
          V[jj][i] = z * c - x * s;
        }
        z = math::module(f, h);
        W[j] = z; //Rotation can be arbitrary if z = 0.
        if (z != 0.) {
          z = consts::one<T> / z;
          c = f * z;
          s = h * z;
        }
        f = c * g + s * y;
        x = c * y - s * g;
        for (size_t jj = 0; jj < mRows; jj++) {
          y = U[jj][j];
          z = U[jj][i];
          U[jj][j] = y * c + z * s;
          U[jj][i] = z * c - y * s;
        }
      }

      rv1[l] = consts::zero<T>;
      rv1[k] = f;
      W[k] = x;
    }
  }
}

template<
  template<typename, size_t, size_t> 
  class Matrix_t, typename T, size_t _rows, size_t _cols
>
inline void SingularValueDecomposition<Matrix_t<T, _rows, _cols>>::reorder()
{
  int i;
  int j;
  int k;
  
  int inc = 1;
  T sw;
  Vector<T, _rows> su(mRows);
  Vector<T, _cols> sv(mCols);

  do { inc *= 3; inc++; } while (inc <= mCols);

  do {
    inc /= 3;
    for (i = inc; i < mCols; i++) {
      sw = W[i];
      for (k = 0; k < mRows; k++) su[k] = U.at(k, i);
      for (k = 0; k < mCols; k++) sv[k] = V.at(k, i);
      j = i;
      while (W[j - inc] < sw) {
        W[j] = W[j - inc];
        for (k = 0; k < mRows; k++) U.at(k, j) = U.at(k, j - inc);
        for (k = 0; k < mCols; k++) V.at(k, j) = V.at(k, j - inc);
        j -= inc;
        if (j < inc) break;
      }
      W[j] = sw;
      for (k = 0; k < mRows; k++) U.at(k, j) = su[k];
      for (k = 0; k < mCols; k++) V.at(k, j) = sv[k];

    }
  } while (inc > 1);

  size_t s;
  for (size_t c = 0; c < mCols; c++) {
    s = 0;

    for (size_t r = 0; r < mRows; r++)
      if (U[r][c] < consts::zero<T>)
        s++;

    for (size_t r = 0; r < mCols; r++)
      if (V[r][c] < consts::zero<T>)
        s++;

    if (s > (mRows + mCols) / 2) {
      U.col(c) = -U.col(c);
      V.col(c) = -V.col(c);
      //for (size_t r = 0; r < mRows; r++) 
      //  U[r][c] = -U[r][c];

      //for (size_t r = 0; r < mCols; r++)
      //  V[r][c] = -V[r][c];
    }
  }
}

#ifdef HAVE_OPENBLAS

template<
  template<typename, size_t, size_t> 
  class Matrix_t, typename T, size_t _rows, size_t _cols
>
inline void SingularValueDecomposition<Matrix_t<T, _rows, _cols>>::lapackeDecompose()
{
  lapack_int info;
  lapack_int lda = mCols;
  lapack_int ldu = mRows;
  lapack_int ldvt = mCols;
  T *superb = new T[std::min(mRows,mCols)-1];

  info = lapackeGESVD(mRows, mCols, A.data(), lda, W.data(), U.data(), ldu, V.data(), ldvt, superb);
  V = V.transpose();

  delete[] superb;

  TL_ASSERT(info >= 0, "The algorithm computing SVD failed to converge.");
}

#endif // HAVE_OPENBLAS

template<
  template<typename, size_t, size_t> 
  class Matrix_t, typename T, size_t _rows, size_t _cols
> 
inline Matrix<T, _rows, _cols> SingularValueDecomposition<Matrix_t<T, _rows, _cols>>::u() const
{
  return U;
}

template<
  template<typename, size_t, size_t> 
  class Matrix_t, typename T, size_t _rows, size_t _cols
> 
inline Matrix<T, _cols, _cols> SingularValueDecomposition<Matrix_t<T, _rows, _cols>>::v() const
{
  return V;
}

template<
  template<typename, size_t, size_t> 
  class Matrix_t, typename T, size_t _rows, size_t _cols
> 
inline Vector<T, _cols> SingularValueDecomposition<Matrix_t<T, _rows, _cols>>::w() const
{
  return W;
}

template<
  template<typename, size_t, size_t> 
  class Matrix_t, typename T, size_t _rows, size_t _cols
> 
inline int SingularValueDecomposition<Matrix_t<T, _rows, _cols>>::maxIterations() const
{
  return mIterationMax;
}

template<
  template<typename, size_t, size_t> 
  class Matrix_t, typename T, size_t _rows, size_t _cols
> 
inline void SingularValueDecomposition<Matrix_t<T, _rows, _cols>>::setMaxIterations(int maxIterations)
{
  mIterationMax = maxIterations;
}


/*! \} */ // end of algebra

/*! \} */ // end of math

} // Fin namespace math


} // End namespace tl


#endif // TL_MATH_SVD_H
