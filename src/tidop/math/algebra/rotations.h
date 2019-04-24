#ifndef TL_MATH_ROTATIONS_H
#define TL_MATH_ROTATIONS_H

#include "config_tl.h"
#include "tidop/math/algebra/quaternion.h"
#include "tidop/math/algebra/euler_angles.h"
#include "tidop/math/algebra/rotation_matrix.h"

namespace tl
{

namespace math
{

/*!
 * \brief Clase para convertir entre diferentes sistemas de rotación
 */
template <typename T>
class RotationConverter
{

public:

private:

  RotationConverter() {}

public:

  /*!
   * \brief Pasa de una matriz de rotación a Cuaterniones
   * \f[ x^2 = (+r00 - r11 - r22 + 1)/4  \f]
   * \f[ y^2 = (-r00 + r11 - r22 + 1)/4  \f]
   * \f[ z^2 = (-r00 - r11 + r22 + 1)/4  \f]
   * \f[ w^2 = (+r00 + r11 + r22 + 1)/4  \f]
   * \f[ x^2 + y^2 = (1 - r22)/2 \f]
   * \f[ z^2 + w^2 = (1 + r22)/2 \f]
   * \f[ y^2 - x^2 = (r11 - r00)/2 \f]
   * \f[ w^2 - z^2 = (r11 + r00)/2 \f]
   * \f[ x*y = (r01 + r10)/4 \f]
   * \f[ x*z = (r02 + r20)/4 \f]
   * \f[ y*z = (r12 + r21)/4 \f]
   *
   *   x*w = (r21 - r12)/4
   *   y*w = (r02 - r20)/4
   *   z*w = (r10 - r01)/4
   *
   * \param[in] rotMatrix
   * \param[out] quaternion
   */
  static void convert(const RotationMatrix<T> &rotationMatrix, Quaternion<T> &quaternion);

