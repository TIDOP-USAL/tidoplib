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

/*! \file Polygon.h
 * \brief Polygon with outer ring and inner holes implementation.
 *
 * This file defines the Polygon and LinearRing class templates.
 * LinearRing represents a closed sequence of points forming a ring.
 * Polygon represents a polygon with one outer ring and zero or more inner rings (holes).
 * ### Classes
 * - \ref tl::LinearRing : Closed sequence of points forming a ring.
 * - \ref tl::Polygon : Polygon with outer and optional inner rings.
 * ### Type Aliases
 * - \ref tl::LinearRing2i, \ref tl::LinearRing2f, \ref tl::LinearRing2d : 2D integer, float, and double rings.
 * - \ref tl::LinearRing3i, \ref tl::LinearRing3f, \ref tl::LinearRing3d : 3D integer, float, and double rings.
 * - \ref tl::Polygon2i, \ref tl::Polygon2f, \ref tl::Polygon2d : 2D integer, float, and double polygons.
 * - \ref tl::Polygon3i, \ref tl::Polygon3f, \ref tl::Polygon3d : 3D integer, float, and double polygons.
 * \see tl::GeometryBase, tl::GeometryContainer, tl::Point
 */

#pragma once

#include <utility>

#include "tidop/geometry/base/Dimension.h"
#include "tidop/geometry/base/Geometry.h"
#include "tidop/geometry/base/Traits.h"
#include "tidop/geometry/base/GeometryContainer.h"
#include "tidop/geometry/primitives/Point.h"
#include "tidop/geometry/primitives/LinearRing.h"
#include "tidop/geometry/algorithms/measurement/Length.h"
#include "tidop/geometry/spatial/BoundingBox.h"

