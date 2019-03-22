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

template<typename T>
void RotationConverter<T>::convert(const RotationMatrix<T> &rotationMatrix, Quaternion<T> &quaternion)
{
  T r22 = rotationMatrix.at(2,2);
  if (r22 <= static_cast<T>(0)) {
    T dif10 = rotationMatrix.at(1, 1) - rotationMatrix.at(0, 0);
    T omr22 = static_cast<T>(1) - r22;
    if (dif10 <= static_cast<T>(0)) {
      T fourXSqr = omr22 - dif10;
      T inv4x = static_cast<T>(0.5) / sqrt(fourXSqr);
      quaternion.x = fourXSqr*inv4x;
      quaternion.y = (rotationMatrix.at(0, 1) + rotationMatrix.at(1, 0)) * inv4x;
      quaternion.z = (rotationMatrix.at(0, 2) + rotationMatrix.at(2, 0)) * inv4x;
      quaternion.w = (rotationMatrix.at(2, 1) - rotationMatrix.at(1, 2)) * inv4x;
    } else {
      T fourYSqr = omr22 + dif10;
      T inv4y = static_cast<T>(0.5) / sqrt(fourYSqr);
      quaternion.x = (rotationMatrix.at(0, 1) + rotationMatrix.at(1, 0))*inv4y;
      quaternion.y = fourYSqr*inv4y;
      quaternion.z = (rotationMatrix.at(1, 2) + rotationMatrix.at(2, 1))*inv4y;
      quaternion.w = (rotationMatrix.at(0, 2) - rotationMatrix.at(2, 0))*inv4y;
    }
  } else {
    T sum10 = rotationMatrix.at(1, 1) + rotationMatrix.at(0, 0);
    T opr22 = static_cast<T>(1) + r22;
    if (sum10 <= static_cast<T>(0)) {
      T fourZSqr = opr22 - sum10;
      T inv4z = (static_cast<T>(0.5)) / sqrt(fourZSqr);
      quaternion.x = (rotationMatrix.at(0, 2) + rotationMatrix.at(2, 0))*inv4z;
      quaternion.y = (rotationMatrix.at(1, 2) + rotationMatrix.at(2, 1))*inv4z;
      quaternion.z = fourZSqr*inv4z;
      quaternion.w = (rotationMatrix.at(1, 0) - rotationMatrix.at(0, 1))*inv4z;
    } else {
      T fourWSqr = opr22 + sum10;
      T inv4w = static_cast<T>(0.5) / sqrt(fourWSqr);
      quaternion.x = (rotationMatrix.at(2, 1) - rotationMatrix.at(1, 2))*inv4w;
      quaternion.y = (rotationMatrix.at(0, 2) - rotationMatrix.at(2, 0))*inv4w;
      quaternion.z = (rotationMatrix.at(1, 0) - rotationMatrix.at(0, 1))*inv4w;
      quaternion.w = fourWSqr*inv4w;
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
