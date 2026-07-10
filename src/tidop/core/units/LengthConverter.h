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

/*! \file LengthConverter.h
 * \brief Utility for converting between different length units.
 *
 * This file defines the `LengthConverter` class, which provides static methods
 * for converting length values between metric and imperial units. The conversion
 * is performed using the metre as the base unit.
 *
 * \ingroup Utilities
 * \see tl::AreaConverter
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
constexpr T metre_to_feet = T{3.28084};

template<Floating T>
constexpr T metre_to_inches = foot_to_inches<T> * metre_to_feet<T>;

template<Floating T>
constexpr T metre_to_yards = foot_to_yards<T> * metre_to_feet<T>;

template<Floating T>
constexpr T metre_to_chains = foot_to_chains<T> * metre_to_feet<T>;

template<Floating T>
constexpr T metre_to_furlongs = foot_to_furlongs<T> * metre_to_feet<T>;

template<Floating T>
constexpr T metre_to_miles = foot_to_mile<T> * metre_to_feet<T>;

template<Floating T>
constexpr T foot_to_metres = T{0.3048};

template<Floating T>
constexpr T inch_to_metres = inch_to_feet<double> * foot_to_metres<double>;

template<Floating T>
constexpr T yard_to_metres = yard_to_feet<double> * foot_to_metres<double>;

template<Floating T>
constexpr T chain_to_metres = chain_to_feet<double> * foot_to_metres<double>;

template<Floating T>
constexpr T furlong_to_metres = furlong_to_feet<double> * foot_to_metres<double>;

template<Floating T>
constexpr T mile_to_metres = mile_to_feet<double> * foot_to_metres<double>;

}

/*!
 * \enum LengthUnit
 * \brief Supported length units.
 */
enum class LengthUnit
{
    metre,          /*!< Metre (m). */
    decimetre,      /*!< Decimetre (dm). */
    centimetre,     /*!< Centimetre (cm). */
    millimetre,     /*!< Millimetre (mm). */
    micrometre,     /*!< Micrometre (µm). */
    nanometre,      /*!< Nanometre (nm). */
    kilometre,      /*!< Kilometre (km). */
    hectometre,     /*!< Hectometre (hm). */
    decametre,      /*!< Decametre (dam). */
    inch,           /*!< Inch (in). */
    foot,           /*!< Foot (ft). */
    yard,           /*!< Yard (yd). */
    chain,          /*!< Chain (ch). */
    furlong,        /*!< Furlong (fur). */
    mile            /*!< Mile (mi). */
};


/*!
 * \class LengthConverter
 * \brief Utility for converting between different length units.
 *
 * This class offers static helper functions to convert between metric and
 * imperial units of length. Internally, each unit encodes its conversion
 * factor relative to the base unit (metre), allowing fast conversion to and
 * from the base unit.
 *
 * ### Example
 * \code
 * double metres = LengthConverter::convert(100.0, LengthUnit::kilometre, LengthUnit::metre);
 * double inches = LengthConverter::convert(1.0, LengthUnit::metre, LengthUnit::inch);
 * \endcode
 *
 * \see AreaConverter
 */
class LengthConverter
{

public:

    /*!
     * \brief Converts a value from one length unit to another.
     * \tparam T Floating-point type (default: `double`).
     * \param[in] value The value to convert.
     * \param[in] from  The source unit.
     * \param[in] to    The target unit.
     * \return The converted value.
     */
    template<Floating T = double>
    [[nodiscard]]
    static auto convert(double value, LengthUnit from, LengthUnit to) -> T
    {
        if (from == to) [[unlikely]] {
            return value;
        }

        return value * (factorToBaseUnit<T>(from) * factorFromBaseUnit<T>(to));
    }

private:

    /*!
     * \brief Returns the conversion factor from the given unit to metres.
     * \tparam T Floating-point type.
     * \param[in] unit The unit to convert from.
     * \return The factor to multiply by to obtain metres.
     */
    template<Floating T = double>
    [[nodiscard]]
    static constexpr auto factorToBaseUnit(LengthUnit unit) noexcept -> T
    {
        switch (unit) {
            // Métrico -> Base (m²)
            case LengthUnit::metre:
                return T{1.0};
            case LengthUnit::decimetre:
                return from_deci<double>;
            case LengthUnit::centimetre:
                return from_centi<double>;
            case LengthUnit::millimetre:
                return from_milli<double>;
            case LengthUnit::micrometre:
                return from_micro<double>;
            case LengthUnit::nanometre:
                return from_nano<double>;
            case LengthUnit::kilometre:
                return from_kilo<double>;
            case LengthUnit::hectometre:
                return from_hecto<double>;
            case LengthUnit::decametre:
                return from_deca<double>;
                // Imperial -> Base (m²)
            case LengthUnit::inch:
                return detail::inch_to_metres<double>;
            case LengthUnit::foot:
                return detail::foot_to_metres<double>;
            case LengthUnit::yard:
                return detail::yard_to_metres<double>;
            case LengthUnit::chain:
                return detail::chain_to_metres<double>;
            case LengthUnit::furlong:
                return detail::furlong_to_metres<double>;
            case LengthUnit::mile:
                return detail::mile_to_metres<double>;
        }

        return T{1.0};
    }

    /*!
     * \brief Returns the conversion factor from metres to the given unit.
     * \tparam T Floating-point type.
     * \param[in] unit The target unit.
     * \return The factor to multiply by to obtain the target unit.
     */
    template<Floating T = double>
    [[nodiscard]]
    static constexpr auto factorFromBaseUnit(LengthUnit unit) noexcept -> T
    {
        switch (unit) {
            // Métrico -> Base (m²)
            case LengthUnit::metre:
                return T{1.0};
            case LengthUnit::decimetre:
                return to_deci<double>;
            case LengthUnit::centimetre:
                return to_centi<double>;
            case LengthUnit::millimetre:
                return to_milli<double>;
            case LengthUnit::micrometre:
                return to_micro<double>;
            case LengthUnit::nanometre:
                return to_nano<double>;
            case LengthUnit::kilometre:
                return to_kilo<double>;
            case LengthUnit::hectometre:
                return to_hecto<double>;
            case LengthUnit::decametre:
                return to_deca<double>;
                // Imperial -> Base (m²)
            case LengthUnit::inch:
                return detail::metre_to_inches<double>;
            case LengthUnit::foot:
                return detail::metre_to_feet<double>;
            case LengthUnit::yard:
                return detail::metre_to_yards<double>;
            case LengthUnit::chain:
                return detail::metre_to_chains<double>;
            case LengthUnit::furlong:
                return detail::metre_to_furlongs<double>;
            case LengthUnit::mile:
                return detail::metre_to_miles<double>;
        }

        return T{1.0};
    }

};

/*! \} */

}