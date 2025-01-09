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

template<typename T>
constexpr enableIfFloating<T, T> square_metre_to_acres = static_cast<T>(1. / 4046.86);
template<typename T>
constexpr enableIfFloating<T, T> acre_to_square_metres = static_cast<T>(4046.86);


class AreaConverter
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


    enum AreaUnits
    {
        square_metre = (0 << 0),       /*!< Base unit for metric */
        square_decametre = (1 << 0),   /*!< 100 square meters */
        square_hectometre = (1 << 2),  /*!< 10000 square meters */
        square_kilometre = (1 << 3),   /*!< 1000000 square meters */
        acre = (1 << 4),               /*!< Base unit for imperial */
        square_yard = (1 << 5),        /*!< 0.8361 square meters */
        square_foot = (1 << 6),        /*!< 0.09290 square meters */
        hectare = square_hectometre    /*!< 10000 square meters */
    };

public:

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

    static auto convert(double length, Units in, Units out) -> double;
    static auto convertFactorToBaseUnit(Units unit) -> double;
    static auto convertFactorFromBaseUnit(Units unit) -> double;

};
ALLOW_BITWISE_FLAG_OPERATIONS(AreaConverter::Units)

}