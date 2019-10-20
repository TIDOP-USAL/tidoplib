#ifndef TL_MATH_ROTATIONS_H
#define TL_MATH_ROTATIONS_H

#include "config_tl.h"

//#include "tidop/math/mathutils.h"
#include "tidop/math/algebra/quaternion.h"
#include "tidop/math/algebra/euler_angles.h"
#include "tidop/math/algebra/rotation_matrix.h"
#include "tidop/math/algebra/axis_angle.h"

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
   * \brief Pasa de una matriz de rotación a cuaterniones
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

  /*!
   * \brief Convierte una rotación como cuaterniones a matriz de rotación
   *
   * \f[ r00 = 1 - 2 * y² - 2 * z \f]
   * \f[ r01 = 2 * x * y - 2 * z * w  \f]
   * \f[ r02 = 2 * x * z + 2 * y * w \f]
   * \f[ r10 = 2 * x * y + 2 * z * w \f]
   * \f[ r11 = 1 - 2 * x² - 2 * z² \f]
   * \f[ r12 = 2 * y * z - 2 * x * w \f]
   * \f[ r20 = 2 * x * z - 2 * y * w \f]
   * \f[ r21 = 2 * y * z + 2 * x * w \f]
   * \f[ r22 = 1 - 2 * x² - 2 * y² \f]
   *
   * \param[in] quaternion
   * \param[out] rotationMatrix
   */
  static void convert(const Quaternion<T> &quaternion, RotationMatrix<T> &rotationMatrix);

  /*!
   * \brief convert
   * \param[in] quaternion
   * \param[out] axisAngle
   */
  static void convert(const Quaternion<T> &quaternion, AxisAngle<T> &axisAngle);
  static void convert(const AxisAngle<T> &axisAngle, Quaternion<T> &quaternion);

  /*!
   * \brief convert
   * \param rotationMatrix
   * \param axisAngle
   */
  static void convert(const RotationMatrix<T> &rotationMatrix, AxisAngle<T> &axisAngle);
  static void convert(const AxisAngle<T> &axisAngle, RotationMatrix<T> &rotationMatrix);


  static void convert(const RotationMatrix<T> &rotationMatrix, EulerAngles<T> &eulerAngles);
  static void convert(const EulerAngles<T> &eulerAngles, RotationMatrix<T> &rotationMatrix);

  static void convert(const Quaternion<T> &quaternion, EulerAngles<T> &eulerAngles);

  /*!
   * \brief Convierte una rotación como ángulos de Euler a cuaterniones
   * https://ntrs.nasa.gov/archive/nasa/casi.ntrs.nasa.gov/19770024290.pdf
   * \param[in] eulerAngles Rotación como ángulos de euler
   * \param[out] quaternion Rotación como cuaterniones
   */
  static void convert(const EulerAngles<T> &eulerAngles, Quaternion<T> &quaternion);


  static void convert(const EulerAngles<T> &eulerAngles, AxisAngle<T> &axisAngle);
  static void convert(const AxisAngle<T> &axisAngle, EulerAngles<T> &eulerAngles);

private:

};

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
  rotationMatrix.at(0, 1) = _2xy + _2zw;
  rotationMatrix.at(0, 2) = _2xz - _2yw;
  rotationMatrix.at(1, 0) = _2xy - _2zw;
  rotationMatrix.at(1, 1) = one - _2xx - _2zz;
  rotationMatrix.at(1, 2) = _2yz + _2xw;
  rotationMatrix.at(2, 0) = _2xz + _2yw;
  rotationMatrix.at(2, 1) = _2yz - _2xw;
  rotationMatrix.at(2, 2) = one - _2xx - _2yy;
}

template<typename T>
void RotationConverter<T>::convert(const Quaternion<T> &quaternion,
                                   AxisAngle<T> &axisAngle)
{
  T one{1};
  T zero{0};

  T n2 = std::sqrt(quaternion.x * quaternion.x + quaternion.y * quaternion.y + quaternion.z * quaternion.z);
  if (n2 > zero){
    axisAngle.axis[0] = quaternion.x / n2;
    axisAngle.axis[1] = quaternion.y / n2;
    axisAngle.axis[2] = quaternion.z / n2;
    axisAngle.angle = T{2}*std::acos(std::min(std::max(-one, quaternion.w), one));
  } else {
    axisAngle.axis[0] = one;
    axisAngle.axis[1] = zero;
    axisAngle.axis[2] = zero;
    axisAngle.angle = zero;
  }
}