  //static void convert(const RotationMatrix<T> &rotationMatrix, EulerAngles<T> &eulerAngles);
  static void convert(const Quaternion<T> &quaternion, RotationMatrix<T> &rotationMatrix);
  static void convert(const Quaternion<T> &quaternion, EulerAngles<T> &eulerAngles);
  static void convert(const EulerAngles<T> &eulerAngles, Quaternion<T> &quaternion);
  static void convert(const EulerAngles<T> &eulerAngles, RotationMatrix<T> &rotationMatrix);

private:

};

// x^2 = (+r00 - r11 - r22 + 1)/4
// y^2 = (-r00 + r11 - r22 + 1)/4
// z^2 = (-r00 - r11 + r22 + 1)/4
// w^2 = (+r00 + r11 + r22 + 1)/4
// x^2 + y^2 = (1 - r22)/2
// z^2 + w^2 = (1 + r22)/2
// y^2 - x^2 = (r11 - r00)/2
// w^2 - z^2 = (r11 + r00)/2
// x*y = (r01 + r10)/4
// x*z = (r02 + r20)/4
// y*z = (r12 + r21)/4
// [GTE_USE_MAT_VEC]
//   x*w = (r21 - r12)/4
//   y*w = (r02 - r20)/4
//   z*w = (r10 - r01)/4
// [GTE_USE_VEC_MAT]
//   x*w = (r12 - r21)/4
//   y*w = (r20 - r02)/4
//   z*w = (r01 - r10)/4
//
// If Q is the 4x1 column vector (x,y,z,w), the previous equations give us
//         +-                  -+
//         | x*x  x*y  x*z  x*w |
// Q*Q^T = | y*x  y*y  y*z  y*w |
//         | z*x  z*y  z*z  z*w |
//         | w*x  w*y  w*z  w*w |
//         +-                  -+

template<typename T>
void RotationConverter<T>::convert(const RotationMatrix<T> &rotationMatrix, Quaternion<T> &quaternion)
{
  T r22 = rotationMatrix.at(2, 2);
  if (r22 <= static_cast<T>(0)) {

    T r11_r00 = rotationMatrix.at(1, 1) - rotationMatrix.at(0, 0);
    if (r11_r00 <= static_cast<T>(0)) {
      quaternion.x = sqrt((static_cast<T>(1) - r22 - r11_r00) / static_cast<T>(4));
      T qx4 = quaternion.x * 4;
      quaternion.y = (rotationMatrix.at(0, 1) + rotationMatrix.at(1, 0)) / qx4;
      quaternion.z = (rotationMatrix.at(0, 2) + rotationMatrix.at(2, 0)) / qx4;
      quaternion.w = (rotationMatrix.at(2, 1) - rotationMatrix.at(1, 2)) / qx4;
    } else {
      quaternion.y = sqrt((static_cast<T>(1) - r22 + r11_r00) / static_cast<T>(4));
      T qy4 = quaternion.y * 4;
      quaternion.x = (rotationMatrix.at(0, 1) + rotationMatrix.at(1, 0)) / qy4;
      quaternion.z = (rotationMatrix.at(1, 2) + rotationMatrix.at(2, 1)) / qy4;
      quaternion.w = (rotationMatrix.at(0, 2) - rotationMatrix.at(2, 0)) / qy4;
    }

  } else {

    T r11_r00 = rotationMatrix.at(1, 1) + rotationMatrix.at(0, 0);
    if (r11_r00 <= static_cast<T>(0)) {
      quaternion.z = sqrt((static_cast<T>(1) + r22 - r11_r00) / static_cast<T>(4));
      T qz4 = quaternion.z * 4;
      quaternion.x = (rotationMatrix.at(0, 2) + rotationMatrix.at(2, 0)) / qz4;
      quaternion.y = (rotationMatrix.at(1, 2) + rotationMatrix.at(2, 1)) / qz4;
      quaternion.w = (rotationMatrix.at(1, 0) - rotationMatrix.at(0, 1)) / qz4;
    } else {
      quaternion.w = sqrt((static_cast<T>(1) + r22 + r11_r00) / static_cast<T>(4));
      T qw4 = quaternion.w * 4;
      quaternion.x = (rotationMatrix.at(2, 1) - rotationMatrix.at(1, 2)) / qw4;
      quaternion.y = (rotationMatrix.at(0, 2) - rotationMatrix.at(2, 0)) / qw4;
      quaternion.z = (rotationMatrix.at(1, 0) - rotationMatrix.at(0, 1)) / qw4;
    }

  }
}

//template<typename T>
//void RotationConverter<T>::convert(const RotationMatrix<T> &rotationMatrix, EulerAngles<T> &eulerAngles)
//{
//  T one{1};

//  if (eulerAngles.axes == EulerAngles<T>::Axes::zxz ||
//      eulerAngles.axes == EulerAngles<T>::Axes::xyx ||
//      eulerAngles.axes == EulerAngles<T>::Axes::yzy ||
//      eulerAngles.axes == EulerAngles<T>::Axes::zyz ||
//      eulerAngles.axes == EulerAngles<T>::Axes::xzx ||
//      eulerAngles.axes == EulerAngles<T>::Axes::yxy){

//    eulerAngles.axes
//    // Map (0,1,2), (1,2,0), and (2,0,1) to +1.
//    // Map (0,2,1), (2,1,0), and (1,0,2) to -1.
//    int parity = (((e.axis[2] | (e.axis[1] << 2)) >> e.axis[0]) & 1);
//    const T sgn = (parity & 1 ? -one : one);

//    if (r(e.axis[2], e.axis[0]) < one)
//    {
//        if (r(e.axis[2], e.axis[0]) > -one)
//        {
//            e.angle[2] = std::atan2(sgn*r(e.axis[1], e.axis[0]),
//                r(e.axis[0], e.axis[0]));
//            e.angle[1] = std::asin(-sgn*r(e.axis[2], e.axis[0]));
//            e.angle[0] = std::atan2(sgn*r(e.axis[2], e.axis[1]),
//                r(e.axis[2], e.axis[2]));
//            e.result = ER_UNIQUE;
//        }
//        else {
//          eulerAngles.omega = static_cast<T>(0);
//          eulerAngles.phi =sgn*static_cast<T>(GTE_C_HALF_PI);
//          eulerAngles.omega = std::atan2(-sgn*r(e.axis[1], e.axis[2]),
//              r(e.axis[1], e.axis[1]));
//            //e.angle[2] = (Real)0;
//            //e.angle[1] = sgn*(Real)GTE_C_HALF_PI;
//            //e.angle[0] = std::atan2(-sgn*r(e.axis[1], e.axis[2]),
//            //    r(e.axis[1], e.axis[1]));
//            e.result = ER_NOT_UNIQUE_DIF;
//        }
//    }
//    else
//    {
//        e.angle[2] = (Real)0;
//        e.angle[1] = -sgn*(Real)GTE_C_HALF_PI;
//        e.angle[0] = std::atan2(-sgn*r(e.axis[1], e.axis[2]),
//            r(e.axis[1], e.axis[1]));
//        e.result = ER_NOT_UNIQUE_SUM;
//    }

//  } else {
//    // Map (0,2,0), (1,0,1), and (2,1,2) to +1.
//    // Map (0,1,0), (1,2,1), and (2,0,2) to -1.
//    int b0 = 3 - e.axis[1] - e.axis[2];
//    int parity = (((b0 | (e.axis[1] << 2)) >> e.axis[2]) & 1);
//    Real const sgn = (parity & 1 ? (Real)+1 : (Real)-1);

//    if (r(e.axis[2], e.axis[2]) < one)
//    {
//        if (r(e.axis[2], e.axis[2]) > -one)
//        {
//            e.angle[2] = std::atan2(r(e.axis[1], e.axis[2]),
//                sgn*r(b0, e.axis[2]));
//            e.angle[1] = std::acos(r(e.axis[2], e.axis[2]));
//            e.angle[0] = std::atan2(r(e.axis[2], e.axis[1]),
//                -sgn*r(e.axis[2], b0));
//            e.result = ER_UNIQUE;
//        }
//        else
//        {
//            e.angle[2] = (Real)0;
//            e.angle[1] = (Real)GTE_C_PI;
//            e.angle[0] = std::atan2(sgn*r(e.axis[1], b0),
//                r(e.axis[1], e.axis[1]));
//            e.result = ER_NOT_UNIQUE_DIF;
//        }
//    }
//    else
//    {
//        e.angle[2] = (Real)0;
//        e.angle[1] = (Real)0;
//        e.angle[0] = std::atan2(sgn*r(e.axis[1], b0),
//            r(e.axis[1], e.axis[1]));
//        e.result = ER_NOT_UNIQUE_SUM;
//    }
//  }

//    //   {(0,1,2),(0,2,1),(1,0,2),(1,2,0),(2,0,1),(2,1,0),
//    //    (0,1,0),(0,2,0),(1,0,1),(1,2,1),(2,0,2),(2,1,2)}
//}

template<typename T>
void RotationConverter<T>::convert(const Quaternion<T> &quaternion, RotationMatrix<T> &rotationMatrix)
{
  rotationMatrix = RotationMatrix<T>::identity();

  T two{2};

  T _2x = two * quaternion.x;
  T _2y = two * quaternion.y;
  T _2z = two * quaternion.z;
  T _2xx = _2x * quaternion.x;
  T _2xy = _2x * quaternion.y;
  T _2xz = _2x * quaternion.z;
  T _2xw = _2x * quaternion.w;
  T _2yy = _2y * quaternion.y;
  T _2yz = _2y * quaternion.z;
  T _2yw = _2y * quaternion.w;
  T _2zz = _2z * quaternion.z;
  T _2zw = _2z * quaternion.w;

  T one{1};

  rotationMatrix.at(0, 0) = one - _2yy - _2zz;
  rotationMatrix.at(0, 1) = _2xy - _2zw;
  rotationMatrix.at(0, 2) = _2xz + _2yw;
  rotationMatrix.at(1, 0) = _2xy + _2zw;
  rotationMatrix.at(1, 1) = one - _2xx - _2zz;
  rotationMatrix.at(1, 2) = _2yz - _2xw;
  rotationMatrix.at(2, 0) = _2xz - _2yw;
  rotationMatrix.at(2, 1) = _2yz + _2xw;
  rotationMatrix.at(2, 2) = one - _2xx - _2yy;
}

template<typename T>
void RotationConverter<T>::convert(const EulerAngles<T> &eulerAngles, Quaternion<T> &quaternion)
{
  //RotationMatrix<T> rot_mat;
  //RotationConverter<T>::convert(eulerAngles, rot_mat);
  //RotationConverter<T>::convert(rot_mat, quaternion);

//  double cy = cos(yaw * 0.5);
//  double sy = sin(yaw * 0.5);
//  double cp = cos(pitch * 0.5);
//  double sp = sin(pitch * 0.5);
//  double cr = cos(roll * 0.5);
//  double sr = sin(roll * 0.5);

//  Quaterniond q;
//  q.w() = cy * cp * cr + sy * sp * sr;
//  q.x() = cy * cp * sr - sy * sp * cr;
//  q.y() = sy * cp * sr + cy * sp * cr;
//  q.z() = sy * cp * cr - cy * sp * sr;
//  return q;
}

template<typename T>
void RotationConverter<T>::convert(const EulerAngles<T> &eulerAngles, RotationMatrix<T> &rotationMatrix)
{
//  RotationMatrix<T> r0, r1, r2;
//  Convert(AxisAngle<N, Real>(Vector<N, Real>::Unit(e.axis[0]), e.angle[0]), r0);
//  Convert(AxisAngle<N, Real>(Vector<N, Real>::Unit(e.axis[1]), e.angle[1]), r1);
//  Convert(AxisAngle<N, Real>(Vector<N, Real>::Unit(e.axis[2]), e.angle[2]), r2);
//  r = r2*r1*r0;
}

} // Fin namespace math

} // End namespace tl

#endif // TL_MATH_ROTATIONS_H
