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

#pragma once

#include <string>
#include <sstream>
#include <regex>
#include <algorithm>

#include <proj.h>

#include "tidop/core/defs.h"

namespace tl {
    // Mover a core en versión 4.0
    TL_EXPORT const auto str_tolower = [](std::string s) {
        std::transform(s.begin(), s.end(), s.begin(), [](unsigned char c) { return std::tolower(c); });
        return s;
    };
    TL_EXPORT const auto str_toupper = [](std::string s) {
        std::transform(s.begin(), s.end(), s.begin(), [](unsigned char c) { return toupper(c); });
        return s;
    };
}  // namespace std

namespace tl {
    // Mover a core en versión 4.0
    TL_EXPORT bool findCaseInsensitiveRegex(const std::string& completeString, const std::string& toSearch);
    // duplicado en utils.h
    //TL_EXPORT std::vector<std::string> split(std::string str, char delim);
    // reemplazada por std::to_string
    //TL_EXPORT std::string to_string(int a_value);
    // Mover a core en versión 4.0
    TL_EXPORT std::string to_string_with_precision(double a_value, const int n);
    TL_EXPORT std::string pjTypeToString(PJ_TYPE type);
    TL_EXPORT PJ_TYPE stringToPjType(std::string strType);
}
