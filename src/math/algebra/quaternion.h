#ifndef TL_MATH_QUATERNION_H
#define TL_MATH_QUATERNION_H

#include "config_tl.h"

#include <vector>
#include <array>

namespace TL
{

namespace math
{

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
   * \param[in] x
   * \param[in] y
   * \param[in] z
   * \param[in] w
   */
  Quaternion(T x, T y, T z, T w);

  /*!
   * \brief Constructor de copia
   * \param[in] quaternion Objeto que se copia
   */
  Quaternion(const Quaternion<T> &quaternion);

  /*!
   * \brief Constructor
   * \param[in] R Matriz de rotación
   */
  Quaternion(std::array<std::array<double, 3>, 3> *R);

  /*!
   * \brief destructora
   */
  ~Quaternion();

  /*!
   * \brief Operador de asignación
   * \param[in] quat Objeto que se copia
   */
  Quaternion& operator = (const Quaternion<T> &quat);

  /*!
   * \brief Conjugado
   * \f[ q = w-xi-yj-zk \f]
   */
  Quaternion<T> conjugate();

  /*!
   * \brief Norma
   * \f[ q = w+xi+yj+zk \f]
   * \f[ n(q) = sqrt{q.q} = sqrt{w^2+x^2+y^2+z^2} \f]
   */
  double norm();

private:

};

template<typename T> inline
Quaternion<T>::Quaternion()
  : x(0), 
    y(0),
    z(0),
    w(0)
{
}

template<typename T> inline
Quaternion<T>::Quaternion(T x, T y, T z, T w)
  : x(x), 
    y(y), 
    z(z), 
    w(w)
{
}

template<typename T> inline
Quaternion<T>::Quaternion(const Quaternion<T> &quaternion)
  : x(quaternion.x), 
    y(quaternion.y), 
    z(quaternion.z), 
    w(quaternion.w)
{
}

template<typename T> inline
Quaternion<T>::Quaternion(std::array<std::array<double, 3>, 3> *R)
{

}

template<typename T> inline
Quaternion<T>::~Quaternion()
{}

template<typename T> inline
Quaternion<T>& Quaternion<T>::operator = (const Quaternion& quaternion)
{
  this->x = quaternion.x;
  this->y = quaternion.y;
  this->z = quaternion.z;
  this->w = quaternion.w;
  return *this;
}

template<typename T> inline
Quaternion<T> Quaternion<T>::conjugate()
{
  return Quaternion<T>(-x, -y, -z, w);
}

template<typename T> inline
double Quaternion<T>::norm()
{
  return sqrt(x*x + y*y + z*z + w*w);
}

// Operaciones entre cuaterniones

/*!
 * \brief Multiplicación de cuaterniones
 * \f[ q1 = w1+x1i+y1j+z1k \f]
 * \f[ q2 = w2+x2i+y2j+z2k \f]
 * \f[ q1.q2 = \f]
 */
template<typename T> inline
Quaternion<T> operator *(const Quaternion<T> &quat1, const Quaternion<T> &quat2)
{
  return Quaternion<T>(
    quat1.x*quat2.w + quat1.y*quat2.z - quat1.z*quat2.y + quat1.w*quat2.x,
    -quat1.x.quat2z + quat1.y*quat2.w + quat1.z*quat2.x + quat1.w*quat2.y,
    quat1.x*quat2.y - quat1.y*quat2.x + quat1.z*quat2.w + quat1.w*quat2.z,
    -quat1.x*quat2.x - quat1.y*quat2.y - quat1.z*quat2.z + quat1.w*quat2.w
  );
}

/*!
 * \brief Suma de cuaterniones
 * \f[ q1 = w1+x1i+y1j+z1k \f]
 * \f[ q2 = w2+x2i+y2j+z2k \f]
 * \f[ q1 + q2 = \f]
 */
template<typename T> inline
Quaternion<T> operator +(const Quaternion<T> &quat1, const Quaternion<T> &quat2)
{
  return Quaternion<T>(quat1.x+quat2.x, quat1.y+quat2.y, quat1.z+quat2.z, quat1.w+quat2.w);
}

} // Fin namespace math

} // End namespace TL

#endif // TL_MATH_QUATERNION_H