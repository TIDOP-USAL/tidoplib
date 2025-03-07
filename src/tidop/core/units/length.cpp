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

#include "tidop/core/units/length.h"
#include "tidop/core/units/si.h"
#include "tidop/core/units/imperial.h"

namespace tl
{

auto LengthConverter::convert(double length, Units in, Units out) -> double
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
        output_unit.isEnabled(static_cast<Units>(System::si))) {

        convert_factor *= foot_to_metres<double>;

    }

    convert_factor *= convertFactorFromBaseUnit(out);

    return length * convert_factor;
}

auto LengthConverter::convertFactorToBaseUnit(Units unit) -> double
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

auto LengthConverter::convertFactorFromBaseUnit(Units unit) -> double
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


}