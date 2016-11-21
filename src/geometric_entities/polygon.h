/*
 La ventana o bbox se tendria que recalcular cada vez que se modifica
 para no tener que recorrer todos los puntos si queremos obtener las dimensiones
 del elemento.
*/

#ifndef I3D_POLYGON_H
#define I3D_POLYGON_H

#include "opencv2/core/core.hpp"

#include "core/utils.h"
#include "core/mathutils.h"
#include "geometric_entities/window.h"
#include "geometric_entities/point.h"
#include "geometric_entities/operations.h"

namespace I3D
{

/*! \defgroup GeometricEntities Entidades geométricas
 *  Puntos, lineas, ...
 *  \{
 */

/* ---------------------------------------------------------------------------------- */

/*!
 * \brief Clase poligono 2D
 *
 * Se han definido los siguientes alias para facilitar el acceso:
 * \code
 * typedef Polygon<int>PolygonI;
 * typedef Polygon<double> PolygonD;
 * typedef Polygon<float> PolygonF;
 * \endcode
 */
template<typename T>
class I3D_EXPORT Polygon : public PointsList<T>
{

public:

  /*!
   * \brief Constructora por defecto
   */
  Polygon() : PointsList<T>() {}

  /*!
   * \brief Constructor que establece el tamaño del poligono
   */
  Polygon(int size) : PointsList<T>(size) {}

  /*!
   * \brief Constructor de copia
   * \param[in] polygon
   */
  Polygon(const Polygon &polygon) : PointsList<T>(multiPoint) {}

  /*!
   * \brief Constructor
   * \param[in] points
   */
  Polygon(const std::vector<cv::Point_<T>> &points) : PointsList<T>(points) {}

  /*!
   * \brief Constructor lista de inicialización
   * \param[in] listPoints Inicializador de lista con los puntos
   */
  Polygon(std::initializer_list<cv::Point_<T>> listPoints) : PointsList<T>(listPoints) {}

  /*!
   * \brief Sobrecarga del operador de asignación
   * \param[in] polygon Poligono que se asigna
   * \return Referencia al poligono
   */
  //Polygon &operator = (const Polygon &polygon);

  /*!
   * \brief Ventana envolvente
   * \return Ventana envolvente del segmento
   */
  //Window<T> getWindow() const;

  /*!
   * \brief Añade un punto a la colección
   * \param[in] point Punto que se añade
   */
  void add(const cv::Point_<T> &point) override;

  /*!
   * \brief Comprueba si un punto esta dentro del poligono
   * \param[in] point Punto
   */
  bool isInner(const cv::Point_<T> &point) const;
  
