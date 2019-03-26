/****************************************************************************
 *                                                                          *
 *  This file is part of TidopLib and can not be copied and/or distributed  *
 *  without the express permision of ITOS3D ENGINEERING S.L                 *
 *                                                                          *
 *  Contact: http://www.itos3d.com                                          *
 *           http://tidop.usal.es                                           *
 *                                                                          *
 *--------------------------------------------------------------------------*
 *                                                                          *
 *  Copyright (C) 2018, ITOS3D ENGINEERING S.L - All rights reserved        *
 *                                                                          *
 ****************************************************************************/


#ifndef TL_GEOM_LINESTRING_H
#define TL_GEOM_LINESTRING_H

#include "config_tl.h"

#include "tidop/core/defs.h"
#include "tidop/core/utils.h"
#include "tidop/geometry/entities/entity.h"
#include "tidop/geometry/entities/point.h"
#include "tidop/geometry/entities/window.h"
#include "tidop/geometry/operations.h"

namespace tl
{

/*! \addtogroup GeometricEntities
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
class LineString
  : public Entity,
    public Entities2D<Point_t>
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
  LineString(typename LineString<Point_t>::size_type size);

  /*!
   * \brief Constructor de copia
   * \param[in] lineString Objeto LineString que se copia
   */
  LineString(const LineString &lineString);

  /*!
   * \brief Constructor de movimiento
   * \param[in] lineString Objeto LineString que se mueve
   */
  LineString(LineString &&lineString) TL_NOEXCEPT;

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
   * \brief Longitud de la polilínea
   * \return Longitud
   */
  double length() const;

  /*!
   * \brief Operador de asignación
   */
  LineString<Point_t> &operator = (const LineString<Point_t> &lineString);
  
  /*!
   * \brief Operador de asignación de movimiento
   */
  LineString<Point_t> &operator = (LineString<Point_t> &&lineString) TL_NOEXCEPT;

  /*!
   * \brief Ventana envolvente
   * \return Ventana envolvente de los puntos
   */
#ifdef TL_ENABLE_DEPRECATED_METHODS
  TL_DEPRECATED("window()", "2.0")
  Window<Point_t> getWindow() const;
#endif
  Window<Point_t> window() const;
};

template <typename Point_t> inline
LineString<Point_t>::LineString()
  : Entity(Entity::Type::linestring2d), 
    Entities2D<Point_t>() 
{
}

template<typename Point_t> inline
LineString<Point_t>::LineString(typename LineString<Point_t>::size_type size)
  : Entity(Entity::Type::linestring2d), 
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
LineString<Point_t>::LineString(LineString &&lineString) TL_NOEXCEPT
  : Entity(std::forward<Entity>(lineString)),
    Entities2D<Point_t>(std::forward<LineString<Point_t>>(lineString))
{
}

template <typename Point_t> inline
LineString<Point_t>::LineString(const std::vector<Point_t> &points)
  : Entity(Entity::Type::linestring2d), 
    Entities2D<Point_t>(points) 
{
}

template <typename Point_t> inline
LineString<Point_t>::LineString(std::initializer_list<Point_t> listPoints)
  : Entity(Entity::Type::linestring2d), 
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
    this->mEntityType = lineString.mEntityType;
    this->mEntities = lineString.mEntities;
  }
  return *this;
}

template<typename Point_t> inline
LineString<Point_t> &LineString<Point_t>::operator = (LineString<Point_t> &&lineString) TL_NOEXCEPT
{
  if (this != &lineString) {
    this->mEntityType = std::move(lineString.mEntityType);
    this->mEntities = std::move(lineString.mEntities);
  }
  return *this;
}

#ifdef TL_ENABLE_DEPRECATED_METHODS
template<typename Point_t> inline
Window<Point_t> LineString<Point_t>::getWindow() const
{
  Window<Point_t> w;
  for (size_t i = 0; i < this->mEntities.size(); i++) {
    if (w.pt1.x > this->mEntities[i].x) w.pt1.x = this->mEntities[i].x;
    if (w.pt1.y > this->mEntities[i].y) w.pt1.y = this->mEntities[i].y;
    if (w.pt2.x < this->mEntities[i].x) w.pt2.x = this->mEntities[i].x;
    if (w.pt2.y < this->mEntities[i].y) w.pt2.y = this->mEntities[i].y;
  }
  return w;
}
#endif

