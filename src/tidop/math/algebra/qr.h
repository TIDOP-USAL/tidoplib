#ifndef TL_MATH_QR_DECOMPOSITION_H
#define TL_MATH_QR_DECOMPOSITION_H

#include "config_tl.h"
#include "tidop/core/defs.h"
#include "tidop/core/messages.h"
#include "tidop/math/algebra/vector.h"

#include <algorithm>

namespace tl
{

namespace math
{

template<typename T, size_t _rows, size_t _cols>
class Matrix;

/*! \addtogroup Math
 *  \{
 */

template<typename T>
class QRDecomposition;

/*! \addtogroup Algebra
 *  \{
 */
template<
  template<typename, size_t, size_t> 
  class Matrix_t, typename T, size_t _rows, size_t _cols
>
class QRDecomposition<Matrix_t<T, _rows, _cols>>
{

public:

  typedef typename T value_type;

public:

  QRDecomposition(const Matrix_t<T, _rows, _cols> &a);

  Vector<T, _rows> solve(const Vector<T, _rows> &b);

private:

  void decompose();
  Vector<T, _rows> QRdcmp::qtmult(const Vector<T, _rows> &b);
  Vector<T, _rows> qtmult(const Vector<T, _rows> &b);
  Vector<T, _rows> rsolve(Vector<T, _rows> &b);
  Vector<T, _rows> update(Vector<T, _rows> &u);
  void rotate(int i, double a, double b);

private:

  TL_TODO("código duplicado de SingularValueDecomposition");
  T sign(T a, T b)
  {
    return ((b) >= static_cast<T>(0) ? fabs(a) : -fabs(a));
  }

protected:

