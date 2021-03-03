/************************************************************************
 *                                                                      *
 * Copyright (C) 2020 by Tidop Research Group                           *
 *                                                                      *
 * This file is part of TidopLib                                        *
 *                                                                      *
 * TidopLib is free software: you can redistribute it and/or modify     *
 * it under the terms of the GNU General Public License as published by *
 * the Free Software Foundation, either version 3 of the License, or    *
 * (at your option) any later version.                                  *
 *                                                                      *
 * TidopLib is distributed in the hope that it will be useful,          *
 * but WITHOUT ANY WARRANTY; without even the implied warranty of       *
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the        *
 * GNU General Public License for more details.                         *
 *                                                                      *
 * You should have received a copy of the GNU General Public License    *
 * along with Foobar.  If not, see <http://www.gnu.org/licenses/>.      *
 *                                                                      *
 * @license GPL-3.0+ <http://spdx.org/licenses/GPL-3.0+>                *
 *                                                                      *
 ************************************************************************/

#ifndef TL_GEOMETRY_ENTITIES_3D_H
#define TL_GEOMETRY_ENTITIES_3D_H

#include "config_tl.h"

#include "tidop/core/defs.h"
#include "tidop/geometry/entities/entity.h"
#include "tidop/geometry/entities/point.h"
#include "tidop/geometry/entities/bbox.h"

namespace tl
{


/*! \addtogroup GeometricEntities Entidades geométricas
 *  Puntos, lineas, ...
 *  \{
 */


/*!
 * \brief Entidades 3D
 */
template<typename Entity_t>
class Entities3D
  : public EntityContainer<Entity_t>
{

public:

  /*!
   * \brief Constructora por defecto
   */
  Entities3D();

  /*!
   * \brief Constructor que reserva tamaño para n puntos
   * \param[in] size Tamaño que se reserva
   * \see entity_type
   */
  Entities3D(typename EntityContainer<Entity_t>::size_type size);

  /*!
   * \brief Constructor de copia
   * \param[in] entity Entidad que se copia
   * \see entity_type
   */
  Entities3D(const Entities3D &entity);
  
  /*!
   * \brief Constructor de movimiento
   * \param[in] entity Entidad que se copia
   * \see entity_type
   */
  Entities3D(Entities3D &&entity) TL_NOEXCEPT;

  /*!
   * \brief Constructor a partir de un vector de entidades
   * \param[in] entities Vector de entidades
   */
  Entities3D(const std::vector<Entity_t> &entities);

  /*!
   * \brief Constructor lista de inicialización
   * \param[in] entities Entidades
   */
  Entities3D(std::initializer_list<Entity_t> entities);
  
  ~Entities3D() override = default;

  /*!
   * \brief Devuelve los puntos que esta dentro de un cuadro delimitador
   * \param[in] bbox Cuadro delimitador
   * \return Puntos que entran dentro de la caja
   */
  //std::vector<Entity_t> entitiesInBoundingBox(const BoundingBox<Entity_t> &bbox) const;

  /*!
   * \brief Caja envolvente
   * \return Caja envolvente de los puntos
   */
  //virtual BoundingBox<Entity_t> boundingBox() const;

  /*!
   * \brief Asignación de copia
   */
  Entities3D<Entity_t>& operator=(const Entities3D<Entity_t> &entity);

  /*!
   * \brief Asignación de movimiento
   */
  Entities3D<Entity_t>& operator=(Entities3D<Entity_t> &&entity) TL_NOEXCEPT;

};


template<typename Entity_t> inline
Entities3D<Entity_t>::Entities3D() 
  : EntityContainer<Entity_t>() 
{
}

template<typename Entity_t> inline
Entities3D<Entity_t>::Entities3D(typename EntityContainer<Entity_t>::size_type size)
  : EntityContainer<Entity_t>(size) 
{
}

template<typename Entity_t> inline
Entities3D<Entity_t>::Entities3D(const Entities3D &entities) 
  : EntityContainer<Entity_t>(entities)
{
}

template<typename Entity_t> inline
Entities3D<Entity_t>::Entities3D(Entities3D &&entities) TL_NOEXCEPT
  : EntityContainer<Entity_t>(std::forward<EntityContainer<Entity_t>>(entities))
{
}

template<typename Entity_t> inline
Entities3D<Entity_t>::Entities3D(const std::vector<Entity_t> &entities)
  : EntityContainer<Entity_t>(entities)
{
}

template<typename Entity_t> inline
Entities3D<Entity_t>::Entities3D(std::initializer_list<Entity_t> entities)
  : EntityContainer<Entity_t>(entities)
{
}

//template<typename Entity_t> inline
//std::vector<Entity_t> Entities3D<Entity_t>::entitiesInBoundingBox(const BoundingBox<Entity_t> &bbox) const
//{
//  std::vector<Entity_t> r_points(this->mEntities.size());
//  size_t j = 0;
//  for (size_t i = 0; i < this->mEntities.size(); i++) {
//    if (bbox.containsPoint(this->mEntities[i])) {
//      r_points[i] = this->mEntities[i];
//      j++;
//    }
//  }
//  r_points.resize(j);
//  return r_points;
//}

//template<typename Entity_t> inline
//BoundingBox<Entity_t> Entities3D<Entity_t>::boundingBox() const
//{
//  BoundingBox<Entity_t> bounding_box;
//  for (size_t i = 0; i < this->mEntities.size(); i++) {
//    if (bounding_box.pt1.x > this->mEntities[i].x) bounding_box.pt1.x = this->mEntities[i].x;
//    if (bounding_box.pt1.y > this->mEntities[i].y) bounding_box.pt1.y = this->mEntities[i].y;
//    if (bounding_box.pt1.z > this->mEntities[i].z) bounding_box.pt1.z = this->mEntities[i].z;
//    if (bounding_box.pt2.x < this->mEntities[i].x) bounding_box.pt2.x = this->mEntities[i].x;
//    if (bounding_box.pt2.y < this->mEntities[i].y) bounding_box.pt2.y = this->mEntities[i].y;
//    if (bounding_box.pt2.z < this->mEntities[i].z) bounding_box.pt2.z = this->mEntities[i].z;
//  }
//  return bounding_box;
//}

template<typename Entity_t> inline
Entities3D<Entity_t> &Entities3D<Entity_t>::operator=(const Entities3D<Entity_t> &entity)
{
  if (this != &entity) {
    EntityContainer<Entity_t>::operator=(entity);
  }
  return (*this);
}

template<typename Entity_t> inline
Entities3D<Entity_t> &Entities3D<Entity_t>::operator=(Entities3D<Entity_t> &&entity) TL_NOEXCEPT
{
  if (this != &entity) {
    EntityContainer<Entity_t>::operator =(std::forward<EntityContainer<Entity_t>>(entity));
  }
  return (*this);
}


/*! \} */ // end of GeometricEntities

} // End namespace tl

#endif // TL_GEOMETRY_ENTITIES_3D_H