template<typename T>
void RotationConverter<T>::convert(const AxisAngle<T> &axisAngle, Quaternion<T> &quaternion)
{
  T a_2 = axisAngle.angle * T{0.5};
  T sin_a_2 = std::sin(a_2);
  quaternion.x = sin_a_2 * axisAngle.axis[0];
  quaternion.y = sin_a_2 * axisAngle.axis[1];
  quaternion.z = sin_a_2 * axisAngle.axis[2];
  quaternion.w = std::cos(a_2);
}

template<typename T>
void RotationConverter<T>::convert(const RotationMatrix<T> &rotationMatrix, AxisAngle<T> &axisAngle)
{
  Quaternion<T> quaternion;
  convert(rotationMatrix, quaternion);
  convert(quaternion, axisAngle);
}



template<typename T>
void RotationConverter<T>::convert(const AxisAngle<T> &axisAngle, RotationMatrix<T> &rotationMatrix)
{
  rotationMatrix = RotationMatrix<T>::identity();

  T ca = std::cos(axisAngle.angle);
  T sa = std::sin(axisAngle.angle);
  T _1mca = T{1} - ca;
  T xy1mca = axisAngle.axis[0] * axisAngle.axis[1] * _1mca;
  T xz1mca = axisAngle.axis[0] * axisAngle.axis[2] * _1mca;
  T yz1mca = axisAngle.axis[1] * axisAngle.axis[2] * _1mca;
  T xsa = axisAngle.axis[0] * sa;
  T ysa = axisAngle.axis[1] * sa;
  T zsa = axisAngle.axis[2] * sa;

  rotationMatrix.at(0, 0) = axisAngle.axis[0] * axisAngle.axis[0] * _1mca + ca;
  rotationMatrix.at(1, 0) = xy1mca - zsa;
  rotationMatrix.at(2, 0) = xz1mca + ysa;
  rotationMatrix.at(0, 1) = xy1mca + zsa;
  rotationMatrix.at(1, 1) = axisAngle.axis[1] * axisAngle.axis[1] * _1mca + ca;
  rotationMatrix.at(2, 1) = yz1mca - xsa;
  rotationMatrix.at(0, 2) = xz1mca - ysa;
  rotationMatrix.at(1, 2) = yz1mca + xsa;
  rotationMatrix.at(2, 2) = axisAngle.axis[2] * axisAngle.axis[2] *_1mca + ca;
}


