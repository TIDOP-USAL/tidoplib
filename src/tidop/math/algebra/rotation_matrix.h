#ifndef TL_MATH_ROTATION_MATRIX_H
#define TL_MATH_ROTATION_MATRIX_H

#include "config_tl.h"
#include "tidop/math/algebra/matrix.h"

#include <vector>
#include <array>

namespace tl
{

namespace math
{

/*!
 * \brief Matriz de rotación
 */
template <size_t dim, typename T>
using RotationMatrix = Matrix<dim, dim, T>;

//template<int dim, typename T = double>
//class RotationMatrix
//  : public Matrix<dim, dim, T>
//{
//  
//public:
//
//  /*!
//   * \brief Constructor por defecto
//   */
//  RotationMatrix();
//  
//  /*!
//   * \brief Constructor de copia
//   * \param[in] rot Matriz de rotación
//   */
//  RotationMatrix(const RotationMatrix<dim, dim, T> &rot);
//
//  /*!
//   * \brief Constructor
//   * \param[in] rot Matriz de rotación
//   */
//  RotationMatrix(const std::array<std::array<T, dim>, dim> &rot);
//
//  /*!
//   * \brief destructora
//   */
//  ~RotationMatrix();
//
//  /*!
//   * \brief Operador de asignación
//   * \param[in] eulerAngles Objeto que se copia
//   */
//  RotationMatrix& operator = (const RotationMatrix<dim, T> &mat);
//
//  RotationMatrix<dim, T> inverse() const;
//
//private:
//
//  RotationMatrix<dim, T> inverse_2x2() const;
//  RotationMatrix<dim, T> inverse_3x3() const;
//  RotationMatrix<dim, T> inverse_4x4() const;
//  RotationMatrix<dim, T> inverse_nxn() const;
//};
//
//template<int dim, typename T> inline
//RotationMatrix<dim, T>::RotationMatrix()
//  : Matrix<dim, dim, T>()
//{
//  mMatrix[0][0] = 1.;
//  mMatrix[1][1] = 1.;
//  mMatrix[2][2] = 1.;
//}
//
//template<int dim, typename T> inline
//RotationMatrix<dim, T>::RotationMatrix(const RotationMatrix<dim, T> &rot)
//  : Matrix<dim, dim, T>(rot)
//{
//}
//
//template<int dim, typename T> inline
//RotationMatrix<dim, T>::RotationMatrix(const std::array<std::array<T, dim>, dim> &rot)
//  : Matrix<dim, dim, T>(rot)
//{
//}
//
//template<int dim, typename T> inline
//RotationMatrix<dim, T>::~RotationMatrix()
//{
//}
//
//template<int dim, typename T> inline
//RotationMatrix<dim, T> &RotationMatrix<dim, T>::operator = (const RotationMatrix& rot)
//{
//  if (this != &rot) {
//    this->mMatrix = rot.mMatrix;
//  }
//  return *this;
//}
//
//template<int dim, typename T> inline
//RotationMatrix<dim, T>  RotationMatrix<dim, T>::inverse() const
//{
//  RotationMatrix<dim, T> inverse;
//  if (mMatrix.size() == 2) {
//    inverse = inverse_2x2();
//  } else if (mMatrix.size() == 3) {
//    inverse = inverse_3x3();
//  } else if (mMatrix.size() == 4) {
//    inverse = inverse_4x4();
//  } else {
//    inverse = inverse_nxn();
//  }
//  return inverse;
//}
//
//template<int dim, typename T> inline
//RotationMatrix<dim, T> RotationMatrix<dim, T>::inverse_2x2() const
//{
//  RotationMatrix<dim, T> inverse;
//  T det = mMatrix[0][0] * mMatrix[1][1] - mMatrix[0][1] * mMatrix[1][0];
//  if (det != static_cast<T>(0)) {
//    inverse.at(0, 0) = mMatrix[1][1] / det;
//    inverse.at(0, 1) = -mMatrix[0][1] / det;
//    inverse.at(1, 0) = -mMatrix[1][0] / det;
//    inverse.at(1, 1) = mMatrix[0][0] / det;
//  } else {
//    ///No invertible
//  }
//  return inverse;
//}
//
//template<int dim, typename T> inline
//RotationMatrix<dim, T> RotationMatrix<dim, T>::inverse_3x3() const
//{
//  return RotationMatrix<dim, T>();
//}
//
//template<int dim, typename T> inline
//RotationMatrix<dim, T> RotationMatrix<dim, T>::inverse_4x4() const
//{
//  return RotationMatrix<dim, T>();
//}
//
//template<int dim, typename T> inline
//RotationMatrix<dim, T> RotationMatrix<dim, T>::inverse_nxn() const
//{
//  return RotationMatrix<dim, T>();
//}


} // Fin namespace math

} // End namespace tl

#endif // TL_MATH_ROTATION_MATRIX_H
