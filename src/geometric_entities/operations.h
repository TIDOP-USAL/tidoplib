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
#include "geometric_entities/bbox.h"

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








/* ---------------------------------------------------------------------------------- */






// Código para probar

/*


  I3D::Polygon<int> polygon{
    PointI(100, 144),
    PointI(157, 93),
    PointI(245, 83),
    PointI(333, 56),
    PointI(399, 82),
    PointI(457, 117),
    PointI(465, 158),
    PointI(433, 225),
    PointI(369, 235),
    PointI(242, 264),
    PointI(171, 227),
    PointI(118, 206) 
  };

  std::vector<cv::Point> points{
    cv::Point(100, 144),
    cv::Point(157, 93),
    cv::Point(245, 83),
    cv::Point(333, 56),
    cv::Point(399, 82),
    cv::Point(457, 117),
    cv::Point(465, 158),
    cv::Point(433, 225),
    cv::Point(369, 235),
    cv::Point(242, 264),
    cv::Point(171, 227),
    cv::Point(118, 206)
  };

  PointI center;

  poleOfInaccessibility(polygon, &center);
  double radius = distPointToPolygon(center, polygon);

  PointI center2;
  _poleOfInaccessibility(points, &center2);
  double radius2 = distPointToPolygon(center2, polygon);

  cv::Mat canvas = cv::Mat::zeros(400, 600, CV_8U);

  cv::Mat aux(points);
  const cv::Point *pts = (const cv::Point*) aux.data;
  int npts = aux.rows;
  cv::fillPoly(canvas, &pts, &npts, 1, cv::Scalar(255, 255, 255) );
  //cv::Mat m_out;
  //cv::distanceTransform(canvas, m_out, DIST_L2, 5);
  //double max_val;
  //cv::Point center;

  //cv::minMaxLoc(m_out, NULL, &max_val, NULL, &center);
  cv::Mat canvas_rgb;
  cvtColor(canvas, canvas_rgb, CV_GRAY2BGR);
  cv::line(canvas_rgb, center, center, cv::Scalar(0, 0, 255), 2);

  //double radius = I3D_DOUBLE_MAX;
  //double dist;
  //for (int i = 0; i < points.size(); i++) {
  //  if (i == points.size() - 1) {
  //    dist = distPointToSegment((PointI &)center, SegmentI((PointI &)points[i], (PointI &)points[0]));
  //  } else {
  //    dist = distPointToSegment((PointI &)center, SegmentI((PointI &)points[i], (PointI &)points[i+1]));
  //  }
  //  if (dist < radius) radius = dist;
  //}

    
  cv::circle(canvas_rgb, center, radius, cv::Scalar(255, 0, 0), 2);

  */





//void poleOfInaccessibility(const std::vector<cv::Point> &points);




/*!
 * \brief Calcula la distancia de un punto a un poligono.
 * \param[in] pt Punto
 * \param[in] polygon Poligono
 * \return Distancia de un punto a un poligono
 */
