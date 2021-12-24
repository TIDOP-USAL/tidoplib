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

#ifndef TL_GEOMETRY_ALGORITHMS_DISTACE_H
#define TL_GEOMETRY_ALGORITHMS_DISTACE_H

#include "config_tl.h"

#include "tidop/core/defs.h"
#include "tidop/geometry/entities/point.h"
#include "tidop/geometry/algorithms/vector.h"
#include "tidop/geometry/algorithms/projection.h"

namespace tl
{

template<typename T> class Segment;
template<typename T> class Segment3D;
template<typename T> class Polygon;
template<typename T> class Polygon3D;

/*! \addtogroup geometry
 *  \{
 */

/*! \addtogroup geometry_algorithms
 *  \{
 */

/*!
 * \brief Distancia entre dos puntos
 * \param[in] pt1 Punto 1
 * \param[in] pt2 Punto 2
 * \return Distancia
 */
template<typename Point_t> inline
double distance(const Point_t &pt1, const Point_t &pt2)
{
  Point_t v;
  v.x = pt2.x - pt1.x;
  v.y = pt2.y - pt1.y;
  return module(v);
}

template<typename Point3_t> inline
double distance3D(const Point3_t &pt1, const Point3_t &pt2)
{
  Point3_t v;
  v.x = pt2.x - pt1.x;
  v.y = pt2.y - pt1.y;
  v.z = pt2.z - pt1.z;
  return module3D(v);
}

/*!
 * \brief Calcula la distancia de un punto a un segmento de linea.
 * \param[in] pt Punto
 * \param[in] ln Linea
 * \return Distancia de un punto a una segmento de linea
 */
template<typename Point_t> inline
double distPointToSegment(const Point_t &pt, const Segment<Point_t> &ln)
{
  Point_t ptp;
  int ipr = projectPointInSegment(ln, pt, &ptp);

  if (ipr == -1) ptp = ln.pt1;
  else if (ipr == 1) ptp = ln.pt2;
  return distance(pt, ptp);
}

template<typename Point3_t> inline
double distPointToSegment(const Point3_t &pt, const Segment3D<Point3_t> &ln)
{
  Point3_t ptp;
  int ipr = projectPointInSegment(ln, pt, &ptp);

  if (ipr == -1) ptp = ln.pt1;
  else if (ipr == 1) ptp = ln.pt2;
  return distance(pt, ptp);
}

/*!
 * \brief distPointToLine
 * \param[in] pt
 * \param[in] ln
 * \return
 */
template<typename Point_t> inline
double distPointToLine(const Point_t &pt, const Segment<Point_t> &ln)
{
  Point_t v1 = vector2D(ln.pt1, pt);
  Point_t v2 = ln.vector();
  double b = dotProduct(v1, v2) / dotProduct(v2, v2);
  Point_t pt2;
  pt2.x = ln.pt1.x + b * v2.x;
  pt2.y = ln.pt1.y + b * v2.y;
  return distance(pt, pt2);
}

/*!
 * \brief Calcula la distancia mínima entre dos segmentos de linea.
 * \param[in] ln1 Segmento 1
 * \param[in] ln2 Segmento 2
 * \return Distancia entre segmentos
 */
template<typename Point_t> inline
double minDistanceSegments(const Segment<Point_t> &ln1, const Segment<Point_t> &ln2)
{
  std::array<double, 4> dist{};
  dist[0] = distPointToSegment(ln1.pt1, ln2);
  dist[1] = distPointToSegment(ln1.pt2, ln2);
  dist[2] = distPointToSegment(ln2.pt1, ln1);
  dist[3] = distPointToSegment(ln2.pt2, ln1);
  return *std::min_element(dist.begin(), dist.end());
}

/*!
 * \brief Calcula la distancia de un punto a un poligono.
 * \param[in] pt Punto
 * \param[in] polygon Poligono
 * \return Distancia de un punto a un poligono
 */
template<typename Point_t> inline
double distPointToPolygon(const Point_t &pt, const Polygon<Point_t> &polygon)
{
  double max_dist = TL_DOUBLE_MAX;
  double dist;
  Polygon<Point_t> _p = polygon;
  for (size_t i = 0; i < polygon.size(); i++) {
    if (i == polygon.size() - 1) {
      dist = distPointToSegment(pt, Segment<Point_t>(_p[i], _p[0]));
    }
    else {
      dist = distPointToSegment(pt, Segment<Point_t>(_p[i], _p[i + 1]));
    }
    if (dist < max_dist) max_dist = dist;
  }
  return max_dist;
}

/*!
 * \brief Calcula la distancia de un punto a un poligono 3d.
 * \param[in] pt Punto
 * \param[in] polygon Poligono
 * \return Distancia de un punto a un poligono
 */
template<typename Point_t> inline
double distPointToPolygon(const Point_t &pt, const Polygon3D<Point_t> &polygon)
{
  double max_dist = TL_DOUBLE_MAX;
  double dist;
  Polygon3D<Point_t> _p = polygon;
  for (size_t i = 0; i < polygon.size(); i++) {
    if (i == polygon.size() - 1) {
      dist = distPointToSegment(pt, Segment3D<Point_t>(_p[i], _p[0]));
    }
    else {
      dist = distPointToSegment(pt, Segment3D<Point_t>(_p[i], _p[i + 1]));
    }
    if (dist < max_dist) max_dist = dist;
  }
  return max_dist;
}

/*!
 * \brief Calcula la distancia de un punto a un poligono 3d mediante iteradores
 *
 * \param[in] pt Punto
 * \param[in] it_begin Iterador al principio del poligono
 * \param[in] it_end Iterador al final del poligono
 * \return Distancia de un punto a un poligono
 */
template<typename Point_t, typename Polygon_it_t> inline
double distPointToPolygon(const Point_t &pt, Polygon_it_t it_begin, Polygon_it_t it_end)
{
  double max_dist = TL_DOUBLE_MAX;
  double dist;
  Polygon_it_t it = it_begin;
  Point_t prev = *(it_end - 1);
  while (it != it_end) {
    dist = distPointToSegment3D(prev, *it, pt);
    if (dist < max_dist) max_dist = dist;
    prev = *it++;
  }
  return max_dist;
}

/*!
 * \brief Obtiene la distancia de un punto a un plano
 *
 * \param[in] pt Punto
 * \param[out] plane Parametros de la ecuación general del plano (A, B, C, D)
 * \return Distancia del punto al plano. + si está por encima y - si está por debajo.
 */
template<typename Point_t> inline
double distantePointToPlane(const Point_t &pt, const std::array<double, 4> &plane)
{
  double num = plane[0] * pt.x + plane[1] * pt.y + plane[2] * pt.z + plane[3];
  double normal = sqrt(plane[0] * plane[0] + plane[1] * plane[1] + plane[2] * plane[2]);
  if (normal == 0.) throw std::runtime_error("3 puntos alineados");
  return(num / normal);
}

/*! \} */ // end of geometry_algorithms

/*! \} */ // end of geometry

} // End namespace tl

#endif // TL_GEOMETRY_ALGORITHMS_DISTACE_H
