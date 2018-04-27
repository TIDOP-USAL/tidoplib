#ifndef TL_GEOM_LINESTRING_H
#define TL_GEOM_LINESTRING_H

#include "config_tl.h"

#ifdef HAVE_OPENCV
#include "opencv2/core/core.hpp"
#endif 

#include "core/defs.h"
#include "core/utils.h"
#include "core/mathutils.h"
#include "geometry/entities/entity.h"
#include "geometry/entities/point.h"
#include "geometry/entities/window.h"
#include "geometry/operations.h"

namespace TL
{

/*! \defgroup GeometricEntities Entidades geométricas
 *  Puntos, lineas, ...
 *  \{
 */

namespace geometry
{

/* ---------------------------------------------------------------------------------- */


/*!
 * \brief Clase polilínea 2D
 *
 * Se han definido los siguientes alias para facilitar el acceso:
 * \code
 * typedef LineString<Point<int>> LineStringI;
 * typedef LineString<Point<double>> LineStringD;
 * typedef LineString<Point<float>> LineStringF;
 * \endcode
 */
template <typename Point_t>
class LineString : public Entity, public Entities2D<Point_t>
{
//public:
//
//  typedef Point_t value_type;

public:

  /*!
   * \brief Constructora por defecto
   * Crea un vector con 0 elementos
   */
  LineString();

  /*!
   * \brief Constructor que establece el tamaño de la polilínea
   */
  LineString(size_type size);

  /*!
   * \brief Constructor de copia
   * \param[in] linestring
   */
  LineString(const LineString &lineString);

  /*!
   * \brief Constructor vector de puntos
   * \param[in] points
   */
  LineString(const std::vector<Point_t> &points);

  /*!
   * \brief Constructor lista de inicialización
   * \param[in] listPoints Inicializador de lista con los puntos
   */
  LineString(std::initializer_list<Point_t> listPoints);

  /*!
   * \brief Destructora
   */
  ~LineString() {}

  /*!
   * \brief Añade un punto a la colección
   * \param[in] point Punto que se añade
   */
  //void add(const Point_t &point) override;

  /*!
   * \brief Longitud de la polilínea
   * \return Longitud
   */
  double length() const;

  /*!
   * \brief Operador de asignación
   */
  LineString<Point_t> &operator = (const LineString<Point_t> &lineString);
};

template <typename Point_t> inline
LineString<Point_t>::LineString()
  : Entity(Entity::type::LINESTRING_2D), 
    Entities2D<Point_t>() 
{
}

template<typename Point_t> inline
LineString<Point_t>::LineString(size_type size)
  : Entity(Entity::type::LINESTRING_2D), 
    Entities2D<Point_t>(size) 
{
}

template <typename Point_t> inline
LineString<Point_t>::LineString(const LineString &lineString)
  : Entity(lineString),
    Entities2D<Point_t>(lineString)
{
}

template <typename Point_t> inline
LineString<Point_t>::LineString(const std::vector<Point_t> &points)
  : Entity(Entity::type::LINESTRING_2D), 
    Entities2D<Point_t>(points) 
{
}

template <typename Point_t> inline
LineString<Point_t>::LineString(std::initializer_list<Point_t> listPoints)
  : Entity(Entity::type::LINESTRING_2D), 
    Entities2D<Point_t>(listPoints) 
{
}

//template<typename Point_t> inline
//void LineString<Point_t>::add(const Point_t &point)
//{
//  this->mPoints.push_back(point);
//}

template<typename Point_t> inline
double LineString<Point_t>::length()  const
{
  double length = 0.;
  for (size_t i = 1; i < this->mEntities.size(); i++) {
    length += distance(this->mEntities[i - 1], this->mEntities[i]);
  }
  return length;
}

template<typename Point_t> inline
LineString<Point_t> &LineString<Point_t>::operator = (const LineString<Point_t> &lineString)
{
  if (this != &lineString) {
    Entity::operator = (lineString);
    Entities2D<Point_t>::operator = (lineString);
  }
  return *this;
}

typedef LineString<Point<int>> LineStringI;
typedef LineString<Point<float>> LineStringF;
typedef LineString<Point<double>> LineStringD;

/* ---------------------------------------------------------------------------------- */


/*!
 * \brief Clase polilínea 3D
 *
 */
template <typename Point3_t>
class LineString3D : public Entity, public Entities3D<Point3_t>
{

public:

  /*!
   * \brief Constructora por defecto
   * Crea un vector con 0 elementos
   */
  LineString3D();

  /*!
   * \brief Constructor que establece el tamaño de la polilínea
   */
  LineString3D(size_type size);

  /*!
   * \brief Constructor de copia
   * \param[in] linestring
   */
  LineString3D(const LineString3D &lineString);

  /*!
   * \brief Constructor vector de puntos
   * \param[in] points
   */
  LineString3D(const std::vector<Point3_t> &points);

  /*!
   * \brief Constructor lista de inicialización
   * \param[in] listPoints Inicializador de lista con los puntos
   */
  LineString3D(std::initializer_list<Point3_t> listPoints);

  /*!
   * \brief Destructora
   */
  ~LineString3D() {}

  /*!
   * \brief Añade un punto a la colección
   * \param[in] point Punto que se añade
   */
  //void add(const Point3_t &point) override;

  /*!
   * \brief Longitud de la polilínea
   * \return Longitud
   */
  double length() const;

