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

#include "tidop/math/algebra/rotations/detail/RotationConverter.h"

namespace tl
{

/*! \addtogroup Rotations
 *  \{
 */

template<typename T>
class OrientationBase;

/// \endcond

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
template<
    template<typename, int... P>
    class OrientationDerived, typename T, int... P>
class OrientationBase<OrientationDerived<T, P...>>
{

public:

    using value_type = T;

public:

    /*!
     * \brief Default constructor
     */
    constexpr OrientationBase() = default;

    /*!
     * \brief Copy constructor
     */
    constexpr OrientationBase(const OrientationBase &) = default;

    /*!
     * \brief Move constructor
     */
    constexpr OrientationBase(OrientationBase &&) noexcept = default;
    ~OrientationBase() = default;

    /*!
     * \brief Move assignment operator
     */
    constexpr auto operator=(const OrientationBase &) -> OrientationBase & = default;

    /*!
     * \brief Move assignment operator
     */
    constexpr auto operator=(OrientationBase &&) noexcept -> OrientationBase & = default;

    /*!
     * \brief Conversion operator to another orientation type
     *
     * Converts this orientation to a different derived orientation type.
     *
     * \tparam OrientationDerived2 The target derived orientation class to convert to.
     * \return The converted orientation object.
     */
    template<
        template<typename>
        class OrientationDerived2>
    [[nodiscard]] 
    operator OrientationDerived2<T>() const
    {
        if constexpr (std::same_as<OrientationDerived<T, P...>, OrientationDerived2<T>>) {
            return derived();
        } else {
            OrientationDerived2<T> orientation;
            RotationConverter<T, P...>::convert(this->derived(), orientation);
            return orientation;
        }
    }

    /*!
     * \brief Conversion operator to another orientation type with additional template parameters
     *
     * Converts this orientation to a different derived orientation type with additional template parameters.
     *
     * \tparam OrientationDerived2 The target derived orientation class to convert to.
     * \tparam Q Additional template parameters for the target orientation class.
     * \return The converted orientation object.
     */
    template<
        template<typename, int... Q>
        class OrientationDerived2, int...Q>
    [[nodiscard]] operator OrientationDerived2<T, Q...>() const
    {
        if constexpr (std::same_as<OrientationDerived<T, P...>, OrientationDerived2<T, Q...>>) {
            return derived();
        } else {
            OrientationDerived2<T, Q...> orientation;
            RotationConverter<T, Q...>::convert(this->derived(), orientation);
            return orientation;
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
    constexpr auto derived() -> OrientationDerived<T, P...> &
    {
        return *static_cast<OrientationDerived<T, P...> *>(this);
    }

    /*!
     * \brief Access the derived object (const version)
     *
     * Returns a reference to the derived object by casting this base class to the derived class.
     *
     * \return A reference to the derived orientation object (const version).
     */
    [[nodiscard]] 
    constexpr auto derived() const -> const OrientationDerived<T, P...> &
    {
        return *static_cast<const OrientationDerived<T, P...> *>(this);
    }
};

/*! \} */

} // End namespace tl


