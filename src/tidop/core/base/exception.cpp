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

#include "tidop/core/base/exception.h"

#include <locale>
#include <codecvt>
#include <utility>

#include "tidop/core/app/app.h"


namespace tl
{

Exception makeException(const std::string &error, 
                        const std::string &file, 
                        int line, 
                        const std::string &function)
{
    return Exception(error, file, line, function);
}

void printNestedException(const std::exception &e, int level)
{
    std::string indent(level, ' ');
    Message::error(">>{}{}", indent, e.what());

    try {
        std::rethrow_if_nested(e);
    } catch (const std::exception &nestedException) {
        printNestedException(nestedException, level + 1);
    } catch (...) {
        Message::error("Unknown exception");
    }
}

void printException(const std::exception &e)
{
    Message::error("Exception trace:");

    printNestedException(e, 0);
}


} // End namespace tl
