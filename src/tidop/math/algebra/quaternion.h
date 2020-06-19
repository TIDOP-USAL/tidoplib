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

#ifndef TL_MATH_QUATERNION_H
#define TL_MATH_QUATERNION_H

#include "config_tl.h"

#include "tidop/math/algebra/rotation_matrix.h"

#include <vector>
#include <array>

namespace tl
{

namespace math
{

/*! \addtogroup Math
 *  \{
 */


/*! \addtogroup Algebra
 *  
 * Algebra
 *
 *  \{
 */

/*!
 * \brief Clase cuaternión para la representación de orientaciones y rotaciones en el espacio
 * Los cuaterniones unitarios proporcionan una notación matemática para representar 
 * las orientaciones y las rotaciones de objetos en tres dimensiones. Comparados con 
 * los ángulos de Euler, son más simples de componer y evitan el problema del bloqueo 
 * del cardán. Comparados con las matrices de rotación, son más eficientes y más 
 * estables numéricamente.
 *
 * Un cuaternión se representa como:
 * \f[ w+xi+yj+zk \f]
 */
template<typename T>
class Quaternion
{

public:

  /*!
   * \brief Constructor por defecto
   */
  Quaternion();
  
  /*!
   * \brief Constructor
   * \param[in] x Coeficiente x
   * \param[in] y Coeficiente y
   * \param[in] z Coeficiente z
   * \param[in] w Coeficiente w
   */
  Quaternion(T x, T y, T z, T w);

  /*!
   * \brief Constructor de copia
   * \param[in] quaternion Objeto Quaternion que se copia
   */
  Quaternion(const Quaternion<T> &quaternion);

  /*!
   * \brief destructora
   */
  ~Quaternion();

  /*!
   * \brief Operador de asignación
   * \param[in] quaternion Objeto que se copia
   */
  Quaternion &operator = (const Quaternion<T> &quaternion);

  /*!
   * \brief Conjugado de un cuaternión
   * El conjugado de cuaternión invierte el signo de los componentes 
   * "agregados" del cuaternión:
   * \f[ q = w-xi-yj-zk \f]
   */
  Quaternion<T> conjugate() const;

  /*!
   * \brief Norma
   * \f[ q = w+xi+yj+zk \f]
   * \f[ n(q) = sqrt{q.q} = sqrt{w^2+x^2+y^2+z^2} \f]
   */
  T norm() const;

  /*!
   * \brief Normaliza el cuaternión
   */
  void normalize();

  // For a nonzero quaternion q = (x,y,z,w), inv(q) = (-x,-y,-z,w)/|q|^2, where
  // |q| is the length of the quaternion.  When q is zero, the function returns
  // zero, which is considered to be an improbable case.
  Quaternion<T> inverse() const;

  /* Factory methods */

  /*!
   * \brief 
   * \f[ z = 0*i + 0*j + 0*k + 0 \f]
   */
  static Quaternion zero();

  /*!
   * \brief
   * \f[ i = 1*i + 0*j + 0*k + 0 \f]
   */
  static Quaternion i();

  /*!
   * \brief 
   * \f[ j = 0*i + 1*j + 0*k + 0 \f]
   */
  static Quaternion j();

  /*!
   * \brief 
   * \f[ k = 0*i + 0*j + 1*k + 0 \f]
   */
  static Quaternion k();

  /*!
   * \brief 
   * \f[ 1 = 0*i + 0*j + 0*k + 1 \f]
   */
  static Quaternion identity();


public:

