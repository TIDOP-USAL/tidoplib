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

#include "tidop/geometry/entities/polygon.h"


namespace tl
{

/*! \addtogroup GeometricEntities
 *  \{
 */

/*!
 * \brief Class representing a collection of 2D polygons.
 * \tparam Point_t Type representing a point in 2D space.
 *
 * The `MultiPolygon` class stores multiple `Polygon` objects, allowing operations
 * on a set of polygons, such as computing a bounding window.
 */
template <typename Point_t>
class MultiPolygon
  : public Entity,
    public Entities2D<Polygon<Point_t>>
{

public:

    using size_type = typename MultiPolygon<Point_t>::size_type;

public:

    /*!
     * \brief Default constructor. Creates an empty MultiPolygon.
     */
    MultiPolygon();

    /*!
     * \brief Constructor specifying the number of polygons.
     * \param[in] size Number of polygons in the collection.
     */
    explicit MultiPolygon(size_type size);

    /*!
     * \brief Copy constructor.
     * \param[in] multiPolygon MultiPolygon to copy.
     */
    MultiPolygon(const MultiPolygon &multiPolygon);

    /*!
     * \brief Move constructor.
     * \param[in] multiPolygon MultiPolygon to move.
     */
    MultiPolygon(MultiPolygon &&multiPolygon) TL_NOEXCEPT;

    /*!
     * \brief Destructor.
     */
    ~MultiPolygon() override = default;

    /*!
     * \brief Copy assignment operator.
     * \param[in] multiPolygon MultiPolygon to copy.
     * \return Reference to this MultiPolygon.
     */
    auto operator =(const MultiPolygon& multiPolygon) -> MultiPolygon<Point_t>&;

    /*!
     * \brief Move assignment operator.
     * \param[in] multiPolygon MultiPolygon to move.
     * \return Reference to this MultiPolygon.
     */
    auto operator =(MultiPolygon&& multiPolygon) TL_NOEXCEPT -> MultiPolygon<Point_t>&;
   
    /*!
     * \brief Computes the bounding window for all polygons in the collection.
     * \return A `Window<Point_t>` representing the bounding area.
     */
    auto window() const -> Window<Point_t>;
};





/*!
 * \brief Class representing a collection of 3D polygons.
 * \tparam Point3_t Type representing a point in 3D space.
 *
 * The `MultiPolygon3D` class stores multiple `Polygon3D` objects, enabling operations
 * on a set of 3D polygons, such as computing a bounding box.
 */
template <typename Point3_t>
class MultiPolygon3D
  : public Entity,
    public Entities3D<Polygon3D<Point3_t>>
{
public:

    using size_type = typename MultiPolygon3D<Point3_t>::size_type;

public:

    /*!
     * \brief Default constructor. Creates an empty MultiPolygon3D.
     */
    MultiPolygon3D();

    /*!
     * \brief Constructor specifying the number of 3D polygons.
     * \param[in] size Number of polygons in the collection.
     */
    MultiPolygon3D(size_type size);

    /*!
     * \brief Copy constructor.
     * \param[in] multiPolygon MultiPolygon3D to copy.
     */
    MultiPolygon3D(const MultiPolygon3D &multiPolygon);

    /*!
     * \brief Move constructor.
     * \param[in] multiPolygon MultiPolygon3D to move.
     */
    MultiPolygon3D(MultiPolygon3D &&multiPolygon) TL_NOEXCEPT;

    /*!
     * \brief Destructor.
     */
    ~MultiPolygon3D() override = default;

    /*!
     * \brief Copy assignment operator.
     * \param[in] multiPolygon MultiPolygon3D to copy.
     * \return Reference to this MultiPolygon3D.
     */
    auto operator =(const MultiPolygon3D &multiPolygon) -> MultiPolygon3D<Point3_t> &;

    /*!
     * \brief Move assignment operator.
     * \param[in] multiPolygon MultiPolygon3D to move.
     * \return Reference to this MultiPolygon3D.
     */
    auto operator =(MultiPolygon3D &&multiPolygon) TL_NOEXCEPT -> MultiPolygon3D<Point3_t> &;

    /*!
     * \brief Computes the bounding box for all 3D polygons in the collection.
     * \return A `BoundingBox<Point3_t>` representing the 3D bounding volume.
     */
    auto boundingBox() const -> BoundingBox<Point3_t>;
};






template <typename Point_t>
MultiPolygon<Point_t>::MultiPolygon()
  : Entity(Type::multipolygon2d),
    Entities2D<Polygon<Point_t>>()
{
}

template<typename Point_t>
MultiPolygon<Point_t>::MultiPolygon(size_type size)
  : Entity(Type::multipolygon2d),
    Entities2D<Polygon<Point_t>>(size) 
{
}

template<typename Point_t>
MultiPolygon<Point_t>::MultiPolygon(const MultiPolygon &multiPolygon) 
  : Entity(multiPolygon), 
    Entities2D<Polygon<Point_t>>(multiPolygon) 
{
}

template<typename Point_t>
MultiPolygon<Point_t>::MultiPolygon(MultiPolygon &&multiPolygon) TL_NOEXCEPT
  : Entity(std::forward<Entity>(multiPolygon)), 
    Entities2D<Polygon<Point_t>>(std::forward<Entities2D<Polygon<Point_t>>>(multiPolygon)) 
{
}

template<typename Point_t>
auto MultiPolygon<Point_t>::operator = (const MultiPolygon &multiPolygon) -> MultiPolygon<Point_t> &
{
    if (this != &multiPolygon) {
        Entity::operator = (multiPolygon);
        Entities2D<Polygon<Point_t>>::operator = (multiPolygon);
    }

    return *this;
}

template<typename Point_t>
auto MultiPolygon<Point_t>::operator = (MultiPolygon &&multiPolygon) TL_NOEXCEPT -> MultiPolygon<Point_t> &
{
    if (this != &multiPolygon) {
        Entity::operator = (std::forward<Entity>(multiPolygon));
        Entities2D<Polygon<Point_t>>::operator = (std::forward<Entities2D<Polygon<Point_t>>>(multiPolygon));
    }

    return *this;
}

template<typename Point_t>
auto MultiPolygon<Point_t>::window() const -> Window<Point_t>
{
    Window<Point_t> w;

    for (size_t i = 0; i < this->size(); i++) {
        w = joinWindow(w, this->at(i).window());
    }

    return w;
}




template <typename Point3_t>
MultiPolygon3D<Point3_t>::MultiPolygon3D()
  : Entity(Type::multipolygon3d),
    Entities3D<Polygon3D<Point3_t>>()
{
}

template<typename Point3_t>
MultiPolygon3D<Point3_t>::MultiPolygon3D(size_type size)
  : Entity(Type::multipolygon3d),
    Entities3D<Polygon3D<Point3_t>>(size) 
{
}

template<typename Point3_t>
MultiPolygon3D<Point3_t>::MultiPolygon3D(const MultiPolygon3D &multiPolygon) 
  : Entity(multiPolygon), 
    Entities3D<Polygon3D<Point3_t>>(multiPolygon) 
{
}

template<typename Point3_t>
MultiPolygon3D<Point3_t>::MultiPolygon3D(MultiPolygon3D &&multiPolygon) TL_NOEXCEPT
  : Entity(std::forward<Entity>(multiPolygon)), 
    Entities3D<Polygon3D<Point3_t>>(std::forward<MultiPolygon3D<Point3_t>>(multiPolygon)) 
{
}

template<typename Point3_t>
auto MultiPolygon3D<Point3_t>::operator = (const MultiPolygon3D &multiPolygon) -> MultiPolygon3D<Point3_t> &
{
    if (this != &multiPolygon) {
        Entity::operator = (multiPolygon);
        Entities3D<Polygon3D<Point3_t>>::operator = (multiPolygon);
    }

    return *this;
}

template<typename Point3_t>
auto MultiPolygon3D<Point3_t>::operator = (MultiPolygon3D &&multiPolygon) TL_NOEXCEPT -> MultiPolygon3D<Point3_t> &
{
    if (this != &multiPolygon) {
        Entity::operator = (std::forward<Entity>(multiPolygon));
        Entities3D<Polygon3D<Point3_t>>::operator = (std::forward<Entities3D<Polygon3D<Point3_t>>>(multiPolygon));
    }

    return *this;
}


template<typename Point3_t>
auto MultiPolygon3D<Point3_t>::boundingBox() const -> BoundingBox<Point3_t>
{
    BoundingBox<Point3_t> bounding_box;

    for (size_t i = 0; i < this->size(); i++) {
        bounding_box = joinBoundingBoxes(bounding_box, this->at(i).boundingBox());
    }

    return bounding_box;
}


/*! \} */ 

}
