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

#include "tidop/img/imgreader.h"

#include "tidop/core/exception.h"
#include "tidop/img/metadata.h"
#include "tidop/img/impl/io/gdalreader.h"
#include "tidop/img/impl/io/canonreader.h"

#ifdef TL_HAVE_OPENCV


#include <utility>

namespace tl
{

ImageReader::ImageReader(tl::Path file)
  : mFile(std::move(file))
{

}

void ImageReader::windowRead(const WindowI &wLoad,
                             WindowI *wRead,
                             Point<int> *offset) const
{
    WindowI image_window(Point<int>(0, 0), Point<int>(this->cols(), this->rows()));

    if (wLoad.isEmpty()) {
        *wRead = image_window;
    } else {
        *wRead = windowIntersection(image_window, wLoad);
        *offset = wRead->pt1 - wLoad.pt1;
    }
}

auto ImageReader::file() const -> tl::Path
{
    return mFile;
}



auto ImageReaderFactory::create(const Path &file) -> ImageReader::Ptr
{
    ImageReader::Ptr image_reader;

    try {

        TL_ASSERT(file.exists(), "File doesn't exist: {}", file.toString());

        std::string extension = file.extension().toString();
#ifdef TL_HAVE_GDAL
        if (gdalValidExtensions(extension)) {
            image_reader = ImageReaderGdal::New(file);
        } else
#endif
#ifdef TL_HAVE_EDSDK
        if (compareInsensitiveCase(extension, ".CR2")) {
            image_reader = ImageReaderCanon::New(file);
        } else
#endif
        {
            TL_THROW_EXCEPTION("Invalid Image Reader: {}", file.fileName().toString());
        }

    } catch (...) {
        TL_THROW_EXCEPTION_WITH_NESTED("Catched exception");
    }

    return image_reader;
}

} // End namespace tl

#endif // TL_HAVE_OPENCV
