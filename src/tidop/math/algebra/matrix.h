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

#include "tidop/core/exception.h"
#include "tidop/core/utils.h"
#include "tidop/math/algebra/vector.h"

namespace tl
{

namespace math
{

//constexpr auto DYN_MATRIX = -1;
constexpr auto DYN_MATRIX = std::numeric_limits<size_t>().max();

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
   * \brief Forma escalonada de fila
   */
  Matrix rowEchelonForm(T *determinant = nullptr) const;

  /*!
   * \brief Forma escalonada de fila reducida
   */
  //Matrix reducedRowEchelonForm(T *determinant = nullptr) const;

  /*!
   * \brief Determinante de la matriz
   * \return Determinante
   */
  T determinant() const;

  T trace() const;

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
  Matrix adjoint2x2() const;
  Matrix adjoint3x3() const;
  Matrix adjoint4x4() const;
  Matrix adjointnxn() const;

public:

  const size_t rows = _rows;
  const size_t cols = _cols;

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
  //: rows(_rows),
  //  cols(_cols)
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
  : mMatrix(mat.mMatrix)/*,
    rows(_rows),
    cols(_cols)*/
{
}

template<size_t _rows, size_t _cols, typename T>
Matrix<_rows, _cols, T>::Matrix(Matrix &&mat) TL_NOEXCEPT
  : mMatrix(std::forward<std::array<std::array<T, _cols>, _rows>>(mat.mMatrix))/*,
    rows(mat.rows),
    cols(mat.cols)*/
{
}


template<size_t _rows, size_t _cols, typename T>
Matrix<_rows, _cols, T>::Matrix(const std::array<std::array<T, _cols>, _rows> &mat)
  : mMatrix(mat)/*,
    rows(_rows),
    cols(_cols)*/
{
}

template<size_t _rows, size_t _cols, typename T> inline
Matrix<_rows, _cols, T>::Matrix(std::initializer_list<std::initializer_list<T>> mat)
  //: rows(_rows),
  //  cols(_cols)
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

template<size_t _rows, size_t _cols, typename T>
Matrix<_rows, _cols, T>::~Matrix()
{
}

template<size_t _rows, size_t _cols, typename T>
Matrix<_rows, _cols, T> &Matrix<_rows, _cols, T>::operator = (const Matrix& rot)
{
  if (this != &rot) {
    this->mMatrix = rot.mMatrix;
    //this->rows = rot.rows;
    //this->cols = rot.cols;
  }
  return *this;
}

template<size_t _rows, size_t _cols, typename T>
Matrix<_rows, _cols, T> &Matrix<_rows, _cols, T>::operator = (Matrix &&rot) TL_NOEXCEPT
{
  if (this != &rot) {
    this->mMatrix = std::forward<std::array<std::array<T, _cols>, _rows>>(rot.mMatrix);
    //this->rows = rot.rows;
    //this->cols = rot.cols;
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

  Matrix<_rows, _cols, T> adj = this->adjoint3x3();
  T det = mMatrix[0][0] * adj.at(0, 0) + mMatrix[0][1] * adj.at(1, 0) + mMatrix[0][2] * adj.at(2, 0);

  if (det != static_cast<T>(0)) {

    matrix = adj / det;

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
  T b5 = m22 * m33 - m23 * m32;
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
  if (det != static_cast<T>(0)) {
    Matrix<_rows, _cols, T> adjoint = this->adjugate();
    matrix = adjoint / det;
  } else {
    if (invertibility != nullptr) *invertibility = false;
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

  Matrix<_cols, _rows, T> matrix;
  if (mMatrix.size() == 2) {
    matrix = adjoint2x2();
  } else if (mMatrix.size() == 3) {
    matrix = adjoint3x3();
  } else if (mMatrix.size() == 4) {
    matrix = adjoint4x4();
  } else {
    matrix = adjointnxn();
  }

  return matrix;
}

template<size_t _rows, size_t _cols, typename T>
Matrix<_rows, _cols, T> Matrix<_rows, _cols, T>::adjoint2x2() const
{
  Matrix<_rows, _cols, T> matrix;
  matrix.at(0, 0) = mMatrix[1][1];
  matrix.at(0, 1) = -mMatrix[0][1];
  matrix.at(1, 0) = -mMatrix[1][0];
  matrix.at(1, 1) = mMatrix[0][0];
  return matrix;
}

template<size_t _rows, size_t _cols, typename T>
Matrix<_rows, _cols, T> Matrix<_rows, _cols, T>::adjoint3x3() const
{
  Matrix<_rows, _cols, T> matrix;
  matrix.at(0, 0) = mMatrix[1][1] * mMatrix[2][2] - mMatrix[1][2] * mMatrix[2][1];
  matrix.at(0, 1) = mMatrix[0][2] * mMatrix[2][1] - mMatrix[0][1] * mMatrix[2][2];
  matrix.at(0, 2) = mMatrix[0][1] * mMatrix[1][2] - mMatrix[0][2] * mMatrix[1][1];
  matrix.at(1, 0) = mMatrix[1][2] * mMatrix[2][0] - mMatrix[1][0] * mMatrix[2][2];
  matrix.at(1, 1) = mMatrix[0][0] * mMatrix[2][2] - mMatrix[0][2] * mMatrix[2][0];
  matrix.at(1, 2) = mMatrix[0][2] * mMatrix[1][0] - mMatrix[0][0] * mMatrix[1][2];
  matrix.at(2, 0) = mMatrix[1][0] * mMatrix[2][1] - mMatrix[1][1] * mMatrix[2][0];
  matrix.at(2, 1) = mMatrix[0][1] * mMatrix[2][0] - mMatrix[0][0] * mMatrix[2][1];
  matrix.at(2, 2) = mMatrix[0][0] * mMatrix[1][1] - mMatrix[0][1] * mMatrix[1][0];
  return matrix;
}

template<size_t _rows, size_t _cols, typename T>
Matrix<_rows, _cols, T> Matrix<_rows, _cols, T>::adjoint4x4() const
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
  T b5 = m22 * m33 - m23 * m32;

  matrix.at(0, 0) =  m11 * b5 - m12 * b4 + m13 * b3;
  matrix.at(0, 1) = -m01 * b5 + m02 * b4 - m03 * b3;
  matrix.at(0, 2) =  m31 * a5 - m32 * a4 + m33 * a3;
  matrix.at(0, 3) = -m21 * a5 + m22 * a4 - m23 * a3;
  matrix.at(1, 0) = -m10 * b5 + m12 * b2 - m13 * b1;
  matrix.at(1, 1) =  m00 * b5 - m02 * b2 + m03 * b1;
  matrix.at(1, 2) = -m30 * a5 + m32 * a2 - m33 * a1;
  matrix.at(1, 3) =  m20 * a5 - m22 * a2 + m23 * a1;
  matrix.at(2, 0) =  m10 * b4 - m11 * b2 + m13 * b0;
  matrix.at(2, 1) = -m00 * b4 + m01 * b2 - m03 * b0;
  matrix.at(2, 2) =  m30 * a4 - m31 * a2 + m33 * a0;
  matrix.at(2, 3) = -m20 * a4 + m21 * a2 - m23 * a0;
  matrix.at(3, 0) = -m10 * b3 + m11 * b1 - m12 * b0;
  matrix.at(3, 1) =  m00 * b3 - m01 * b1 + m02 * b0;
  matrix.at(3, 2) = -m30 * a3 + m31 * a1 - m32 * a0;
  matrix.at(3, 3) =  m20 * a3 - m21 * a1 + m22 * a0;

  return matrix;
}

template<size_t _rows, size_t _cols, typename T>
Matrix<_rows, _cols, T> Matrix<_rows, _cols, T>::adjointnxn() const
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
Matrix<_rows, _cols, T> Matrix<_rows, _cols, T>::rowEchelonForm(T *determinant) const
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
      d = zero;
      break;
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
        
  if (determinant) {
    *determinant = d;
  }

  return matrix;
}

//template<size_t _rows, size_t _cols, typename T> 
//Matrix<_rows, _cols, T> Matrix<_rows, _cols, T>::reducedRowEchelonForm(T *determinant) const
//{
//  const T zero{0};
//  const T one{1};
//  T d = one;
//  size_t n = mMatrix.size();
//  std::array<std::array<T, _cols>, _rows> matrix = this->rowEchelonForm(determinant);
//
//  return matrix;
//}

template<size_t _rows, size_t _cols, typename T> 
T Matrix<_rows, _cols, T>::determinant() const
{
  static_assert(_rows == _cols, "Non-Square Matrix");

  T d = static_cast<T>(1);
  size_t size = mMatrix.size();

  if (size == 2) {
    d = determinant2x2();
  } else if (size == 3) {
    d = determinant3x3();
  } else if (size == 4) {
    d = determinant4x4();
  } else {
    d = determinantnxn();
  }

  return d;
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


template<size_t _rows, size_t _cols, typename T> inline
T Matrix<_rows, _cols, T>::trace() const
{
  static_assert(_rows == _cols, "Non-Square Matrix");

  T trace;
  for (size_t i = 0; r < mMatrix.size(); i++) {
    trace += mMatrix[i][i];
  }
  return trace;
}

template<size_t _rows, size_t _cols, typename T> inline
bool Matrix<_rows, _cols, T>::invertible()
{
  T det = determinant();
  if (det == static_cast<T>(0)) return false;
  else return true;
}

template<size_t _rows, size_t _cols, typename T> inline
bool Matrix<_rows, _cols, T>::singular()
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




/* Matriz dinámica */




/*!
 * \brief Clase matriz 
 *
 */
template<typename T>
class MatrixDyn
{

public:

  typedef T value_type;

public:

  /*!
   * \brief Constructora por defecto
   */
  MatrixDyn();

  /*!
   * \brief Constructora filas-columnas
   */
  MatrixDyn(size_t rows, size_t cols);

  /*!
   * \brief Constructora de copia
   * \param[in] mat Objeto MatrixDyn que se copia
   */
  MatrixDyn(const MatrixDyn &mat);

  /*!
   * \brief Constructor de movimiento
   * \param[in] mat Objeto MatrixDyn que se mueve
   */
  MatrixDyn(MatrixDyn &&mat) TL_NOEXCEPT;
  
  MatrixDyn(std::initializer_list<std::initializer_list<T>> mat);

  /*!
   * \brief destructora
   */
  ~MatrixDyn();

  /*!
   * \brief Operador de asignación de copia
   * \param[in] mat Objeto que se copia
   */
  MatrixDyn &operator = (const MatrixDyn<T> &mat);
  
  /*!
   * \brief Operador de asignación de movimiento
   * \param[in] mat Objeto que se mueve
   */
  MatrixDyn &operator = (MatrixDyn<T> &&mat) TL_NOEXCEPT;

  /*!
   * \brief Referencia al elemento en la posición fila (r) y columna (c)
   * \param[in] r Fila de la matriz
   * \param[in] c Columna de la matriz
   * \return Valor de la matriz en la posición fila y columna
   * <h4>Ejemplo</h4>
   * \code
   * MatrixDyn<double> matrix(5, 5);
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
  size_t rows() const;

  /*!
   * \brief Número de columnas de la matriz
   * \return Número de columnas
   */
  size_t cols() const;

  void resize(size_t rows, size_t cols);

  /*!
   * \brief Matriz inversa
   * Una matriz cuadrada e invertible A tiene una matriz inversa \f[ A^{-1} \f] 
   * \param[out] invertibility Comprueba si la matriz es invertible
   * \return Matriz inversa
   * <h4>Ejemplo</h4>
   * MatrixDyn<double> mat_2x2(2, 2);
   * mat_2x2.at(0, 0) = 2.;
   * mat_2x2.at(0, 1) = 3.;
   * mat_2x2.at(1, 0) = 1.;
   * mat_2x2.at(1, 1) = 4.;
   * bool invertible;
   * MatrixDyn<double> inv_mat = mat_2x2.inverse(&invertible);
   * \endcode
   */
  MatrixDyn inverse(bool *invertibility = nullptr) const;

  /*!
   * \brief Calcula la matriz transpuesta
   * \return Matriz transpuesta
   */
  MatrixDyn<T> transpose() const;

  /*!
   * \brief Calcula la matriz de adjuntos
   * \f[ adj(A) = C^T \f]
   * \return Matriz de adjuntos
   */
  MatrixDyn adjugate() const;

  /*!
   * \brief Calcula la matriz cofactor
   * \return Matriz cofactor
   */
  MatrixDyn cofactorMatrix() const;

  /*!
   * \brief Forma escalonada de fila
   */
  MatrixDyn rowEchelonForm(T *determinant = nullptr) const;

  /*!
   * \brief Forma escalonada de fila reducida
   */
  //MatrixDyn reducedRowEchelonForm(T *determinant = nullptr) const;

  /*!
   * \brief Determinante de la matriz
   * \return Determinante
   */
  T determinant() const;

  T trace() const;

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
  static MatrixDyn zero(size_t rows, size_t cols);

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
  static MatrixDyn ones(size_t rows, size_t cols);

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
  static MatrixDyn identity(size_t rows, size_t cols);

private:

  T determinant2x2() const;
  T determinant3x3() const;
  T determinant4x4() const;
  T determinantnxn() const;
  MatrixDyn inverse2x2(bool *invertibility) const;
  MatrixDyn inverse3x3(bool *invertibility) const;
  MatrixDyn inverse4x4(bool *invertibility) const;
  MatrixDyn inversenxn(bool *invertibility) const;
  MatrixDyn adjoint2x2() const;
  MatrixDyn adjoint3x3() const;
  MatrixDyn adjoint4x4() const;
  MatrixDyn adjointnxn() const;

protected:

  std::vector<VectorDyn<T>> mMatrix;
};




/* Implementación MatrixDyn */

template<typename T>
MatrixDyn<T>::MatrixDyn()
{
}

template<typename T> inline
MatrixDyn<T>::MatrixDyn(size_t rows, size_t cols)
  : mMatrix(rows, VectorDyn<T>(cols, -std::numeric_limits<T>().max()))
{
}

template<typename T>
MatrixDyn<T>::MatrixDyn(const MatrixDyn &mat)
  : mMatrix(mat.mMatrix)
{
}

template<typename T>
MatrixDyn<T>::MatrixDyn(MatrixDyn &&mat) TL_NOEXCEPT
  : mMatrix(std::forward<std::vector<VectorDyn<T>>>(mat.mMatrix))
{
}

template<typename T> inline
MatrixDyn<T>::MatrixDyn(std::initializer_list<std::initializer_list<T>> mat)
{
  TL_TODO("ver por que peta con mas de 3 filas")
  size_t n_rows = mat.size();
  auto it_row = mat.begin();
  const T zero{0};
  for (size_t r = 0; r < this->rows(); r++) {
    if (r <= n_rows){
      auto it_col = it_row->begin();
      for (size_t c = 0; c < this->cols(); c++) {
        if (r <= n_rows){
          this->mMatrix[r][c] = *it_col++;
        } else{
          this->mMatrix[r][c] = zero;
        }
      }
      it_row++;
    } else{
      for (size_t c = 0; c < this->cols(); c++) {
        this->mMatrix[r][c] = zero;
      }
    }
  }

}

template<typename T>
MatrixDyn<T>::~MatrixDyn()
{
}

template<typename T>
MatrixDyn<T> &MatrixDyn<T>::operator = (const MatrixDyn& rot)
{
  if (this != &rot) {
    this->mMatrix = rot.mMatrix;
  }
  return *this;
}

template<typename T>
MatrixDyn<T> &MatrixDyn<T>::operator = (MatrixDyn &&rot) TL_NOEXCEPT
{
  if (this != &rot) {
    this->mMatrix = std::forward<std::vector<VectorDyn<T>>>(rot.mMatrix);
  }
  return *this;
}

template<typename T>
T &MatrixDyn<T>::at(size_t r, size_t c)
{
  return mMatrix[r][c];
}

template<typename T>
const T &MatrixDyn<T>::at(size_t r, size_t c) const
{
  return mMatrix[r][c];
}

template<typename T> inline
size_t MatrixDyn<T>::rows() const
{
  return mMatrix.size();
}

template<typename T> inline
size_t MatrixDyn<T>::cols() const
{
  if (mMatrix.empty()) {
    return 0;
  } else {
    VectorDyn<T> v = mMatrix[0];
    return v.size();
  }
}

template<typename T> inline
void MatrixDyn<T>::resize(size_t rows, size_t cols)
{
  mMatrix.resize(rows, VectorDyn<T>(cols));
}

template<typename T> 
MatrixDyn<T> MatrixDyn<T>::inverse(bool *invertibility) const
{
  size_t rows = this->rows();
  size_t cols = this->cols();
  TL_ASSERT(rows == cols, "Non-Square Matrix");
  MatrixDyn<T> matrix(rows, cols);

  if (rows == 2) {
    matrix = inverse2x2(invertibility);
  } else if (rows == 3) {
    matrix = inverse3x3(invertibility);
  } else if (rows == 4) {
    matrix = inverse4x4(invertibility);
  } else {
    matrix = inversenxn(invertibility);
  }

  return matrix;
}

template<typename T> 
MatrixDyn<T> MatrixDyn<T>::inverse2x2(bool *invertibility) const
{
  MatrixDyn<T> matrix(2, 2);
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

template<typename T> 
MatrixDyn<T> MatrixDyn<T>::inverse3x3(bool *invertibility) const
{
  MatrixDyn<T> matrix(3, 3);

  MatrixDyn<T> adj = this->adjoint3x3();
  T det = mMatrix[0][0] * adj.at(0, 0) + 
          mMatrix[0][1] * adj.at(1, 0) + 
          mMatrix[0][2] * adj.at(2, 0);

  if (det != static_cast<T>(0)) {

    matrix = adj / det;

    if (invertibility) *invertibility = true;
  } else {
    if (invertibility) *invertibility = false;
  }

  return matrix;
}

template<typename T> 
MatrixDyn<T> MatrixDyn<T>::inverse4x4(bool *invertibility) const
{
  MatrixDyn<T> matrix(4, 4);

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
  T b5 = m22 * m33 - m23 * m32;
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

template<typename T> 
MatrixDyn<T> MatrixDyn<T>::inversenxn(bool *invertibility) const
{
  MatrixDyn<T> matrix(this->rows(), this->cols());

  T det = determinantnxn();
  if (det != static_cast<T>(0)) {
    MatrixDyn<T> adjoint = this->adjugate();
    matrix = adjoint / det;
  } else {
    if (invertibility != nullptr) *invertibility = false;
  }

  return matrix;
}

template<typename T> 
MatrixDyn<T> MatrixDyn<T>::transpose() const
{
  size_t rows = this->rows();
  size_t cols = this->cols();

  MatrixDyn<T> matrix(cols, rows);
  for (size_t r = 0; r < rows; r++) {
    for (size_t c = 0; c < cols; c++) {
      matrix.at(c, r) = mMatrix[r][c];
    }
  }
  return matrix;
}

template<typename T>
MatrixDyn<T> MatrixDyn<T>::adjugate() const
{
  size_t rows = this->rows();
  size_t cols = this->cols();
  TL_ASSERT(rows == cols, "Non-Square Matrix");

  MatrixDyn<T> matrix(rows, cols);
  if (mMatrix.size() == 2) {
    matrix = adjoint2x2();
  } else if (mMatrix.size() == 3) {
    matrix = adjoint3x3();
  } else if (mMatrix.size() == 4) {
    matrix = adjoint4x4();
  } else {
    matrix = adjointnxn();
  }

  return matrix;
}

template<typename T>
MatrixDyn<T> MatrixDyn<T>::adjoint2x2() const
{
  MatrixDyn<T> matrix(2, 2);
  matrix.at(0, 0) = mMatrix[1][1];
  matrix.at(0, 1) = -mMatrix[0][1];
  matrix.at(1, 0) = -mMatrix[1][0];
  matrix.at(1, 1) = mMatrix[0][0];
  return matrix;
}

template<typename T>
MatrixDyn<T> MatrixDyn<T>::adjoint3x3() const
{
  MatrixDyn<T> matrix(3, 3);
  matrix.at(0, 0) = mMatrix[1][1] * mMatrix[2][2] - mMatrix[1][2] * mMatrix[2][1];
  matrix.at(0, 1) = mMatrix[0][2] * mMatrix[2][1] - mMatrix[0][1] * mMatrix[2][2];
  matrix.at(0, 2) = mMatrix[0][1] * mMatrix[1][2] - mMatrix[0][2] * mMatrix[1][1];
  matrix.at(1, 0) = mMatrix[1][2] * mMatrix[2][0] - mMatrix[1][0] * mMatrix[2][2];
  matrix.at(1, 1) = mMatrix[0][0] * mMatrix[2][2] - mMatrix[0][2] * mMatrix[2][0];
  matrix.at(1, 2) = mMatrix[0][2] * mMatrix[1][0] - mMatrix[0][0] * mMatrix[1][2];
  matrix.at(2, 0) = mMatrix[1][0] * mMatrix[2][1] - mMatrix[1][1] * mMatrix[2][0];
  matrix.at(2, 1) = mMatrix[0][1] * mMatrix[2][0] - mMatrix[0][0] * mMatrix[2][1];
  matrix.at(2, 2) = mMatrix[0][0] * mMatrix[1][1] - mMatrix[0][1] * mMatrix[1][0];
  return matrix;
}

template<typename T>
MatrixDyn<T> MatrixDyn<T>::adjoint4x4() const
{
  MatrixDyn<T> matrix(4, 4);

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
  T b5 = m22 * m33 - m23 * m32;

  matrix.at(0, 0) =  m11 * b5 - m12 * b4 + m13 * b3;
  matrix.at(0, 1) = -m01 * b5 + m02 * b4 - m03 * b3;
  matrix.at(0, 2) =  m31 * a5 - m32 * a4 + m33 * a3;
  matrix.at(0, 3) = -m21 * a5 + m22 * a4 - m23 * a3;
  matrix.at(1, 0) = -m10 * b5 + m12 * b2 - m13 * b1;
  matrix.at(1, 1) =  m00 * b5 - m02 * b2 + m03 * b1;
  matrix.at(1, 2) = -m30 * a5 + m32 * a2 - m33 * a1;
  matrix.at(1, 3) =  m20 * a5 - m22 * a2 + m23 * a1;
  matrix.at(2, 0) =  m10 * b4 - m11 * b2 + m13 * b0;
  matrix.at(2, 1) = -m00 * b4 + m01 * b2 - m03 * b0;
  matrix.at(2, 2) =  m30 * a4 - m31 * a2 + m33 * a0;
  matrix.at(2, 3) = -m20 * a4 + m21 * a2 - m23 * a0;
  matrix.at(3, 0) = -m10 * b3 + m11 * b1 - m12 * b0;
  matrix.at(3, 1) =  m00 * b3 - m01 * b1 + m02 * b0;
  matrix.at(3, 2) = -m30 * a3 + m31 * a1 - m32 * a0;
  matrix.at(3, 3) =  m20 * a3 - m21 * a1 + m22 * a0;

  return matrix;
}

template<typename T>
MatrixDyn<T> MatrixDyn<T>::adjointnxn() const
{
  TL_ASSERT(this->rows() == this->cols(), "Non-Square Matrix");

  MatrixDyn<T> matrix = this->cofactorMatrix();
  return matrix.transpose();
}

template<typename T> 
MatrixDyn<T> MatrixDyn<T>::cofactorMatrix() const
{
  size_t rows = this->rows();
  size_t cols = this->cols();
  TL_ASSERT(rows == cols, "Non-Square Matrix");

  MatrixDyn<T> matrix(rows, cols);
  for (size_t r = 0; r < rows; r++) {
    for (size_t c = 0; c < cols; c++) {
      matrix.at(r, c) = cofactor(r, c);
    }
  }
  return matrix;
}

template<typename T> 
MatrixDyn<T> MatrixDyn<T>::rowEchelonForm(T *determinant) const
{
  const T zero{0};
  const T one{1};
  T d = one;
  size_t n = mMatrix.size();
  //std::array<std::array<T, _cols>, _rows> matrix(mMatrix);
  std::vector<VectorDyn<T>> matrix(mMatrix);

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
      d = zero;
      break;
    }

    if (pivotRow != i) {
      std::swat(matrix[i], matrix[pivotRow]);
      d *= -one;
    }

    d *= pivotElement;

    for (size_t r = i + 1; r < n; ++r) {
      for (size_t c = i + 1; c < n; ++c) {
        matrix[r][c] -= matrix[r][i] * matrix[i][c] / pivotElement;
      }
    }
  }
        
  if (determinant) {
    *determinant = d;
  }

  return matrix;
}

//template<typename T> 
//MatrixDyn<T> MatrixDyn<T>::reducedRowEchelonForm(T *determinant) const
//{
//  const T zero{0};
//  const T one{1};
//  T d = one;
//  size_t n = mMatrix.size();
//  std::array<std::array<T, _cols>, _rows> matrix = this->rowEchelonForm(determinant);
//
//  return matrix;
//}

template<typename T> 
T MatrixDyn<T>::determinant() const
{
  TL_ASSERT(this->rows() == this->cols(), "Non-Square Matrix");

  T d = static_cast<T>(1);
  size_t size = mMatrix.size();

  if (size == 2) {
    d = determinant2x2();
  } else if (size == 3) {
    d = determinant3x3();
  } else if (size == 4) {
    d = determinant4x4();
  } else {
    d = determinantnxn();
  }

  return d;
}

template<typename T> 
T MatrixDyn<T>::determinant2x2() const
{
  T det = mMatrix[0][0] * mMatrix[1][1] - mMatrix[0][1] * mMatrix[1][0];
  return det;
}

template<typename T> 
T MatrixDyn<T>::determinant3x3() const
{
  T c00 = mMatrix[1][1] * mMatrix[2][2] - mMatrix[1][2] * mMatrix[2][1];
  T c10 = mMatrix[1][2] * mMatrix[2][0] - mMatrix[1][0] * mMatrix[2][2];
  T c20 = mMatrix[1][0] * mMatrix[2][1] - mMatrix[1][1] * mMatrix[2][0];
  T det = mMatrix[0][0] * c00 + mMatrix[0][1] * c10 + mMatrix[0][2] * c20;
  return det;
}

template<typename T> 
T MatrixDyn<T>::determinant4x4() const
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

template<typename T> 
T MatrixDyn<T>::determinantnxn() const
{
  const T zero{0};
  const T one{1};
  T d = one;
  size_t n = mMatrix.size();
  std::vector<VectorDyn<T>> matrix(mMatrix);

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
      std::swap(matrix[i], matrix[pivotRow]);
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


template<typename T> inline
T MatrixDyn<T>::trace() const
{
  //static_assert(_rows == _cols, "Non-Square Matrix");
  TL_ASSERT(this->rows() == this->cols(), "Non-Square Matrix");

  T trace;
  for (size_t i = 0; r < mMatrix.size(); i++) {
    trace += mMatrix[i][i];
  }
  return trace;
}

template<typename T> inline
bool MatrixDyn<T>::invertible()
{
  T det = determinant();
  if (det == static_cast<T>(0)) return false;
  else return true;
}

template<typename T> inline
bool MatrixDyn<T>::singular()
{
  T det = determinant();
  if (det == static_cast<T>(0)) return true;
  else return false;
}

template<typename T> 
T MatrixDyn<T>::cofactor(size_t r, size_t c) const
{
  TL_ASSERT(this->rows() == this->cols(), "Non-Square Matrix");

  T sign = ((r + c) % 2 == 0) ? T{1} : -T{1};
  return sign * this->firstMinor(r, c);
}

template<typename T> 
T MatrixDyn<T>::firstMinor(size_t r, size_t c) const
{
  size_t _rows = this->rows();
  size_t _cols = this->cols();
  TL_ASSERT(_rows == _cols, "Non-Square Matrix");

  size_t i = 0;
  size_t j = 0;
  MatrixDyn<T> matrix(_rows-1, _cols-1);
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

template<typename T>
MatrixDyn<T> MatrixDyn<T>::zero(size_t rows, size_t cols)
{
  const T zero{0};
  MatrixDyn<T> matrix(rows, cols);
  for (size_t r = 0; r < rows; r++) {
    for (size_t c = 0; c < cols; c++) {
      matrix.at(r, c) = zero;
    }
  }
  return matrix;
  //return MatrixDyn<T>(rows, VectorDyn<T>(cols, static_cast<T>(0)));
}

template<typename T>
MatrixDyn<T> MatrixDyn<T>::ones(size_t rows, size_t cols)
{
  const T one{1};
  MatrixDyn<T> matrix(rows, cols);
  for (size_t r = 0; r < rows; r++) {
    for (size_t c = 0; c < cols; c++) {
      matrix.at(r, c) = one;
    }
  }
  return matrix;
  //return MatrixDyn<T>(rows, VectorDyn<T>(cols, static_cast<T>(1)));
}

template<typename T>
MatrixDyn<T> MatrixDyn<T>::identity(size_t rows, size_t cols)
{
  const T zero{0}; 
  const T one{1};
  MatrixDyn<T> matrix(rows, cols);
  for (size_t r = 0; r < rows; r++) {
    for (size_t c = 0; c < cols; c++) {
      if (r == c) {
        matrix.at(r, c) = one;
      } else {
        matrix.at(r, c) = zero;
      }
    }
  }
  return matrix;
}


/* Operaciones unarias */

template<typename T>  static
MatrixDyn<T> operator + (const MatrixDyn<T> &matrix)
{
  return matrix;
}

template<typename T> static
MatrixDyn<T> operator - (const MatrixDyn<T> &matrix)
{
  size_t rows = matrix.rows();
  size_t cols = matrix.cols();
  MatrixDyn<T> _m(rows, cols);
  for (size_t r = 0; r < rows; r++) {
    for (size_t c = 0; c < cols; c++) {
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
template<typename T> static
MatrixDyn<T> operator + (const MatrixDyn<T> &matrix1,
                         const MatrixDyn<T> &matrix2)
{
  MatrixDyn<T> matrix = matrix1;
  return matrix += matrix2;
}

template<typename T> static
MatrixDyn<T> &operator += (MatrixDyn<T> &matrix1,
                           const MatrixDyn<T> &matrix2)
{
  for (int r = 0; r < matrix1.rows(); r++) {
    for (int c = 0; c < matrix1.cols(); c++) {
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
template<typename T> static
MatrixDyn<T> operator - (const MatrixDyn<T> &matrix1,
                                    const MatrixDyn<T> &matrix2)
{
  MatrixDyn<T> matrix = matrix1;
  return matrix -= matrix2;
}

template<typename T> static
MatrixDyn<T> &operator -= (MatrixDyn<T> &matrix1,
                           const MatrixDyn<T> &matrix2)
{
  for (int r = 0; r < matrix1.rows(); r++) {
    for (int c = 0; c < matrix1.cols(); c++) {
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
template<typename T>  static
MatrixDyn<T> operator * (const MatrixDyn<T> &matrix1,
                         const MatrixDyn<T> &matrix2)
{
  size_t rows = matrix1.rows();
  size_t dim1 = matrix1.cols();
  size_t cols = matrix2.cols();
  size_t dim2 = matrix2.rows();
  TL_ASSERT(dim1 == dim2, "A columns != B rows");

  MatrixDyn<T> matrix = MatrixDyn<T>::zero(rows, cols);
  for (size_t r = 0; r < rows; r++) {
    for (size_t c = 0; c < cols; c++) {
      for (size_t i = 0; i < dim1; i++) {
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
template<typename T> static
MatrixDyn<T> operator * (const MatrixDyn<T> &matrix, T scalar)
{
  MatrixDyn<T> _matrix = matrix;
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
template<typename T> static
MatrixDyn<T> operator * (T scalar, const MatrixDyn<T> &matrix)
{
  MatrixDyn<T> _matrix = matrix;
  return _matrix *= scalar;
}

template<typename T> static
MatrixDyn<T> &operator *= (MatrixDyn<T> &matrix, T scalar)
{
  for (int r = 0; r < matrix.rows(); r++) {
    for (int c = 0; c < matrix.cols(); c++) {
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
template<typename T> static
MatrixDyn<T> operator / (const MatrixDyn<T> &matrix, T scalar)
{
  MatrixDyn<T> _matrix = matrix;
  return _matrix /= scalar;
}

template<typename T> static
MatrixDyn<T> &operator /= (MatrixDyn<T> &matrix, T scalar)
{
  if (scalar != static_cast<T>(0)) {
    for (int r = 0; r < matrix.rows(); r++) {
      for (int c = 0; c < matrix.cols(); c++) {
        matrix.at(r, c) /= scalar;
      }
    }
  } else {
    matrix = MatrixDyn<T>::zero(matrix.rows(), matrix.cols());
  }
  return matrix;
}











//////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////


template<typename T, size_t _rows, size_t _cols>
class MatrixBase;

template<typename T, size_t _rows, size_t _cols>
class MatrixBase
{

public:

  using value_type      = T;
  using size_type       = size_t;
  using pointer         = T*;
  using const_pointer   = const T*;
  using reference       = T&;
  using const_reference = const T&;

public:

  MatrixBase();
  MatrixBase(size_t rows, size_t cols);
  MatrixBase(size_t rows, size_t cols, T value);
  MatrixBase(const MatrixBase &matrix);
  MatrixBase(MatrixBase &&matrix) TL_NOEXCEPT;
  //MatrixBase(std::initializer_list<std::initializer_list<T>> matrix);
  virtual ~MatrixBase() {}

  MatrixBase &operator = (const MatrixBase &matrix);
  MatrixBase &operator = (MatrixBase &&matrix) TL_NOEXCEPT;

  /*!
   * \brief Referencia al elemento en la posición fila (r) y columna (c)
   * \param[in] r Fila de la matriz
   * \param[in] c Columna de la matriz
   * \return Valor de la matriz en la posición fila y columna
   * <h4>Ejemplo</h4>
   * \code
   * Matrix<double,3,3> matrix;
   * matrix.at(0, 0) = 1.5;
   * double value = matrix.at(0, 0);
   * \endcode
   */
  reference at(size_t r, size_t c);

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
  const_reference at(size_t r, size_t c) const;

  //value_type operator()(size_t r, size_t c) const;

  //void setValue(size_t r, size_t c, T value)
  //{
  //  mData[r * this->cols() + c] = value;
  //}

  /*!
   * \brief Número de filas de la matriz
   * \return Número de filas
   */
  size_t rows() const;

  /*!
   * \brief Número de columnas de la matriz
   * \return Número de columnas
   */
  size_t cols() const;

  void swapRows(size_t i, size_t j);

  //pointer data()
  //{
  //  return mData.data();
  //}
    
  //const_pointer data() const
  //{
  //  return mData.data();
  //}

private:

  //std::array<std::array<T, _cols>, _rows> mData;
  std::array<T, _rows * _cols> mData;
};


template<typename T, size_t _rows, size_t _cols> inline 
MatrixBase<T, _rows, _cols>::MatrixBase()
{
  //T ini_val = -std::numeric_limits<T>().max();
  //for (size_t r = 0; r < _rows; r++) {
  //  for (size_t c = 0; c < _cols; c++) {
  //    this->mData[r][c] = ini_val;
  //  }
  //}
  T ini_val = -std::numeric_limits<T>().max();
  for (size_t r = 0; r < _rows * _cols; r++) {
    this->mData[r] = ini_val;
  }
}

template<typename T, size_t _rows, size_t _cols> inline
MatrixBase<T, _rows, _cols>::MatrixBase(size_t rows, size_t cols)
{
  static_assert(_rows == -1 && _cols == -1, "Dynamic Matrix not support resize");
}

template<typename T, size_t _rows, size_t _cols> inline 
MatrixBase<T, _rows, _cols>::MatrixBase(size_t rows, size_t cols, T val)
{
  static_assert(_rows == -1 && _cols == -1, "Dynamic Matrix not support resize");
}

template<typename T, size_t _rows, size_t _cols> inline 
MatrixBase<T, _rows, _cols>::MatrixBase(const MatrixBase &matrix)
  : mData(std::move(matrix.mData))
{
}

template<typename T, size_t _rows, size_t _cols> inline 
MatrixBase<T, _rows, _cols>::MatrixBase(MatrixBase &&matrix) TL_NOEXCEPT
  : mData(std::move(matrix.mData))
{
}

template<typename T, size_t _rows, size_t _cols> inline 
MatrixBase<T, _rows, _cols> &MatrixBase<T, _rows, _cols>::operator = (const MatrixBase &matrix)
{
  if (this != &matrix) {
    this->mData = matrix.mData;
  }
  return *this;
}

template<typename T, size_t _rows, size_t _cols> inline 
MatrixBase<T, _rows, _cols> &MatrixBase<T, _rows, _cols>::operator = (MatrixBase &&matrix) TL_NOEXCEPT
{
  if (this != &matrix) {
    this->mData = std::move(matrix.mData);
  }
  return *this;
}

template<typename T, size_t _rows, size_t _cols> inline 
T &MatrixBase<T, _rows, _cols>::at(size_t r, size_t c)
{
  //return mData[r][c];
  return mData[r * this->cols() + c];
}

template<typename T, size_t _rows, size_t _cols> inline 
const T &MatrixBase<T, _rows, _cols>::at(size_t r, size_t c) const
{
  //return mData[r][c];
  return mData[r * this->cols() + c];
}

//template<typename T, size_t _rows, size_t _cols> inline 
//T MatrixBase<T, _rows, _cols>::operator()(size_t r, size_t c) const
//{
//  return mData[r * this->cols() + c];
//}

template<typename T, size_t _rows, size_t _cols> inline 
size_t MatrixBase<T, _rows, _cols>::rows() const
{
  return _rows;// mData.size();
}

template<typename T, size_t _rows, size_t _cols> inline 
size_t MatrixBase<T, _rows, _cols>::cols() const
{
  //if (mData.empty()) {
  //  return 0;
  //} else {
  //  return mData[0].size();
  //}
  return _cols;
}

template<typename T, size_t _rows, size_t _cols> inline 
void MatrixBase<T, _rows, _cols>::swapRows(size_t i, size_t j)
{
  //mData[i].swap(mData[j]);
  for (size_t c = 0; c < _cols; c++) {
    std::swap(mData[i* _cols + c], mData[j * _cols + c]);
  }
}





//template<typename T>
//class MatrixBase<T, DYN_MATRIX, DYN_MATRIX>
//{
//
//public:
//
//  using value_type      = T;
//  using size_type       = size_t;
//  using pointer         = T*;
//  using const_pointer   = const T*;
//  using reference       = T&;
//  using const_reference = const T&;
//
//public:
//
//  MatrixBase();
//  MatrixBase(size_t rows, size_t cols);
//  MatrixBase(size_t rows, size_t cols, T val);
//  MatrixBase(const MatrixBase &matrix);
//  MatrixBase(MatrixBase &&matrix) TL_NOEXCEPT;
//  //MatrixBase(std::initializer_list<std::initializer_list<T>> matrix);
//  virtual ~MatrixBase() {}
//
//  MatrixBase &operator = (const MatrixBase &matrix);
//  MatrixBase &operator = (MatrixBase &&matrix) TL_NOEXCEPT;
//
//  reference at(size_t r, size_t c);
//  const_reference at(size_t r, size_t c) const;
//
//  //value_type operator()(size_t r, size_t c) const;
//
//  //void setValue(size_t r, size_t c, T value)
//  //{
//  //  mData[r * this->cols() + c] = value;
//  //}
//
//  size_t rows() const;
//  size_t cols() const;
//
//  void swapRows(size_t i, size_t j);
//
//  //pointer data()
//  //{
//  //  return mData.data();
//  //}
//  // 
//  //const_pointer data() const
//  //{
//  //  return mData.data();
//  //}
//
//private:
//
//  //std::vector<std::vector<T>> mData;
//  std::vector<T> mData;
//  size_t mRows;
//  size_t mCols;
//};
//
//
//template<typename T> inline 
//MatrixBase<T, DYN_MATRIX, DYN_MATRIX>::MatrixBase()
//  : mRows(0),
//    mCols(0)
//{
//}
//
//template<typename T> inline
//MatrixBase<T, DYN_MATRIX, DYN_MATRIX>::MatrixBase(size_t rows, size_t cols)
//  //: mData(rows, std::vector<T>(cols, -std::numeric_limits<T>().max()))
//  : mData(rows * cols, -std::numeric_limits<T>().max()),
//    mRows(rows),
//    mCols(cols)
//{
//}
//
//template<typename T> inline
//MatrixBase<T, DYN_MATRIX, DYN_MATRIX>::MatrixBase(size_t rows, size_t cols, T val)
//  //: mData(rows, std::vector<T>(cols, val))
//  : mData(rows * cols, val),
//    mRows(rows),
//    mCols(cols)
//{
//}
//
//template<typename T> inline 
//MatrixBase<T, DYN_MATRIX, DYN_MATRIX>::MatrixBase(const MatrixBase &matrix)
//  : mData(matrix.mData), 
//    mRows(matrix.mRows),
//    mCols(matrix.mCols)
//{
//}
//
//template<typename T> inline 
//MatrixBase<T, DYN_MATRIX, DYN_MATRIX>::MatrixBase(MatrixBase &&matrix) TL_NOEXCEPT
//  : mData(std::move(matrix.mData)), 
//    mRows(matrix.mRows),
//    mCols(matrix.mCols)
//{
//}
//
//template<typename T> inline 
//MatrixBase<T, DYN_MATRIX, DYN_MATRIX> &MatrixBase<T, DYN_MATRIX, DYN_MATRIX>::operator = (const MatrixBase<T, DYN_MATRIX, DYN_MATRIX> &matrix)
//{
//  if (this != &matrix) {
//    this->mData = matrix.mData;
//    this->mRows = matrix.mRows;
//    this->mCols = matrix.mCols;
//  }
//  return *this;
//}
//
//template<typename T> inline 
//MatrixBase<T, DYN_MATRIX, DYN_MATRIX> &MatrixBase<T, DYN_MATRIX, DYN_MATRIX>::operator = (MatrixBase<T, DYN_MATRIX, DYN_MATRIX> &&matrix) TL_NOEXCEPT
//{
//  if (this != &matrix) {
//    this->mData = std::move(matrix.mData);
//    this->mRows = matrix.mRows;
//    this->mCols = matrix.mCols;
//  }
//  return *this;
//}
//
//template<typename T> inline 
//T &MatrixBase<T, DYN_MATRIX, DYN_MATRIX>::at(size_t r, size_t c)
//{
//  //return mData[r][c];
//  return mData[r * this->cols() + c];
//}
//
//template<typename T> inline 
//const T &MatrixBase<T, DYN_MATRIX, DYN_MATRIX>::at(size_t r, size_t c) const
//{
//  //return mData[r][c];
//  return mData[r * this->cols() + c];
//}
//
////template<typename T> inline 
////T MatrixBase<T, DYN_MATRIX, DYN_MATRIX>::operator()(size_t r, size_t c) const
////{
////  return mData[r * this->cols() + c];
////}
//
//template<typename T> inline 
//size_t MatrixBase<T, DYN_MATRIX, DYN_MATRIX>::rows() const
//{
//  //return mData.size();
//  return mRows;
//}
//
//template<typename T> inline 
//size_t MatrixBase<T, DYN_MATRIX, DYN_MATRIX>::cols() const
//{
//  //if (mData.empty()) {
//  //  return 0;
//  //} else {
//  //  return mData[0].size();
//  //}
//  return mCols;
//}
//
//template<typename T> inline 
//void MatrixBase<T, DYN_MATRIX, DYN_MATRIX>::swapRows(size_t i, size_t j)
//{
//  //mData[i].swap(mData[j]);
//  for (size_t c = 0; c < mCols; c++) {
//    std::swap(mData[i* this->cols() + c], mData[j * this->cols() + c]);
//  }
//}



////////////////////////////////////////////////////////////////////////////////////////////////



template<typename T>
class MatrixBase<T, DYN_MATRIX, DYN_MATRIX>
{

public:

  using value_type      = T;
  using size_type       = size_t;
  using pointer         = T*;
  using const_pointer   = const T*;
  using reference       = T&;
  using const_reference = const T&;

public:

  MatrixBase();
  MatrixBase(size_t rows, size_t cols);
  MatrixBase(size_t rows, size_t cols, T val);
  MatrixBase(const MatrixBase &matrix);
  MatrixBase(MatrixBase &&matrix) TL_NOEXCEPT;
  //MatrixBase(std::initializer_list<std::initializer_list<T>> matrix);
  virtual ~MatrixBase() {}

  MatrixBase &operator = (const MatrixBase &matrix);
  MatrixBase &operator = (MatrixBase &&matrix) TL_NOEXCEPT;

  reference at(size_t r, size_t c);
  const_reference at(size_t r, size_t c) const;

  size_t rows() const;
  size_t cols() const;

  void swapRows(size_t i, size_t j);

  pointer data()
  {
    return mData;
  }

private:

  //std::vector<std::vector<T>> mData;
  std::valarray<T> mData;
  size_t mRows;
  size_t mCols;

};


template<typename T> inline 
MatrixBase<T, DYN_MATRIX, DYN_MATRIX>::MatrixBase()
  : mRows(0),
    mCols(0)
{
}

template<typename T> inline
MatrixBase<T, DYN_MATRIX, DYN_MATRIX>::MatrixBase(size_t rows, size_t cols)
  //: mData(rows, std::vector<T>(cols, -std::numeric_limits<T>().max()))
  : mData(-std::numeric_limits<T>().max(), rows *cols),
    mRows(rows),
    mCols(cols)
{
}

template<typename T> inline
MatrixBase<T, DYN_MATRIX, DYN_MATRIX>::MatrixBase(size_t rows, size_t cols, T val)
  : mData(val, rows * cols), 
    mRows(rows),
    mCols(cols)
{
}

template<typename T> inline 
MatrixBase<T, DYN_MATRIX, DYN_MATRIX>::MatrixBase(const MatrixBase &matrix)
  : mData(matrix.mData), 
    mRows(matrix.mRows),
    mCols(matrix.mCols)
{
}

template<typename T> inline 
MatrixBase<T, DYN_MATRIX, DYN_MATRIX>::MatrixBase(MatrixBase &&matrix) TL_NOEXCEPT
  : mData(std::move(matrix.mData)), 
    mRows(matrix.mRows),
    mCols(matrix.mCols)
    
{
}

template<typename T> inline 
MatrixBase<T, DYN_MATRIX, DYN_MATRIX> &MatrixBase<T, DYN_MATRIX, DYN_MATRIX>::operator = (const MatrixBase<T, DYN_MATRIX, DYN_MATRIX> &matrix)
{
  if (this != &matrix) {
    this->mData = matrix.mData;
    this->mRows = matrix.mRows;
    this->mCols = matrix.mCols;
  }
  return *this;
}

template<typename T> inline 
MatrixBase<T, DYN_MATRIX, DYN_MATRIX> &MatrixBase<T, DYN_MATRIX, DYN_MATRIX>::operator = (MatrixBase<T, DYN_MATRIX, DYN_MATRIX> &&matrix) TL_NOEXCEPT
{
  if (this != &matrix) {
    this->mData = std::move(matrix.mData);
    this->mRows = matrix.mRows;
    this->mCols = matrix.mCols;
  }
  return *this;
}

template<typename T> inline 
T &MatrixBase<T, DYN_MATRIX, DYN_MATRIX>::at(size_t r, size_t c)
{
  return mData[r * this->cols() + c];
}

template<typename T> inline 
const T &MatrixBase<T, DYN_MATRIX, DYN_MATRIX>::at(size_t r, size_t c) const
{
  return mData[r * this->cols() + c];
}

template<typename T> inline 
size_t MatrixBase<T, DYN_MATRIX, DYN_MATRIX>::rows() const
{
  return mRows;
}

template<typename T> inline 
size_t MatrixBase<T, DYN_MATRIX, DYN_MATRIX>::cols() const
{
  return mCols;
}

template<typename T> inline 
void MatrixBase<T, std::numeric_limits<size_t>().max(), DYN_MATRIX>::swapRows(size_t i, size_t j)
{
  for (size_t c = 0; c < mCols; c++) {
    std::swap(mData[i* this->cols() + c], mData[j * this->cols() + c]);
  }
}



//////////////////////////////////////////////////////////////////////////////////////////////



//template<typename T>
//class MatrixBase<T, DYN_MATRIX, DYN_MATRIX>
//{
//
//public:
//
//  using value_type      = T;
//  using size_type       = size_t;
//  using pointer         = T*;
//  using const_pointer   = const T*;
//  using reference       = T&;
//  using const_reference = const T&;
//
//public:
//
//  MatrixBase();
//  MatrixBase(size_t rows, size_t cols);
//  MatrixBase(size_t rows, size_t cols, T val);
//  MatrixBase(const MatrixBase &matrix);
//  MatrixBase(MatrixBase &&matrix) TL_NOEXCEPT;
//  //MatrixBase(std::initializer_list<std::initializer_list<T>> matrix);
//  virtual ~MatrixBase() {}
//
//  MatrixBase &operator = (const MatrixBase &matrix);
//  MatrixBase &operator = (MatrixBase &&matrix) TL_NOEXCEPT;
//
//  reference at(size_t r, size_t c);
//  const_reference at(size_t r, size_t c) const;
//
//  size_t rows() const;
//  size_t cols() const;
//
//  void swapRows(size_t i, size_t j);
//
//  pointer data()
//  {
//    return mData;
//  }
//
//private:
//
//  //std::vector<std::vector<T>> mData;
//  //std::valarray<T> mData;
//  T *mData;
//  size_t mRows;
//  size_t mCols;
//
//};
//
//
//template<typename T> inline 
//MatrixBase<T, DYN_MATRIX, DYN_MATRIX>::MatrixBase()
//  : mData(nullptr),
//    mRows(0),
//    mCols(0)
//{
//}
//
//template<typename T> inline
//MatrixBase<T, DYN_MATRIX, DYN_MATRIX>::MatrixBase(size_t rows, size_t cols)
//  : mData(new T[rows * cols]),
//    mRows(rows),
//    mCols(cols)
//{
//}
//
//template<typename T> inline
//MatrixBase<T, DYN_MATRIX, DYN_MATRIX>::MatrixBase(size_t rows, size_t cols, T val)
//  : mRows(rows),
//    mCols(cols)
//{
//  mData = new T[rows * cols];
//  T ini_val = -std::numeric_limits<T>().max();
//  for (size_t r = 0; r < rows * cols; r++) {
//    this->mData[r] = ini_val;
//  }
//}
//
//template<typename T> inline 
//MatrixBase<T, DYN_MATRIX, DYN_MATRIX>::MatrixBase(const MatrixBase &matrix)
//  : mRows(matrix.mRows),
//    mCols(matrix.mCols)
//{
//  mData = new T[matrix.mRows * matrix.mCols];
//  *mData = *matrix.mData;
//  //for (size_t r = 0; r < _rows * _cols; r++) {
//  //  this->mData[r] = matrix.at(r);
//  //}
//}
//
//template<typename T> inline 
//MatrixBase<T, DYN_MATRIX, DYN_MATRIX>::MatrixBase(MatrixBase &&matrix) TL_NOEXCEPT
//  : mRows(matrix.mRows),
//    mCols(matrix.mCols)
//    
//{
//  mData = new T[matrix.mRows * matrix.mCols];
//  *this->mData = std::move(*matrix.mData);
//}
//
//template<typename T> inline 
//MatrixBase<T, DYN_MATRIX, DYN_MATRIX> &MatrixBase<T, DYN_MATRIX, DYN_MATRIX>::operator = (const MatrixBase<T, DYN_MATRIX, DYN_MATRIX> &matrix)
//{
//  if (this != &matrix) {
//    mData = new T[matrix.mRows * matrix.mCols];
//    *this->mData = *matrix.mData;
//    this->mRows = matrix.mRows;
//    this->mCols = matrix.mCols;
//  }
//  return *this;
//}
//
//template<typename T> inline 
//MatrixBase<T, DYN_MATRIX, DYN_MATRIX> &MatrixBase<T, DYN_MATRIX, DYN_MATRIX>::operator = (MatrixBase<T, DYN_MATRIX, DYN_MATRIX> &&matrix) TL_NOEXCEPT
//{
//  if (this != &matrix) {
//    mData = new T[matrix.mRows * matrix.mCols];
//    *mData = *matrix.mData;
//    //this->mData = std::move(matrix.mData);
//    this->mRows = matrix.mRows;
//    this->mCols = matrix.mCols;
//  }
//  return *this;
//}
//
//template<typename T> inline 
//T &MatrixBase<T, DYN_MATRIX, DYN_MATRIX>::at(size_t r, size_t c)
//{
//  return mData[r * this->cols() + c];
//}
//
//template<typename T> inline 
//const T &MatrixBase<T, DYN_MATRIX, DYN_MATRIX>::at(size_t r, size_t c) const
//{
//  return mData[r * this->cols() + c];
//}
//
//template<typename T> inline 
//size_t MatrixBase<T, DYN_MATRIX, DYN_MATRIX>::rows() const
//{
//  return mRows;
//}
//
//template<typename T> inline 
//size_t MatrixBase<T, DYN_MATRIX, DYN_MATRIX>::cols() const
//{
//  return mCols;
//}
//
//template<typename T> inline 
//void MatrixBase<T, std::numeric_limits<size_t>().max(), DYN_MATRIX>::swapRows(size_t i, size_t j)
//{
//  for (size_t c = 0; c < mCols; c++) {
//    std::swap(mData[i* this->cols() + c], mData[j * this->cols() + c]);
//  }
//}



//////////////////////////////////////////////////////////////////////////////////////////////




/*!
 * \brief Clase matriz 
 *
 */
template<typename T, size_t _rows = DYN_MATRIX, size_t _cols = DYN_MATRIX>
class Matrix2
  : public MatrixBase<T, _rows, _cols>
{

//public:
//
//  typedef T value_type;

public:

  /*!
   * \brief Constructora por defecto
   */
  Matrix2();

  /*!
   * \brief Constructora filas-columnas
   */
  Matrix2(size_t rows, size_t cols);

  /*!
   * \brief Constructora filas, columnas y valor
   */
  Matrix2(size_t rows, size_t cols, T val);

  /*!
   * \brief Constructora de copia
   * \param[in] mat Objeto Matrix que se copia
   */
  Matrix2(const Matrix2 &mat);

  /*!
   * \brief Constructor de movimiento
   * \param[in] mat Objeto Matrix que se mueve
   */
  Matrix2(Matrix2 &&mat) TL_NOEXCEPT;

  /*!
   * \brief Constructor
   * \param[in] mat Matriz
   */
  //Matrix2(const std::array<std::array<T, _cols>, _rows> &mat);
  
  //Matrix2(std::initializer_list<std::initializer_list<T>> mat);

  /*!
   * \brief destructora
   */
  ~Matrix2() override = default;

  /*!
   * \brief Operador de asignación de copia
   * \param[in] mat Objeto que se copia
   */
  Matrix2 &operator = (const Matrix2 &mat);
  
  /*!
   * \brief Operador de asignación de movimiento
   * \param[in] mat Objeto que se mueve
   */
  Matrix2 &operator = (Matrix2 &&mat) TL_NOEXCEPT;

  /*!
   * \brief Matriz inversa
   * Una matriz cuadrada e invertible A tiene una matriz inversa \f[ A^{-1} \f] 
   * \param[out] invertibility Comprueba si la matriz es invertible
   * \return Matriz inversa
   * <h4>Ejemplo</h4>
   * Matrix<double, 2, 2> mat_2x2;
   * mat_2x2.at(0, 0) = 2.;
   * mat_2x2.at(0, 1) = 3.;
   * mat_2x2.at(1, 0) = 1.;
   * mat_2x2.at(1, 1) = 4.;
   * bool invertible;
   * Matrix<double, 2, 2> inv_mat = mat_2x2.inverse(&invertible);
   * \endcode
   */
  Matrix2 inverse(bool *invertibility = nullptr) const;

  /*!
   * \brief Calcula la matriz transpuesta
   * \return Matriz transpuesta
   */
  Matrix2<T, _cols, _rows> transpose() const;

  /*!
   * \brief Calcula la matriz de adjuntos
   * \f[ adj(A) = C^T \f]
   * \return Matriz de adjuntos
   */
  Matrix2 adjugate() const;

  /*!
   * \brief Calcula la matriz cofactor
   * \return Matriz cofactor
   */
  Matrix2 cofactorMatrix() const;

  /*!
   * \brief Forma escalonada de fila
   */
  Matrix2 rowEchelonForm(T *determinant = nullptr) const;

  /*!
   * \brief Forma escalonada de fila reducida
   */
  //Matrix reducedRowEchelonForm(T *determinant = nullptr) const;

  /*!
   * \brief Determinante de la matriz
   * \return Determinante
   */
  T determinant() const;

  T trace() const;

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
  static Matrix2 zero();
  static Matrix2 zero(size_t row, size_t cols);

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
  static Matrix2 ones();
  static Matrix2 ones(size_t row, size_t cols);

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
  static Matrix2 identity();
  static Matrix2 identity(size_t row, size_t cols);

  //static Matrix2 transpose(const Matrix2 &matrix);

private:


  T determinant2x2() const;
  T determinant3x3() const;
  T determinant4x4() const;
  T determinantnxn() const;
  Matrix2 inverse2x2(bool *invertibility) const;
  Matrix2 inverse3x3(bool *invertibility) const;
  Matrix2 inverse4x4(bool *invertibility) const;
  Matrix2 inversenxn(bool *invertibility) const;
  Matrix2 adjoint2x2() const;
  Matrix2 adjoint3x3() const;
  Matrix2 adjoint4x4() const;
  Matrix2 adjointnxn() const;

  void adjoint2x2(Matrix2<T, _rows, _cols> &matrix) const;
  void adjoint3x3(Matrix2<T, _rows, _cols> &matrix) const;

//public:
//
//  const size_t rows = _rows;
//  const size_t cols = _cols;

protected:

  //std::array<std::array<T, _cols>, _rows> mMatrix;
 // size_t mRows;
 // size_t mCols;
};



/* Definición de alias Matrix */

//typedef Matrix<2,2,int>    Matrix2x2i;
//typedef Matrix<2,2,float>  Matrix2x2f;
//typedef Matrix<2,2,double> Matrix2x2d;
//typedef Matrix<3,3,int>    Matrix3x3i;
//typedef Matrix<3,3,float>  Matrix3x3f;
//typedef Matrix<3,3,double> Matrix3x3d;
//typedef Matrix<4,4,int>    Matrix4x4i;
//typedef Matrix<4,4,float>  Matrix4x4f;
//typedef Matrix<4,4,double> Matrix4x4d;


template<typename T, size_t _rows, size_t _cols>
Matrix2<T, _rows, _cols> matrixReserve(const Matrix2<T, _rows, _cols> &_matrix)
{
  return Matrix2<T, _cols, _rows>();
}

template<typename T>
Matrix2<T, DYN_MATRIX, DYN_MATRIX> matrixReserve(const Matrix2<T, DYN_MATRIX, DYN_MATRIX> &matrix)
{
  return Matrix2<T>(matrix.cols(), matrix.rows());
}


template<typename T, size_t _rows, size_t _cols>
Matrix2<T, _cols, _rows> transposeReserve(const Matrix2<T, _rows, _cols> &_matrix)
{
  Matrix2<T, _cols, _rows> matrix;
  return matrix;
}

template<typename T>
Matrix2<T, DYN_MATRIX, DYN_MATRIX> transposeReserve(const Matrix2<T, DYN_MATRIX, DYN_MATRIX> &matrix)
{
  Matrix2<T> _matrix = Matrix2<T>::zero(matrix.cols(), matrix.rows());
  return _matrix;
}



/* Implementación Matrix */

template<typename T, size_t _rows, size_t _cols> inline
Matrix2<T, _rows, _cols>::Matrix2()
{
}

template<typename T, size_t _rows, size_t _cols> inline
Matrix2<T, _rows, _cols>::Matrix2(size_t rows, size_t cols)
  : MatrixBase(rows, cols)
{
}

template<typename T, size_t _rows, size_t _cols> inline
Matrix2<T, _rows, _cols>::Matrix2(size_t rows, size_t cols, T val)
  : MatrixBase(rows, cols, val)
{
}

template<typename T, size_t _rows, size_t _cols> inline
Matrix2<T, _rows, _cols>::Matrix2(const Matrix2 &matrix)
  : MatrixBase(matrix)
{
}

template<typename T, size_t _rows, size_t _cols> inline
Matrix2<T, _rows, _cols>::Matrix2(Matrix2 &&matrix) TL_NOEXCEPT
  : MatrixBase(std::forward<MatrixBase<T, _rows, _cols>>(matrix))
{
}

//template<size_t _rows, size_t _cols, typename T>
//Matrix2<_rows, _cols, T>::Matrix2(const std::array<std::array<T, _cols>, _rows> &mat)
//  : mMatrix(mat)/*,
//    rows(_rows),
//    cols(_cols)*/
//{
//}
//
//template<size_t _rows, size_t _cols, typename T> inline
//Matrix2<_rows, _cols, T>::Matrix2(std::initializer_list<std::initializer_list<T>> mat)
//  //: rows(_rows),
//  //  cols(_cols)
//{
//  TL_TODO("ver por que peta con mas de 3 filas")
//  size_t n_rows = mat.size();
//  auto it_row = mat.begin();
//  const T zero{0};
//  for (size_t r = 0; r < this->rows; r++) {
//    if (r <= n_rows){
//      auto it_col = it_row->begin();
//      for (size_t c = 0; c < this->cols; c++) {
//        if (r <= n_rows){
//          this->mMatrix[r][c] = *it_col++;
//        } else{
//          this->mMatrix[r][c] = zero;
//        }
//      }
//      it_row++;
//    } else{
//      for (size_t c = 0; c < this->cols; c++) {
//        this->mMatrix[r][c] = zero;
//      }
//    }
//  }
//
//}

template<typename T, size_t _rows, size_t _cols> inline
Matrix2<T, _rows, _cols> &Matrix2<T, _rows, _cols>::operator = (const Matrix2 &matrix)
{
  if (this != &matrix) {
    MatrixBase::operator=(std::forward<MatrixBase<T, _rows, _cols>>(matrix));
  }
  return *this;
}

template<typename T, size_t _rows, size_t _cols> inline
Matrix2<T, _rows, _cols> &Matrix2<T, _rows, _cols>::operator = (Matrix2 &&matrix) TL_NOEXCEPT
{
  if (this != &matrix) {
    MatrixBase::operator=(std::forward<MatrixBase<T, _rows, _cols>>(matrix));
  }
  return *this;
}

//template<size_t _rows, size_t _cols, typename T>
//T &Matrix2<_rows, _cols, T>::at(size_t r, size_t c)
//{
//  return mMatrix[r][c];
//}
//
//template<size_t _rows, size_t _cols, typename T>
//const T &Matrix2<_rows, _cols, T>::at(size_t r, size_t c) const
//{
//  return mMatrix[r][c];
//}

template<typename T, size_t _rows, size_t _cols> inline
Matrix2<T, _rows, _cols> Matrix2<T, _rows, _cols>::inverse(bool *invertibility) const
{
  static_assert(_rows == _cols, "Non-Square Matrix");

  /// TODO: Pasar matrix como referencia para no llamar mas de una vez a la constructora

  Matrix2<T, _rows, _cols> matrix;
  
  size_t rows = this->rows();

  if (rows == 2) {
    matrix = inverse2x2(invertibility);
  } else if (rows == 3) {
    matrix = inverse3x3(invertibility);
  } else if (rows == 4) {
    matrix = inverse4x4(invertibility);
  } else {
    matrix = inversenxn(invertibility);
  }

  return matrix;
}

template<typename T, size_t _rows, size_t _cols> inline
Matrix2<T, _rows, _cols> Matrix2<T, _rows, _cols>::inverse2x2(bool *invertibility) const
{
  Matrix2<T, _rows, _cols> matrix(*this);
  T det = determinant2x2();
  if (det != static_cast<T>(0)) {
    matrix.at(0, 0) =  this->at(1,1) / det;
    matrix.at(0, 1) = -this->at(0,1) / det;
    matrix.at(1, 0) = -this->at(1,0) / det;
    matrix.at(1, 1) =  this->at(0,0) / det;
    if (invertibility) *invertibility = true;
  } else {
    if (invertibility) *invertibility = false;
  }
  return matrix;
}

template<typename T, size_t _rows, size_t _cols> inline
Matrix2<T, _rows, _cols> Matrix2<T, _rows, _cols>::inverse3x3(bool *invertibility) const
{
  Matrix2<T, _rows, _cols> matrix(*this);

  Matrix2<T, _rows, _cols> adj = this->adjoint3x3();
  T det = this->at(0,0) * adj.at(0, 0) + this->at(0,1) * adj.at(1, 0) + this->at(0,2) * adj.at(2, 0);

  if (det != static_cast<T>(0)) {

    matrix = adj / det;

    if (invertibility) *invertibility = true;
  } else {
    if (invertibility) *invertibility = false;
  }

  return matrix;
}

template<typename T, size_t _rows, size_t _cols> inline 
Matrix2<T, _rows, _cols> Matrix2<T, _rows, _cols>::inverse4x4(bool *invertibility) const
{
  Matrix2<T, _rows, _cols> matrix(*this);

  T m00 = this->at(0,0);
  T m01 = this->at(0,1);
  T m02 = this->at(0,2);
  T m03 = this->at(0,3);
  T m10 = this->at(1,0);
  T m11 = this->at(1,1);
  T m12 = this->at(1,2);
  T m13 = this->at(1,3);
  T m20 = this->at(2,0);
  T m21 = this->at(2,1);
  T m22 = this->at(2,2);
  T m23 = this->at(2,3);
  T m30 = this->at(3,0);
  T m31 = this->at(3,1);
  T m32 = this->at(3,2);
  T m33 = this->at(3,3);

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
  T b5 = m22 * m33 - m23 * m32;
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

template<typename T, size_t _rows, size_t _cols> inline 
Matrix2<T, _rows, _cols> Matrix2<T, _rows, _cols>::inversenxn(bool *invertibility) const
{
  Matrix2<T, _rows, _cols> matrix(*this);

  T det = determinantnxn();
  if (det != static_cast<T>(0)) {
    Matrix2<T, _rows, _cols> adjoint = this->adjugate();
    matrix = adjoint / det;
  } else {
    if (invertibility != nullptr) *invertibility = false;
  }

  return matrix;
}

template<typename T, size_t _rows, size_t _cols> inline 
Matrix2<T, _cols, _rows> Matrix2<T, _rows, _cols>::transpose() const
{
  size_t rows = this->rows();
  size_t cols = this->cols();
  Matrix2<T, _cols, _rows> matrix = transposeReserve(*this);

  for (size_t r = 0; r < rows; r++) {
    for (size_t c = 0; c < cols; c++) {
      matrix.at(c, r) = this->at(r, c);
    }
  }

  //parallel_for(static_cast<size_t>(0), rows, [&](size_t r) {
  //  for (size_t c = 0; c < cols; c++) {
  //    matrix.at(c, r) = this->at(r, c);
  //  }
  //});

  return matrix;
}

template<typename T, size_t _rows, size_t _cols> inline
Matrix2<T, _rows, _cols> Matrix2<T, _rows, _cols>::adjugate() const
{
  static_assert(_rows == _cols, "Non-Square Matrix");
  size_t rows = this->rows();
  size_t cols = this->cols();
  TL_ASSERT(rows == cols, "Non-Square Matrix");

  Matrix2<T, _rows, _cols> matrix;

  if (rows == 2) {
    matrix = adjoint2x2();
  } else if (rows == 3) {
    matrix = adjoint3x3();
  } else if (rows == 4) {
    matrix = adjoint4x4();
  } else {
    matrix = adjointnxn();
  }

  //Matrix2<T, _rows, _cols> matrix(matrixReserve(*this));
  
  //if (rows == 2) {
  //  adjoint2x2(matrix);
  //  //matrix = adjoint2x2();
  //} else if (rows == 3) {
  //  //adjoint3x3(matrix);
  //  matrix = adjoint3x3();
  //} else if (rows == 4) {
  //  matrix = adjoint4x4();
  //} else {
  //  matrix = adjointnxn();
  //}

  return matrix;
}

template<typename T, size_t _rows, size_t _cols> inline
Matrix2<T, _rows, _cols> Matrix2<T, _rows, _cols>::adjoint2x2() const
{
  Matrix2<T, _rows, _cols> matrix(*this);
  matrix.at(0, 0) = this->at(1,1);
  matrix.at(0, 1) = -this->at(0,1);
  matrix.at(1, 0) = -this->at(1,0);
  matrix.at(1, 1) = this->at(0,0);
  return matrix;
}

template<typename T, size_t _rows, size_t _cols> inline
void Matrix2<T, _rows, _cols>::adjoint2x2(Matrix2<T, _rows, _cols> &matrix) const
{
  matrix.at(0, 0) = this->at(1,1);
  matrix.at(0, 1) = -this->at(0,1);
  matrix.at(1, 0) = -this->at(1,0);
  matrix.at(1, 1) = this->at(0,0);
}

template<typename T, size_t _rows, size_t _cols> inline
void Matrix2<T, _rows, _cols>::adjoint3x3(Matrix2<T, _rows, _cols> &matrix) const
{

  matrix.at(0, 0) = this->at(1,1) * this->at(2,2) - this->at(1,2) * this->at(2,1);
  matrix.at(0, 1) = this->at(0,2) * this->at(2,1) - this->at(0,1) * this->at(2,2);
  matrix.at(0, 2) = this->at(0,1) * this->at(1,2) - this->at(0,2) * this->at(1,1);
  matrix.at(1, 0) = this->at(1,2) * this->at(2,0) - this->at(1,0) * this->at(2,2);
  matrix.at(1, 1) = this->at(0,0) * this->at(2,2) - this->at(0,2) * this->at(2,0);
  matrix.at(1, 2) = this->at(0,2) * this->at(1,0) - this->at(0,0) * this->at(1,2);
  matrix.at(2, 0) = this->at(1,0) * this->at(2,1) - this->at(1,1) * this->at(2,0);
  matrix.at(2, 1) = this->at(0,1) * this->at(2,0) - this->at(0,0) * this->at(2,1);
  matrix.at(2, 2) = this->at(0,0) * this->at(1,1) - this->at(0,1) * this->at(1,0);
}

template<typename T, size_t _rows, size_t _cols> inline
Matrix2<T, _rows, _cols> Matrix2<T, _rows, _cols>::adjoint3x3() const
{
  Matrix2<T, _rows, _cols> matrix(*this);

  matrix.at(0, 0) = this->at(1,1) * this->at(2,2) - this->at(1,2) * this->at(2,1);
  matrix.at(0, 1) = this->at(0,2) * this->at(2,1) - this->at(0,1) * this->at(2,2);
  matrix.at(0, 2) = this->at(0,1) * this->at(1,2) - this->at(0,2) * this->at(1,1);
  matrix.at(1, 0) = this->at(1,2) * this->at(2,0) - this->at(1,0) * this->at(2,2);
  matrix.at(1, 1) = this->at(0,0) * this->at(2,2) - this->at(0,2) * this->at(2,0);
  matrix.at(1, 2) = this->at(0,2) * this->at(1,0) - this->at(0,0) * this->at(1,2);
  matrix.at(2, 0) = this->at(1,0) * this->at(2,1) - this->at(1,1) * this->at(2,0);
  matrix.at(2, 1) = this->at(0,1) * this->at(2,0) - this->at(0,0) * this->at(2,1);
  matrix.at(2, 2) = this->at(0,0) * this->at(1,1) - this->at(0,1) * this->at(1,0);

  return matrix;
}

template<typename T, size_t _rows, size_t _cols> inline
Matrix2<T, _rows, _cols> Matrix2<T, _rows, _cols>::adjoint4x4() const
{
  Matrix2<T, _rows, _cols> matrix(*this);

  T m00 = this->at(0,0);
  T m01 = this->at(0,1);
  T m02 = this->at(0,2);
  T m03 = this->at(0,3);
  T m10 = this->at(1,0);
  T m11 = this->at(1,1);
  T m12 = this->at(1,2);
  T m13 = this->at(1,3);
  T m20 = this->at(2,0);
  T m21 = this->at(2,1);
  T m22 = this->at(2,2);
  T m23 = this->at(2,3);
  T m30 = this->at(3,0);
  T m31 = this->at(3,1);
  T m32 = this->at(3,2);
  T m33 = this->at(3,3);

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
  T b5 = m22 * m33 - m23 * m32;

  matrix.at(0, 0) =  m11 * b5 - m12 * b4 + m13 * b3;
  matrix.at(0, 1) = -m01 * b5 + m02 * b4 - m03 * b3;
  matrix.at(0, 2) =  m31 * a5 - m32 * a4 + m33 * a3;
  matrix.at(0, 3) = -m21 * a5 + m22 * a4 - m23 * a3;
  matrix.at(1, 0) = -m10 * b5 + m12 * b2 - m13 * b1;
  matrix.at(1, 1) =  m00 * b5 - m02 * b2 + m03 * b1;
  matrix.at(1, 2) = -m30 * a5 + m32 * a2 - m33 * a1;
  matrix.at(1, 3) =  m20 * a5 - m22 * a2 + m23 * a1;
  matrix.at(2, 0) =  m10 * b4 - m11 * b2 + m13 * b0;
  matrix.at(2, 1) = -m00 * b4 + m01 * b2 - m03 * b0;
  matrix.at(2, 2) =  m30 * a4 - m31 * a2 + m33 * a0;
  matrix.at(2, 3) = -m20 * a4 + m21 * a2 - m23 * a0;
  matrix.at(3, 0) = -m10 * b3 + m11 * b1 - m12 * b0;
  matrix.at(3, 1) =  m00 * b3 - m01 * b1 + m02 * b0;
  matrix.at(3, 2) = -m30 * a3 + m31 * a1 - m32 * a0;
  matrix.at(3, 3) =  m20 * a3 - m21 * a1 + m22 * a0;

  return matrix;
}

template<typename T, size_t _rows, size_t _cols> inline
Matrix2<T, _rows, _cols> Matrix2<T, _rows, _cols>::adjointnxn() const
{
  Matrix2<T, _rows, _cols> matrix = this->cofactorMatrix();
  return matrix.transpose();
}

template<typename T, size_t _rows, size_t _cols> inline
Matrix2<T, _rows, _cols> Matrix2<T, _rows, _cols>::cofactorMatrix() const
{
  static_assert(_rows == _cols, "Non-Square Matrix");

  size_t rows = this->rows();
  size_t cols = this->cols();
  TL_ASSERT(rows == cols, "Non-Square Matrix");

  Matrix2<T, _rows, _cols> matrix(*this);
  for (size_t r = 0; r < rows; r++) {
    for (size_t c = 0; c < cols; c++) {
      matrix.at(r, c) = cofactor(r, c);
    }
  }
  return matrix;
}

template<typename T, size_t _rows, size_t _cols> inline
Matrix2<T, _rows, _cols> Matrix2<T, _rows, _cols>::rowEchelonForm(T *determinant) const
{
  const T zero{0};
  const T one{1};
  T d = one;
  size_t rows = this->rows();
  size_t cols = this->cols();

  Matrix2<T, _rows, _cols> matrix(*this);

  for (size_t i = 0; i < rows; ++i) {
    T pivotElement = matrix.at(i,i);
    size_t pivotRow = i;
    for (size_t r = i + 1; r < rows; ++r) {
      if (std::abs(matrix.at(r,i)) > std::abs(pivotElement)) {
        pivotElement = matrix.at(r,i);
        pivotRow = r;
      }
    }

    if (pivotElement == zero) {
      d = zero;
      break;
    }

    if (pivotRow != i) {
      matrix.swapRows(i, pivotRow);
      d *= -one;
    }

    d *= pivotElement;

    for (size_t r = i + 1; r < rows; ++r) {
      for (size_t c = i + 1; c < cols; ++c) {
        matrix.at(r,c) -= matrix.at(r,i) * matrix.at(i,c) / pivotElement;
      }
    }
  }
        
  if (determinant) {
    *determinant = d;
  }

  return matrix;
}

////template<size_t _rows, size_t _cols, typename T> 
////Matrix2<_rows, _cols, T> Matrix2<_rows, _cols, T>::reducedRowEchelonForm(T *determinant) const
////{
////  const T zero{0};
////  const T one{1};
////  T d = one;
////  size_t n = mMatrix.size();
////  std::array<std::array<T, _cols>, _rows> matrix = this->rowEchelonForm(determinant);
////
////  return matrix;
////}

template<typename T, size_t _rows, size_t _cols> inline
T Matrix2<T, _rows, _cols>::determinant() const
{
  static_assert(_rows == _cols, "Non-Square Matrix");

  T d = static_cast<T>(1);
  size_t rows = this->rows();

  if (rows == 2) {
    d = determinant2x2();
  } else if (rows == 3) {
    d = determinant3x3();
  } else if (rows == 4) {
    d = determinant4x4();
  } else {
    d = determinantnxn();
  }

  return d;
}

template<typename T, size_t _rows, size_t _cols> inline
T Matrix2<T, _rows, _cols>::determinant2x2() const
{
  T det = this->at(0,0) * this->at(1,1) - this->at(0,1) * this->at(1,0);
  return det;
}

template<typename T, size_t _rows, size_t _cols> inline
T Matrix2<T, _rows, _cols>::determinant3x3() const
{
  T c00 = this->at(1,1) * this->at(2,2) - this->at(1,2) * this->at(2,1);
  T c10 = this->at(1,2) * this->at(2,0) - this->at(1,0) * this->at(2,2);
  T c20 = this->at(1,0) * this->at(2,1) - this->at(1,1) * this->at(2,0);
  T det = this->at(0,0) * c00 + this->at(0,1) * c10 + this->at(0,2) * c20;
  return det;
}

template<typename T, size_t _rows, size_t _cols> inline
T Matrix2<T, _rows, _cols>::determinant4x4() const
{
  T a0 = this->at(0,0) * this->at(1,1) - this->at(0,1) * this->at(1,0);
  T a1 = this->at(0,0) * this->at(1,2) - this->at(0,2) * this->at(1,0);
  T a2 = this->at(0,0) * this->at(1,3) - this->at(0,3) * this->at(1,0);
  T a3 = this->at(0,1) * this->at(1,2) - this->at(0,2) * this->at(1,1);
  T a4 = this->at(0,1) * this->at(1,3) - this->at(0,3) * this->at(1,1);
  T a5 = this->at(0,2) * this->at(1,3) - this->at(0,3) * this->at(1,2);
  T b0 = this->at(2,0) * this->at(3,1) - this->at(2,1) * this->at(3,0);
  T b1 = this->at(2,0) * this->at(3,2) - this->at(2,2) * this->at(3,0);
  T b2 = this->at(2,0) * this->at(3,3) - this->at(2,3) * this->at(3,0);
  T b3 = this->at(2,1) * this->at(3,2) - this->at(2,2) * this->at(3,1);
  T b4 = this->at(2,1) * this->at(3,3) - this->at(2,3) * this->at(3,1);
  T b5 = this->at(2,2) * this->at(3,3) - this->at(2,3) * this->at(3,2);
  T det = a0 * b5 - a1 * b4 + a2 * b3 + a3 * b2 - a4 * b1 + a5 * b0;
  return det;
}

template<typename T, size_t _rows, size_t _cols> inline
T Matrix2<T, _rows, _cols>::determinantnxn() const
{
  T d;
  this->rowEchelonForm(&d);
  return d;
}

template<typename T, size_t _rows, size_t _cols> inline
T Matrix2<T, _rows, _cols>::trace() const
{
  static_assert(_rows == _cols, "Non-Square Matrix");

  size_t rows = this->rows();
  size_t cols = this->cols();
  
  TL_ASSERT(rows == cols, "Non-Square Matrix");

  T trace = static_cast<T>(0);
  for (size_t r = 0; r < rows; r++) {
    trace += this->at(r,r);
  }
  return trace;
}

template<typename T, size_t _rows, size_t _cols> inline
bool Matrix2<T, _rows, _cols>::invertible()
{
  T det = determinant();
  if (det == static_cast<T>(0)) return false;
  else return true;
}

template<typename T, size_t _rows, size_t _cols> inline
bool Matrix2<T, _rows, _cols>::singular()
{
  T det = determinant();
  if (det == static_cast<T>(0)) return true;
  else return false;
}

template<typename T, size_t _rows, size_t _cols> inline 
T Matrix2<T, _rows, _cols>::cofactor(size_t r, size_t c) const
{
  static_assert(_rows == _cols, "Non-Square Matrix");

  T sign = ((r + c) % 2 == 0) ? static_cast<T>(1) : -static_cast<T>(1);
  return sign * this->firstMinor(r, c);
}

template<typename T, size_t _rows, size_t _cols> inline 
T Matrix2<T, _rows, _cols>::firstMinor(size_t r, size_t c) const
{
  static_assert(_rows == _cols, "Non-Square Matrix");

  size_t rows = this->rows();
  size_t cols = this->cols();
  
  TL_ASSERT(rows == cols, "Non-Square Matrix");

  T determinant = static_cast<T>(0);

  size_t i = 0;
  size_t j = 0;

  constexpr static size_t _r = _rows == DYN_MATRIX ? DYN_MATRIX : _rows - 1;
  constexpr static size_t _c = _cols == DYN_MATRIX ? DYN_MATRIX : _cols - 1;

  if (_rows == DYN_MATRIX && _cols == DYN_MATRIX) {
    Matrix2<T> matrix(rows-1, cols-1);
    for (size_t row = 0; row < rows; row++) {
      for (size_t col = 0; col < cols; col++) {
        if (row != r && col != c) {
          matrix.at(i, j++) = this->at(row,col);
          if (j == rows - 1) {
            j = 0;
            i++;
          }
        }
      }
    }
    determinant = matrix.determinant();
  } else {
    Matrix2<T, _r, _c> matrix;
    for (size_t row = 0; row < rows; row++) {
      for (size_t col = 0; col < cols; col++) {
        if (row != r && col != c) {
          matrix.at(i, j++) = this->at(row,col);
          if (j == rows - 1) {
            j = 0;
            i++;
          }
        }
      }
    }
    determinant = matrix.determinant();
  }

  return determinant;
}

template<typename T, size_t _rows, size_t _cols> inline 
Matrix2<T, _rows, _cols> Matrix2<T, _rows, _cols>::zero()
{
  const T zero{0};
  Matrix2<T, _rows, _cols> matrix;
  for (size_t r = 0; r < matrix.rows(); r++) {
    for (size_t c = 0; c < matrix.cols(); c++) {
      matrix.at(r, c) = zero;
    }
  }
  return matrix;
}

template<typename T, size_t _rows, size_t _cols> inline 
Matrix2<T, _rows, _cols> Matrix2<T, _rows, _cols>::zero(size_t rows, size_t cols)
{
  static_assert(_rows == DYN_MATRIX || _cols == DYN_MATRIX, "Dynamic Matrix not support resize");

  return Matrix2<T>(rows, cols, static_cast<T>(0));
}

template<typename T, size_t _rows, size_t _cols> inline 
Matrix2<T, _rows, _cols> Matrix2<T, _rows, _cols>::ones()
{
  const T one{1};
  Matrix2<T, _rows, _cols> matrix;
  for (size_t r = 0; r < matrix.rows(); r++) {
    for (size_t c = 0; c < matrix.cols(); c++) {
      matrix.at(r, c) = one;
    }
  }
  return matrix;
}

template<typename T, size_t _rows, size_t _cols> inline 
Matrix2<T, _rows, _cols> Matrix2<T, _rows, _cols>::ones(size_t rows, size_t cols)
{
  static_assert(_rows == DYN_MATRIX || _cols == DYN_MATRIX, "Dynamic Matrix not support resize");

  return Matrix2<T>(rows, cols, static_cast<T>(1));
}

template<typename T, size_t _rows, size_t _cols> inline 
Matrix2<T, _rows, _cols> Matrix2<T, _rows, _cols>::identity()
{
  const T zero{0}; 
  const T one{1};
  Matrix2<T, _rows, _cols> matrix;
  for (size_t r = 0; r < matrix.rows(); r++) {
    for (size_t c = 0; c < matrix.cols(); c++) {
      if (r == c) {
        matrix.at(r, c) = one;
      } else {
        matrix.at(r, c) = zero;
      }
    }
  }
  return matrix;
}

template<typename T, size_t _rows, size_t _cols> inline 
Matrix2<T, _rows, _cols> Matrix2<T, _rows, _cols>::identity(size_t rows, size_t cols)
{
  static_assert(_rows == DYN_MATRIX || _cols == DYN_MATRIX, "Dynamic Matrix not support resize");

  const T zero{0}; 
  const T one{1};
  Matrix2<T> matrix(rows, cols);
  for (size_t r = 0; r < matrix.rows(); r++) {
    for (size_t c = 0; c < matrix.cols(); c++) {
      if (r == c) {
        matrix.at(r, c) = one;
      } else {
        matrix.at(r, c) = zero;
      }
    }
  }
  return matrix;
}

/* Operaciones unarias */

template<typename T, size_t _rows, size_t _cols> inline  static
Matrix2<T, _rows, _cols> operator + (const Matrix2<T, _rows, _cols> &matrix)
{
  return matrix;
}

template<typename T, size_t _rows, size_t _cols> inline static
Matrix2<T, _rows, _cols> operator - (const Matrix2<T, _rows, _cols> &matrix)
{
  Matrix2<T, _rows, _cols> _m(matrix);
  for (size_t r = 0; r < matrix.rows(); r++) {
    for (size_t c = 0; c < matrix.cols(); c++) {
      _m.at(r, c) = -_m.at(r, c);
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
template<typename T, size_t _rows, size_t _cols> inline static
Matrix2<T, _rows, _cols> operator + (const Matrix2<T, _rows, _cols> &matrix1,
                                     const Matrix2<T, _rows, _cols> &matrix2)
{
  Matrix2<T, _rows, _cols> matrix = matrix1;
  return matrix += matrix2;
}

template<typename T, size_t _rows, size_t _cols> inline static
Matrix2<T, _rows, _cols> &operator += (Matrix2<T, _rows, _cols> &matrix1,
                                       const Matrix2<T, _rows, _cols> &matrix2)
{
  size_t rows1 = matrix1.rows();
  size_t cols1 = matrix1.cols();
  size_t rows2 = matrix2.rows();
  size_t cols2 = matrix2.cols();
  TL_ASSERT(rows1 == rows2 && cols1 == cols2, "A size != B size");

  for (int r = 0; r < rows1; r++) {
    for (int c = 0; c < cols1; c++) {
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
template<typename T, size_t _rows, size_t _cols> inline static
Matrix2<T, _rows, _cols> operator - (const Matrix2<T, _rows, _cols> &matrix1,
                                    const Matrix2<T, _rows, _cols> &matrix2)
{
  Matrix2<T, _rows, _cols> matrix = matrix1;
  return matrix -= matrix2;
}

template<typename T, size_t _rows, size_t _cols> inline static
Matrix2<T, _rows, _cols> &operator -= (Matrix2<T, _rows, _cols> &matrix1,
                                       const Matrix2<T, _rows, _cols> &matrix2)
{
  size_t rows1 = matrix1.rows();
  size_t cols1 = matrix1.cols();
  size_t rows2 = matrix2.rows();
  size_t cols2 = matrix2.cols();
  TL_ASSERT(rows1 == rows2 && cols1 == cols2, "A size != B size");


  for (int r = 0; r < matrix1.rows(); r++) {
    for (int c = 0; c < matrix1.cols(); c++) {
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
template<typename T, size_t _rows, size_t _dim, size_t _cols> inline  static
Matrix2<T, _rows, _cols> operator * (const Matrix2<T, _rows, _dim> &matrix1,
                                     const Matrix2<T, _dim, _cols> &matrix2)
{
  Matrix2<T, _rows, _cols> matrix = Matrix2<T, _rows, _cols>::zero();
  for (size_t r = 0; r < _rows; r++) {
    for (size_t c = 0; c < _cols; c++) {
      for (size_t i = 0; i < _dim; i++) {
        matrix.at(r, c) += matrix1.at(r, i) * matrix2.at(i, c);
      }
    }
  }
  return matrix;
}

template<typename T> inline  static
Matrix2<T> operator * (const Matrix2<T> &matrix1,
                       const Matrix2<T> &matrix2)
{
  size_t rows = matrix1.rows();
  size_t dim1 = matrix1.cols();
  size_t cols = matrix2.cols();
  size_t dim2 = matrix2.rows();
  TL_ASSERT(dim1 == dim2, "A columns != B rows");

  Matrix2<T> matrix = Matrix2<T>::zero(rows, cols);
  for (size_t r = 0; r < rows; r++) {
    for (size_t c = 0; c < cols; c++) {
      for (size_t i = 0; i < dim1; i++) {
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
template<typename T, size_t _rows, size_t _cols> inline static
Matrix2<T, _rows, _cols> operator * (const Matrix2<T, _rows, _cols> &matrix,
                                     T scalar)
{
  Matrix2<T, _rows, _cols> _matrix = matrix;
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
template<typename T, size_t _rows, size_t _cols> inline static
Matrix2<T, _rows, _cols> operator * (T scalar, const Matrix2<T, _rows, _cols> &matrix)
{
  Matrix2<T, _rows, _cols> _matrix = matrix;
  return _matrix *= scalar;
}

template<typename T, size_t _rows, size_t _cols> inline static
Matrix2<T, _rows, _cols> &operator *= (Matrix2<T, _rows, _cols> &matrix, T scalar)
{
  for (int r = 0; r < matrix.rows(); r++) {
    for (int c = 0; c < matrix.cols(); c++) {
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
template<typename T, size_t _rows, size_t _cols> inline static
Matrix2<T, _rows, _cols> operator / (const Matrix2<T, _rows, _cols> &matrix, T scalar)
{
  Matrix2<T, _rows, _cols> _matrix = matrix;
  return _matrix /= scalar;
}

template<typename T, size_t _rows, size_t _cols> inline static
Matrix2<T, _rows, _cols> &operator /= (Matrix2<T, _rows, _cols> &matrix, T scalar)
{
  if (scalar != static_cast<T>(0)) {
    for (int r = 0; r < matrix.rows(); r++) {
      for (int c = 0; c < matrix.cols(); c++) {
        matrix.at(r, c) /= scalar;
      }
    }
  } else {
    matrix = Matrix2<T, _rows, _cols>::zero();
  }
  return matrix;
}

template<typename T> inline static
Matrix2<T> &operator /= (Matrix2<T> &matrix, T scalar)
{
  if (scalar != static_cast<T>(0)) {
    for (int r = 0; r < matrix.rows(); r++) {
      for (int c = 0; c < matrix.cols(); c++) {
        matrix.at(r, c) /= scalar;
      }
    }
  } else {
    matrix = Matrix2<T>::zero(matrix.rows(), matrix.cols());
  }
  return matrix;
}

/*! \} */ // end of Algebra

/*! \} */ // end of Math

} // Fin namespace math


} // End namespace tl

#endif // TL_MATH_MATRIX_H
