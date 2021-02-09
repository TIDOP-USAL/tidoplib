#ifndef TL_MATH_CHOLESKY_DECOMPOSITION_H
#define TL_MATH_CHOLESKY_DECOMPOSITION_H

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


/*! \addtogroup Math
 *  \{
 */


/*! \addtogroup Algebra
 *  \{
 */


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

  //Vector<T, _rows> solve(const Vector<T, _rows> &b);
  //Vector<T, _rows> elmult(const Vector<T, _rows> &y);
  //Vector<T, _rows> elsolve(const Vector<T, _rows> &b);
  //Matrix<T, _rows, _cols> inverse() const;
  Matrix<T, _rows, _cols> l() const;

private:

  void decompose();

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

  this->decompose();
}

//template<
//  template<typename, size_t, size_t> 
//  class Matrix_t, typename T, size_t _rows, size_t _cols
//>
//inline Vector<T, _rows> CholeskyDecomposition<Matrix_t<T, _rows, _cols>>::solve(const Vector<T, _rows> &b)
//{
//  TL_ASSERT(b.size() == mRows, "bad lengths in Cholesky");
//
//  int i,k;
//  double sum;
//  Vector<T, _rows> x(b);
//
//  for (i = 0; i < mRows; i++) {
//    for (sum = b[i], k = i - 1; k >= 0; k--) {
//      sum -= this->L.at(i, k) * x[k];
//    }
//    x[i] = sum / this->L.at(i, i);
//  }
//
//  for (i = mRows - 1; i >= 0; i--) {
//    for (sum = x[i], k = i + 1; k < mRows; k++) {
//      sum -= this->L.at(k, i) * x[k];
//    }
//    x[i] = sum / this->L.at(i, i);
//  }
//
//  return x;
//}

template<
  template<typename, size_t, size_t> 
  class Matrix_t, typename T, size_t _rows, size_t _cols
>
inline void CholeskyDecomposition<Matrix_t<T, _rows, _cols>>::decompose()
{
  int k;
  size_t i, j;
  T sum;

  for (i = 0; i < mRows; i++) {
    for (j = i; j < mRows; j++) {
      
      for (sum = this->L.at(i, j), k = i - 1; k >= 0; k--) {
        sum -= this->L.at(i, k) * this->L.at(j, k);
      }

      if (i == j) {
        TL_ASSERT(sum > 0.0, "Cholesky failed");
        this->L.at(i, i) = sqrt(sum);
      } else {
        this->L.at(j, i) = sum / this->L.at(i, i);
      }

    }
  }

  for (i = 0; i < mRows; i++) {
    for (j = 0; j < i; j++) {
      this->L.at(j, i) = static_cast<T>(0);
    }
  }
}

//template<
//  template<typename, size_t, size_t> 
//  class Matrix_t, typename T, size_t _rows, size_t _cols
//>
//inline Vector<T, _rows> CholeskyDecomposition<Matrix_t<T, _rows, _cols>>::elmult(const Vector<T, _rows> &y) 
//{
//  TL_ASSERT((y.size() == mRows, "bad lengths");
//
//  int i, j;
//  Vector<T, _rows> b(mRows);
//
//  
//  for (size_t i = 0; i < mRows; i++) {
//    b[i] = 0.;
//    for (j = 0; j <= i; j++) {
//      b[i] += this->L.at(i, j) * y[j];
//    }
//  }
//
//  return b;
//}

//template<
//  template<typename, size_t, size_t> 
//  class Matrix_t, typename T, size_t _rows, size_t _cols
//>
//inline Vector<T, _rows> CholeskyDecomposition<Matrix_t<T, _rows, _cols>>::elsolve(const Vector<T, _rows> &b) 
//{
//  TL_ASSERT(b.size() == mRows, "bad lengths");
//
//  size_t j;
//  double sum;
//  Vector<T, _rows> y(b);
//
//  for (size_t i = 0; i < mRows; i++) {
//    for (sum = b[i], j = 0; j < i; j++) {
//      sum -= this->L.at(i, j) * y[j];
//    }
//
//    y[i] = sum / this->L.at(i, i);
//  }
//
//  return y;
//}

//template<
//  template<typename, size_t, size_t> 
//  class Matrix_t, typename T, size_t _rows, size_t _cols
//>
//inline Matrix<T, _rows, _cols> CholeskyDecomposition<Matrix_t<T, _rows, _cols>>::inverse() const
//{
//  int i, j, k;
//  double sum;
//  Matrix<T, _rows, _cols> ainv(mRows, mRows);
//
//  for (i = 0; i < mRows; i++) {
//    for (j = 0; j <= i; j++) {
//      sum = (i == j ? 1. : 0.);
//      for (k = i - 1; k >= j; k--) { 
//        sum -= this->L.at(i, k) * ainv.at(j, k); 
//      }
//      ainv.at(j, i) = sum / this->L.at(i, i);
//    }
//  }
//
//  for (i = mRows - 1; i >= 0; i--) {
//    for (j = 0; j <= i; j++) {
//      sum = (i < j ? 0. : ainv.at(j, i));
//      for (k = i + 1; k < mRows; k++) { 
//        sum -= this->L.at(k, i) * ainv.at(j, k); 
//      }
//      ainv.at(i, j) = ainv.at(j, i) = sum / this->L.at(i, i);
//    }
//  }
//
//  return ainv;
//}

template<
  template<typename, size_t, size_t> 
  class Matrix_t, typename T, size_t _rows, size_t _cols
>
inline Matrix<T, _rows, _cols> CholeskyDecomposition<Matrix_t<T, _rows, _cols>>::l() const
{
  return this->L;
}

//template<
//  template<typename, size_t, size_t> 
//  class Matrix_t, typename T, size_t _rows, size_t _cols
//>
//inline double CholeskyDecomposition<Matrix_t<T, _rows, _cols>>::logdet() 
//{
//  double sum = 0.;
//  for (int i=0; i<mRows; i++) sum += log(this->L.at(i, i));
//  return 2.*sum;
//}

/*! \} */ // end of Algebra

/*! \} */ // end of Math

} // Fin namespace math


} // End namespace tl


#endif // TL_MATH_CHOLESKY_DECOMPOSITION_H