template<typename Point_t> inline
Window<Point_t> LineString<Point_t>::window() const
{
  Window<Point_t> w;
  for (size_t i = 0; i < this->mEntities.size(); i++) {
    if (w.pt1.x > this->mEntities[i].x) w.pt1.x = this->mEntities[i].x;
    if (w.pt1.y > this->mEntities[i].y) w.pt1.y = this->mEntities[i].y;
    if (w.pt2.x < this->mEntities[i].x) w.pt2.x = this->mEntities[i].x;
    if (w.pt2.y < this->mEntities[i].y) w.pt2.y = this->mEntities[i].y;
  }
  return w;
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
class LineString3D
  : public Entity,
    public Entities3D<Point3_t>
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
  LineString3D(typename LineString3D<Point3_t>::size_type size);

  /*!
   * \brief Constructor de copia
   * \param[in] lineString Objeto LineString3D que se copia
   */
  LineString3D(const LineString3D &lineString);

  /*!
   * \brief Constructor de movimiento
   * \param[in] lineString Objeto LineString3D que se mueve
   */
  LineString3D(LineString3D &&lineString) TL_NOEXCEPT;

  /*!
   * \brief Constructor vector de puntos
   * \param[in] points Vector con los puntos de la polilinea
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
   * \brief Longitud de la polilínea
   * \return Longitud
   */
  double length() const;

  /*!
   * \brief Operador de asignación
   * \param[in] lineString Objeto que se asigna
   * \return lineString Objeto que se asigna
   */
  LineString3D<Point3_t> &operator = (const LineString3D<Point3_t> &lineString);

  /*!
   * \brief Operador de asignación de movimiento
   * \param[in] lineString Objeto que se mueve
   * \return
   */
  LineString3D<Point3_t> &operator = (LineString3D<Point3_t> &&lineString) TL_NOEXCEPT;


#ifdef TL_ENABLE_DEPRECATED_METHODS
  /*!
   * \brief Caja envolvente
   * \return Caja envolvente de la polilinea
   * \deprecated Use 'box()' en su lugar
   */
  TL_DEPRECATED("box()", "2.0")
  Box<Point3_t> getBox() const;
#endif

  /*!
   * \brief Caja envolvente
   * \return Caja envolvente de la polilinea
   */
  Box<Point3_t> box() const;
};

template <typename Point3_t> inline
LineString3D<Point3_t>::LineString3D()
  : Entity(Entity::Type::linestring3d), 
    Entities3D<Point3_t>() 
{
}

template<typename Point3_t> inline
LineString3D<Point3_t>::LineString3D(typename LineString3D<Point3_t>::size_type size)
  : Entity(Entity::Type::linestring3d), 
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
LineString3D<Point3_t>::LineString3D(LineString3D &&lineString) TL_NOEXCEPT
  : Entity(std::forward<Entity>(lineString)), 
    Entities3D<Point3_t>(std::forward<Entities3D<Point3_t>>(lineString))
{
}

template <typename Point3_t> inline
LineString3D<Point3_t>::LineString3D(const std::vector<Point3_t> &points)
  : Entity(Entity::Type::linestring3d), 
    Entities3D<Point3_t>(points) 
{
}

template <typename Point3_t> inline
LineString3D<Point3_t>::LineString3D(std::initializer_list<Point3_t> listPoints)
  : Entity(Entity::Type::linestring3d), 
    Entities3D<Point3_t>(listPoints) 
{
}

template<typename Point3_t> inline
double LineString3D<Point3_t>::length()  const
{
  double length = 0.;
  for (size_t i = 1; i < this->mEntities.size(); i++) {
    length += distance(this->mEntities[i - 1], this->mEntities[i]);
  }
  return length;
}

template<typename Point3_t> inline
LineString3D<Point3_t> &LineString3D<Point3_t>::operator = (const LineString3D &lineString)
{
  if (this != &lineString) {
    Entity::operator = (lineString);
    this->mEntities = lineString.mEntities;
    //Entities3D<Point3_t>::operator = (lineString);
  }
  return *this;
}

template<typename Point3_t> inline
LineString3D<Point3_t> &LineString3D<Point3_t>::operator = (LineString3D &&lineString) TL_NOEXCEPT
{
  if (this != &lineString) {
    this->mEntityType = std::move(lineString.mEntityType);
    //Entities3D<Point3_t>::operator = (lineString);
    this->mEntities = std::move(lineString.mEntities);
  }
  return *this;
}

#ifdef TL_ENABLE_DEPRECATED_METHODS
template<typename Point3_t> inline
Box<Point3_t> LineString3D<Point3_t>::getBox() const
{
  Box<Point3_t> box;
  for (size_t i = 0; i < this->mEntities.size(); i++) {
    if (box.pt1.x > this->mEntities[i].x) box.pt1.x = this->mEntities[i].x;
    if (box.pt1.y > this->mEntities[i].y) box.pt1.y = this->mEntities[i].y;
    if (box.pt1.z > this->mEntities[i].z) box.pt1.z = this->mEntities[i].z;
    if (box.pt2.x < this->mEntities[i].x) box.pt2.x = this->mEntities[i].x;
    if (box.pt2.y < this->mEntities[i].y) box.pt2.y = this->mEntities[i].y;
    if (box.pt2.z < this->mEntities[i].z) box.pt2.z = this->mEntities[i].z;
  }
  return box;
}
#endif

