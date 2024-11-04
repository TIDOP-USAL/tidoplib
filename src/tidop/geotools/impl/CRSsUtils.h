/**************************************************************************
 *                                                                        *
 * Copyright (C) 2021 by Tidop Research Group                             *
 * Copyright (C) 2021 by David Hernandez Lopez                            *
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

#ifndef CRSSUTILS_CRSTOOLS_H
#define CRSSUTILS_CRSTOOLS_H

#include <string>
#include <sstream>
#include <regex>

#include <proj.h>

#include "tidop/core/defs.h"

namespace tl {
    TL_EXPORT bool findCaseInsensitiveRegex(const std::string& completeString, const std::string& toSearch);
    TL_EXPORT std::vector<std::string> split(std::string str, char delim);
    TL_EXPORT std::string to_string(int a_value);
    TL_EXPORT std::string to_string_with_precision(double a_value, const int n);
    TL_EXPORT std::string pjTypeToString(PJ_TYPE type);
    TL_EXPORT PJ_TYPE stringToPjType(std::string strType);
}
#endif