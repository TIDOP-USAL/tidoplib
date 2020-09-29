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
#include "tidop/math/algebra/rotations.h"

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
 * \brief Notación axial-angular
 */
template<typename T>
class AxisAngle
  : public RotationBase<T>
{

public:

  AxisAngle();
  AxisAngle(T angle, const Vector<3, T> &axis);
  ~AxisAngle() override;

public:

  T angle;
  Vector<3, T> axis;
};

template<typename T>
AxisAngle<T>::AxisAngle()
  : RotationBase<T>(Rotation::Type::axis_angle),
    angle(0)
{
  axis.at(0) = 1;
  axis.at(1) = 0;
  axis.at(2) = 0;
}

template<typename T>
AxisAngle<T>::AxisAngle(T angle, const Vector<3, T> &axis)
  : RotationBase<T>(Rotation::Type::axis_angle),
    angle(angle),
    axis(axis)
{
  TL_TODO("normalizar vector")
}

template<typename T>
AxisAngle<T>::~AxisAngle()
{

}

/*! \} */ // end of Algebra

/*! \} */ // end of Math

} // Fin namespace math

} // End namespace tl

#endif // TL_MATH_AXIS_ANGLE_H
