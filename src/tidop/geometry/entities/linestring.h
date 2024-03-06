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
 * \brief LineString 2D
 */
template <typename Point_t>
class LineString
  : public Entity,
    public Entities2D<Point_t>
{

public:

    using size_type = typename LineString<Point_t>::size_type;

public:

    LineString();
    LineString(size_type size);
    LineString(const LineString &lineString);
    LineString(LineString &&lineString) TL_NOEXCEPT;
    LineString(const std::vector<Point_t> &points);
    LineString(std::initializer_list<Point_t> listPoints);

    ~LineString() override = default;

    /*!
     * \brief Polyline length
     * \return Length
     */
    auto length() const -> double;

    /*!
     * \brief Copy assignment operator
     */
    auto operator = (const LineString<Point_t> &lineString) -> LineString<Point_t> &;

    /*!
     * \brief Move assignment operator
     */
    auto operator = (LineString<Point_t> &&lineString) TL_NOEXCEPT -> LineString<Point_t> &;

    auto window() const -> Window<Point_t>;

};



using LineStringI = LineString<Point<int> >;
using LineStringF = LineString<Point<float> >;
using LineStringD = LineString<Point<double> >;



/* ---------------------------------------------------------------------------------- */


/*!
 * \brief 3D LineString
 *
 */
template <typename Point3_t>
class LineString3D
  : public Entity,
    public Entities3D<Point3_t>
{

public:

    using size_type = typename LineString3D<Point3_t>::size_type;

public:

    LineString3D();
    LineString3D(size_type size);
    LineString3D(const LineString3D &lineString);
    LineString3D(LineString3D &&lineString) TL_NOEXCEPT;
    LineString3D(const std::vector<Point3_t> &points);
    LineString3D(std::initializer_list<Point3_t> listPoints);

    ~LineString3D() override = default;

    /*!
     * \brief Polyline length
     * \return Length
     */
    auto length() const -> double;

    /*!
     * \brief Copy assignment operator
     */
    auto operator = (const LineString3D<Point3_t> &lineString) -> LineString3D<Point3_t> &;

    /*!
     * \brief Move assignment operator
     */
    auto operator = (LineString3D<Point3_t> &&lineString) TL_NOEXCEPT -> LineString3D<Point3_t> &;

    /*!
     * \brief Bounding box
     * \return Polyline bounding box
     */
    auto boundingBox() const -> BoundingBox<Point3_t>;
};



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

    using size_type = typename MultiLineString<Point_t>::size_type;

public:

    MultiLineString();
    MultiLineString(size_type size);
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




/* ---------------------------------------------------------------------------------- */


template <typename Point3_t>
class MultiLineString3D
  : public Entity,
    public Entities3D<LineString3D<Point3_t>>
{

public:

    using size_type = typename MultiLineString3D<Point3_t>::size_type;

public:

    MultiLineString3D();
    MultiLineString3D(size_type size);
    MultiLineString3D(const MultiLineString3D &multiLineString);
    MultiLineString3D(MultiLineString3D &&multiLineString) TL_NOEXCEPT;

    ~MultiLineString3D() override = default;

    /*!
     * \brief Copy assignment operator
     */
    auto operator = (const MultiLineString3D<Point3_t> &multiLineString) -> MultiLineString3D<Point3_t> &;

    auto operator = (MultiLineString3D<Point3_t> &&multiLineString) TL_NOEXCEPT -> MultiLineString3D<Point3_t> &;

    /*!
     * \brief Bounding box
     * \return Polyline bounding box
     */
    auto boundingBox() const -> BoundingBox<Point3_t>;

};





/* LineString implementation */

template <typename Point_t> inline
LineString<Point_t>::LineString()
  : Entity(Entity::Type::linestring2d), 
    Entities2D<Point_t>() 
{
}

template<typename Point_t> inline
LineString<Point_t>::LineString(size_type size)
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

template<typename Point_t>
auto LineString<Point_t>::length() const -> double
{
    double length = 0.;

    for (size_t i = 1; i < this->size(); i++) {
        length += distance(this->at(i - 1), this->at(i));
    }

    return length;
}

template<typename Point_t> inline
auto LineString<Point_t>::operator = (const LineString<Point_t> &lineString) -> LineString<Point_t> &
{
    if (this != &lineString) {
        Entity::operator = (lineString);
        Entities2D<Point_t>::operator = (lineString);
    }

    return *this;
}

template<typename Point_t> inline
auto LineString<Point_t>::operator = (LineString<Point_t> &&lineString) TL_NOEXCEPT -> LineString<Point_t> &
{
    if (this != &lineString) {
        Entity::operator = (std::forward<Entity>(lineString));
        Entities2D<Point_t>::operator = (std::forward<Entities2D<Point_t>>(lineString));
    }

    return *this;
}

template<typename Point_t>
auto LineString<Point_t>::window() const -> Window<Point_t> 
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



/* LineString3D implementation */


template <typename Point3_t> inline
LineString3D<Point3_t>::LineString3D()
  : Entity(Entity::Type::linestring3d), 
    Entities3D<Point3_t>() 
{
}

template<typename Point3_t> inline
LineString3D<Point3_t>::LineString3D(size_type size)
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

template<typename Point3_t>
auto LineString3D<Point3_t>::length() const -> double
{
    double length = 0.;

    for (size_t i = 1; i < this->size(); i++) {
        length += distance(this->at(i - 1), this->at(i));
    }

    return length;
}

template<typename Point3_t> inline
auto LineString3D<Point3_t>::operator = (const LineString3D &lineString) -> LineString3D<Point3_t> &
{
    if (this != &lineString) {
        Entity::operator = (lineString);
        Entities3D<Point3_t>::operator = (lineString);
    }

    return *this;
}

template<typename Point3_t> inline
auto LineString3D<Point3_t>::operator = (LineString3D &&lineString) TL_NOEXCEPT -> LineString3D<Point3_t> &
{
    if (this != &lineString) {
        Entity::operator = (std::forward<Entity>(lineString));
        Entities3D<Point3_t>::operator = (std::forward<Entities3D<Point3_t>>(lineString));
    }

    return *this;
}

template<typename Point3_t>
auto LineString3D<Point3_t>::boundingBox() const -> BoundingBox<Point3_t>
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




/* MultiLineString implementation */


template <typename Point_t>
MultiLineString<Point_t>::MultiLineString()
  : Entity(Entity::Type::multiline2d),
    Entities2D<LineString<Point_t>>()
{
}

template<typename Point_t> inline
MultiLineString<Point_t>::MultiLineString(size_type size)
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
auto MultiLineString<Point_t>::operator = (const MultiLineString &multiLineString) -> MultiLineString<Point_t> &
{
    if (this != &multiLineString) {
        Entity::operator = (multiLineString);
        Entities2D<LineString<Point_t>>::operator = (multiLineString);
    }

    return *this;
}

template<typename Point_t> inline
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
auto MultiLineString3D<Point3_t>::operator = (const MultiLineString3D &multiLineString) -> MultiLineString3D<Point3_t> &
{
    if (this != &multiLineString) {
        Entity::operator = (multiLineString);
        Entities3D<LineString3D<Point3_t>>::operator = (multiLineString);
    }

    return *this;
}

template<typename Point3_t> inline
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

/*! \} */ // end of geometry

} // Fin namespace tl
