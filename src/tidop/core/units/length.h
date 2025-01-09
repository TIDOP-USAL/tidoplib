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
constexpr enableIfFloating<T,T> metre_to_feets = static_cast<T>(3.28084);
template<typename T>
constexpr enableIfFloating<T,T> foot_to_metres = static_cast<T>(0.3048);

/*!
 * \brief LengthConverter: A utility for converting between metric and imperial length units.
 *
 * This class supports a variety of units from both metric (e.g., metres, millimetres) and
 * imperial systems (e.g., feet, inches). Conversion is handled internally by mapping each
 * unit to a base unit (metres for metric, feet for imperial) and applying scaling factors.
 *
 * \h4 Example Usage </h4>
 *
 * \code
 * double lengthInFeet = LengthConverter::convert(100.0, LengthConverter::Units::metre, LengthConverter::Units::foot);
 * double lengthInKilometres = LengthConverter::convert(lengthInFeet, LengthConverter::Units::foot, LengthConverter::Units::kilometre);
 * \endcode
 */
class LengthConverter
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
     * \brief Enum for base length units
     */
    enum LengthUnits
    {
        metre = (0 << 0),        /*!< Base unit for metric */
        decimetre = (1 << 0),    /*!< 0.1 metres */
        centimetre = (1 << 2),   /*!< 0.01 metres */
        millimetre = (1 << 3),   /*!< 0.001 metres */
        micrometre = (1 << 4),   /*!< 1e-6 metres */
        nanometre = (1 << 5),    /*!< 1e-9 metres */
        kilometre = (1 << 6),    /*!< 1000 metres */
        hectometre = (1 << 7),   /*!< 100 metres */
        decametre = (1 << 8),    /*!< 10 metres */
        inch = (1 << 9),         /*!< 1/12 feet */
        foot = (1 << 10),        /*!< Base unit for imperial */
        yard = (1 << 11),        /*!< 3 feet */
        chain = (1 << 12),       /*!< 66 feet */
        furlong = (1 << 13),     /*!< 660 feet */
        mile = (1 << 14)         /*!< 5280 feet */
    };

public:

    /*!
     * \brief Enum for all supported units
     */
    enum class Units
    {
        metre = static_cast<int>(LengthUnits::metre) | System::si,
        decimetre = static_cast<int>(LengthUnits::decimetre) | System::si,
        centimetre = static_cast<int>(LengthUnits::centimetre) | System::si,
        millimetre = static_cast<int>(LengthUnits::millimetre) | System::si,
        micrometre = static_cast<int>(LengthUnits::micrometre) | System::si,
        nanometre = static_cast<int>(LengthUnits::nanometre) | System::si,
        kilometre = static_cast<int>(LengthUnits::kilometre) | System::si,
        hectometre = static_cast<int>(LengthUnits::hectometre) | System::si,
        decametre = static_cast<int>(LengthUnits::decametre) | System::si,
        inch = static_cast<int>(LengthUnits::inch) | System::imperial,
        foot = static_cast<int>(LengthUnits::foot) | System::imperial,
        yard = static_cast<int>(LengthUnits::yard) | System::imperial,
        chain = static_cast<int>(LengthUnits::chain) | System::imperial,
        furlong = static_cast<int>(LengthUnits::furlong) | System::imperial,
        mile = static_cast<int>(LengthUnits::mile) | System::imperial
    };

public:

    /*!
     * \brief Converts a length from one unit to another.
     * \param[in] length The length value to be converted.
     * \param[in] in The input unit.
     * \param[in] out The output unit.
     * \return The converted length value.
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
ALLOW_BITWISE_FLAG_OPERATIONS(LengthConverter::Units)

/*! \} */

}