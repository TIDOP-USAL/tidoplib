#ifndef I3D_SEGMENT_H
#define I3D_SEGMENT_H

#include "core/config.h"

#ifdef HAVE_OPENCV
#include "opencv2/core/core.hpp"
#endif

#include "core/defs.h"
#include "core/utils.h"
#include "core/mathutils.h"
#include "geometric_entities/point.h"
#include "geometric_entities/entity.h"
#include "geometric_entities/window.h"
#include "geometric_entities/operations.h"

namespace I3D
{

// forward declaration
template<typename T> class Window;

/*! \defgroup GeometricEntities Entidades geométricas
 *  Puntos, lineas, ...
 *  \{
 */

/* ---------------------------------------------------------------------------------- */

/*!
 * \brief Clase segmento 2D
 *
 * Esta template representa un segmento recto entre dos puntos independientemente de
 * que sean int, double o float. Se utiliza la clase point_ de OpenCV.
 *
 * Se han definido los siguientes alias para facilitar el acceso:
 * \code
 * typedef Segment<int> SegmentI;
 * typedef Segment<double> SegmentD;
 * typedef Segment<float> SegmentF;
 * typedef SegmentI Line;
 * \endcode
 */
template<typename T>
class Segment : public Entity<T>
{
public:

  /*!
   * \brief type
   */
  typedef T value_type;

  /*!
   * \brief Punto 1
   */
  Point<T> pt1;

  /*!
   * \brief Punto 2
   */
  Point<T> pt2;

public:

  /*!
   * \brief Constructora por defecto
   */
  Segment();

  /*!
   * \brief Constructor de copia
   */
  Segment(const Segment &segment);

  /*!
   * \brief Constructor segment
   * \param[in] _pt1
   * \param[in] _pt2
   */
  Segment(const Point<T> &_pt1, const Point<T> &_pt2);

#ifdef HAVE_OPENCV

  /*!
   * \brief Constructor segment
   * \param[in] lvect
   */
  Segment(const cv::Vec<T, 4> &lvect);

#endif

  /*!
   * \brief Constructor segment
   * \param[in] pt Coordenadas del punto central o inicial (según el valor de bCenter)
   * \param[in] angle Ángulo de la recta
   * \param[in] length Longitud del segmento
   * \param[in] bCenter Si es verdadero es el centro de la línea
   */
  Segment(const Point<T> &pt, double angle, double length, bool bCenter = true);

  /*!
   * \brief Sobrecarga del operador de asignación
   * \param[in] segment Segmento que se asigna
   * \return Referencia al segmento
   */
  Segment &operator = (const Segment &segment);

  /*!
   * \brief Conversión a un segmento de un tipo diferente
   */
  template<typename T2> operator Segment<T2>() const;

  /*!
   * \brief Angulo medido respecto al eje x.
   * \return Ángulo en radianes
   */
  double angleOX() const;

  /*!
   * \brief Angulo medido respecto al eje y.
   * \return Ángulo en radianes
   */
  double angleOY() const;

  /*!
   * \brief Ventana envolvente
   * \return Ventana envolvente del segmento
   */
  Window<T> getWindow() const;

  /*!
   * \brief Comprueba si dos segmentos están próximos
   * \param[in] l2 Segmento con el que se compara
   * \param[in] dist Distancia máxima de separación
   * \return Verdadero si esta a menos distancia de la indicada.
   */
  bool isNear(const Segment<T> &l2, double dist = 10.) const;

  /*!
   * \brief Comprueba si es paralelo a otro segmento
   * \param[in] l2 Segmento con el que se compara
   * \param[in] tol Toleracia angular. Si es 0 los segmentos tienen que ser paralelos.
   *  Si tiene otro valor se aceptan lineas que esten por debajo de esa tolerancia
   * \return Verdadero si es paralela (± tol).
   */
  bool isParallel(const Segment<T> &l2, double tol = 0.) const;

  /*!
   * \brief Longitud del segmento
   * \return Longitud del segmento
   */
  double length() const { return distance(pt1, pt2); }

