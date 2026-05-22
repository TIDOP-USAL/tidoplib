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

#pragma once

#include <limits>

#include "tidop/core/base/defs.h"
#include "tidop/geometry/primitives/Point.h"
#include "tidop/geometry/primitives/Polygon.h"
#include "tidop/math/algebra/rotations/RotationMatrix.h"

namespace tl
{

/*!
 * \addtogroup geospatial
 *
 * \{
 */


TL_EXPORT int utmZoneFromLongitude(double longitude);
TL_EXPORT std::pair<int, char> utmZoneFromLonLat(double longitude, double latitude);


//TODO: En https://github.com/omtinez/CenterPolygon estas variables las da como enteros
//pero aqui la definimos como double para evitar un problema de perdida de precisión al dividir
//que se ha visto que daba problemas
//#define N_CELLS 20.
//#define M_CELLS 20.
constexpr auto CONSECUTIVE_MISS = 15;

template<typename Point_t> inline
Point_t findInscribedCircleSequential(const Polygon<Point_t> &polygon,
                                      const BoundingBox<Point_t> bounds,
                                      double nCells,
                                      double mCells)
{
    Point_t pia = bounds.center();
    Point_t tmp{};

    // calculate the required increment for x and y
    double increment_x = (bounds.max().x() - bounds.min().x()) / nCells;
    double increment_y = (bounds.max().y() - bounds.min().y()) / mCells;

    // biggest known distance
    double max_distance = 0.;

    double tmp_distance = std::numeric_limits<double>::max();
    for (int i = 0; static_cast<double>(i) <= nCells; i++) {

        tmp.x() = bounds.min().x() + i * increment_x;

        for (int j = 0; static_cast<double>(j) <= mCells; j++) {

            tmp.y() = bounds.min().y() + j * increment_y;

            if (polygon.isInner(tmp)) {
                tmp_distance = distPointToPolygon(tmp, polygon);
                if (tmp_distance > max_distance) {
                    max_distance = tmp_distance;
                    pia.x() = tmp.x();
                    pia.y() = tmp.y();
                }
            }
        }
    }

    return pia;
}

// Polo de inaccesibilidad para un poligono 2D
template<typename Point_t> inline
void poleOfInaccessibility(const Polygon<Point_t> &polygon,
                           Point_t *pole,
                           double nCells = 20.,
                           double mCells = 20.)
{
    if (pole == NULL) return;
    BoundingBox<Point_t> w = polygon.boundingBox();

    Point_t point_tmp;

    int count = 1;
    while (count++) {

        /*if (method == METHOD_SEQUENTIAL) {*/
        point_tmp = findInscribedCircleSequential(polygon, w, nCells, mCells);
        /*} else if (method == METHOD_RANDOMIZED) {
          point_tmp = findInscribedCircleRandomized(polygon, w);
        }*/

        pole->x() = point_tmp.x();
        pole->y() = point_tmp.y();

        Point_t aux{};
        aux.x() = (w.max().x() - w.min().x()) / (sqrt(2.) * 2.);
        aux.y() = (w.max().y() - w.min().y()) / (sqrt(2.) * 2.);

        w.min() = *pole - aux;
        w.max() = *pole + aux;

        if (w.max().x() - w.min().x() < 0.01 || w.max().y() - w.min().y() < 0.01) break;

    }
}

/*! \} */ // end of geospatial

} // End namespace tl