template<typename T>
void RotationConverter<T>::convert(const RotationMatrix<T> &rotationMatrix,
                                   EulerAngles<T> &eulerAngles)
{
  /// https://www.geometrictools.com/GTEngine/Include/Mathematics/GteRotation.h
  Vector<3, int> axis;
  T one{1};
  T cero{0};

  // Tait-Bryan angles
  auto clamp = [&](const T& value, const T& _min, const T& _max) {
    return std::max(_min, std::min(_max, value));
  };

  typename EulerAngles<T>::Axes axes = eulerAngles.axes;

  // Tait-Bryan angles
  if (axes == EulerAngles<T>::Axes::xyz) {

    eulerAngles.phi = asin(clamp(rotationMatrix.at(0, 2), -1, 1));

    if (std::abs(rotationMatrix.at(0, 2)) < one) {

      eulerAngles.omega = atan2(-rotationMatrix.at(1,2), rotationMatrix.at(2,2));
      eulerAngles.kappa = atan2(-rotationMatrix.at(0,1), rotationMatrix.at(0,0));

    } else {

      eulerAngles.omega = atan2(rotationMatrix.at(2,1), rotationMatrix.at(1,1));
      eulerAngles.kappa = 0;

    }

  } else if (axes == EulerAngles<T>::Axes::yxz) {

    eulerAngles.omega = asin(-clamp(rotationMatrix.at(1, 2), - 1, 1));

    if (std::abs(rotationMatrix.at(1, 2)) < one) {

      eulerAngles.phi = atan2(rotationMatrix.at(0,2), rotationMatrix.at(2,2));
      eulerAngles.kappa = atan2(rotationMatrix.at(1,0), rotationMatrix.at(1,1));

    } else {

      eulerAngles.phi = atan2(-rotationMatrix.at(2,0), rotationMatrix.at(0,0));
      eulerAngles.kappa = 0;

    }

  } else if (axes == EulerAngles<T>::Axes::zxy) {


    eulerAngles.omega = asin(clamp(rotationMatrix.at(2,1), -1, 1));

    if (abs(rotationMatrix.at(2,1)) < one) {

      eulerAngles.phi = atan2(-rotationMatrix.at(2,0), rotationMatrix.at(2,2));
      eulerAngles.kappa = atan2(-rotationMatrix.at(0,1), rotationMatrix.at(1,1));

    } else {

      eulerAngles.phi = 0;
      eulerAngles.kappa = atan2(rotationMatrix.at(1,0), rotationMatrix.at(0,0));

    }

  } else if (axes == EulerAngles<T>::Axes::zyx) {

    eulerAngles.phi = asin(-clamp(rotationMatrix.at(2,0), -1, 1));

    if (abs(rotationMatrix.at(2,0)) < one) {

      eulerAngles.omega = atan2(rotationMatrix.at(2,1), rotationMatrix.at(2,2));
      eulerAngles.kappa = atan2(rotationMatrix.at(1,0), rotationMatrix.at(0,0));

    } else {

      eulerAngles.omega = 0;
      eulerAngles.kappa = atan2(-rotationMatrix.at(0,1), rotationMatrix.at(1,1));

    }

  } else if (axes == EulerAngles<T>::Axes::yzx) {

    eulerAngles.kappa = asin(clamp(rotationMatrix.at(1,0), -1, 1));

    if (abs(rotationMatrix.at(1,0)) < 0.99999) {

      eulerAngles.omega = atan2(-rotationMatrix.at(1,2), rotationMatrix.at(1,1));
      eulerAngles.phi = atan2(-rotationMatrix.at(2,0), rotationMatrix.at(0,0));

    } else {

      eulerAngles.omega = 0;
      eulerAngles.phi = atan2(rotationMatrix.at(0,2), rotationMatrix.at(2,2));

    }

  } else if (axes == EulerAngles<T>::Axes::xzy) {

    eulerAngles.kappa = asin(-clamp(rotationMatrix.at(0,1), -1, 1));

    if (abs(rotationMatrix.at(0,1)) < one) {

      eulerAngles.omega = atan2(rotationMatrix.at(2,1), rotationMatrix.at(1,1));
      eulerAngles.phi = atan2(rotationMatrix.at(0,2), rotationMatrix.at(0,0));

    } else {

      eulerAngles.omega = atan2(-rotationMatrix.at(1,2), rotationMatrix.at(2,2));
      eulerAngles.phi = 0;

    }

  } else if (axes == EulerAngles<T>::Axes::xyx) {


  }
  // Euler angles
//  else {
//    if (eulerAngles.axes == EulerAngles<T>::Axes::xyx) {
//      axis = {0,1,0};
//    } else if (eulerAngles.axes == EulerAngles<T>::Axes::xzx) {
//      axis = {0,2,0};
//    } else if (eulerAngles.axes == EulerAngles<T>::Axes::yxy) {
//      axis = {1,0,1};
//    } else if (eulerAngles.axes == EulerAngles<T>::Axes::yzy) {
//      axis = {1,2,1};
//    } else if (eulerAngles.axes == EulerAngles<T>::Axes::zxz) {
//      axis = {2,0,2};
//    } else if (eulerAngles.axes == EulerAngles<T>::Axes::zyz) {
//      axis = {2,1,2};
//    }

//    //#if defined(GTE_USE_MAT_VEC)
//    //          // Map (0,2,0), (1,0,1), and (2,1,2) to +1.
//    //          // Map (0,1,0), (1,2,1), and (2,0,2) to -1.
//    //          int b0 = 3 - axis[1] - axis[2];
//    //          int parity = (((b0 | (axis[1] << 2)) >> axis[2]) & 1);
//    //          Real const sgn = (parity & 1 ? (Real)+1 : (Real)-1);

//    //          if (rotationMatrix.at(axis[2], axis[2]) < (Real)1)
//    //          {
//    //              if (rotationMatrix.at(axis[2], axis[2]) > (Real)-1)
//    //              {
//    //                  eulerAngles.angle[2] = std::atan2(rotationMatrix.at(axis[1], axis[2]),
//    //                      sgn*rotationMatrix.at(b0, axis[2]));
//    //                  eulerAngles.angle[1] = std::acos(rotationMatrix.at(axis[2], axis[2]));
//    //                  eulerAngles.angle[0] = std::atan2(rotationMatrix.at(axis[2], axis[1]),
//    //                      -sgn*rotationMatrix.at(axis[2], b0));
//    //                  e.result = ER_UNIQUE;
//    //              }
//    //              else
//    //              {
//    //                  eulerAngles.angle[2] = (Real)0;
//    //                  eulerAngles.angle[1] = (Real)GTE_C_PI;
//    //                  eulerAngles.angle[0] = std::atan2(sgn*rotationMatrix.at(axis[1], b0),
//    //                      rotationMatrix.at(axis[1], axis[1]));
//    //                  e.result = ER_NOT_UNIQUE_DIF;
//    //              }
//    //          }
//    //          else
//    //          {
//    //              eulerAngles.angle[2] = (Real)0;
//    //              eulerAngles.angle[1] = (Real)0;
//    //              eulerAngles.angle[0] = std::atan2(sgn*rotationMatrix.at(axis[1], b0),
//    //                  rotationMatrix.at(axis[1], axis[1]));
//    //              e.result = ER_NOT_UNIQUE_SUM;
//    //          }
//    //#else
//              // Map (0,2,0), (1,0,1), and (2,1,2) to -1.
//              // Map (0,1,0), (1,2,1), and (2,0,2) to +1.
//        int b2 = 3 - axis[0] - axis[1];
//        int parity = (((b2 | (axis[1] << 2)) >> axis[0]) & 1);
//        T sgn = (parity & 1 ? -one : one);
//        if (rotationMatrix.at(axis[0], axis[0]) < one) {
//          if (rotationMatrix.at(axis[0], axis[0]) > -one) {
//            eulerAngles.omega = std::atan2(rotationMatrix.at(axis[1], axis[0]), sgn*rotationMatrix.at(b2, axis[0]));
//            eulerAngles.phi = std::acos(rotationMatrix.at(axis[0], axis[0]));
//            eulerAngles.kappa = std::atan2(rotationMatrix.at(axis[0], axis[1]), -sgn*rotationMatrix.at(axis[0], b2));
//          } else {
//            eulerAngles.omega = cero;
//            eulerAngles.phi = static_cast<T>(TL_PI);
//            eulerAngles.kappa = std::atan2(sgn*rotationMatrix.at(axis[1], b2), rotationMatrix.at(axis[1], axis[1]));
//          }
//        } else {
//          eulerAngles.omega = cero;
//          eulerAngles.phi = cero;
//          eulerAngles.kappa = std::atan2(sgn*rotationMatrix.at(axis[1], b2), rotationMatrix.at(axis[1], axis[1]));
//        }
//    //#endif

//  }

}