namespace tl
{
	
/*! \addtogroup Primitives
 *  \{
 */

/*!
 * \class Polygon
 * \brief A polygon defined by an outer ring and optional inner holes.
 *
 * \tparam Point_t The point type (e.g., Point<double, Dimension::dim2>).
 */
template<typename Point_t>
class Polygon 
  : public Geometry<Polygon<Point_t>>
{

public:

    /*! \brief Type of points stored in the polygon. */
    using value_type = Point_t;

private:

    LinearRing<Point_t> mOuter;                 /*!< Outer ring of the polygon. */
    std::vector<LinearRing<Point_t>> mInners;   /*!< Inner rings (holes) of the polygon. */

public:

    /*!
     * \brief Default constructor.
     */
    Polygon() = default;

    /*!
     * \brief Constructs a polygon from a vector of points (outer ring only).
     * \param[in] points Points forming the outer ring.
     */
    explicit Polygon(const std::vector<Point_t> &points);

    /*!
     * \brief Constructs a polygon with specified outer and inner rings.
     * \param[in] outer Outer ring of the polygon.
     * \param[in] inners Inner rings (holes) of the polygon.
     */
    Polygon(LinearRing<Point_t> outer, std::vector<LinearRing<Point_t>> inners = {});

    /*!
     * \brief Constructs a polygon from an initializer list (outer ring only).
     * \param[in] list Initializer list of points for the outer ring.
     */
    Polygon(std::initializer_list<Point_t> list);

    /*!
     * \brief Constructs a polygon with a pre-allocated outer ring size.
     * \param[in] size Number of points in the outer ring.
     */
    explicit Polygon(size_t size);

    /*!
     * \brief Returns a const reference to the outer ring.
     * \return Const reference to the outer ring.
     */
    auto outer() const -> const LinearRing<Point_t> &;

    /*!
     * \brief Returns a reference to the outer ring.
     * \return Reference to the outer ring.
     */
    auto outer() -> LinearRing<Point_t> &;
    
    /*!
     * \brief Returns a const reference to the vector of inner rings.
     * \return Const reference to the inner rings.
     */
    auto inners() const -> const std::vector<LinearRing<Point_t>> &;

    /*!
     * \brief Returns a reference to the vector of inner rings.
     * \return Reference to the inner rings.
     */
    auto inners() -> std::vector<LinearRing<Point_t>> &;

    /*!
     * \brief Adds an inner ring (hole) to the polygon.
     * \param[in] hole Inner ring to add.
     */
    void addInner(const LinearRing<Point_t> &hole);

    /*!
     * \brief Returns a const reference to a specific inner ring.
     * \param[in] i Index of the inner ring.
     * \return Const reference to the i-th inner ring.
     */
    auto inner(std::size_t i) const -> const LinearRing<Point_t> &;

    /*!
     * \brief Returns a reference to a specific inner ring.
     * \param[in] i Index of the inner ring.
     * \return Reference to the i-th inner ring.
     */
    auto inner(std::size_t i) -> LinearRing<Point_t> &;

    /*!
     * \brief Returns the number of inner rings.
     * \return Number of inner rings (holes).
     */
    auto numInners() const->size_t;

    /*!
     * \brief Computes the perimeter of the polygon.
     * \return Total perimeter (outer ring + inner rings) as a double.
     */
    auto perimeter() const -> double;

    auto isEmpty() const -> bool
    {
        return mOuter.isEmpty();
    }
};

// TYPE ALIASES FOR POLYGON

/*! \brief 2D polygon with integer coordinates. */
using Polygon2i = Polygon<Point2<int>>;

/*! \brief 2D polygon with double coordinates. */
using Polygon2d = Polygon<Point2<double>>;

/*! \brief 2D polygon with float coordinates. */
using Polygon2f = Polygon<Point2<float>>;

/*! \brief 3D polygon with integer coordinates. */
using Polygon3i = Polygon<Point3<int>>;

/*! \brief 3D polygon with double coordinates. */
using Polygon3d = Polygon<Point3<double>>;

/*! \brief 3D polygon with float coordinates. */
using Polygon3f = Polygon<Point3<float>>;


// METHOD IMPLEMENTATIONS

template<typename Point_t>
Polygon<Point_t>::Polygon(const std::vector<Point_t> &points)
    : mOuter(points) {
}

template<typename Point_t>
Polygon<Point_t>::Polygon(LinearRing<Point_t> outer,
                          std::vector<LinearRing<Point_t>> inners)
  : mOuter(std::move(outer)),
    mInners(std::move(inners)) {
}

template<typename Point_t>
Polygon<Point_t>::Polygon(std::initializer_list<Point_t> list) 
  : mOuter(list) 
{
}

template<typename Point_t>
Polygon<Point_t>::Polygon(size_t size)
{
    mOuter.reserve(size);
}

template<typename Point_t>
auto Polygon<Point_t>::outer() const -> const LinearRing<Point_t> &
{ 
    return mOuter;
}

template<typename Point_t>
auto Polygon<Point_t>::outer() -> LinearRing<Point_t> &
{ 
    return mOuter; 
}

template<typename Point_t>
auto Polygon<Point_t>::inners() const -> const std::vector<LinearRing<Point_t>> &
{ 
    return mInners; 
}

template<typename Point_t>
auto Polygon<Point_t>::inners() -> std::vector<LinearRing<Point_t>> &
{ 
    return mInners; 
}

template<typename Point_t>
void Polygon<Point_t>::addInner(const LinearRing<Point_t> &hole) 
{ 
    mInners.push_back(hole); 
}

template<typename Point_t>
auto Polygon<Point_t>::inner(std::size_t i) const -> const LinearRing<Point_t> &
{ 
    return mInners[i]; 
}

template<typename Point_t>
auto Polygon<Point_t>::inner(std::size_t i) -> LinearRing<Point_t> &
{ 
    return mInners[i];
}

template<typename Point_t>
auto Polygon<Point_t>::numInners() const -> size_t 
{
    return mInners.size();
}

template<typename Point_t>
auto Polygon<Point_t>::perimeter() const -> double
{
    return length(*this);
}


/*! \} */





} // End namespace tl