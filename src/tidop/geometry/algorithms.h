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

#ifndef TL_GEOMETRY_ALGORITHMS_H
#define TL_GEOMETRY_ALGORITHMS_H

#include "tidop/geometry/algorithms/distance.h"
#include "tidop/geometry/algorithms/angle.h"
#include "tidop/geometry/algorithms/intersect.h"

/*! \addtogroup geometry
 *  \{
 */

/*! \defgroup geometry_algorithms Geometry algorithms
 *  \{
 */

/*!
 * \brief Comprueba si un punto esta a la derecha o izquierda de una linea
 */
template<typename Point_t> inline
int isLeft(Point_t ln_pt1, Point_t ln_pt2, Point_t pt)
{
  int r_value{ 0 };

  double aux = (ln_pt2.x - ln_pt1.x) * (pt.y - ln_pt1.y)
    - (pt.x - ln_pt1.x) * (ln_pt2.y - ln_pt1.y);
  if (aux > 0) r_value = 1;
  else if (aux < 0) r_value = -1;

  return r_value;
}

/*! \} */ // end of geometry_algorithms

/*! \} */ // end of geometry


#endif // TL_GEOMETRY_ALGORITHMS_H
