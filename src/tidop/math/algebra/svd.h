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
#include "tidop/core/defs.h"
#include "tidop/core/messages.h"
#include "tidop/math/mathutils.h"
#include "tidop/math/algebra/matrix.h"
#include "tidop/math/algebra/vector.h"

#ifdef HAVE_OPENBLAS
#include <lapacke.h>
#endif // HAVE_OPENBLAS

#include <algorithm>

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

  int iterationMax() const;
  void setIterationMax(int iterationMax);

private:

  void decompose();
  void reorder();
#ifdef HAVE_OPENBLAS
  void lapackeDecompose();
#endif // HAVE_OPENBLAS

protected:

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
  tsh = static_cast<T>(0.5) * sqrt(mRows + mCols + static_cast<T>(1)) * W[0] * eps;
#endif // HAVE_OPENBLAS

}


template<
  template<typename, size_t, size_t>
  class Matrix_t, typename T, size_t _rows, size_t _cols
>
Vector<T, _cols> SingularValueDecomposition<Matrix_t<T, _rows, _cols>>::solve(const Vector<T, _rows> &B)
{
  T s;
  Vector<T, _cols> C(mCols);
  Vector<T, _cols> tmp(mCols);

  for (size_t j = 0; j < mCols; j++) {
    s = 0.0;
    if (W[j] > tsh) {
      for (size_t i = 0; i < mRows; i++)
        s += U.at(i, j) * B[i];
      s /= W[j];
    }
    tmp[j] = s;
  }

  for (size_t j = 0; j < mCols; j++) {
    s = 0.0;
    for (size_t k = 0; k < mCols; k++)
      s += V.at(j, k) * tmp[k];
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
  int i, j, k, l;
  T anorm, f, g, h, s, scale;
  g = scale = anorm = 0.0; //Householder reduction to bidiagonal form.

  int one = 1;

  for (i = 0; i < mCols; i++) {
    l = i + 2;
    rv1[i] = scale * g;
    g = s = scale = 0.0;

    if (i < mRows) {
      for (k = i; k < mRows; k++) scale += fabs(this->U.at(k, i));
      if (scale != 0.) {
        for (k = i; k < mRows; k++) {
          this->U.at(k, i) /= scale;
          s += this->U.at(k, i) * this->U.at(k, i);
        }
        f = this->U.at(i, i);
        g = -std::copysign(sqrt(s), f);
        h = f * g - s;
        this->U.at(i, i) = f - g;
        for (j = l - one; j < mCols; j++) {
          for (s = 0.0, k = i; k < mRows; k++)
            s += this->U.at(k, i) * this->U.at(k, j);
          f = s / h;
          for (k = i; k < mRows; k++)
            this->U.at(k, j) += f * this->U.at(k, i);
        }
        for (k = i; k < mRows; k++)
          this->U.at(k, i) *= scale;
      }
    }

    W[i] = scale * g;
    g = s = scale = 0.0;
    if (i + one <= mRows && i + one != mCols) {
      for (k = l - one; k < mCols; k++) scale += fabs(this->U.at(i, k));
      if (scale != 0.) {
        for (k = l - one; k < mCols; k++) {
          this->U.at(i, k) /= scale;
          s += this->U.at(i, k) * this->U.at(i, k);
        }
        f = this->U.at(i, l - one);
        g = -std::copysign(sqrt(s), f);
        h = f * g - s;
        this->U.at(i, l - one) = f - g;

        for (k = l - one; k < mCols; k++)
          rv1[k] = this->U.at(i, k) / h;

        for (j = l - one; j < mRows; j++) {
          for (s = 0.0, k = l - one; k < mCols; k++)
            s += this->U.at(j, k) * this->U.at(i, k);
          for (k = l - one; k < mCols; k++)
            this->U.at(j, k) += s * rv1[k];
        }

        for (k = l - one; k < mCols; k++)
          this->U.at(i, k) *= scale;
      }
    }
    anorm = std::max(anorm, std::abs(W[i]) + std::abs(rv1[i]));
  }


  for (i = static_cast<int>(mCols) - one; i >= 0; i--) { //Accumulation of right-hand transformations.
    if (i < mCols - one) {
      if (g != 0.) {
        for (j = l; j < mCols; j++) //Double division to avoid possible underflow.
          V.at(j, i) = (this->U.at(i, j) / this->U.at(i, l)) / g;
        for (j = l; j < mCols; j++) {
          for (s = 0.0, k = l; k < mCols; k++)
            s += this->U.at(i, k) * V.at(k, j);
          for (k = l; k < mCols; k++)
            V.at(k, j) += s * V.at(k, i);
        }
      }

      for (j = l; j < mCols; j++)
        this->V.at(i, j) = this->V.at(j, i) = 0.0;
    }
    this->V.at(i, i) = 1.0;
    g = rv1[i];
    l = i;
  }

  for (i = static_cast<int>(std::min(mRows, mCols)) - one; i >= 0; i--) { //Accumulation of left-hand transformations.
    l = i + one;
    g = W[i];
    for (j = l; j < mCols; j++) this->U.at(i, j) = 0.0;
    if (g != 0.) {
      g = static_cast<T>(1.0) / g;
      for (j = l; j < mCols; j++) {
        for (s = 0.0, k = l; k < mRows; k++)
          s += this->U.at(k, i) * this->U.at(k, j);
        f = (s / this->U.at(i, i)) * g;
        for (k = i; k < mRows; k++) U.at(k, j) += f * U.at(k, i);
      }
      for (j = i; j < mRows; j++)
        U.at(j, i) *= g;
    } else {
      for (j = i; j < mRows; j++)
        U.at(j, i) = 0.0;
    }
    ++this->U.at(i, i);
  }

  bool flag;
  int its;
  size_t nm;
  T c, x, y, z;
  for (k = static_cast<int>(mCols) - one; k >= 0; k--) { //Diagonalization of the bidiagonal form: Loop over
    for (its = 0; its < mIterationMax; its++) { // singular values, and over allowed iterations.
      flag = true;
      for (l = k; l >= 0; l--) { //Test for splitting.
        nm = l - one; //Note that rv1[1] is always zero.
        if (l == 0 || abs(rv1[l]) <= eps * anorm) {
          flag = false;
          break;
        }
        if (abs(this->W[nm]) <= eps * anorm) break;
      }
      if (flag) {
        c = 0.0; //Cancellation of rv1[l], if l > 1.
        s = 1.0;
        for (i = l; i < k + one; i++) {
          f = s * rv1[i];
          rv1[i] = c * rv1[i];
          if (abs(f) <= eps * anorm) break;
          g = W[i];
          h = math::module(f, g);
          W[i] = h;
          h = static_cast<T>(1) / h;
          c = g * h;
          s = -f * h;
          for (j = 0; j < mRows; j++) {
            y = this->U.at(j, nm);
            z = this->U.at(j, i);
            this->U.at(j, nm) = y * c + z * s;
            this->U.at(j, i) = z * c - y * s;
          }
        }
      }
      z = W[k];
      if (l == k) { //Convergence.
        if (z < 0.0) { //Singular value is made nonnegative.
          W[k] = -z;
          for (j = 0; j < mCols; j++) V.at(j, k) = -V.at(j, k);
        }
        break;
      }
      if (its == 29) throw std::runtime_error("no convergence in 30 svdcmp iterations");
      x = W[l]; //Shift from bottom 2-by-2 minor.
      nm = k - one;
      y = W[nm];
      g = rv1[nm];
      h = rv1[k];
      f = ((y - z) * (y + z) + (g - h) * (g + h)) / (static_cast<T>(2) * h * y);
      g = math::module(f, static_cast<T>(1));
      f = ((x - z) * (x + z) + h * ((y / (f + std::copysign(g, f))) - h)) / x;
      c = s = 1.0; //Next QR transformation:
      for (j = l; j <= nm; j++) {
        i = j + one;
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
          x = V.at(jj, j);
          z = V.at(jj, i);
          V.at(jj, j) = x * c + z * s;
          V.at(jj, i) = z * c - x * s;
        }
        z = math::module(f, h);
        W[j] = z; //Rotation can be arbitrary if z = 0.
        if (z != 0.) {
          z = static_cast<T>(1) / z;
          c = f * z;
          s = h * z;
        }
        f = c * g + s * y;
        x = c * y - s * g;
        for (size_t jj = 0; jj < mRows; jj++) {
          y = this->U.at(jj, j);
          z = this->U.at(jj, i);
          this->U.at(jj, j) = y * c + z * s;
          this->U.at(jj, i) = z * c - y * s;
        }
      }
      rv1[l] = 0.0;
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
  int i, j, k, s, inc = 1;
  T sw;
  Vector<T, _rows> su(mRows);
  Vector<T, _cols> sv(mCols);
  
  if (mRows == DynamicVector) 

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

  for (k = 0; k < mCols; k++) {
    s = 0;
    for (i = 0; i < mRows; i++) if (U.at(i, k) < 0.) s++;
    for (j = 0; j < mCols; j++) if (V.at(j, k) < 0.) s++;
    if (s > (mRows + mCols) / 2) {
      for (i = 0; i < mRows; i++) U.at(i, k) = -U.at(i, k);
      for (j = 0; j < mCols; j++) V.at(j, k) = -V.at(j, k);
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
  //Vector<T, _cols> _s(mCols);
  T *superb = new T[std::min(mRows,mCols)-1];

  info = lapackeGESVD(mRows, mCols, A.data(), lda, W.data(), U.data(), ldu, V.data(), ldvt, superb);
  TL_TODO("Hay que transponer la matriz para que tenga sentido con el código propio...")
  V = V.transpose();

  delete[] superb;

  TL_ASSERT(info == 0, "The algorithm computing SVD failed to converge.");
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
inline int SingularValueDecomposition<Matrix_t<T, _rows, _cols>>::iterationMax() const
{
  return mIterationMax;
}

template<
  template<typename, size_t, size_t> 
  class Matrix_t, typename T, size_t _rows, size_t _cols
> 
inline void SingularValueDecomposition<Matrix_t<T, _rows, _cols>>::setIterationMax(int iterationMax)
{
  mIterationMax = iterationMax;
}


/*! \} */ // end of Algebra

/*! \} */ // end of Math

} // Fin namespace math


} // End namespace tl


#endif // TL_MATH_SVD_H
