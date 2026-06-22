/**************************************************************************
 *                                                                        *
 * Copyright (C) 2021 by Tidop Research Group                             *
 * Copyright (C) 2021 by David Hernandez Lopez                            *
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

#include "tidop/config.h"
#include "tidop/core/base/Path.h"
#include "tidop/core/base/macros/SmartPtr.h"
#include "tidop/pctools/io/impl/PointCloudReader.h"

namespace tl
{

/*! \addtogroup PointCloudToolsIO
 *  \{
 */


/*!
 * \brief Factory class for creating point cloud readers.
 *
 * The PointCloudReaderFactory provides a static method to instantiate a suitable
 * \ref PointCloudReader based on the input file format (e.g., LAS, LAZ, PLY).
 *
 * Internally, it detects the file extension and selects an appropriate implementation
 * that supports reading point cloud data and metadata.
 *
 * ### Example:
 * \code
 * #include <tidop/pctools/PointCloudReader.h>
 *
 * tl::Path file("example.laz");
 * auto reader = tl::PointCloudReaderFactory::create(file);
 * reader->open();
 *
 * if (reader->isOpen()) {
 *     std::cout << "File loaded successfully.\n";
 *     reader->close();
 * }
 * \endcode
 */
class TL_EXPORT PointCloudReaderFactory
{

private:

    PointCloudReaderFactory() = default;

public:

    /*!
     * \brief Creates a point cloud reader for the given file.
     *
     * This method determines the format of the file and returns a concrete implementation
     * of \ref PointCloudReader capable of reading its content.
     *
     * \param[in] file Path to the point cloud file.
     * \return Smart pointer to a \ref PointCloudReader instance.
     */
    static auto create(const Path &file) -> PointCloudReaderBase::Ptr;
};


/*! \} */

}
