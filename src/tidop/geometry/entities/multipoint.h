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
 * \brief Multi-point class
 *
 * This template represents a set of related points which are grouped 
 * together into a single multi-point entity.
 *
 */
template<typename Point_t>
class MultiPoint
  : public Entity,
    public Entities2D<Point_t>
{

public:

    using size_type = typename MultiPoint<Point_t>::size_type;
public:

    MultiPoint();
    MultiPoint(size_type size);
    MultiPoint(const MultiPoint &multiPoint);
    MultiPoint(MultiPoint &&multiPoint) TL_NOEXCEPT;
    MultiPoint(const std::vector<Point_t> &points);
    MultiPoint(std::initializer_list<Point_t> points);

    ~MultiPoint() override = default;

    /*!
     * \brief Copy assignment operator
     */
    auto operator = (const MultiPoint &multiPoint) -> MultiPoint<Point_t> &;

    /*!
     * \brief Move assignment operator
     */
    auto operator = (MultiPoint &&multiPoint) TL_NOEXCEPT -> MultiPoint<Point_t> &;

    /*!
     * \brief Ventana envolvente
     */
    auto window() const -> Window<Point_t>;

};


using MultiPointI = MultiPoint<Point<int> >;
using MultiPointD = MultiPoint<Point<double> >;
using MultiPointF = MultiPoint<Point<float> >;



/* ---------------------------------------------------------------------------------- */


/*!
 * \brief Multi-point 3D class
 *
 */
template<typename Point_t>
class MultiPoint3D
  : public Entity,
    public Entities3D<Point_t>
{

public:

    using size_type = typename MultiPoint3D<Point_t>::size_type;

public:

    MultiPoint3D();
    MultiPoint3D(size_type size);
    MultiPoint3D(const MultiPoint3D &multiPoint);
    MultiPoint3D(MultiPoint3D &&multiPoint) TL_NOEXCEPT;
    MultiPoint3D(const std::vector<Point_t> &vPoint);
    MultiPoint3D(std::initializer_list<Point_t> listPoints);

    ~MultiPoint3D() override = default;

    /*!
     * \brief Copy assignment operator
     */
    auto operator = (const MultiPoint3D &multiPoint) -> MultiPoint3D<Point_t> &;

    /*!
     * \brief Move assignment operator
     */
    auto operator = (MultiPoint3D &&multiPoint) TL_NOEXCEPT -> MultiPoint3D<Point_t> &;

    /*!
     * \brief Bounding Box
     */
    auto boundingBox() const -> BoundingBox<Point_t>;

};



/* MultiPoint implementation */

template<typename Point_t> inline
MultiPoint<Point_t>::MultiPoint() 
  : Entity(Entity::Type::multipoint2d),
    Entities2D<Point_t>() 
{
}

template<typename Point_t> inline
MultiPoint<Point_t>::MultiPoint(size_type size)
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
auto MultiPoint<Point_t>::operator = (const MultiPoint &multiPoint) -> MultiPoint<Point_t> &
{
    if (this != &multiPoint) {
        Entity::operator = (multiPoint);
        Entities2D<Point_t>::operator = (multiPoint);
    }

    return *this;
}

template<typename Point_t> inline
auto MultiPoint<Point_t>::operator = (MultiPoint &&multiPoint) TL_NOEXCEPT -> MultiPoint<Point_t> &
{
    if (this != &multiPoint) {
        Entity::operator = (std::forward<Entity>(multiPoint));
        Entities2D<Point_t>::operator = (std::forward<Entities2D<Point_t>>(multiPoint));
    }

    return *this;
}

template<typename Point_t>
auto MultiPoint<Point_t>::window() const -> Window<Point_t>
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



/* MultiPoint3D implementation */

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
auto MultiPoint3D<Point_t>::operator = (const MultiPoint3D &multiPoint) -> MultiPoint3D<Point_t> &
{
    if (this != &multiPoint) {
        Entity::operator=(multiPoint);
        Entities3D<Point_t>::operator = (multiPoint);
    }

    return *this;
}

template<typename Point_t> inline
auto MultiPoint3D<Point_t>::operator = (MultiPoint3D &&multiPoint) TL_NOEXCEPT -> MultiPoint3D<Point_t> &
{
    if (this != &multiPoint) {
        Entity::operator = (std::forward<Entity>(multiPoint));
        Entities3D<Point_t>::operator = (std::forward<Entities3D<Point_t>>(multiPoint));
    }

    return *this;
}

template<typename Point_t>
auto MultiPoint3D<Point_t>::boundingBox() const -> BoundingBox<Point_t>
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


/*! \} */ // end of geometry

} // End namespace tl
