#ifndef I3D_POINT_H
#define I3D_POINT_H

#include <limits>
#include <numeric>

#include "opencv2/core/core.hpp"

#include "core/defs.h"
#include "geometric_entities/window.h"

namespace I3D
{

/*! \defgroup GeometricEntities Entidades geométricas
 *  Puntos, lineas, ...
 *  \{
 */

///*!
// * \brief Clase punto
// *
// * Esta template es un wrapper de la template point_<T> de OpenCV.
// *
// * Se han definido los siguientes alias para facilitar el acceso:
// * \code
// * typedef Point<int> PointI;
// * typedef Point<double> PointD;
// * typedef Point<float> PointF;
// * \endcode
// */
//template<typename T>
//class I3D_EXPORT Point
//{
//public:
//
//  /*!
//   * \brief type
//   */
//  typedef T type;
//
//private:
//
//  /*!
//   * \brief Punto
//   */
//  cv::Point_<T> mPoint;
//
//public:
//
//  /*!
//   * \brief Constructora por defecto
//   */
//  Point();
//
//  /*!
//   * \brief Constructor
//   * \param[in] x Coordenada x
//   * \param[in] y Coordenada y
//   */
//  Point(T x, T y);
//
//  /*!
//   * \brief Constructor de copia
//   * \param[in] point Objeto Point que se copia
//   */
//  Point(const Point &point);
//
//};

/* ---------------------------------------------------------------------------------- */

template<typename T>
class I3D_EXPORT PointsList
{
public:

  /*!
   * \brief type
   */
  typedef T type;

protected:

  /*!
   * \brief Conjunto de puntos
   */
  std::vector<cv::Point_<T>> mPoints;

public:

  /*!
   * \brief Constructora por defecto
   */
  PointsList();

  /*!
   * \brief Constructor que reserva tamaño para n puntos
   */
  PointsList(int size);

  /*!
   * \brief Constructor de copia
   * \param[in] multiPoint Objeto MultiPoint que se copia
   */
  PointsList(const PointsList &listPoints);

  /*!
   * \brief Constructor
   * \param[in] vPoint vector de puntos
   */
  PointsList(const std::vector<cv::Point_<T>> &vPoint);

  /*!
   * \brief Constructor lista de inicialización
   * \param[in] listPoints Inicializador de lista con los puntos
   */
  PointsList(std::initializer_list<cv::Point_<T>> listPoints);

  //~PointsList();

  /*!
   * \brief Sobrecarga del operador de asignación
   * \param[in] pointsList PointsList que se asigna
   * \return Referencia al PointsList
   */
  PointsList &operator = (const PointsList &pointsList);

  /*!
   * \brief Añade un punto a la colección
   * \param[in] point Punto que se añade
   */
  virtual void add(const cv::Point_<T> &point) = 0;

  /*!
   * \brief Devuelve los puntos de la colección
   * \return vector de puntos
   */
  const std::vector<cv::Point_<T>> &getPoints() const { return mPoints; }

  /*!
   * \brief Devuelve los puntos que esta dentro de una ventana
   * \param[in] w Ventana
   * \return Puntos que entran dentro de la ventana
   */
  std::vector<cv::Point_<T>> getPointsInWindow(const Window<T> &w) const;

  /*!
   * \brief Número de puntos de la colección
   * \return Número de puntos
   */
  int getSize() const { return static_cast<int>(mPoints.size()); }

  /*!
   * \brief Ventana envolvente
   * \return Ventana envolvente de los puntos
   */
  Window<T> getWindow() const;

  /*!
   * \brief resize
   * \param[in] size nuevo tamaño
   */
  //void resize(int size) { mPoints.resize(size); }

  /*!
   * \brief Operador de indexación sobrecargado
   * \param[in] id indice del elemento
   * \return cv::Point_<T>
   */
  cv::Point_<T> &operator[](size_t id) { return mPoints[id];  }

};


template<typename T> inline
PointsList<T>::PointsList() : mPoints(0) {}

template<typename T> inline
PointsList<T>::PointsList(int size) : mPoints(size) {}

template<typename T> inline
PointsList<T>::PointsList(const PointsList &pointsList) : mPoints(pointsList.mPoints) {}

template<typename T> inline
PointsList<T>::PointsList(const std::vector<cv::Point_<T>> &vPoint) : mPoints(vPoint) {}

template<typename T> inline
PointsList<T>::PointsList(std::initializer_list<cv::Point_<T>> pointsList) : mPoints(pointsList) {}

template<typename T> inline
PointsList<T> &PointsList<T>::operator = (const PointsList &pointsList)
{
  if (this != &pointsList) {
    mPoints = pointsList.mPoints;
  }
  return *this;
}

//template<typename T> inline
//void PointsList<T>::add(const cv::Point_<T> &point)
//{
//  mPoints.push_back(point);
//}

template<typename T> inline
std::vector<cv::Point_<T>> PointsList<T>::getPointsInWindow(const Window<T> &w) const
{
  std::vector<cv::Point_<T>> r_points(mPoints.size());
  int j = 0;
  for (size_t i = 0; i < mPoints.size(); i++) {
    if (w.containsPoint(mPoints[i])) {
      r_points[i] = mPoints[i];
      j++;
    }
  }
  r_points.resize(j);
  return r_points;
}

template<typename T> inline
Window<T> PointsList<T>::getWindow() const
{
  Window<T> w;
  for (size_t i = 0; i < mPoints.size(); i++) {
    if (w.pt1.x > mPoints[i].x) w.pt1.x = mPoints[i].x;
    if (w.pt1.y > mPoints[i].y) w.pt1.y = mPoints[i].y;
    if (w.pt2.x < mPoints[i].x) w.pt2.x = mPoints[i].x;
    if (w.pt2.y < mPoints[i].y) w.pt2.y = mPoints[i].y;
  }
  return w;
}

/* ---------------------------------------------------------------------------------- */

template<typename T>
class I3D_EXPORT Points3DList
{
public:

  /*!
   * \brief type
   */
  typedef T type;

protected:

  /*!
   * \brief Conjunto de puntos
   */
  std::vector<cv::Point3_<T>> mPoints;

public:

  /*!
   * \brief Constructora por defecto
   */
  Points3DList();

  /*!
   * \brief Constructor que reserva tamaño para n puntos
   */
  Points3DList(int size);

  /*!
   * \brief Constructor de copia
   * \param[in] multiPoint Objeto MultiPoint que se copia
   */
  Points3DList(const Points3DList &listPoints);

  /*!
   * \brief Constructor
   * \param[in] vPoint vector de puntos
   */
  Points3DList(const std::vector<cv::Point3_<T>> &vPoint);

  /*!
   * \brief Constructor lista de inicialización
   * \param[in] listPoints Inicializador de lista con los puntos
   */
  Points3DList(std::initializer_list<cv::Point3_<T>> listPoints);

  //~Points3DList();

  /*!
   * \brief Sobrecarga del operador de asignación
   * \param[in] pointsList PointsList que se asigna
   * \return Referencia al PointsList
   */
  Points3DList &operator = (const Points3DList &pointsList);

  /*!
   * \brief Añade un punto a la colección
   * \param[in] point Punto que se añade
   */
  virtual void add(const cv::Point3_<T> &point) = 0;

  /*!
   * \brief Devuelve los puntos de la colección
   * \return vector de puntos
   */
  const std::vector<cv::Point3_<T>> &getPoints() const { return mPoints; }

  /*!
   * \brief Devuelve los puntos que esta dentro de una ventana
   * \param[in] w Ventana
   * \return Puntos que entran dentro de la ventana
   */
  std::vector<cv::Point3_<T>> getPointsInWindow(const Window<T> &w) const;

  /*!
   * \brief Número de puntos de la colección
   * \return Número de puntos
   */
  int getSize() const { return static_cast<int>(mPoints.size()); }

  /*!
   * \brief Ventana envolvente
   * \return Ventana envolvente de los puntos
   */
  Window<T> getWindow() const;

  /*!
   * \brief resize
   * \param[in] size nuevo tamaño
   */
  //void resize(int size) { mPoints.resize(size); }

  /*!
   * \brief Operador de indexación sobrecargado
   * \param[in] id indice del elemento
   * \return cv::Point_<T>
   */
  cv::Point3_<T> &operator[](size_t id) { return mPoints[id];  }

};


template<typename T> inline
Points3DList<T>::Points3DList() : mPoints(0) {}

template<typename T> inline
Points3DList<T>::Points3DList(int size) : mPoints(size) {}

template<typename T> inline
Points3DList<T>::Points3DList(const Points3DList &pointsList) : mPoints(pointsList.mPoints) {}

template<typename T> inline
Points3DList<T>::Points3DList(const std::vector<cv::Point3_<T>> &vPoint) : mPoints(vPoint) {}

template<typename T> inline
Points3DList<T>::Points3DList(std::initializer_list<cv::Point3_<T>> pointsList) : mPoints(pointsList) {}

template<typename T> inline
Points3DList<T> &Points3DList<T>::operator = (const Points3DList &pointsList)
{
  if (this != &pointsList) {
    mPoints = pointsList.mPoints;
  }
  return *this;
}

template<typename T> inline
std::vector<cv::Point3_<T>> Points3DList<T>::getPointsInWindow(const Window<T> &w) const
{
  std::vector<cv::Point3_<T>> r_points(mPoints.size());
  int j = 0;
  for (size_t i = 0; i < mPoints.size(); i++) {
    if (w.containsPoint(mPoints[i])) {
      r_points[i] = mPoints[i];
      j++;
    }
  }
  r_points.resize(j);
  return r_points;
}

template<typename T> inline
Window<T> Points3DList<T>::getWindow() const
{
  Window<T> w;
  for (size_t i = 0; i < mPoints.size(); i++) {
    if (w.pt1.x > mPoints[i].x) w.pt1.x = mPoints[i].x;
    if (w.pt1.y > mPoints[i].y) w.pt1.y = mPoints[i].y;
    if (w.pt2.x < mPoints[i].x) w.pt2.x = mPoints[i].x;
    if (w.pt2.y < mPoints[i].y) w.pt2.y = mPoints[i].y;
  }
  return w;
}

/* ---------------------------------------------------------------------------------- */

/*!
 * \brief Clase multi-punto
 *
 * Esta template representa un conjunto de puntos relaccionados que se agrupan
 * en una misma entidad multipunto.
 *
 * Se han definido los siguientes alias para facilitar el acceso:
 * \code
 * typedef MultiPoint<int> MultiPointI;
 * typedef MultiPoint<double> MultiPointD;
 * typedef MultiPoint<float> MultiPointF;
 * \endcode
 */
template<typename T>
class I3D_EXPORT MultiPoint : public PointsList<T>
{

public:

