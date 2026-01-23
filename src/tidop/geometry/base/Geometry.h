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
 * \see tl::EntityContainer, tl::Dimension
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
 * \brief Abstract base class for geometric entities.
 *
 * Provides a common interface for all geometric types, including
 * type identification and dimension queries.
 */
class TL_EXPORT Geometry
{

public:

    /*! \brief Default constructor. */
    Geometry() = default;

    /*! \brief Virtual destructor. */
    virtual ~Geometry() = default;

    /*!
     * \brief Returns the geometry type.
     * \return Geometry type identifier.
     */
    virtual auto type() const noexcept -> GeometryType = 0;

    /*!
     * \brief Returns the dimension of the geometry.
     * \return Dimension of the geometry.
     */
    virtual auto dimension() const noexcept -> Dimension = 0;

    /*!
     * \brief Checks if the geometry is a collection (multi-type).
     * \return true if the geometry is a collection, false otherwise.
     */
    virtual auto isMulti() const noexcept -> bool { return false; }

};

/*!
 * \class GeometryBase
 * \brief CRTP base class for geometric entities.
 *
 * Provides static polymorphism and compile-time type information
 * for geometric entities using the Curiously Recurring Template Pattern.
 *
 * \tparam Derived The derived geometry class.
 */
template<typename Derived>
class GeometryBase
  : public Geometry
{

public:

    /*!
     * \brief Returns the geometry type (compile-time).
     * \return Geometry type identifier from traits.
     */
    constexpr auto type() const noexcept -> GeometryType final
    {
        return geometry_traits<Derived>::type;
    }

    /*!
     * \brief Returns the dimension of the geometry (compile-time).
     * \return Dimension from traits.
     */
    constexpr auto dimension() const noexcept -> Dimension final
    {
        return geometry_traits<Derived>::dimension;
    }

};

/*! \} */ 

} // End namespace tl
