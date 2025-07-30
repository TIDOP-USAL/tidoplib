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


/*! \addtogroup VectorIO
 *  \{
 */


/*!
 * \class VectorReader
 * \brief Abstract base class for reading various vector file formats.
 *
 * The `VectorReader` class defines the interface for reading vector data from files. 
 * It provides methods to open, check the status, close the file, and read layers from it. 
 * Derived classes should implement the specific logic for handling different vector file formats.
 */
class TL_EXPORT VectorReader
{

    GENERATE_UNIQUE_PTR(VectorReader)

protected:

    Path mFile;

public:

    /*!
     * \brief Constructs a `VectorReader` with the given file path.
     * \param file The path to the vector file.
     */
    VectorReader(Path file);

    /*!
     * \brief Virtual destructor for proper cleanup in derived classes.
     */
    virtual ~VectorReader() = default;

    /*!
     * \brief Opens the vector file.
     *
     * This method must be implemented by derived classes to open the specified file
     * and prepare it for reading.
     */
    virtual void open() = 0;

    /*!
     * \brief Checks if the vector file has been successfully opened.
     * \return `true` if the file is open, `false` otherwise.
     *
     * This method should be overridden to provide the actual status of the file opening process.
     */
    virtual auto isOpen() const -> bool = 0;

    /*!
     * \brief Closes the vector file.
     *
     * Derived classes should implement this method to close the file and release any resources.
     */
    virtual void close() = 0;

    /*!
     * \brief Gets the number of layers in the vector file.
     * \return The count of layers.
     *
     * This method should return the total number of layers available in the opened vector file.
     */
    virtual auto layersCount() const -> int = 0;

    /*!
     * \brief Reads a layer from the vector file by its ID.
     * \param[in] layerId The ID of the layer to be read.
     * \return A shared pointer to the `GLayer` object representing the layer.
     *
     * This method reads the specified layer by its ID and returns it as a `GLayer` object.
     */
    virtual auto read(int layerId) -> std::shared_ptr<GLayer> = 0;

    /*!
     * \brief Reads a layer from the vector file by its name.
     * \param[in] layerName The name of the layer to be read.
     * \return A shared pointer to the `GLayer` object representing the layer.
     *
     * This method reads the specified layer by its name and returns it as a `GLayer` object.
     */
    virtual auto read(const std::string &layerName) -> std::shared_ptr<GLayer> = 0;

    /*!
     * \brief Gets the Coordinate Reference System (CRS) in Well-Known Text (WKT) format.
     * \return A string containing the CRS in WKT format.
     *
     * This method should return the CRS of the vector file as a WKT string.
     */
    virtual auto crsWkt() const -> std::string = 0;

};



/*!
 * \class VectorReaderFactory
 * \brief Factory class for creating `VectorReader` instances to handle different vector file formats.
 *
 * The `VectorReaderFactory` class provides a static method to create appropriate `VectorReader` objects
 * based on the specified file path. This factory pattern allows for the encapsulation of the logic
 * required to determine the correct `VectorReader` implementation for a given vector file format.
 */
class TL_EXPORT VectorReaderFactory
{

private:

    /*!
     * \brief Private constructor to prevent instantiation of `VectorReaderFactory`.
     *
     * This ensures that the factory is used solely through its static `create` method.
     */
    VectorReaderFactory() = default;

public:

    /*!
     * \brief Creates a `VectorReader` instance for the specified vector file.
     * \param[in] file The path to the vector file.
     * \return A shared pointer to a `VectorReader` object that can read the specified file.
     *
     * This static method analyzes the file type based on its extension or content and
     * returns an appropriate `VectorReader` instance capable of handling that specific format.
     * If the format is unsupported, it may return a null pointer or throw an exception.
     */
    static auto create(const Path &file) -> VectorReader::Ptr;

};


/*! \} */ // end of vector

} // End namespace tl
