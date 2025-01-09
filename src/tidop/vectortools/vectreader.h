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

#pragma once

#include <memory>
#include <list>
#include <string>

#include "tidop/core/base/defs.h"
#include "tidop/core/base/path.h"
#include "tidop/core/base/ptr.h"


namespace tl
{


class GLayer;


/*! \addtogroup vector
 *  \{
 */


class TL_EXPORT VectorReader
{

    GENERATE_UNIQUE_PTR(VectorReader)

protected:

    Path mFile;

public:

    VectorReader(Path file);
    virtual ~VectorReader() = default;

    /*!
     * \brief Opens the file
     */
    virtual void open() = 0;

    /*!
     * \brief Checks if the file has been loaded correctly
     */
    virtual auto isOpen() const -> bool = 0;

    /*!
     * \brief Closes the file
     */
    virtual void close() = 0;

    virtual auto layersCount() const -> int = 0;
    virtual auto read(int layerId) -> std::shared_ptr<GLayer> = 0;
    virtual auto read(const std::string& layerName) -> std::shared_ptr<GLayer> = 0;

    /*!
     * \brief Reference system in WKT format
     */
    virtual auto crsWkt() const -> std::string = 0;

};


/*!
 * \brief Factoría de clases para la lectura de formatos vectoriales
 */
class TL_EXPORT VectorReaderFactory
{

private:

    VectorReaderFactory() = default;

public:

    static auto create(const Path &file) -> VectorReader::Ptr;
    TL_DEPRECATED("create", "2.1")
    static auto createReader(const Path &file) -> VectorReader::Ptr;
};


/*! \} */ // end of vector

} // End namespace tl
