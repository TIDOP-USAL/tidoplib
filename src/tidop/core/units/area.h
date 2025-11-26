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
#include "tidop/core/base/type.h"

namespace tl
{

/*! \addtogroup Units
 *  \{
 */

/*!
 * \brief Conversion factor from square metres to acres.
 *
 * This variable template provides the multiplicative factor required to convert
 * an area expressed in square metres to acres.
 *
 * \tparam T Floating-point type used for the conversion factor.
 */
template<typename T>
constexpr enableIfFloating<T, T> square_metre_to_acres = static_cast<T>(1. / 4046.86);

/*!
 * \brief Conversion factor from acres to square metres.
 *
 * This variable template provides the multiplicative factor required to convert
 * an area expressed in acres to square metres.
 *
 * \tparam T Floating-point type used for the conversion factor.
 */
template<typename T>
constexpr enableIfFloating<T, T> acre_to_square_metres = static_cast<T>(4046.86);


/*!
 * \brief Utility class providing conversions between different area units.
 *
 * This class offers static helper functions to convert between metric and
 * imperial units of area. Internally, each unit encodes both its base unit
 * value and its system (metric or imperial), allowing fast conversion to and
 * from a common base unit.
 */
class AreaConverter
{

private:

    /*!
     * \brief Unit system flags.
     *
     * These flags are bit-encoded and combined with area unit identifiers to
     * distinguish whether a unit belongs to the metric (SI) or imperial system.
     */
    enum System
    {
        si = (1 << 20),         /*!< Metric system */
        imperial = (1 << 21),   /*!< Imperial system */
    };

    /*!
     * \brief Internal identifiers for area units.
     *
     * Each value corresponds to a specific area unit. The encoded value also
     * implicitly defines its relative scale when converting to a base unit
     * (square metre for SI, acre for imperial).
     */
    enum AreaUnits
    {
        square_metre = (0 << 0),       /*!< Base unit for metric */
        square_decametre = (1 << 0),   /*!< 100 square meters */
        square_hectometre = (1 << 2),  /*!< 10,000 square meters */
        square_kilometre = (1 << 3),   /*!< 1,000,000 square meters */
        acre = (1 << 4),               /*!< Base unit for imperial */
        square_yard = (1 << 5),        /*!< 0.8361 square meters */
        square_foot = (1 << 6),        /*!< 0.09290 square meters */
        hectare = square_hectometre    /*!< Alias for 10,000 square metres */
    };

public:

    /*!
     * \brief Public enumeration of supported area units.
     *
     * Combines the internal unit identifier with the unit system flag so that
     * each enumerator fully describes both its scale and system.
     */
    enum class Units
    {
        square_metre = static_cast<int>(AreaUnits::square_metre) | System::si,
        square_decametre = static_cast<int>(AreaUnits::square_decametre) | System::si,
        square_hectometre = static_cast<int>(AreaUnits::square_hectometre) | System::si,
        square_kilometre = static_cast<int>(AreaUnits::square_kilometre) | System::si,
        acre = static_cast<int>(AreaUnits::acre) | System::imperial,
        square_yard = static_cast<int>(AreaUnits::square_yard) | System::imperial,
        square_foot = static_cast<int>(AreaUnits::square_foot) | System::imperial
    };

public:

    /*!
     * \brief Converts an area value from one unit to another.
     *
     * The conversion is performed by transforming the input value to the base
     * unit of its system (square metre for SI, acre for imperial) and then
     * converting it to the desired output unit.
     *
     * \param[in] length Area value to convert.
     * \param[in] in Input unit.
     * \param[in] out Output unit.
     *
     * \return Converted area in the requested unit.
     */
    static auto convert(double length, Units in, Units out) -> double;

    /*!
     * \brief Returns the multiplicative factor required to convert a given unit
     *        to its system's base unit.
     *
     * For SI units, the base unit is the square metre.
     * For imperial units, the base unit is the acre.
     *
     * \param[in] unit Unit whose factor to base must be computed.
     *
     * \return Factor used to convert _unit → base unit_.
     */
    static auto convertFactorToBaseUnit(Units unit) -> double;

    /*!
     * \brief Returns the multiplicative factor required to convert from the
     *        system’s base unit into a given unit.
     *
     * For SI units, the base unit is the square metre.
     * For imperial units, the base unit is the acre.
     *
     * \param[in] unit Output unit.
     *
     * \return Factor used to convert _base unit → unit_.
     */
    static auto convertFactorFromBaseUnit(Units unit) -> double;

};
ALLOW_BITWISE_FLAG_OPERATIONS(AreaConverter::Units)

/*! \} */

}