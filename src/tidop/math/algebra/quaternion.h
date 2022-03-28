/**************************************************************************
 *                                                                        *
 * Copyright (C) 2021 by Tidop Research Group                             *
 * Copyright (C) 2021 by Esteban Ruiz de Oña Crespo                       *
 *                                                                        *
 * This file is part of TidopLib                                          *
 *                                                                        *
 * TidopLib is free software: you can redistribute it and/or modify       *
 * it under the terms of the GNU Lesser General Public License as         *
 * published by the Free Software Foundation, either version 3 of the     *
 * License, or (at your option) any later version.                        *
 *                                                                        *
 * TidopLib is distributed in the hope that it will be useful,            *
 * but WITHOUT ANY WARRANTY; without even the implied warranty of         *
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the          *
 * GNU Lesser General Public License for more details.                    *
 *                                                                        *
 * You should have received a copy of the GNU Lesser General Public       *
 * License along with TidopLib. If not, see <http://www.gnu.org/licenses>.*
 *                                                                        *
 * @license LGPL-3.0 <https://www.gnu.org/licenses/lgpl-3.0.html>         *
 *                                                                        *
 **************************************************************************/

#ifndef TL_MATH_QUATERNION_H
#define TL_MATH_QUATERNION_H

#include "config_tl.h"

#include <vector>
#include <array>

#include "tidop/math/math.h"
#include "tidop/math/algebra/rotations.h"

namespace tl
{

namespace math
{

/*! \addtogroup math
 *  \{
 */

/*! \addtogroup algebra
 *  \{
 */

/*! \defgroup rotations Rotations
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
  : public RotationBase<T>
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
   * \brief Constructor de movimiento
   * \param[in] quaternion Objeto Quaternion que se mueve
   */
  Quaternion(Quaternion<T> &&quaternion) TL_NOEXCEPT;

  /*!
   * \brief destructora
   */
  ~Quaternion() override;

  /*!
   * \brief Operador de asignación
   * \param[in] quaternion Objeto que se copia
   */
  Quaternion &operator = (const Quaternion<T> &quaternion);

  /*!
   * \brief Operador de movimiento
   * \param[in] quaternion Objeto Quaternion que se mueve
   */
  Quaternion &operator = (Quaternion<T> &&quaternion) TL_NOEXCEPT;

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

  static Quaternion normalize(const Quaternion<T> &quaternion);

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
  : RotationBase<T>(Rotation::Type::quaternion),
    x(-std::numeric_limits<T>().max()), 
    y(-std::numeric_limits<T>().max()),
    z(-std::numeric_limits<T>().max()),
    w(-std::numeric_limits<T>().max())
{
}

template<typename T>
Quaternion<T>::Quaternion(T x, T y, T z, T w)
  : RotationBase<T>(Rotation::Type::quaternion),
    x(x), 
    y(y), 
    z(z), 
    w(w)
{
}

template<typename T>
Quaternion<T>::Quaternion(const Quaternion<T> &quaternion)
  : RotationBase<T>(Rotation::Type::quaternion),
    x(quaternion.x), 
    y(quaternion.y), 
    z(quaternion.z), 
    w(quaternion.w)
{
}

template<typename T>
Quaternion<T>::Quaternion(Quaternion<T> &&quaternion) TL_NOEXCEPT
  : RotationBase<T>(Rotation::Type::quaternion),
    x(std::move(quaternion.x)),
    y(std::move(quaternion.y)),
    z(std::move(quaternion.z)),
    w(std::move(quaternion.w))
{
}

template<typename T>
Quaternion<T>::~Quaternion()
{}

template<typename T>
Quaternion<T> &Quaternion<T>::operator = (const Quaternion &quaternion)
{
  if (this != &quaternion){
    this->x = quaternion.x;
    this->y = quaternion.y;
    this->z = quaternion.z;
    this->w = quaternion.w;
  }

  return *this;
}

template<typename T>
Quaternion<T>& Quaternion<T>::operator = (Quaternion &&quaternion) TL_NOEXCEPT
{
  if (this != &quaternion){
    RotationBase<T>::operator=(std::forward<RotationBase<T>>(quaternion));
    this->x = std::move(quaternion.x);
    this->y = std::move(quaternion.y);
    this->z = std::move(quaternion.z);
    this->w = std::move(quaternion.w);
  }

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
  return Quaternion(consts::zero<T>,
                    consts::zero<T>,
                    consts::zero<T>,
                    consts::zero<T>);
}

template<typename T>
Quaternion<T> Quaternion<T>::i()
{
  return Quaternion(consts::one<T>,
                    consts::zero<T>,
                    consts::zero<T>,
                    consts::zero<T>);
}

template<typename T>
Quaternion<T> Quaternion<T>::j()
{
  return Quaternion(consts::zero<T>,
                    consts::one<T>,
                    consts::zero<T>,
                    consts::zero<T>);
}

template<typename T>
Quaternion<T> Quaternion<T>::k()
{
  return Quaternion(consts::zero<T>,
                    consts::zero<T>,
                    consts::one<T>,
                    consts::zero<T>);
}

template<typename T>
Quaternion<T> Quaternion<T>::identity()
{
  return Quaternion(consts::zero<T>,
                    consts::zero<T>,
                    consts::zero<T>,
                    consts::one<T>);
}

template<typename T> inline
Quaternion<T> Quaternion<T>::normalize(const Quaternion<T> &quaternion)
{
  Quaternion<T> _quaternion(quaternion);
  _quaternion.normalize();
  return _quaternion;
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
  if (scalar != consts::zero<T>) {
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

template<typename T> static inline
bool operator == (const Quaternion<T> &q1, const Quaternion<T> &q2)
{
  return q1.x == q2.x && q1.y == q2.y && q1.z == q2.z && q1.w == q2.w;
}

template<typename T> static inline
bool operator != (const Quaternion<T> &q1, const Quaternion<T> &q2)
{
  return q1.x != q2.x || q1.y != q2.y || q1.z != q2.z || q1.w != q2.w;
}

/*! \} */ // end of rotations

/*! \} */ // end of algebra

/*! \} */ // end of math

} // Fin namespace math

} // End namespace tl

#endif // TL_MATH_QUATERNION_H
