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

/*! \file MassConverter.h
 * \brief Utility for converting between different mass units.
 *
 * This file defines the `MassConverter` class, which provides static methods
 * for converting mass values between metric and imperial units. The conversion
 * is performed using the gram as the base unit.
 *
 * \ingroup Utilities
 * \see tl::LengthConverter, tl::AreaConverter
 */

#pragma once


#include "tidop/core/base/Concepts.h"

namespace tl
{

/*! \addtogroup Units
 *  \{
 */

namespace detail
{

template<Floating T>
constexpr T gram_to_pounds = static_cast<T>(0.00220462);

template<Floating T>
constexpr T gram_to_grains = gram_to_pounds<double> * pound_to_grains<double>;

template<Floating T>
constexpr T gram_to_ounces = gram_to_pounds<double> * pound_to_ounces<double>;

template<Floating T>
constexpr T gram_to_stone = gram_to_pounds<double> * pound_to_stone<double>;

template<Floating T>
constexpr T gram_to_quarter = gram_to_pounds<double> * pound_to_quarter<double>;

template<Floating T>
constexpr T gram_to_hundredweights = gram_to_pounds<double> *pound_to_hundredweights<double>;

template<Floating T>
constexpr T gram_to_ton = gram_to_pounds<double> * pound_to_ton<double>;


template<Floating T>
constexpr T pound_to_grams = static_cast<T>(453.59237);

template<Floating T>
constexpr T grain_to_grams = grain_to_pounds<double> * pound_to_grams<double>;

template<Floating T>
constexpr T ounce_to_grams = ounce_to_pounds<double> * pound_to_grams<double>;

template<Floating T>
constexpr T stone_to_grams = stone_to_pounds<double> * pound_to_grams<double>;

template<Floating T>
constexpr T quarter_to_grams = quarter_to_pounds<double> * pound_to_grams<double>;

template<Floating T>
constexpr T hundredweight_to_grams = hundredweight_to_pounds<double> * pound_to_grams<double>;

template<Floating T>
constexpr T ton_to_grams = ton_to_pounds<double> * pound_to_grams<double>;

}


/*!
 * \enum MassUnit
 * \brief Supported mass units.
 */
enum class MassUnit
{
    gram,           /*!< Gram (g). */
    decigram,       /*!< Decigram (dg). */
    centigram,      /*!< Centigram (cg). */
    milligram,      /*!< Milligram (mg). */
    microgram,      /*!< Microgram (µg). */
    nanogram,       /*!< Nanogram (ng). */
    kilogram,       /*!< Kilogram (kg). */
    hectogram,      /*!< Hectogram (hg). */
    decagram,       /*!< Decagram (dag). */
    grain,          /*!< Grain (gr). */
    ounce,          /*!< Ounce (oz). */
    pound,          /*!< Pound (lb). */
    stone,          /*!< Stone (st). */
    quarter,        /*!< Quarter (qr). */
    hundredweight,  /*!< Hundredweight (cwt). */
    ton             /*!< Ton (t). */
};


/*!
 * \class MassConverter
 * \brief Utility for converting between different mass units.
 *
 * This class offers static helper functions to convert between metric and
 * imperial units of mass. Internally, each unit encodes its conversion
 * factor relative to the base unit (gram), allowing fast conversion to and
 * from the base unit.
 *
 * ### Example
 * \code
 * double kilograms = MassConverter::convert(1000.0, MassUnit::gram, MassUnit::kilogram);
 * double pounds = MassConverter::convert(1.0, MassUnit::kilogram, MassUnit::pound);
 * double ounces = MassConverter::convert(500.0, MassUnit::gram, MassUnit::ounce);
 * \endcode
 *
 * \see LengthConverter, AreaConverter
 */
class MassConverter
{

public:

    /*!
     * \brief Converts a value from one mass unit to another.
     * \tparam T Floating-point type (default: `double`).
     * \param[in] value The value to convert.
     * \param[in] from  The source unit.
     * \param[in] to    The target unit.
     * \return The converted value.
     */
    template<Floating T = double>
    [[nodiscard]]
    static auto convert(double value, MassUnit from, MassUnit to) -> T
    {
        if (from == to) [[unlikely]] {
            return value;
        }

        return value * factorToBaseUnit<T>(from) * factorFromBaseUnit<T>(to);
    }

    /*!
     * \brief Returns the conversion factor from the given unit to grams.
     * \tparam T Floating-point type.
     * \param[in] unit The unit to convert from.
     * \return The factor to multiply by to obtain grams.
     */
    template<Floating T = double>
    [[nodiscard]]
    static constexpr auto factorToBaseUnit(MassUnit unit) noexcept -> T
    {
        switch(unit) {
            case MassUnit::gram:
                return T{1.0};
            case MassUnit::decigram:
                return from_deci<double>;
            case MassUnit::centigram:
                return from_centi<double>;
            case MassUnit::milligram:
                return from_milli<double>;
            case MassUnit::microgram:
                return from_micro<double>;
            case MassUnit::nanogram:
                return from_nano<double>;
            case MassUnit::kilogram:
                return from_kilo<double>;
            case MassUnit::hectogram:
                return from_hecto<double>;
            case MassUnit::decagram:
                return from_deca<double>;
            case MassUnit::grain:
                return detail::grain_to_grams<double>;
            case MassUnit::ounce:
                return detail::ounce_to_grams<double>;
            case MassUnit::pound:
                return detail::pound_to_grams<double>;
            case MassUnit::stone:
                return detail::stone_to_grams<double>;
            case MassUnit::quarter:
                return detail::quarter_to_grams<double>;
            case MassUnit::hundredweight:
                return detail::hundredweight_to_grams<double>;
            case MassUnit::ton:
                return detail::ton_to_grams<double>;
            default:
                return T{1.0};
        }

    }

    /*!
     * \brief Returns the conversion factor from grams to the given unit.
     * \tparam T Floating-point type.
     * \param[in] unit The target unit.
     * \return The factor to multiply by to obtain the target unit.
     */
    template<Floating T = double>
    [[nodiscard]]
    static constexpr auto factorFromBaseUnit(MassUnit unit) noexcept -> T
    {
        switch (unit) {
        case MassUnit::gram:
            return T{1.0};
        case MassUnit::decigram:
            return to_deci<double>;
        case MassUnit::centigram:
            return to_centi<double>;
        case MassUnit::milligram:
            return to_milli<double>;
        case MassUnit::microgram:
            return to_micro<double>;
        case MassUnit::nanogram:
            return to_nano<double>;
        case MassUnit::kilogram:
            return to_kilo<double>;
        case MassUnit::hectogram:
            return to_hecto<double>;
        case MassUnit::decagram:
            return to_deca<double>;
        case MassUnit::grain:
            return detail::gram_to_grains<double>;
        case MassUnit::ounce:
            return detail::gram_to_ounces<double>;
        case MassUnit::pound:
            return detail::gram_to_pounds<double>;
        case MassUnit::stone:
            return detail::gram_to_stone<double>;
        case MassUnit::quarter:
            return detail::gram_to_quarter<double>;
        case MassUnit::hundredweight:
            return detail::gram_to_hundredweights<double>;
        case MassUnit::ton:
            return detail::gram_to_ton<double>;
        default:
            return T{1.0};
        }
    }

};

/*! \} */

}