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
#include "tidop/core/base/macros/smart_ptr.h"

namespace tl
{

class GLayer;
class VectorWriterBase;


/*! \addtogroup VectorIO
 *  \{
 */

/*!
 * \brief Class for writing vector data files.
 *
 * The `VectorWriter` class provides an interface for creating and writing
 * vector data to files. It supports writing layers and assigning a
 * Coordinate Reference System (CRS) using an EPSG code.
 *
 * ### Example Usage
 * \code{.cpp}
 * tl::VectorWriter writer("output.shp");
 * if (writer.isOpen()) {
 *     writer.setCRS("EPSG:4326");
 *     writer.write(layer);
 *     writer.close();
 * }
 * \endcode
 */
class TL_EXPORT VectorWriter
{

private:

    std::unique_ptr<VectorWriterBase> mWriter;

public:

    /*!
     * \brief Default constructor.
     *
     * Creates an empty `VectorWriter` object. A file can be opened later
     * using the open() method.
     */
    VectorWriter();

    /*!
     * \brief Constructs a `VectorWriter` with the given file path.
     * \param[in] file Path to the vector file to be created.
     */
    VectorWriter(Path file);

    /*!
     * \brief Destructor.
     *
     * Releases any resources associated with the writer.
     */
    ~VectorWriter();

    /*!
     * \brief Opens the specified vector file for writing.
     * \param[in] file Output file path.
     */
    void open(const Path &file);

    /*!
     * \brief Checks if the vector file has been successfully opened.
     * \return `true` if the file is open, `false` otherwise.
     */
    auto isOpen() const -> bool;

    /*!
     * \brief Closes the currently opened vector file.
     *
     * This method should be called after all write operations
     * are completed to finalize the file.
     */
    void close();

    /*!
     * \brief Writes a layer to the vector file.
     * \param[in] layer The `GLayer` object representing the layer to be written.
     */
    void write(const GLayer &layer);

    /*!
     * \brief Sets the Coordinate Reference System (CRS) for the output file.
     * \param[in] epsgCode The EPSG code representing the desired CRS.
     *
     * This method allows assigning a spatial reference system
     * to the vector data by specifying an EPSG code.
     */
    void setCRS(const std::string &epsgCode);

};


/*! \} */ // end of vector

} // End namespace tl
