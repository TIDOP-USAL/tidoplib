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

#include "tidop/featmatch/io/matchwriter.h"
#include "tidop/featmatch/io/impl/matches/binarywriter.h"
#include "tidop/featmatch/io/impl/matches/opencvwriter.h"
#include "tidop/core/base/exception.h"
#include "tidop/core/base/string_utils.h"

#include <fstream>

namespace tl
{


auto MatchesWriterFactory::create(const Path&file) -> std::unique_ptr<MatchesWriter>
{
    std::unique_ptr<MatchesWriter> matches_writer;

    try {

        std::string ext = file.extension().toString();

        if (compareInsensitiveCase(ext, ".bin")) {
            matches_writer = std::make_unique<MatchesWriterBinary>(file);
        } else if (compareInsensitiveCase(ext, ".xml") || compareInsensitiveCase(ext, ".yml")) {
            matches_writer = std::make_unique<MatchesWriterOpenCV>(file);
        } else {
            TL_THROW_EXCEPTION("No MatchesWriter found for: {}", file.fileName().toString());
        }

    } catch (...) {
        TL_THROW_EXCEPTION_WITH_NESTED("Catched exception");
    }

    return matches_writer;
}


} // namespace tl
