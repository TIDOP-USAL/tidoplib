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
#include "tidop/core/defs.h"
#include "tidop/core/messages.h"
#include "tidop/math/algebra/vector.h"
#include "tidop/math/algebra/matrix.h"

#include <algorithm>

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
  
  void update(const Vector<T, _rows> &u, const Vector<T, _rows> &v);
  void jacobiRotation(int i, T a, T b);

  Matrix<T, _rows, _cols> q() const;
  Matrix<T, _rows, _cols> r() const;

private:

  //Householder
  void decompose();
  Vector<T, _rows> qtmult(const Vector<T, _rows> &b);
  Vector<T, _rows> rsolve(Vector<T, _rows> &b);

protected:

  Matrix<T, _rows, _cols> Q_t;
  Matrix<T, _rows, _cols> R;
  bool sing;
  size_t mRows;
};


template<
  template<typename, size_t, size_t> 
  class Matrix_t, typename T, size_t _rows, size_t _cols
>
QRDecomposition<Matrix_t<T, _rows, _cols>>::QRDecomposition(const Matrix_t<T, _rows, _cols> &a)
  : Q_t(Matrix<T, _rows, _cols>::identity(a.rows(), a.rows())),
    R(a),
    sing(false),
    mRows(a.rows())
{
  static_assert(std::is_floating_point<T>::value, "Integral type not supported");

  this->decompose();
}

template<
  template<typename, size_t, size_t>
  class Matrix_t, typename T, size_t _rows, size_t _cols
>
void QRDecomposition<Matrix_t<T, _rows, _cols>>::decompose()
{
  size_t i, j, k;
  T scale, sigma, sum, tau;

  Vector<T, _rows> c(mRows);
  Vector<T, _rows> d(mRows);

  for (k = 0; k < mRows - 1; k++) {

    scale = static_cast<T>(0);

    for (i = k; i < mRows; i++) {
      scale = std::max(scale, std::abs(this->R.at(i, k)));
    }

    if (scale == 0.0) {
      sing = true;
      c[k] = d[k] = 0.0;
    } else {
      for (i = k; i < mRows; i++) {
        this->R.at(i, k) /= scale; 
      }

      for (sum = 0.0, i = k; i < mRows; i++) { 
        sum += this->R.at(i, k)*this->R.at(i, k); 
      }

      sigma = std::copysign(sqrt(sum), this->R.at(k, k));
      this->R.at(k, k) += sigma;
      c[k] = sigma * this->R.at(k, k);
      d[k] = -scale * sigma;
      for (j = k + 1; j < mRows; j++) {
        for (sum = 0.0, i = k; i < mRows; i++) {
          sum += this->R.at(i, k) * this->R.at(i, j);
        }

        tau = sum / c[k];
        for (i = k; i < mRows; i++) {
          this->R.at(i, j) -= tau * this->R.at(i, k);
        }
      }
    }
  }

  d[mRows - 1] = this->R.at(mRows - 1, mRows - 1);
  if (d[mRows - 1] == 0.0) sing = true;

  for (k = 0; k < mRows - 1; k++) {
    if (c[k] != 0.0) {
      for (j = 0; j < mRows; j++) {
        sum = 0.0;
        for (i = k; i < mRows; i++)
          sum += this->R.at(i, k) * this->Q_t.at(i, j);
        sum /= c[k];

        for (i = k; i < mRows; i++)
          this->Q_t.at(i, j) -= sum * this->R.at(i, k);
      }
    }
  }

  for (i = 0; i < mRows; i++) {
    this->R.at(i, i) = d[i];
    for (j = 0; j < i; j++) 
      this->R.at(i, j) = 0.0;
  }
}

template<
  template<typename, size_t, size_t>
  class Matrix_t, typename T, size_t _rows, size_t _cols
>
Vector<T, _rows> QRDecomposition<Matrix_t<T, _rows, _cols>>::solve(const Vector<T, _rows> &b)
{
  TL_ASSERT(b.size() == mRows, "QRDecomposition::solve bad sizes");

  //if (b.size() != mRows || x.size() != mRows)
  //  throw("LUdcmp::solve bad sizes");
  
  Vector<T, _rows> x(b);

  x = this->qtmult(b);
  x = this->rsolve(x);

  return x;
}

template<
  template<typename, size_t, size_t>
  class Matrix_t, typename T, size_t _rows, size_t _cols
>
inline Matrix<T, _rows, _cols> QRDecomposition<Matrix_t<T, _rows, _cols>>::q() const
{
  return this->Q_t.transpose();
}

template<
  template<typename, size_t, size_t>
  class Matrix_t, typename T, size_t _rows, size_t _cols
