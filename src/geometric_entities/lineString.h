#ifndef I3D_LINESTRING_H
#define I3D_LINESTRING_H

#include "opencv2/core/core.hpp"

#include "core/defs.h"
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
class LineString : public EntityPoints<Point_t>
{
public:

  typedef Point_t value_type;

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
  void add(const Point_t &point) override;

  /*!
   * \brief Longitud de la polilínea
   * \return Longitud
   */
  double length() const;

};

template <typename Point_t> inline
LineString<Point_t>::LineString()
  : EntityPoints<Point_t>(Entity::type::LINESTRING_2D) 
{
}

template<typename Point_t> inline
LineString<Point_t>::LineString(int size)
  : EntityPoints<Point_t>(Entity::type::LINESTRING_2D, size) 
{
}

template <typename Point_t> inline
LineString<Point_t>::LineString(const LineString &lineString)
  : EntityPoints<Point_t>(Entity::type::LINESTRING_2D, lineString)
{
}

template <typename Point_t> inline
LineString<Point_t>::LineString(const std::vector<Point_t> &points)
  : EntityPoints<Point_t>(Entity::type::LINESTRING_2D, points) 
{
}

template <typename Point_t> inline
LineString<Point_t>::LineString(std::initializer_list<Point_t> listPoints)
  : EntityPoints<Point_t>(Entity::type::LINESTRING_2D, listPoints) 
{
}

template<typename Point_t> inline
void LineString<Point_t>::add(const Point_t &point)
{
  this->mPoints.push_back(point);
}

template<typename Point_t> inline
double LineString<Point_t>::length()  const
{
  double length = 0.;
  for (size_t i = 1; i < this->mPoints.size(); i++) {
    length += distance(this->mPoints[i - 1], this->mPoints[i]);
  }
  return length;
}

typedef LineString<Point<int>> LineStringI;
typedef LineString<Point<float>> LineStringF;
typedef LineString<Point<double>> LineStringD;


/* ---------------------------------------------------------------------------------- */


template <typename Point_t>
class MultiLineString : public Entity
{
public:

  typedef Point_t value_type;

protected:

  /*!
   * \brief Conjunto de polilineas
   */
  std::vector<LineString<Point_t>> mLines;

public:

  MultiLineString();
  ~MultiLineString() {}


};

template <typename Point_t>
MultiLineString<Point_t>::MultiLineString()
  : Entity(Entity::type::MULTILINE_2D),
    mLines(0)
{
}


/* ---------------------------------------------------------------------------------- */


/*!
 * \brief Clase polilínea 3D
 *
 */
template <typename Point3_t>
class LineString3D : public Entity3DPoints<Point3_t>
{
public:

  typedef Point3_t value_type;

public:

  /*!
   * \brief Constructora por defecto
   * Crea un vector con 0 elementos
   */
  LineString3D();

  /*!
   * \brief Constructor que establece el tamaño de la polilínea
   */
  LineString3D(int size);

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
  void add(const Point3_t &point) override;

  /*!
   * \brief Longitud de la polilínea
   * \return Longitud
   */
  double length() const;

};

template <typename Point3_t> inline
LineString3D<Point3_t>::LineString3D()
  : Entity3DPoints<Point3_t>(Entity::type::LINESTRING_3D) 
{
}

template<typename Point3_t> inline
LineString3D<Point3_t>::LineString3D(int size)
  : Entity3DPoints<Point3_t>(Entity::type::LINESTRING_3D, size) 
{
}

template <typename Point3_t> inline
LineString3D<Point3_t>::LineString3D(const LineString3D &lineString)
  : Entity3DPoints<Point3_t>(Entity::type::LINESTRING_3D, lineString)
{
}

template <typename Point3_t> inline
LineString3D<Point3_t>::LineString3D(const std::vector<Point3_t> &points)
  : Entity3DPoints<Point3_t>(Entity::type::LINESTRING_3D, points) 
{
}

template <typename Point3_t> inline
LineString3D<Point3_t>::LineString3D(std::initializer_list<Point3_t> listPoints)
  : Entity3DPoints<Point3_t>(Entity::type::LINESTRING_3D, listPoints) 
{
}

template<typename Point3_t> inline
void LineString3D<Point3_t>::add(const Point3_t &point)
{
  this->mPoints.push_back(point);
}

template<typename Point3_t> inline
double LineString3D<Point3_t>::length()  const
{
  double length = 0.;
  for (size_t i = 1; i < this->mPoints.size(); i++) {
    length += distance(this->mPoints[i - 1], this->mPoints[i]);
  }
  return length;
}


/* ---------------------------------------------------------------------------------- */


template <typename Point3_t>
class MultiLineString3D : public Entity
{
public:

  typedef Point3_t value_type;

protected:

  /*!
   * \brief Conjunto de polilineas
   */
  std::vector<LineString<Point3_t>> mLines;

public:

  MultiLineString3D();
  ~MultiLineString3D() {}


};

template <typename Point3_t>
MultiLineString3D<Point3_t>::MultiLineString3D()
  : Entity(Entity::type::MULTILINE_3D),
    mLines(0)
{
}


} // Fin namespace Geometry

/*! \} */ // end of GeometricEntities

} // Fin namespace I3D

#endif  // I3D_LINESTRING_H
