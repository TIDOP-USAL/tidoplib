#ifndef I3D_LINESTRING_H
#define I3D_LINESTRING_H

#include "opencv2/core/core.hpp"

#include "core/utils.h"
#include "core/mathutils.h"
#include "geometric_entities/entity.h"
#include "geometric_entities/point.h"
#include "geometric_entities/window.h"
#include "geometric_entities/operations.h"

namespace I3D
{

/*! \defgroup GeometricEntities Entidades geométricas
 *  Puntos, lineas, ...
 *  \{
 */

/* ---------------------------------------------------------------------------------- */

/*!
 * \brief Clase polilínea 2D
 *
 * Se han definido los siguientes alias para facilitar el acceso:
 * \code
 * typedef LineString<int> LineStringI;
 * typedef LineString<double> LineStringD;
 * typedef LineString<float> LineStringF;
 * \endcode
 */
template <typename T>
class I3D_EXPORT LineString : public EntityPoints<T> 
{

public:

  /*!
   * \brief Constructora por defecto
   * Crea un vector con 0 elementos
   */
	LineString();

  /*!
   * \brief Constructor que establece el tamaño de la polilínea
   */
  LineString(int size);

  /*!
   * \brief Constructor de copia
   * \param[in] linestring
   */
	LineString(const LineString &linestring);

  /*!
   * \brief Constructor vector de puntos
   * \param[in] points
   */
  LineString(const std::vector<cv::Point_<T>> &points);

  /*!
   * \brief Constructor lista de inicialización
   * \param[in] listPoints Inicializador de lista con los puntos
   */
	LineString(std::initializer_list<cv::Point_<T>> listPoints);
    
  /*!
   * \brief Destructora
   */
  ~LineString() {}

  /*!
   * \brief Añade un punto a la colección
   * \param[in] point Punto que se añade
   */
  void add(const cv::Point_<T> &point) override;

  /*!
   * \brief Longitud de la polilínea
   * \return Longitud
   */
  double length() const;

};

template <typename T> inline
LineString<T>::LineString()
  : EntityPoints<T>(entity_type::LINESTRING_2D) {}

template<typename T> inline
LineString<T>::LineString(int size) 
  : EntityPoints<T>(entity_type::LINESTRING_2D, size) {}

template <typename T> inline 
LineString<T>::LineString(const LineString &linea) 
  : EntityPoints<T>(entity_type::LINESTRING_2D, linea) {}

template <typename T> inline 
LineString<T>::LineString(const std::vector<cv::Point_<T>> &points)
  : EntityPoints<T>(entity_type::LINESTRING_2D, points) {}

template <typename T> inline
LineString<T>::LineString(std::initializer_list<cv::Point_<T>> listPoints) 
  : EntityPoints<T>(entity_type::LINESTRING_2D, listPoints) {}

template<typename T> inline
void LineString<T>::add(const cv::Point_<T> &point)
{ 
  mPoints.push_back(point);
}

template<typename T> inline
double LineString<T>::length()  const
{
  double length = 0.;
  for (size_t i = 1; i < mPoints.size(); i++) {
    length += distance(mPoints[i-1], mPoints[i]);
  }
  return length;
}

typedef LineString<int> LineStringI;
typedef LineString<float> LineStringF;
typedef LineString<double> LineStringD;

/*! \} */ // end of GeometricEntities

} // End namespace I3D

#endif  // I3D_LINESTRING_H