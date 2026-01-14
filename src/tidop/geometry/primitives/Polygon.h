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

#include <utility>

#include "tidop/geometry/base/Dimension.h"
#include "tidop/geometry/base/Geometry.h"
#include "tidop/geometry/base/Traits.h"
#include "tidop/geometry/base/EntityContainer.h"
#include "tidop/geometry/primitives/Point.h"
#include "tidop/geometry/spatial/BoundingBox.h"
#include "tidop/geometry/algorithms/spatial/Envelope.h"
#include "tidop/geometry/algorithms/measurement/Length.h"

namespace tl
{
	
/*! \addtogroup GeometricEntities
 *  \{
 */

template<typename Point_t>
class LinearRing
  : public EntityContainer<Point_t>
{

public:

    using EntityContainer<Point_t>::EntityContainer; // Heredamos constructores
    
public:

    bool isClosed() const
    {
        if (this->size() < 3) {
            return false;
        }
        return this->front() == this->back();
    }

    bool isValid() const 
    {
        return isClosed();
    }
};

using LinearRing2i = LinearRing<Point2<int>>;
using LinearRing2d = LinearRing<Point2<double>>;
using LinearRing2f = LinearRing<Point2<float>>;
using LinearRing3i = LinearRing<Point3<int>>;
using LinearRing3d = LinearRing<Point3<double>>;
using LinearRing3f = LinearRing<Point3<float>>;


/*!
 * \class Polygon
 * \brief A polygon defined by an outer ring and optional inner holes.
 * \tparam Point_t The point type (e.g., Point<double, Dimension::dim2>)
 */
template<typename Point_t>
class Polygon 
  : public GeometryBase<Polygon<Point_t>>
{

public:

    using value_type = Point_t;

private:

    LinearRing<Point_t> mOuter;
    std::vector<LinearRing<Point_t>> mInners;

public:

    // Constructores
    Polygon() = default;
    explicit Polygon(const std::vector<Point_t> &points) 
      : mOuter(points) {}

    Polygon(LinearRing<Point_t> outer, std::vector<LinearRing<Point_t>> inners)
      : mOuter(std::move(outer)),
        mInners(std::move(inners)) {}

    Polygon(std::initializer_list<Point_t> list) : mOuter(list) {}

    // Constructor de reserva
    explicit Polygon(size_t size) 
    {
        mOuter.reserve(size);
    }

    // Acceso a datos
    auto outer() const -> const LinearRing<Point_t> & { return mOuter; }
    auto outer() -> LinearRing<Point_t> & { return mOuter; }
    
    auto inners() const -> const std::vector<LinearRing<Point_t>> & { return mInners; }
    auto inners() -> std::vector<LinearRing<Point_t>> & { return mInners; }
    void addInner(const LinearRing<Point_t>& hole) { mInners.push_back(hole); }

    auto inner(std::size_t i) const -> const LinearRing<Point_t> & { return mInners[i]; }
    auto inner(std::size_t i) -> LinearRing<Point_t> & { return mInners[i]; }

    auto boundingBox() const 
	{
        return envelope(*this);
    }

    auto numInners() const -> size_t { return mInners.size(); }

    auto perimeter() const -> double
    {
        return length(*this);
    }

    //auto area() const -> double
    //{
    //    return tl::geometry::area(*this);
    //}
};

using Polygon2i = Polygon<Point2<int>>;
using Polygon2d = Polygon<Point2<double>>;
using Polygon2f = Polygon<Point2<float>>;
using Polygon3i = Polygon<Point3<int>>;
using Polygon3d = Polygon<Point3<double>>;
using Polygon3f = Polygon<Point3<float>>;


/*! \} */

} // End namespace tl