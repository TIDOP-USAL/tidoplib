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

#ifndef TL_MATH_AXIS_ANGLE_H
#define TL_MATH_AXIS_ANGLE_H

#include "config_tl.h"

#include <vector>
#include <array>

#include "tidop/math/algebra/vector.h"

namespace tl
{

namespace math
{

/*!
 * \brief Ángulos de Euler
 */
template<typename T>
class AxisAngle
{

public:

  enum class Axis{
    x,
    y,
    z
  };


  T angle;
  Vector<3, T> axis;

public:

  /*!
   * \brief Constructor por defecto
   */
  AxisAngle();

  AxisAngle(T angle, const Vector<3, T> &axis);
  //AxisAngle(T angle, Axis axis);

  /*!
   * \brief destructora
   */
  ~AxisAngle();


};

template<typename T>
AxisAngle<T>::AxisAngle()
  : angle(0)
{
  axis.at(0) = 1;
  axis.at(1) = 0;
  axis.at(2) = 0;
}

template<typename T>
AxisAngle<T>::AxisAngle(T angle, const Vector<3, T> &axis)
  : angle(angle),
    axis(axis)
{

}

//template<typename T>
//AxisAngle<T>::AxisAngle(T angle, AxisAngle::Axis axis)
//  : angle(angle)
//{
//  if (axis == AxisAngle<T>::Axis::x){
//    axis.at(0) = static_cast<T>(1);
//    axis.at(1) = static_cast<T>(0);
//    axis.at(2) = static_cast<T>(0);
//  } else if (axis == AxisAngle<T>::Axis::y){
//    axis.at(0) = static_cast<T>(0);
//    axis.at(1) = static_cast<T>(1);
//    axis.at(2) = static_cast<T>(0);
//  } else if (axis == AxisAngle<T>::Axis::z){
//    axis.at(0) = static_cast<T>(0);
//    axis.at(1) = static_cast<T>(0);
//    axis.at(2) = static_cast<T>(1);
//  }
//}

template<typename T>
AxisAngle<T>::~AxisAngle()
{

}



} // Fin namespace math

} // End namespace tl

#endif // TL_MATH_AXIS_ANGLE_H
