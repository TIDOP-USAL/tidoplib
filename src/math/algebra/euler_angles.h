#ifndef TL_MATH_EULER_ANGLES_H
#define TL_MATH_EULER_ANGLES_H

#include "config_tl.h"

#include <vector>
#include <array>

namespace TL
{

namespace math
{

/*!
 * \brief Ángulos de Euler
 */
template<typename T>
class EulerAngles
{

public:

  double omega;
  double phi;
  double kappa;

public:

  /*!
   * \brief Constructor por defecto
   */
  EulerAngles();

  /*!
   * \brief Constructor de copia
   * \param[in] eulerAngles Objeto que se copia
   */
  EulerAngles(const EulerAngles<T> &eulerAngles);

  /*!
   * \brief destructora
   */
  ~EulerAngles();

  /*!
   * \brief Operador de asignación
   * \param[in] eulerAngles Objeto que se copia
   */
  EulerAngles& operator = (const EulerAngles<T> &eulerAngles);

};

template<typename T> inline
EulerAngles<T>::EulerAngles()
{
}

template<typename T> inline
EulerAngles<T>::EulerAngles(const EulerAngles<T> &eulerAngles)
{
}

template<typename T> inline
EulerAngles<T>::~EulerAngles()
{}

template<typename T> inline
EulerAngles<T>& EulerAngles<T>::operator = (const EulerAngles& eulerAngles)
{
  return *this;
}

} // Fin namespace math

} // End namespace TL

#endif // TL_MATH_EULER_ANGLES_H