template<typename T>
void RotationConverter<T>::convert(const EulerAngles<T> &eulerAngles, RotationMatrix<T> &rotationMatrix)
{
//  RotationMatrix<T> r0, r1, r2;
//  Convert(AxisAngle<N, Real>(Vector<N, Real>::Unit(axis[0]), e.angle[0]), r0);
//  Convert(AxisAngle<N, Real>(Vector<N, Real>::Unit(axis[1]), e.angle[1]), r1);
//  Convert(AxisAngle<N, Real>(Vector<N, Real>::Unit(axis[2]), e.angle[2]), r2);
//  r = r2*r1*r0;

}


template<typename T>
void RotationConverter<T>::convert(const Quaternion<T> &quaternion, EulerAngles<T> &eulerAngles)
{
  RotationMatrix<T> rotationMatrix;
  convert(quaternion, rotationMatrix);
  convert(rotationMatrix, eulerAngles);
}

template<typename T>
void RotationConverter<T>::convert(const EulerAngles<T> &eulerAngles, Quaternion<T> &quaternion)
{
  T omega = eulerAngles.omega;
  T phi = eulerAngles.phi;
  T kappa = eulerAngles.kappa;
  typename EulerAngles<T>::Axes axes = eulerAngles.axes;

  T c1 = cos(omega / 2.);
  T c2 = cos(phi / 2.);
  T c3 = cos(kappa / 2.);

  T s1 = sin(omega / 2.);
  T s2 = sin(phi / 2.);
  T s3 = sin(kappa / 2.);

  // Tait-Bryan angles
  if (axes == EulerAngles<T>::Axes::xyz) {

    quaternion.x = s1 * c2 * c3 + c1 * s2 * s3;
    quaternion.y = c1 * s2 * c3 - s1 * c2 * s3;
    quaternion.z = c1 * c2 * s3 + s1 * s2 * c3;
    quaternion.w = c1 * c2 * c3 - s1 * s2 * s3;

  } else if (axes == EulerAngles<T>::Axes::yxz) {

    quaternion.x = s1 * c2 * c3 + c1 * s2 * s3;
    quaternion.y = c1 * s2 * c3 - s1 * c2 * s3;
    quaternion.z = c1 * c2 * s3 - s1 * s2 * c3;
    quaternion.w = c1 * c2 * c3 + s1 * s2 * s3;

  } else if (axes == EulerAngles<T>::Axes::zxy) {

    quaternion.x = s1 * c2 * c3 - c1 * s2 * s3;
    quaternion.y = c1 * s2 * c3 + s1 * c2 * s3;
    quaternion.z = c1 * c2 * s3 + s1 * s2 * c3;
    quaternion.w = c1 * c2 * c3 - s1 * s2 * s3;

  } else if (axes == EulerAngles<T>::Axes::zyx) {

    quaternion.x = s1 * c2 * c3 - c1 * s2 * s3;
    quaternion.y = c1 * s2 * c3 + s1 * c2 * s3;
    quaternion.z = c1 * c2 * s3 - s1 * s2 * c3;
    quaternion.w = c1 * c2 * c3 + s1 * s2 * s3;

  } else if (axes == EulerAngles<T>::Axes::yzx) {

    quaternion.x = s1 * c2 * c3 + c1 * s2 * s3;
    quaternion.y = c1 * s2 * c3 + s1 * c2 * s3;
    quaternion.z = c1 * c2 * s3 - s1 * s2 * c3;
    quaternion.w = c1 * c2 * c3 - s1 * s2 * s3;

  } else if (axes == EulerAngles<T>::Axes::xzy) {

    quaternion.x = s1 * c2 * c3 - c1 * s2 * s3;
    quaternion.y = c1 * s2 * c3 - s1 * c2 * s3;
    quaternion.z = c1 * c2 * s3 + s1 * s2 * c3;
    quaternion.w = c1 * c2 * c3 + s1 * s2 * s3;

  }
  // Euler angles
  else if (axes == EulerAngles<T>::Axes::xyx) {

    quaternion.x = c2 * sin((omega+kappa)/2.);
    quaternion.y = s2 * cos((omega-kappa)/2.);
    quaternion.z = s2 * sin((omega-kappa)/2.);
    quaternion.w = c2 * cos((omega+kappa)/2.);

  } else if (axes == EulerAngles<T>::Axes::xzx) {

    quaternion.x = c2 * sin((omega+kappa)/2.);
    quaternion.y = -s2 * sin((omega-kappa)/2.);
    quaternion.z = s2 * cos((omega-kappa)/2.);
    quaternion.w = c2 * cos((omega+kappa)/2.);

  } else if (axes == EulerAngles<T>::Axes::yxy) {


    quaternion.x = s2 * cos((omega-kappa)/2.);
    quaternion.y = c2 * sin((omega+kappa)/2.);
    quaternion.z = -s2 * sin((omega-kappa)/2.);
    quaternion.w = c2 * cos((omega+kappa)/2.);

  } else if (axes == EulerAngles<T>::Axes::yzy) {

    quaternion.x = s2 * sin((omega-kappa)/2.);
    quaternion.y = c2 * sin((omega+kappa)/2.);
    quaternion.z = s2 * cos((omega-kappa)/2.);
    quaternion.w = c2 * cos((omega+kappa)/2.);

  } else if (axes == EulerAngles<T>::Axes::zxz) {

    quaternion.x = s2 * cos((omega-kappa)/2.);
    quaternion.y = s2 * sin((omega-kappa)/2.);
    quaternion.z = c2 * cos((omega+kappa)/2.);
    quaternion.w = c2 * cos((omega+kappa)/2.);

  } else if (axes == EulerAngles<T>::Axes::zyz) {

    quaternion.x = -s2 * sin((omega-kappa)/2.);
    quaternion.y = s2 * cos((omega-kappa)/2.);
    quaternion.z = c2 * sin((omega+kappa)/2.);
    quaternion.w = c2 * cos((omega+kappa)/2.);

  }
}

template<typename T>
void RotationConverter<T>::convert(const EulerAngles<T> &eulerAngles, AxisAngle<T> &axisAngle)
{
  Quaternion<T> quaternion;
  convert(eulerAngles, quaternion);
  convert(quaternion, axisAngle);
}

template<typename T>
void RotationConverter<T>::convert(const AxisAngle<T> &axisAngle, EulerAngles<T> &eulerAngles)
{
  Quaternion<T> quaternion;
  convert(axisAngle, quaternion);
  convert(quaternion, eulerAngles);
}


} // Fin namespace math

} // End namespace tl

#endif // TL_MATH_ROTATIONS_H