  /*!
   * \brief Constructora por defecto
   */
  MultiPoint() : PointsList<T>() {}

  /*!
   * \brief Constructor que reserva tamaño para n puntos
   */
  MultiPoint(int size) : PointsList<T>(size) {}

  /*!
   * \brief Constructor de copia
   * \param[in] multiPoint Objeto MultiPoint que se copia
   */
  MultiPoint(const MultiPoint &multiPoint) : PointsList<T>(multiPoint) {}

  /*!
   * \brief Constructor
   * \param[in] vPoint vector de puntos
   */
  MultiPoint(const std::vector<cv::Point_<T>> &vPoint) : PointsList<T>(vPoint) {}

  /*!
   * \brief Constructor lista de inicialización
   * \param[in] listPoints Inicializador de lista con los puntos
   */
  MultiPoint(std::initializer_list<cv::Point_<T>> listPoints) : PointsList<T>(listPoints) {}

  //~MultiPoint();

  /*!
   * \brief Sobrecarga del operador de asignación
   * \param[in] multiPoint MultiPoint que se asigna
   * \return Referencia al MultiPoint
   */
  //MultiPoint &operator = (const MultiPoint &multiPoint);

  /*!
   * \brief Añade un punto a la colección
   * \param[in] point Punto que se añade
   */
  void add(const cv::Point_<T> &point) override;

  /*!
   * \brief Devuelve los puntos de la colección
   * \return vector de puntos
   */
  //const std::vector<cv::Point_<T>> &getPoints() const { return mPoints; }

  /*!
   * \brief Devuelve los puntos que esta dentro de una ventana
   * \param[in] w Ventana
   * \return Puntos que entran dentro de la ventana
   */
  //std::vector<cv::Point_<T>> getPointsInWindow(const Window<T> &w) const;

  /*!
   * \brief Número de puntos de la colección
   * \return Número de puntos
   */
  //int getSize() const { return static_cast<int>(mPoints.size()); }

  /*!
   * \brief Ventana envolvente
   * \return Ventana envolvente del MultiPoint
   */
  //Window<T> getWindow() const;

  /*!
   * \brief resize
   * \param[in] size nuevo tamaño
   */
  void resize(int size) { mPoints.resize(size); }

  /*!
   * \brief Operador de indexación sobrecargado
   * \param[in] id indice del elemento
   * \return cv::Point_<T>
   */
  //cv::Point_<T> &operator[](size_t id) { return mPoints[id];  }

};


//template<typename T> inline
//MultiPoint<T>::MultiPoint() : mPoints(0) {}
//
//template<typename T> inline
//MultiPoint<T>::MultiPoint(int size) : mPoints(size) {}
//
//template<typename T> inline
//MultiPoint<T>::MultiPoint(const MultiPoint &multiPoint) : mPoints(multiPoint.mPoints) {}
//
//template<typename T> inline
//MultiPoint<T>::MultiPoint(const std::vector<cv::Point_<T>> &vPoint) : mPoints(vPoint) {}
//
//template<typename T> inline
//MultiPoint<T>::MultiPoint(std::initializer_list<cv::Point_<T>> listPoints) : mPoints(listPoints) {}
//
//template<typename T> inline
//MultiPoint<T> &MultiPoint<T>::operator = (const MultiPoint &multiPoint)
//{
//  if (this != &multiPoint) {
//    mPoints = multiPoint.mPoints;
//  }
//  return *this;
//}

template<typename T> inline
void MultiPoint<T>::add(const cv::Point_<T> &point)
{
  mPoints.push_back(point);
}

//template<typename T> inline
//std::vector<cv::Point_<T>> MultiPoint<T>::getPointsInWindow(const Window<T> &w) const
//{
//  std::vector<cv::Point_<T>> r_points(mPoints.size());
//  int j = 0;
//  for (size_t i = 0; i < mPoints.size(); i++) {
//    if (w.containsPoint(mPoints[i])) {
//      r_points[i] = mPoints[i];
//      j++;
//    }
//  }
//  r_points.resize(j);
//  return r_points;
//}
//
//template<typename T> inline
//Window<T> MultiPoint<T>::getWindow() const
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

typedef MultiPoint<int> MultiPointI;
typedef MultiPoint<double> MultiPointD;
typedef MultiPoint<float> MultiPointF;

/*! \} */ // end of GeometricEntities

} // End namespace I3D

#endif // I3D_POINT_H
