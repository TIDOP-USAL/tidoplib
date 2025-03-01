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


#include "tidop/geometry/entities/linestring.h"

namespace tl
{

/*! \addtogroup GeometricEntities
 *  \{
 */


template <typename Point_t>
class MultiLineString
  : public Entity,
    public Entities2D<LineString<Point_t>>
{

public:

    using size_type = typename MultiLineString<Point_t>::size_type;

public:

    MultiLineString();
    explicit MultiLineString(size_type size);
    MultiLineString(const MultiLineString &multiLineString);
    MultiLineString(MultiLineString &&multiLineString) TL_NOEXCEPT;

    ~MultiLineString() override = default;

    /*!
     * \brief Copy assignment operator
     */
    auto operator = (const MultiLineString<Point_t> &multiLineString) -> MultiLineString<Point_t> &;

    /*!
     * \brief Move assignment operator
     */
    auto operator = (MultiLineString<Point_t> &&multiLineString) TL_NOEXCEPT -> MultiLineString<Point_t> &;

    auto window() const -> Window<Point_t>;

};





template <typename Point3_t>
class MultiLineString3D
  : public Entity,
    public Entities3D<LineString3D<Point3_t>>
{

public:

    using size_type = typename MultiLineString3D<Point3_t>::size_type;

public:

    MultiLineString3D();
    explicit MultiLineString3D(size_type size);
    MultiLineString3D(const MultiLineString3D &multiLineString);
    MultiLineString3D(MultiLineString3D &&multiLineString) TL_NOEXCEPT;

    ~MultiLineString3D() override = default;

    /*!
     * \brief Copy assignment operator
     */
    auto operator = (const MultiLineString3D<Point3_t> &multiLineString) -> MultiLineString3D<Point3_t> &;

    /*!
     * \brief Move assignment operator
     */
    auto operator = (MultiLineString3D<Point3_t> &&multiLineString) TL_NOEXCEPT -> MultiLineString3D<Point3_t> &;

    /*!
     * \brief Bounding box
     * \return Polyline bounding box
     */
    auto boundingBox() const -> BoundingBox<Point3_t>;

};






/* MultiLineString implementation */


template <typename Point_t>
MultiLineString<Point_t>::MultiLineString()
  : Entity(Type::multiline2d),
    Entities2D<LineString<Point_t>>()
{
}

template<typename Point_t>
MultiLineString<Point_t>::MultiLineString(size_type size)
  : Entity(Type::multiline2d),
    Entities2D<LineString<Point_t>>(size) 
{
}

template<typename Point_t>
MultiLineString<Point_t>::MultiLineString(const MultiLineString &multiLineString) 
  : Entity(multiLineString), 
    Entities2D<LineString<Point_t>>(multiLineString) 
{
}

template<typename Point_t>
MultiLineString<Point_t>::MultiLineString(MultiLineString &&multiLineString) TL_NOEXCEPT
  : Entity(std::forward<Entity>(multiLineString)), 
    Entities2D<LineString<Point_t>>(std::forward<Entities2D<LineString<Point_t>>>(multiLineString)) 
{
}

template<typename Point_t>
auto MultiLineString<Point_t>::operator = (const MultiLineString &multiLineString) -> MultiLineString<Point_t> &
{
    if (this != &multiLineString) {
        Entity::operator = (multiLineString);
        Entities2D<LineString<Point_t>>::operator = (multiLineString);
    }

    return *this;
}

template<typename Point_t>
auto MultiLineString<Point_t>::operator = (MultiLineString &&multiLineString) TL_NOEXCEPT -> MultiLineString<Point_t> &
{
    if (this != &multiLineString) {
        Entity::operator = (std::forward<Entity>(multiLineString));
        Entities2D<LineString<Point_t>>::operator = (std::forward<Entities2D<LineString<Point_t>>>(multiLineString));
    }

    return *this;
}

template<typename Point_t>
auto MultiLineString<Point_t>::window() const -> Window<Point_t>
{
    Window<Point_t> w;

    for (size_t i = 0; i < this->size(); i++) {
        w = joinWindow(w, this->at(i).window());
    }

    return w;
}




/* MultiLineString3D implementation */


template <typename Point3_t>
MultiLineString3D<Point3_t>::MultiLineString3D()
  : Entity(Type::multiline3d),
    Entities3D<LineString3D<Point3_t>>()
{
}

template<typename Point3_t>
MultiLineString3D<Point3_t>::MultiLineString3D(size_type size)
  : Entity(Type::multiline3d),
    Entities3D<LineString3D<Point3_t>>(size) 
{
}

template<typename Point3_t>
MultiLineString3D<Point3_t>::MultiLineString3D(const MultiLineString3D &multiLineString) 
  : Entity(multiLineString), 
    Entities3D<LineString3D<Point3_t>>(multiLineString) 
{
}

template<typename Point3_t>
MultiLineString3D<Point3_t>::MultiLineString3D(MultiLineString3D &&multiLineString) TL_NOEXCEPT
  : Entity(std::forward<Entity>(multiLineString)), 
    Entities3D<LineString3D<Point3_t>>(std::forward<Entities3D<LineString3D<Point3_t>>>(multiLineString)) 
{
}

template<typename Point3_t>
auto MultiLineString3D<Point3_t>::operator = (const MultiLineString3D &multiLineString) -> MultiLineString3D<Point3_t> &
{
    if (this != &multiLineString) {
        Entity::operator = (multiLineString);
        Entities3D<LineString3D<Point3_t>>::operator = (multiLineString);
    }

    return *this;
}

template<typename Point3_t>
auto MultiLineString3D<Point3_t>::operator = (MultiLineString3D &&multiLineString) TL_NOEXCEPT -> MultiLineString3D<Point3_t> &
{
    if (this != &multiLineString) {
        Entity::operator = (std::forward<Entity>(multiLineString));
        Entities3D<LineString3D<Point3_t>>::operator = (std::forward<Entities3D<LineString3D<Point3_t>>>(multiLineString));
    }

    return *this;
}

template<typename Point3_t>
auto MultiLineString3D<Point3_t>::boundingBox() const -> BoundingBox<Point3_t>
{
    BoundingBox<Point3_t> bounding_box;

    for (size_t i = 0; i < this->size(); i++) {
        bounding_box = joinBoundingBoxes(bounding_box, this->at(i).boundingBox());
    }

    return bounding_box;
}

/*! \} */

}
