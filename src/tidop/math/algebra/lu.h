#ifndef TL_MATH_LU_DECOMPOSITION_H
#define TL_MATH_LU_DECOMPOSITION_H

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
class LuDecomposition;

/*! \addtogroup Algebra
 *  \{
 */
template<
  template<typename, size_t, size_t> 
  class Matrix_t, typename T, size_t _rows, size_t _cols
>
class LuDecomposition<Matrix_t<T, _rows, _cols>>
{

public:

  typedef typename T value_type;

public:

  LuDecomposition(const Matrix_t<T, _rows, _cols> &a);

  Vector<T, _cols> solve(const Vector<T, _rows> &b);
  Matrix<T, _rows, _cols> solve(const Matrix<T, _rows, _cols> &b);

  Matrix<T, _rows, _cols> lu() const;

  T determinant() const;
  Matrix<T, _rows, _cols> inverse();

private:

  void decompose();

protected:

  Matrix<T, _rows, _cols> LU;
  Vector<size_t, _rows> mIndx;
  T d;
  size_t mRows;
};


template<
  template<typename, size_t, size_t> 
  class Matrix_t, typename T, size_t _rows, size_t _cols
>
LuDecomposition<Matrix_t<T, _rows, _cols>>::LuDecomposition(const Matrix_t<T, _rows, _cols> &a)
  : LU(a),
    mIndx(a.rows()),
    mRows(a.rows())
{
  static_assert(_rows == _cols, "Non-Square Matrix");
  TL_ASSERT(LU.rows() == LU.cols(), "Non-Square Matrix");

  this->decompose();
}

template<
  template<typename, size_t, size_t>
  class Matrix_t, typename T, size_t _rows, size_t _cols
>
Vector<T, _cols> LuDecomposition<Matrix_t<T, _rows, _cols>>::solve(const Vector<T, _rows> &b)
{
  TL_ASSERT(b.size() == mRows, "LuDecomposition::solve bad sizes");

  Vector<T, _rows> x(b);
    
  T sum;
  size_t ii = 0;
  size_t j;
  size_t ip;
  for (size_t i = 0; i < mRows; i++) {
    ip = mIndx[i];
    sum = x[ip];
    x[ip] = x[i];
    if (ii != static_cast<size_t>(0)) {
      for (j = ii - static_cast<size_t>(1); j < i; j++) { 
        sum -= this->LU.at(i, j) * x[j]; 
      }
    } else if (sum != static_cast<T>(0)) {
      ii = i + static_cast<size_t>(1);
    }
      
    x[i] = sum;
  }

  for (int i = static_cast<int>(mRows) - 1; i >= 0; i--) {
    sum = x[i];
    for (j = i + 1; j < mRows; j++) sum -= this->LU.at(i, j) * x[j];
    x[i] = sum / this->LU.at(i, i);
  }

  return x;
}

template<
  template<typename, size_t, size_t>
  class Matrix_t, typename T, size_t _rows, size_t _cols
>
Matrix<T, _rows, _cols> LuDecomposition<Matrix_t<T, _rows, _cols>>::solve(const Matrix<T, _rows, _cols> &b)
{
  //static_assert(_rows == b.cols(), "LuDecomposition::solve bad sizes");
  TL_ASSERT(b.cols() == mRows, "LuDecomposition::solve bad sizes");

  Matrix<T, _rows, _cols> x(b);
  Vector<T, _rows> xx(mRows);

  for (size_t j = 0; j < mRows; j++) {

    for (size_t i = 0; i < mRows; i++) {
      xx[i] = b.at(i,j);
    }

    xx = this->solve(xx);

    for (size_t i = 0; i < mRows; i++) { 
      x.at(i,j) = xx[i]; 
    }
  }

  return x;
}

template<
  template<typename, size_t, size_t>
  class Matrix_t, typename T, size_t _rows, size_t _cols
>
void LuDecomposition<Matrix_t<T, _rows, _cols>>::decompose()
{
	const T TINY = static_cast<T>(1.0e-40);
  
  T big, temp;
	Vector<T, _rows> vv(mRows);

  this->d = static_cast<T>(1);

  for (size_t i = 0; i < mRows; i++) {

    big = static_cast<T>(0);

    for (size_t j = 0; j < mRows; j++) {
      if ((temp = abs(this->LU.at(i,j))) > big) {
        big = temp;
      }
    }
      
    //if (big == 0.0) throw("Singular matrix in LUdcmp");
    TL_ASSERT(big != 0.0, "Singular matrix");

    vv[i] = static_cast<T>(1) / big;
  }

  size_t imax = 0;

  for (size_t k = 0; k < mRows; k++) {

    big = 0.0;
    imax = k;

    for (size_t i = k; i < mRows; i++) {
      temp = vv[i] * abs(this->LU.at(i,k));
      if (temp > big) {
        big = temp;
        imax = i;
      }
    }

    if (k != imax) {
      for (size_t j = 0; j < mRows; j++) {
        temp = this->LU.at(imax, j);
        this->LU.at(imax, j) = this->LU.at(k, j);
        this->LU.at(k, j) = temp;
      }
      this->d = -this->d;
      vv[imax] = vv[k];
    }

    mIndx[k] = imax;
    if (this->LU.at(k,k) == 0.0) this->LU.at(k,k) = TINY;
    for (size_t i = k + static_cast<size_t>(1); i < mRows; i++) {
      temp = this->LU.at(i, k) /= this->LU.at(k, k);
      for (size_t j = k + 1; j < mRows; j++)
        this->LU.at(i, j) -= temp * this->LU.at(k, j);
    }
  }
}

template<
  template<typename, size_t, size_t> 
  class Matrix_t, typename T, size_t _rows, size_t _cols
> 
inline Matrix<T, _rows, _cols> LuDecomposition<Matrix_t<T, _rows, _cols>>::lu() const
{
  return this->LU;
}

template<
  template<typename, size_t, size_t> 
  class Matrix_t, typename T, size_t _rows, size_t _cols
> 
T LuDecomposition<Matrix_t<T, _rows, _cols>>::determinant() const
{
  T dd = this->d;

	for (size_t i = 0; i < mRows; i++) 
    dd *= this->LU.at(i, i);

	return dd;
}

template<
  template<typename, size_t, size_t> 
  class Matrix_t, typename T, size_t _rows, size_t _cols
> 
Matrix<T, _rows, _cols> LuDecomposition<Matrix_t<T, _rows, _cols>>::inverse()
{
  Matrix<T, _rows, _cols> matrix(mRows, mRows);

  for (size_t r = 0; r < mRows; r++) {
    for (size_t c = 0; c < mRows; c++) {
      if (r == c) {
        matrix.at(r, c) = static_cast<T>(1);
      } else {
        matrix.at(r, c) = static_cast<T>(0);
      }
    }
  }

  Matrix<T, _rows, _cols> inv = this->solve(matrix);
  return inv;
}


/*! \} */ // end of Algebra

/*! \} */ // end of Math

} // Fin namespace math


} // End namespace tl


#endif // TL_MATH_LU_DECOMPOSITION_H
