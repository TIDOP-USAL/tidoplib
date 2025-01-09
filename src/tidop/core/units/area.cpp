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

#include "tidop/core/units/area.h"
#include "tidop/core/units/si.h"
#include "tidop/core/units/imperial.h"

namespace tl
{

auto AreaConverter::convert(double length, Units in, Units out) -> double
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
        output_unit.isEnabled(static_cast<Units>(System::si))) {

        convert_factor *= acre_to_square_metres<double>;

    }

    convert_factor *= convertFactorFromBaseUnit(out);

    return length * convert_factor;
}

auto AreaConverter::convertFactorToBaseUnit(Units unit) -> double
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

auto AreaConverter::convertFactorFromBaseUnit(Units unit) -> double
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


}