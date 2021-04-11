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
 * License along with Foobar. If not, see <http://www.gnu.org/licenses/>. *
 *                                                                        *
 * @license LGPL-3.0 <https://www.gnu.org/licenses/lgpl-3.0.html>         *
 *                                                                        *
 **************************************************************************/

#ifndef TL_MATH_VECTORS_H
#define TL_MATH_VECTORS_H

#include "config_tl.h"
#include "tidop/core/defs.h"
#include "tidop/math/mathutils.h"

namespace tl
{

namespace math
{

/*!
 * \addtogroup Math
 * \{
 */


/* ---------------------------------------------------------------------------------- */
/*                                 ALGEBRA                                            */
/* ---------------------------------------------------------------------------------- */
/* Operaciones entre vectores en el plano y en el espacio                             */
/* ---------------------------------------------------------------------------------- */




///*!
// * \brief Vector entre dos puntos
// * \param[in] pt1 punto 1
// * \param[in] pt2 punto 2
// * \return Vector
// */
//template<typename Point_t> inline
//Point_t vector2D(const Point_t &pt1, const Point_t &pt2)
//{
//  Point_t v;
//  v.x = pt2.x - pt1.x;
//  v.y = pt2.y - pt1.y;
//  return v;
//}
//
//template<typename Point_t> inline
//Point_t vector3D(const Point_t &pt1, const Point_t &pt2)
//{
//  Point_t v;
//  v.x = pt2.x - pt1.x;
//  v.y = pt2.y - pt1.y;
//  v.z = pt2.z - pt1.z;
//  return v;
//}
//
///*!
// * \brief Producto vectorial de dos vectores en el plano
// * \param[in] pt1 Vector o punto 1
// * \param[in] pt2 Vector o punto 2
// * \return Producto vectorial de los dos vectores
// */
//template<typename Point_t> inline
//double crossProduct(const Point_t &pt1, const Point_t &pt2)
//{
//  return static_cast<double>(pt1.x*pt2.y - pt1.y*pt2.x);
//}
//
///*!
// * \brief Producto vectorial de dos vectores en 3 dimensiones
// * \param[in] pt1 Vector o punto 1
// * \param[in] pt2 Vector o punto 2
// * \return Producto vectorial de los dos vectores
// */
//template<typename Point3_t> inline
//Point3_t crossProduct3D(const Point3_t &pt1, const Point3_t &pt2)
//{
//  return Point3_t(pt1.y * pt2.z - pt1.z * pt2.y,
//                  pt1.z * pt2.x - pt1.x * pt2.z,
//                  pt1.x * pt2.y - pt1.y * pt2.x);
//}
//
///*!
// * \brief Producto escalar de dos vectores en el plano
// * \param[in] pt1 Vector o punto 1
// * \param[in] pt2 Vector o punto 2
// * \return Producto escalar de los dos vectores
// */
//template<typename Point_t> inline
//double dotProduct(const Point_t &pt1, const Point_t &pt2)
//{
//  return static_cast<double>(pt1.x) * static_cast<double>(pt2.x) +
//         static_cast<double>(pt1.y) * static_cast<double>(pt2.y);
//}
//
///*!
// * \brief Producto escalar de dos vectores en 3 dimensiones
// * \param[in] pt1 Vector o punto 1
// * \param[in] pt2 Vector o punto 2
// * \return Producto escalar de los dos vectores
// */
//template<typename Point3_t> inline
//double dotProduct3D(const Point3_t &pt1, const Point3_t &pt2)
//{
//  return static_cast<double>(pt1.x) * static_cast<double>(pt2.x) +
//         static_cast<double>(pt1.y) * static_cast<double>(pt2.y) +
//         static_cast<double>(pt1.z) * static_cast<double>(pt2.z);
//}
//
///*!
// * \brief Módulo de un vector 2D
// * \param[in] v Vector
// */
//template<typename Point_t> inline
//double module(const Point_t &v)
//{
//  TL_TODO("cambiar por math::module para evitar desbordes")
//  return sqrt(dotProduct(v, v));
//}
//
///*!
// * \brief Módulo de un vector 3D
// * \param[in] v Vector
// */
//template<typename Point3_t> inline
//double module3D(const Point3_t &v)
//{
//  TL_TODO("cambiar por math::module3d para evitar desbordes")
//  return sqrt(dotProduct3D(v, v));
//}



/*!
 * \brief Devuelve el ángulo entre dos vectores
 * \param[in] v1 Vector 1
 * \param[in] v2 Vector 2
 * \return Ángulo en radianes
 */
template<typename Point_t> inline
double vectorAngle(const Point_t &v1, const Point_t &v2)
{
  TL_TODO("debería estar en geometry con la clase punto")
  if (v1 == Point_t() || v2 == Point_t())
    return 0.0;
  return acos((v1.x*v2.x + v1.y*v2.y) / (module(v1.x, v1.y) * module(v2.x, v2.y)));
}

template<typename Point_t> inline
double vectorAngleOX(const Point_t &v)
{
  TL_TODO("debería estar en geometry con la clase punto")
  double angle = 0.0;
  if (!(v.x == 0 && v.y == 0))
    angle = atan2(v.y, v.x);
  return angle;
}

template<typename Point_t> inline
double vectorAngleOY(const Point_t &v)
{
  TL_TODO("debería estar en geometry con la clase punto")
  double angle = 0.0;
  if (!(v.x == 0 && v.y == 0))
    angle = atan2(v.x, v.y);
  return angle;
}

template<typename Point_t> inline
double azimut(const Point_t &pt1, const Point_t &pt2)
{
  double azimut = 0.;
  Point_t v;
  v.x = pt2.x - pt1.x;
  v.y = pt2.y - pt1.y;
  if (v.x == 0 && v.y == 0) return azimut;
  azimut = atan2(v.x, v.y);
  if (azimut < 0.) azimut += two_pi;
  return azimut;
}

/*! \} */ // end of math



} // namespace math

} // namespace tl


#endif // TL_MATH_VECTORS_H