template<typename T> inline
I3D_EXPORT double distPointToPolygon(const Point<T> &pt, const Polygon<T> &polygon)
{
  double max_dist = I3D_DOUBLE_MAX;
  double dist;
  Polygon<T> _p = polygon;
  for (size_t i = 0; i < polygon.getSize(); i++) {
    if (i == polygon.getSize() - 1) {
      dist = distPointToSegment(pt, Segment<T>(_p[i], _p[0]));
    } else {
      dist = distPointToSegment(pt, Segment<T>(_p[i], _p[i+1]));
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
template<typename T> inline
I3D_EXPORT double distPointToPolygon(const Point3<T> &pt, const Polygon3D<T> &polygon)
{
  double max_dist = I3D_DOUBLE_MAX;
  double dist;
  Polygon3D<T> _p = polygon;
  for (size_t i = 0; i < polygon.getSize(); i++) {
    if (i == polygon.getSize() - 1) {
      dist = distPointToSegment(pt, Segment3D<T>(_p[i], _p[0]));
    } else {
      dist = distPointToSegment(pt, Segment3D<T>(_p[i], _p[i+1]));
    }
    if (dist < max_dist) max_dist = dist;
  }
  return max_dist;
}


//TODO: En https://github.com/omtinez/CenterPolygon estas variables las da como enteros
//pero aqui la definimos como double para evitar un problema de perdida de precisión al dividir
//que se ha visto que daba problemas
//#define N_CELLS 20.
//#define M_CELLS 20.
#define CONSECUTIVE_MISS	15

template<typename T> inline
Point<T> &findInscribedCircleSequential(const Polygon<T> &polygon, const Window<T> bounds, double nCells, double mCells) 
{
  Point<T> pia = bounds.getCenter();
  Point<T> tmp;

	// calculate the required increment for x and y
	float increment_x = (bounds.pt2.x - bounds.pt1.x) / nCells;
	float increment_y = (bounds.pt2.y - bounds.pt1.y) / mCells;

	// biggest known distance
	double max_distance = 0.;

	int i, j;
  double tmp_distance = I3D_DOUBLE_MAX;
	for (i = 0; i <= nCells; i++) {

		tmp.x = bounds.pt1.x + i * increment_x;

		for (j = 0; j <= mCells; j++) {

			tmp.y = bounds.pt1.y + j * increment_y;

      if (polygon.isInner(tmp)) {
        tmp_distance = distPointToPolygon(tmp, polygon);
				if (tmp_distance > max_distance) {
					max_distance = tmp_distance;
					pia.x = tmp.x;
					pia.y = tmp.y;
				}
			}
		}
	}

	return pia;
}


// Inaccessibility of poles problem. Private function using randomized method
//template<typename T> inline
//Point<T> &findInscribedCircleRandomized(const Polygon<T> &polygon, const Window<T> bounds) 
//{
//	//struct timeval time_seed;
//	//gettimeofday(&time_seed, NULL);
//	//srand(time_seed.tv_usec);
//
//	Point<T> pia = bounds.getCenter();
//	Point<T> tmp;
//
//	// biggest known distance
//	double max_distance = 0;
//
//	// loop through x and y coordinates to hit all the nodes
//	int count = 0;
//	//float tmp_distance = I3D_FLOAT_MAX;
//  double tmp_distance = I3D_DOUBLE_MAX;
//
//	while(count < CONSECUTIVE_MISS) {
//
//		// find x and y values
//		tmp.x = bounds.pt1.x + (rand() % (int) ((bounds.pt2.x - bounds.pt1.x) * 100000) / 100000.0f);
//		tmp.y = bounds.pt1.y + (rand() % (int) ((bounds.pt2.y - bounds.pt1.y) * 100000) / 100000.0f);
//
//		// compare with candidate PIA if point is in polygon
//		//if (Geometry_is_point_in_polygon(polygon, tmp->x, tmp->y)) {
//    if (polygon.isInner(tmp)) {
//			//tmp_distance = Geometry_distance_between_point_and_polygon(polygon, tmp);
//      tmp_distance = distPointToPolygon(tmp, polygon);
//			if (tmp_distance > max_distance) {
//				max_distance = tmp_distance;
//				pia = tmp;
//				count = 0;
//			} else {
//				count++;
//			}
//		}
//	}
//
//	return pia;
//}

// Inaccessibility of poles problem. Private function using LP method
//template<typename T> inline
//Point<T> &findInscribedCircleLP(const Polygon<T> &polygon, const Window<T> bounds) {
//	Matrix_2D* m = Matrix_2D_new(polygon->n + 1, 4);
//
//	Edge* edge_tmp;
//	int index = 0;
//	for (edge_tmp = polygon->first_edge; edge_tmp != NULL; edge_tmp = edge_tmp->next) {
//
//		// get two points slightly away from the edge perpendicularly to test
//		// which side of the edge is facing the inside of the polygon
//		Point* point_test = Point_new((edge_tmp->v1->x + edge_tmp->v2->x) / 2.0f,
//			(edge_tmp->v1->y + edge_tmp->v2->y) / 2);
//		Line* line_test = Geometry_perpendicular_line_at_point(
//				edge_tmp->l, point_test->x, point_test->y);
//		Point* p1 = Geometry_find_point_in_line_given_point_and_distance(line_test, point_test, 1.0f);
//		Point* p2 = Geometry_find_point_in_line_given_point_and_distance(line_test, point_test, -1.0f);
//
//		// when the face of the edge facing the inside of the polygon is above the edge's line
//		if ((Geometry_is_point_in_polygon(polygon, p1->x, p1->y)
//			&& (p1->y - edge_tmp->l->m * p1->x - edge_tmp->l->n) > 0)
//		|| (Geometry_is_point_in_polygon(polygon, p2->x, p2->y)
//			&& (p2->y - edge_tmp->l->m * p2->x - edge_tmp->l->n) > 0)) {
//			Matrix_2D_set_row(m, index,
//				edge_tmp->l->m,
//				-1.0f,
//				0.0f,
//				-edge_tmp->l->n);
//			Matrix_2D_scale_row(m, index, 1.0f / sqrtf(
//					(edge_tmp->l->m * edge_tmp->l->m) + 1));
//			Matrix_2D_set_cell(m, index++, 2, 1.0f);
//
//		// when the face of the edge facing the inside of the polygon is below the edge's line
//		} else if ((Geometry_is_point_in_polygon(polygon, p1->x, p1->y)
//			&& (p1->y - edge_tmp->l->m * p1->x - edge_tmp->l->n) < 0)
//		|| (Geometry_is_point_in_polygon(polygon, p2->x, p2->y)
//			&& (p2->y - edge_tmp->l->m * p2->x - edge_tmp->l->n) < 0)) {
//			Matrix_2D_set_row(m, index,
//				edge_tmp->l->m,
//				-1.0f,
//				0.0f,
//				-edge_tmp->l->n);
//			Matrix_2D_scale_row(m, index, -1.0f / sqrtf(
//					(edge_tmp->l->m * edge_tmp->l->m) + 1));
//			Matrix_2D_set_cell(m, index++, 2, 1.0f);
//		}
//
//		// cleanup
//		Point_destroy(p1);
//		Point_destroy(p2);
//		Point_destroy(point_test);
//		Line_destroy(line_test);
//	}
//
//
//	// the objective function
//	Matrix_2D_set_row(m, m->rows - 1, 0.0f, 0.0f, 1.0f, 0.0f);
////	Matrix_2D_print(m);
//
//	// find solution
//	Vector* v = LPSolver_simplex_solve(m);
//	Point* point_pia = (v == NULL) ? NULL : Point_new(Vector_get(v, 0), Vector_get(v, 1));
//
//	// clean up
//	Vector_destroy(v);
//	Matrix_2D_destroy(m);
//
//	// return solution
//	return point_pia;
//}

template<typename T> inline
I3D_EXPORT void poleOfInaccessibility(const Polygon<T> &polygon, Point<T> *pole, double nCells = 20., double mCells = 20.) 
{
  if (pole == NULL) return;
  Window<T> w = polygon.getWindow();

	Point<T> point_tmp;

	int count = 1;
	while (count++) {

		/*if (method == METHOD_SEQUENTIAL) {*/
			point_tmp =	findInscribedCircleSequential(polygon, w, nCells, mCells);
		/*} else if (method == METHOD_RANDOMIZED) {
			point_tmp = findInscribedCircleRandomized(polygon, w);
		}*/

		pole->x = point_tmp.x;
		pole->y = point_tmp.y;

    Point<T> aux;
    aux.x = (w.pt2.x - w.pt1.x) / (sqrt(2.) * 2.);
		aux.y = (w.pt2.y - w.pt1.y) / (sqrt(2.) * 2.);

    w.pt1 = *pole - aux;
    w.pt2 = *pole + aux;

		if (w.pt2.x - w.pt1.x < 0.01 || w.pt2.y - w.pt1.y < 0.01) break;

	}
}



// Lo mismo para contenedores y puntos genericos

//https://stackoverflow.com/a/19931604

//template <class T>
//    using Value_Type = typename std::remove_reference<
//        decltype(*std::begin(std::declval<
//            typename std::add_lvalue_reference<T>::type>()))>::type;
//
//template<typename Polygon_t, typename Window_t, typename Point_t> inline
//void _findInscribedCircleSequential(const Polygon_t &polygon, const Window_t bounds, Point_t *pia, double nCells, double mCells) 
//{
//  //typename typedef Value_Type<Polygon_t> type;
//
//  *pia = bounds.getCenter();
//  PointD tmp;
//
//	// calculate the required increment for x and y
//	float increment_x = (bounds.pt2.x - bounds.pt1.x) / nCells;
//	float increment_y = (bounds.pt2.y - bounds.pt1.y) / mCells;
//
//	// biggest known distance
//	double max_distance = 0.;
//
//	int i, j;
//  double tmp_distance = I3D_DOUBLE_MAX;
//	for (i = 0; i <= nCells; i++) {
//
//		tmp.x = bounds.pt1.x + i * increment_x;
//
//		for (j = 0; j <= mCells; j++) {
//
//			tmp.y = bounds.pt1.y + j * increment_y;
//
//      //TODO: isInner y distPointToPolygon se tienen que sacar a una función para permitir uso de contenedores
//   //   if (pol.isInner(tmp)) {
//   //     tmp_distance = distPointToPolygon(tmp, pol);
//			//	if (tmp_distance > max_distance) {
//			//		max_distance = tmp_distance;
//			//		pia->x = tmp.x;
//			//		pia->y = tmp.y;
//			//	}
//			//}
//		}
//	}
//}
//
//template<typename Polygon_t, typename Point_t> inline
//I3D_EXPORT void _poleOfInaccessibility(const Polygon_t &polygon, Point_t *pole, double nCells = 20., double mCells = 20.) 
//{
//  if (pole == NULL) return;
//  //Window<T> w = polygon.getWindow();
//  //typename Value_Type<Point_t> type;
//  //// O si quiero que sea mas generica la funcion y se le pueda pasar cualquier contenedor
//  //Window<typename Value_Type<Point_t>> w;
//  WindowF w;
//  for (auto &pt : polygon) {
//    if (w.pt1.x > pt.x) w.pt1.x = pt.x;
//    if (w.pt1.y > pt.y) w.pt1.y = pt.y;
//    if (w.pt2.x < pt.x) w.pt2.x = pt.x;
//    if (w.pt2.y < pt.y) w.pt2.y = pt.y;
//  }
//
//	Point_t point_tmp;
//
//	int count = 1;
//	while (count++) {
//
//		/*if (method == METHOD_SEQUENTIAL) {*/
//			_findInscribedCircleSequential(polygon, w, &point_tmp, nCells, mCells);
//		/*} else if (method == METHOD_RANDOMIZED) {
//			point_tmp = findInscribedCircleRandomized(polygon, w);
//		}*/
//
//		pole->x = point_tmp.x;
//		pole->y = point_tmp.y;
//
//    Point_t aux;
//    aux.x = (w.pt2.x - w.pt1.x) / (sqrt(2.) * 2.);
//		aux.y = (w.pt2.y - w.pt1.y) / (sqrt(2.) * 2.);
//
//    w.pt1 = *pole - aux;
//    w.pt2 = *pole + aux;
//
//		if (w.pt2.x - w.pt1.x < 0.01 || w.pt2.y - w.pt1.y < 0.01) break;
//
//	}
//}



















template<typename T> inline
Point3<T> &findInscribedCircleSequential(const Polygon3D<T> &polygon, const Bbox<T> bounds, double xCells, double yCells, double zCells) 
{
  Point3<T> pia = bounds.getCenter();
  Point3<T> tmp;

	float increment_x = (bounds.pt2.x - bounds.pt1.x) / xCells;
	float increment_y = (bounds.pt2.y - bounds.pt1.y) / yCells;
  float increment_z = (bounds.pt2.z - bounds.pt1.z) / zCells;

	double max_distance = 0.;

  double tmp_distance = I3D_DOUBLE_MAX;
	for (int i = 0; i <= xCells; i++) {

		tmp.x = bounds.pt1.x + i * increment_x;

		for (int j = 0; j <= yCells; j++) {
      
      tmp.y = bounds.pt1.y + j * increment_y;
      
      for (int k = 0; k <= zCells; k++) {

        tmp.z = bounds.pt1.z + k * increment_z;

        Bbox<T> box = polygon.getBox();
        if ( box.containsPoint(tmp) ) {
        //if (polygon.isInner(tmp)) {
          tmp_distance = distPointToPolygon(tmp, polygon);
          if (tmp_distance > max_distance) {
            max_distance = tmp_distance;
            pia.x = tmp.x;
            pia.y = tmp.y;
            pia.z = tmp.z;
          }
        }

      }
		}
	}

	return pia;
}


template<typename T> inline
I3D_EXPORT void poleOfInaccessibility(const Polygon3D<T> &polygon, Point3<T> *pole, double xCells = 20., double yCells = 20., double zCells = 20.) 
{
  if (pole == NULL) return;
  Bbox<T> box = polygon.getBox();

	Point3<T> point_tmp;

	int count = 1;
	while (count++) {

		/*if (method == METHOD_SEQUENTIAL) {*/
			point_tmp =	findInscribedCircleSequential(polygon, box, xCells, yCells, zCells);
		/*} else if (method == METHOD_RANDOMIZED) {
			point_tmp = findInscribedCircleRandomized(polygon, w);
		}*/

		pole->x = point_tmp.x;
		pole->y = point_tmp.y;
    pole->z = point_tmp.z;

    Point3<T> aux;
    aux.x = (box.pt2.x - box.pt1.x) / (sqrt(2.) * 2.);
		aux.y = (box.pt2.y - box.pt1.y) / (sqrt(2.) * 2.);
    aux.z = (box.pt2.z - box.pt1.z) / (sqrt(2.) * 2.);

    box.pt1 = *pole - aux;
    box.pt2 = *pole + aux;

		if (box.pt2.x - box.pt1.x < 0.01 || box.pt2.y - box.pt1.y < 0.01 || box.pt2.z - box.pt1.z < 0.01 ) break;

	}
}





} // End namespace I3D

#endif // I3D_OPERATIONS_H