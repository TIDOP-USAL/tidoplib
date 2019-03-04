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

/*!
 * \brief Clase cuaterni�n para la representaci�n de orientaciones y rotaciones en el espacio
 * Los cuaterniones unitarios proporcionan una notaci�n matem�tica para representar 
 * las orientaciones y las rotaciones de objetos en tres dimensiones. Comparados con 
 * los �ngulos de Euler, son m�s simples de componer y evitan el problema del bloqueo 
 * del card�n. Comparados con las matrices de rotaci�n, son m�s eficientes y m�s 
 * estables num�ricamente.
 *
 * Un cuaterni�n se representa como:
 * \f[ w+xi+yj+zk \f]
 */
template<typename T>
class Quaternion
{

public:

  T x;
  T y;
  T z;
  T w;

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
   * \brief Constructor a partir de una matriz de rotaci�n
   * Inicializa los cuaterniones con una matriz de rotaci�n
   * 
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
   * [GTE_USE_MAT_VEC]
   *   x*w = (r21 - r12)/4
   *   y*w = (r02 - r20)/4
   *   z*w = (r10 - r01)/4
   * [GTE_USE_VEC_MAT]
   *   x*w = (r12 - r21)/4
   *   y*w = (r20 - r02)/4
   *   z*w = (r01 - r10)/4
   * 
   * If Q is the 4x1 column vector (x,y,z,w), the previous equations give us
   *         +-                  -+
   *         | x*x  x*y  x*z  x*w |
   * Q*Q^T = | y*x  y*y  y*z  y*w |
   *         | z*x  z*y  z*z  z*w |
   *         | w*x  w*y  w*z  w*w |
   *         +-                  -+
   * The code extracts the row of maximum length, normalizing it to obtain
   * the result q.
   *
   * \param[in] rot Matriz de rotaci�n
   */
  Quaternion(const RotationMatrix<3, T> &rot);

  /*!
   * \brief destructora
   */
  ~Quaternion();

  /*!
   * \brief Operador de asignaci�n
   * \param[in] quat Objeto que se copia
   */
  Quaternion &operator = (const Quaternion<T> &quaternion);

