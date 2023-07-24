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

#include "tidop/core/exception.h"

#include <locale>
#include <codecvt>
#include <utility>


#include "tidop/core/app.h"

//TODO: mirar
//https://en.cppreference.com/w/cpp/error/nested_exception
//https://en.cppreference.com/w/cpp/error/throw_with_nested
//https://stackoverflow.com/questions/37227300/why-doesnt-c-use-stdnested-exception-to-allow-throwing-from-destructor/37227893#37227893
//https://github.com/GPMueller/mwe-cpp-exception
//https://www.boost.org/doc/libs/1_65_1/doc/html/stacktrace/getting_started.html#stacktrace.getting_started.how_to_print_current_call_stack



namespace tl
{

Exception makeException(const std::string &error, 
                        const std::string &file, 
                        int line, 
                        const std::string &function)
{
    return Exception(error, file, line, function);
}


void printException(const std::exception &e, int level)
{
    if (level == 0) {
        Message::error("{}Exception: {}", std::string(level, ' '), e.what());
        Message::error("Exception trace:");
    } else {
        Message::error(">>{} {}", std::string(level, ' '), e.what());
    }

    try {
        std::rethrow_if_nested(e);
    } catch (const std::exception &e) {
        printException(e, level + 1);
    } catch (...) {
        Message::error("Unknow exception");
    }
}


} // End namespace tl
