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

#ifndef TL_GEOMETRY_ALGORITHMS_PROJECTION_H
#define TL_GEOMETRY_ALGORITHMS_PROJECTION_H

#include "config_tl.h"

#include "tidop/core/defs.h"
#include "tidop/geometry/entities/point.h"

namespace tl
{

template<typename T> class Segment;
template<typename T> class Segment3D;

/*!
 * \brief Projecta un punto en un segmento de recta.
 * Si no hay punto de proyección en el segmento se devuelve nulo
 * \param[in] ln Segmento de línea
 * \param[in] pt Punto que se proyecta
 * \param[out] ptp Punto proyectado
 * \return -1, 0, 1
 */
template<typename Point_t> inline
int projectPointInSegment(const Segment<Point_t> &ln, const Point_t &pt, Point_t *ptp)
{
  int iret = 0;
  if (pt == ln.pt1 || pt == ln.pt2) {
    *ptp = pt;
    return 2;
  }
  Point_t v1 = pt - ln.pt1;
  Point_t v2 = ln.vector();
  double daux = dotProduct(v1, v2);
  double r = daux / (v2.x * v2.x + v2.y * v2.y);

  if (typeid(typename Point_t::value_type) == typeid(int)) {
    ptp->x = ln.pt1.x + TL_ROUND_TO_INT(v2.x * r);
    ptp->y = ln.pt1.y + TL_ROUND_TO_INT(v2.y * r);
  } else {
    ptp->x = ln.pt1.x + static_cast<typename Point_t::value_type>(v2.x * r);
    ptp->y = ln.pt1.y + static_cast<typename Point_t::value_type>(v2.y * r);
  }

  if (daux <= 0) iret = -1;
  else if (daux >= (v2.x * v2.x + v2.y * v2.y)) iret = 1;
  else if (daux == 0.) iret = 2; // Esta en la línea
  return iret;
}

template<typename Point_t> inline
int projectPointInSegment(const Segment3D<Point_t> &ln, const Point_t &pt, Point_t *ptp)
{
  int iret = 0;
  if (pt == ln.pt1 || pt == ln.pt2) {
    *ptp = pt;
    return 2;
  }
  Point3D v1 = pt - ln.pt1;
  Point3D v2 = ln.vector();
  double daux = dotProduct3D(v1, v2);
  double r = daux / (v2.x * v2.x + v2.y * v2.y + v2.z * v2.z);

  if (typeid(typename Point_t::value_type) == typeid(int)) {
    ptp->x = ln.pt1.x + TL_ROUND_TO_INT(v2.x * r);
    ptp->y = ln.pt1.y + TL_ROUND_TO_INT(v2.y * r);
    ptp->z = ln.pt1.z + TL_ROUND_TO_INT(v2.z * r);
  } else {
    ptp->x = ln.pt1.x + static_cast<typename Point_t::value_type>(v2.x * r);
    ptp->y = ln.pt1.y + static_cast<typename Point_t::value_type>(v2.y * r);
    ptp->z = ln.pt1.z + static_cast<typename Point_t::value_type>(v2.z * r);
  }

  if (daux <= 0) iret = -1;
  else if (daux >= (v2.x * v2.x + v2.y * v2.y + v2.z * v2.z)) iret = 1;
  else if (daux == 0.) iret = 2; // Esta en la línea
  return iret;
}


} // End namespace tl

#endif // TL_GEOMETRY_ALGORITHMS_PROJECTION_H
