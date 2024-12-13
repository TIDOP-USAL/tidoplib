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


#include <fmt/core.h>

#include "tidop/core/flags.h"
#include "tidop/core/units/si.h"
#include "tidop/core/units/imperial.h"

namespace tl
{

template<typename T>
constexpr std::enable_if_t<std::is_floating_point<T>::value, T> to_square_deca = static_cast<T>(si::square_metre / si::square_decametre);
template<typename T>
constexpr std::enable_if_t<std::is_floating_point<T>::value, T> from_square_deca = static_cast<T>(si::square_decametre);
template<typename T>
constexpr std::enable_if_t<std::is_floating_point<T>::value, T> to_square_hecto = static_cast<T>(si::square_metre / si::square_hectometre);
template<typename T>
constexpr std::enable_if_t<std::is_floating_point<T>::value, T> from_square_hecto = static_cast<T>(si::square_hectometre);
template<typename T>
constexpr std::enable_if_t<std::is_floating_point<T>::value, T> to_square_kilo = static_cast<T>(si::square_metre / si::square_kilometre);
template<typename T>
constexpr std::enable_if_t<std::is_floating_point<T>::value, T> from_square_kilo = static_cast<T>(si::square_kilometre);

template<typename T>
constexpr std::enable_if_t<std::is_floating_point<T>::value, T> square_metre_to_acres = static_cast<T>(1. / 4046.86);
template<typename T>
constexpr std::enable_if_t<std::is_floating_point<T>::value, T> acre_to_square_metres = static_cast<T>(4046.86);


class AreaConverter
{

private:

    enum System
    {
        si = (1 << 20),
        imperial = (1 << 21),
    };

    enum AreaUnits
    {
        square_metre = (0 << 0),
        square_decametre = (1 << 0),
        square_hectometre = (1 << 2),
        square_kilometre = (1 << 3),
        acre = (1 << 4),
        square_yard = (1 << 5),
        square_foot = (1 << 6)
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

    static auto convert(double length, Units in, Units out) -> double
    {
        EnumFlags<Units> input_unit(in);
        EnumFlags<Units> output_unit(out);

        if (input_unit.flags() == output_unit.flags()) {
            return length;
        }

        double convert_factor = convertFactorToBaseUnit(in);

        if (input_unit.isEnabled(static_cast<Units>(System::si)) && 
            output_unit.isEnabled(static_cast<Units>(System::imperial))) {

            convert_factor *= square_metre_to_acres<double>;

        } else if (input_unit.isEnabled(static_cast<Units>(System::imperial)) &&
                   output_unit.isEnabled(static_cast<Units>(System::si))){

            convert_factor *= acre_to_square_metres<double>;

        }

        convert_factor *= convertFactorFromBaseUnit(out);

        return length * convert_factor;
    }

    static auto convertFactorToBaseUnit(Units unit) -> double
    {
        EnumFlags<Units> flags(unit);

        double convert_factor = 1.;

        if (flags.isEnabled(static_cast<Units>(System::si))) {

            switch (unit) {
            case Units::square_metre:
                convert_factor = 1.;
                break;
            case Units::square_decametre:
                convert_factor = from_deci<double>;
                break;
            case Units::square_hectometre:
                convert_factor = from_centi<double>;
                break;
            case Units::square_kilometre:
                convert_factor = from_milli<double>;
                break;
            default: 
                convert_factor = 1.;
            }

        } else if (flags.isEnabled(static_cast<Units>(System::imperial))) {

            switch (unit) {
            case Units::acre:
                convert_factor = 1.;
                break;
            case Units::square_yard:
                convert_factor = from_square_yard<double>;
                break;
            case Units::square_foot:
                convert_factor = from_square_foot<double>;
                break;
            default:
                convert_factor = 1.;
            }

        }

        return convert_factor;
    }

    static auto convertFactorFromBaseUnit(Units unit) -> double
    {
        EnumFlags<Units> flags(unit);

        double convert_factor = 1.;

        if (flags.isEnabled(static_cast<Units>(System::si))) {

            switch (unit) {
            case Units::square_metre:
                convert_factor = 1.;
                break;
            case Units::square_decametre:
                convert_factor = to_deci<double>;
                break;
            case Units::square_hectometre:
                convert_factor = to_centi<double>;
                break;
            case Units::square_kilometre:
                convert_factor = to_milli<double>;
                break;
            default:
                convert_factor = 1.;
            }

        } else if (flags.isEnabled(static_cast<Units>(System::imperial))) {

            switch (unit) {
            case Units::acre:
                convert_factor = 1.;
                break;
            case Units::square_yard:
                convert_factor = to_square_yard<double>;
                break;
            case Units::square_foot:
                convert_factor = to_square_foot<double>;
                break;
            default:
                convert_factor = 1.;
            }

        }

        return convert_factor;
    }

};
ALLOW_BITWISE_FLAG_OPERATIONS(AreaConverter::Units)

}