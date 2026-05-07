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

#include "tidop/geometry/base/Concepts.h"
#include "tidop/geometry/base/Dimension.h"
#include "tidop/geometry/base/Traits.h"
//#include "tidop/geometry/spatial/BoundingBox.h"

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
    //static_assert(GeometryConcept<Derived>,
    //              "Derived must be a geometry type");

protected:

    Geometry() = default;
    ~Geometry() = default;

public:

    /*!
     * \brief Returns the geometry type.
     * \return Geometry type identifier.
     */
    [[nodiscard]]
    constexpr auto type() const noexcept -> GeometryType
    {
        return geometry_traits<Derived>::type;
    }

    /*!
     * \brief Returns the dimension of the geometry.
     * \return Dimension of the geometry.
     */
    [[nodiscard]]
    constexpr auto dimension() const noexcept -> Dimension
    {
        return geometry_traits<Derived>::dimension;
    }

    /*!
     * \brief Checks if the geometry is a collection (multi-type).
     * \return true if the geometry is a collection, false otherwise.
     */
    [[nodiscard]] 
    constexpr auto isMulti() const noexcept -> bool
    {
        return geometry_traits<Derived>::is_multi;
    }

    /*!
     * \brief Checks if the geometry is two-dimensional.
     *
     * Returns true if the geometry exists in 2D space (XY coordinates).
     * This corresponds to geometries using xy_tag or xym_tag point types.
     *
     * \return true if the geometry is 2D, false otherwise.
     * \see is3D(), is4D(), dimension()
     */
    [[nodiscard]]
    constexpr auto is2D() const noexcept -> bool
    {
        return is_2d_v<Derived>;
    }

    /*!
     * \brief Checks if the geometry is three-dimensional.
     *
     * Returns true if the geometry exists in 3D space (XYZ coordinates).
     * This corresponds to geometries using xyz_tag or xyzm_tag point types.
     *
     * \return true if the geometry is 3D, false otherwise.
     * \see is2D(), is4D(), dimension()
     */
    [[nodiscard]]
    constexpr auto is3D() const noexcept -> bool
    {
        return is_3d_v<Derived>;
    }

    /*!
     * \brief Checks if the geometry is four-dimensional.
     *
     * Returns true if the geometry exists in 4D space (XYZW coordinates).
     * This corresponds to geometries using xyzw_tag point types.
     * Note that four-dimensional geometries are not OGC-compliant.
     *
     * \return true if the geometry is 4D, false otherwise.
     * \see is2D(), is3D(), dimension()
     */
    [[nodiscard]] 
    constexpr auto is4D() const noexcept -> bool
    {
        return is_4d_v<Derived>;
    }

    /*!
     * \brief Checks if the geometry contains measure (M) coordinates.
     *
     * Returns true if the geometry includes an M (measure) coordinate
     * in addition to its spatial dimensions. This is independent of
     * the spatial dimensionality of the geometry.
     *
     * \return true if the geometry contains measure coordinates, false otherwise.
     * \see is2D(), is3D()
     */
    [[nodiscard]] 
    constexpr auto hasMeasure() const noexcept -> bool
    {
        return has_m_v<Derived>;
    }

    /*!
     * \brief Returns the minimum bounding box (envelope) of the geometry.
     * \return Axis-aligned bounding box containing the geometry.
     */
    //[[nodiscard]] 
    //auto envelope() const -> BoundingBox<typename geometry_traits<Derived>::point_type>;

    /*!
     * \brief Alias for envelope().
     * \return Same as envelope().
     */
    //[[nodiscard]] 
    //auto boundingBox() const -> BoundingBox<typename geometry_traits<Derived>::point_type>;

    //SRID
    // AsText
    // AsBinary
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

//#include "Geometry.impl.h"