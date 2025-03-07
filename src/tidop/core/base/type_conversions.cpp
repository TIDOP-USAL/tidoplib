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

#include "tidop/core/base/type_conversions.h"

#include "tidop/core/base/exception.h"

#ifdef TL_HAVE_BOOST
#include <boost/algorithm/string.hpp>
#elif  CPP_VERSION >= 14
#include <algorithm>
#endif


namespace tl
{

int stringToInteger(const std::string &text, Base base)
{
    std::istringstream ss(text);
    switch (base) {
    case Base::octal:
        ss.setf(std::ios_base::oct, std::ios::basefield);
        break;
    case Base::decimal:
        ss.setf(std::ios_base::dec, std::ios::basefield);
        break;
    case Base::hexadecimal:
        ss.setf(std::ios_base::hex, std::ios::basefield);
        break;
    }
    int number;
    return ss >> number ? number : 0;
}

} // End namespace tl

