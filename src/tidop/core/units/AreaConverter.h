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

/*! \file AreaConverter.h
 * \brief Utility for converting between different area units.
 *
 * This file defines the `AreaConverter` class, which provides static methods
 * for converting area values between metric and imperial units. The conversion
 * is performed using a base unit (square metre) as the intermediate.
 *
 * \ingroup Utilities
 * \see tl::LengthConverter
 */

#pragma once

#include "tidop/core/base/Concepts.h"
#include "tidop/core/units/SI.h"
#include "tidop/core/units/Imperial.h"

namespace tl
{

/*! \addtogroup Units
 *  \{
 */

namespace detail
{

template<Floating T>
constexpr T square_metre_to_square_deca = static_cast<T>(si::square_metre / si::square_decametre);
template<Floating T>
constexpr T square_metre_from_square_deca = static_cast<T>(si::square_decametre);
template<Floating T>
constexpr T square_metre_to_square_hecto = static_cast<T>(si::square_metre / si::square_hectometre);
template<Floating T>
constexpr T square_metre_from_square_hecto = static_cast<T>(si::square_hectometre);
template<Floating T>
constexpr T square_metre_to_square_kilo = static_cast<T>(si::square_metre / si::square_kilometre);
template<Floating T>
constexpr T square_metre_from_square_kilo = static_cast<T>(si::square_kilometre);
template<Floating T>
constexpr T square_metre_from_acres = static_cast<T>(4046.8564224);
template<Floating T>
constexpr T square_metre_to_acres = T{1} / square_metre_from_acres<T>;
template<Floating T>
constexpr T square_metre_from_square_yard = static_cast<T>(0.83612736);
template<Floating T>
constexpr T square_metre_to_square_yard = T{1} / square_metre_from_square_yard<T>;
template<Floating T>
constexpr T square_metre_from_square_foot = static_cast<T>(0.09290304);
template<Floating T>
constexpr T square_metre_to_square_foot = T{1} / square_metre_from_square_foot<T>;

}

/*!
 * \enum AreaUnit
 * \brief Supported area units.
 */
enum class AreaUnit
{
    square_metre,       /*!< Square metre (m²). */
    square_decametre,   /*!< Square decametre (dam²). */
    square_hectometre,  /*!< Square hectometre (hm²). */
    square_kilometre,   /*!< Square kilometre (km²). */
    hectare,            /*!< Hectare (ha). */
    acre,               /*!< Acre. */
    square_yard,        /*!< Square yard (yd²). */
    square_foot         /*!< Square foot (ft²). */
};


/*!
 * \class AreaConverter
 * \brief Utility for converting between different area units.
 *
 * This class offers static helper functions to convert between metric and
 * imperial units of area. Internally, each unit encodes both its base unit
 * value and its system (metric or imperial), allowing fast conversion to and
 * from a common base unit (square metre).
 *
 * ### Example
 * \code
 * double hectares = AreaConverter::convert(1000.0, AreaUnit::square_metre, AreaUnit::hectare);
 * double acres = AreaConverter::convert(500.0, AreaUnit::square_metre, AreaUnit::acre);
 * \endcode
 *
 * \see LengthConverter
 */
class AreaConverter
{

public:

    /*!
     * \brief Converts a value from one area unit to another.
     * \tparam T Floating-point type (default: `double`).
     * \param[in] value The value to convert.
     * \param[in] from   The source unit.
     * \param[in] to     The target unit.
     * \return The converted value.
     */
    template<Floating T = double>
    [[nodiscard]]
    static constexpr auto convert(T value, AreaUnit from, AreaUnit to) noexcept -> T
    {
        if (from == to) [[unlikely]] {
            return value;
        }

        return value * (factorToBaseUnit<T>(from) / factorToBaseUnit<T>(to));
    }

private:

    /*!
     * \brief Returns the conversion factor from the given unit to square metres.
     * \tparam T Floating-point type.
     * \param[in] unit The unit to convert from.
     * \return The factor to multiply by to obtain square metres.
     */
    template<Floating T = double>
    [[nodiscard]]
    static constexpr auto factorToBaseUnit(AreaUnit unit) noexcept -> T
    {
        switch (unit) {
            // Métrico -> Base (m²)
            case AreaUnit::square_metre:
                return T{1.0};
            case AreaUnit::square_decametre: 
                return detail::square_metre_from_square_deca<T>;
            case AreaUnit::square_hectometre:
            case AreaUnit::hectare:
                return detail::square_metre_from_square_hecto<T>;
            case AreaUnit::square_kilometre: 
                return detail::square_metre_from_square_kilo<T>;
            // Imperial -> Base (m²)
            case AreaUnit::acre:
                return detail::square_metre_from_acres<T>;
            case AreaUnit::square_yard:
                return detail::square_metre_from_square_yard<T>;
            case AreaUnit::square_foot:
                return detail::square_metre_from_square_foot<T>;
        }

        return T{1.0};
    }
};

/*! \} */

}