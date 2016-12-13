#ifndef I3D_POINT_H
#define I3D_POINT_H

#include <limits>
#include <numeric>

#include "opencv2/core/core.hpp"

#include "core/defs.h"
#include "geometric_entities/entity.h"
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
class I3D_EXPORT MultiPoint : public EntityPoints<T>
{

public:

  /*!
   * \brief Constructora por defecto
   */
  MultiPoint();

  /*!
   * \brief Constructor que reserva tamaño para n puntos
   */
  MultiPoint(int size);

  /*!
   * \brief Constructor de copia
   * \param[in] multiPoint Objeto MultiPoint que se copia
   */
  MultiPoint(const MultiPoint &multiPoint);

  /*!
   * \brief Constructor
   * \param[in] vPoint vector de puntos
   */
  MultiPoint(const std::vector<cv::Point_<T>> &vPoint);

  /*!
   * \brief Constructor lista de inicialización
   * \param[in] listPoints Inicializador de lista con los puntos
   */
  MultiPoint(std::initializer_list<cv::Point_<T>> listPoints);

  ~MultiPoint() {}

  /*!
   * \brief Añade un punto a la colección
   * \param[in] point Punto que se añade
   */
  void add(const cv::Point_<T> &point) override;

  /*!
   * \brief resize
   * \param[in] size nuevo tamaño
   */
  void resize(int size) { this->mPoints.resize(size); }

};

template<typename T> inline
MultiPoint<T>::MultiPoint() 
  : EntityPoints<T>(entity_type::MULTIPOINT_POINT_2D) {}

template<typename T> inline
MultiPoint<T>::MultiPoint(int size) 
  : EntityPoints<T>(entity_type::MULTIPOINT_POINT_2D, size) {}

template<typename T> inline
MultiPoint<T>::MultiPoint(const MultiPoint &multiPoint) 
  : EntityPoints<T>(entity_type::MULTIPOINT_POINT_2D, multiPoint) {}

template<typename T> inline
MultiPoint<T>::MultiPoint(const std::vector<cv::Point_<T>> &vPoint) 
  : EntityPoints<T>(entity_type::MULTIPOINT_POINT_2D, vPoint) {}

template<typename T> inline
MultiPoint<T>::MultiPoint(std::initializer_list<cv::Point_<T>> listPoints) 
: EntityPoints<T>(entity_type::MULTIPOINT_POINT_2D, listPoints) {}


template<typename T> inline
void MultiPoint<T>::add(const cv::Point_<T> &point)
{
  this->mPoints.push_back(point);
}

typedef MultiPoint<int> MultiPointI;
typedef MultiPoint<double> MultiPointD;
typedef MultiPoint<float> MultiPointF;

/*! \} */ // end of GeometricEntities

} // End namespace I3D

#endif // I3D_POINT_H
