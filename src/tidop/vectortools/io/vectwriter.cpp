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

#include "tidop/vectortools/io/vectwriter.h"
#include "tidop/vectortools/impl/io/gdalwriter.h"
#include "tidop/core/base/string_utils.h"

namespace tl
{

VectorWriter::VectorWriter(Path file)
  : mFile(std::move(file))
{
}


auto VectorWriterFactory::create(const Path& file) -> VectorWriter::Ptr
{
    VectorWriter::Ptr vector_writer;

    try {

        std::string extension = file.extension().toString();
#ifdef TL_HAVE_GDAL
        if (compareInsensitiveCase(extension, ".dxf") ||
            compareInsensitiveCase(extension, ".dwg") ||
            compareInsensitiveCase(extension, ".dgn") ||
            compareInsensitiveCase(extension, ".shp") ||
            compareInsensitiveCase(extension, ".gml") ||
            compareInsensitiveCase(extension, ".kml") ||
            compareInsensitiveCase(extension, ".kmz") ||
            compareInsensitiveCase(extension, ".json") ||
            compareInsensitiveCase(extension, ".osm")) {
            vector_writer = std::make_unique<VectorWriterGdal>(file);
        } else
#endif
        {
            TL_THROW_EXCEPTION("Invalid Vector Writer: {}", file.fileName().toString());
        }

    } catch (...) {
        TL_THROW_EXCEPTION_WITH_NESTED("Catched exception");
    }

    return vector_writer;
}


} // End namespace tl