>
inline Matrix<T, _rows, _cols> QRDecomposition<Matrix_t<T, _rows, _cols>>::r() const
{
  return this->R;
}

template<
  template<typename, size_t, size_t>
  class Matrix_t, typename T, size_t _rows, size_t _cols
>
Vector<T, _rows> QRDecomposition<Matrix_t<T, _rows, _cols>>::qtmult(const Vector<T, _rows> &b) 
{
  size_t i, j;
  T sum;
  Vector<T, _rows> x(mRows);

  for (i = 0; i < mRows; i++) {
    sum = static_cast<T>(0);
    for (j = 0; j < mRows; j++) {
      sum += this->Q_t.at(i, j) * b[j];
    }
    x[i] = sum;
  }

  return x;
}

template<
  template<typename, size_t, size_t>
  class Matrix_t, typename T, size_t _rows, size_t _cols
>
Vector<T, _rows> QRDecomposition<Matrix_t<T, _rows, _cols>>::rsolve(Vector<T, _rows> &b)
{
  TL_ASSERT(!sing, "attempting solve in a singular QR");

  int i, j;
  T sum;
  Vector<T, _rows> x(b);

  for (i = static_cast<int>(mRows) - 1; i >= 0; i--) {
    sum = b[i];
    for (j = i + 1; j < static_cast<int>(mRows); j++) 
      sum -= this->R.at(i, j) * x[j];
    x[i] = sum / this->R.at(i, i);
  }

  return x;
}

template<
  template<typename, size_t, size_t>
  class Matrix_t, typename T, size_t _rows, size_t _cols
>
void QRDecomposition<Matrix_t<T, _rows, _cols>>::update(const Vector<T, _rows> &u, 
                                                        const Vector<T, _rows> &v)
{
  int i, k;
  T aux;
  Vector<T, _rows> w(u);

  for (k = mRows - 1; k >= 0; k--)
    if (w[k] != 0.0) break;

  if (k < 0) k = 0;

  // Transform R + u*v to upper Hessenberg
  for (i = k - 1; i >= 0; i--) {
    jacobiRotation(i, w[i], -w[i + 1]);
    if (w[i] == 0.0) {
      w[i] = abs(w[i + 1]);
    } else if (abs(w[i]) > abs(w[i + 1])) {
      aux = w[i + 1] / w[i];
      w[i] = abs(w[i]) * sqrt(1.0 + aux*aux);
    } else {
      aux = w[i] / w[i + 1];
      w[i] = abs(w[i + 1]) * sqrt(1.0 + aux*aux);
    }
  }

  for (i = 0; i < mRows; i++) 
    this->R.at(0, i) += w[0] * v[i];

  // transform upper Hessenberg matrix to upper triangular
  for (i = 0; i < k; i++)
    jacobiRotation(i, this->R.at(i, i), -this->R.at(i + 1, i));

  for (i = 0; i < mRows; i++) {
    if (this->R.at(i, i) == 0.0)
      sing = true;
  }
}
  
template<
  template<typename, size_t, size_t>
  class Matrix_t, typename T, size_t _rows, size_t _cols
>
void QRDecomposition<Matrix_t<T, _rows, _cols>>::jacobiRotation(int i, T a, T b)
{
  int j;
  T c, fact, s, w, y;

  if (a == static_cast<T>(0)) {
    c = static_cast<T>(0);
    s = (b >= static_cast<T>(0) ? static_cast<T>(1) : -static_cast<T>(1));
  } else if (abs(a) > abs(b)) {
    fact = b / a;
    c = std::copysign(static_cast<T>(1) / sqrt(static_cast<T>(1)+ (fact * fact)), a);
    s = fact * c;
  } else {
    fact = a / b;
    s = std::copysign(static_cast<T>(1) / sqrt(static_cast<T>(1) + (fact * fact)), b);
    c = fact * s;
  }

  for (j = i; j < mRows; j++) {
    y = this->R.at(i, j);
    w = this->R.at(i + 1, j);
    this->R.at(i, j) = c * y - s * w;
    this->R.at(i + 1, j) = s * y + c * w;
  }

  for (j = 0; j < mRows; j++) {
    y = this->Q_t.at(i, j);
    w = this->Q_t.at(i + 1, j);
    this->Q_t.at(i, j) = c * y - s * w;
    this->Q_t.at(i + 1, j) = s * y + c * w;
  }
}



/*! \} */ // end of Algebra

/*! \} */ // end of Math

} // Fin namespace math


} // End namespace tl


#endif // TL_MATH_QR_DECOMPOSITION_H
