#ifndef I3D_OPERATIONS_H
#define I3D_OPERATIONS_H

#include <limits>

#include "opencv2/core/core.hpp"

#include "core/defs.h"
#include "geometric_entities/segment.h"

namespace I3D
{
	
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
I3D_EXPORT double length(const cv::Point_<T> &v)
{
  return (sqrt(v.x*v.x + v.y*v.y));
}

/*!
 * \brief Longitud o modulo de un vector 3D
 * \param[in] v Vector
 * \return Longitud
 */
template<typename T> inline
I3D_EXPORT double length(const cv::Point3_<T> &v)
{
  return (sqrt(v.x*v.x + v.y*v.y + v.z*v.z));
}

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


I3D_EXPORT int pointNearest(const std::vector<cv::Point> &pts_fourier, const cv::Point &pt_intersect);

/*!
 * Comprobación de que los 3 puntos están en la misma línea
 */
I3D_EXPORT bool isCollinearPoints(const cv::Point &pt_c, const Line &line_i_r, double tolerance = 2.);

/*! \} */ // end of GeometricEntities

} // End namespace I3D

#endif // I3D_OPERATIONS_H