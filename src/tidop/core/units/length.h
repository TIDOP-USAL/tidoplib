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
constexpr enableIfFloating<T,T> metre_to_feets = static_cast<T>(3.28084);
template<typename T>
constexpr enableIfFloating<T,T> foot_to_metres = static_cast<T>(0.3048);


class LengthConverter
{

private:

    enum System
    {
        si = (1 << 20),
        imperial = (1 << 21),
    };

    enum LengthUnits
    {
        metre = (0 << 0),
        decimetre = (1 << 0),
        centimetre = (1 << 2),
        millimetre = (1 << 3),
        micrometre = (1 << 4),
        nanometre = (1 << 5),
        kilometre = (1 << 6),
        hectometre = (1 << 7),
        decametre = (1 << 8),
        inch = (1 << 9),
        foot = (1 << 10),
        yard = (1 << 11),
        chain = (1 << 12),
        furlong = (1 << 13),
        mile = (1 << 14)
    };

public:

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

            convert_factor *= metre_to_feets<double>;

        } else if (input_unit.isEnabled(static_cast<Units>(System::imperial)) &&
                   output_unit.isEnabled(static_cast<Units>(System::si))){

            convert_factor *= foot_to_metres<double>;

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
            case Units::metre:
                convert_factor = 1.;
                break;
            case Units::decimetre:
                convert_factor = from_deci<double>;
                break;
            case Units::centimetre:
                convert_factor = from_centi<double>;
                break;
            case Units::millimetre:
                convert_factor = from_milli<double>;
                break;
            case Units::micrometre:
                convert_factor = from_micro<double>;
                break;
            case Units::nanometre:
                convert_factor = from_nano<double>;
                break;
            case Units::kilometre:
                convert_factor = from_kilo<double>;
                break;
            case Units::hectometre:
                convert_factor = from_hecto<double>;
                break;
            case Units::decametre:
                convert_factor = from_deca<double>;
                break;
            default: 
                convert_factor = 1.;
            }

        } else if (flags.isEnabled(static_cast<Units>(System::imperial))) {

            switch (unit) {
            case Units::inch:
                convert_factor = inch_to_feet<double>;
                break;
            case Units::foot:
                convert_factor = 1.;
                break;
            case Units::yard:
                convert_factor = yard_to_feet<double>;
                break;
            case Units::chain:
                convert_factor = chain_to_feet<double>;
                break;
            case Units::furlong:
                convert_factor = furlong_to_feet<double>;
                break;
            case Units::mile:
                convert_factor = mile_to_feet<double>;
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
            case Units::metre:
                convert_factor = 1.;
                break;
            case Units::decimetre:
                convert_factor = to_deci<double>;
                break;
            case Units::centimetre:
                convert_factor = to_centi<double>;
                break;
            case Units::millimetre:
                convert_factor = to_milli<double>;
                break;
            case Units::micrometre:
                convert_factor = to_micro<double>;
                break;
            case Units::nanometre:
                convert_factor = to_nano<double>;
                break;
            case Units::kilometre:
                convert_factor = to_kilo<double>;
                break;
            case Units::hectometre:
                convert_factor = to_hecto<double>;
                break;
            case Units::decametre:
                convert_factor = to_deca<double>;
                break;
            default:
                convert_factor = 1.;
            }

        } else if (flags.isEnabled(static_cast<Units>(System::imperial))) {

            switch (unit) {
            case Units::inch:
                convert_factor = foot_to_inches<double>;
                break;
            case Units::foot:
                convert_factor = 1.;
                break;
            case Units::yard:
                convert_factor = foot_to_yards<double>;
                break;
            case Units::chain:
                convert_factor = foot_to_chains<double>;
                break;
            case Units::furlong:
                convert_factor = foot_to_furlongs<double>;
                break;
            case Units::mile:
                convert_factor = foot_to_mile<double>;
                break;
            default:
                convert_factor = 1.;
            }

        }

        return convert_factor;
    }

};
ALLOW_BITWISE_FLAG_OPERATIONS(LengthConverter::Units)

}