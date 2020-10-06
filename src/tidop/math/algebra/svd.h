#ifndef TL_MATH_SVD_H
#define TL_MATH_SVD_H

#include "config_tl.h"
#include "tidop/core/defs.h"
#include "tidop/core/messages.h"
#include "tidop/math/algebra/matrix.h"
#include "tidop/math/algebra/vector.h"

#include <algorithm>

namespace tl
{

namespace math
{

/*! \addtogroup Math
 *  \{
 */

template<typename T>
class SingularValueDecomposition;

/*! \addtogroup Algebra
 *  \{
 */
template<
  template<size_t, size_t, typename> 
  class Matrix_t, size_t _rows, size_t _cols, typename T
>
class SingularValueDecomposition<Matrix_t<_rows, _cols, T>>
{

public:

  typedef typename T value_type;
  size_t rows = _rows;
  size_t cols = _cols;

public:

  SingularValueDecomposition(const Matrix_t<_rows, _cols, T> &a);

  Vector<_cols, T> solve(const Vector<_rows, T> &b);

  Matrix<_rows, _cols, T> u() const;
  Matrix<_cols, _cols, T> v() const;
  Vector<_cols, T> w() const;

  int iterationMax() const;
  void setIterationMax(int iterationMax);

private:

  double sign(double a, double b)
  {
    return ((b) >= 0.0 ? fabs(a) : -fabs(a));
  }

  double pythag(const double a, const double b)
  {
    double absa = abs(a);
    double absb = abs(b);
    double div = absb / absa;
    double inv_div = 1 / div;
	  return (absa > absb ? absa*sqrt(1.0+div*div) :
		       (absb == 0.0 ? 0.0 : absb*sqrt(1.0+inv_div*inv_div)));
  }

  void decompose();
  void reorder();

protected:

