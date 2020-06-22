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

#ifndef TL_MATH_EULER_ANGLES_H
#define TL_MATH_EULER_ANGLES_H

#include "config_tl.h"

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
 * \brief Ángulos de Euler
 */
template<typename T>
class EulerAngles
{

public:

  enum class Axes
  {
    //Euler angles
    zxz,
    xyx,
    yzy,
    zyz,
    xzx,
    yxy,
    //TaitBryan angles
    xyz,
    yzx,
    zxy,
    xzy,
    zyx,
    yxz
  };

public:

  EulerAngles();
  EulerAngles(double omega, double phi, double kappa, Axes axes);
  EulerAngles(const EulerAngles<T> &eulerAngles);
  ~EulerAngles();

  /*!
   * \brief Operador de asignación
   * \param[in] eulerAngles Objeto que se copia
   */
  EulerAngles &operator = (const EulerAngles<T> &eulerAngles);

public:

  double omega;
  double phi;
  double kappa;
  Axes axes;

};

template<typename T>
EulerAngles<T>::EulerAngles()
  : omega{0},
    phi{0},
    kappa{0},
    axes(Axes::xyz)
{
}

template<typename T>
EulerAngles<T>::EulerAngles(double omega, 
                            double phi, 
                            double kappa, 
                            Axes axes)
  : omega(omega),
    phi(phi),
    kappa(kappa),
    axes(axes)
{
}

template<typename T>
EulerAngles<T>::EulerAngles(const EulerAngles<T> &eulerAngles)
  : omega(eulerAngles.omega),
    phi(eulerAngles.phi),
    kappa(eulerAngles.kappa),
    axes(eulerAngles.axes)
{
}

template<typename T>
EulerAngles<T>::~EulerAngles()
{}

template<typename T>
EulerAngles<T> &EulerAngles<T>::operator = (const EulerAngles &eulerAngles)
{
  if (this != &eulerAngles) {
    omega = eulerAngles.omega;
    phi = eulerAngles.phi;
    kappa = eulerAngles.kappa;
    axes = eulerAngles.axes;
  }
  return *this;
}

/*! \} */ // end of Algebra

/*! \} */ // end of Math

} // Fin namespace math

} // End namespace tl

#endif // TL_MATH_EULER_ANGLES_H
