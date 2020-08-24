/****************************************************************************
 *                                                                          *
 *  This file is part of TidopLib and can not be copied and/or distributed  *
 *  without the express permision of ITOS3D ENGINEERING S.L                 *
 *                                                                          *
 *  Contact: http://www.itos3d.com                                          *
 *           http://tidop.usal.es                                           *
 *                                                                          *
 *--------------------------------------------------------------------------*
 *                                                                          *
 *  Copyright (C) 2018, ITOS3D ENGINEERING S.L - All rights reserved        *
 *                                                                          *
 ****************************************************************************/

#ifndef TL_MATH_MATRIX_H
#define TL_MATH_MATRIX_H

#include "config_tl.h"

#include "tidop/core/defs.h"

#include <vector>
#include <array>
#include <valarray>

namespace tl
{

namespace math
{

/*! \addtogroup Math
 *  \{
 */


/*! \defgroup Algebra Algebra
 *  
 * Algebra
 *
 *  \{
 */


/*!
 * \brief Clase matriz 
 *
 */
template<size_t _rows, size_t _cols, typename T = double>
class Matrix
{

public:

  typedef T value_type;

public:

  /*!
   * \brief Constructora por defecto
   */
  Matrix();

  /*!
   * \brief Constructora de copia
   * \param[in] mat Objeto Matrix que se copia
   */
  Matrix(const Matrix &mat);

  /*!
   * \brief Constructor de movimiento
   * \param[in] mat Objeto Matrix que se mueve
   */
  Matrix(Matrix &&mat) TL_NOEXCEPT;

  /*!
   * \brief Constructor
   * \param[in] mat Matriz
   */
  Matrix(const std::array<std::array<T, _cols>, _rows> &mat);
  
  Matrix(std::initializer_list<std::initializer_list<T>> mat);

  /*!
   * \brief destructora
   */
  ~Matrix();

  /*!
   * \brief Operador de asignación de copia
   * \param[in] mat Objeto que se copia
   */
  Matrix &operator = (const Matrix<_rows, _cols, T> &mat);
  
  /*!
   * \brief Operador de asignación de movimiento
   * \param[in] mat Objeto que se mueve
   */
  Matrix &operator = (Matrix<_rows, _cols, T> &&mat) TL_NOEXCEPT;

  /*!
   * \brief Referencia al elemento en la posición fila (r) y columna (c)
   * \param[in] r Fila de la matriz
   * \param[in] c Columna de la matriz
   * \return Valor de la matriz en la posición fila y columna
   * <h4>Ejemplo</h4>
   * \code
   * Matrix3x3d matrix;
   * matrix.at(0, 0) = 1.5;
   * double value = matrix.at(0, 0);
   * \endcode
   */
  T &at(size_t r, size_t c);

  /*!
   * \brief Referencia constante al elemento en la posición fila (r) y columna (c)
   * \param[in] r Fila
   * \param[in] c Columna
   * \return Valor de la matriz en la posición fila y columna
   * <h4>Ejemplo</h4>
   * \code
   * const double value = matrix.at(0, 0);
   * \endcode
   */
  const T &at(size_t r, size_t c) const;

  /*!
   * \brief Número de filas de la matriz
   * \return Número de filas
   */
  //size_t rows() const;

  /*!
   * \brief Número de columnas de la matriz
   * \return Número de columnas
   */
  //size_t cols() const;

  /*!
   * \brief Matriz inversa
   * Una matriz cuadrada e invertible A tiene una matriz inversa \f[ A^{-1} \f] 
   * \param[out] invertibility Comprueba si la matriz es invertible
   * \return Matriz inversa
   * <h4>Ejemplo</h4>
   * Matrix<2, 2> mat_2x2;
   * mat_2x2.at(0, 0) = 2.;
   * mat_2x2.at(0, 1) = 3.;
   * mat_2x2.at(1, 0) = 1.;
   * mat_2x2.at(1, 1) = 4.;
   * bool invertible;
   * Matrix<2, 2> inv_mat = mat_2x2.inverse(&invertible);
   * \endcode
   */
  Matrix inverse(bool *invertibility = nullptr) const;

  /*!
   * \brief Calcula la matriz transpuesta
   * \return Matriz transpuesta
   */
  Matrix<_cols, _rows, T> transpose() const;

  /*!
   * \brief Calcula la matriz de adjuntos
   * \f[ adj(A) = C^T \f]
   * \return Matriz de adjuntos
   */
  Matrix adjugate() const;

  /*!
   * \brief Calcula la matriz cofactor
   * \return Matriz cofactor
   */
  Matrix cofactorMatrix() const;

  /*!
   * \brief Determinante de la matriz
   * \return Determinante
   */
  T determinant() const;

  /*!
   * \brief Comprueba si la matrix es invertible
   * \return Verdadero si la matriz es invertible
   */
  bool invertible();