  Matrix<T, _rows, _cols> qt;
  Matrix<T, _rows, _cols> r;
  bool sing;
  size_t mRows;
};


template<
  template<typename, size_t, size_t> 
  class Matrix_t, typename T, size_t _rows, size_t _cols
>
QRDecomposition<Matrix_t<T, _rows, _cols>>::QRDecomposition(const Matrix_t<T, _rows, _cols> &a)
  : qt(a.rows(), a.rows()),
    r(a),
  sing(false),
    mRows(a.rows())
{
  static_assert(_rows == _cols, "Non-Square Matrix");
  TL_ASSERT(this->qt.rows() == this->qt.cols(), "Non-Square Matrix");

  this->decompose();
}

template<
  template<typename, size_t, size_t>
  class Matrix_t, typename T, size_t _rows, size_t _cols
>
void QRDecomposition<Matrix_t<T, _rows, _cols>>::decompose()
{
  int i, j, k;
  T scale, sigma, sum, tau;

  Vector<T, _rows> c(n);
  Vector<T, _rows> d(n);

  for (k = 0; k < n - 1; k++) {

    scale = static_cast<T>(0);

    for (i = k; i < n; i++) {
      //scale = MAX(scale, abs(this->r(i, k)));
      scale = std::max(scale, abs(this->r(i, k)));
    }

    if (scale == 0.0) {
      sing = true;
      c[k] = d[k] = 0.0;
    } else {
      for (i = k; i < n; i++) {
        this->r(i, k) /= scale; 
      }

      for (sum = 0.0, i = k; i < n; i++) { 
        //sum += SQR(this->r(i, k));
        sum += sqrt(this->r(i, k)); 
      }

      sigma = sign(sqrt(sum), this->r(k, k));
      this->r(k, k) += sigma;
      c[k] = sigma * this->r(k, k);
      d[k] = -scale * sigma;
      for (j = k + 1; j < n; j++) {
        for (sum = 0.0, i = k; i < n; i++) {
          sum += this->r(i, k) * this->r(i, j);
        }

        tau = sum / c[k];
        for (i = k; i < n; i++) {
          this->r(i, j) -= tau * this->r(i, k);
        }
      }
    }
  }

  d[n - 1] = this->r(n - 1, n - 1);
  if (d[n - 1] == 0.0) sing = true;
  for (i = 0; i < n; i++) {
    for (j = 0; j < n; j++) this->qt(i, j) = 0.0;
    this->qt(i, i) = 1.0;
  }

  for (k = 0; k < n - 1; k++) {
    if (c[k] != 0.0) {
      for (j = 0; j < n; j++) {
        sum = 0.0;
        for (i = k; i < n; i++)
          sum += this->r(i, k) * this->qt(i, j);
        sum /= c[k];

        for (i = k; i < n; i++)
          this->qt(i, j) -= sum * this->r(i, k);
      }
    }
  }

  for (i = 0; i < n; i++) {
    this->r(i, i) = d[i];
    for (j = 0; j < i; j++) 
      this->r(i, j) = 0.0;
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
Vector<T, _rows> QRDecomposition<Matrix_t<T, _rows, _cols>>::qtmult(const Vector<T, _rows> &b) 
{
  int i,j;
  T sum;
  Vector<T, _rows> x;

  for (i=0;i<n;i++) {
    sum = 0.;
    for (j = 0; j < n; j++) { 
      sum += this->qt(i, j) * b[j];
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
  int i, j;
  double sum;
  Vector<T, _rows> x;
  if (sing) throw("attempting solve in a singular QR");
  for (i = n - 1; i >= 0; i--) {
    sum = b[i];
    for (j = i + 1; j < n; j++) sum -= this->r(i, j) * x[j];
    x[i] = sum / this->r(i, i);
  }
  return x;
}

template<
  template<typename, size_t, size_t>
  class Matrix_t, typename T, size_t _rows, size_t _cols
>
Vector<T, _rows> QRDecomposition<Matrix_t<T, _rows, _cols>>::update(Vector<T, _rows> &u)
{
  Vector<T, _rows>v(this);
  int i, k;
  Vector<T, _rows> w(u);
  for (k = n - 1; k >= 0; k--)
    if (w[k] != 0.0) break;
  if (k < 0) k = 0;
  for (i = k - 1; i >= 0; i--) {
    rotate(i, w[i], -w[i + 1]);
    if (w[i] == 0.0)
      w[i] = abs(w[i + 1]);
    else if (abs(w[i]) > abs(w[i + 1]))
      w[i] = abs(w[i]) * sqrt(1.0 + SQR(w[i + 1] / w[i]));
    else w[i] = abs(w[i + 1]) * sqrt(1.0 + SQR(w[i] / w[i + 1]));
  }
  for (i = 0; i < n; i++) this->r(0, i) += w[0] * v[i];
  for (i = 0; i < k; i++)
    rotate(i, this->r(i, i), -this->r(i + 1, i));
  for (i = 0; i < n; i++)
    if (this->r(i, i) == 0.0) sing = true;
  return v;
}
  
template<
  template<typename, size_t, size_t>
  class Matrix_t, typename T, size_t _rows, size_t _cols
>
void QRDecomposition<Matrix_t<T, _rows, _cols>>::rotate(int i, double a, double b)
{
  int j;
  double c, fact, s, w, y;
  if (a == 0.0) {
    c = 0.0;
    s = (b >= 0.0 ? 1.0 : -1.0);
  } else if (abs(a) > abs(b)) {
    fact = b / a;
    c = SIGN(1.0 / sqrt(1.0 + (fact * fact)), a);
    s = fact * c;
  } else {
    fact = a / b;
    s = SIGN(1.0 / sqrt(1.0 + (fact * fact)), b);
    c = fact * s;
  }
  for (j = i; j < n; j++) {
    y = this->r(i, j);
    w = this->r(i + 1, j);
    this->r(i, j) = c * y - s * w;
    this->r(i + 1, j) = s * y + c * w;
  }
  for (j = 0; j < n; j++) {
    y = this->qt(i, j);
    w = this->qt(i + 1, j);
    this->qt(i, j) = c * y - s * w;
    this->qt(i + 1, j) = s * y + c * w;
  }
}



/*! \} */ // end of Algebra

/*! \} */ // end of Math

} // Fin namespace math


} // End namespace tl


#endif // TL_MATH_QR_DECOMPOSITION_H
