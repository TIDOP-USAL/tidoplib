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

/*! \file Geometry.h
 * \brief Base classes for geometric entities.
 *
 * This file defines the base classes for all geometric entities in the library.
 * It provides both a polymorphic base class and a CRTP base class for static
 * polymorphism and compile-time type information.
 * ### Classes
 * - \ref tl::Geometry : Abstract base class for geometric entities.
 * - \ref tl::GeometryBase : CRTP base class for geometric entities.
 * \see tl::GeometryContainer, tl::Dimension
 */

#pragma once

#include "tidop/config.h"

#include "tidop/geometry/base/Dimension.h"
#include "tidop/geometry/base/Traits.h"


namespace tl
{
	
/*! \addtogroup Geometry
 *  \{
 */

/*!
 * \class Geometry
 * \brief CRTP base class for geometric entities.
 *
 * Provides static polymorphism and compile-time type information
 * for geometric entities using the Curiously Recurring Template Pattern.
 *
 * \tparam Derived The derived geometry class.
 */
template<typename Derived>
class Geometry
{

protected:

    Geometry() = default;
    ~Geometry() = default;

public:
    /*!
     * \brief Returns the geometry type.
     * \return Geometry type identifier.
     */
    constexpr auto type() const noexcept -> GeometryType
    {
        return geometry_traits<Derived>::type;
    }

    /*!
     * \brief Returns the dimension of the geometry.
     * \return Dimension of the geometry.
     */
    constexpr auto dimension() const noexcept -> Dimension
    {
        return geometry_traits<Derived>::dimension;
    }

    /*!
     * \brief Checks if the geometry is a collection (multi-type).
     * \return true if the geometry is a collection, false otherwise.
     */
    constexpr auto isMulti() const noexcept -> bool
    {
        return geometry_traits<Derived>::is_multi;
    }

    constexpr auto is2D() const noexcept -> bool
    {
        return is_2d_v<Derived>;
    }

    constexpr auto is3D() const noexcept -> bool
    {
        return is_3d_v<Derived>;
    }

    constexpr auto is4D() const noexcept -> bool
    {
        return is_4d_v<Derived>;
    }

    constexpr auto hasMeasure() const noexcept -> bool
    {
        return has_m_v<Derived>;
    }

    /*!
     * \brief Returns the minimum bounding box (envelope) of the geometry.
     * \return Axis-aligned bounding box containing the geometry.
     */
    auto envelope() const -> BoundingBox<typename geometry_traits<Derived>::point_type>;

    /*!
     * \brief Alias for envelope().
     * \return Same as envelope().
     */
    auto boundingBox() const -> BoundingBox<typename geometry_traits<Derived>::point_type>;

    //SRID
    // AsText
    // AsBinary
    // IsEmpty()
    // IsSimple()
    // Boundary()


private:

    auto derived() -> Derived &
    {
        return *static_cast<Derived *>(this);
    }

    auto derived() const -> const Derived &
    {
        return *static_cast<const Derived *>(this);
    }
};

/*! \} */ 

} // End namespace tl

#include "Geometry.impl.h"