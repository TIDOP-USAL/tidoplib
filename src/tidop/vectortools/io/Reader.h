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

#include "tidop/core/base/Defs.h"
#include "tidop/core/base/Path.h"


namespace tl
{

class GLayer;
class VectorReaderBase;

/*! \addtogroup VectorIO
 *  \{
 */

/*!
 * \brief Class for reading vector data files.
 *
 * The `VectorReader` class provides an interface for opening, reading,
 * and accessing layers from vector data sources. It supports reading
 * layers by index or by name, and retrieving metadata such as the
 * Coordinate Reference System (CRS) in Well-Known Text (WKT) format.
 *
 * ### Example Usage
 * \code{.cpp}
 * tl::VectorReader reader("data/shapefile.shp");
 * if (reader.isOpen()) {
 *     int numLayers = reader.layersCount();
 *     auto layer = reader.read(0); // Read first layer
 *     std::string crs = reader.crsWkt();
 * }
 * \endcode
 */
class TL_EXPORT VectorReader
{

private:

    std::unique_ptr<VectorReaderBase> mReader;

public:

    /*!
     * \brief Default constructor.
     *
     * Creates an empty `VectorReader` object. A file can be opened later
     * using the open() method.
     */
    VectorReader();

    /*!
     * \brief Constructs a `VectorReader` with the given file path.
     * \param[in] file The path to the vector file to be opened.
     */
    VectorReader(Path file);

    /*!
     * \brief Destructor.
     *
     * Releases any resources associated with the reader.
     */
    ~VectorReader();

    /*!
     * \brief Opens the specified vector file.
     * \param[in] file Path to the vector file.
     */
    void open(const tl::Path &file);

    /*!
     * \brief Checks if the vector file has been successfully opened.
     * \return `true` if the file is open, `false` otherwise.
     */
    auto isOpen() const -> bool;

    /*!
     * \brief Closes the currently opened vector file.
     */
    void close();

    /*!
     * \brief Gets the number of layers in the vector file.
     * \return The number of layers contained in the file.
     */
    auto layersCount() const -> int;

    /*!
     * \brief Reads a layer from the vector file by its ID.
     * \param[in] layerId The ID of the layer to be read.
     * \return A shared pointer to the `GLayer` object representing the layer.
     */
    auto read(int layerId) -> std::shared_ptr<GLayer>;

    /*!
     * \brief Reads a layer from the vector file by its name.
     * \param[in] layerName The name of the layer to be read.
     * \return A shared pointer to the `GLayer` object representing the layer.
     */
    auto read(const std::string &layerName) -> std::shared_ptr<GLayer>;

    /*!
     * \brief Copies the vector dataset to a new file, with optional CRS reprojection.
     *
     * This method creates a copy of the current vector dataset at the given output path.
     * If \p targetEpsg is provided and differs from the source CRS, all layers and features
     * are reprojected on the fly to the target CRS before being written.
     *
     * - All layers are copied preserving layer names, geometry types, and attribute schemas.
     * - The output file is created using the default driver inferred from \p outputPath,
     *   unless a specific driver is provided with the overload that accepts \p driver and
     *   \p creationOptions.
     * - If \p overwrite is true and the destination exists, it will be replaced.
     *
     * \param[in] outputPath Destination path for the copied dataset.
     * \param[in] targetEpsg Optional target CRS in EPSG form (e.g. "EPSG:4326"). If empty or equal
     *                       to the source CRS, a direct copy is performed without reprojection.
     *
     * \throw std::runtime_error If the reader is not open, the output cannot be created,
     *                           a layer cannot be written, or the reprojection fails.
     *
     * ### Example
     * \code{.cpp}
     * tl::VectorReader reader("data/roads.gpkg");
     * reader.copy("backup/roads_copy.gpkg"); // plain copy, no reprojection
     *
     * // Reproject to WGS84
     * reader.copy("out/roads_wgs84.gpkg", "EPSG:4326");
     * \endcode
     */
    //void copy(const tl::Path &outputPath, const std::string &targetEpsg = "") const;

    /*!
     * \brief Gets the Coordinate Reference System (CRS) of the file
     *        in Well-Known Text (WKT) format.
     * \return A string containing the CRS in WKT format.
     */
    auto crsWkt() const -> std::string;

};


/*! \} */ // end of vector

} // End namespace tl
