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

#include "tidop/config.h"

#include "tidop/geometry/base/Dimension.h"
#include "tidop/geometry/base/Traits.h"

namespace tl
{

namespace geometry
{
	
/*! \addtogroup GeometricEntities
 *  \{
 */


/*!
 * \brief Base class for geometric entities.
 */
class TL_EXPORT Geometry
{

public:


    Geometry() = default;
    virtual ~Geometry() = default;

    virtual auto type() const noexcept -> GeometryType = 0;
    virtual auto dimension() const noexcept -> Dimension = 0;

    virtual auto isMulti() const noexcept -> bool { return false; }

    //virtual void accept(GeometryVisitor &visitor) = 0;
};

/*!
 *\class GeometryBase
 *\brief CRTP base class for geometric entities.
 *
 *\tparam Derived The derived geometry class.
 *\tparam GT The geometry type.
 *\tparam D The dimension of the geometry.
*/
template<typename Derived>
class GeometryBase
  : public Geometry
{

    //static constexpr GeometryType type = geometry_traits<Derived>::type;
    //static constexpr Dimension _dimension = geometry_traits<Derived>::dimension;

public:

    constexpr auto type() const noexcept -> GeometryType final
    {
        return geometry_traits<Derived>::type;
    }

    constexpr auto dimension() const noexcept -> Dimension final
    {
        return geometry_traits<Derived>::dimension;
    }

    // Implementación genérica de accept usando CRTP
    //void accept(GeometryVisitor &visitor) override {
    //    visitor.visit(static_cast<Derived &>(*this));
    //}
};

/*! \} */ 

}

}
