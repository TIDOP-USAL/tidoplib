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

#ifndef TL_GEOMETRY_ENTITIES_2D_H
#define TL_GEOMETRY_ENTITIES_2D_H

#include "config_tl.h"

#include "tidop/core/defs.h"
#include "tidop/geometry/entities/entity.h"
#include "tidop/geometry/entities/window.h"

namespace tl
{


/*! \addtogroup geometry
 *  \{
 */


/*!
 * \brief Entidades 2D
 */
template<typename Entity_t>
class Entities2D
  : public EntityContainer<Entity_t>
{

public:

  /*!
   * \brief Constructora por defecto
   */
  Entities2D();

  /*!
   * \brief Constructor que reserva tamaño para n puntos
   * \param[in] size Tamaños que se reserva
   */
  Entities2D(typename Entities2D<Entity_t>::size_type size);

  /*!
   * \brief Constructor de copia
   * \param[in] entity Objeto Entities2D que se copia
   */
  Entities2D(const Entities2D &entity);

  /*!
   * \brief Constructor de movimiento
   * \param[in] entity Objeto Entities2D que se copia
   */
  Entities2D(Entities2D &&entity) TL_NOEXCEPT;

  /*!
   * \brief Constructor
   * \param[in] entities vector de entidades
   */
  Entities2D(const std::vector<Entity_t> &entities);

  /*!
   * \brief Constructor lista de inicialización
   * \param[in] entities Inicializador de lista con las entidades
   */
  Entities2D(std::initializer_list<Entity_t> entities);

  /*!
   * \brief Devuelve las entidades que están dentro de una ventana
   * \param[in] window Ventana
   * \return Entidades seleccionadas
   */
  template<typename Window_t>
  std::vector<Entity_t> entitiesInWindow(const Window_t &window) const;

  /*!
   * \brief Asignación de copia
   */
  Entities2D<Entity_t>& operator=(const Entities2D<Entity_t> &entity);

  /*!
   * \brief Asignación de movimiento
   */
  Entities2D<Entity_t>& operator=(Entities2D<Entity_t> &&entity) TL_NOEXCEPT;

};


template<typename Entity_t> inline
Entities2D<Entity_t>::Entities2D() 
  : EntityContainer<Entity_t>(0)
{
}

template<typename Entity_t> inline
Entities2D<Entity_t>::Entities2D(typename Entities2D<Entity_t>::size_type size)
  : EntityContainer<Entity_t>(size) 
{
}

template<typename Entity_t> inline
Entities2D<Entity_t>::Entities2D(const Entities2D &entities) 
  : EntityContainer<Entity_t>(entities) 
{
}

template<typename Entity_t> inline
Entities2D<Entity_t>::Entities2D(Entities2D &&entities) TL_NOEXCEPT
  : EntityContainer<Entity_t>(std::forward<EntityContainer<Entity_t>>(entities))
{
}

template<typename Entity_t> inline
Entities2D<Entity_t>::Entities2D(const std::vector<Entity_t> &entities)
  : EntityContainer<Entity_t>(entities)
{
}

template<typename Entity_t> inline
Entities2D<Entity_t>::Entities2D(std::initializer_list<Entity_t> entities)
  : EntityContainer<Entity_t>(entities)
{
}

template<typename Entity_t> template<typename Window_t> inline
std::vector<Entity_t> Entities2D<Entity_t>::entitiesInWindow(const Window_t &window) const
{
  std::vector<Entity_t> r_points(this->mEntities.size());
  size_t j = 0;
  for (size_t i = 0; i < this->mEntities.size(); i++) {
    if (window.containsPoint(this->mEntities[i])) {
      r_points[i] = this->mEntities[i];
      j++;
    }
  }
  r_points.resize(j);
  return r_points;
}

template<typename Entity_t> inline
Entities2D<Entity_t> &Entities2D<Entity_t>::operator=(const Entities2D<Entity_t> &entity)
{
  if (this != &entity) {
    EntityContainer<Entity_t>::operator=(entity);
  }
  return (*this);
}

template<typename Entity_t> inline
Entities2D<Entity_t> &Entities2D<Entity_t>::operator=(Entities2D<Entity_t> &&entity) TL_NOEXCEPT
{
  if (this != &entity) {
    EntityContainer<Entity_t>::operator =(std::forward<EntityContainer<Entity_t>>(entity));
  }
  return (*this);
}


/*! \} */ // end of geometry

} // End namespace tl

#endif // TL_GEOMETRY_ENTITIES_2D_H
