#ifndef TL_MATH_ROTATION_MATRIX_H
#define TL_MATH_ROTATION_MATRIX_H

#include "config_tl.h"
#include "tidop/math/algebra/matrix.h"
//#include "tidop/math/algebra/quaternion.h"

#include <vector>
#include <array>

namespace tl
{

namespace math
{

/*!
 * \brief Matriz de rotación
 */
//template <typename T>
//using RotationMatrix = Matrix<3, 3, T>;

/*!
 * \brief Matriz de rotación
 */
template <typename T>
class RotationMatrix 
  : public Matrix<3, 3, T>
{

public:
  
  RotationMatrix();
  //RotationMatrix(const Quaternion<T> &quaternion);
  ~RotationMatrix() {}

private:

};


template <typename T> inline
RotationMatrix<T>::RotationMatrix()
  : Matrix<3, 3, T>()
{
}

//template<typename T> inline
//RotationMatrix<T>::RotationMatrix(const Quaternion<T> &quaternion)
//  : Matrix<3, 3, T>()
//{
//  T _2{ 2 };
//
//  T _2x = _2 * quaternion.x;
//  T _2y = _2 * quaternion.y;
//  T _2z = _2 * quaternion.z;
//  T _2xx = _2x * quaternion.x;
//  T _2xy = _2x * quaternion.y;
//  T _2xz = _2x * quaternion.z;
//  T _2xw = _2x * quaternion.w;
//  T _2yy = _2y * quaternion.y;
//  T _2yz = _2y * quaternion.z;
//  T _2yw = _2y * quaternion.w;
//  T _2zz = _2z * quaternion.z;
//  T _2zw = _2z * quaternion.w;
//
//  T _1{ 1 };
//
//  this->at(0, 0) = _1 - _2yy - _2zz;
//  this->at(0, 1) = _2xy - _2zw;
//  this->at(0, 2) = _2xz + _2yw;
//  this->at(1, 0) = _2xy + _2zw;
//  this->at(1, 1) = _1 - _2xx - _2zz;
//  this->at(1, 2) = _2yz - _2xw;
//  this->at(2, 0) = _2xz - _2yw;
//  this->at(2, 1) = _2yz + _2xw;
//  this->at(2, 2) = _1 - _2xx - _2yy;
//}

} // Fin namespace math

} // End namespace tl

#endif // TL_MATH_ROTATION_MATRIX_H
