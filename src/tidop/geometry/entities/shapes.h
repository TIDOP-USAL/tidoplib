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


#ifndef TL_GEOM_SHAPES_H
#define TL_GEOM_SHAPES_H

#include "config_tl.h"

#include <limits>
#include <numeric>

#include "tidop/core/defs.h"

#ifdef HAVE_OPENCV
#include "opencv2/core/types.hpp"
#endif // HAVE_OPENCV

#include "tidop/geometry/entities/entity.h"
#include "tidop/geometry/entities/point.h"

namespace tl
{

/*! \addtogroup GeometricEntities
 *  \{
 */


namespace geometry
{

/* ---------------------------------------------------------------------------------- */

/*!
 * \brief Clase triangulo
 *
 */
template<typename T>
class Triangle
  : public Entity
{

public:

  /*!
   * \brief type
   */
  typedef T value_type;

public:

  /*!
   * \brief Constructora por defecto
   */
  Triangle();

  /*!
   * \brief Area de un circulo
   * \return Area
   */
  double area() const;

};


template<typename T> inline
Triangle<T>::Triangle()
{
}

template<typename T> inline
double Triangle<T>::area() const
{
  return 0;
}



/*!
 * \brief Clase rectangulo
 *
 */
template<typename T>
class Rectangle
  : public Entity
{

public:

  /*!
   * \brief type
   */
  typedef T value_type;

public:

  /*!
   * \brief Constructora por defecto
   */
  Rectangle();

  /*!
   * \brief Area
   * \return Area
   */
  double area() const;

};


template<typename T> inline
Rectangle<T>::Rectangle()
{
}


template<typename T> inline
double Rectangle<T>::area() const
{
  return 0;
}

 
} // End namespace geometry

/*! \} */ // end of GeometricEntities

} // End namespace tl

#endif // TL_GEOM_SHAPES_H