  Matrix_t<_rows, _cols, T> A;
  Matrix_t<_rows, _cols, T> U;
  Matrix_t<_cols, _cols, T> V;
  Vector<_cols, T> W;
  int mIterationMax;
  double eps;
  double tsh;
};


template<
  template<size_t, size_t, typename> 
  class Matrix_t, size_t _rows, size_t _cols, typename T
>
SingularValueDecomposition<Matrix_t<_rows, _cols, T>>::SingularValueDecomposition(const Matrix_t<_rows, _cols, T> &a)
  : A(a),
    mIterationMax(30)
{
  eps = std::numeric_limits<double>::epsilon();
  this->decompose();
  this->reorder();
  tsh = 0.5 * sqrt(_rows + _cols + 1.) * W[0] * eps;
}


template<
  template<size_t, size_t, typename>
  class Matrix_t, size_t _rows, size_t _cols, typename T
>
Vector<_cols, T> SingularValueDecomposition<Matrix_t<_rows, _cols, T>>::solve(const Vector<_rows, T> &B)
{
	T s;
  Vector<_cols, T> C;
  Vector<_cols, T> tmp;

  //tsh = (thresh >= 0. ? thresh : 0.5 * sqrt(_rows + _cols + 1.) * W[0] * eps);
	
  for (size_t j = 0; j < _cols; j++) {
    s = 0.0;
    if (W[j] > tsh) {
      for (size_t i = 0; i < _rows; i++) 
        s += U.at(i,j) * B[i];
      s /= W[j];
    }
    tmp[j] = s;
  }
	
  for (size_t j = 0; j < _cols; j++) {
    s = 0.0;
    for (size_t k = 0; k < _cols; k++)
      s += V.at(j,k) * tmp[k];
    C[j] = s;
  }

  return C;
}

template<template<size_t, size_t, typename> class Matrix_t, size_t _rows, size_t _cols, typename T>
inline void SingularValueDecomposition<Matrix_t<_rows,_cols,T>>::decompose()
{
  this->U = this->A;
  Vector<_cols, T> rv1;
  int i, j, k, l;
  double anorm, f, g, h, s, scale;
  g = scale = anorm = 0.0; //Householder reduction to bidiagonal form.

  for (i = 0; i < _cols; i++) {
    l = i + 2;
    rv1[i] = scale * g;
    g = s = scale = 0.0;

    if (i < _rows) {
      for (k = i; k < _rows; k++) scale += fabs(this->U.at(k,i));
      if (scale != 0.) {
        for (k = i; k < _rows; k++) {
          this->U.at(k,i) /= scale;
          s += this->U.at(k,i) * this->U.at(k,i);
        }
        f = this->U.at(i,i);
        g = -sign(sqrt(s), f);
        h = f*g - s;
        this->U.at(i,i) = f - g;
        for (j = l-1; j < _cols; j++) {
          for (s = 0.0, k = i; k < _rows; k++)
            s += this->U.at(k,i) * this->U.at(k,j);
          f = s / h;
          for (k = i; k < _rows; k++)
            this->U.at(k,j) += f*this->U.at(k,i);
        }
        for (k = i; k < _rows; k++)
          this->U.at(k,i) *= scale;
      }
    }

    W[i] = scale *g;
    g = s = scale = 0.0;
    if (i+1 <= _rows && i+1 != _cols) {
      for (k = l-1; k < _cols; k++) scale += fabs(this->U.at(i,k));
      if (scale != 0.) {
        for (k = l-1; k < _cols; k++) {
          this->U.at(i,k) /= scale;
          s += this->U.at(i,k) * this->U.at(i,k);
        }
        f = this->U.at(i,l-1);
        g = -sign(sqrt(s), f);
        h = f*g - s;
        this->U.at(i,l-1) = f - g;

        for (k = l-1; k < _cols; k++)
          rv1[k] = this->U.at(i,k) / h;

        for (j = l-1; j < _rows; j++) {
          for (s = 0.0, k = l-1; k < _cols; k++)
            s += this->U.at(j,k) * this->U.at(i,k);
          for (k = l-1; k < _cols; k++)
            this->U.at(j,k) += s*rv1[k];
        }

        for (k = l-1; k < _cols; k++)
          this->U.at(i,k) *= scale;
      }
    }
    anorm = std::max(anorm, static_cast<double>(std::abs(W[i]) + std::abs(rv1[i])));
  }


  for (i = _cols - 1; i >= 0; i--) { //Accumulation of right-hand transformations.
    if (i < _cols - 1) {
      if (g != 0.) {
        for (j = l; j < _cols; j++) //Double division to avoid possible underflow.
          V.at(j,i) = (this->U.at(i,j) / this->U.at(i,l)) / g;
        for (j = l; j < _cols; j++) {
          for (s = 0.0, k = l; k < _cols; k++)
            s += this->U.at(i,k) * V.at(k,j);
          for (k = l; k < _cols; k++)
            V.at(k,j) += s*V.at(k,i);
        }
      }

      for (j = l; j < _cols; j++)
        this->V.at(i,j) = this->V.at(j,i) = 0.0;
    }
    this->V.at(i,i) = 1.0;
    g = rv1[i];
    l = i;
  }

  for (i = std::min(_rows, _cols) - 1 ; i >= 0; i--) { //Accumulation of left-hand transformations.
    l = i + 1;
    g = W[i];
    for (j = l; j < _cols; j++) this->U.at(i,j) = 0.0;
    if (g != 0.) {
      g = 1.0 / g;
      for (j = l; j < _cols; j++) {
        for (s = 0.0, k = l; k < _rows; k++) 
          s += this->U.at(k,i) * this->U.at(k,j);
        f = (s / this->U.at(i,i))*g;
        for (k = i; k < _rows; k++) U.at(k,j) += f*U.at(k,i);
      }
      for (j = i; j < _rows; j++)
        U.at(j,i) *= g;
    } else {
      for (j = i; j < _rows; j++)
        U.at(j,i) = 0.0;
    }
    ++this->U.at(i,i);
  }

  bool flag;
  int its;
  size_t nm;
  double c, x, y, z;
  for (k = _cols - 1; k >= 0; k--) { //Diagonalization of the bidiagonal form: Loop over
    for (its = 0; its < mIterationMax; its++) { // singular values, and over allowed iterations.
      flag = true;
      for (l = k; l >= 0; l--) { //Test for splitting.
        nm = l - 1; //Note that rv1[1] is always zero.
        if (l == 0 || abs(rv1[l]) <= eps*anorm) {
          flag = false;
          break;
        }
        if (abs(this->W[nm]) <= eps*anorm) break;
      }
      if (flag) {
        c = 0.0; //Cancellation of rv1[l], if l > 1.
        s = 1.0;
        for (i = l; i < k+1; i++) {
          f = s*rv1[i];
          rv1[i] = c*rv1[i];
          if (abs(f) <= eps*anorm) break;
          g = W[i];
          h = pythag(f,g);
          W[i] = h;
          h = 1.0 / h;
          c = g*h;
          s = -f*h;
          for (j = 0; j < _rows; j++) {
            y = this->U.at(j,nm);
            z = this->U.at(j,i);
            this->U.at(j,nm) = y*c + z*s;
            this->U.at(j,i) = z*c - y*s;
          }
        }
      }
      z = W[k];
      if (l == k) { //Convergence.
        if (z < 0.0) { //Singular value is made nonnegative.
          W[k] = -z;
          for (j = 0; j < _cols; j++) V.at(j,k) = -V.at(j,k);
        }
        break;
      }
      if (its == 29) throw std::runtime_error("no convergence in 30 svdcmp iterations");
      x = W[l]; //Shift from bottom 2-by-2 minor.
      nm = k - 1;
      y = W[nm];
      g = rv1[nm];
      h = rv1[k];
      f = ((y - z)*(y + z) + (g - h)*(g + h)) / (2.0*h*y);
      g = pythag(f,1.0);
      f = ((x - z)*(x + z) + h*((y / (f + sign(g, f))) - h)) / x;
      c = s = 1.0; //Next QR transformation:
      for (j = l; j <= nm; j++) {
        i = j + 1;
        g = rv1[i];
        y = W[i];
        h = s*g;
        g = c*g;
        z = pythag(f,h);
        rv1[j] = z;
        c = f / z;
        s = h / z;
        f = x*c + g*s;
        g = g*c - x*s;
        h = y*s;
        y *= c;
        for (size_t jj = 0; jj < _cols; jj++) {
          x = V.at(jj,j);
          z = V.at(jj,i);
          V.at(jj,j) = x*c + z*s;
          V.at(jj,i) = z*c - x*s;
        }
        z = pythag(f,h);
        W[j] = z; //Rotation can be arbitrary if z = 0.
        if (z != 0.) {
          z = 1.0 / z;
          c = f*z;
          s = h*z;
        }
        f = c*g + s*y;
        x = c*y - s*g;
        for (size_t jj = 0; jj < _rows; jj++) {
          y = this->U.at(jj,j);
          z = this->U.at(jj,i);
          this->U.at(jj,j) = y*c + z*s;
          this->U.at(jj,i)= z*c - y*s;
        }
      }
      rv1[l] = 0.0;
      rv1[k] = f;
      W[k] = x;
    }
  }
}

template<template<size_t, size_t, typename> class Matrix_t, size_t _rows, size_t _cols, typename T> inline
void SingularValueDecomposition<Matrix_t<_rows, _cols, T>>::reorder()
{
  int i, j, k, s, inc = 1;
  double sw;
  Vector<_rows, T> su;
  Vector<_cols, T> sv;

  do { inc *= 3; inc++; } while (inc <= _cols);

  do {
    inc /= 3;
    for (i = inc; i < _cols; i++) {
      sw = W[i];
      for (k = 0; k < _rows; k++) su[k] = U.at(k, i);
      for (k = 0; k < _cols; k++) sv[k] = V.at(k, i);
      j = i;
      while (W[j - inc] < sw) {
        W[j] = W[j - inc];
        for (k = 0; k < _rows; k++) U.at(k, j) = U.at(k, j - inc);
        for (k = 0; k < _cols; k++) V.at(k, j) = V.at(k, j - inc);
        j -= inc;
        if (j < inc) break;
      }
      W[j] = sw;
      for (k = 0; k < _rows; k++) U.at(k, j) = su[k];
      for (k = 0; k < _cols; k++) V.at(k, j) = sv[k];

    }
  } while (inc > 1);

  for (k = 0; k < _cols; k++) {
    s = 0;
    for (i = 0; i < _rows; i++) if (U.at(i, k) < 0.) s++;
    for (j = 0; j < _cols; j++) if (V.at(j, k) < 0.) s++;
    if (s > (_rows + _cols) / 2) {
      for (i = 0; i < _rows; i++) U.at(i, k) = -U.at(i, k);
      for (j = 0; j < _cols; j++) V.at(j, k) = -V.at(j, k);
    }
  }
}

template<template<size_t, size_t, typename> class Matrix_t, size_t _rows, size_t _cols, typename T> inline
Matrix<_rows, _cols, T> SingularValueDecomposition<Matrix_t<_rows, _cols, T>>::u() const
{
  return U;
}

template<template<size_t, size_t, typename> class Matrix_t, size_t _rows, size_t _cols, typename T> inline
Matrix<_cols, _cols, T> SingularValueDecomposition<Matrix_t<_rows, _cols, T>>::v() const
{
  return V;
}

template<template<size_t, size_t, typename> class Matrix_t, size_t _rows, size_t _cols, typename T> inline
Vector<_cols, T> SingularValueDecomposition<Matrix_t<_rows, _cols, T>>::w() const
{
  return W;
}

template<template<size_t, size_t, typename> class Matrix_t, size_t _rows, size_t _cols, typename T> inline
int SingularValueDecomposition<Matrix_t<_rows, _cols, T>>::iterationMax() const
{
  return mIterationMax;
}

template<template<size_t, size_t, typename> class Matrix_t, size_t _rows, size_t _cols, typename T> inline
void SingularValueDecomposition<Matrix_t<_rows, _cols, T>>::setIterationMax(int iterationMax)
{
  mIterationMax = iterationMax;
}


/*! \} */ // end of Algebra

/*! \} */ // end of Math

} // Fin namespace math


} // End namespace tl


#endif // TL_MATH_SVD_H
