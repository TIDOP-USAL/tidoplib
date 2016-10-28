#ifndef I3D_SEGMENT_H
#define I3D_SEGMENT_H

#include "opencv2/core/core.hpp"

#include "utils.h"
#include "geometric_entities\window.h"

namespace I3D
{

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
class I3D_EXPORT Segment
{
public:

  /*!
   * \brief type
   */
  typedef T type;

  /*!
   * \brief Punto 1
   */
  cv::Point_<T> pt1;

  /*!
   * \brief Punto 2
   */
  cv::Point_<T> pt2;

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
  Segment(const cv::Point_<T> &_pt1, const cv::Point_<T> &_pt2);

  /*!
   * \brief Constructor segment
   * \param[in]lvect
   */
  Segment(const cv::Vec<T, 4> &lvect);

  /*!
   * \brief Sobrecarga del operador de asignación
   * \param[in] segment Segmento que se asigna
   * \return Referencia a la ventana
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
  cv::Point_<T> vector() const { return (pt2 - pt1); }
};

// Definición de métodos

template<typename T> inline
Segment<T>::Segment( ) : pt1(0, 0), pt2(0, 0) {}

template<typename T> inline
Segment<T>::Segment(const Segment &Segment) : pt1(Segment.pt1), pt2(Segment.pt2) {}

template<typename T> inline
Segment<T>::Segment( const cv::Point_<T> &_pt1, const cv::Point_<T> &_pt2 ) : pt1(_pt1), pt2(_pt2) {}

template<typename T> inline
Segment<T>::Segment( const cv::Vec<T, 4> &lvect )
{
  pt1.x = lvect[0];
  pt1.y = lvect[1];
  pt2.x = lvect[2];
  pt2.y = lvect[3];
}

template<typename T> inline
Segment<T> &Segment<T>::operator = (const Segment &segment)
{
  if (this != &segment) {
    pt1 = segment.pt1;
    pt2 = segment.pt2;
  }
  return *this;
}

template<typename T> template<typename T2> inline
Segment<T>::operator Segment<T2>() const
{
  cv::Point_<T2> _pt1 = pt1;
  cv::Point_<T2> _pt2 = pt2;
  return Segment<T2>(_pt1, _pt2);
}

template<typename T> inline
double Segment<T>::angleOX() const
{
  double angle = 0.0;
  if (pt1 != pt2){
    angle = vectorAngleOX(vector());
  } else
    angle = NULL;
  return angle;

}

template<typename T> inline
double Segment<T>::angleOY() const
{
  double angle = 0.0;
  if (pt1 != pt2){
    angle = vectorAngleOY(vector());
  } else
    angle = NULL;
  return angle;
}

template<typename T> inline
Window<T> Segment<T>::getWindow() const
{
  return Window<T>(pt1,pt2);
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
class I3D_EXPORT Segment3D
{
public:

  typedef T type;

  /*!
   * \brief Punto 1
   */
  cv::Point3_<T> pt1;

  /*!
   * \brief Punto 2
   */
  cv::Point3_<T> pt2;

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
  Segment3D(const cv::Point3_<T> &_pt1, const cv::Point3_<T> &_pt2);

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
  cv::Point3_<T> vector() const { return (pt2 - pt1); }
};

// Definición de métodos

template<typename T> inline
Segment3D<T>::Segment3D( ) : pt1(0, 0, 0), pt2(0, 0, 0) {}

template<typename T> inline
Segment3D<T>::Segment3D(const Segment3D &Segment) : pt1(Segment.pt1), pt2(Segment.pt2) {}

template<typename T> inline
Segment3D<T>::Segment3D( const cv::Point3_<T> &_pt1, const cv::Point3_<T> &_pt2 ) : pt1(_pt1), pt2(_pt2) {}

template<typename T> inline
Segment3D<T> &Segment3D<T>::operator = (const Segment3D &segment)
{
  if (this != &segment) {
    pt1 = segment.pt1;
    pt2 = segment.pt2;
  }
  return *this;
}

template<typename T> template<typename T2> inline
Segment3D<T>::operator Segment3D<T2>() const
{
  cv::Point3_<T2> _pt1 = pt1;
  cv::Point3_<T2> _pt2 = pt2;
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

/* ---------------------------------------------------------------------------------- */
/*                               Operaciones con Líneas                               */
/* ---------------------------------------------------------------------------------- */

/*!
 * \brief Crea un buffer entorno a una linea
 * \param[in] ln Línea
 * \param[in] size Tamaño de buffer
 * \param[out] buff Buffer
 */
void I3D_EXPORT lineBuffer(const Line &ln, int size, std::vector<cv::Point> *buff);

/*!
 * \brief Projecta un punto en un segmento de recta.
 * Si no hay punto de proyección en el segmento se devuelve nulo
 * \param[in] ln Segmento de línea
 * \param[in] pt Punto que se proyecta
 * \param[out] ptp Punto proyectado
 * \return -1, 0, 1
 */
int I3D_EXPORT projectPointInSegment(const Line &ln, const cv::Point &pt, cv::Point *ptp);

/*!
 * \brief Calcula la distancia de un punto a un segmento de linea.
 * \param[in] pt Punto
 * \param[in] ln Linea
 * \return Distancia de un punto a una segmento de linea
 */
double I3D_EXPORT distPointToSegment(const cv::Point &pt, const Line &ln);

/*!
 * \brief distPointToLine
 * \param[in] pt
 * \param[in] ln
 * \return
 */
double I3D_EXPORT distPointToLine(const cv::Point &pt, const Line &ln);

/*!
 * \brief Calcula la distancia mínima entre dos segmentos de linea.
 * \param[in] ln1 Segmento 1
 * \param[in] ln2 Segmento 2
 * \return Distancia entre segmentos
 */
double I3D_EXPORT minDistanceSegments(const Line &ln1, const Line &ln2);

/*!
 * \brief Intersect de dos segmentos de línea
 * \param ln1 Primer segmento
 * \param ln2 Segundo segmento
 * \param pt Punto de intersección
 * \return
 */
int I3D_EXPORT intersectSegments(const Line &ln1, const Line &ln2, cv::Point *pt);

/*!
 * \brief Determina la intersección de los lineas
 * \param[in] ln1 Primera línea
 * \param[in] ln2 Segunda linea
 * \param[out] pt Punto de intersección
 * \return
 */
int I3D_EXPORT intersectLines(const Line &ln1, const Line &ln2, cv::Point *pt);

/*!
 * \brief joinLinesByDist
 * \param[in] linesIn
 * \param[out] linesOut
 * \param[in] dist
 */
void I3D_EXPORT joinLinesByDist(const std::vector<Line> &linesIn, std::vector<Line> *linesOut, int dist);


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

  /*!
   * \brief Añade una línea
   * \param[in] lvect
   */
  void add(const cv::Vec4i &lvect);

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

void I3D_EXPORT groupParallelLines(std::vector<Line> linesaux, std::vector<ldGroupLines> *curLinesGrops, double angTol);

void I3D_EXPORT groupLinesByDist(const std::vector<Line> &linesIn, std::vector<ldGroupLines> *curLinesGrops, int dist);

void I3D_EXPORT delLinesGroupBySize(std::vector<ldGroupLines> *vlg, int size);

/*! \} */ // end of GeometricEntities

} // End namespace I3D

#endif // I3D_SEGMENT_H