template<typename Point3_t> inline
Box<Point3_t> LineString3D<Point3_t>::box() const
{
  Box<Point3_t> box;
  for (size_t i = 0; i < this->mEntities.size(); i++) {
    if (box.pt1.x > this->mEntities[i].x) box.pt1.x = this->mEntities[i].x;
    if (box.pt1.y > this->mEntities[i].y) box.pt1.y = this->mEntities[i].y;
    if (box.pt1.z > this->mEntities[i].z) box.pt1.z = this->mEntities[i].z;
    if (box.pt2.x < this->mEntities[i].x) box.pt2.x = this->mEntities[i].x;
    if (box.pt2.y < this->mEntities[i].y) box.pt2.y = this->mEntities[i].y;
    if (box.pt2.z < this->mEntities[i].z) box.pt2.z = this->mEntities[i].z;
  }
  return box;
}


typedef LineString3D<Point3<int>> LineString3dI;
typedef LineString3D<Point3<double>> LineString3dD;
typedef LineString3D<Point3<float>> LineString3dF;

/* ---------------------------------------------------------------------------------- */

template <typename Point_t>
class MultiLineString
  : public Entity,
    public Entities2D<LineString<Point_t>>
{

public:

  /*!
   * \brief Constructora por defecto
   */
  MultiLineString();

  /*!
   * \brief Constructor que reserva tamaño para n polilineas
   */
  MultiLineString(typename MultiLineString<Point_t>::size_type size);

  /*!
   * \brief Constructor de copia
   * \param[in] multiLineString Objeto MultiLineString que se copia
   */
  MultiLineString(const MultiLineString &multiLineString);

  /*!
   * \brief Constructor de movimiento
   * \param[in] multiLineString Objeto MultiLineString que se mueve
   */
  MultiLineString( MultiLineString &&multiLineString) TL_NOEXCEPT;

  /*!
   * \brief Destructora
   */
  ~MultiLineString() {}

  /*!
   * \brief Operador de asignación
   * \param[in] multiLineString Objeto que se asigna
   * \return Referencia al objeto 
   */
  MultiLineString<Point_t> &operator = (const MultiLineString<Point_t> &multiLineString);

  /*!
   * \brief Operador de asignación de movimiento
   * \param[in] multiLineString Objeto que se mueve
   * \return Referencia al objeto
   */
  MultiLineString<Point_t> &operator = (MultiLineString<Point_t> &&multiLineString) TL_NOEXCEPT;

#ifdef TL_ENABLE_DEPRECATED_METHODS
  /*!
   * \brief Ventana envolvente
   * \return Ventana envolvente de los puntos
   * \deprecated Use 'window()' en su lugar
   */
  TL_DEPRECATED("window()", "2.0")
  Window<Point_t> getWindow() const;
#endif

  /*!
   * \brief Ventana envolvente
   * \return Ventana envolvente de los puntos
   */
  Window<Point_t> window() const;

};

template <typename Point_t>
MultiLineString<Point_t>::MultiLineString()
  : Entity(Entity::Type::multiline2d),
    Entities2D<LineString<Point_t>>()
{
}

template<typename Point_t> inline
MultiLineString<Point_t>::MultiLineString(typename MultiLineString<Point_t>::size_type size)
  : Entity(Entity::Type::multiline2d),
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
MultiLineString<Point_t>::MultiLineString(MultiLineString &&multiLineString) TL_NOEXCEPT
  : Entity(std::forward<Entity>(multiLineString)), 
    Entities2D<LineString<Point_t>>(std::forward<MultiLineString<Point_t>>(multiLineString)) 
{
}

template<typename Point_t> inline
MultiLineString<Point_t> &MultiLineString<Point_t>::operator = (const MultiLineString &multiLineString)
{
  if (this != &multiLineString) {
    Entity::operator = (multiLineString);
    //Entities2D<LineString<Point_t>>::operator = (multiLineString);
    this->mEntities = multiLineString.mEntities;
  }
  return *this;
}

template<typename Point_t> inline
MultiLineString<Point_t> &MultiLineString<Point_t>::operator = (MultiLineString &&multiLineString) TL_NOEXCEPT
{
  if (this != &multiLineString) {
    this->mEntityType = std::move(multiLineString.mEntityType);
    //Entities2D<LineString<Point_t>>::operator = (std::forward<MultiLineString<Point_t>>(multiLineString));
    this->mEntities = std::move(multiLineString.mEntities);
  }
  return *this;
}

