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

#include "tidop/rastertools/io/imgwriter.h"

#ifdef TL_HAVE_OPENCV

#include "tidop/rastertools/io/formats.h"
#include "tidop/rastertools/io/metadata.h"
#include "tidop/rastertools/impl/io/gdalwriter.h"
#include "tidop/core/base/exception.h"


namespace tl
{


ImageWriter::ImageWriter(tl::Path file)
  : mFile(std::move(file))
{

}

void ImageWriter::windowWrite(const WindowI &window,
                              WindowI *windowWrite,
                              Point<int> *offset) const
{
    WindowI window_all(Point<int>(0, 0), Point<int>(this->cols(), this->rows()));   // Ventana total de imagen
    if (window.isEmpty()) {
        *windowWrite = window_all;  // Se lee toda la ventana
    } else {
        *windowWrite = windowIntersection(window_all, window);
        *offset = windowWrite->pt1 - window.pt1;
    }
}



auto ImageWriterFactory::create(const Path &file) -> ImageWriter::Ptr
{
    ImageWriter::Ptr image_writer;

    try {

        std::string extension = file.extension().toString();

#ifdef TL_HAVE_GDAL
        if (gdalValidExtensions(extension)) {
            image_writer = ImageWriterGdal::New(file);
        } else
#endif
        {
            TL_THROW_EXCEPTION("Invalid Image Writer: {}", file.fileName().toString());
        }

    } catch (...) {
        TL_THROW_EXCEPTION_WITH_NESTED("Catched exception");
    }

    return image_writer;
}

} // End namespace tl

#endif // TL_HAVE_OPENCV
