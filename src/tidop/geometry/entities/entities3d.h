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

#pragma once

#include "tidop/geometry/entities/entity.h"

namespace tl
{


/*! \addtogroup geometry
 *  \{
 */


/*!
 * \brief 3D entities
 */
template<typename Entity_t>
class Entities3D
    : public EntityContainer<Entity_t>
{

public:

    using size_type = typename EntityContainer<Entity_t>::size_type;

public:

    Entities3D();
    explicit Entities3D(size_type size);
    Entities3D(const Entities3D &entities);
    Entities3D(Entities3D &&entities) TL_NOEXCEPT;
    explicit Entities3D(const std::vector<Entity_t> &entities);
    Entities3D(std::initializer_list<Entity_t> entities);

    ~Entities3D() override = default;

    /*!
     * \brief Copy assignment operator
     */
    auto operator=(const Entities3D<Entity_t>& entities) -> Entities3D<Entity_t>&;

    /*!
     * \brief Move assignment operator
     */
    auto operator=(Entities3D<Entity_t>&& entities) TL_NOEXCEPT -> Entities3D<Entity_t>&;

};


template<typename Entity_t>
Entities3D<Entity_t>::Entities3D() 
  : EntityContainer<Entity_t>() 
{
}

template<typename Entity_t>
Entities3D<Entity_t>::Entities3D(size_type size)
  : EntityContainer<Entity_t>(size) 
{
}

template<typename Entity_t>
Entities3D<Entity_t>::Entities3D(const Entities3D &entities) 
  : EntityContainer<Entity_t>(entities)
{
}

template<typename Entity_t>
Entities3D<Entity_t>::Entities3D(Entities3D &&entities) TL_NOEXCEPT
  : EntityContainer<Entity_t>(std::forward<EntityContainer<Entity_t>>(entities))
{
}

template<typename Entity_t>
Entities3D<Entity_t>::Entities3D(const std::vector<Entity_t> &entities)
  : EntityContainer<Entity_t>(entities)
{
}

template<typename Entity_t>
Entities3D<Entity_t>::Entities3D(std::initializer_list<Entity_t> entities)
  : EntityContainer<Entity_t>(entities)
{
}

template<typename Entity_t>
auto Entities3D<Entity_t>::operator=(const Entities3D<Entity_t>& entities) -> Entities3D<Entity_t>&
{
    if (this != &entities) {
        EntityContainer<Entity_t>::operator=(entities);
    }

    return (*this);
}

template<typename Entity_t>
auto Entities3D<Entity_t>::operator=(Entities3D<Entity_t>&& entities) TL_NOEXCEPT -> Entities3D<Entity_t>&
{
    if (this != &entities) {
        EntityContainer<Entity_t>::operator =(std::forward<EntityContainer<Entity_t>>(entities));
    }

    return (*this);
}


/*! \} */ // end of geometry

} // End namespace tl