#ifdef TL_ENABLE_DEPRECATED_METHODS
template<typename Point_t> inline
Window<Point_t> MultiLineString<Point_t>::getWindow() const
{
  Window<Point_t> w;
  for (size_t i = 0; i < this->mEntities.size(); i++) {
    w = joinWindow(w, this->mEntities[i].getWindow());
  }
  return w;
}
#endif

template<typename Point_t> inline
Window<Point_t> MultiLineString<Point_t>::window() const
{
  Window<Point_t> w;
  for (size_t i = 0; i < this->mEntities.size(); i++) {
    w = joinWindow(w, this->mEntities[i].getWindow());
  }
  return w;
}

/* ---------------------------------------------------------------------------------- */

template <typename Point3_t>
class MultiLineString3D
  : public Entity,
    public Entities3D<LineString3D<Point3_t>>
{

public:

  /*!
   * \brief Constructora por defecto
   */
  MultiLineString3D();

  /*!
   * \brief Constructor que reserva tamaño para n puntos
   */
  MultiLineString3D(typename MultiLineString3D<Point3_t>::size_type size);

  /*!
   * \brief Constructor de copia
   * \param[in] multiLineString Objeto MultiLineString que se copia
   */
  MultiLineString3D(const MultiLineString3D &multiLineString);
  
  /*!
   * \brief Constructor de movimiento
   * \param[in] multiLineString Objeto MultiLineString que se mueve
   */
  MultiLineString3D(MultiLineString3D &&multiLineString) TL_NOEXCEPT;
   
  /*!
   * \brief Destructora
   */
  ~MultiLineString3D() {}

  /*!
   * \brief Operador de asignación
   * \param[in] multiLineString Objeto que se asigna
   * \return Referencia al objeto
   */
  MultiLineString3D<Point3_t> &operator = (const MultiLineString3D<Point3_t> &multiLineString);
  
  /*!
   * \brief Operador de asignación de movimiento
   * \param[in] multiLineString Objeto que se mueve
   * \return Referencia al objeto
   */
  MultiLineString3D<Point3_t> &operator = (MultiLineString3D<Point3_t> &&multiLineString) TL_NOEXCEPT;

  /*!
   * \brief Caja envolvente
   * \return Caja envolvente de las polilineas
   */
  Box<Point3_t> box() const;
};

template <typename Point3_t>
MultiLineString3D<Point3_t>::MultiLineString3D()
  : Entity(Entity::Type::multiline3d),
    Entities3D<LineString3D<Point3_t>>()
{
}

template<typename Point3_t> inline
MultiLineString3D<Point3_t>::MultiLineString3D(typename MultiLineString3D<Point3_t>::size_type size)
  : Entity(Entity::Type::multiline3d),
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
MultiLineString3D<Point3_t>::MultiLineString3D(MultiLineString3D &&multiLineString) TL_NOEXCEPT
  : Entity(std::forward<Entity>(multiLineString)), 
    Entities3D<LineString3D<Point3_t>>(std::forward<MultiLineString3D<Point3_t>>(multiLineString)) 
{
}

template<typename Point3_t> inline
MultiLineString3D<Point3_t> &MultiLineString3D<Point3_t>::operator = (const MultiLineString3D &multiLineString)
{
  if (this != &multiLineString) {
    Entity::operator = (multiLineString);
    //Entities3D<LineString3D<Point3_t>>::operator = (multiLineString);
    this->mEntities = multiLineString.mEntities;
  }
  return *this;
}

template<typename Point3_t> inline
MultiLineString3D<Point3_t> &MultiLineString3D<Point3_t>::operator = (MultiLineString3D &&multiLineString) TL_NOEXCEPT
{
  if (this != &multiLineString) {
    this->mEntityType = std::move(multiLineString.mEntityType);
    //Entities3D<LineString3D<Point3_t>>::operator = (std::forward<Entities3D<LineString3D<Point3_t>>>(multiLineString));
    this->mEntities = std::move(multiLineString.mEntities);
  }
  return *this;
}

template<typename Point3_t> inline
Box<Point3_t> MultiLineString3D<Point3_t>::box() const
{
  Box<Point3_t> box;
  for (size_t i = 0; i < this->mEntities.size(); i++) {
    box = joinBox(box, this->mEntities[i].getBox());
  }
  return box;
}

} // Fin namespace Geometry

/*! \} */ // end of GeometricEntities

} // Fin namespace TL

#endif  // TL_GEOM_LINESTRING_H