  /*!
   * \brief Vector
   * \return Vector del segmento
   */
  Point<T> vector() const { return (pt2 - pt1); }
};

// Definición de métodos

template<typename T> inline
Segment<T>::Segment()
: Entity<T>(entity_type::SEGMENT_2D), pt1(0, 0), pt2(0, 0) {}

template<typename T> inline
Segment<T>::Segment(const Segment &Segment)
: Entity<T>(entity_type::SEGMENT_2D), pt1(Segment.pt1), pt2(Segment.pt2) {}

template<typename T> inline
Segment<T>::Segment(const Point<T> &_pt1, const Point<T> &_pt2)
: Entity<T>(entity_type::SEGMENT_2D), pt1(_pt1), pt2(_pt2) {}

#ifdef HAVE_OPENCV

template<typename T> inline
Segment<T>::Segment(const cv::Vec<T, 4> &lvect)
: Entity<T>(entity_type::SEGMENT_2D)
{
  pt1.x = lvect[0];
  pt1.y = lvect[1];
  pt2.x = lvect[2];
  pt2.y = lvect[3];
}

#endif

template<typename T> inline
Segment<T>::Segment(const Point<T> &pt, double angle, double length, bool bCenter)
: Entity<T>(entity_type::SEGMENT_2D)
{
  double a = cos(angle), b = sin(angle);
  double l1 = 0;
  double l2 = length;
  if (bCenter) {
    l1 = l2 = length / 2;
  }
  pt1.x = I3D_ROUND_TO_INT(pt.x - l1 * -b);
  pt1.y = I3D_ROUND_TO_INT(pt.y - l1 * a);
  pt2.x = I3D_ROUND_TO_INT(pt.x + l2 * -b);
  pt2.y = I3D_ROUND_TO_INT(pt.y + l2 * a);
}


template<typename T> inline
Segment<T> &Segment<T>::operator = (const Segment &segment)
{
  if (this != &segment) {
    this->pt1 = segment.pt1;
    this->pt2 = segment.pt2;
    this->mEntityType = segment.mEntityType;
  }
  return *this;
}

template<typename T> template<typename T2> inline
Segment<T>::operator Segment<T2>() const
{
  Point<T2> _pt1 = pt1;
  Point<T2> _pt2 = pt2;
  return Segment<T2>(_pt1, _pt2);
}

template<typename T> inline
double Segment<T>::angleOX() const
{
  double angle = 0.0;
  if (pt1 != pt2) {
    angle = vectorAngleOX(vector());
  }
  return angle;

}

template<typename T> inline
double Segment<T>::angleOY() const
{
  double angle = 0.0;
  if (pt1 != pt2) {
    angle = vectorAngleOY(vector());
  }
  return angle;
}

template<typename T> inline
Window<T> Segment<T>::getWindow() const
{
  return Window<T>(pt1, pt2);
}

template<typename T> inline
bool Segment<T>::isNear(const Segment<T> &l2, double dist) const
{
  double dist1 = minDistanceSegments(*this, l2);
  return (dist1 <= dist);
}

template<typename T> inline
bool Segment<T>::isParallel(const Segment<T> &l2, double tol) const
{
  return (abs(angleOX() - l2.angleOX()) < tol);
}

typedef Segment<int> SegmentI;
typedef Segment<double> SegmentD;
typedef Segment<float> SegmentF;
typedef SegmentI Line;

/* ---------------------------------------------------------------------------------- */

#ifdef HAVE_OPENCV

/*!
 * \brief Clase segmento 3D
 *
 * Esta template representa un segmento recto entre dos puntos independientemente de
 * que sean int, double o float. Se utiliza la clase Point3_ de OpenCV.
 *
 * Se han definido los siguientes alias para facilitar el acceso:
 *
 * typedef Segment3D<int> Segment3dI;
 * typedef Segment3D<double> Segment3dD;
 * typedef Segment3D<float> Segment3dF;
 *
 */
template<typename T>
class Segment3D : public Entity<T>
{
public:

  typedef T type;

  /*!
   * \brief Punto 1
   */
  Point3<T> pt1;

  /*!
   * \brief Punto 2
   */
  Point3<T> pt2;

public:

  /*!
   * \brief Constructor por defecto
   */
  Segment3D();

  /*!
   * \brief Constructor de copia
   * \param[in] segment Segmento que se asigna
   */
  Segment3D(const Segment3D &segment);

  /*!
   * \brief Constructor segment
   * \param[in] _pt1 Punto 1
   * \param[in] _pt2 Punto 2
   */
  Segment3D(const Point3<T> &_pt1, const Point3<T> &_pt2);

  /*!
   * \brief Sobrecarga del operador de asignación
   * \param[in] segment Segmento que se asigna
   * \return Referencia a la ventana
   */
  Segment3D &operator = (const Segment3D &segment);

  /*!
   * \brief Conversión a un segmento de un tipo diferente
   */
  template<typename T2> operator Segment3D<T2>() const;


  //  Window<T> getWindow() const;


  //  bool isNear(const Segment<T> &l2, double dist = 10) const;

  /*!
   * \brief Longitud del segmento
   * \return Longitud del segmento
   */
  double length() const { return distance(pt1, pt2); }

