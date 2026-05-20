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

#include "tidop/core/base/StringUtils.h"

#include "tidop/core/base/Exception.h"

#ifdef TL_HAVE_BOOST
#include <boost/algorithm/string.hpp>
#elif  TL_CPP_VERSION>= 14
#include <algorithm>
#endif


namespace tl
{


#if TL_CPP_VERSION>= 17
bool compareInsensitiveCase(std::string_view source, std::string_view compare)
#else
bool compareInsensitiveCase(const std::string &source, const std::string &compare)
#endif
{
#ifdef TL_HAVE_BOOST
    return boost::iequals(source, compare);
#elif TL_CPP_VERSION >= 14
    //https://stackoverflow.com/questions/11635/case-insensitive-string-comparison-in-c
    return std::equal(source.begin(), source.end(),
                      compare.begin(), compare.end(),
                      [](char a, char b) {
                          return std::tolower(static_cast<unsigned char>(a)) == 
                                 std::tolower(static_cast<unsigned char>(b));
                      });
#else

    unsigned int sz = source.size();
    if (compare.size() != sz)
        return false;
    for (unsigned int i = 0; i < sz; ++i)
        if (std::tolower(static_cast<unsigned char>(source[i])) != 
            std::tolower(static_cast<unsigned char>(compare[i])))
            return false;
    return true;
#endif
}

void replaceString(std::string *str, const std::string &str_old, const std::string &str_new)
{
    TL_ASSERT(str != nullptr, "String pointer cannot be nullptr");
    TL_ASSERT(!str_old.empty(), "Search string cannot be empty");

    std::size_t ini = str->find(str_old);
    while (ini != std::string::npos) {
        str->replace(ini, str_old.size(), str_new);
        ini = str->find(str_old, str_new.size() + ini);
    }
}


} // End namespace tl

