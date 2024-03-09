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

#include "tidop/core/defs.h"
#include "tidop/core/path.h"
#include "tidop/core/ptr.h"

namespace tl
{

class GLayer;

class TL_EXPORT VectorWriter
{

    GENERATE_UNIQUE_PTR(VectorWriter)

public:

    VectorWriter(Path file);
    virtual ~VectorWriter() = default;

    /*!
     * \brief Open the file
     */
    virtual void open() = 0;

    /*!
     * \brief Check if the file has been loaded correctly
     */
    virtual auto isOpen() const -> bool = 0;

    /*!
     * \brief Close the file
     */
    virtual void close() = 0;

    /*!
     * \brief Create the file
     */
    virtual void create() = 0;

    /*!
     * \brief Write the layer to the file
     * \param[in] layer The layer to be written
     */
    virtual void write(const GLayer &layer) = 0;

    /*!
     * \brief Set the Coordinate Reference System
     * \param[in] epsgCode Coordinate Reference System in EPSG code format
     */
    virtual void setCRS(const std::string &epsgCode) = 0;

protected:

    Path mFile;

};


/*!
 * \brief Factory class for writing various vector formats
 */
class TL_EXPORT VectorWriterFactory
{

private:

    VectorWriterFactory() = default;

public:

    static auto create(const Path &file) -> VectorWriter::Ptr;
    TL_DEPRECATED("create", "2.1")
    static auto createWriter(const Path &file) -> VectorWriter::Ptr;
};


} // End namespace tl