  /*!
   * \brief Vector
   * \return Vector del segmento
   */
  Point3<T> vector() const { return (pt2 - pt1); }
};

// Definición de métodos

template<typename T> inline
Segment3D<T>::Segment3D()
: Entity<T>(entity_type::SEGMENT_3D), pt1(Point3<T>()), pt2(Point3<T>()) {}

template<typename T> inline
Segment3D<T>::Segment3D(const Segment3D &Segment)
: Entity<T>(entity_type::SEGMENT_3D), pt1(Segment.pt1), pt2(Segment.pt2) {}

template<typename T> inline
Segment3D<T>::Segment3D(const Point3<T> &_pt1, const Point3<T> &_pt2)
: Entity<T>(entity_type::SEGMENT_3D), pt1(_pt1), pt2(_pt2) {}

template<typename T> inline
Segment3D<T> &Segment3D<T>::operator = (const Segment3D &segment)
{
  if (this != &segment) {
    this->pt1 = segment.pt1;
    this->pt2 = segment.pt2;
    this->mEntityType = segment.mEntityType;
  }
  return *this;
}

template<typename T> template<typename T2> inline
Segment3D<T>::operator Segment3D<T2>() const
{
  Point3<T2> _pt1 = pt1;
  Point3<T2> _pt2 = pt2;
  return Segment3D<T2>(_pt1, _pt2);
}

//template<typename T> inline
//Window<T> Segment<T>::getWindow() const
//{
//  return Window<T>(pt1,pt2);
//}

//template<typename T> inline
//bool Segment<T>::isNear(const Segment<T> &l2, double dist) const
//{
//  double dist1 = minDistanceSegments(*this, l2);
//  return (dist1 < dist);
//}

typedef Segment3D<int> Segment3dI;
typedef Segment3D<double> Segment3dD;
typedef Segment3D<float> Segment3dF;

#endif

/* ---------------------------------------------------------------------------------- */
/*                               Operaciones con Líneas                               */
/* ---------------------------------------------------------------------------------- */

//TODO: creo que mejor mover a operations

/*!
 * \brief Crea un buffer entorno a una linea
 * \param[in] ln Línea
 * \param[in] size Tamaño de buffer
 * \param[out] buff Buffer
 */
I3D_EXPORT void lineBuffer(const Line &ln, int size, std::vector<PointI> *buff);

/*!
 * \brief Projecta un punto en un segmento de recta.
 * Si no hay punto de proyección en el segmento se devuelve nulo
 * \param[in] ln Segmento de línea
 * \param[in] pt Punto que se proyecta
 * \param[out] ptp Punto proyectado
 * \return -1, 0, 1
 */
//I3D_EXPORT int projectPointInSegment(const Line &ln, const PointI &pt, PointI *ptp);

template<typename T> inline
int projectPointInSegment(const Segment<T> &ln, const Point<T> &pt, Point<T> *ptp)
{
  int iret = 0;
  if (pt == ln.pt1 || pt == ln.pt2) {
    *ptp = pt;
    return 2;
  }
  PointD v1 = pt - ln.pt1;
  PointD v2 = ln.vector();
  double daux = v1.ddot(v2);
  double r = daux / (v2.x * v2.x + v2.y * v2.y);

  //if (typeid(T) == typeid(int)) {
  //  ptp->x = ln.pt1.x + I3D_ROUND_TO_INT((ln.pt2.x - ln.pt1.x) * r);
  //  ptp->y = ln.pt1.y + I3D_ROUND_TO_INT((ln.pt2.y - ln.pt1.y) * r);
  //} else {
  //  ptp->x = ln.pt1.x + (ln.pt2.x - ln.pt1.x) * r;
  //  ptp->y = ln.pt1.y + (ln.pt2.y - ln.pt1.y) * r;
  //}
  if (typeid(T) == typeid(int)) {
    ptp->x = ln.pt1.x + I3D_ROUND_TO_INT(v2.x * r);
    ptp->y = ln.pt1.y + I3D_ROUND_TO_INT(v2.y * r);
  } else {
    ptp->x = ln.pt1.x + static_cast<T>(v2.x * r);
    ptp->y = ln.pt1.y + static_cast<T>(v2.x * r);
  }

  if (daux <= 0) iret = -1;
  else if (daux >= (v2.x * v2.x + v2.y * v2.y)) iret = 1;
  else if (daux == 0) iret = 2; // Esta en la línea
  return iret;
}


template<typename T> inline
int projectPointInSegment(const Segment3D<T> &ln, const Point3<T> &pt, Point3<T> *ptp)
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

