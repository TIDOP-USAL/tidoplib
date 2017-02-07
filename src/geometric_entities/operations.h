#ifndef I3D_OPERATIONS_H
#define I3D_OPERATIONS_H

#include "core/config.h"

#ifdef HAVE_OPENCV
#include "opencv2/core/core.hpp"
#endif

#include "core/defs.h"
#include "geometric_entities/point.h"

namespace I3D
{

// forward declaration
template<typename T> class Segment;

/*! \defgroup GeometricEntities Entidades geométricas
 *  Puntos, lineas, ...
 *  \{
 */

/* ---------------------------------------------------------------------------------- */

// Templates para operaciones entre puntos

/*!
 * \brief Longitud o modulo de un vector 2D
 * \param[in] v vector
 * \return Longitud
 */
template<typename T> inline
I3D_EXPORT double length(const Point<T> &v)
{
  return sqrt(v.x*v.x + v.y*v.y);
}

#ifdef HAVE_OPENCV

/*!
 * \brief Longitud o modulo de un vector 3D
 * \param[in] v Vector
 * \return Longitud
 */
template<typename T> inline
I3D_EXPORT double length(const cv::Point3_<T> &v)
{
  return sqrt(v.x*v.x + v.y*v.y + v.z*v.z);
}

#endif

/*!
 * \brief Distancia entre dos puntos
 * \param pt1[in] Punto 1
 * \param pt2[in] Punto 2
 * \return Distancia
 */
template<typename T> inline
I3D_EXPORT double distance(const T &pt1, const T &pt2)
{
  T v = pt2 - pt1;
  return length(v);
}


/* ---------------------------------------------------------------------------------- */


I3D_EXPORT int pointNearest(const std::vector<PointI> &pts_fourier, const PointI &pt_intersect);

/*!
 * Comprobación de que los 3 puntos están en la misma línea
 */
I3D_EXPORT bool isCollinearPoints(const PointI &pt_c, const Segment<int> &line_i_r, double tolerance = 2.);

/*! \} */ // end of GeometricEntities

} // End namespace I3D

#endif // I3D_OPERATIONS_H