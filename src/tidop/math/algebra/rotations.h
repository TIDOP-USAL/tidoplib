#ifndef TL_MATH_ROTATIONS_H
#define TL_MATH_ROTATIONS_H

#include "config_tl.h"
#include "tidop/math/algebra/quaternion.h"
#include "tidop/math/algebra/euler_angles.h"
#include "tidop/math/algebra/rotation_matrix.h"

namespace TL
{

namespace math
{

//https://www.geometrictools.com/GTEngine/Include/Mathematics/GteRotation.h

template <typename T>
class Rotation
{

public:

private:


  Rotation() {}

public:

  static void convert(const RotationMatrix<T> &rotMatrix, Quaternion<T> &quaternion);
  static void convert(const RotationMatrix<T> &rotMatrix, EulerAngles<T> &eulerAngles);
  static void convert(const Quaternion<T> &quaternion, RotationMatrix<T> &rotMatrix);
  static void convert(const Quaternion<T> &quaternion, EulerAngles<T> &eulerAngles);
  static void convert(const EulerAngles<T> &eulerAngles, Quaternion<T> &quaternion);
  static void convert(const EulerAngles<T> &eulerAngles, RotationMatrix<T> &rotMatrix);

private:

};


template <typename T>
void Rotation<T>::convert(const RotationMatrix<T> &rotMatrix, Quaternion<T> &quaternion)
{
  T r22 = rotMatrix.at(2, 2);
  
  if (r22 <= static_cast<T>(0)) {
    T dif10 = rotMatrix.at(1, 1) - rotMatrix.at(0, 0);
    T omr22 = static_cast<T>(1) - r22;
    if (dif10 <= static_cast<T>(0)) {
      T fourXSqr = omr22 - dif10;
      T inv4x = 0.5 / sqrt(fourXSqr);
      quaternion.x = fourXSqr*inv4x;
      quaternion.y = (rotMatrix.at(0, 1) + rotMatrix.at(1, 0))*inv4x;
      quaternion.z = (rotMatrix.at(0, 2) + rotMatrix.at(2, 0))*inv4x;
      quaternion.w = (rotMatrix.at(2, 1) - rotMatrix.at(1, 2))*inv4x;
      /// otro método
      ///quaternion.w = (r(1, 2) - r(2, 1))*inv4x;
    } else {
      T sum10 = r(1, 1) + r(0, 0);
      T opr22 = static_cast<T>(1) + r22;
      if (sum10 <= static_cast<T>(0)) {
        T fourZSqr = opr22 - sum10;
        T inv4z = static_cast<T>(0.5) / sqrt(fourZSqr);
        quaternion.x = (r(0, 2) + r(2, 0))*inv4z;
        quaternion.y = (r(1, 2) + r(2, 1))*inv4z;
        quaternion.z = fourZSqr*inv4z;
        quaternion.w = (r(1, 0) - r(0, 1))*inv4z;
        ///quaternion.w = (r(0, 1) - r(1, 0))*inv4z;
      } else {
        T fourWSqr = opr22 + sum10;
        T inv4w = static_cast<T>(0.5) / sqrt(fourWSqr);
        quaternion.x = (r(2, 1) - r(1, 2))*inv4w;
        quaternion.y = (r(0, 2) - r(2, 0))*inv4w;
        quaternion.z = (r(1, 0) - r(0, 1))*inv4w;
        //q[0] = (r(1, 2) - r(2, 1))*inv4w;
        //q[1] = (r(2, 0) - r(0, 2))*inv4w;
        //q[2] = (r(0, 1) - r(1, 0))*inv4w;
        quaternion.w = fourWSqr*inv4w;
      }
    }
  }
}

template <typename T>
void Rotation<T>::convert(const RotationMatrix<T> &rotMatrix, EulerAngles<T> &eulerAngles)
{
  if (0 <= e.axis[0] && e.axis[0] < 3
      && 0 <= e.axis[1] && e.axis[1] < 3
      && 0 <= e.axis[2] && e.axis[2] < 3
      && e.axis[1] != e.axis[0]
      && e.axis[1] != e.axis[2]) {
    if (e.axis[0] != e.axis[2]) {
//#if defined(GTE_USE_MAT_VEC)
      // Map (0,1,2), (1,2,0), and (2,0,1) to +1.
      // Map (0,2,1), (2,1,0), and (1,0,2) to -1.
      int parity = (((e.axis[2] | (e.axis[1] << 2)) >> e.axis[0]) & 1);
      Real const sgn = (parity & 1 ? (Real)-1 : (Real)+1);

      if (r(e.axis[2], e.axis[0]) < (Real)1)
      {
        if (r(e.axis[2], e.axis[0]) > (Real)-1)
        {
          e.angle[2] = atan2(sgn*r(e.axis[1], e.axis[0]),
                             r(e.axis[0], e.axis[0]));
          e.angle[1] = asin(-sgn*r(e.axis[2], e.axis[0]));
          e.angle[0] = atan2(sgn*r(e.axis[2], e.axis[1]),
                             r(e.axis[2], e.axis[2]));
          e.result = ER_UNIQUE;
        }
        else
        {
          e.angle[2] = (Real)0;
          e.angle[1] = sgn*(Real)GTE_C_HALF_PI;
          e.angle[0] = atan2(-sgn*r(e.axis[1], e.axis[2]),
                             r(e.axis[1], e.axis[1]));
          e.result = ER_NOT_UNIQUE_DIF;
        }
      }
      else
      {
        e.angle[2] = (Real)0;
        e.angle[1] = -sgn*(Real)GTE_C_HALF_PI;
        e.angle[0] = atan2(-sgn*r(e.axis[1], e.axis[2]),
                           r(e.axis[1], e.axis[1]));
        e.result = ER_NOT_UNIQUE_SUM;
      }
//#else
//      // Map (0,1,2), (1,2,0), and (2,0,1) to +1.
//      // Map (0,2,1), (2,1,0), and (1,0,2) to -1.
//      int parity = (((e.axis[0] | (e.axis[1] << 2)) >> e.axis[2]) & 1);
//      Real const sgn = (parity & 1 ? (Real)+1 : (Real)-1);
//
//      if (r(e.axis[0], e.axis[2]) < (Real)1) {
//        if (r(e.axis[0], e.axis[2]) > (Real)-1) {
//          e.angle[0] = atan2(sgn*r(e.axis[1], e.axis[2]),
//                             r(e.axis[2], e.axis[2]));
//          e.angle[1] = asin(-sgn*r(e.axis[0], e.axis[2]));
//          e.angle[2] = atan2(sgn*r(e.axis[0], e.axis[1]),
//                             r(e.axis[0], e.axis[0]));
//          e.result = ER_UNIQUE;
//        } else {
//          e.angle[0] = (Real)0;
//          e.angle[1] = sgn*(Real)GTE_C_HALF_PI;
//          e.angle[2] = atan2(-sgn*r(e.axis[1], e.axis[0]),
//                             r(e.axis[1], e.axis[1]));
//          e.result = ER_NOT_UNIQUE_DIF;
//        }
//      } else {
//        e.angle[0] = (Real)0;
//        e.angle[1] = -sgn*(Real)GTE_C_HALF_PI;
//        e.angle[2] = atan2(-sgn*r(e.axis[1], e.axis[0]),
//                           r(e.axis[1], e.axis[1]));
//        e.result = ER_NOT_UNIQUE_SUM;
//      }
//#endif
    } else {
//#if defined(GTE_USE_MAT_VEC)
      // Map (0,2,0), (1,0,1), and (2,1,2) to +1.
      // Map (0,1,0), (1,2,1), and (2,0,2) to -1.
      int b0 = 3 - e.axis[1] - e.axis[2];
      int parity = (((b0 | (e.axis[1] << 2)) >> e.axis[2]) & 1);
      Real const sgn = (parity & 1 ? (Real)+1 : (Real)-1);

      if (r(e.axis[2], e.axis[2]) < (Real)1)
      {
        if (r(e.axis[2], e.axis[2]) > (Real)-1)
        {
          e.angle[2] = atan2(r(e.axis[1], e.axis[2]),
                             sgn*r(b0, e.axis[2]));
          e.angle[1] = acos(r(e.axis[2], e.axis[2]));
          e.angle[0] = atan2(r(e.axis[2], e.axis[1]),
                             -sgn*r(e.axis[2], b0));
          e.result = ER_UNIQUE;
        }
        else
        {
          e.angle[2] = (Real)0;
          e.angle[1] = (Real)GTE_C_PI;
          e.angle[0] = atan2(sgn*r(e.axis[1], b0),
                             r(e.axis[1], e.axis[1]));
          e.result = ER_NOT_UNIQUE_DIF;
        }
      }
      else
      {
        e.angle[2] = (Real)0;
        e.angle[1] = (Real)0;
        e.angle[0] = atan2(sgn*r(e.axis[1], b0),
                           r(e.axis[1], e.axis[1]));
        e.result = ER_NOT_UNIQUE_SUM;
      }
//#else
//      // Map (0,2,0), (1,0,1), and (2,1,2) to -1.
//      // Map (0,1,0), (1,2,1), and (2,0,2) to +1.
//      int b2 = 3 - e.axis[0] - e.axis[1];
//      int parity = (((b2 | (e.axis[1] << 2)) >> e.axis[0]) & 1);
//      Real const sgn = (parity & 1 ? (Real)-1 : (Real)+1);
//
//      if (r(e.axis[0], e.axis[0]) < (Real)1) {
//        if (r(e.axis[0], e.axis[0]) > (Real)-1) {
//          e.angle[0] = atan2(r(e.axis[1], e.axis[0]),
//                             sgn*r(b2, e.axis[0]));
//          e.angle[1] = acos(r(e.axis[0], e.axis[0]));
//          e.angle[2] = atan2(r(e.axis[0], e.axis[1]),
//                             -sgn*r(e.axis[0], b2));
//          e.result = ER_UNIQUE;
//        } else {
//          e.angle[0] = (Real)0;
//          e.angle[1] = (Real)GTE_C_PI;
//          e.angle[2] = atan2(sgn*r(e.axis[1], b2),
//                             r(e.axis[1], e.axis[1]));
//          e.result = ER_NOT_UNIQUE_DIF;
//        }
//      } else {
//        e.angle[0] = (Real)0;
//        e.angle[1] = (Real)0;
//        e.angle[2] = atan2(sgn*r(e.axis[1], b2),
//                           r(e.axis[1], e.axis[1]));
//        e.result = ER_NOT_UNIQUE_SUM;
//      }
//#endif
    }
  } else {
    // Invalid angles.
    e.angle[0] = (Real)0;
    e.angle[1] = (Real)0;
    e.angle[2] = (Real)0;
    e.result = ER_INVALID;
  }
}

template <typename T>
void Rotation<T>::convert(const Quaternion<T> &quaternion, RotationMatrix<T> &rotMatrix)
{
  rotMatrix = RotationMatrix<T>();

  T a =  2*quaternion.x;
  T b =  2*quaternion.y;
  T c =  2*quaternion.z;
  T ax = a*quaternion.x;
  T ay = a*quaternion.y;
  T az = a*quaternion.z;
  T aw = a*quaternion.w;
  T by = b*quaternion.y;
  T bz = b*quaternion.z;
  T bw = b*quaternion.w;
  T cz = c*quaternion.z;
  T cw = c*quaternion.w;

//#if defined(GTE_USE_MAT_VEC)
    r(0, 0) = 1 - by - cz;
    r(0, 1) = ay - cw;
    r(0, 2) = az + bw;
    r(1, 0) = ay + cw;
    r(1, 1) = 1 - ax - cz;
    r(1, 2) = bz - aw;
    r(2, 0) = az - bw;
    r(2, 1) = bz + aw;
    r(2, 2) = 1 - ax - by;
//#else
//    r(0, 0) = 1 - by - cz;
//    r(1, 0) = ay - cw;
//    r(2, 0) = az + bw;
//    r(0, 1) = ay + cw;
//    r(1, 1) = 1 - ax - cz;
//    r(2, 1) = bz - aw;
//    r(0, 2) = az - bw;
//    r(1, 2) = bz + aw;
//    r(2, 2) = 1 - ax - by;
//#endif
}

template <typename T>
void Rotation<T>::Convert(const Quaternion<T> &quaternion, EulerAngles<T> &eulerAngles)
{

}

template <typename T>
void Rotation<T>::convert(const EulerAngles<T> &eulerAngles, Quaternion<T> &quaternion)
{
}

template <typename T>
void Rotation<T>::Convert(const EulerAngles<T> &eulerAngles, RotationMatrix<T> &rotMatrix)
{

}


} // Fin namespace math

} // End namespace TL

#endif // TL_MATH_ROTATIONS_H