  //if (typeid(T) == typeid(int)) {
  //  ptp->x = ln.pt1.x + I3D_ROUND_TO_INT((ln.pt2.x - ln.pt1.x) * r);
  //  ptp->y = ln.pt1.y + I3D_ROUND_TO_INT((ln.pt2.y - ln.pt1.y) * r);
  //} else {
  //  ptp->x = ln.pt1.x + (ln.pt2.x - ln.pt1.x) * r;
  //  ptp->y = ln.pt1.y + (ln.pt2.y - ln.pt1.y) * r;
  //}
  //if (typeid(T) == typeid(int)) {
  //  ptp->x = ln.pt1.x + I3D_ROUND_TO_INT(v2.x * r);
  //  ptp->y = ln.pt1.y + I3D_ROUND_TO_INT(v2.y * r);
  //} else {
  *ptp = ln.pt1 + v2 * r;
  //}

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
//I3D_EXPORT double distPointToSegment(const PointI &pt, const Line &ln);

template<typename T> inline
double distPointToSegment(const Point<T> &pt, const Segment<T> &ln)
{
  Point<T> ptp;
  int ipr = projectPointInSegment(ln, pt, &ptp);

  if (ipr == -1) ptp = ln.pt1;
  else if (ipr == 1) ptp = ln.pt2;
  Point<T> _pt(pt);
  return distance(_pt, ptp);
}

template<typename T> inline
double distPointToSegment(const Point3<T> &pt, const Segment3D<T> &ln)
{
  Point3<T> ptp;
  int ipr = projectPointInSegment(ln, pt, &ptp);

  if (ipr == -1) ptp = ln.pt1;
  else if (ipr == 1) ptp = ln.pt2;
  Point3<T> _pt(pt);
  return distance(_pt, ptp);
}

/*!
 * \brief distPointToLine
 * \param[in] pt
 * \param[in] ln
 * \return
 */
I3D_EXPORT double distPointToLine(const PointI &pt, const Line &ln);

/*!
 * \brief Calcula la distancia mínima entre dos segmentos de linea.
 * \param[in] ln1 Segmento 1
 * \param[in] ln2 Segmento 2
 * \return Distancia entre segmentos
 */
I3D_EXPORT double minDistanceSegments(const Line &ln1, const Line &ln2);

I3D_DISABLE_WARNING(4244)

/*!
 * \brief Intersect de dos segmentos de línea
 * \param ln1 Primer segmento
 * \param ln2 Segundo segmento
 * \param pt Punto de intersección
 * \return
 */
 //I3D_EXPORT int intersectSegments(const Line &ln1, const Line &ln2, PointI *pt);
 template <typename T>
inline int intersectSegments(const Segment<T> &ln1, const Segment<T> &ln2, Point<T> *pt)
{
  int iret = 0;
  Point<T> vs1, vs2;
  vs1 = ln1.vector();
  vs2 = ln2.vector();
  // si el producto vectorial de los vectores que unen ambos segmentos es 0 son paralelas
  if (double cross_product = vs1.cross(vs2)) {
    Point<T> v11_12 = ln2.pt1 - ln1.pt1;
    double t = v11_12.cross(vs2) / cross_product;
    double u = v11_12.cross(vs1) / cross_product;
    if (t >= 0.  &&  t <= 1 && u >= 0.  &&  u <= 1) {
      if (typeid(T) == typeid(int)) {
        pt->x = I3D_ROUND_TO_INT(ln1.pt1.x + t * vs1.x);
        pt->y = I3D_ROUND_TO_INT(ln1.pt1.y + t * vs1.y);
      } else {
        pt->x += static_cast<T>(ln1.pt1.x + t * vs1.x);
        pt->y += static_cast<T>(ln1.pt1.y + t * vs1.y);
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
template <typename T>
inline int intersectLines(const Segment<T> &ln1, const Segment<T> &ln2, Point<T> *pt)
{
  int iret = 0;
  Point<T> vs1, vs2;
  vs1 = ln1.vector();
  vs2 = ln2.vector();
  // si el producto vectorial de los vectores que unen ambos segmentos es 0 son paralelas
  if (double cross_product = vs1.cross(vs2)) {
    Point<T> v11_12 = ln2.pt1 - ln1.pt1;
    double t = v11_12.cross(vs2) / cross_product;
    if (typeid(T) == typeid(int)) {
      pt->x = I3D_ROUND_TO_INT(ln1.pt1.x + t * vs1.x);
      pt->y = I3D_ROUND_TO_INT(ln1.pt1.y + t * vs1.y);
    } else {
      pt->x = static_cast<T>(ln1.pt1.x + t * vs1.x);
      pt->y = static_cast<T>(ln1.pt1.y + t * vs1.y);
    }
    iret = 1;
  }
  return(iret);
}

I3D_ENABLE_WARNING(4244)

/*!
 * \brief joinLinesByDist
 * \param[in] linesIn
 * \param[out] linesOut
 * \param[in] dist
 */
 I3D_EXPORT void joinLinesByDist(const std::vector<Line> &linesIn, std::vector<Line> *linesOut, int dist);


/* ---------------------------------------------------------------------------------- */
/*                                 Grupos de Líneas                                   */
/* ---------------------------------------------------------------------------------- */


/*!
 * \brief The ldGroupLines class
 */
class I3D_EXPORT ldGroupLines
{
private:
  /*!
   * \brief linesgrops
   */
  std::vector<Line> linesgroup;

  ///*!
  // * \brief ángulo máximo del grupo de líneas
  // */
  //double anglemax;

  ///*!
  // * \brief ángulo mínimo del grupo de líneas
  // */
  //double anglemin;

  /*!
   * \brief Ventana envolvente del grupo de líneas
   */
  WindowI bbox;

public:
  /*!
   * \brief Constructora GroupLines
   */
  ldGroupLines();

  ldGroupLines(const std::vector<Line> &lines);

  /*!
   * \brief Destructora GroupLines
   */
  //  ~ldGroupLines();

  /*!
   * \brief Añade una línea
   * \param[in] Line Linea
   */
  void add(const Line &line);

#ifdef HAVE_OPENCV

  /*!
   * \brief Añade una línea
   * \param[in] lvect
   */
  void add(const cv::Vec4i &lvect);

#endif

  ///*!
  // * \brief Ángulo máximo del grupo de líneas
  // * \return
  // */
  //double AngleMax() {return anglemax; };

  ///*!
  // * \brief Ángulo mínimo del grupo de líneas
  // * \return
  // */
  //double AngleMin() { return anglemin; };

  /*!
   * \brief Ángulo medio
   * \return
   */
  double angleMean();

  void DeleteLine(int id);

  /*!
   * \brief Ventana envolvente del grupo de lineas
   * \return
   */
  WindowI getBbox() const { return bbox; }

  /*!
   * \brief Número de líneas
   * \return
   */
  int getSize() const { return static_cast<int>(linesgroup.size()); }

  /*!
   * \brief Operador de indexación sobrecargado
   * \param[in] id Indice del elemento
   * \return Linea
   */
  const Line &operator[](int id) const { return linesgroup[id]; }

  const std::vector<Line> &getLines() const { return linesgroup; }

};

/* ---------------------------------------------------------------------------------- */
/*                          Operaciones con grupos de Líneas                          */
/* ---------------------------------------------------------------------------------- */

I3D_EXPORT void groupParallelLines(std::vector<Line> linesaux, std::vector<ldGroupLines> *curLinesGrops, double angTol);

I3D_EXPORT void groupLinesByDist(const std::vector<Line> &linesIn, std::vector<ldGroupLines> *curLinesGrops, int dist);

I3D_EXPORT void delLinesGroupBySize(std::vector<ldGroupLines> *vlg, int size);















namespace geometry {

/* ---------------------------------------------------------------------------------- */

/*!
 * \brief Clase segmento 2D
 *
 * Esta template representa un segmento recto entre dos puntos independientemente de
 * que sean int, double o float. Se utiliza la clase point_ de OpenCV.
 *
 * Se han definido los siguientes alias para facilitar el acceso:
 * \code
 * typedef Segment<int> SegmentI;
 * typedef Segment<double> SegmentD;
 * typedef Segment<float> SegmentF;
 * typedef SegmentI Line;
 * \endcode
 */
template<typename Point_t>
class Segment : public Entity
{

public:

  /*!
   * \brief type Tipo de punto
   */
  typedef Point_t value_type;

  /*!
   * \brief Punto inicial del segmento
   */
  Point_t pt1;

  /*!
   * \brief Punto final del segmento
   */
  Point_t pt2;

public:

  /*!
   * \brief Constructora por defecto
   */
  Segment();

  /*!
   * \brief Constructor de copia
   */
  Segment(const Segment &segment);

  /*!
   * \brief Constructor segment
   * \param[in] _pt1 Punto inicial del segmento
   * \param[in] _pt2 Punto final del segmento
   */
  Segment(const Point_t &_pt1, const Point_t &_pt2);

//#ifdef HAVE_OPENCV
//
//  /*!
//   * \brief Constructor segment
//   * \param[in] lvect
//   */
//  Segment(const cv::Vec<T, 4> &lvect);
//
//#endif

  /*!
   * \brief Constructor segment
   * \param[in] pt Coordenadas del punto central o inicial (según el valor de bCenter)
   * \param[in] angle Ángulo de la recta
   * \param[in] length Longitud del segmento
   * \param[in] bCenter Si es verdadero es el centro de la línea
   */
  Segment(const Point_t &pt, double angle, double length, bool bCenter = true);

  /*!
   * \brief Sobrecarga del operador de asignación
   * \param[in] segment Segmento que se asigna
   * \return Referencia al segmento
   */
  Segment &operator = (const Segment &segment);

  /*!
   * \brief Conversión a un segmento de un tipo diferente
   */
  template<typename Point_t2> operator Segment<Point_t2>() const;

  /*!
   * \brief Angulo medido respecto al eje x.
   * \return Ángulo en radianes
   */
  double angleOX() const;

  /*!
   * \brief Angulo medido respecto al eje y.
   * \return Ángulo en radianes
   */
  double angleOY() const;

  /*!
   * \brief Ventana envolvente
   * \return Ventana envolvente del segmento
   */
  Window<Point_t> getWindow() const;

  /*!
   * \brief Comprueba si el segmento esta vacio pt1 = (0, 0) ; pt2 = (0, 0)
   * \return Verdadero si esta vacio
   */
  bool isEmpty() const;

  /*!
   * \brief Comprueba si dos segmentos están próximos
   * \param[in] l2 Segmento con el que se compara
   * \param[in] dist Distancia máxima de separación
   * \return Verdadero si esta a menos distancia de la indicada.
   */
  bool isNear(const Segment<Point_t> &l2, double dist = 10.) const;

  /*!
   * \brief Comprueba si es paralelo a otro segmento
   * \param[in] l2 Segmento con el que se compara
   * \param[in] tol Toleracia angular. Si es 0 los segmentos tienen que ser paralelos.
   *  Si tiene otro valor se aceptan lineas que esten por debajo de esa tolerancia
   * \return Verdadero si es paralela (± tol).
   */
  bool isParallel(const Segment<Point_t> &l2, double tol = 0.) const;

  /*!
   * \brief Longitud del segmento
   * \return Longitud del segmento
   */
  double length() const;

  /*!
   * \brief Vector
   * \return Vector del segmento
   */
  Point_t vector() const;
};

// Definición de métodos

template<typename Point_t> inline
Segment<Point_t>::Segment()
  : Entity(Entity::type::SEGMENT_2D), 
    pt1(), 
    pt2()
{
}

template<typename Point_t> inline
Segment<Point_t>::Segment(const Segment &segment)
  : Entity(Entity::type::SEGMENT_2D), 
    pt1(segment.pt1), 
    pt2(segment.pt2) 
{
}

template<typename Point_t> inline
Segment<Point_t>::Segment(const Point_t &_pt1, const Point_t &_pt2)
  : Entity(Entity::type::SEGMENT_2D), 
    pt1(_pt1), 
    pt2(_pt2)
{
}

//#ifdef HAVE_OPENCV
//
//template<typename T> inline
//Segment<T>::Segment(const cv::Vec<T, 4> &lvect)
//: Entity(Entity::type::SEGMENT_2D)
//{
//  pt1.x = lvect[0];
//  pt1.y = lvect[1];
//  pt2.x = lvect[2];
//  pt2.y = lvect[3];
//}
//
//#endif

template<typename Point_t> inline
Segment<Point_t>::Segment(const Point_t &pt, double angle, double length, bool bCenter)
  : Entity(Entity::type::SEGMENT_2D)
{
  double a = cos(angle), b = sin(angle);
  double l1 = 0;
  double l2 = length;
  if (bCenter) {
    l1 = l2 = length / 2;
  }
  if (typeid(typename Point_t::value_type) == typeid(int)) {
    pt1.x = I3D_ROUND_TO_INT(pt.x - l1 * -b);
    pt1.y = I3D_ROUND_TO_INT(pt.y - l1 * a);
    pt2.x = I3D_ROUND_TO_INT(pt.x + l2 * -b);
    pt2.y = I3D_ROUND_TO_INT(pt.y + l2 * a);
  } else {
    pt1.x = static_cast<typename Point_t::value_type>(pt.x - l1 * -b);
    pt1.y = static_cast<typename Point_t::value_type>(pt.y - l1 * a);
    pt2.x = static_cast<typename Point_t::value_type>(pt.x + l2 * -b);
    pt2.y = static_cast<typename Point_t::value_type>(pt.y + l2 * a);
  }
}


template<typename Point_t> inline
Segment<Point_t> &Segment<Point_t>::operator = (const Segment &segment)
{
  if (this != &segment) {
    this->pt1 = segment.pt1;
    this->pt2 = segment.pt2;
    this->mEntityType = segment.mEntityType;
  }
  return *this;
}

template<typename Point_t> template<typename Point_t2> inline
Segment<Point_t>::operator Segment<Point_t2>() const
{
  Segment<Point_t2> s;
  if (typeid(typename Point_t2::value_type) == typeid(int)) {
    s.pt1.x = I3D_ROUND_TO_INT(pt1.x);
    s.pt1.y = I3D_ROUND_TO_INT(pt1.y);
    s.pt2.x = I3D_ROUND_TO_INT(pt2.x);
    s.pt2.y = I3D_ROUND_TO_INT(pt2.y);
  } else {
    s.pt1.x = static_cast<typename Point_t2::value_type>(pt1.x);
    s.pt1.y = static_cast<typename Point_t2::value_type>(pt1.y);
    s.pt2.x = static_cast<typename Point_t2::value_type>(pt2.x);
    s.pt2.y = static_cast<typename Point_t2::value_type>(pt1.y);
  }
  return s;
}

template<typename Point_t> inline
double Segment<Point_t>::angleOX() const
{
  double angle = 0.0;
  if (pt1 != pt2) {
    angle = vectorAngleOX(vector());
  }
  return angle;

}

template<typename Point_t> inline
double Segment<Point_t>::angleOY() const
{
  double angle = 0.0;
  if (pt1 != pt2) {
    angle = vectorAngleOY(vector());
  }
  return angle;
}

template<typename Point_t> inline
Window<Point_t> Segment<Point_t>::getWindow() const
{
  return Window<Point_t>(pt1, pt2);
}

template<typename Point_t> inline
bool Segment<Point_t>::isEmpty() const
{
  return (   pt1.x == static_cast<typename Point_t::value_type>(0)
          && pt1.y == static_cast<typename Point_t::value_type>(0)
          && pt2.x == static_cast<typename Point_t::value_type>(0) 
          && pt2.y == static_cast<typename Point_t::value_type>(0)); 
}

template<typename Point_t> inline
bool Segment<Point_t>::isNear(const Segment<Point_t> &l2, double dist) const
{
  double dist1 = minDistanceSegments(*this, l2);
  return (dist1 <= dist);
}

template<typename Point_t> inline
bool Segment<Point_t>::isParallel(const Segment<Point_t> &l2, double tol) const
{
  return (abs(angleOX() - l2.angleOX()) < tol);
}

template<typename Point_t> inline
double Segment<Point_t>::length() const 
{ 
  return distance(pt1, pt2); 
}

template<typename Point_t> inline
Point_t Segment<Point_t>::vector() const 
{ 
  return (pt2 - pt1); 
}

typedef Segment<Point<int>> SegmentI;
typedef Segment<Point<double>> SegmentD;
typedef Segment<Point<float>> SegmentF;
typedef SegmentI Line;




/*!
 * \brief Clase segmento 3D
 *
 * Esta template representa un segmento recto entre dos puntos independientemente de
 * que sean int, double o float. Se utiliza la clase Point3_ de OpenCV.
 *
 * Se han definido los siguientes alias para facilitar el acceso:
 *
 * typedef Segment3D<int> Segment3dI;
 * typedef Segment3D<double> Segment3dD;
 * typedef Segment3D<float> Segment3dF;
 *
 */
template<typename Point3_t>
class Segment3D : public Entity
{
public:

  typedef Point3_t type;

  /*!
   * \brief Punto 1
   */
  Point3_t pt1;

  /*!
   * \brief Punto 2
   */
  Point3_t pt2;

public:

  /*!
   * \brief Constructor por defecto
   */
  Segment3D();

  /*!
   * \brief Constructor de copia
   * \param[in] segment Segmento que se asigna
   */
  Segment3D(const Segment3D &segment);

  /*!
   * \brief Constructor segment
   * \param[in] _pt1 Punto 1
   * \param[in] _pt2 Punto 2
   */
  Segment3D(const Point3_t &_pt1, const Point3_t &_pt2);

  /*!
   * \brief Sobrecarga del operador de asignación
   * \param[in] segment Segmento que se asigna
   * \return Referencia a la ventana
   */
  Segment3D &operator = (const Segment3D &segment);

  /*!
   * \brief Conversión a un segmento de un tipo diferente
   */
  template<typename Point3_t2> operator Segment3D<Point3_t2>() const;

  /*!
   * \brief Caja envolvente del segmento
   */
  Box<Point3_t> getBox() const;

  /*!
   * \brief Comprueba si el segmento esta vacio pt1 = (0, 0, 0) ; pt2 = (0, 0, 0)
   * \return Verdadero si esta vacio
   */
  bool isEmpty() const;

  //  bool isNear(const Segment<T> &l2, double dist = 10) const;

  /*!
   * \brief Longitud del segmento
   * \return Longitud del segmento
   */
  double length() const;

  /*!
   * \brief Vector
   * \return Vector del segmento
   */
  Point3_t vector() const;
};

// Definición de métodos

template<typename Point3_t> inline
Segment3D<Point3_t>::Segment3D()
  : Entity(Entity::type::SEGMENT_3D), 
    pt1(Point3_t()), 
    pt2(Point3_t()) 
{
}

template<typename T> inline
Segment3D<T>::Segment3D(const Segment3D &Segment)
: Entity(Entity::type::SEGMENT_3D), 
  pt1(Segment.pt1), 
  pt2(Segment.pt2) 
{
}

template<typename Point3_t> inline
Segment3D<Point3_t>::Segment3D(const Point3_t &_pt1, const Point3_t &_pt2)
  : Entity(Entity::type::SEGMENT_3D), 
    pt1(_pt1), 
    pt2(_pt2) 
{
}

template<typename Point3_t> inline
Segment3D<Point3_t> &Segment3D<Point3_t>::operator = (const Segment3D &segment)
{
  if (this != &segment) {
    this->pt1 = segment.pt1;
    this->pt2 = segment.pt2;
    this->mEntityType = segment.mEntityType;
  }
  return *this;
}

template<typename Point3_t> template<typename Point3_t2> inline
Segment3D<Point3_t>::operator Segment3D<Point3_t2>() const
{
  //Point3_t2 _pt1 = pt1;
  //Point3_t2 _pt2 = pt2;
  //return Segment3D<Point3_t2>(_pt1, _pt2);
  Segment3D<Point3_t2> s;
  if (typeid(typename Point3_t2::value_type) == typeid(int)) {
    s.pt1.x = I3D_ROUND_TO_INT(pt1.x);
    s.pt1.y = I3D_ROUND_TO_INT(pt1.y);
    s.pt1.z = I3D_ROUND_TO_INT(pt1.z);
    s.pt2.x = I3D_ROUND_TO_INT(pt2.x);
    s.pt2.y = I3D_ROUND_TO_INT(pt2.y);
    s.pt2.z = I3D_ROUND_TO_INT(pt2.z);
  } else {
    s.pt1.x = static_cast<typename Point3_t2::value_type>(pt1.x);
    s.pt1.y = static_cast<typename Point3_t2::value_type>(pt1.y);
    s.pt1.z = static_cast<typename Point3_t2::value_type>(pt1.z);
    s.pt2.x = static_cast<typename Point3_t2::value_type>(pt2.x);
    s.pt2.y = static_cast<typename Point3_t2::value_type>(pt1.y);
    s.pt2.z = static_cast<typename Point3_t2::value_type>(pt1.z);
  }
  return s;
}

template<typename Point3_t> inline
Box<Point3_t> Segment3D<Point3_t>::getBox() const
{
  return Box<Point3_t>(pt1, pt2);
}

template<typename Point3_t> inline
bool Segment3D<Point3_t>::isEmpty() const
{
  return (   pt1.x == static_cast<typename Point3_t::value_type>(0)
          && pt1.y == static_cast<typename Point3_t::value_type>(0)
          && pt2.x == static_cast<typename Point3_t::value_type>(0) 
          && pt2.y == static_cast<typename Point3_t::value_type>(0)); 
}

//template<typename T> inline
//bool Segment<T>::isNear(const Segment<T> &l2, double dist) const
//{
//  double dist1 = minDistanceSegments(*this, l2);
//  return (dist1 < dist);
//}

template<typename Point3_t> inline
double Segment3D<Point3_t>::length() const 
{ 
  return distance(pt1, pt2); 
}

template<typename Point3_t> inline
Point3_t Segment3D<Point3_t>::vector() const 
{ 
  return (pt2 - pt1); 
}

typedef Segment3D<Point3<int>> Segment3dI;
typedef Segment3D<Point3<double>> Segment3dD;
typedef Segment3D<Point3<float>> Segment3dF;


}



/*! \} */ // end of GeometricEntities

} // End namespace I3D

#endif // I3D_SEGMENT_H