  /*!
   * \brief Conjugado de un cuaterni�n
   * El conjugado de cuaterni�n invierte el signo de los componentes 
   * "agregados" del cuaterni�n:
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
   * \brief Normaliza el cuaterni�n
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

private:

};

/* Definici�n de alias Matrix */

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

// https://www.geometrictools.com/GTEngine/Include/Mathematics/GteRotation.h
template<typename T>
Quaternion<T>::Quaternion(const RotationMatrix<3, T> &rot)
{
  //static_assert(N == 3 || N == 4, "Dimension must be 3 or 4.");

  T r22 = rot.at(2,2);
  if (r22 <= static_cast<T>(0)) {
    T dif10 = rot.at(1, 1) - rot.at(0, 0);
    T omr22 = static_cast<T>(1) - r22;
    if (dif10 <= static_cast<T>(0)) {
      T fourXSqr = omr22 - dif10;
      T inv4x = static_cast<T>(0.5) / std::sqrt(fourXSqr);
      this->x = fourXSqr*inv4x;
      this->y = (rot.at(0, 1) + rot.at(1, 0))*inv4x;
      this->z = (rot.at(0, 2) + rot.at(2, 0))*inv4x;
//#if defined(GTE_USE_MAT_VEC)
      this->w = (rot.at(2, 1) - rot.at(1, 2))*inv4x;
//#else
//      this->w = (rot.at(1, 2) - rot.at(2, 1))*inv4x;
//#endif
    } else {
      T fourYSqr = omr22 + dif10;
      T inv4y = static_cast<T>(0.5) / std::sqrt(fourYSqr);
      this->x = (rot.at(0, 1) + rot.at(1, 0))*inv4y;
      this->y = fourYSqr*inv4y;
      this->z = (rot.at(1, 2) + rot.at(2, 1))*inv4y;
//#if defined(GTE_USE_MAT_VEC)
      this->w = (rot.at(0, 2) - rot.at(2, 0))*inv4y;
//#else
//      this->w = (rot.at(2, 0) - rot.at(0, 2))*inv4y;
//#endif
    }
  } else {
    T sum10 = rot.at(1, 1) + rot.at(0, 0);
    T opr22 = static_cast<T>(1) + r22;
    if (sum10 <= static_cast<T>(0)) {
      T fourZSqr = opr22 - sum10;
      T inv4z = (static_cast<T>(0.5)) / std::sqrt(fourZSqr);
      this->x = (rot.at(0, 2) + rot.at(2, 0))*inv4z;
      this->y = (rot.at(1, 2) + rot.at(2, 1))*inv4z;
      this->z = fourZSqr*inv4z;
//#if defined(GTE_USE_MAT_VEC)
      this->w = (rot.at(1, 0) - rot.at(0, 1))*inv4z;
//#else
//      this->w = (rot.at(0, 1) - rot.at(1, 0))*inv4z;
//#endif
    } else {
      T fourWSqr = opr22 + sum10;
      T inv4w = static_cast<T>(0.5) / std::sqrt(fourWSqr);
//#if defined(GTE_USE_MAT_VEC)
      this->x = (rot.at(2, 1) - rot.at(1, 2))*inv4w;
      this->y = (rot.at(0, 2) - rot.at(2, 0))*inv4w;
      this->z = (rot.at(1, 0) - rot.at(0, 1))*inv4w;
//#else
//      this->x = (rot.at(1, 2) - rot.at(2, 1))*inv4w;
//      this->y = (rot.at(2, 0) - rot.at(0, 2))*inv4w;
//      this->z = (rot.at(0, 1) - rot.at(1, 0))*inv4w;
//#endif
      this->w = fourWSqr*inv4w;
    }
  }
}

template<typename T>
Quaternion<T>::~Quaternion()
{}

template<typename T>
Quaternion<T>& Quaternion<T>::operator = (const Quaternion& quaternion)
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
  if (length > static_cast<T>(0)) {
    *this /= length;
  } else {
    *this = Quaternion<T>::zero();
  }
}

template <typename T>
Quaternion<T> Quaternion<T>::inverse() const
{
  T _dot = dot(*this, *this);
  if (_dot > static_cast<T>(0)) {
    Quaternion<T> inverse = this->conjugate() / _dot;
    return inverse;
  } else {
    return Quaternion<T>::zero();
  }
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
 * \brief Multiplicaci�n de cuaterniones
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
  return Quaternion<T>(quat1.x + quat2.x, 
                       quat1.y + quat2.y, 
                       quat1.z + quat2.z, 
                       quat1.w + quat2.w);
}

template<typename T>
Quaternion<T> operator - (const Quaternion<T> &quat1, 
                          const Quaternion<T> &quat2)
{
  Quaternion<T> q = quat1;
  return q -= quat2;
}

template<typename T>
Quaternion<T> operator*(const Quaternion<T> &quaternion, T scalar)
{
  Quaternion<T> q = quaternion;
  return q *= scalar;
}

template<typename T>
Quaternion<T> operator*(T scalar, const Quaternion<T> &quaternion)
{
  Quaternion<T> q = quaternion;
  return q *= scalar;
}

template<typename T>
Quaternion<T> operator / (const Quaternion<T> &quaternion, T scalar)
{
  Quaternion<T> q = quaternion;
  return q /= scalar;
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
Quaternion<T> &operator -= (Quaternion<T> &quat1, const Quaternion<T> &quat2)
{
  quat1.x -= quat2.x;
  quat1.y -= quat2.y;
  quat1.z -= quat2.z;
  quat1.w -= quat2.w;
  return quat1;
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

} // Fin namespace math

} // End namespace tl

#endif // TL_MATH_QUATERNION_H