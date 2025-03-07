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


#include "tidop/core/base/flags.h"
#include "tidop/core/base/common.h"

namespace tl
{

/*! \addtogroup Units
 *  \{
 */

template<typename T>
constexpr enableIfFloating<T,T> gram_to_pounds = static_cast<T>(0.00220462);
template<typename T>
constexpr enableIfFloating<T,T> pound_to_grams = static_cast<T>(453.59237);

/*!
 * \brief MassConverter: A utility for converting between metric and imperial mass units.
 *
 * This class supports a wide range of mass units, including SI units (e.g., grams, kilograms)
 * and imperial units (e.g., pounds, ounces). Conversion is performed by mapping each unit to
 * a base unit (grams for SI, pounds for imperial) and applying predefined scaling factors.
 *
 * ### Example Usage
 * \code{.cpp}
 * double weightInPounds = MassConverter::convert(1000.0, MassConverter::Units::gram, MassConverter::Units::pound);
 * double weightInKilograms = MassConverter::convert(weightInPounds, MassConverter::Units::pound, MassConverter::Units::kilogram);
 * \endcode
 */
class MassConverter
{

private:

    /*!
     * \brief Enum for unit systems
     */
    enum System
    {
        si = (1 << 20),         /*!< Metric system */
        imperial = (1 << 21),   /*!< Imperial system */
    };

    /*!
     * \brief Enum for base mass units
     */
    enum MassUnits
    {
        gram = (0 << 0),         /*!< Base unit for metric */
        decigram = (1 << 0),     /*!< 0.1 grams */
        centigram = (1 << 2),    /*!< 0.01 grams */
        milligram = (1 << 3),    /*!< 0.001 grams */
        microgram = (1 << 4),    /*!< 1e-6 grams */
        nanogram = (1 << 5),     /*!< 1e-9 grams */
        kilogram = (1 << 6),     /*!< 1000 grams */
        hectogram = (1 << 7),    /*!< 100 grams */
        decagram = (1 << 8),     /*!< 10 grams */
        grain = (1 << 9),        /*!< 1/7000 pounds */
        ounce = (1 << 10),       /*!< 1/16 pounds */
        pound = (1 << 11),       /*!< Base unit for imperial */
        stone = (1 << 12),       /*!< 14 pounds */
        quarter = (1 << 13),     /*!< 28 pounds */
        hundredweight = (1 << 14), /*!< 112 pounds */
        ton = (1 << 15)          /*!< 2240 pounds (long ton) */
    };

public:

    /*!
     * \brief Enum for all supported units
     */
    enum class Units
    {
        gram = static_cast<int>(MassUnits::gram) | System::si,
        decigram = static_cast<int>(MassUnits::decigram) | System::si,
        centigram = static_cast<int>(MassUnits::centigram) | System::si,
        milligram = static_cast<int>(MassUnits::milligram) | System::si,
        microgram = static_cast<int>(MassUnits::microgram) | System::si,
        nanogram = static_cast<int>(MassUnits::nanogram) | System::si,
        kilogram = static_cast<int>(MassUnits::kilogram) | System::si,
        hectogram = static_cast<int>(MassUnits::hectogram) | System::si,
        decagram = static_cast<int>(MassUnits::decagram) | System::si,
        grain = static_cast<int>(MassUnits::grain) | System::imperial,
        ounce = static_cast<int>(MassUnits::ounce) | System::imperial,
        pound = static_cast<int>(MassUnits::pound) | System::imperial,
        stone = static_cast<int>(MassUnits::stone) | System::imperial,
        quarter = static_cast<int>(MassUnits::quarter) | System::imperial,
        hundredweight = static_cast<int>(MassUnits::hundredweight) | System::imperial,
        ton = static_cast<int>(MassUnits::ton) | System::imperial
    };

public:

    /*!
     * \brief Converts a mass from one unit to another.
     * \param[in] mass The mass value to be converted.
     * \param[in] in The input unit.
     * \param[in] out The output unit.
     * \return The converted mass value.
     */
    static auto convert(double length, Units in, Units out) -> double;

    /*!
     * \brief Converts a unit to its base unit scaling factor.
     * \param[in] unit The unit to convert.
     * \return The scaling factor to the base unit.
     */
    static auto convertFactorToBaseUnit(Units unit) -> double;

    /*!
     * \brief Converts a base unit to its output unit scaling factor.
     * \param[in] unit The target unit.
     * \return The scaling factor from the base unit.
     */
    static auto convertFactorFromBaseUnit(Units unit) -> double;

};
ALLOW_BITWISE_FLAG_OPERATIONS(MassConverter::Units)

/*! \} */

}