  T x;
  T y;
  T z;
  T w;

};

/* Definición de alias Matrix */

typedef Quaternion<float>   Quaternionf;
typedef Quaternion<double>  Quaterniond;


template<typename T>
Quaternion<T>::Quaternion()
  : x(-std::numeric_limits<T>().max()), 
    y(-std::numeric_limits<T>().max()),
    z(-std::numeric_limits<T>().max()),
    w(-std::numeric_limits<T>().max())
{
}

template<typename T>
Quaternion<T>::Quaternion(T x, T y, T z, T w)
  : x(x), 
    y(y), 
    z(z), 
    w(w)
{
}

template<typename T>
Quaternion<T>::Quaternion(const Quaternion<T> &quaternion)
  : x(quaternion.x), 
    y(quaternion.y), 
    z(quaternion.z), 
    w(quaternion.w)
{
}

//template<typename T>
//Quaternion<T>::Quaternion(const RotationMatrix<T> &rot)
//{
//  T r22 = rot.at(2, 2);
//
//  if (r22 <= static_cast<T>(0)) {
//
//    T dif10 = rot.at(1, 1) - rot.at(0, 0);
//    T omr22 = static_cast<T>(1) - r22;
//    if (dif10 <= static_cast<T>(0)) {
//      T fourXSqr = omr22 - dif10;
//      T inv4x = static_cast<T>(0.5) / sqrt(fourXSqr);
//      this->x = fourXSqr*inv4x;
//      this->y = (rot.at(0, 1) + rot.at(1, 0)) * inv4x;
//      this->z = (rot.at(0, 2) + rot.at(2, 0)) * inv4x;
//      this->w = (rot.at(2, 1) - rot.at(1, 2)) * inv4x;
//    } else {
//      T fourYSqr = omr22 + dif10;
//      T inv4y = static_cast<T>(0.5) / sqrt(fourYSqr);
//      this->x = (rot.at(0, 1) + rot.at(1, 0))*inv4y;
//      this->y = fourYSqr*inv4y;
//      this->z = (rot.at(1, 2) + rot.at(2, 1))*inv4y;
//      this->w = (rot.at(0, 2) - rot.at(2, 0))*inv4y;
//    }
//
//  } else {
//
//    T sum10 = rot.at(1, 1) + rot.at(0, 0);
//    T opr22 = static_cast<T>(1) + r22;
//    if (sum10 <= static_cast<T>(0)) {
//      T fourZSqr = opr22 - sum10;
//      T inv4z = (static_cast<T>(0.5)) / sqrt(fourZSqr);
//      this->x = (rot.at(0, 2) + rot.at(2, 0))*inv4z;
//      this->y = (rot.at(1, 2) + rot.at(2, 1))*inv4z;
//      this->z = fourZSqr*inv4z;
//      this->w = (rot.at(1, 0) - rot.at(0, 1))*inv4z;
//    } else {
//      T fourWSqr = opr22 + sum10;
//      T inv4w = static_cast<T>(0.5) / sqrt(fourWSqr);
//      this->x = (rot.at(2, 1) - rot.at(1, 2))*inv4w;
//      this->y = (rot.at(0, 2) - rot.at(2, 0))*inv4w;
//      this->z = (rot.at(1, 0) - rot.at(0, 1))*inv4w;
//      this->w = fourWSqr*inv4w;
//    }
//
//  }
//}


template<typename T>
Quaternion<T>::~Quaternion()
{}

template<typename T>
Quaternion<T>& Quaternion<T>::operator = (const Quaternion &quaternion)
{
  this->x = quaternion.x;
  this->y = quaternion.y;
  this->z = quaternion.z;
  this->w = quaternion.w;
  return *this;
}

template<typename T>
Quaternion<T> Quaternion<T>::conjugate() const
{
  return Quaternion<T>(-x, -y, -z, w);
}

template<typename T>
T Quaternion<T>::norm() const
{
  return sqrt(x*x + y*y + z*z + w*w);
}

template<typename T>
void Quaternion<T>::normalize()
{
  T length = this->norm();
  *this /= length;
}

template <typename T>
Quaternion<T> Quaternion<T>::inverse() const
{
  T _dot = dot(*this, *this);
  Quaternion<T> inverse = this->conjugate() / _dot;
  return inverse;
}

template<typename T>
Quaternion<T> Quaternion<T>::zero()
{
  return Quaternion(static_cast<T>(0), 
                    static_cast<T>(0), 
                    static_cast<T>(0), 
                    static_cast<T>(0));
}

template<typename T>
Quaternion<T> Quaternion<T>::i()
{
  return Quaternion(static_cast<T>(1), 
                    static_cast<T>(0), 
                    static_cast<T>(0), 
                    static_cast<T>(0));
}

template<typename T>
Quaternion<T> Quaternion<T>::j()
{
  return Quaternion(static_cast<T>(0), 
                    static_cast<T>(1), 
                    static_cast<T>(0), 
                    static_cast<T>(0));
}

template<typename T>
Quaternion<T> Quaternion<T>::k()
{
  return Quaternion(static_cast<T>(0), 
                    static_cast<T>(0), 
                    static_cast<T>(1), 
                    static_cast<T>(0));
}

template<typename T>
Quaternion<T> Quaternion<T>::identity()
{
  return Quaternion(static_cast<T>(0), 
                    static_cast<T>(0), 
                    static_cast<T>(0), 
                    static_cast<T>(1));
}


/* Operaciones unarias */

template <typename T>
Quaternion<T> operator - (const Quaternion<T> &quaternion)
{
  return Quaternion<T>(-quaternion.x,
                       -quaternion.y, 
                       -quaternion.z,
                       -quaternion.w);
}

/* Operaciones entre cuaterniones */

/*!
 * \brief Multiplicación de cuaterniones
 * \f[ q1 = w1+x1*i+y1*j+z1*k \f]
 * \f[ q2 = w2+x2*i+y2*j+z2*k \f]
 * \f[ q1.q2 = \f]
 */
template<typename T>
Quaternion<T> operator *(const Quaternion<T> &quat1, 
                         const Quaternion<T> &quat2)
{
  return Quaternion<T>(
     quat1.x * quat2.w + quat1.y * quat2.z - quat1.z * quat2.y + quat1.w * quat2.x,
    -quat1.x * quat2.z + quat1.y * quat2.w + quat1.z * quat2.x + quat1.w * quat2.y,
     quat1.x * quat2.y - quat1.y * quat2.x + quat1.z * quat2.w + quat1.w * quat2.z,
    -quat1.x * quat2.x - quat1.y * quat2.y - quat1.z * quat2.z + quat1.w * quat2.w
  );
}

/*!
 * \brief Suma de cuaterniones
 * La suma de cuaterniones se realiza termino a termino
 * \f[ q1 = w1+x1i+y1j+z1k \f]
 * \f[ q2 = w2+x2i+y2j+z2k \f]
 * \f[ q = q1 + q2 \f]
 * \f[ q = (w1+w2) + (x1+x2)i + (y1+y2)j + (z1+z2)k \f]
 */
template<typename T>
Quaternion<T> operator + (const Quaternion<T> &quat1, 
                          const Quaternion<T> &quat2)
{
  Quaternion<T> q = quat1;
  return q += quat2;
}

template<typename T>
Quaternion<T> &operator += (Quaternion<T> &quat1, const Quaternion<T> &quat2)
{
  quat1.x += quat2.x;
  quat1.y += quat2.y;
  quat1.z += quat2.z;
  quat1.w += quat2.w;
  return quat1;
}

template<typename T>
Quaternion<T> operator - (const Quaternion<T> &quat1, 
                          const Quaternion<T> &quat2)
{
  Quaternion<T> q = quat1;
  return q -= quat2;
}

template<typename T>
Quaternion<T> &operator -= (Quaternion<T> &quat1, const Quaternion<T> &quat2)
{
  quat1.x -= quat2.x;
  quat1.y -= quat2.y;
  quat1.z -= quat2.z;
  quat1.w -= quat2.w;
  return quat1;
}

template<typename T>
Quaternion<T> operator * (const Quaternion<T> &quaternion, T scalar)
{
  Quaternion<T> q = quaternion;
  return q *= scalar;
}

template<typename T>
Quaternion<T> operator * (T scalar, const Quaternion<T> &quaternion)
{
  Quaternion<T> q = quaternion;
  return q *= scalar;
}

template<typename T>
Quaternion<T> &operator *= (Quaternion<T> &quaternion, T scalar)
{
  quaternion.x *= scalar;
  quaternion.y *= scalar;
  quaternion.z *= scalar;
  quaternion.w *= scalar;
  return quaternion;
}

template<typename T>
Quaternion<T> operator / (const Quaternion<T> &quaternion, T scalar)
{
  Quaternion<T> q = quaternion;
  return q /= scalar;
}

template<typename T>
Quaternion<T> operator / (T scalar, const Quaternion<T> &quaternion)
{
  Quaternion<T> q = quaternion;
  return q /= scalar;
}

template<typename T>
Quaternion<T> &operator /= (Quaternion<T> &quaternion, T scalar)
{
  if (scalar != static_cast<T>(0)) {
    quaternion.x /= scalar;
    quaternion.y /= scalar;
    quaternion.z /= scalar;
    quaternion.w /= scalar;
  } else {
    quaternion = Quaternion<T>::zero();
  }
  return quaternion;
}

template<typename T>
T dot(const Quaternion<T> &quat1, const Quaternion<T> &quat2)
{
  return quat1.x * quat2.x + quat1.y * quat2.y + quat1.z * quat2.z + quat1.w * quat2.w;
}

/*! \} */ // end of Algebra

/*! \} */ // end of Math

} // Fin namespace math

} // End namespace tl

#endif // TL_MATH_QUATERNION_H
