#ifndef I3D_OPERATIONS_H
#define I3D_OPERATIONS_H

#include <type_traits>

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

/*! \defgroup GeometricEntities Entidades geométricas
 *  Puntos, lineas, ...
 *  \{
 */

namespace geometry
{

// forward declaration
template<typename T> class Segment;
template<typename T> class Segment3D;
class GroupLines;

/* ---------------------------------------------------------------------------------- */
/*             TEMPLATES PARA OPERACIONES ENTRE ENTIDADES GEOMÉTRICAS                 */
/* ---------------------------------------------------------------------------------- */

/*!
 * \brief Longitud o modulo de un vector 2D
 * \param[in] v vector
 * \return Longitud
 * \deprecated { Reemplazada por I3D::Translate::transform }
 */
template<typename Point_t> inline
I3D_DEPRECATED("double module(const Point_t &v)")
double length(const Point_t &v)
{
  return sqrt(v.x*v.x + v.y*v.y);
}

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

// Comprueba si un punto esta a la derecha o izquierda de una linea
template<typename Point_t> inline
int isLeft( Point_t ln_pt1, Point_t ln_pt2, Point_t pt )
{
  double aux = (ln_pt2.x - ln_pt1.x) * (pt.y - ln_pt1.y)
             - (pt.x - ln_pt1.x) * (ln_pt2.y - ln_pt1.y);
  if (aux > 0) return 1;
  else if (aux < 0) return -1;
  else return 0;
}

/*!
 * \brief Crea un buffer entorno a una linea
 * \param[in] ln Línea
 * \param[in] size Tamaño de buffer
 * \param[out] buff Buffer
 */
template<typename Point_t> inline
void lineBuffer(const Segment<Point_t> &ln, int size, std::vector<Point_t> *buff)
{
  *buff = std::vector<Point_t>(4);
  Point_t pt1 = ln.pt1;
  Point_t pt2 = ln.pt2;
  double acimut = azimut(pt1, pt2);
  double dx = size * sin(acimut + I3D_PI_2);
  double dy = size * cos(acimut + I3D_PI_2);
  (*buff)[0] = Point_t(pt1.x + dx, pt1.y + dy);
  (*buff)[1] = Point_t(pt2.x + dx, pt2.y + dy);
  (*buff)[2] = Point_t(pt2.x - dx, pt2.y - dy);
  (*buff)[3] = Point_t(pt1.x - dx, pt1.y - dy);
}

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
  PointD v1 = pt - ln.pt1;
  PointD v2 = ln.vector();
  double daux = dotProduct(v1, v2);
  double r = daux / (v2.x * v2.x + v2.y * v2.y);

  if (typeid(typename Point_t::value_type) == typeid(int)) {
    ptp->x = ln.pt1.x + I3D_ROUND_TO_INT(v2.x * r);
    ptp->y = ln.pt1.y + I3D_ROUND_TO_INT(v2.y * r);
  } else {
    ptp->x = ln.pt1.x + static_cast<typename Point_t::value_type>(v2.x * r);
    ptp->y = ln.pt1.y + static_cast<typename Point_t::value_type>(v2.y * r);
  }

  if (daux <= 0) iret = -1;
  else if (daux >= (v2.x * v2.x + v2.y * v2.y)) iret = 1;
  else if (daux == 0) iret = 2; // Esta en la línea
  return iret;
}


template<typename Point_t> inline
I3D_DEPRECATED("int projectPointInSegment(const Segment3D<Point_t> &ln, const Point_t &pt, Point_t *ptp)")
int projectPointInSegment3D(const Segment3D<Point_t> &ln, const Point_t &pt, Point_t *ptp)
{
  int iret = 0;
  if (pt == ln.pt1 || pt == ln.pt2) {
    *ptp = pt;
    return 2;
  }
  Point3D v1 = pt - ln.pt1;
  Point3D v2 = ln.vector();
  double daux = v1.ddot(v2);
  double r = daux / (v2.x * v2.x + v2.y * v2.y + v2.z * v2.z);

  if (typeid(typename Point_t::value_type) == typeid(int)) {
    ptp->x = ln.pt1.x + I3D_ROUND_TO_INT(v2.x * r);
    ptp->y = ln.pt1.y + I3D_ROUND_TO_INT(v2.y * r);
    ptp->z = ln.pt1.z + I3D_ROUND_TO_INT(v2.z * r);
  } else {
    ptp->x = ln.pt1.x + static_cast<typename Point_t::value_type>(v2.x * r);
    ptp->y = ln.pt1.y + static_cast<typename Point_t::value_type>(v2.y * r);
    ptp->z = ln.pt1.z + static_cast<typename Point_t::value_type>(v2.z * r);
  }
  //*ptp = ln.pt1 + v2 * r;

  if (daux <= 0) iret = -1;
  else if (daux >= (v2.x * v2.x + v2.y * v2.y + v2.z * v2.z)) iret = 1;
  else if (daux == 0) iret = 2; // Esta en la línea
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
    ptp->x = ln.pt1.x + I3D_ROUND_TO_INT(v2.x * r);
    ptp->y = ln.pt1.y + I3D_ROUND_TO_INT(v2.y * r);
    ptp->z = ln.pt1.z + I3D_ROUND_TO_INT(v2.z * r);
  } else {
    ptp->x = ln.pt1.x + static_cast<typename Point_t::value_type>(v2.x * r);
    ptp->y = ln.pt1.y + static_cast<typename Point_t::value_type>(v2.y * r);
    ptp->z = ln.pt1.z + static_cast<typename Point_t::value_type>(v2.z * r);
  }
  //*ptp = ln.pt1 + point_cast<Point_t>(v2 * r);

  if (daux <= 0) iret = -1;
  else if (daux >= (v2.x * v2.x + v2.y * v2.y + v2.z * v2.z)) iret = 1;
  else if (daux == 0) iret = 2; // Esta en la línea
  return iret;
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
  //Point_t _pt(pt);
  return distance(pt, ptp);
}


template<typename Point3_t> inline 
I3D_DEPRECATED("double distPointToSegment(const Point3_t &pt, const Segment3D<Point3_t> &ln)")
double distPointToSegment3D(const Point3_t &pt, const Segment3D<Point3_t> &ln)
{
  Point3_t ptp;
  int ipr = projectPointInSegment(ln, pt, &ptp);

  if (ipr == -1) ptp = ln.pt1;
  else if (ipr == 1) ptp = ln.pt2;
  //Point3<T> _pt(pt);
  return distance(pt, ptp);
}

template<typename Point3_t> inline
double distPointToSegment(const Point3_t &pt, const Segment3D<Point3_t> &ln)
{
  Point3_t ptp;
  int ipr = projectPointInSegment(ln, pt, &ptp);

  if (ipr == -1) ptp = ln.pt1;
  else if (ipr == 1) ptp = ln.pt2;
  //Point3<T> _pt(pt);
  return distance(pt, ptp);
}

/*!
 * \brief distPointToLine
 * \param[in] pt
 * \param[in] ln
 * \return
 */
//I3D_EXPORT double distPointToLine(const PointI &pt, const Line &ln);
//double distPointToLine(const PointI &pt, const Line &ln)
//{
//  PointI v1 = pt - ln.pt1;
//  PointI v2 = ln.vector();
//  return abs(v1.ddot(v2) / (v2.x * v2.x + v2.y * v2.y));
//}

/*!
 * \brief Calcula la distancia mínima entre dos segmentos de linea.
 * \param[in] ln1 Segmento 1
 * \param[in] ln2 Segmento 2
 * \return Distancia entre segmentos
 */
template<typename Point_t> inline
double minDistanceSegments(const Segment<Point_t> &ln1, const Segment<Point_t> &ln2)
{
  double dist[4];
  dist[0] = distPointToSegment(ln1.pt1, ln2);
  dist[1] = distPointToSegment(ln1.pt2, ln2);
  dist[2] = distPointToSegment(ln2.pt1, ln1);
  dist[3] = distPointToSegment(ln2.pt2, ln1);
  return *std::min_element(dist, dist + 4);
}

/*!
 * \brief Intersect de dos segmentos de línea
 * \param ln1 Primer segmento
 * \param ln2 Segundo segmento
 * \param pt Punto de intersección
 * \return
 */
 //I3D_EXPORT int intersectSegments(const Line &ln1, const Line &ln2, PointI *pt);
