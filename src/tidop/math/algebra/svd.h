#ifndef TL_MATH_SVD_H
#define TL_MATH_SVD_H

#include "tidop/math/algebra/matrix.h"
#include "tidop/math/algebra/vector.h"

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

template <typename Matrix_t>
class SingularValueDecomposition
{

public:

  typedef typename Matrix_t::value_type value_type;

protected:

  Matrix<Matrix_t::rows, Matrix_t::cols, value_type> u;

public:

  SingularValueDecomposition();
};

template<typename Matrix_t>
SingularValueDecomposition<Matrix_t>::SingularValueDecomposition()
{
}


//#define SIGN(a,b) ((b) >= 0.0 ? fabs(a) : -fabs(a))
//template<size_t _rows, size_t _cols, typename T = double>
////void svdcmp(float **a, int _cols, int _rows, float w[], float **v)
//Vector<_cols,T> svdcmp(const Matrix<_rows, _cols, T> &a, const Vector<_cols,T> &b)
//{
//  Vector<_cols,T> w;
//  float pythag(float a, float b);//???
//  size_t j, k, l;
//  double anorm, f, g, h, s, scale/*, *rv1*/;
//////  // reserva de memoria.
//////  rv1 = vector(1, _rows);
//  Vector<_rows, T> rv1;
//  g = scale = anorm = 0.0; //Householder reduction to bidiagonal form.
//
//  for (size_t i = 1; i <= _rows; i++) {
//    l = i + 1;
//    rv1[i] = scale*g;
//    g = s = scale = 0.0;
//    if (i <= _cols) {
//      for (k = i; k <= _cols; k++) scale += fabs(a.at(k,i));
//      if (scale != 0.) {
//        for (k = i; k <= _cols; k++) {
//          a.at(k,i) /= scale;
//          s += a.at(k,i) * a.at(k,i);
//        }
//        f = a.at(i,i);
//        g = -SIGN(sqrt(s), f);
//        h = f*g - s;
//        a.at(i,i) = f - g;
//        for (j = l; j <= _rows; j++) {
//          for (s = 0.0, k = i; k <= _cols; k++)
//            s += a.at(k,i) * a.at(k,j);
//          f = s / h;
//          for (k = i; k <= _cols; k++)
//            a.at(k,j) += f*a.at(k,i);
//        }
//        for (k = i; k <= _cols; k++)
//          a.at(k,i) *= scale;
//      }
//    }
//    w[i] = scale *g;
//    g = s = scale = 0.0;
//    if (i <= _cols && i != _rows) {
//      for (k = l; k <= _rows; k++) scale += fabs(a.at(i,k));
//      if (scale != 0.) {
//        for (k = l; k <= _rows; k++) {
//          a.at(i,k) /= scale;
//          s += a.at(i,k) * a.at(i,k);
//        }
//        f = a.at(i,l);
//        g = -SIGN(sqrt(s), f);
//        h = f*g - s;
//        a.at(i,l) = f - g;
//
//        for (k = l; k <= _rows; k++)
//          rv1[k] = a.at(i,k) / h;
//
//        for (j = l; j <= _cols; j++) {
//          for (s = 0.0, k = l; k <= _rows; k++)
//            s += a.at(j,k) * a.at(i,k);
//          for (k = l; k <= _rows; k++)
//            a.at(j,k) += s*rv1[k];
//        }
//
//        for (k = l; k <= _rows; k++)
//          a.at(i,k) *= scale;
//      }
//    }
//    anorm = std::max(anorm, (fabs(w[i]) + fabs(rv1[i])));
//  }
//
//  for (size_t i = _rows; i >= 1; i--) { //Accumulation of right-hand transformations.
//    if (i < _rows) {
//      if (g) {
//        for (j = l; j <= _rows; j++) //Double division to avoid possible underflow.
//          v[j][i] = (a[i][j] / a[i][l]) / g;
//        for (j = l; j <= _rows; j++) {
//          for (s = 0.0, k = l; k <= _rows; k++) s += a[i][k] * v[k][j];
//          for (k = l; k <= _rows; k++) v[k][j] += s*v[k][i];
//        }
//      }
//
//      for (j = l; j <= _rows; j++)
//        v[i][j] = v[j][i] = 0.0;
//    }
//    v[i][i] = 1.0;
//    g = rv1[i];
//    l = i;
//  }
//
//  for (size_t i = std::min(_cols, _rows); i >= 1; i--) { //Accumulation of left-hand transformations.
//    l = i + 1;
//    g = w[i];
//    for (j = l; j <= _rows; j++) a[i][j] = 0.0;
//    if (g) {
//      g = 1.0 / g;
//      for (j = l; j <= _rows; j++) {
//        for (s = 0.0, k = l; k <= _cols; k++) s += a[k][i] * a[k][j];
//        f = (s / a[i][i])*g;
//        for (k = i; k <= _cols; k++) a[k][j] += f*a[k][i];
//      }
//      for (j = i; j <= _cols; j++) a[j][i] *= g;
//    } else for (j = i; j <= _cols; j++) a[j][i] = 0.0;
//    ++a[i][i];
//  }
//
//  int flag, its, nm;
//  double c, x, y, z;
//  for (k = _rows; k >= 1; k--) { //Diagonalization of the bidiagonal form: Loop over
//    for (its = 1; its <= 30; its++) { // singular values, and over allowed iterations.
//        flag = 1;
//      for (l = k; l >= 1; l--) { //Test for splitting.
//        nm = l - 1; //Note that rv1[1] is always zero.
//        if ((float)(fabs(rv1[l]) + anorm) == anorm) {
//          flag = 0;
//          break;
//        }
//        if ((float)(fabs(w[nm]) + anorm) == anorm) break;
//      }
//      if (flag) {
//        c = 0.0; //Cancellation of rv1[l], if l > 1.
//        s = 1.0;
//        for (i = l; i <= k; i++) {
//          f = s*rv1[i];
//          rv1[i] = c*rv1[i];
//          if ((float)(fabs(f) + anorm) == anorm) break;
//          g = w[i];
//          h = pythag(f, g);
//          w[i] = h;
//          h = 1.0 / h;
//          c = g*h;
//          s = -f*h;
//          for (j = 1; j <= _cols; j++) {
//            y = a[j][nm];
//            z = a[j][i];
//            a[j][nm] = y*c + z*s;
//            a[j][i] = z*c - y*s;
//          }
//        }
//      }
//      z = w[k];
//      if (l == k) { //Convergence.
//        if (z < 0.0) { //Singular value is made nonnegative.
//          w[k] = -z;
//          for (j = 1; j <= _rows; j++) v[j][k] = -v[j][k];
//        }
//        break;
//      }
//      if (its == 30) msgError("no convergence in 30 svdcmp iterations");
//      x = w[l]; //Shift from bottom 2-by-2 minor.
//      nm = k - 1;
//      y = w[nm];
//      g = rv1[nm];
//      h = rv1[k];
//      f = ((y - z)*(y + z) + (g - h)*(g + h)) / (2.0*h*y);
//      g = pythag(f, 1.0);
//      f = ((x - z)*(x + z) + h*((y / (f + SIGN(g, f))) - h)) / x;
//      c = s = 1.0; //Next QR transformation:
//      for (j = l; j <= nm; j++) {
//        i = j + 1;
//        g = rv1[i];
//        y = w[i];
//        h = s*g;
//        g = c*g;
//        z = pythag(f, h);
//        rv1[j] = z;
//        c = f / z;
//        s = h / z;
//        f = x*c + g*s;
//        g = g*c - x*s;
//        h = y*s;
//        y *= c;
//        for (size_t jj = 1; jj <= _rows; jj++) {
//          x = v[jj][j];
//          z = v[jj][i];
//          v[jj][j] = x*c + z*s;
//          v[jj][i] = z*c - x*s;
//        }
//        z = pythag(f, h);
//        w[j] = z; //Rotation can be arbitrary if z = 0.
//        if (z) {
//          z = 1.0 / z;
//          c = f*z;
//          s = h*z;
//        }
//        f = c*g + s*y;
//        x = c*y - s*g;
//        for (size_t jj = 1; jj <= _cols; jj++) {
//          y = a[jj][j];
//          z = a[jj][i];
//          a[jj][j] = y*c + z*s;
//          a[jj][i] = z*c - y*s;
//        }
//      }
//      rv1[l] = 0.0;
//      rv1[k] = f;
//      w[k] = x;
//    }
//  }
////  free_vector(rv1, 1, _rows);
//}


/// svdcmp es la descomposición. Hay otra función que es la que resuelve el sistema svdfit

/*! \} */ // end of Algebra

/*! \} */ // end of Math

} // Fin namespace math


} // End namespace tl


#endif // TL_MATH_SVD_H