  /*!
   * \brief Operador de asignación
   * \return lineString Objeto que se asigna
   */
  LineString3D<Point3_t> &operator = (const LineString3D<Point3_t> &lineString);

};

template <typename Point3_t> inline
LineString3D<Point3_t>::LineString3D()
  : Entity(Entity::type::LINESTRING_3D), 
    Entities3D<Point3_t>() 
{
}

template<typename Point3_t> inline
LineString3D<Point3_t>::LineString3D(size_type size)
  : Entity(Entity::type::LINESTRING_3D), 
    Entities3D<Point3_t>(size) 
{
}

template <typename Point3_t> inline
LineString3D<Point3_t>::LineString3D(const LineString3D &lineString)
  : Entity(lineString), 
    Entities3D<Point3_t>(lineString)
{
}

template <typename Point3_t> inline
LineString3D<Point3_t>::LineString3D(const std::vector<Point3_t> &points)
  : Entity(Entity::type::LINESTRING_3D), 
    Entities3D<Point3_t>(points) 
{
}

template <typename Point3_t> inline
LineString3D<Point3_t>::LineString3D(std::initializer_list<Point3_t> listPoints)
  : Entity(Entity::type::LINESTRING_3D), 
    Entities3D<Point3_t>(listPoints) 
{
}

//template<typename Point3_t> inline
//void LineString3D<Point3_t>::add(const Point3_t &point)
//{
//  this->mPoints.push_back(point);
//}

template<typename Point3_t> inline
double LineString3D<Point3_t>::length()  const
{
  double length = 0.;
  for (size_t i = 1; i < this->mEntities.size(); i++) {
    length += distance(this->mEntities[i - 1], this->mEntities[i]);
  }
  return length;
}

template<typename Point_t> inline
LineString3D<Point_t> &LineString3D<Point_t>::operator = (const LineString3D &lineString)
{
  if (this != &lineString) {
    Entity::operator = (lineString);
    Entities3D<Point_t>::operator = (lineString);
  }
  return *this;
}

typedef LineString3D<Point3<int>> LineString3dI;
typedef LineString3D<Point3<double>> LineString3dD;
typedef LineString3D<Point3<float>> LineString3dF;

/* ---------------------------------------------------------------------------------- */

template <typename Point_t>
class MultiLineString : public Entity, public Entities2D<LineString<Point_t>>
{

public:

  /*!
   * \brief Constructora por defecto
   */
  MultiLineString();

  /*!
   * \brief Constructor que reserva tamaño para n polilineas
   */
  MultiLineString(size_type size);

  /*!
   * \brief Constructor de copia
   * \param[in] multiLineString Objeto MultiLineString que se copia
   */
  MultiLineString(const MultiLineString &multiLineString);

  ~MultiLineString() {}

  /*!
   * \brief Operador de asignación
   * \return lineString Objeto que se asigna
   */
  MultiLineString<Point_t> &operator = (const MultiLineString<Point_t> &multiLineString);
};

template <typename Point_t>
MultiLineString<Point_t>::MultiLineString()
  : Entity(Entity::type::MULTILINE_2D),
    Entities2D<LineString<Point_t>>()
{
}

template<typename Point_t> inline
MultiLineString<Point_t>::MultiLineString(size_type size) 
  : Entity(Entity::type::MULTILINE_2D),
    Entities2D<LineString<Point_t>>(size) 
{
}

template<typename Point_t> inline
MultiLineString<Point_t>::MultiLineString(const MultiLineString &multiLineString) 
  : Entity(multiLineString), 
    Entities2D<LineString<Point_t>>(multiLineString) 
{
}

template<typename Point_t> inline
MultiLineString<Point_t> &MultiLineString<Point_t>::operator = (const MultiLineString &multiLineString)
{
  if (this != &multiLineString) {
    Entity::operator = (multiLineString);
    Entities2D<LineString<Point_t>>::operator = (multiLineString);
  }
  return *this;
}

/* ---------------------------------------------------------------------------------- */

template <typename Point3_t>
class MultiLineString3D : public Entity, public Entities3D<LineString3D<Point3_t>>
{

public:

  /*!
   * \brief Constructora por defecto
   */
  MultiLineString3D();

  /*!
   * \brief Constructor que reserva tamaño para n puntos
   */
  MultiLineString3D(size_type size);

  /*!
   * \brief Constructor de copia
   * \param[in] multiLineString Objeto MultiLineString que se copia
   */
  MultiLineString3D(const MultiLineString3D &multiLineString);

  ~MultiLineString3D() {}

  /*!
   * \brief Operador de asignación
   * \return lineString Objeto que se asigna
   */
  MultiLineString3D<Point3_t> &operator = (const MultiLineString3D<Point3_t> &multiLineString);

};

template <typename Point3_t>
MultiLineString3D<Point3_t>::MultiLineString3D()
  : Entity(Entity::type::MULTILINE_3D),
    Entities3D<LineString3D<Point3_t>>()
{
}

template<typename Point3_t> inline
MultiLineString3D<Point3_t>::MultiLineString3D(size_type size) 
  : Entity(Entity::type::MULTILINE_3D),
    Entities3D<LineString3D<Point3_t>>(size) 
{
}

template<typename Point3_t> inline
MultiLineString3D<Point3_t>::MultiLineString3D(const MultiLineString3D &multiLineString) 
  : Entity(multiLineString), 
    Entities3D<LineString3D<Point3_t>>(multiLineString) 
{
}

template<typename Point3_t> inline
MultiLineString3D<Point3_t> &MultiLineString3D<Point3_t>::operator = (const MultiLineString3D &multiLineString)
{
  if (this != &multiLineString) {
    Entity::operator = (multiLineString);
    Entities3D<LineString3D<Point3_t>>::operator = (multiLineString);
  }
  return *this;
}


} // Fin namespace Geometry

/*! \} */ // end of GeometricEntities

} // Fin namespace TL

#endif  // TL_GEOM_LINESTRING_H