  /*!
   * \brief Perimetro del poligono
   * \return Perimetro del poligono
   */
  double perimeter() const;

};

// Definición de métodos

//template<typename T> inline
//Polygon<T>::Polygon( ) : mPoints(0) {}
//
//template<typename T> inline
//Polygon<T>::Polygon(const Polygon &polygon) : mPoints(polygon.mPoints) {}
//
//template<typename T> inline
//Polygon<T>::Polygon(const std::vector<cv::Point_<T>> &vPoint) : mPoints(vPoint) {}
//
//template<typename T> inline
//Polygon<T>::Polygon(std::initializer_list<cv::Point_<T>> listPoints) : mPoints(listPoints) {}
//
//template<typename T> inline
//Polygon<T> &Polygon<T>::operator = (const Polygon &poligon)
//{
//  if (this != &poligon) mPoints = poligon.mPoints;
//  return *this;
//}

//template<typename T> inline
//Window<T> Polygon<T>::getWindow() const
//{
//  Window<T> w;
//  for (size_t i = 0; i < mPoints.size(); i++) {
//    if (w.pt1.x > mPoints[i].x) w.pt1.x = mPoints[i].x;
//    if (w.pt1.y > mPoints[i].y) w.pt1.y = mPoints[i].y;
//    if (w.pt2.x < mPoints[i].x) w.pt2.x = mPoints[i].x;
//    if (w.pt2.y < mPoints[i].y) w.pt2.y = mPoints[i].y;
//  }
//  return w;
//}

template<typename T> inline
void Polygon<T>::add(const cv::Point_<T> &point)
{
  mPoints.push_back(point);
}

template<typename T> inline
bool Polygon<T>::isInner(const cv::Point_<T> &point) const
{
  Window<T> w = getWindow();
  // Comprueba si esta dentro de la ventana envolvente.
  if (w.containsPoint(point) == false) return false;

  // Comprueba si corta una recta que pase por el punto corta en un numero 
  // par al poligono. La recta va desde el punto a la parte inferior, superior,
  // derecha o izquierda de la ventana.
  //... Se suma uno porque Mat comienza en cero. Se podria hacer una conversion a cv::Rect 
  //cv::Mat aux(w.getHeight()+1, w.getWidth()+1, CV_8UC3);
  //cv::line(aux, point-w.pt1, point-w.pt1, Color(Color::NAME::Red).get<cv::Scalar>(), 1);
  int nIntersection = 0;
  bool bVertex = false;
  for (size_t i = 0, j = 1; i < mPoints.size(); i++, j++) {
    if (j == mPoints.size()) j = 0;
    //cv::line(aux, mPoints[i]-w.pt1, mPoints[j]-w.pt1, Color(Color::NAME::Blue).get<cv::Scalar>(), 1);
    Segment<T> segment(mPoints[i], mPoints[j]);
    
    // El punto es colineal con el segmento
    if ( isCollinearPoints(point, segment, 0.5)) return true;
    cv::Point ptp;
    //if (projectPointInSegment(segment, point, &ptp) == 2) return true;
    Segment<T> sPointH(point, cv::Point_<T>(w.pt2.x, point.y));
    if (point.y == segment.pt1.y || point.y == segment.pt2.y)
      bVertex = true;
    nIntersection += intersectSegments(segment, sPointH, &ptp);
  }
  if (bVertex) 
    nIntersection--;
  if (nIntersection % 2 == 0) return false;
  else return true;
}

template<typename T> inline
double Polygon<T>::perimeter()  const
{
  double perimeter = 0.;
  for (size_t i = 1; i < mPoints.size(); i++) {
    perimeter += distance(mPoints[i-1], mPoints[i]);
  }
  return perimeter;
}

typedef Polygon<int> PolygonI;
typedef Polygon<double> PolygonD;
typedef Polygon<float> PolygonF;

/* ---------------------------------------------------------------------------------- */

/*!
 * \brief Clase poligono 3D
 *
 * Se han definido los siguientes alias para facilitar el acceso:
 *
 * typedef Polygon3D<int> Polygon3dI;
 * typedef Polygon3D<double> Polygon3dD;
 * typedef Polygon3D<float> Polygon3dF;
 *
 */
template<typename T>
class I3D_EXPORT Polygon3D : public Points3DList<T>
{

public:

  /*!
   * \brief Constructora por defecto
   */
  Polygon3D() : Points3DList<T>() {}

  /*!
   * \brief Constructor que establece el tamaño del poligono
   */
  Polygon3D(int size) : Points3DList<T>(size) {}

  /*!
   * \brief Constructor de copia
   * \param[in] polygon
   */
  Polygon3D(const Polygon3D &polygon) : Points3DList<T>(multiPoint) {}

  /*!
   * \brief Constructor
   * \param[in] points
   */
  Polygon3D(const std::vector<cv::Point3_<T>> &points) : Points3DList<T>(vPoint) {}

  /*!
   * \brief Constructor lista de inicialización
   * \param[in] listPoints Inicializador de lista con los puntos
   */
  Polygon3D(std::initializer_list<cv::Point3_<T>> listPoints) : Points3DList<T>(listPoints) {}

  /*!
   * \brief Añade un punto a la colección
   * \param[in] point Punto que se añade
   */
  void add(const cv::Point3_<T> &point) override;

  /*!
   * \brief Perimetro del poligono
   * \return Perimetro del poligono
   */
  double perimeter() const;

};

template<typename T> inline
void Polygon3D<T>::add(const cv::Point3_<T> &point)
{
  mPoints.push_back(point);
}

template<typename T> inline
double Polygon3D<T>::perimeter()  const
{
  double perimeter = 0.;
  for (size_t i = 1; i < mPoints.size(); i++) {
    perimeter += distance(mPoints[i-1], mPoints[i]);
  }
  return perimeter;
}

typedef Polygon3D<int> Polygon3dI;
typedef Polygon3D<double> Polygon3dD;
typedef Polygon3D<float> Polygon3dF;


/*! \} */ // end of GeometricEntities

} // End namespace I3D

#endif // I3D_POLYGON_H