template <typename Point_t>
inline int intersectSegments(const Segment<Point_t> &ln1, const Segment<Point_t> &ln2, Point_t *pt)
{
  int iret = 0;
  Point_t vs1, vs2;
  vs1 = ln1.vector();
  vs2 = ln2.vector();
  // si el producto vectorial de los vectores que unen ambos segmentos es 0 son paralelas
  if (double cross_product = crossProduct(vs1, vs2)) {
    Point_t v11_12 = ln2.pt1 - ln1.pt1;
    double t = crossProduct(v11_12, vs2) / cross_product;
    double u = crossProduct(v11_12, vs1) / cross_product;
    if (t >= 0.  &&  t <= 1 && u >= 0.  &&  u <= 1) {
      if (typeid(typename Point_t::value_type) == typeid(int)) {
        pt->x = I3D_ROUND_TO_INT(ln1.pt1.x + t * vs1.x);
        pt->y = I3D_ROUND_TO_INT(ln1.pt1.y + t * vs1.y);
      } else {
        pt->x += static_cast<Point_t::value_type>(ln1.pt1.x + t * vs1.x);
        pt->y += static_cast<Point_t::value_type>(ln1.pt1.y + t * vs1.y);
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
//I3D_EXPORT int intersectLines(const Line &ln1, const Line &ln2, PointI *pt);
template<typename Point_t>
inline int intersectLines(const Segment<Point_t> &ln1, const Segment<Point_t> &ln2, Point_t *pt)
{
  int iret = 0;
  Point_t vs1, vs2;
  vs1 = ln1.vector();
  vs2 = ln2.vector();
  // si el producto vectorial de los vectores que unen ambos segmentos es 0 son paralelas
  if (double cross_product = crossProduct(vs1, vs2)) {
    Point_t v11_12 = ln2.pt1 - ln1.pt1;
    double t = crossProduct(v11_12, vs2) / cross_product;
    if (typeid(typename Point_t::value_type) == typeid(int)) {
      pt->x = I3D_ROUND_TO_INT(ln1.pt1.x + t * vs1.x);
      pt->y = I3D_ROUND_TO_INT(ln1.pt1.y + t * vs1.y);
    } else {
      pt->x = static_cast<typename Point_t::value_type>(ln1.pt1.x + t * vs1.x);
      pt->y = static_cast<typename Point_t::value_type>(ln1.pt1.y + t * vs1.y);
    }
    iret = 1;
  }
  return(iret);
}

template<typename Point_t>
bool linePlaneIntersection(const std::array<double, 4> &plane, const Segment<Point_t> &ln, Point_t *intersect)
{
  //Segment<Point_t>  v = ln.vector();
  //double num = (plane[0] * ln.pt1.x + plane[1] * ln.pt1.y + plane[2] * ln.pt1.z + plane[3]);
  //double den = (plane[0] * v + plane[1] * v + plane[2] * v);
  //double t = num / den;
  //planePoint->x = point.x + plane[0] * t;
  //planePoint->y = point.y + plane[1] * t;
  //planePoint->z = point.z + plane[2] * t;
}

/*!
 * \brief joinLinesByDist
 * \param[in] linesIn
 * \param[out] linesOut
 * \param[in] dist
 */
I3D_EXPORT void joinLinesByDist(const std::vector<Segment<PointI>> &linesIn, std::vector<Segment<PointI>> *linesOut, int dist);


I3D_EXPORT void groupParallelLines(std::vector<Segment<Point<int>>> linesaux, std::vector<GroupLines> *curLinesGrops, double angTol);

I3D_EXPORT void groupLinesByDist(const std::vector<Segment<PointI>> &linesIn, std::vector<GroupLines> *curLinesGrops, int dist);

I3D_EXPORT void delLinesGroupBySize(std::vector<GroupLines> *vlg, int size);

template<typename Point_t> inline
int pointNearest(const std::vector<Point_t> &pts_fourier, const Point_t &pt_intersect)
{
  double distmin = I3D_DOUBLE_MAX;
  int ipt = -1;
  for (size_t i = 0; i < pts_fourier.size(); i++) {
    double dist = distance(pts_fourier[i], pt_intersect);
    if (distmin > dist) {
      distmin = dist;
      ipt = static_cast<int>(i);
    }
  }
  return ipt;
}

/*!
 * Comprobación de que los 3 puntos están en la misma línea
 */
template<typename Point_t> inline
bool isCollinearPoints(const Point_t &pt_c, const Segment<Point_t> &line_i_r, double tolerance = 2.)
{
  return tolerance > distPointToSegment(pt_c, line_i_r);
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
  double max_dist = I3D_DOUBLE_MAX;
  double dist;
  Polygon<Point_t> _p = polygon;
  for (size_t i = 0; i < polygon.size(); i++) {
    if (i == polygon.size() - 1) {
      dist = distPointToSegment(pt, Segment<Point_t>(_p[i], _p[0]));
    } else {
      dist = distPointToSegment(pt, Segment<Point_t>(_p[i], _p[i+1]));
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
  double max_dist = I3D_DOUBLE_MAX;
  double dist;
  Polygon3D<Point_t> _p = polygon;
  for (size_t i = 0; i < polygon.size(); i++) {
    if (i == polygon.size() - 1) {
      dist = distPointToSegment(pt, Segment3D<Point_t>(_p[i], _p[0]));
    } else {
      dist = distPointToSegment(pt, Segment3D<Point_t>(_p[i], _p[i+1]));
    }
    if (dist < max_dist) max_dist = dist;
  }
  return max_dist;
}

/*!
 * \brief Calcula la distancia de un punto a un poligono 3d mediante iteradores
 *
 * \param[in] pt Punto
 * \param[in] polygon Poligono
 * \return Distancia de un punto a un poligono
 */
template<typename Point_t, typename Polygon_it_t> inline 
double distPointToPolygon(const Point_t &pt, Polygon_it_t it_begin, Polygon_it_t it_end)
{
  double max_dist = I3D_DOUBLE_MAX;
  double dist;
  Polygon_it_t it = it_begin;
  Point_t prev = *(it_end-1);
  while (it != it_end) {
    dist = distPointToSegment3D(prev, *it, center3D);
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
  if ( normal == 0. ) throw std::runtime_error( "3 puntos alineados" );
  return(num / normal);
}




//TODO: testear
// http://geomalgorithms.com/a04-_planes.html

/*!
 * \brief Proyecta ortogonalmente un punto en un plano
 *
 * \param[in] point Punto
 * \param[in] plane Parametros de la ecuación general del plano (A, B, C, D)
 * \param[out] planePoint Punto proyectado
 */
template<typename Point_t> inline 
bool projectPointToPlane(const Point_t &point, const std::array<double, 4> &plane, Point_t *planePoint)
{
  double num = plane[0] * point.x + plane[1] * point.y + plane[2] * point.z + plane[3];
  double den = (plane[0] * plane[0] + plane[1] * plane[1] + plane[2] * plane[2]);
  if (den != 0.) {
    double t = num / den;
    planePoint->x = point.x - plane[0] * t;
    planePoint->y = point.y - plane[1] * t;
    planePoint->z = point.z - plane[2] * t;
    return true;
  } return false;
  //return projectPointToPlane(point, plane, planePoint, Point_t(plane[0], plane[1], plane[2]));
}

/*!
 * \brief Proyecta un punto en un plano según una dirección dada
 *
 * \param[in] point Punto que se quiere proyectar
 * \param[in] plane Parametros de la ecuación general del plano (A, B, C, D)
 * \param[out] planePoint Punto proyectado
 * \param[in] v Vector según el cual se proyecta
 */
template<typename Point_t>
bool projectPointToPlane(const Point_t &point, const std::array<double, 4> &plane, Point_t *planePoint, const Point_t &v)
{
  double num = plane[0] * point.x + plane[1] * point.y + plane[2] * point.z + plane[3];
  double den = (plane[0] * v.x + plane[1] * v.y + plane[2] * v.z);
  if (den != 0.) {
    double t = num / den;
    planePoint->x = point.x - v.x * t;
    planePoint->y = point.y - v.y * t;
    planePoint->z = point.z - v.z * t;
  } else return false;
}

// TODO: esto debería ser mucho mas generico. Para cualquier entidad gráfica

/*!
 * \brief Proyecta un poligono en un plano
 *
 * \param[in] polygon Puntos que definen el plano
 * \param[out] plane Parametros de la ecuación general del plano (A, B, C, D)
 * \param[int] projectPolygon Poligono proyectado
 */
template<typename Point_t>
I3D_EXPORT inline void projectPolygonToPlane(const Polygon3D<Point_t> &polygon, const std::array<double, 4> &plane, Polygon3D<Point_t> *projectPolygon)
{
  for (int i = 0; i < polygon.getSize(); i++) {
    Point_t pt;
    projectPointToPlane(polygon3d[i], plane, &pt);
    projectPolygon->add();
  }
}


//TODO: Mover aqui la determinación de ángulos entre superficies (linea-linea, linea plano, plano-plano)

//template<typename Point3_t> inline
//double linePlaneAngle(const Segment3D<Point3_t> &line, const std::array<double, 4> &plane)
//{
//}

/*!
 * \brief Ángulo entre línea y plano
 * \param[in] line Línea
 * \param[in] plane Vector normal al plano
 * \return Ángulo
 * http://www.vitutor.com/geometry/distance/line_plane.html
 */
template<typename Point3_t> inline
double angleBetweenLineAndPlane(const Segment3D<Point3_t> &line, const Point3_t &plane)
{
  Point3_t v1 = line.vector();
  double dot = dotProduct3D(v1, plane);
  double det = module3D(v1)*module3D(plane);
  return det ? asin(dot / det) : 0.;
}

/*!
 * \brief Ángulo entre dos planos
 * \param[in] plane1 Ecuación del primer plano
 * \param[in] plane2 Ecuación del segundo plano
 * \return Ángulo
 * http://www.vitutor.com/geometry/distance/angle_planes.html
 */
template<typename T> inline
double angleBetweenPlanes(const std::array<T, 4> &plane1, const std::array<T, 4> &plane2)
{
  return angleBetweenPlanes(Point3<T>(plane1[0], plane1[1], plane1[2]), Point3<T>(plane2[0], plane2[1], plane2[2]));
}

/*!
 * \brief Ángulo entre dos planos
 * \param[in] plane1 Vector normal al primer plano
 * \param[in] plane2 Vector normal al segundo plano
 * \return Ángulo
 * http://www.vitutor.com/geometry/distance/angle_planes.html
 */
template<typename Vect_t> inline
double angleBetweenPlanes(const Vect_t &plane1, const Vect_t &plane2)
{
  double dot = dotProduct3D(plane1, plane2);
  double det = module3D(plane1) * module3D(plane2);
  return det ? acos(dot / det) : 0.;
}

I3D_DISABLE_WARNING(4100)

//TODO: En https://github.com/omtinez/CenterPolygon estas variables las da como enteros
//pero aqui la definimos como double para evitar un problema de perdida de precisión al dividir
//que se ha visto que daba problemas
//#define N_CELLS 20.
//#define M_CELLS 20.
#define CONSECUTIVE_MISS	15

template<typename Point_t> inline
Point_t findInscribedCircleSequential(const Polygon<Point_t> &polygon, const Window<Point_t> bounds, double nCells, double mCells) 
{
  Point_t pia = bounds.getCenter();
  Point_t tmp;

	// calculate the required increment for x and y
	double increment_x = (bounds.pt2.x - bounds.pt1.x) / nCells;
	double increment_y = (bounds.pt2.y - bounds.pt1.y) / mCells;

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

// Polo de inaccesibilidad para un poligono 2D
template<typename Point_t> inline
void poleOfInaccessibility(const Polygon<Point_t> &polygon, Point_t *pole, double nCells = 20., double mCells = 20.) 
{
  if (pole == NULL) return;
  Window<Point_t> w = polygon.getWindow();

	Point_t point_tmp;

	int count = 1;
	while (count++) {

		/*if (method == METHOD_SEQUENTIAL) {*/
			point_tmp =	findInscribedCircleSequential(polygon, w, nCells, mCells);
		/*} else if (method == METHOD_RANDOMIZED) {
			point_tmp = findInscribedCircleRandomized(polygon, w);
		}*/

		pole->x = point_tmp.x;
		pole->y = point_tmp.y;

    Point_t aux;
    aux.x = (w.pt2.x - w.pt1.x) / (sqrt(2.) * 2.);
		aux.y = (w.pt2.y - w.pt1.y) / (sqrt(2.) * 2.);

    w.pt1 = *pole - aux;
    w.pt2 = *pole + aux;

		if (w.pt2.x - w.pt1.x < 0.01 || w.pt2.y - w.pt1.y < 0.01) break;

	}
}









template<typename Point3_t> inline
Point3_t findInscribedCircleSequential(const Polygon3D<Point3_t> &polygon, const Box<Point3_t> bounds, double xCells, double yCells, double zCells) 
{
  Point3_t pia = bounds.getCenter();
  Point3_t tmp;

  // Se determina el plano de mejor ajuste
  std::array<double, 4> plane;
  nPointsPlaneLS(polygon.begin(), polygon.end(), plane);
  //Polygon3D<T> _poligon = polygon;
  //// Proyectar polilinea en plano
  //Polygon3D<T> poligon2;
  //for (int i = 0; i < _poligon.getSize(); i++) {
  //  Point3<T> pt;
  //  projectPointToPlane(_poligon[i], plane, &pt);
  //  poligon2.add(pt);
  //}
  // Máxima distancia de los puntos al plano
  double max_dist = I3D_DOUBLE_MIN;
  for (int i = 0; i < polygon.size(); i++) {
    Point3_t pt;
    double distance = distantePointToPlane(polygon[i], plane);
    if (distance > max_dist) max_dist = distance;
  }

  // Plano horizontal (1,1,0)
  std::array<double, 4> plane_z;
  plane_z[0] = 1;
  plane_z[1] = 1;
  plane_z[2] = 0;
  plane_z[3] = pia.x + pia.y;

  Polygon<Point<typename Point3_t::value_type>> poligon_z;
  for (int i = 0; i < polygon.size(); i++) {
    Point3_t pt;
    projectPointToPlane(polygon[i], plane_z, &pt);
    poligon_z.add(Point<typename Point3_t::value_type>(pt.x, pt.y));
  }


	double increment_x = (bounds.pt2.x - bounds.pt1.x) / xCells;
	double increment_y = (bounds.pt2.y - bounds.pt1.y) / yCells;
  double increment_z = (bounds.pt2.z - bounds.pt1.z) / zCells;

  //max_dist = std::max(max_dist, std::min(increment_x, std::min(increment_y, increment_z)));



	double max_distance = -I3D_DOUBLE_MAX;

  double tmp_distance = 0.;
	for (int i = 0; i <= xCells; i++) {

		tmp.x = bounds.pt1.x + i * increment_x;

		for (int j = 0; j <= yCells; j++) {
      
      tmp.y = bounds.pt1.y + j * increment_y;
      
      for (int k = 0; k <= zCells; k++) {

        tmp.z = bounds.pt1.z + k * increment_z;


        if ( bounds.containsPoint(tmp) ) {
              
          Point3_t pt;
          projectPointToPlane(tmp, plane_z, &pt);
          Point<typename Point3_t::value_type> pt_2d(pt.x, pt.y);
          if (abs(distantePointToPlane(tmp, plane)) < max_dist && poligon_z.isInner(pt_2d)) { 
            // Proyectar al plano z = 0 y hacer isInner()
            // 
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
	}

	return pia;
}


template<typename Point3_t> inline
void poleOfInaccessibility(const Polygon3D<Point3_t> &polygon, Point3_t *pole, double xCells = 20., double yCells = 20., double zCells = 20.) 
{
  if (pole == NULL) return;
  Box<Point3_t> box = polygon.getBox();

	Point3_t point_tmp;

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

    Point3_t aux;
    aux.x = (box.pt2.x - box.pt1.x) / (sqrt(2.) * 2.);
		aux.y = (box.pt2.y - box.pt1.y) / (sqrt(2.) * 2.);
    aux.z = (box.pt2.z - box.pt1.z) / (sqrt(2.) * 2.);

    box.pt1 = *pole - aux;
    box.pt2 = *pole + aux;

		if (box.pt2.x - box.pt1.x < 0.01 || box.pt2.y - box.pt1.y < 0.01 || box.pt2.z - box.pt1.z < 0.01 ) break;

	}
}























// Forma generica para cualquier contenedor


template<typename Point_t> inline
int projectPointInSegment(const Point_t &segment_pt1, const Point_t &segment_pt2, const Point_t &pt, Point_t *ptp)
{
  int iret = 0;
  if ((pt.x == segment_pt1.x && pt.y == segment_pt1.y ) || 
      (pt.x == segment_pt2.x && pt.y == segment_pt2.y)) {
    *ptp = pt;
    return 2;
  }
  std::array<double, 2> v1;
  std::array<double, 2> v2;
  v1[0] = pt.x - segment_pt1.x;
  v1[1] = pt.y - segment_pt1.y;
  v2[0] = segment_pt2.x - segment_pt1.x;
  v2[1] = segment_pt2.y - segment_pt1.y;

  double daux = v1[0]*v2[0] + v1[1]*v2[1];
  double r = daux / (v2[0] * v2[0] + v2[1] * v2[1]);

  if (typeid(typename Point_t::value_type) == typeid(int)) {
    ptp->x = segment_pt1.x + I3D_ROUND_TO_INT(v2[0] * r);
    ptp->y = segment_pt1.y + I3D_ROUND_TO_INT(v2[1] * r);
  } else {
    ptp->x = segment_pt1.x + static_cast<typename Point_t::value_type>(v2[0] * r);
    ptp->y = segment_pt1.y + static_cast<typename Point_t::value_type>(v2[1] * r);
  }

  //TODO: comprobar esto
  if (daux <= 0) iret = -1;
  else if (daux >= (v2[0] * v2[0] + v2[1] * v2[1])) iret = 1;
  else if (daux == 0) iret = 2; // Esta en la línea
  return iret;
}

template<typename Point_t> inline
int projectPointInSegment3D(const Point_t &segment_pt1, const Point_t &segment_pt2, const Point_t &pt, Point_t *ptp)
{
  int iret = 0;
  if (pt == segment_pt1 || pt == segment_pt2) {
    *ptp = pt;
    return 2;
  }
  Point_t v1; //= pt - segment_pt1;
  Point_t v2; //= ln.vector();
  v1.x = pt.x - segment_pt1.x;
  v1.y = pt.y - segment_pt1.y;
  v1.z = pt.z - segment_pt1.z;
  v2.x = segment_pt2.x - segment_pt1.x;
  v2.y = segment_pt2.y - segment_pt1.y;
  v2.z = segment_pt2.z - segment_pt1.z;

  double daux = v1.x*v2.x + v1.y*v2.y + v1.z*v2.z; // v1.ddot(v2);
  double r = daux / (v2.x * v2.x + v2.y * v2.y + v2.z * v2.z);

  if (typeid(typename Point_t::value_type) == typeid(int)) {
    ptp->x = segment_pt1.x + I3D_ROUND_TO_INT(v2.x * r);
    ptp->y = segment_pt1.y + I3D_ROUND_TO_INT(v2.y * r);
    ptp->z = segment_pt1.z + I3D_ROUND_TO_INT(v2.z * r);
  } else {
    ptp->x = segment_pt1.x + static_cast<typename Point_t::value_type>(v2.x * r);
    ptp->y = segment_pt1.y + static_cast<typename Point_t::value_type>(v2.y * r);
    ptp->z = segment_pt1.z + static_cast<typename Point_t::value_type>(v2.z * r);
  }

  if (daux <= 0) iret = -1;
  else if (daux >= (v2.x * v2.x + v2.y * v2.y + v2.z * v2.z)) iret = 1;
  else if (daux == 0) iret = 2; // Esta en la línea
  return iret;
}

/*!
 * \brief Calcula la distancia de un punto a un segmento de linea.
 * \param[in] pt Punto
 * \param[in] ln Linea
 * \return Distancia de un punto a una segmento de linea
 */
template<typename Point_t> inline
double distPointToSegment(const Point_t &segment_pt1, const Point_t &segment_pt2, const Point_t &pt)
{
  Point_t ptp;
  int ipr = projectPointInSegment(segment_pt1, segment_pt2, pt, &ptp);

  if (ipr == -1) ptp = segment_pt1;
  else if (ipr == 1) ptp = segment_pt2;
  return distance(pt, ptp);
}

/*!
 * \brief Calcula la distancia de un punto a un segmento de linea 3D.
 * \param[in] pt Punto
 * \param[in] ln Linea
 * \return Distancia de un punto a una segmento de linea
 */
template<typename Point_t> inline
double distPointToSegment3D(const Point_t &segment_pt1, const Point_t &segment_pt2, const Point_t &pt)
{
  Point_t ptp;
  int ipr = projectPointInSegment3D(segment_pt1, segment_pt2, pt, &ptp);

  if (ipr == -1) ptp = segment_pt1;
  else if (ipr == 1) ptp = segment_pt2;
  return distance(pt, ptp);
}


template<typename Point_t> inline
bool collinearPoints(const Point_t &segment_pt1, const Point_t &segment_pt2, const Point_t &pt_c, double tolerance)
{
  return tolerance > distPointToSegment(segment_pt1, segment_pt2, pt_c);
}

/*!
 * \brief Intersect de dos segmentos de línea
 * \param ln1 Primer segmento
 * \param ln2 Segundo segmento
 * \param pt Punto de intersección
 * \return
 */
template <typename Point_t>
inline int intersectSegments(const Point_t &segment1_pt1, const Point_t &segment1_pt2, const Point_t &segment2_pt1, const Point_t &segment2_pt2, Point_t *pt)
{
  int iret = 0;
  std::array<double, 2> vs1;
  std::array<double, 2> vs2;
  vs1[0] = segment1_pt2.x - segment1_pt1.x;
  vs1[1] = segment1_pt2.y - segment1_pt1.y;
  vs2[0] = segment2_pt2.x - segment2_pt1.x;
  vs2[1] = segment2_pt2.y - segment2_pt1.y;

  // si el producto vectorial de los vectores que unen ambos segmentos es 0 son paralelas
  double cross_product = vs1[0]*vs2[1] - vs1[1]*vs2[0];
  if (cross_product) {
    Point_t v11_12(segment2_pt1.x - segment1_pt1.x, segment2_pt1.y - segment1_pt1.y);
    double t = (v11_12.x*vs2[1] - v11_12.y*vs2[0]) / cross_product;
    double u = (v11_12.x*vs1[1] - v11_12.y*vs1[0]) / cross_product;
    if (t >= 0.  &&  t <= 1 && u >= 0.  &&  u <= 1) {
      if (typeid(typename Point_t::value_type) == typeid(int)) {
        pt->x = I3D_ROUND_TO_INT(segment1_pt1.x + t * vs1[0]);
        pt->y = I3D_ROUND_TO_INT(segment1_pt1.y + t * vs1[1]);
      } else {
        pt->x += static_cast<Point_t::value_type>(segment1_pt1.x + t * vs1[0]);
        pt->y += static_cast<Point_t::value_type>(segment1_pt1.y + t * vs1[1]);
      }
      iret = 1;
    }
  }
  return(iret);
}


template<typename T> inline
typename std::iterator_traits<T>::value_type 
findInscribedCircleSequential(T in_first, T in_last, 
                              const std::array<typename std::iterator_traits<T>::value_type, 2> bounds, 
                              double xCells, double yCells, double zCells) 
{
  typedef typename std::iterator_traits<T>::value_type Point3_t;

  Point3_t pia((bounds[0].x + bounds[1].x) / 2., (bounds[0].y + bounds[1].y) / 2., (bounds[0].z + bounds[1].z) / 2.);
  Point3_t tmp;
  
  // Se determina el plano de mejor ajuste
  std::array<double, 4> plane;
  nPointsPlaneLS(in_first, in_last, plane, true);

  // Máxima distancia de los puntos al plano
  double max_dist = I3D_DOUBLE_MIN;

  T temp = in_first;
  while (temp != in_last) {
    Point3_t pt;
    double distance = distantePointToPlane(*temp, plane);
    if (distance > max_dist) max_dist = distance;
    *temp++;
  }


  // Proyectar polilinea en el plano de mejor ajuste
  std::vector<Point3_t> poligon_plane;
  temp = in_first;
  while (temp != in_last) {
    Point3_t pt;
    projectPointToPlane(*temp, plane, &pt);
    msgInfo("%f %f %f", pt.x, pt.y, pt.z);
    poligon_plane.push_back(pt);
    *temp++;
  }


  // Plano horizontal (1,1,0)
  std::array<double, 4> plane_z;
  plane_z[0] = 0;
  plane_z[1] = 0;
  plane_z[2] = 1;
  plane_z[3] = -plane_z[0] * pia.x - plane_z[1] * pia.y - plane_z[2] * pia.z;

  std::vector<Point3_t> poligon_z;

  for (int i = 0; i < poligon_plane.size(); i++){
    Point3_t pt;
    projectPointToPlane(poligon_plane[i], plane_z, &pt/*, Point3_t(plane_z[0], plane_z[1], plane_z[2])*/);
    msgInfo("%f %f %f", pt.x, pt.y, pt.z);
    poligon_z.push_back(pt);
  }

  std::array<Point3_t, 2> w_poligon_z;
  w_poligon_z[0].x = w_poligon_z[0].y = w_poligon_z[0].z = std::numeric_limits<typename Point3_t::value_type>().max();
  w_poligon_z[1].x = w_poligon_z[1].y = w_poligon_z[1].z = -std::numeric_limits<typename Point3_t::value_type>().max();
  for (size_t i = 0; i < poligon_z.size(); i++) {
    if (w_poligon_z[0].x > poligon_z[i].x) w_poligon_z[0].x = poligon_z[i].x;
    if (w_poligon_z[0].y > poligon_z[i].y) w_poligon_z[0].y = poligon_z[i].y;
    if (w_poligon_z[1].x < poligon_z[i].x) w_poligon_z[1].x = poligon_z[i].x;
    if (w_poligon_z[1].y < poligon_z[i].y) w_poligon_z[1].y = poligon_z[i].y;
  }

	double increment_x = (bounds[1].x - bounds[0].x) / xCells;
	double increment_y = (bounds[1].y - bounds[0].y) / yCells;
  double increment_z = (bounds[1].z - bounds[0].z) / zCells;


	double max_distance = -I3D_DOUBLE_MAX;

  double tmp_distance = 0.;
	for (int i = 0; i <= xCells; i++) {

		tmp.x = bounds[0].x + i * increment_x;

		for (int j = 0; j <= yCells; j++) {
      
      tmp.y = bounds[0].y + j * increment_y;
      
      for (int k = 0; k <= zCells; k++) {

        tmp.z = bounds[0].z + k * increment_z;

        if ((bounds[1].x >= tmp.x) && (bounds[1].y >= tmp.y) && 
            (bounds[1].z >= tmp.z) && (bounds[0].x <= tmp.x) && 
            (bounds[0].y <= tmp.y) && (bounds[0].z <= tmp.z)) {
              
          Point3_t pt;
          projectPointToPlane(tmp, plane_z, &pt);
          //Point2_t pt_2d(pt.x, pt.y);
          if (abs(distantePointToPlane(tmp, plane)) < max_dist) { 
            //if (poligon_z.isInner(pt_2d)) {

            //}
            bool isInner = false;
            int nIntersection = 0;
            bool bVertex = false;
            
              //Point3_t ptp;
              //Point3_t sPointH_pt1 = point;
              //Point3_t sPointH_pt1(w.pt2.x, point.y));
              //if (point.y == segment.pt1.y || point.y == segment.pt2.y)
              //  bVertex = true;
              //nIntersection += intersectSegments(segment, sPointH, &ptp);
              //*temp++;
            for (size_t i = 0, j = 1; i < poligon_z.size(); i++, j++) {
              if (j == poligon_z.size()) j = 0;
    
              // El punto es colineal con el segmento
              if (collinearPoints(poligon_z[i], poligon_z[j], pt, 0.005)) {
                isInner = true;
                break;
              }
              Point3_t ptp;
              Point3_t pth1 = pt;
              Point3_t pth2(w_poligon_z[1].x, pt.y, 0);
              if (pt.y == poligon_z[i].y || pt.y == poligon_z[j].y)
                bVertex = true;
              nIntersection += intersectSegments(poligon_z[i], poligon_z[j], pth1, pth2, &ptp);
            }
            if (isInner == false) {
              if (bVertex) 
                nIntersection--;
              if (nIntersection % 2 == 0) isInner = false;
              else isInner = true;
            }

            if (isInner) {
              //tmp_distance = distPointToPolygon(tmp, polygon);
              double tmp_distance = 0.;
              double dist;
              temp = in_first;
              T prev = in_last-1;
              while (temp != in_last) {
                dist = distPointToSegment(*prev, *temp, tmp);
                if (dist < tmp_distance) tmp_distance = dist;
                *prev = *temp++;
              }

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
		}
	}
	return pia;
}

// La aproximación creo que no esta bien.
// Debería calcular el plano de mejor ajuste y despues proyectar los puntos a ese plano
// Despues se tendria que girar ese plano para que sea N(0,0,1)
// https://stackoverflow.com/questions/23814234/convert-3d-plane-to-2d
//template<typename T> inline
//void poleOfInaccessibility(T in_first, T in_last, typename std::iterator_traits<T>::value_type *pole, double xCells = 20., double yCells = 20., double zCells = 20.) 
//{
//  if (pole == NULL) return;
//
//  typedef typename std::iterator_traits<T>::value_type Point_t;
//  T temp = in_first;
//  
//  //TODO: sacar a función
//  std::array<Point_t, 2> box;
//  box[0].x = box[0].y = box[0].z = std::numeric_limits<typename Point_t::value_type>().max();
//  box[1].x = box[1].y = box[1].z = -std::numeric_limits<typename Point_t::value_type>().max();
//  while (temp != in_last) {
//    if (box[0].x > temp->x) box[0].x = temp->x;
//    if (box[0].y > temp->y) box[0].y = temp->y;
//    if (box[0].z > temp->z) box[0].z = temp->z;
//    if (box[1].x < temp->x) box[1].x = temp->x;
//    if (box[1].y < temp->y) box[1].y = temp->y;
//    if (box[1].z < temp->z) box[1].z = temp->z;
//    *temp++;
//  }
// 
//  Point_t point_tmp;
//
//	int count = 1;
//	while (count++) {
//
//		/*if (method == METHOD_SEQUENTIAL) {*/
//			point_tmp =	findInscribedCircleSequential(in_first, in_last, box, xCells, yCells, zCells);
//		/*} else if (method == METHOD_RANDOMIZED) {
//			point_tmp = findInscribedCircleRandomized(polygon, w);
//		}*/
//
//		pole->x = point_tmp.x;
//		pole->y = point_tmp.y;
//    pole->z = point_tmp.z;
//
//    Point_t aux;
//    aux.x = (box[1].x - box[0].x) / (sqrt(2.) * 2.);
//		aux.y = (box[1].y - box[0].y) / (sqrt(2.) * 2.);
//    aux.z = (box[1].z - box[0].z) / (sqrt(2.) * 2.);
//
//    box[0] = *pole - aux;
//    box[1] = *pole + aux;
//
//		if (box[1].x - box[0].x < 0.01 || box[1].y - box[0].y < 0.01 || box[1].z - box[0].z < 0.01 ) break;
//
//	}
//}








//TODO: Si funciona esto borrar lo anterior...

// se define un tipo de punto 2d auxiliar para que  en el paso de 3d a 2d no haya que conocer el tipo de punto 2d
template <typename T> 
class point_2d_temp
{
public:
  typedef T value_type;
  T x;
  T y;
  point_2d_temp() : x(0), y(0) {}
  point_2d_temp(T _x, T _y) : x(_x), y(_y) {}
};

template<typename T> inline
typename std::iterator_traits<T>::value_type 
findInscribedCircleSequential(T in_first, T in_last, const std::array<typename std::iterator_traits<T>::value_type, 2> bounds, double nCells, double mCells) 
{
  typedef typename std::iterator_traits<T>::value_type Point_t;
  typename std::iterator_traits<T>::difference_type size = std::distance(in_first, in_last);

  Point_t pia((bounds[0].x + bounds[1].x) / 2., (bounds[0].y + bounds[1].y) / 2.);
  Point_t pt_tmp;

  // Incrementos para x e y
	double increment_x = (bounds[1].x - bounds[0].x) / nCells;
	double increment_y = (bounds[1].y - bounds[0].y) / mCells;

  // Ventana envolvente del poligono 
  std::array<Point_t, 2> w_pol;
  w_pol[0].x = w_pol[0].y = std::numeric_limits<typename Point_t::value_type>().max();
  w_pol[1].x = w_pol[1].y = -std::numeric_limits<typename Point_t::value_type>().max();
  T temp = in_first;
  while (temp != in_last) {
    if (w_pol[0].x > temp->x) w_pol[0].x = temp->x;
    if (w_pol[0].y > temp->y) w_pol[0].y = temp->y;
    if (w_pol[1].x < temp->x) w_pol[1].x = temp->x;
    if (w_pol[1].y < temp->y) w_pol[1].y = temp->y;
    *temp++;
  }


  // Maxima distancia
	double max_distance = 0.;

	int i, j;
  double tmp_distance = std::numeric_limits<double>().max();
	for (i = 0; i <= nCells; i++) {

		pt_tmp.x = bounds[0].x + i * increment_x;

		for (j = 0; j <= mCells; j++) {

			pt_tmp.y = bounds[0].y + j * increment_y;

      // Se comprueba si esta dentro de la ventana envolvente
      if ((bounds[1].x >= pt_tmp.x) && (bounds[1].y >= pt_tmp.y) &&
          (bounds[0].x <= pt_tmp.x) && (bounds[0].y <= pt_tmp.y)) {

        bool isInner = false;

        // Se comprueba si el punto es uno de los vertices
        //for (int i = 0; i < mEntities.size(); i++) {
        T temp = in_first;
        while (temp != in_last) {
          // Por ahora se devuelve true. Lo suyo sería indicar que es un vertice.
          if (temp == pt_tmp) isInner = true;
          temp++;
        }

        if (isInner == false) {
            
          Point_t pth1 = pt_tmp;
          Point_t pth2(w_pol[1].x, pt_tmp.y);

          int nIntersection = 0;
          bool bVertex = false;
          std::vector<int> vertex_id;

          temp = in_first;
          Point_t prev = *(in_last -1);
          int i = 0;
          while (temp != in_last) {
            //if (collinearPoints(prev, temp, pt_tmp, 0.005)) {
            //  isInner = true;
            //  break;
            //}
            // El punto es colineal con el segmento y esta dentro del mismo.
            //Implementarlo con iteradores
            //if (distPointToSegment(point, segment) == 0) {
            //  isInner = true;
            //  break;
            //}

            if (pt_tmp.y == temp->y) {
              vertex_id.push_back(i);
              bVertex = true;
            }
            
            if (pt_tmp.y == temp->y && pt_tmp.y == prev.y) {
              // los dos segmentos forman parte de la misma linea horizontal
            } else {
              Point_t ptp;
              nIntersection += intersectSegments(prev, *temp, pth1, pth2, &ptp);
            }
            prev = *temp++;
            i++;
          }

          
          if (bVertex == true) {
            std::vector<int> order;
            int vertex_prev = 0;
            int vertex_next = 0;
            Point_t pt_prev;
            Point_t pt_next;
            for (int i = 0; i < vertex_id.size(); i++) {
              // Se comprueban los puntos anterior y siguiente
              if (vertex_id[i] == 0) {
                pt_prev = *(in_first + size - 1);
                vertex_prev = size - 1;
              } else {
                pt_prev = (in_first + i - 1);
                vertex_prev = vertex_id[i] - 1;
              }

              if (vertex_id[i] == size - 1) {
                pt_next = *(in_first);
                vertex_next = 0;
              } else {
                pt_next = *(in_first + i + 1);
                vertex_next = vertex_id[i] + 1;
              }

              if (pt_prev.y == pth1.y) {
                continue;
              } else {
                if (pt_next.y == pth1.y) {
                  int prev = isLeft(pth1, pth2, pt_prev);
                  if (vertex_next == size - 1) {
                    vertex_next = 0;
                    pt_next = *(in_first);
                  } else {
                    vertex_next = vertex_next + 1;
                    pt_next = *(in_first + vertex_next + 1);
                  }
                  int next = isLeft(pth1, pth2, pt_next);
                  if (prev == next) nIntersection -= 2;
                  else nIntersection--;
                } else {
                  int prev = isLeft(pth1, pth2, pt_prev);
                  int next = isLeft(pth1, pth2, pt_next);
                  if (prev == next) nIntersection -= 2;
                  else nIntersection--;
                }

              }

            }
          }
          if (bVertex) 
            nIntersection--;
          if (nIntersection < 0 || nIntersection % 2 == 0) isInner = false;
          else isInner = true;
        }

        if (isInner) {
          double tmp_distance = I3D_DOUBLE_MAX;
          double dist;
          temp = in_first;
          prev = *(in_last-1);
          while (temp != in_last) {
            msgInfo("%f,%f", temp->x, temp->y);
            dist = distPointToSegment(prev, *temp, pt_tmp);
            if (dist < tmp_distance) tmp_distance = dist;
            prev = *temp++;
          }

          if (tmp_distance > max_distance) {
            max_distance = tmp_distance;
            pia.x = pt_tmp.x;
            pia.y = pt_tmp.y;
          }
        }
      }
      
		}
	}

	return pia;
}


template<typename T> inline
void poleOfInaccessibility2D(T in_first, T in_last, typename std::iterator_traits<T>::value_type *pole, double nCells = 20., double mCells = 20.) 
{
  if (pole == NULL) return;
  typedef typename std::iterator_traits<T>::value_type Point_t;

  //Window<Point_t> w = polygon.getWindow();
  std::array<Point_t, 2> w;
  w[0].x = w[0].y = std::numeric_limits<typename Point_t::value_type>().max();
  w[1].x = w[1].y = -std::numeric_limits<typename Point_t::value_type>().max();
  T temp = in_first;
  while (temp != in_last) {
    if (w[0].x > temp->x) w[0].x = temp->x;
    if (w[0].y > temp->y) w[0].y = temp->y;
    if (w[1].x < temp->x) w[1].x = temp->x;
    if (w[1].y < temp->y) w[1].y = temp->y;
    *temp++;
  }

	Point_t point_tmp;

	int count = 1;
	while (count++) {

		/*if (method == METHOD_SEQUENTIAL) {*/
			point_tmp =	findInscribedCircleSequential(in_first, in_last, w, nCells, mCells);
		/*} else if (method == METHOD_RANDOMIZED) {
			point_tmp = findInscribedCircleRandomized(polygon, w);
		}*/

		pole->x = point_tmp.x;
		pole->y = point_tmp.y;

    Point_t aux;
    aux.x = (w[1].x - w[0].x) / (sqrt(2.) * 2.);
		aux.y = (w[1].y - w[0].y) / (sqrt(2.) * 2.);

    w[0].x = pole->x - aux.x;
    w[0].y = pole->y - aux.y;
    w[1].x = pole->x + aux.x;
    w[1].y = pole->y + aux.y;

		if (w[1].x - w[0].x < 0.01 || w[1].y - w[0].y < 0.01) break;

	}
}



template<typename T> inline
void poleOfInaccessibility3D(T in_first, T in_last, typename std::iterator_traits<T>::value_type *pole, double xCells = 20., double yCells = 20., double zCells = 20.) 
{
  if (pole == NULL) return;

  typedef typename std::iterator_traits<T>::value_type Point_t;
  typename std::iterator_traits<T>::difference_type size = std::distance(in_first, in_last);

  T it = in_first;
  
  std::array<Point_t, 2> box;
  box[0].x = box[0].y = box[0].z = std::numeric_limits<typename Point_t::value_type>().max();
  box[1].x = box[1].y = box[1].z = -std::numeric_limits<typename Point_t::value_type>().max();
  while (it != in_last) {
    if (box[0].x > it->x) box[0].x = it->x;
    if (box[0].y > it->y) box[0].y = it->y;
    if (box[0].z > it->z) box[0].z = it->z;
    if (box[1].x < it->x) box[1].x = it->x;
    if (box[1].y < it->y) box[1].y = it->y;
    if (box[1].z < it->z) box[1].z = it->z;
    *it++;
  }
 

  // Se determina el plano de mejor ajuste
  std::array<double, 4> plane;
  nPointsPlaneLS(in_first, in_last, plane, true);

  // Máxima distancia de los puntos al plano
  double max_dist = I3D_DOUBLE_MIN;

  it = in_first;
  while (it != in_last) {
    Point_t pt;
    double distance = distantePointToPlane(*it, plane);
    if (distance > max_dist) max_dist = distance;
    *it++;
  }

  // Proyectar polilinea en el plano de mejor ajuste
  std::vector<Point_t> poligon_plane;
  it = in_first;
  while (it != in_last) {
    Point_t pt;
    projectPointToPlane(*it, plane, &pt);
    poligon_plane.push_back(pt);
    *it++;
  }

  // Punto central
  Point_t pc((box[0].x + box[1].x) / 2., (box[0].y + box[1].y) / 2., (box[0].z + box[1].z) / 2.);

  // Plano horizontal (1,1,0)
  std::array<double, 4> plane_z;
  plane_z[0] = 0;
  plane_z[1] = 0;
  plane_z[2] = 1;
  plane_z[3] = -plane_z[0] * pc.x - plane_z[1] * pc.y - plane_z[2] * pc.z;

  std::vector<point_2d_temp<typename Point_t::value_type>> poligon_z;

  for (int i = 0; i < poligon_plane.size(); i++){
    Point_t pt;
    projectPointToPlane(poligon_plane[i], plane_z, &pt);
    //msgInfo("%f %f %f", pt.x, pt.y, pt.z);
    poligon_z.push_back(point_2d_temp<typename Point_t::value_type>(pt.x, pt.y));
  }

  typedef typename point_2d_temp<typename Point_t::value_type> point2d_t;

  // Ventana envolvente del poligono 
  std::array<point2d_t, 2> w_pol;
  // Ventana de busqueda
  std::array<Point_t, 2> w;
  w[0].x = w[0].y = std::numeric_limits<typename Point_t::value_type>().max();
  w[1].x = w[1].y = -std::numeric_limits<typename Point_t::value_type>().max();
  auto it2 = poligon_z.begin();
  while (it2 != poligon_z.end()) {
    if (w[0].x > it2->x) w_pol[0].x = w[0].x = it2->x;
    if (w[0].y > it2->y) w_pol[0].y = w[0].y = it2->y;
    if (w[1].x < it2->x) w_pol[1].x = w[1].x = it2->x;
    if (w[1].y < it2->y) w_pol[1].y = w[1].y = it2->y;
    *it2++;
  }

  //Point_t point_tmp;
  point2d_t center2D;

	int count = 1;
	while (count++) {

		//point_tmp =	findInscribedCircleSequential(in_first, in_last, w, nCells, mCells);
    point2d_t pia_2d((w[0].x + w[1].x) / 2., (w[0].y + w[1].y) / 2.);
    point2d_t tmp;

    // Incrementos para x e y
	  double increment_x = (w[1].x - w[0].x) / xCells;
	  double increment_y = (w[1].y - w[0].y) / yCells;

    // Maxima distancia
	  double max_distance = 0.;

	  int i, j;
    double tmp_distance = std::numeric_limits<double>().max();
	  for (i = 0; i <= xCells; i++) {

		  tmp.x = w[0].x + i * increment_x;

		  for (j = 0; j <= yCells; j++) {

			  tmp.y = w[0].y + j * increment_y;

        // Se comprueba si esta dentro de la ventana envolvente
        if ((w[1].x >= tmp.x) && (w[1].y >= tmp.y) &&
            (w[0].x <= tmp.x) && (w[0].y <= tmp.y)) {

          bool isInner = false;

          // Se comprueba si el punto es uno de los vertices
          it2 = poligon_z.begin();
          while (it2 != poligon_z.end()) {
            // Por ahora se devuelve true. Lo suyo sería indicar que es un vertice.
            if (it2->x == tmp.x && it2->y == tmp.y) 
              isInner = true;
            it2++;
          }
          
          if (isInner == false) {

            point2d_t pth1 = tmp;
            point2d_t pth2(w_pol[1].x, tmp.y);

            int nIntersection = 0;
            bool bVertex = false;
            std::vector<int> vertex_id;

            it2 = poligon_z.begin();
            point2d_t prev = *(poligon_z.end() - 1);
            int i = 0;
            while (it2 != poligon_z.end()) {

              if (tmp.y == it2->y) {
                vertex_id.push_back(i);
                bVertex = true;
              }

              if (tmp.y == it2->y && tmp.y == prev.y) {
                // los dos segmentos forman parte de la misma linea horizontal
              } else {
                point2d_t ptp;
                nIntersection += intersectSegments(prev, *it2, pth1, pth2, &ptp);
              }
              prev = *it2++;
              i++;
            }


            if (bVertex == true) {
              std::vector<int> order;
              int vertex_prev = 0;
              int vertex_next = 0;
              point2d_t pt_prev;
              point2d_t pt_next;
              for (int i = 0; i < vertex_id.size(); i++) {
                // Se comprueban los puntos anterior y siguiente
                if (vertex_id[i] == 0) {
                  pt_prev = *(poligon_z.begin() + size - 1);
                  vertex_prev = size - 1;
                } else {
                  pt_prev = *(poligon_z.begin() + vertex_id[i] - 1);
                  vertex_prev = vertex_id[i] - 1;
                }

                if (vertex_id[i] == size - 1) {
                  pt_next = *(poligon_z.begin());
                  vertex_next = 0;
                } else {
                  pt_next = *(poligon_z.begin() + vertex_id[i] + 1);
                  vertex_next = vertex_id[i] + 1;
                }

                if (pt_prev.y == pth1.y) {
                  continue;
                } else {
                  if (pt_next.y == pth1.y) {
                    int prev = isLeft(pth1, pth2, pt_prev);
                    if (vertex_next == size - 1) {
                      vertex_next = 0;
                      pt_next = *(poligon_z.begin());
                    } else {
                      vertex_next = vertex_next + 1;
                      pt_next = *(poligon_z.begin() + vertex_next + 1);
                    }
                    int next = isLeft(pth1, pth2, pt_next);
                    if (prev == next) nIntersection -= 2;
                    else nIntersection--;
                  } else {
                    int prev = isLeft(pth1, pth2, pt_prev);
                    int next = isLeft(pth1, pth2, pt_next);
                    if (prev == next) nIntersection -= 2;
                    else nIntersection--;
                  }

                }

              }
            }
            if (nIntersection < 0 || nIntersection % 2 == 0) isInner = false;
            else isInner = true;
          }

          // Si el punto esta dentro se pasa a 3D y se calcula la máxima distancia
          if (isInner) {

            //Proyectar el punto al revés
            Point_t center3D;
            Point_t _center3D(tmp.x, tmp.y, pc.z);
            projectPointToPlane(_center3D, plane, &center3D, Point3D(plane_z[0], plane_z[1], plane_z[2]));

            // inicio distPointToPolygon
            double tmp_distance = I3D_DOUBLE_MAX;
            double dist;
            it = in_first;
            Point_t prev2 = *(in_last-1);
            while (it != in_last) {
              dist = distPointToSegment3D(prev2, *it, center3D);
              if (dist < tmp_distance) tmp_distance = dist;
              prev2 = *it++;
            }

            if (tmp_distance > max_distance) {
              max_distance = tmp_distance;
              pole->x = center3D.x;
              pole->y = center3D.y;
              pole->z = center3D.z;
              pia_2d.x = tmp.x;
              pia_2d.y = tmp.y;
              
            }
          }

        }
		  }
	  }

    Point_t aux;
    aux.x = (w[1].x - w[0].x) / (sqrt(2.) * 2.);
		aux.y = (w[1].y - w[0].y) / (sqrt(2.) * 2.);

    w[0].x = pia_2d.x - aux.x;
    w[0].y = pia_2d.y - aux.y;
    w[1].x = pia_2d.x + aux.x;
    w[1].y = pia_2d.y + aux.y;

    // TODO: relaccionar este valor con el tamaño de la ventana envolvente
		if (w[1].x - w[0].x < 0.0001 || w[1].y - w[0].y < 0.0001) break;

	}

}


//template<typename T> inline
//typename std::iterator_traits<T>::value_type 
//findInscribedCircleSequential(T in_first, T in_last, const std::array<typename std::iterator_traits<T>::value_type, 2> bounds, double nCells, double mCells) 
//{
//  typedef typename std::iterator_traits<T>::value_type Point_t;
//  typename std::iterator_traits<T>::difference_type size = std::distance(in_first, in_last);
//
//  Point_t pia((bounds[0].x + bounds[1].x) / 2., (bounds[0].y + bounds[1].y) / 2.);
//  Point_t pt_tmp;
//
//  // Incrementos para x e y
//	double increment_x = (bounds[1].x - bounds[0].x) / nCells;
//	double increment_y = (bounds[1].y - bounds[0].y) / mCells;
//
//  // Ventana envolvente del poligono 
//  std::array<Point_t, 2> w_pol;
//  w_pol[0].x = w_pol[0].y = std::numeric_limits<typename Point_t::value_type>().max();
//  w_pol[1].x = w_pol[1].y = -std::numeric_limits<typename Point_t::value_type>().max();
//  T temp = in_first;
//  while (temp != in_last) {
//    if (w_pol[0].x > temp->x) w_pol[0].x = temp->x;
//    if (w_pol[0].y > temp->y) w_pol[0].y = temp->y;
//    if (w_pol[1].x < temp->x) w_pol[1].x = temp->x;
//    if (w_pol[1].y < temp->y) w_pol[1].y = temp->y;
//    *temp++;
//  }
//
//
//  // Maxima distancia
//	double max_distance = 0.;
//
//	int i, j;
//  double tmp_distance = std::numeric_limits<double>().max();
//	for (i = 0; i <= nCells; i++) {
//
//		pt_tmp.x = bounds[0].x + i * increment_x;
//
//		for (j = 0; j <= mCells; j++) {
//
//			pt_tmp.y = bounds[0].y + j * increment_y;
//
//      if ((bounds[1].x >= pt_tmp.x) && (bounds[1].y >= pt_tmp.y) &&
//          (bounds[0].x <= pt_tmp.x) && (bounds[0].y <= pt_tmp.y)) {
//        bool isInner = false;
//        int nIntersection = 0;
//        bool bVertex = false;
//        T temp = in_first;
//        Point_t prev = *(in_last -1);
//        while (temp != in_last) {
//          //if (collinearPoints(prev, temp, pt_tmp, 0.005)) {
//          //  isInner = true;
//          //  break;
//          //}
//          
//          if (pt_tmp.y == temp->y || pt_tmp.y == prev.y)
//            bVertex = true;
//
//          Point_t ptp;
//          Point_t pth1 = pt_tmp;
//          Point_t pth2(w_pol[1].x, pt_tmp.y);
//          nIntersection += intersectSegments(prev, *temp, pth1, pth2, &ptp);
//          prev = *temp++;
//        }
//        if (bVertex) 
//          nIntersection--;
//        if (nIntersection < 0 || nIntersection % 2 == 0) isInner = false;
//        else isInner = true;
//
//        if (isInner) {
//          double tmp_distance = I3D_DOUBLE_MAX;
//          double dist;
//          temp = in_first;
//          prev = *(in_last-1);
//          while (temp != in_last) {
//            //msgInfo("%f,%f", temp->x, temp->y);
//            dist = distPointToSegment(prev, *temp, pt_tmp);
//            if (dist < tmp_distance) tmp_distance = dist;
//            prev = *temp++;
//          }
//
//          if (tmp_distance > max_distance) {
//            max_distance = tmp_distance;
//            pia.x = pt_tmp.x;
//            pia.y = pt_tmp.y;
//          }
//        }
//      }
//      
//      //if (polygon.isInner(tmp)) {
//   //     tmp_distance = distPointToPolygon(tmp, polygon);
//			//	if (tmp_distance > max_distance) {
//			//		max_distance = tmp_distance;
//			//		pia.x = tmp.x;
//			//		pia.y = tmp.y;
//			//	}
//			//}
//		}
//	}
//
//	return pia;
//}
//
//
//template<typename T> inline
//void poleOfInaccessibility2D(T in_first, T in_last, typename std::iterator_traits<T>::value_type *pole, double nCells = 20., double mCells = 20.) 
//{
//  if (pole == NULL) return;
//  typedef typename std::iterator_traits<T>::value_type Point_t;
//
//  //Window<Point_t> w = polygon.getWindow();
//  std::array<Point_t, 2> w;
//  w[0].x = w[0].y = std::numeric_limits<typename Point_t::value_type>().max();
//  w[1].x = w[1].y = -std::numeric_limits<typename Point_t::value_type>().max();
//  T temp = in_first;
//  while (temp != in_last) {
//    if (w[0].x > temp->x) w[0].x = temp->x;
//    if (w[0].y > temp->y) w[0].y = temp->y;
//    if (w[1].x < temp->x) w[1].x = temp->x;
//    if (w[1].y < temp->y) w[1].y = temp->y;
//    *temp++;
//  }
//
//	Point_t point_tmp;
//
//	int count = 1;
//	while (count++) {
//
//		/*if (method == METHOD_SEQUENTIAL) {*/
//			point_tmp =	findInscribedCircleSequential(in_first, in_last, w, nCells, mCells);
//		/*} else if (method == METHOD_RANDOMIZED) {
//			point_tmp = findInscribedCircleRandomized(polygon, w);
//		}*/
//
//		pole->x = point_tmp.x;
//		pole->y = point_tmp.y;
//
//    Point_t aux;
//    aux.x = (w[1].x - w[0].x) / (sqrt(2.) * 2.);
//		aux.y = (w[1].y - w[0].y) / (sqrt(2.) * 2.);
//
//    w[0].x = pole->x - aux.x;
//    w[0].y = pole->y - aux.y;
//    w[1].x = pole->x + aux.x;
//    w[1].y = pole->y + aux.y;
//
//		if (w[1].x - w[0].x < 0.01 || w[1].y - w[0].y < 0.01) break;
//
//	}
//}
//
//
//
//template<typename T> inline
//void poleOfInaccessibility3D(T in_first, T in_last, typename std::iterator_traits<T>::value_type *pole, double xCells = 20., double yCells = 20., double zCells = 20.) 
//{
//  if (pole == NULL) return;
//
//  typedef typename std::iterator_traits<T>::value_type Point_t;
//  T temp = in_first;
//  
//  //TODO: sacar a función
//  std::array<Point_t, 2> box;
//  box[0].x = box[0].y = box[0].z = std::numeric_limits<typename Point_t::value_type>().max();
//  box[1].x = box[1].y = box[1].z = -std::numeric_limits<typename Point_t::value_type>().max();
//  while (temp != in_last) {
//    if (box[0].x > temp->x) box[0].x = temp->x;
//    if (box[0].y > temp->y) box[0].y = temp->y;
//    if (box[0].z > temp->z) box[0].z = temp->z;
//    if (box[1].x < temp->x) box[1].x = temp->x;
//    if (box[1].y < temp->y) box[1].y = temp->y;
//    if (box[1].z < temp->z) box[1].z = temp->z;
//    *temp++;
//  }
// 
//
//  // Se determina el plano de mejor ajuste
//  std::array<double, 4> plane;
//  nPointsPlaneLS(in_first, in_last, plane, true);
//
//  // Máxima distancia de los puntos al plano
//  double max_dist = I3D_DOUBLE_MIN;
//
//  temp = in_first;
//  while (temp != in_last) {
//    Point_t pt;
//    double distance = distantePointToPlane(*temp, plane);
//    if (distance > max_dist) max_dist = distance;
//    *temp++;
//  }
//
//  // Proyectar polilinea en el plano de mejor ajuste
//  std::vector<Point_t> poligon_plane;
//  temp = in_first;
//  while (temp != in_last) {
//    Point_t pt;
//    projectPointToPlane(*temp, plane, &pt);
//    //msgInfo("%f %f %f", pt.x, pt.y, pt.z);
//    poligon_plane.push_back(pt);
//    *temp++;
//  }
//
//  // Punto central
//  Point_t pc((box[0].x + box[1].x) / 2., (box[0].y + box[1].y) / 2., (box[0].z + box[1].z) / 2.);
//
//  // Plano horizontal (1,1,0)
//  std::array<double, 4> plane_z;
//  plane_z[0] = 0;
//  plane_z[1] = 0;
//  plane_z[2] = 1;
//  plane_z[3] = -plane_z[0] * pc.x - plane_z[1] * pc.y - plane_z[2] * pc.z;
//
//  std::vector<point_2d_temp<typename Point_t::value_type>> poligon_z;
//
//  for (int i = 0; i < poligon_plane.size(); i++){
//    Point_t pt;
//    projectPointToPlane(poligon_plane[i], plane_z, &pt);
//    //msgInfo("%f %f %f", pt.x, pt.y, pt.z);
//    poligon_z.push_back(point_2d_temp<typename Point_t::value_type>(pt.x, pt.y));
//  }
//
//  //Ahora el problema se reduce a su solución en 2D
//  point_2d_temp<typename Point_t::value_type> center2D;
//  poleOfInaccessibility2D(poligon_z.begin(), poligon_z.end(), &center2D);
//
//  Point_t _center3D(center2D.x, center2D.y, pc.z);
//
//  //Proyectar el punto al revés
//  projectPointToPlane(_center3D, plane_z, pole, Point3D(plane[0], plane[1], plane[2]));
//
//}


I3D_ENABLE_WARNING(4100)

} // Fin namespacegeometry

/*! \} */ // end of GeometricEntities


} // End namespace I3D

#endif // I3D_OPERATIONS_H