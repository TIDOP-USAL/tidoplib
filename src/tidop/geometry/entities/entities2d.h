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
#include "tidop/geometry/entities/window.h"

namespace tl
{


/*! \addtogroup GeometricEntities
 *  \{
 */


/*!
 * \brief Represents a container for 2D entities.
 * \tparam Entity_t The type of entity stored in the container.
 */
template<typename Entity_t>
class Entities2D
    : public EntityContainer<Entity_t>
{

public:

    using size_type = typename Entities2D<Entity_t>::size_type;

public:

    /*!
     * \brief Default constructor.
     */
    Entities2D();

    /*!
     * \brief Constructs a container with a specified number of elements.
     * \param[in] size Number of elements to allocate.
     */
    explicit Entities2D(size_type size);

    /*!
     * \brief Copy constructor.
     * \param[in] entities Another `Entities2D` object to copy.
     */
    Entities2D(const Entities2D &entities);

    /*!
     * \brief Move constructor.
     * \param[in] entities Another `Entities2D` object to move.
     */
    Entities2D(Entities2D &&entities) TL_NOEXCEPT;

    /*!
     * \brief Constructs a container from a vector of entities.
     * \param[in] entities A vector of entities to initialize the container.
     */
    explicit Entities2D(const std::vector<Entity_t> &entities);

    /*!
     * \brief Constructs a container from an initializer list of entities.
     * \param[in] entities A list of entities.
     */
    Entities2D(std::initializer_list<Entity_t> entities);

    ~Entities2D() override = default;

    /*!
     * \brief Retrieves the entities that are inside a given window.
     * \tparam Window_t The type representing the window.
     * \param[in] window The window to filter entities.
     * \return A vector containing the selected entities.
     */
    template<typename Window_t>
    auto entitiesInWindow(const Window_t& window) const -> std::vector<Entity_t>;

    /*!
     * \brief Copy assignment operator.
     * \param[in] entities Another `Entities2D` object to copy.
     * \return A reference to the assigned `Entities2D` object.
     */
    auto operator=(const Entities2D<Entity_t> &entities) -> Entities2D<Entity_t> &;

    /*!
     * \brief Move assignment operator.
     * \param[in] entities Another `Entities2D` object to move.
     * \return A reference to the assigned `Entities2D` object.
     */
    auto operator=(Entities2D<Entity_t> &&entities) TL_NOEXCEPT -> Entities2D<Entity_t> &;

};


template<typename Entity_t>
Entities2D<Entity_t>::Entities2D() 
  : EntityContainer<Entity_t>(0)
{
}

template<typename Entity_t>
Entities2D<Entity_t>::Entities2D(size_type size)
  : EntityContainer<Entity_t>(size) 
{
}

template<typename Entity_t>
Entities2D<Entity_t>::Entities2D(const Entities2D &entities)
  : EntityContainer<Entity_t>(entities)
{
}

template<typename Entity_t>
Entities2D<Entity_t>::Entities2D(Entities2D &&entities) TL_NOEXCEPT
  : EntityContainer<Entity_t>(std::forward<EntityContainer<Entity_t>>(entities))
{
}

template<typename Entity_t>
Entities2D<Entity_t>::Entities2D(const std::vector<Entity_t> &entities)
  : EntityContainer<Entity_t>(entities)
{
}

template<typename Entity_t>
Entities2D<Entity_t>::Entities2D(std::initializer_list<Entity_t> entities)
  : EntityContainer<Entity_t>(entities)
{
}

template<typename Entity_t> template<typename Window_t>
auto Entities2D<Entity_t>::entitiesInWindow(const Window_t& window) const -> std::vector<Entity_t>
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

template<typename Entity_t>
auto Entities2D<Entity_t>::operator=(const Entities2D<Entity_t>& entities) -> Entities2D<Entity_t>&
{
    if (this != &entities) {
        EntityContainer<Entity_t>::operator=(entities);
    }

    return (*this);
}

template<typename Entity_t>
auto Entities2D<Entity_t>::operator=(Entities2D<Entity_t>&& entities) TL_NOEXCEPT -> Entities2D<Entity_t>&
{
    if (this != &entities) {
        EntityContainer<Entity_t>::operator =(std::forward<EntityContainer<Entity_t>>(entities));
    }

    return (*this);
}


/*! \} */ // end of geometry

} // End namespace tl
