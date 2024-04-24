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


#include "tidop/core/flags.h"
#include "tidop/core/units/si.h"
#include "tidop/core/units/imperial.h"

namespace tl
{

template<typename T>
constexpr std::enable_if_t<std::is_floating_point<T>::value, T> gram_to_pounds = static_cast<T>(0.00220462);
template<typename T>
constexpr std::enable_if_t<std::is_floating_point<T>::value, T> pound_to_grams = static_cast<T>(453.59237);


class MassConverter
{

private:

    enum System
    {
        si = (1 << 20),
        imperial = (1 << 21),
    };

    enum MassUnits
    {
        gram = (0 << 0),
        decigram = (1 << 0),
        centigram = (1 << 2),
        milligram = (1 << 3),
        microgram = (1 << 4),
        nanogram = (1 << 5),
        kilogram = (1 << 6),
        hectogram = (1 << 7),
        decagram = (1 << 8),
        grain = (1 << 9),
        ounce = (1 << 10),
        pound = (1 << 11),
        stone = (1 << 12),
        quarter = (1 << 13),
        hundredweight = (1 << 14),
        ton = (1 << 15)
    };

public:

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

            convert_factor *= gram_to_pounds<double>;

        } else if (input_unit.isEnabled(static_cast<Units>(System::imperial)) &&
                   output_unit.isEnabled(static_cast<Units>(System::si))){

            convert_factor *= pound_to_grams<double>;

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
            case Units::gram:
                convert_factor = 1.;
                break;
            case Units::decigram:
                convert_factor = from_deci<double>;
                break;
            case Units::centigram:
                convert_factor = from_centi<double>;
                break;
            case Units::milligram:
                convert_factor = from_milli<double>;
                break;
            case Units::microgram:
                convert_factor = from_micro<double>;
                break;
            case Units::nanogram:
                convert_factor = from_nano<double>;
                break;
            case Units::kilogram:
                convert_factor = from_kilo<double>;
                break;
            case Units::hectogram:
                convert_factor = from_hecto<double>;
                break;
            case Units::decagram:
                convert_factor = from_deca<double>;
                break;
            default: 
                convert_factor = 1.;
            }

        } else if (flags.isEnabled(static_cast<Units>(System::imperial))) {

            switch (unit) {
            case Units::grain:
                convert_factor = grain_to_pounds<double>;
                break;
            case Units::pound:
                convert_factor = 1.;
                break;
            case Units::ounce:
                convert_factor = ounce_to_pounds<double>;
                break;
            case Units::stone:
                convert_factor = stone_to_pounds<double>;
                break;
            case Units::quarter:
                convert_factor = quarter_to_pounds<double>;
                break;
            case Units::hundredweight:
                convert_factor = hundredweight_to_pounds<double>;
                break;
            case Units::ton:
                convert_factor = ton_to_pounds<double>;
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
            case Units::gram:
                convert_factor = 1.;
                break;
            case Units::decigram:
                convert_factor = to_deci<double>;
                break;
            case Units::centigram:
                convert_factor = to_centi<double>;
                break;
            case Units::milligram:
                convert_factor = to_milli<double>;
                break;
            case Units::microgram:
                convert_factor = to_micro<double>;
                break;
            case Units::nanogram:
                convert_factor = to_nano<double>;
                break;
            case Units::kilogram:
                convert_factor = to_kilo<double>;
                break;
            case Units::hectogram:
                convert_factor = to_hecto<double>;
                break;
            case Units::decagram:
                convert_factor = to_deca<double>;
                break;
            default:
                convert_factor = 1.;
            }

        } else if (flags.isEnabled(static_cast<Units>(System::imperial))) {

            switch (unit) {
            case Units::grain:
                convert_factor = pound_to_grains<double>;
                break;
            case Units::pound:
                convert_factor = 1.;
                break;
            case Units::ounce:
                convert_factor = pound_to_ounces<double>;
                break;
            case Units::stone:
                convert_factor = pound_to_stone<double>;
                break;
            case Units::quarter:
                convert_factor = pound_to_quarter<double>;
                break;
            case Units::hundredweight:
                convert_factor = pound_to_hundredweights<double>;
                break;
            case Units::ton:
                convert_factor = pound_to_ton<double>;
                break;
            default:
                convert_factor = 1.;
            }

        }

        return convert_factor;
    }

};
ALLOW_BITWISE_FLAG_OPERATIONS(MassConverter::Units)

}