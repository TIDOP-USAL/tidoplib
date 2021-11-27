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

#ifndef TL_MATH_QR_DECOMPOSITION_H
#define TL_MATH_QR_DECOMPOSITION_H

#include "config_tl.h"

#include <algorithm>

//#ifdef HAVE_OPENBLAS
//#include <lapacke.h>
//#endif // HAVE_OPENBLAS

#include "tidop/math/math.h"
#include "tidop/core/messages.h"
#include "tidop/math/algebra/vector.h"
#include "tidop/math/algebra/matrix.h"

namespace tl
{

namespace math
{

//template<typename T, size_t _rows, size_t _cols>
//class Matrix;

/*! \addtogroup Math
 *  \{
 */


/*! \addtogroup Algebra
 *  \{
 */

//https://www.math.usm.edu/lambers/mat610/sum10/lecture9.pdf

//#ifdef HAVE_OPENBLAS
//
//template<typename T> inline
//  typename std::enable_if<
//  std::is_same<float, typename std::remove_cv<T>::type>::value, int>::type
//  lapackeGEQRF(lapack_int rows, lapack_int cols, T *a, lapack_int lda, T *superb)
//{
//  lapack_int info = LAPACKE_sgeqrf(LAPACK_ROW_MAJOR, rows, cols, a, lda, superb);
//  return info;
//}
//
//template<typename T> inline
//  typename std::enable_if<
//  std::is_same<double, typename std::remove_cv<T>::type>::value, int>::type
//  lapackeGEQRF(lapack_int rows, lapack_int cols, T *a, lapack_int lda, T *superb)
//{
//  lapack_int info = LAPACKE_dgeqrf(LAPACK_ROW_MAJOR, rows, cols, a, lda, superb);
//  return info;
//}
//
//
//#endif // HAVE_OPENBLAS


//https://rosettacode.org/wiki/QR_decomposition#C.2B.2B

/*!
 * \brief Factorización QR
 * 
 * La descomposición o factorización QR de una matriz es una descomposición 
 * de la misma como producto de una matriz ortogonal por una triangular 
 * superior.
 *
 * La descomposición QR de una matriz cuadrada T A es:
 * 
 * \f[ A = Q*R \f]
 *
 * donde Q es una matriz ortogonal:
 * 
 * \f[ Q^t*Q = I \f]
 * 
 * y R es una matriz triangular superior.
 */
template<typename T>
class QRDecomposition;

template<
  template<typename, size_t, size_t> 
  class Matrix_t, typename T, size_t _rows, size_t _cols
>
class QRDecomposition<Matrix_t<T, _rows, _cols>>
{

public:

  QRDecomposition(const Matrix_t<T, _rows, _cols> &a);

  Vector<T, _rows> solve(const Vector<T, _rows> &b);

  Matrix<T, _rows, _cols> q() const;
  Matrix<T, _rows, _cols> r() const;
  Matrix<T, _rows, _cols> qr() const;

private:

  //Householder
  void decompose();

//#ifdef HAVE_OPENBLAS
//  void lapackeDecompose();
//#endif // HAVE_OPENBLAS

private:

