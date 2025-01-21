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

#include "tidop/vectortools/io/vectreader.h"
#include "tidop/core/base/exception.h"
#include "tidop/core/private/gdalreg.h"
#include "tidop/vectortools/impl/io/gdalreader.h"

#ifdef TL_HAVE_GDAL
TL_DISABLE_WARNINGS
#include "ogrsf_frmts.h"
TL_DEFAULT_WARNINGS
#endif // TL_HAVE_GDAL

namespace tl
{

VectorReader::VectorReader(Path file)
  : mFile(std::move(file))
{
}



auto VectorReaderFactory::create(const Path &file) -> VectorReader::Ptr
{
    VectorReader::Ptr vector_reader;

    try {

        TL_ASSERT(file.exists(), "File doesn't exist: {}", file.toString());

        std::string extension = file.extension().toString();
#ifdef TL_HAVE_GDAL
        if (driverAvailable(file)){
        //if (VectorReaderGdal::isExtensionSupported(extension)) {
            vector_reader = VectorReaderGdal::New(file);
        } else
#endif
        {
            TL_THROW_EXCEPTION("Invalid Vector Reader: {}", file.fileName().toString());
        }

    } catch (...) {
        TL_THROW_EXCEPTION_WITH_NESTED("Catched exception");
    }

    return vector_reader;
}

} // End namespace tl
