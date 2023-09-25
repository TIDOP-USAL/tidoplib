/**************************************************************************
 *                                                                        *
 * Copyright (C) 2021 by Tidop Research Group                             *
 * Copyright (C) 2021 by Esteban Ruiz de Oña Crespo                       *
 *                                                                        *
 * This file is part of TidopLib                                          *
 *                                                                        *
 * TidopLib is free software: you can redistribute it and/or modify       *
 * it under the terms of the GNU Lesser General Public License as         *
 * published by the Free Software Foundation, either version 3 of the     *
 * License, or (at your option) any later version.                        *
 *                                                                        *
 * TidopLib is distributed in the hope that it will be useful,            *
 * but WITHOUT ANY WARRANTY; without even the implied warranty of         *
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the          *
 * GNU Lesser General Public License for more details.                    *
 *                                                                        *
 * You should have received a copy of the GNU Lesser General Public       *
 * License along with TidopLib. If not, see <http://www.gnu.org/licenses>.*
 *                                                                        *
 * @license LGPL-3.0 <https://www.gnu.org/licenses/lgpl-3.0.html>         *
 *                                                                        *
 **************************************************************************/

#ifndef TL_GEOMETRY_LINESTRING_H
#define TL_GEOMETRY_LINESTRING_H

#include "tidop/core/utils.h"
#include "tidop/geometry/entities/entities2d.h"
#include "tidop/geometry/entities/entities3d.h"
#include "tidop/geometry/entities/point.h"
#include "tidop/geometry/algorithms.h"

namespace tl
{

/*! \addtogroup geometry
 *  \{
 */


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
  LineString(size_t size);

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
  ~LineString() override = default;

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
  Window<Point_t> window() const;
  
};

template <typename Point_t> inline
LineString<Point_t>::LineString()
  : Entity(Entity::Type::linestring2d), 
    Entities2D<Point_t>() 
{
}

template<typename Point_t> inline
LineString<Point_t>::LineString(size_t size)
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
    Entities2D<Point_t>(std::forward<Entities2D<Point_t>>(lineString))
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

template<typename Point_t> inline
double LineString<Point_t>::length()  const
{
  double length = 0.;

  for (size_t i = 1; i < this->size(); i++) {
    length += distance(this->at(i - 1), this->at(i));
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

template<typename Point_t> inline
LineString<Point_t> &LineString<Point_t>::operator = (LineString<Point_t> &&lineString) TL_NOEXCEPT
{
  if (this != &lineString) {
    Entity::operator = (std::forward<Entity>(lineString));
    Entities2D<Point_t>::operator = (std::forward<Entities2D<Point_t>>(lineString));
  }

  return *this;
}

template<typename Point_t> inline
Window<Point_t> LineString<Point_t>::window() const
{
  Window<Point_t> w;

  for (size_t i = 0; i < this->size(); i++) {
    if (w.pt1.x > this->at(i).x) w.pt1.x = this->at(i).x;
    if (w.pt1.y > this->at(i).y) w.pt1.y = this->at(i).y;
    if (w.pt2.x < this->at(i).x) w.pt2.x = this->at(i).x;
    if (w.pt2.y < this->at(i).y) w.pt2.y = this->at(i).y;
  }

  return w;
}


using LineStringI = LineString<Point<int> >;
using LineStringF = LineString<Point<float> >;
using LineStringD = LineString<Point<double> >;

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
  ~LineString3D() override = default;

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

  /*!
   * \brief Caja envolvente
   * \return Caja envolvente de la polilinea
   */
  BoundingBox<Point3_t> boundingBox() const;
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

  for (size_t i = 1; i < this->size(); i++) {
    length += distance(this->at(i - 1), this->at(i));
  }

  return length;
}

template<typename Point3_t> inline
LineString3D<Point3_t> &LineString3D<Point3_t>::operator = (const LineString3D &lineString)
{
  if (this != &lineString) {
    Entity::operator = (lineString);
    Entities3D<Point3_t>::operator = (lineString);
  }

  return *this;
}

template<typename Point3_t> inline
LineString3D<Point3_t> &LineString3D<Point3_t>::operator = (LineString3D &&lineString) TL_NOEXCEPT
{
  if (this != &lineString) {
    Entity::operator = (std::forward<Entity>(lineString));
    Entities3D<Point3_t>::operator = (std::forward<Entities3D<Point3_t>>(lineString));
  }

  return *this;
}

template<typename Point3_t> inline
BoundingBox<Point3_t> LineString3D<Point3_t>::boundingBox() const
{
  BoundingBox<Point3_t> bounding_box;

  for (size_t i = 0; i < this->size(); i++) {
    if (bounding_box.pt1.x > this->at(i).x) bounding_box.pt1.x = this->at(i).x;
    if (bounding_box.pt1.y > this->at(i).y) bounding_box.pt1.y = this->at(i).y;
    if (bounding_box.pt1.z > this->at(i).z) bounding_box.pt1.z = this->at(i).z;
    if (bounding_box.pt2.x < this->at(i).x) bounding_box.pt2.x = this->at(i).x;
    if (bounding_box.pt2.y < this->at(i).y) bounding_box.pt2.y = this->at(i).y;
    if (bounding_box.pt2.z < this->at(i).z) bounding_box.pt2.z = this->at(i).z;
  }

  return bounding_box;
}


using LineString3dI = LineString3D<Point3<int>>;
using LineString3dD = LineString3D<Point3<double>>;
using LineString3dF = LineString3D<Point3<float>>;

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
  ~MultiLineString() override = default;

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
    Entities2D<LineString<Point_t>>(std::forward<Entities2D<LineString<Point_t>>>(multiLineString)) 
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

template<typename Point_t> inline
MultiLineString<Point_t> &MultiLineString<Point_t>::operator = (MultiLineString &&multiLineString) TL_NOEXCEPT
{
  if (this != &multiLineString) {
    Entity::operator = (std::forward<Entity>(multiLineString));
    Entities2D<LineString<Point_t>>::operator = (std::forward<Entities2D<LineString<Point_t>>>(multiLineString));
  }

  return *this;
}

template<typename Point_t> inline
Window<Point_t> MultiLineString<Point_t>::window() const
{
  Window<Point_t> w;

  for (size_t i = 0; i < this->size(); i++) {
    w = joinWindow(w, this->at(i).window());
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
  ~MultiLineString3D() override = default;

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
  BoundingBox<Point3_t> boundingBox() const;
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
    Entities3D<LineString3D<Point3_t>>(std::forward<Entities3D<LineString3D<Point3_t>>>(multiLineString)) 
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

template<typename Point3_t> inline
MultiLineString3D<Point3_t> &MultiLineString3D<Point3_t>::operator = (MultiLineString3D &&multiLineString) TL_NOEXCEPT
{
  if (this != &multiLineString) {
    Entity::operator = (std::forward<Entity>(multiLineString));
    Entities3D<LineString3D<Point3_t>>::operator = (std::forward<Entities3D<LineString3D<Point3_t>>>(multiLineString));
  }

  return *this;
}

template<typename Point3_t> inline
BoundingBox<Point3_t> MultiLineString3D<Point3_t>::boundingBox() const
{
  BoundingBox<Point3_t> bounding_box;

  for (size_t i = 0; i < this->size(); i++) {
    bounding_box = joinBoundingBoxes(bounding_box, this->at(i).boundingBox());
  }

  return bounding_box;
}

/*! \} */ // end of geometry

} // Fin namespace tl

#endif  // TL_GEOMETRY_LINESTRING_H