  Matrix<T, _rows, _cols> Q_t;
  Matrix<T, _rows, _cols> R;
//#ifdef HAVE_OPENBLAS
//  Matrix<T, _rows, _cols> QR;
//#endif // HAVE_OPENBLAS
  bool singular;
  size_t mRows;
};


template<
  template<typename, size_t, size_t> 
  class Matrix_t, typename T, size_t _rows, size_t _cols
>
QRDecomposition<Matrix_t<T, _rows, _cols>>::QRDecomposition(const Matrix_t<T, _rows, _cols> &a)
  : Q_t(Matrix<T, _rows, _cols>::identity(a.rows(), a.rows())),
    R(a),
//#ifdef HAVE_OPENBLAS
//    QR(a),
//#endif
    singular(false),
    mRows(a.rows())
{
  static_assert(std::is_floating_point<T>::value, "Integral type not supported");

//#ifdef HAVE_OPENBLAS
//  this->lapackeDecompose();
//#else
  this->decompose();
//#endif // HAVE_OPENBLAS

}

template<
  template<typename, size_t, size_t>
  class Matrix_t, typename T, size_t _rows, size_t _cols
>
void QRDecomposition<Matrix_t<T, _rows, _cols>>::decompose()
{

  Vector<T, _rows> c(mRows);
  Vector<T, _rows> diagonal(mRows);

  for (size_t k = 0; k < mRows - 1; k++) {

    T scale = consts::zero<T>;

    for (size_t i = k; i < mRows; i++) {
      scale = std::max(scale, std::abs(R[i][k]));
    }

    if (scale == consts::zero<T>) {

      singular = true;
      c[k] = diagonal[k] = consts::zero<T>;

    } else {

      for (size_t i = k; i < mRows; i++) {
        R[i][k] /= scale; 
      }

      T aux{0};

      for (size_t i = k; i < mRows; i++) { 
        aux += R[i][k] * R[i][k];
      }

      T sigma = std::copysign(sqrt(aux), R[k][k]);
      R.at(k, k) += sigma;
      c[k] = sigma * R[k][k];
      diagonal[k] = -scale * sigma;

      for (size_t j = k + 1; j < mRows; j++) {

        T aux{0};

        for (size_t i = k; i < mRows; i++) {
          aux += R[i][k] * R[i][j];
        }

        T tau = aux / c[k];
        
        for (size_t i = k; i < mRows; i++) {
          R[i][j] -= tau * R[i][k];
        }

      }
    }
  }

  diagonal[mRows - 1] = R[mRows - 1][mRows - 1];

  singular = (diagonal[mRows - 1] == consts::zero<T>);

  for (size_t k = 0; k < mRows - 1; k++) {

    if (c[k] != consts::zero<T>) {

      for (size_t j = 0; j < mRows; j++) {

        T aux{ 0 };
        for (size_t i = k; i < mRows; i++)
          aux += R[i][k] * Q_t[i][j];

        aux /= c[k];

        for (size_t i = k; i < mRows; i++)
          Q_t[i][j] -= aux * R[i][k];
      }

    }
  }

  for (size_t r = 0; r < mRows; r++) {

    R[r][r] = diagonal[r];

    for (size_t c = 0; c < r; c++)
      R[r][c] = consts::zero<T>;

  }
}

template<
  template<typename, size_t, size_t>
class Matrix_t, typename T, size_t _rows, size_t _cols
>
Vector<T, _rows> QRDecomposition<Matrix_t<T, _rows, _cols>>::solve(const Vector<T, _rows> &b)
{
  TL_ASSERT(b.size() == mRows, "QRDecomposition::solve bad sizes")
  TL_ASSERT(!singular, "Singular")

  Vector<T, _rows> x = Q_t * b;

  T aux;

  for (size_t i = mRows; i > 0; i--) {
    size_t r = i - 1;
    aux = x[r];
    for (size_t c = i; c < mRows; c++)
      aux -= R[r][c] * x[c];
    x[r] = aux / R[r][r];
  }

  return x;
}

template<
  template<typename, size_t, size_t>
class Matrix_t, typename T, size_t _rows, size_t _cols
>
inline Matrix<T, _rows, _cols> QRDecomposition<Matrix_t<T, _rows, _cols>>::q() const
{
  return Q_t.transpose();
}

//template<
//  template<typename, size_t, size_t>
//class Matrix_t, typename T, size_t _rows, size_t _cols
//>
//inline Matrix<T, _rows, _cols> QRDecomposition<Matrix_t<T, _rows, _cols>>::qr() const
//{
//  return QR;
//}

template<
  template<typename, size_t, size_t>
class Matrix_t, typename T, size_t _rows, size_t _cols
>
inline Matrix<T, _rows, _cols> QRDecomposition<Matrix_t<T, _rows, _cols>>::r() const
{
  return R;
}

//#ifdef HAVE_OPENBLAS
//
//template<
//  template<typename, size_t, size_t>
//class Matrix_t, typename T, size_t _rows, size_t _cols
//>
//inline void QRDecomposition<Matrix_t<T, _rows, _cols>>::lapackeDecompose()
//{
//  lapack_int info;
//  lapack_int lda = QR.cols();
//  T *superb = new T[std::min(QR.rows(), QR.cols()) - 1];
//  info = lapackeGEQRF(QR.rows(), QR.cols(), QR.data(), lda, superb);
//
//  //std::cout << QR << std::endl;
//  //for (size_t r = 1; r < QR.rows(); r++) {
//  //  for (size_t c = 0; c < r; c++){
//  //    R[r][c] = QR[r][c];
//  //  }
//  //}
//
//  TL_ASSERT(info >= 0, "The algorithm computing SVD failed to converge.");
//
//}
//
//#endif // HAVE_OPENBLAS



/*! \} */ // end of Algebra

/*! \} */ // end of Math

} // Fin namespace math


} // End namespace tl


#endif // TL_MATH_QR_DECOMPOSITION_H
