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

#ifndef TL_GEOMETRY_ALGORITHMS_INTERSECT_H
#define TL_GEOMETRY_ALGORITHMS_INTERSECT_H

#include "config_tl.h"

#include "tidop/core/defs.h"
#include "tidop/geometry/entities/point.h"
#include "tidop/geometry/entities/segment.h"
#include "tidop/geometry/algorithms/vector.h"

namespace tl
{


/// Clase Intersection que a partir de dos geometrias calcule su intersección


/*! \addtogroup geometry
 *  \{
 */

/*! \addtogroup geometry_algorithms
 *  \{
 */

/*!
 * \brief Intersect de dos segmentos de línea
 * \param ln1 Primer segmento
 * \param ln2 Segundo segmento
 * \param pt Punto de intersección
 * \return
 */
template <typename Point_t>
inline int intersectSegments(const Segment<Point_t> &ln1, const Segment<Point_t> &ln2, Point_t *pt)
{
  int iret = 0;
  Point_t vs1;
  Point_t vs2;
  vs1 = ln1.vector();
  vs2 = ln2.vector();
  // si el producto vectorial de los vectores que unen ambos segmentos es 0 son paralelas
  double cross_product = crossProduct(vs1, vs2);
  if (cross_product != 0.) {
    Point_t v11_12 = ln2.pt1 - ln1.pt1;
    double t = crossProduct(v11_12, vs2) / cross_product;
    double u = crossProduct(v11_12, vs1) / cross_product;
    if (t >= 0.  &&  t <= 1 && u >= 0.  &&  u <= 1) {
      if (typeid(typename Point_t::value_type) == typeid(int)) {
        pt->x = roundToInteger(ln1.pt1.x + t * vs1.x);
        pt->y = roundToInteger(ln1.pt1.y + t * vs1.y);
      } else {
        pt->x += static_cast<typename Point_t::value_type>(ln1.pt1.x + t * vs1.x);
        pt->y += static_cast<typename Point_t::value_type>(ln1.pt1.y + t * vs1.y);
      }
      iret = 1;
    }
  }
  return(iret);
}

/*!
 * \brief Determina la intersección de los lineas
 * \param[in] ln1 Primera línea
 * \param[in] ln2 Segunda linea
 * \param[out] pt Punto de intersección
 * \return
 */
template<typename Point_t>
inline int intersectLines(const Segment<Point_t> &ln1, const Segment<Point_t> &ln2, Point_t *pt)
{
  int iret = 0;
  Point_t vs1;
  Point_t vs2;
  vs1 = ln1.vector();
  vs2 = ln2.vector();
  // si el producto vectorial de los vectores que unen ambos segmentos es 0 son paralelas
  double cross_product = crossProduct(vs1, vs2);
  if (cross_product != 0.) {
    Point_t v11_12 = ln2.pt1 - ln1.pt1;
    double t = crossProduct(v11_12, vs2) / cross_product;
    if (typeid(typename Point_t::value_type) == typeid(int)) {
      pt->x = roundToInteger(ln1.pt1.x + t * vs1.x);
      pt->y = roundToInteger(ln1.pt1.y + t * vs1.y);
    } else {
      pt->x = static_cast<typename Point_t::value_type>(ln1.pt1.x + t * vs1.x);
      pt->y = static_cast<typename Point_t::value_type>(ln1.pt1.y + t * vs1.y);
    }
    iret = 1;
  }
  return(iret);
}


//template<typename Point_t>
//bool linePlaneIntersection(const std::array<double, 4> &plane, const Segment<Point_t> &ln, Point_t *intersect)
//{
  //Segment<Point_t>  v = ln.vector();
  //double num = (plane[0] * ln.pt1.x + plane[1] * ln.pt1.y + plane[2] * ln.pt1.z + plane[3]);
  //double den = (plane[0] * v + plane[1] * v + plane[2] * v);
  //double t = num / den;
  //planePoint->x = point.x + plane[0] * t;
  //planePoint->y = point.y + plane[1] * t;
  //planePoint->z = point.z + plane[2] * t;
//}

/*! \} */ // end of geometry_algorithms

/*! \} */ // end of geometry

} // End namespace tl

#endif // TL_GEOMETRY_ALGORITHMS_INTERSECT_H