  /*!
   * \brief Comprueba si la matrix es singular
   * Una matriz cuadrada que no tiene inversa es singular. El 
   * determinante de una matriz singular es 0
   * \return Verdadero si la matriz es singular
   */
  bool singular();

  /*!
   * \brief Cofactor
   * El determinante obtenido al eliminar la fila y la columna de un elemento dado de una matriz o determinante. 
   * El cofactor está precedido por un signo + o - dependiendo de si el elemento está en una posición + o -.
   * \f[ (-)^{r+j} \f]
   * \return cofactor
   */
  T cofactor(size_t r, size_t c) const;

  /*!
   * \brief Primero menor
   * Un menor de una matriz cuadrada A es el determinante de alguna de las 
   * submatrices obtenidas a partir de la eliminación de una filas y una columna.
   * Se utilizan para el cálculo de la matriz de cofactores.
   *
   * \f[
   * A=\begin{bmatrix}
   * a1 & a2 & a3 \\
   * a4 & a5 & a6 \\
   * a7 & a8 & a9 \\
   * \end{bmatrix}
   * \f]
   *
   * \f[ M_{23} = a8*a1-a2*a7 \f]
   *
   * \return Primero menor
   */
  T firstMinor(size_t r, size_t c) const;

  /*!
   * \brief Construye una matriz de ceros
   * \f[
   * A=\begin{bmatrix}
   * 0 & 0 & 0 \\
   * 0 & 0 & 0 \\
   * 0 & 0 & 0 \\
   * \end{bmatrix}
   * \f]
   * \return
   */
  static Matrix zero();

  /*!
   * \brief Construye una matriz de 'unos'
   * \f[
   * A=\begin{bmatrix}
   * 1 & 1 & 1 \\
   * 1 & 1 & 1 \\
   * 1 & 1 & 1 \\
   * \end{bmatrix}
   * \f]
   * \return
   */
  static Matrix ones();

  /*!
   * \brief Construye la matriz identidad
   * \f[
   * A=\begin{bmatrix}
   * 1 & 0 & 0 \\
   * 0 & 1 & 0 \\
   * 0 & 0 & 1 \\
   * \end{bmatrix}
   * \f]
   * \return
   */
  static Matrix identity();

private:

  T determinant2x2() const;
  T determinant3x3() const;
  T determinant4x4() const;
  T determinantnxn() const;
  Matrix inverse2x2(bool *invertibility) const;
  Matrix inverse3x3(bool *invertibility) const;
  Matrix inverse4x4(bool *invertibility) const;
  Matrix inversenxn(bool *invertibility) const;

public:

  size_t rows = _rows;
  size_t cols = _cols;

protected:

  std::array<std::array<T, _cols>, _rows> mMatrix;
 // size_t mRows;
 // size_t mCols;
};



/* Definición de alias Matrix */

typedef Matrix<2,2,int>    Matrix2x2i;
typedef Matrix<2,2,float>  Matrix2x2f;
typedef Matrix<2,2,double> Matrix2x2d;
typedef Matrix<3,3,int>    Matrix3x3i;
typedef Matrix<3,3,float>  Matrix3x3f;
typedef Matrix<3,3,double> Matrix3x3d;
typedef Matrix<4,4,int>    Matrix4x4i;
typedef Matrix<4,4,float>  Matrix4x4f;
typedef Matrix<4,4,double> Matrix4x4d;



/* Implementación Matrix */

template<size_t _rows, size_t _cols, typename T>
Matrix<_rows, _cols, T>::Matrix()
  : rows(_rows),
    cols(_cols)
{
  T ini_val = -std::numeric_limits<T>().max();
  for (size_t r = 0; r < _rows; r++) {
    for (size_t c = 0; c < _cols; c++) {
      this->mMatrix[r][c] = ini_val;
    }
  }
}

template<size_t _rows, size_t _cols, typename T>
Matrix<_rows, _cols, T>::Matrix(const Matrix &mat)
  : mMatrix(mat.mMatrix),
    rows(_rows),
    cols(_cols)
{
}

template<size_t _rows, size_t _cols, typename T>
Matrix<_rows, _cols, T>::Matrix(Matrix &&mat) TL_NOEXCEPT
  : mMatrix(std::forward<std::array<std::array<T, _cols>, _rows>>(mat.mMatrix)),
    rows(mat.rows),
    cols(mat.cols)
{
}


template<size_t _rows, size_t _cols, typename T>
Matrix<_rows, _cols, T>::Matrix(const std::array<std::array<T, _cols>, _rows> &mat)
  : mMatrix(mat),
    rows(_rows),
    cols(_cols)
{
}

template<size_t _rows, size_t _cols, typename T> inline
Matrix<_rows, _cols, T>::Matrix(std::initializer_list<std::initializer_list<T>> mat)
  : rows(_rows),
    cols(_cols)
{
  TL_TODO("ver por que peta con mas de 3 filas")
  size_t n_rows = mat.size();
  auto it_row = mat.begin();
  const T zero{0};
  for (size_t r = 0; r < this->rows; r++) {
    if (r <= n_rows){
      auto it_col = it_row->begin();
      for (size_t c = 0; c < this->cols; c++) {
        if (r <= n_rows){
          this->mMatrix[r][c] = *it_col++;
        } else{
          this->mMatrix[r][c] = zero;
        }
      }
      it_row++;
    } else{
      for (size_t c = 0; c < this->cols; c++) {
        this->mMatrix[r][c] = zero;
      }
    }
  }

}

//template<size_t _rows, size_t _cols, typename T> inline
//Matrix<_rows, _cols, T>::Matrix(std::initializer_list<std::initializer_list<T>> mat)
//  : mMatrix(),
//    mRows(_rows),
//    mCols(_cols)
//{
//
//}

template<size_t _rows, size_t _cols, typename T>
Matrix<_rows, _cols, T>::~Matrix()
{
}

template<size_t _rows, size_t _cols, typename T>
Matrix<_rows, _cols, T> &Matrix<_rows, _cols, T>::operator = (const Matrix& rot)
{
  if (this != &rot) {
    this->mMatrix = rot.mMatrix;
    this->rows = rot.rows;
    this->cols = rot.cols;
  }
  return *this;
}

template<size_t _rows, size_t _cols, typename T>
Matrix<_rows, _cols, T> &Matrix<_rows, _cols, T>::operator = (Matrix &&rot) TL_NOEXCEPT
{
  if (this != &rot) {
    this->mMatrix = std::forward<std::array<std::array<T, _cols>, _rows>>(rot.mMatrix);
    this->rows = rot.rows;
    this->cols = rot.cols;
  }
  return *this;
}

template<size_t _rows, size_t _cols, typename T>
T &Matrix<_rows, _cols, T>::at(size_t r, size_t c)
{
  return mMatrix[r][c];
}

template<size_t _rows, size_t _cols, typename T>
const T &Matrix<_rows, _cols, T>::at(size_t r, size_t c) const
{
  return mMatrix[r][c];
}

//template<size_t _rows, size_t _cols, typename T>
//size_t Matrix<_rows, _cols, T>::rows() const
//{
//  return mRows;
//}

//template<size_t _rows, size_t _cols, typename T>
//size_t Matrix<_rows, _cols, T>::cols() const
//{
//  return mCols;
//}

template<size_t _rows, size_t _cols, typename T> 
Matrix<_rows, _cols, T> Matrix<_rows, _cols, T>::inverse(bool *invertibility) const
{
  static_assert(_rows == _cols, "Non-Square Matrix");
  Matrix<_rows, _cols, T> matrix;

  if (mMatrix.size() == 2) {
    matrix = inverse2x2(invertibility);
  } else if (mMatrix.size() == 3) {
    matrix = inverse3x3(invertibility);
  } else if (mMatrix.size() == 4) {
    matrix = inverse4x4(invertibility);
  } else {
    matrix = inversenxn(invertibility);
  }

  return matrix;
}

template<size_t _rows, size_t _cols, typename T> 
Matrix<_cols, _rows, T> Matrix<_rows, _cols, T>::transpose() const
{
  Matrix<_cols, _rows, T> matrix;
  for (size_t r = 0; r < _rows; r++) {
    for (size_t c = 0; c < _cols; c++) {
      matrix.at(c, r) = mMatrix[r][c];
    }
  }
  return matrix;
}

template<size_t _rows, size_t _cols, typename T>
Matrix<_rows, _cols, T> Matrix<_rows, _cols, T>::adjugate() const
{
  static_assert(_rows == _cols, "Non-Square Matrix");
  Matrix<_rows, _cols, T> matrix = this->cofactorMatrix();
  return matrix.transpose();
}

template<size_t _rows, size_t _cols, typename T> 
Matrix<_rows, _cols, T> Matrix<_rows, _cols, T>::cofactorMatrix() const
{
  static_assert(_rows == _cols, "Non-Square Matrix");
  Matrix<_rows, _cols, T> matrix;
  for (size_t r = 0; r < _rows; r++) {
    for (size_t c = 0; c < _cols; c++) {
      matrix.at(r, c) = cofactor(r, c);
    }
  }
  return matrix;
}

template<size_t _rows, size_t _cols, typename T> 
T Matrix<_rows, _cols, T>::determinant() const
{
  static_assert(_rows == _cols, "Non-Square Matrix");

  T d = static_cast<T>(1);
 
  if (mMatrix.size() == 2) {
    d = determinant2x2();
  } else if (mMatrix.size() == 3) {
    d = determinant3x3();
  } else if (mMatrix.size() == 4) {
    d = determinant4x4();
  } else {
    d = determinantnxn();
  }

  return d;
}

template<size_t _rows, size_t _cols, typename T> inline
bool tl::math::Matrix<_rows, _cols, T>::invertible()
{
  T det = determinant();
  if (det == static_cast<T>(0)) return false;
  else return true;
}

template<size_t _rows, size_t _cols, typename T> inline
bool tl::math::Matrix<_rows, _cols, T>::singular()
{
  T det = determinant();
  if (det == static_cast<T>(0)) return true;
  else return false;
}

template<size_t _rows, size_t _cols, typename T> 
T Matrix<_rows, _cols, T>::cofactor(size_t r, size_t c) const
{
  static_assert(_rows == _cols, "Non-Square Matrix");
  T sign = ((r + c) % 2 == 0) ? T{1} : -T{1};
  return sign * this->firstMinor(r, c);
}

template<size_t _rows, size_t _cols, typename T> 
T Matrix<_rows, _cols, T>::firstMinor(size_t r, size_t c) const
{
  static_assert(_rows == _cols, "Non-Square Matrix");

  size_t i = 0;
  size_t j = 0;
  Matrix<_rows-1, _cols-1, T> matrix;
  for (size_t row = 0; row < _rows; row++) {
    for (size_t col = 0; col < _cols; col++) {
      if (row != r && col != c) {
        matrix.at(i, j++) = this->mMatrix[row][col];
        if (j == _rows - 1) {
          j = 0;
          i++;
        }
      }
    }
  }
  return matrix.determinant();
}

template<size_t _rows, size_t _cols, typename T>
Matrix<_rows, _cols, T> Matrix<_rows, _cols, T>::zero()
{
  const T zero{0};
  Matrix<_rows, _cols, T> matrix;
  for (size_t r = 0; r < _rows; r++) {
    for (size_t c = 0; c < _cols; c++) {
      matrix.at(r, c) = zero;
    }
  }
  return matrix;
}

template<size_t _rows, size_t _cols, typename T>
Matrix<_rows, _cols, T> Matrix<_rows, _cols, T>::ones()
{
  const T one{1};
  Matrix<_rows, _cols, T> matrix;
  for (size_t r = 0; r < _rows; r++) {
    for (size_t c = 0; c < _cols; c++) {
      matrix.at(r, c) = one;
    }
  }
  return matrix;
}

template<size_t _rows, size_t _cols, typename T>
Matrix<_rows, _cols, T> Matrix<_rows, _cols, T>::identity()
{
  const T zero{0}; 
  const T one{1};
  Matrix<_rows, _cols, T> matrix;
  for (size_t r = 0; r < _rows; r++) {
    for (size_t c = 0; c < _cols; c++) {
      if (r == c) {
        matrix.at(r, c) = one;
      } else {
        matrix.at(r, c) = zero;
      }
    }
  }
  return matrix;
}

template<size_t _rows, size_t _cols, typename T> 
T Matrix<_rows, _cols, T>::determinant2x2() const
{
  T det = mMatrix[0][0] * mMatrix[1][1] - mMatrix[0][1] * mMatrix[1][0];
  return det;
}

template<size_t _rows, size_t _cols, typename T> 
T Matrix<_rows, _cols, T>::determinant3x3() const
{
  T c00 = mMatrix[1][1] * mMatrix[2][2] - mMatrix[1][2] * mMatrix[2][1];
  T c10 = mMatrix[1][2] * mMatrix[2][0] - mMatrix[1][0] * mMatrix[2][2];
  T c20 = mMatrix[1][0] * mMatrix[2][1] - mMatrix[1][1] * mMatrix[2][0];
  T det = mMatrix[0][0] * c00 + mMatrix[0][1] * c10 + mMatrix[0][2] * c20;
  return det;
}

template<size_t _rows, size_t _cols, typename T> 
T Matrix<_rows, _cols, T>::determinant4x4() const
{
  T a0 = mMatrix[0][0] * mMatrix[1][1] - mMatrix[0][1] * mMatrix[1][0];
  T a1 = mMatrix[0][0] * mMatrix[1][2] - mMatrix[0][2] * mMatrix[1][0];
  T a2 = mMatrix[0][0] * mMatrix[1][3] - mMatrix[0][3] * mMatrix[1][0];
  T a3 = mMatrix[0][1] * mMatrix[1][2] - mMatrix[0][2] * mMatrix[1][1];
  T a4 = mMatrix[0][1] * mMatrix[1][3] - mMatrix[0][3] * mMatrix[1][1];
  T a5 = mMatrix[0][2] * mMatrix[1][3] - mMatrix[0][3] * mMatrix[1][2];
  T b0 = mMatrix[2][0] * mMatrix[3][1] - mMatrix[2][1] * mMatrix[3][0];
  T b1 = mMatrix[2][0] * mMatrix[3][2] - mMatrix[2][2] * mMatrix[3][0];
  T b2 = mMatrix[2][0] * mMatrix[3][3] - mMatrix[2][3] * mMatrix[3][0];
  T b3 = mMatrix[2][1] * mMatrix[3][2] - mMatrix[2][2] * mMatrix[3][1];
  T b4 = mMatrix[2][1] * mMatrix[3][3] - mMatrix[2][3] * mMatrix[3][1];
  T b5 = mMatrix[2][2] * mMatrix[3][3] - mMatrix[2][3] * mMatrix[3][2];
  T det = a0 * b5 - a1 * b4 + a2 * b3 + a3 * b2 - a4 * b1 + a5 * b0;
  return det;
}

template<size_t _rows, size_t _cols, typename T> 
T Matrix<_rows, _cols, T>::determinantnxn() const
{
  const T zero{0};
  const T one{1};
  T d = one;
  size_t n = mMatrix.size();
  std::array<std::array<T, _cols>, _rows> matrix(mMatrix);

  for (size_t i = 0; i < n; ++i) {
    T pivotElement = matrix[i][i];
    size_t pivotRow = i;
    for (size_t r = i + 1; r < n; ++r) {
      if (std::abs(matrix[r][i]) > std::abs(pivotElement)) {
        pivotElement = matrix[r][i];
        pivotRow = r;
      }
    }

    if (pivotElement == zero) {
      return zero;
    }

    if (pivotRow != i) {
      matrix[i].swap(matrix[pivotRow]);
      d *= -one;
    }

    d *= pivotElement;

    for (size_t r = i + 1; r < n; ++r) {
      for (size_t c = i + 1; c < n; ++c) {
        matrix[r][c] -= matrix[r][i] * matrix[i][c] / pivotElement;
      }
    }
  }
  return d;
}

template<size_t _rows, size_t _cols, typename T> 
Matrix<_rows, _cols, T> Matrix<_rows, _cols, T>::inverse2x2(bool *invertibility) const
{
  Matrix<_rows, _cols, T> matrix;
  T det = determinant2x2();
  if (det != static_cast<T>(0)) {
    matrix.at(0, 0) =  mMatrix[1][1] / det;
    matrix.at(0, 1) = -mMatrix[0][1] / det;
    matrix.at(1, 0) = -mMatrix[1][0] / det;
    matrix.at(1, 1) =  mMatrix[0][0] / det;
    if (invertibility) *invertibility = true;
  } else {
    if (invertibility) *invertibility = false;
  }
  return matrix;
}

template<size_t _rows, size_t _cols, typename T> 
Matrix<_rows, _cols, T> Matrix<_rows, _cols, T>::inverse3x3(bool *invertibility) const
{
  Matrix<_rows, _cols, T> matrix;

  T c00 = mMatrix[1][1] * mMatrix[2][2] - mMatrix[1][2] * mMatrix[2][1];
  T c10 = mMatrix[1][2] * mMatrix[2][0] - mMatrix[1][0] * mMatrix[2][2];
  T c20 = mMatrix[1][0] * mMatrix[2][1] - mMatrix[1][1] * mMatrix[2][0];
  T det = mMatrix[0][0] * c00 + mMatrix[0][1] * c10 + mMatrix[0][2] * c20;

  if (det != static_cast<T>(0)) {

    matrix.at(0, 0) = c00 / det;
    matrix.at(0, 1) = (mMatrix[0][2] * mMatrix[2][1] - mMatrix[0][1] * mMatrix[2][2]) / det;
    matrix.at(0, 2) = (mMatrix[0][1] * mMatrix[1][2] - mMatrix[0][2] * mMatrix[1][1]) / det;
    matrix.at(1, 0) = c10 / det;
    matrix.at(1, 1) = (mMatrix[0][0] * mMatrix[2][2] - mMatrix[0][2] * mMatrix[2][0]) / det;
    matrix.at(1, 2) = (mMatrix[0][2] * mMatrix[1][0] - mMatrix[0][0] * mMatrix[1][2]) / det;
    matrix.at(2, 0) = c20 / det;
    matrix.at(2, 1) = (mMatrix[0][1] * mMatrix[2][0] - mMatrix[0][0] * mMatrix[2][1]) / det;
    matrix.at(2, 2) = (mMatrix[0][0] * mMatrix[1][1] - mMatrix[0][1] * mMatrix[1][0]) / det;
    if (invertibility) *invertibility = true;
  } else {
    if (invertibility) *invertibility = false;
  }

  return matrix;
}

template<size_t _rows, size_t _cols, typename T> 
Matrix<_rows, _cols, T> Matrix<_rows, _cols, T>::inverse4x4(bool *invertibility) const
{
  Matrix<_rows, _cols, T> matrix;

  T m00 = mMatrix[0][0];
  T m01 = mMatrix[0][1];
  T m02 = mMatrix[0][2];
  T m03 = mMatrix[0][3];
  T m10 = mMatrix[1][0];
  T m11 = mMatrix[1][1];
  T m12 = mMatrix[1][2];
  T m13 = mMatrix[1][3];
  T m20 = mMatrix[2][0];
  T m21 = mMatrix[2][1];
  T m22 = mMatrix[2][2];
  T m23 = mMatrix[2][3];
  T m30 = mMatrix[3][0];
  T m31 = mMatrix[3][1];
  T m32 = mMatrix[3][2];
  T m33 = mMatrix[3][3];

  T a0 = m00 * m11 - m01 * m10;
  T a1 = m00 * m12 - m02 * m10;
  T a2 = m00 * m13 - m03 * m10;
  T a3 = m01 * m12 - m02 * m11;
  T a4 = m01 * m13 - m03 * m11;
  T a5 = m02 * m13 - m03 * m12;
  T b0 = m20 * m31 - m21 * m30;
  T b1 = m20 * m32 - m22 * m30;
  T b2 = m20 * m33 - m23 * m30;
  T b3 = m21 * m32 - m22 * m31;
  T b4 = m21 * m33 - m23 * m31;
  T b5 = m21 * m33 - m23 * m32;
  T det = a0 * b5 - a1 * b4 + a2 * b3 + a3 * b2 - a4 * b1 + a5 * b0;

  if (det != static_cast<T>(0)) {
    
    matrix.at(0, 0) = ( m11 * b5 - m12 * b4 + m13 * b3) / det;
    matrix.at(0, 1) = (-m01 * b5 + m02 * b4 - m03 * b3) / det;
    matrix.at(0, 2) = ( m31 * a5 - m32 * a4 + m33 * a3) / det;
    matrix.at(0, 3) = (-m21 * a5 + m22 * a4 - m23 * a3) / det;
    matrix.at(1, 0) = (-m10 * b5 + m12 * b2 - m13 * b1) / det;
    matrix.at(1, 1) = ( m00 * b5 - m02 * b2 + m03 * b1) / det;
    matrix.at(1, 2) = (-m30 * a5 + m32 * a2 - m33 * a1) / det;
    matrix.at(1, 3) = ( m20 * a5 - m22 * a2 + m23 * a1) / det;
    matrix.at(2, 0) = ( m10 * b4 - m11 * b2 + m13 * b0) / det;
    matrix.at(2, 1) = (-m00 * b4 + m01 * b2 - m03 * b0) / det;
    matrix.at(2, 2) = ( m30 * a4 - m31 * a2 + m33 * a0) / det;
    matrix.at(2, 3) = (-m20 * a4 + m21 * a2 - m23 * a0) / det;
    matrix.at(3, 0) = (-m10 * b3 + m11 * b1 - m12 * b0) / det;
    matrix.at(3, 1) = ( m00 * b3 - m01 * b1 + m02 * b0) / det;
    matrix.at(3, 2) = (-m30 * a3 + m31 * a1 - m32 * a0) / det;
    matrix.at(3, 3) = ( m20 * a3 - m21 * a1 + m22 * a0) / det;

    if (invertibility) *invertibility = true;
  } else {
    if (invertibility) *invertibility = false;
  }

  return matrix;
}

template<size_t _rows, size_t _cols, typename T> 
Matrix<_rows, _cols, T> Matrix<_rows, _cols, T>::inversenxn(bool *invertibility) const
{
  Matrix<_rows, _cols, T> matrix;
  T det = determinantnxn(); 
  if (det == static_cast<T>(0)) { 
    // Matriz singular, no se puede encontrar la inversa;
    if (invertibility != nullptr) *invertibility = false;
    return matrix; 
  }


  return matrix;
}





/* Operaciones unarias */

template<size_t _rows, size_t _cols, typename T>  static
Matrix<_rows, _cols, T> operator + (const Matrix<_rows, _cols, T> &matrix)
{
  return matrix;
}

template<size_t _rows, size_t _cols, typename T> static
Matrix<_rows, _cols, T> operator - (const Matrix<_rows, _cols, T> &matrix)
{
  Matrix<_rows, _cols, T> _m;
  for (size_t r = 0; r < _rows; r++) {
    for (size_t c = 0; c < _cols; c++) {
      _m.at(r, c) = -matrix.at(r, c);
    }
  }
  return _m;
}

/* Operaciones binarias entre matrices */

/*!
 * \brief Suma o adición de matrices 
 * 
 * \f[ C = A + B \f]
 *
 * \f[
 * A=\begin{bmatrix}
 * a1 & a2 & a3 \\
 * a4 & a5 & a6 \\
 * a7 & a8 & a9 \\
 * \end{bmatrix}
 *
 * B=\begin{bmatrix}
 * b1 & b2 & b3 \\
 * b4 & b5 & b6 \\
 * b7 & b8 & b9 \\
 * \end{bmatrix}
 *
 * C=\begin{bmatrix}
 * a1+b1 & a2+b2 & a3+b3 \\
 * a4+b4 & a5+b5 & a6+b6 \\
 * a7+b7 & a8+b8 & a9+b9 \\
 * \end{bmatrix}
 * \f]
 *
 * <h4>Ejemplo</h4>
 * \code
 * Matrix2x2i A;
 * Matrix2x2i B;
 *
 * A.at(0, 0) = 1;
 * A.at(0, 1) = 4;
 * A.at(1, 0) = 3;
 * A.at(1, 1) = 2;
 *
 * B.at(0, 0) = 4;
 * B.at(0, 1) = 5;
 * B.at(1, 0) = 2;
 * B.at(1, 1) = 8;
 *
 * Matrix2x2i C = A + B;
 * \endcode
 */
template<size_t _rows, size_t _cols, typename T> static
Matrix<_rows, _cols, T> operator + (const Matrix<_rows, _cols, T> &matrix1,
                                    const Matrix<_rows, _cols, T> &matrix2)
{
  Matrix<_rows, _cols, T> matrix = matrix1;
  return matrix += matrix2;
}

template<size_t _rows, size_t _cols, typename T> static
Matrix<_rows, _cols, T> &operator += (Matrix<_rows, _cols, T> &matrix1,
                                      const Matrix<_rows, _cols, T> &matrix2)
{
  for (int r = 0; r < _rows; r++) {
    for (int c = 0; c < _cols; c++) {
      matrix1.at(r, c) += matrix2.at(r, c);
    }
  }
  return matrix1;
}

/*!
 * \brief Resta de matrices 
 * 
 * \f[ C = A - B \f]
 *
 * \f[
 * A=\begin{bmatrix}
 * a1 & a2 & a3 \\
 * a4 & a5 & a6 \\
 * a7 & a8 & a9 \\
 * \end{bmatrix}
 *
 * B=\begin{bmatrix}
 * b1 & b2 & b3 \\
 * b4 & b5 & b6 \\
 * b7 & b8 & b9 \\
 * \end{bmatrix}
 *
 * C=\begin{bmatrix}
 * a1-b1 & a2-b2 & a3-b3 \\
 * a4-b4 & a5-b5 & a6-b6 \\
 * a7-b7 & a8-b8 & a9-b9 \\
 * \end{bmatrix}
 * \f]
 *
 * <h4>Ejemplo</h4>
 * \code
 * Matrix2x2i A;
 * Matrix2x2i B;
 *
 * A.at(0, 0) = 1;
 * A.at(0, 1) = 4;
 * A.at(1, 0) = 3;
 * A.at(1, 1) = 2;
 *
 * B.at(0, 0) = 4;
 * B.at(0, 1) = 5;
 * B.at(1, 0) = 2;
 * B.at(1, 1) = 8;
 *
 * Matrix2x2i C = A - B;
 * \endcode
 */
template<size_t _rows, size_t _cols, typename T> static
Matrix<_rows, _cols, T> operator - (const Matrix<_rows, _cols, T> &matrix1,
                                    const Matrix<_rows, _cols, T> &matrix2)
{
  Matrix<_rows, _cols, T> matrix = matrix1;
  return matrix -= matrix2;
}

template<size_t _rows, size_t _cols, typename T> static
Matrix<_rows, _cols, T> &operator -= (Matrix<_rows, _cols, T> &matrix1,
                                      const Matrix<_rows, _cols, T> &matrix2)
{
  for (int r = 0; r < _rows; r++) {
    for (int c = 0; c < _cols; c++) {
      matrix1.at(r, c) -= matrix2.at(r, c);
    }
  }
  return matrix1;
}

/*!
 * \brief Multiplicación de matrices
 *
 * \f[ C = A * B \f]
 *
 * \f[
 * A=\begin{bmatrix}
 * a1 & a2  \\
 * a3 & a4  \\
 * \end{bmatrix}
 *
 * B=\begin{bmatrix}
 * b1 & b2 & b3 \\
 * b4 & b5 & b6 \\
 * \end{bmatrix}
 *
 * C=\begin{bmatrix}
 * a1*b1+a2*b4 & a1*b2+a2*b5 & a1*b3+a2*b6 \\
 * a3*b1+a4*b4 & a3*b2+a4*b5 & a3*b3+a4*b6 \\
 * \end{bmatrix}
 * \f]
 *
 * <h4>Ejemplo</h4>
 * \code
 * Matrix2x2i A;
 * Matrix2x2i B;
 *
 * A.at(0, 0) = 1;
 * A.at(0, 1) = 4;
 * A.at(1, 0) = 3;
 * A.at(1, 1) = 2;
 *
 * B.at(0, 0) = 4;
 * B.at(0, 1) = 5;
 * B.at(1, 0) = 2;
 * B.at(1, 1) = 8;
 *
 * Matrix2x2i C = A * B;
 * \endcode
 */
template<size_t _rows, size_t _dim, size_t _cols, typename T>  static
Matrix<_rows, _cols, T> operator * (const Matrix<_rows, _dim, T> &matrix1,
                                    const Matrix<_dim, _cols, T> &matrix2)
{
  Matrix<_rows, _cols, T> matrix = Matrix<_rows, _cols, T>::zero();
  for (size_t r = 0; r < _rows; r++) {
    for (size_t c = 0; c < _cols; c++) {
      for (size_t i = 0; i < _dim; i++) {
        matrix.at(r, c) += matrix1.at(r, i) * matrix2.at(i, c);
      }
    }
  }
  return matrix;
}

/*!
 * \brief Multiplicación de una matriz por un escalar
 *
 * \f[ C = A * s \f]
 *
 * \f[
 * A=\begin{bmatrix}
 * a1 & a2 & a3 \\
 * a4 & a5 & a6 \\
 * a7 & a8 & a9 \\
 * \end{bmatrix}
 *
 * C=\begin{bmatrix}
 * a1*s & a2*s & a3*s \\
 * a4*s & a5*s & a6*s \\
 * a7*s & a8*s & a9*s \\
 * \end{bmatrix}
 * \f]
 *
 * <h4>Ejemplo</h4>
 * \code
 * Matrix2x2i A;
 *
 * A.at(0, 0) = 1;
 * A.at(0, 1) = 4;
 * A.at(1, 0) = 3;
 * A.at(1, 1) = 2;
 *
 * int s = 2;
 * Matrix2x2i C = A * s;
 * \endcode
 */
template<size_t _rows, size_t _cols, typename T> static
Matrix<_rows, _cols, T> operator * (const Matrix<_rows, _cols, T> &matrix, T scalar)
{
  Matrix<_rows, _cols, T> _matrix = matrix;
  return _matrix *= scalar;
}


/*!
 * \brief Multiplicación de un escalar por una matriz
 *
 * \f[ C = s * A \f]
 *
 * \f[
 * A=\begin{bmatrix}
 * a1 & a2 & a3 \\
 * a4 & a5 & a6 \\
 * a7 & a8 & a9 \\
 * \end{bmatrix}
 *
 * C=\begin{bmatrix}
 * s*a1 & s*a2 & s*a3 \\
 * s*a4 & s*a5 & s*a6 \\
 * s*a7 & s*a8 & s*a9 \\
 * \end{bmatrix}
 * \f]
 *
 * <h4>Ejemplo</h4>
 * \code
 * Matrix2x2i A;
 *
 * A.at(0, 0) = 1;
 * A.at(0, 1) = 4;
 * A.at(1, 0) = 3;
 * A.at(1, 1) = 2;
 *
 * int s = 2;
 * Matrix2x2i C = s * A;
 * \endcode
 */
template<size_t _rows, size_t _cols, typename T> static
Matrix<_rows, _cols, T> operator * (T scalar, const Matrix<_rows, _cols, T> &matrix)
{
  Matrix<_rows, _cols, T> _matrix = matrix;
  return _matrix *= scalar;
}

template<size_t _rows, size_t _cols, typename T> static
Matrix<_rows, _cols, T> &operator *= (Matrix<_rows, _cols, T> &matrix, T scalar)
{
  for (int r = 0; r < _rows; r++) {
    for (int c = 0; c < _cols; c++) {
      matrix.at(r, c) *= scalar;
    }
  }
  return matrix;
}

/*!
 * \brief División de una matriz por un escalar
 *
 * \f[ C = A / s \f]
 *
 * \f[
 * A=\begin{bmatrix}
 * a1 & a2 & a3 \\
 * a4 & a5 & a6 \\
 * a7 & a8 & a9 \\
 * \end{bmatrix}
 *
 * C=\begin{bmatrix}
 * a1/s & a2/s & a3/s \\
 * a4/s & a5/s & a6/s \\
 * a7/s & a8/s & a9/s \\
 * \end{bmatrix}
 * \f]
 *
 * <h4>Ejemplo</h4>
 * \code
 * Matrix2x2f A;
 *
 * A.at(0, 0) = 1.f;
 * A.at(0, 1) = 4.f;
 * A.at(1, 0) = 3.f;
 * A.at(1, 1) = 2.f;
 *
 * float s = 2;
 * Matrix2x2f C = A / s;
 * \endcode
 */
template<size_t _rows, size_t _cols, typename T> static
Matrix<_rows, _cols, T> operator / (const Matrix<_rows, _cols, T> &matrix, T scalar)
{
  Matrix<_rows, _cols, T> _matrix = matrix;
  return _matrix /= scalar;
}

template<size_t _rows, size_t _cols, typename T> static
Matrix<_rows, _cols, T> &operator /= (Matrix<_rows, _cols, T> &matrix, T scalar)
{
  if (scalar != static_cast<T>(0)) {
    for (int r = 0; r < _rows; r++) {
      for (int c = 0; c < _cols; c++) {
        matrix.at(r, c) /= scalar;
      }
    }
  } else {
    matrix = Matrix<_rows, _cols, T>::zero();
  }
  return matrix;
}


/*! \} */ // end of Algebra

/*! \} */ // end of Math

} // Fin namespace math


} // End namespace tl

#endif // TL_MATH_MATRIX_H