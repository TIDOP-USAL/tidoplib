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
 * \class VectorWriter
 * \brief Abstract base class for writing vector data to files.
 *
 * The `VectorWriter` class provides an interface for creating, opening, writing to,
 * and closing vector data files. It is designed to be extended by specific implementations
 * that handle different vector file formats.
 *
 * \note This class uses the `Path` type to represent file paths and expects
 * derived classes to implement the actual file operations.
 *
 * \tparam Path Type representing the file path.
 */
class TL_EXPORT VectorWriter
{

    GENERATE_UNIQUE_PTR(VectorWriter)

public:

    /*!
     * \brief Constructor for `VectorWriter`.
     * \param[in] file The path to the vector file.
     */
    VectorWriter(Path file);

    virtual ~VectorWriter() = default;

    /*!
     * \brief Open the file.
     *
     * This method is responsible for opening the file for writing.
     * Derived classes should provide specific implementation details.
     */
    virtual void open() = 0;

    /*!
     * \brief Check if the file has been loaded correctly.
     * \return `true` if the file is open, `false` otherwise.
     */
    virtual auto isOpen() const -> bool = 0;

    /*!
     * \brief Close the file.
     *
     * This method is used to close the file once all writing operations are completed.
     */
    virtual void close() = 0;

    /*!
     * \brief Create the file.
     *
     * This method is responsible for creating a new file before writing data to it.
     */
    virtual void create() = 0;

    /*!
     * \brief Write the layer to the file.
     * \param[in] layer The `GLayer` object representing the layer to be written.
     */
    virtual void write(const GLayer &layer) = 0;

    /*!
     * \brief Set the Coordinate Reference System (CRS).
     * \param[in] epsgCode The EPSG code representing the desired CRS.
     *
     * This method allows setting the CRS for the vector data using an EPSG code.
     */
    virtual void setCRS(const std::string &epsgCode) = 0;

protected:

    Path mFile;

};




/*!
 * \class VectorWriterFactory
 * \brief Factory class for creating instances of `VectorWriter` for various vector formats.
 *
 * The `VectorWriterFactory` provides a static method to create `VectorWriter` instances
 * based on the provided file path. It abstracts the creation logic, allowing users to
 * work with different vector file formats without needing to know the specific implementation details.
 *
 * This class follows the factory design pattern to simplify the instantiation of
 * different `VectorWriter` types based on the file format.
 */
class TL_EXPORT VectorWriterFactory
{

private:

    /*!
     * \brief Private constructor to prevent instantiation.
     *
     * The constructor is private because this class is intended to be used
     * as a factory with static methods, and thus should not be instantiated directly.
     */
    VectorWriterFactory() = default;

public:

    /*!
     * \brief Create a `VectorWriter` instance for the given file.
     * \param file The path to the vector file.
     * \return A smart pointer (`Ptr`) to the created `VectorWriter` instance.
     *
     * This method inspects the file path and creates an appropriate `VectorWriter`
     * instance that can handle the specific format of the given vector file.
     * The actual type of `VectorWriter` returned depends on the file's extension or contents.
     */
    static auto create(const Path &file) -> VectorWriter::Ptr;
};

/*! \} */ // end of vector

} // End namespace tl
