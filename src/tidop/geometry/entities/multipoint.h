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

#ifndef TL_GEOMETRY_MULTI_POINT_H
#define TL_GEOMETRY_MULTI_POINT_H

#include <limits>
#include <numeric>
#include <array>

#include "tidop/geometry/entities/point.h"
#include "tidop/geometry/entities/entities2d.h"
#include "tidop/geometry/entities/entities3d.h"

namespace tl
{

/*! \addtogroup geometry
 *  \{
 */


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
template<typename Point_t>
class MultiPoint 
  : public Entity, 
    public Entities2D<Point_t>
{

public:

  /*!
   * \brief Constructora por defecto
   */
  MultiPoint();

  /*!
   * \brief Constructor que reserva tamaño para n puntos
   * \param[in] size Tamaño que se reserva para el contenedor
   */
  MultiPoint(typename MultiPoint<Point_t>::size_type size);

  /*!
   * \brief Constructor de copia
   * \param[in] multiPoint Objeto MultiPoint que se copia
   */
  MultiPoint(const MultiPoint &multiPoint);

  /*!
   * \brief Constructor de movimiento
   * \param[in] multiPoint Objeto MultiPoint que se mueve
   */
  MultiPoint(MultiPoint &&multiPoint) TL_NOEXCEPT;

  /*!
   * \brief Constructor
   * \param[in] vPoint vector de puntos
   */
  MultiPoint(const std::vector<Point_t> &vPoint);

  /*!
   * \brief Constructor lista de inicialización
   * \param[in] listPoints Inicializador de lista con los puntos
   */
  MultiPoint(std::initializer_list<Point_t> listPoints);

  /*!
   * \brief Destructora
   */
  ~MultiPoint() override = default;

  /*!
   * \brief Operador asignación
   * \param[in] multiPoint Objeto MultiPoint que se copia
   */
  MultiPoint<Point_t> &operator = (const MultiPoint &multiPoint);

  /*!
   * \brief Operador de asignación de movimiento
   * \param[in] multiPoint Objeto MultiPoint que se mueve
   */
  MultiPoint<Point_t> &operator = (MultiPoint &&multiPoint) TL_NOEXCEPT; 

  /*!
   * \brief Ventana envolvente
   * \return Ventana envolvente de los puntos
   */
  Window<Point_t> window() const;
};

template<typename Point_t> inline
MultiPoint<Point_t>::MultiPoint() 
  : Entity(Entity::Type::multipoint2d),
    Entities2D<Point_t>() 
{
}

template<typename Point_t> inline
MultiPoint<Point_t>::MultiPoint(typename MultiPoint<Point_t>::size_type size)
  : Entity(Entity::Type::multipoint2d),
    Entities2D<Point_t>(size) 
{
}

template<typename Point_t> inline
MultiPoint<Point_t>::MultiPoint(const MultiPoint &multiPoint) 
  : Entity(Entity::Type::multipoint2d),
    Entities2D<Point_t>(multiPoint) 
{
}

template<typename Point_t> inline
MultiPoint<Point_t>::MultiPoint(MultiPoint &&multiPoint) TL_NOEXCEPT
  : Entity(std::forward<Entity>(multiPoint)), 
    Entities2D<Point_t>(std::forward<Entities2D<Point_t>>(multiPoint)) 
{
}

template<typename Point_t> inline
MultiPoint<Point_t>::MultiPoint(const std::vector<Point_t> &vPoint) 
  : Entity(Entity::Type::multipoint2d),
    Entities2D<Point_t>(vPoint) 
{
}

template<typename Point_t> inline
MultiPoint<Point_t>::MultiPoint(std::initializer_list<Point_t> listPoints) 
  : Entity(Entity::Type::multipoint2d),
    Entities2D<Point_t>(listPoints)
{
}

template<typename Point_t> inline
MultiPoint<Point_t> &MultiPoint<Point_t>::operator = (const MultiPoint &multiPoint)
{
  if (this != &multiPoint) {
    Entity::operator = (multiPoint);
    Entities2D<Point_t>::operator = (multiPoint);
  }
  return *this;
}

template<typename Point_t> inline
MultiPoint<Point_t> &MultiPoint<Point_t>::operator = (MultiPoint &&multiPoint) TL_NOEXCEPT
{
  if (this != &multiPoint) {
    Entity::operator = (std::forward<Entity>(multiPoint));
    Entities2D<Point_t>::operator = (std::forward<Entities2D<Point_t>>(multiPoint));
  }
  return *this;
}

template<typename Point_t> inline
Window<Point_t> MultiPoint<Point_t>::window() const
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

using MultiPointI = MultiPoint<Point<int> >;
using MultiPointD = MultiPoint<Point<double> >;
using MultiPointF = MultiPoint<Point<float> >;

/* ---------------------------------------------------------------------------------- */

/*!
 * \brief Clase multi-punto 3D
 *
 * Esta template representa un conjunto de puntos relaccionados que se agrupan
 * en una misma entidad multipunto.
 *
 * Se han definido los siguientes alias para facilitar el acceso:
 * \code
 * typedef MultiPoint3D<int> MultiPoint3dI;
 * typedef MultiPoint3D<double> MultiPoint3dD;
 * typedef MultiPoint3D<float> MultiPoint3dF;
 * \endcode
 */
template<typename Point_t>
class MultiPoint3D
  : public Entity,
    public Entities3D<Point_t>
{
  
public:

  /*!
   * \brief Constructora por defecto
   */
  MultiPoint3D();

  /*!
   * \brief Constructor que reserva tamaño para n puntos
   * \param[in] size Tamaño que se reserva para el contenedor
   */
  MultiPoint3D(typename MultiPoint3D<Point_t>::size_type size);

  /*!
   * \brief Constructor de copia
   * \param[in] multiPoint Objeto MultiPoint que se copia
   */
  MultiPoint3D(const MultiPoint3D &multiPoint);

  /*!
   * \brief Constructor de movimiento
   * \param[in] multiPoint Objeto MultiPoint3D que se mueve
   */
  MultiPoint3D(MultiPoint3D &&multiPoint) TL_NOEXCEPT;

  /*!
   * \brief Constructor
   * \param[in] vPoint vector de puntos
   */
  MultiPoint3D(const std::vector<Point_t> &vPoint);

  /*!
   * \brief Constructor lista de inicialización
   * \param[in] listPoints Inicializador de lista con los puntos
   */
  MultiPoint3D(std::initializer_list<Point_t> listPoints);
  
  /*!
   * \brief Destructora
   */
  ~MultiPoint3D() override = default;

  /*!
   * \brief Operador asignación
   * \param[in] multiPoint Objeto MultiPoint3D que se copia
   */
  MultiPoint3D<Point_t> &operator = (const MultiPoint3D &multiPoint);

  /*!
   * \brief Operador de asignación de movimiento
   * \param[in] multiPoint Objeto MultiPoint3D que se mueve
   */
  MultiPoint3D<Point_t> &operator = (MultiPoint3D &&multiPoint) TL_NOEXCEPT;

  /*!
   * \brief Caja envolvente
   * \return Caja envolvente de los puntos
   */
  BoundingBox<Point_t> boundingBox() const;
};

template<typename Point_t> inline
MultiPoint3D<Point_t>::MultiPoint3D() 
  : Entity(Entity::Type::multipoint3d),
    Entities3D<Point_t>() 
{
}

template<typename Point_t> inline
MultiPoint3D<Point_t>::MultiPoint3D(typename MultiPoint3D<Point_t>::size_type size)
  : Entity(Entity::Type::multipoint3d),
    Entities3D<Point_t>(size) 
{
}

template<typename Point_t> inline
MultiPoint3D<Point_t>::MultiPoint3D(const MultiPoint3D &multiPoint) 
  : Entity(multiPoint),
    Entities3D<Point_t>(multiPoint) 
{
}

template<typename Point_t> inline
MultiPoint3D<Point_t>::MultiPoint3D(MultiPoint3D &&multiPoint) TL_NOEXCEPT
  : Entity(std::forward<Entity>(multiPoint)),
    Entities3D<Point_t>(std::forward<MultiPoint3D<Point_t>>(multiPoint)) 
{
}

template<typename Point_t> inline
MultiPoint3D<Point_t>::MultiPoint3D(const std::vector<Point_t> &vPoint) 
  : Entity(Entity::Type::multipoint3d),
    Entities3D<Point_t>(vPoint) 
{
}

template<typename Point_t> inline
MultiPoint3D<Point_t>::MultiPoint3D(std::initializer_list<Point_t> listPoints) 
  : Entity(Entity::Type::multipoint3d),
    Entities3D<Point_t>(listPoints)
{
}

template<typename Point_t> inline
MultiPoint3D<Point_t> &MultiPoint3D<Point_t>::operator = (const MultiPoint3D &multiPoint)
{
  if (this != &multiPoint) {
    Entity::operator=(multiPoint);
    Entities3D<Point_t>::operator = (multiPoint);
  }

  return *this;
}

template<typename Point_t> inline
MultiPoint3D<Point_t> &MultiPoint3D<Point_t>::operator = (MultiPoint3D &&multiPoint) TL_NOEXCEPT
{
  if (this != &multiPoint) {
    Entity::operator = (std::forward<Entity>(multiPoint));
    Entities3D<Point_t>::operator = (std::forward<Entities3D<Point_t>>(multiPoint));
  }

  return *this;
}

template<typename Point_t> inline
BoundingBox<Point_t> MultiPoint3D<Point_t>::boundingBox() const
{
  BoundingBox<Point_t> bounding_box;

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

using MultiPoint3dI = MultiPoint3D<Point3<int> >;
using MultiPoint3dD = MultiPoint3D<Point3<double> >;
using MultiPoint3dF = MultiPoint3D<Point3<float> >;

/* ---------------------------------------------------------------------------------- */



/*! \} */ // end of geometry

} // End namespace tl

#endif // TL_GEOMETRY_MULTI_POINT_H
