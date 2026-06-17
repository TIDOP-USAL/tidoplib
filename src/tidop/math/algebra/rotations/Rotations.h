/**************************************************************************
 *                                                                        *
 * Copyright (C) 2021 by Tidop Research Group                             *
 * Copyright (C) 2021 by Esteban Ruiz de OÃ±a Crespo                       *
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

#include <concepts>
#include <type_traits>

#include "tidop/math/algebra/rotations/Concepts.h" 
#include "tidop/math/algebra/rotations/detail/RotationConverter.h"

namespace tl
{

/*! \addtogroup Rotations
 *  \{
 */

/*!
 * \brief CRTP base class for orientation representations.
 *
 * The base provides type-safe conversions between orientation representations
 * without runtime polymorphism.
 * 
 * \tparam OrientationDerived The derived class representing a specific orientation type (e.g., AxisAngle, EulerAngles, etc.)
 * \tparam T The scalar type (e.g., float, double) used for the orientation representation.
 * \tparam P Additional template parameters that may be used by the derived classes.
 */
template<typename Derived>
class RotationBase
{

public:

    /*!
     * \brief Default constructor
     */
    constexpr RotationBase() = default;

    /*!
     * \brief Copy constructor
     */
    constexpr RotationBase(const RotationBase &) = default;

    /*!
     * \brief Move constructor
     */
    constexpr RotationBase(RotationBase &&) noexcept = default;
    ~RotationBase() = default;

    /*!
     * \brief Copy assignment operator
     */
    constexpr auto operator=(const RotationBase &) -> RotationBase & = default;

    /*!
     * \brief Move assignment operator
     */
    constexpr auto operator=(RotationBase &&) noexcept -> RotationBase & = default;

    /*!
     * \brief Conversion operator to another orientation type
     *
     * Converts this orientation to a different derived orientation type.
     *
     * \tparam Target The target derived orientation class to convert to.
     * \return The converted orientation object.
     */
    template<typename Target>
    [[nodiscard]] 
    operator Target() const
        requires (OrientationConvertible<Derived, Target>)
    {
        if constexpr (std::same_as<Derived, Target>) {
            return derived();
        } else {
            Target target;
            detail::convert(this->derived(), target);
            return target;
        }
    }

private:

    /*!
     * \brief Access the derived object
     *
     * Returns a reference to the derived object by casting this base class to the derived class.
     *
     * \return A reference to the derived orientation object.
     */
    [[nodiscard]] 
    constexpr auto derived() -> Derived &
    {
        return *static_cast<Derived *>(this);
    }

    /*!
     * \brief Access the derived object (const version)
     *
     * Returns a reference to the derived object by casting this base class to the derived class.
     *
     * \return A reference to the derived orientation object (const version).
     */
    [[nodiscard]] 
    constexpr auto derived() const -> const Derived &
    {
        return *static_cast<const Derived *>(this);
    }
};

/*! \} */

} // End namespace tl


