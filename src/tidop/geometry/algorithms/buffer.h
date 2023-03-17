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

#ifndef TL_GEOMETRY_ALGORITHMS_BUFFER_H
#define TL_GEOMETRY_ALGORITHMS_BUFFER_H

#include "tidop/geometry/entities/point.h"
#include "tidop/geometry/entities/polygon.h"
#include "tidop/geometry/algorithms/angle.h"
#include "tidop/math/math.h"

namespace tl
{

/*! \addtogroup geometry
 *  \{
 */

/*! \addtogroup geometry_algorithms
 *  \{
 */

/*!
 * \brief Crea un buffer entorno a una linea
 * \param[in] ln Línea
 * \param[in] size Tamaño de buffer
 * \return Buffer
 */
template<typename Point_t> inline
Polygon<Point_t> buffer(const Segment<Point_t> &ln, int size)
{
  Polygon<Point_t> buff(4);

  Point_t pt1 = ln.pt1;
  Point_t pt2 = ln.pt2;
  double acimut = azimut(pt1, pt2);
  double dx = size * sin(acimut + math::consts::half_pi<typename Point_t::value_type>);
  double dy = size * cos(acimut + math::consts::half_pi<typename Point_t::value_type>);

  buff[0] = Point_t(pt1.x + dx, pt1.y + dy);
  buff[1] = Point_t(pt2.x + dx, pt2.y + dy);
  buff[2] = Point_t(pt2.x - dx, pt2.y - dy);
  buff[3] = Point_t(pt1.x - dx, pt1.y - dy);

  return buff;
}

/*! \} */ // end of geometry_algorithms

/*! \} */ // end of geometry

} // End namespace tl

#endif // TL_GEOMETRY_ALGORITHMS_BUFFER_H
