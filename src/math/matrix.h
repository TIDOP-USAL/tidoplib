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
template<int rows, int cols, typename T = double>
class Matrix
{

protected:

  std::array<std::array<T, cols>, rows> mMatrix;
  
public:

  /*!
   * \brief Constructor por defecto
   */
  Matrix();

  /*!
   * \brief Constructor de copia
   * \param[in] mat Matri
   */
  Matrix(const Matrix<rows, cols, T> &mat);

  /*!
   * \brief Constructor
   * \param[in] mat Matriz
   */
  Matrix(const std::array<std::array<T, cols>, rows> &mat);

  /*!
   * \brief destructora
   */
  ~Matrix();

  /*!
   * \brief Operador de asignación
   * \param[in] mat Objeto que se copia
   */
  Matrix& operator = (const Matrix<rows, cols, T> &mat);
  
  T &at(int r, int c);
  const T &at(int r, int c) const;
};

template<int rows, int cols, typename T> inline
Matrix<rows, cols, T>::Matrix()
{
}

template<int rows, int cols, typename T> inline
Matrix<rows, cols, T>::Matrix(const Matrix<rows, cols, T> &rot)
  : mMatrix(rot.mMatrix)
{
}

template<int rows, int cols, typename T> inline
Matrix<rows, cols, T>::Matrix(const std::array<std::array<T, cols>, rows> &rot)
  : mMatrix(rot)
{
}

template<int rows, int cols, typename T> inline
Matrix<rows, cols, T>::~Matrix()
{
}

template<int rows, int cols, typename T> inline
Matrix<rows, cols, T> &Matrix<rows, cols, T>::operator = (const Matrix& rot)
{
  if (this != &rot) {
    this->mMatrix = rot.mMatrix;
  }
  return *this;
}

template<int rows, int cols, typename T> inline
T &Matrix<rows, cols, T>::at(int r, int c)
{
  return mMatrix[r][c];
}

template<int rows, int cols, typename T> inline
const T &Matrix<rows, cols, T>::at(int r, int c) const
{
  return mMatrix[r][c];
}

} // Fin namespace math

} // End namespace TL

#endif // TL_MATH_MATRIX_H