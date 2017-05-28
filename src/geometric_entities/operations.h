#ifndef I3D_OPERATIONS_H
#define I3D_OPERATIONS_H

#include "core/config.h"

#ifdef HAVE_OPENCV
#include "opencv2/core/core.hpp"
#endif

#include "core/defs.h"
#include "geometric_entities/point.h"
#include "geometric_entities/polygon.h"
#include "geometric_entities/window.h"

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
 * \param[in] pt1 Punto 1
 * \param[in] pt2 Punto 2
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


void poleOfInaccessibility(const std::vector<cv::Point> &points);

#define N_CELLS 20
#define M_CELLS 20

template<typename T> inline
void Geometry_find_biggest_inscribed_circle_in_polygon_sequential(
		const Polygon<T> &polygon, const Window<T> bounds) {
  Point<T> pia = bounds.getCenter();
  Point<T> tmp;
	//Point* pia = Point_new((bounds[0] + bounds[1]) / 2, (bounds[2] + bounds[3]) / 2);
	//Point* tmp = Point_new(0, 0);

	// calculate the required increment for x and y
	float increment_x = (bounds.pt2.x - bounds.pt1.x) / N_CELLS;
	float increment_y = (bounds.pt2.y - bounds.pt1.y) / M_CELLS;

	// biggest known distance
	float max_distance = 0;

	// loop through x and y coordinates to hit all the nodes
	int i, j;
	float tmp_distance = I3D_FLOAT_MAX;
	for (i = 0; i <= N_CELLS; i++) {

		// find x value
		tmp.x = bounds.pt1-x + i * increment_x;

		for (j = 0; j <= M_CELLS; j++) {

			// find y value
			tmp.y = bounds.pt1.y + j * increment_y;

			// compare with candidate PIA if point is in polygon
			//if (Geometry_is_point_in_polygon(polygon, tmp->x, tmp->y)) {
      if (polygon.isInner(tmp)) {
				tmp_distance = Geometry_distance_between_point_and_polygon(polygon, tmp);
				if (tmp_distance > max_distance) {
					max_distance = tmp_distance;
					pia.x = tmp.x;
					pia.y = tmp.y;
				}
			}
		}
	}

	//// cleanup and return
	//Point_destroy(tmp);
	//return pia;
}

template<typename T> inline
I3D_EXPORT void poleOfInaccessibility(const Polygon<T> &polygon) 
{
  //Window<T> w = polygon.getWindow();
 
  // O si quiero que sea mas generica la funcion y se le pueda pasar cualquier contenedor
  Window<T> w;
  for (auto &pt : polygon) {
    if (w.pt1.x > pt.x) w.pt1.x = pt.x;
    if (w.pt1.y > pt.y) w.pt1.y = pt.y;
    if (w.pt2.x < pt.x) w.pt2.x = pt.x;
    if (w.pt2.y < pt.y) w.pt2.y = pt.y;
  }


	//float bounds[4] = { polygon->first_edge->v1->x, polygon->first_edge->v1->x,
	//	polygon->first_edge->v1->y, polygon->first_edge->v1->y };

	//Edge* tmp_edge;
	//for (tmp_edge = polygon->first_edge; tmp_edge != NULL; tmp_edge = tmp_edge->next) {
	//	if (tmp_edge->v1->x < bounds[0]) bounds[0] = tmp_edge->v1->x;
	//	if (tmp_edge->v1->x > bounds[1]) bounds[1] = tmp_edge->v1->x;
	//	if (tmp_edge->v1->y < bounds[2]) bounds[2] = tmp_edge->v1->y;
	//	if (tmp_edge->v1->y > bounds[3]) bounds[3] = tmp_edge->v1->y;
	//}

	Point<T> point_pia();
	Point<T> point_tmp();
	float flt_tmp = I3D_FLOAT_MAX;
	int count = 1;
	while (count++) {

		// find new candidate PIA
		if (method == METHOD_SEQUENTIAL) {
			point_tmp =
				Geometry_find_biggest_inscribed_circle_in_polygon_sequential(polygon, bounds);
		} else if (method == METHOD_RANDOMIZED) {
			point_tmp =
				Geometry_find_biggest_inscribed_circle_in_polygon_randomized(polygon, bounds);
		}

		// update current PIA
		point_pia.x = point_tmp.x;
		point_pia.y = point_tmp.y;

		// update the bounds
		flt_tmp = (bounds[1] - bounds[0]) / (sqrtf(2) * 2);
		bounds[0] = point_pia->x - flt_tmp;
		bounds[1] = point_pia->x + flt_tmp;
		flt_tmp = (bounds[3] - bounds[2]) / (sqrtf(2) * 2);
		bounds[2] = point_pia->y - flt_tmp;
		bounds[3] = point_pia->y + flt_tmp;

		// check distance between upper and lower bounds
		if (bounds[1] - bounds[0] < 0.01 || bounds[3] - bounds[2] < 0.01) break;

	}

	//Point_destroy(point_tmp);
	//return point_pia;

}



} // End namespace I3D

#endif // I3D_OPERATIONS_H