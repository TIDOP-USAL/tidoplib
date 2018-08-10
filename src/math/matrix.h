#ifndef TL_MATH_MATRIX_H
#define TL_MATH_MATRIX_H

#include "config_tl.h"

#include <vector>
#include <array>

namespace TL
{

namespace math
{

/*!
 * \brief Matriz de rotación
 */
template<size_t _rows, size_t _cols, typename T = double>
class Matrix
{

protected:

  std::array<std::array<T, _cols>, _rows> mMatrix;
  //std::array<Matrix<1, _cols, T>, _rows> mMatrix;
  size_t mRows;
  size_t mCols;

public:

  /*!
   * \brief Constructor por defecto
   */
  Matrix();

  /*!
   * \brief Constructor de copia
   * \param[in] mat Matri
   */
  Matrix(const Matrix<_rows, _cols, T> &mat);

  /*!
   * \brief Constructor
   * \param[in] mat Matriz
   */
  Matrix(const std::array<std::array<T, _cols>, _rows> &mat);

  /*!
   * \brief destructora
   */
  ~Matrix();

  /*!
   * \brief Operador de asignación
   * \param[in] mat Objeto que se copia
   */
  Matrix& operator = (const Matrix<_rows, _cols, T> &mat);
  
  T &at(size_t r, size_t c);
  const T &at(size_t r, size_t c) const;

  size_t rows() const;
  size_t cols() const;
};

template<size_t _rows, size_t _cols, typename T> inline
Matrix<_rows, _cols, T>::Matrix()
  : mMatrix(),
    mRows(_rows),
    mCols(_cols)
{
}

template<size_t _rows, size_t _cols, typename T> inline
Matrix<_rows, _cols, T>::Matrix(const Matrix<_rows, _cols, T> &rot)
: mMatrix(rot.mMatrix),
  mRows(_rows),
  mCols(_cols)
{
}

template<size_t _rows, size_t _cols, typename T> inline
Matrix<_rows, _cols, T>::Matrix(const std::array<std::array<T, _cols>, _rows> &rot)
  : mMatrix(rot),
    mRows(_rows),
    mCols(_cols)
{
}

template<size_t _rows, size_t _cols, typename T> inline
Matrix<_rows, _cols, T>::~Matrix()
{
}

template<size_t _rows, size_t _cols, typename T> inline
Matrix<_rows, _cols, T> &Matrix<_rows, _cols, T>::operator = (const Matrix& rot)
{
  if (this != &rot) {
    this->mMatrix = rot.mMatrix;
  }
  return *this;
}

template<size_t _rows, size_t _cols, typename T> inline
T &Matrix<_rows, _cols, T>::at(size_t r, size_t c)
{
  return mMatrix[r][c];
}

template<size_t _rows, size_t _cols, typename T> inline
const T &Matrix<_rows, _cols, T>::at(size_t r, size_t c) const
{
  return mMatrix[r][c];
}

template<size_t _rows, size_t _cols, typename T> inline
size_t Matrix<_rows, _cols, T>::rows() const
{
  return mRows;
}

template<size_t _rows, size_t _cols, typename T> inline
size_t Matrix<_rows, _cols, T>::cols() const
{
  return mCols;
}

} // Fin namespace math

} // End namespace TL

#endif // TL_MATH_MATRIX_H