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

#include <map>
#include <string>
#include <memory>

#include "tidop/config.h"
#include "tidop/core/base/defs.h"
#include "tidop/core/base/path.h"
#include "tidop/core/base/macros/smart_ptr.h"
#include "tidop/geometry/bbox.h"

#include <proj.h>

#include "tidop/geotools/GeoTools.h"
#include "PointCloudToolsDefinitions.h"

namespace tl
{

/*! \addtogroup PointCloudToolsIO
 *  \{
 */

/*!
 * \brief Abstract base class for reading point cloud files.
 *
 * This class defines a unified interface for reading point clouds from multiple formats
 * (e.g., LAS, LAZ, PLY), with support for:
 * - Metadata extraction (CRS, bounding box, dimensions).
 * - Point data reading with optional filtering by bounding box or resolution.
 * - Coordinate transformation using CRS.
 * - COPC-specific structure access.
 *
 * Use \ref PointCloudReaderFactory::create to instantiate a concrete reader.
 * 
 * ### Example:
 * \code
 * #include <tidop/pointcloud/PointCloudReader.h>
 *
 * tl::Path file("example.laz");
 * auto reader = tl::PointCloudReaderFactory::create(file);
 * reader->open();
 *
 * if (reader->isOpen()) {
 *     auto dims = reader->getDimensionsNames();
 *     auto bbox = reader->getBoundingBox();
 *     auto offset = reader->getOffset();
 *     auto fist_point = reader->getCoordinates(0);
 *     auto intensity = reader->getField(0, "Intensity");
 * 
 *     // Loads all point data.
 *     std::vector<std::vector<float>> coords;
 *     std::vector<std::vector<float>> dimValues;
 *     double xo, yo, zo;
 *     reader->getPoints(xo, yo, zo, coords, dims, dimValues);
 * 
 *     reader->close();
 *
 *     std::cout << "Read " << coords.size() << " points.\n";
 *     std::cout << "Origin: (" << xo << ", " << yo << ", " << zo << ")\n";
 * }
 * \endcode
 */
class TL_EXPORT PointCloudReader
{
    GENERATE_UNIQUE_PTR(PointCloudReader)

public:

    /*!
     * \brief Constructor
     * \param file Path to the input point cloud file.
     */
    PointCloudReader(tl::Path file);

    /*!
     * \brief Virtual destructor
     */
    virtual ~PointCloudReader() = default;

    /*!
     * \brief Opens the point cloud file.
     */
    virtual void open() = 0;

    /*!
     * \brief Checks whether the file has been successfully opened.
     * \return True if the file is open.
     */
    virtual auto isOpen() const -> bool = 0;

    /*!
     * \brief Closes the file and releases resources
     */
    virtual void close() = 0;

    /*!
     * \brief Reference system in WKT format
     */
     //virtual auto crsWkt() const->std::string = 0;

    /*!
     * \brief Exports the COPC bounding box structure as CSV.
     * \param[in] fileName Output CSV path.
     * \param[in] crsId Optional CRS to reproject coordinates before exporting.
     */
    virtual void copcDumpBoundingBoxToCsv(const std::string &fileName, std::string crsId = "") = 0;

    /*!
     * \brief Gets average resolution (spacing) per level in a COPC structure.
     * \param[in] resolutionByLevel Output map of resolution by depth level.
     */
    virtual void copcGetResolutionByLevel(std::map<int, double> &resolutionByLevel) = 0;

    /*!
     * \brief Path of the currently loaded file.
     * \return The file path as a `tl::Path` object.
     */
    auto file() const -> tl::Path;

    /*!
     * \brief Gets the bounding box of the point cloud.
     * \param[out] x_min Minimum X.
     * \param[out] y_min Minimum Y.
     * \param[out] z_min Minimum Z.
     * \param[out] x_max Maximum X.
     * \param[out] y_max Maximum Y.
     * \param[out] z_max Maximum Z.
     * \param[in] crsId Optional target CRS for output values.
     */
    virtual void getBoundingBox(double &x_min, double &y_min, double &z_min,
                                double &x_max, double &y_max, double &z_max, std::string crsId = "") = 0;

    /*!
     * \brief Gets the bounding box as a BoundingBoxd structure.
     * \param[in] crsId Optional target CRS for output.
     * \return Bounding box in 3D.
     */
    virtual auto getBoundingBox(std::string crsId = "") const -> BoundingBoxd = 0;

    /*!
     * \brief Retrieves the names of the available dimensions in the point cloud.
     *
     * These may include standard dimensions such as "X", "Y", "Z", as well as additional attributes
     * like "Intensity", "Classification", "Red", "Green", "Blue", etc., depending on the file format.
     *
     * \return A vector of strings with the names of all dimensions present in the point cloud.
     */
    virtual auto getDimensionsNames() const -> std::vector<std::string> = 0;

    /*!
     * \brief Checks whether the loaded file is in COPC format.
     *
     * COPC (Cloud Optimized Point Cloud) is a specialized format based on LAZ 1.4 that includes
     * spatial indexing and metadata for optimized access.
     *
     * \return True if the point cloud file is a valid COPC file; false otherwise.
     */
    virtual bool getIsCopc() const = 0;

    /*!
     * \brief Loads all point data.
     * \param[out] x_o Origin X.
     * \param[out] y_o Origin Y.
     * \param[out] z_o Origin Z.
     * \param[out] coordinates Output vector of [N][3] point positions.
     * \param[out] dimensionsNames List of extra dimensions to load.
     * \param[out] dimensionsValues Output values for each dimension [N][D].
     * \param[int] crsId Optional target CRS.
     */
    virtual void getPoints(double &x_o, double &y_o, double &z_o,
                           std::vector<std::vector<float> > &coordinates,
                           std::vector<std::string> dimensionsNames,
                           std::vector<std::vector<float> > &dimensionsValues,
                           std::string crsId = "") = 0;

    /*!
     * \brief Loads point data within a bounding box.
     * \param[out] x_o Origin X.
     * \param[out] y_o Origin Y.
     * \param[out] z_o Origin Z.
     * \param[out] coordinates Output vector of [N][3] point positions.
     * \param[out] dimensionsNames List of extra dimensions to load.
     * \param[out] dimensionsValues Output values for each dimension [N][D].
     * \param[in] x_min, y_min, z_min Minimum bounds.
     * \param[in] x_max, y_max, z_max Maximum bounds.
     * \param[in] crsId Optional target CRS.
     */
    virtual void getPoints(double &x_o, double &y_o, double &z_o,
                           std::vector<std::vector<float> > &coordinates,
                           std::vector<std::string> dimensionsNames,
                           std::vector<std::vector<float> > &dimensionsValues,
                           double x_min, double y_min, double z_min,
                           double x_max, double y_max, double z_max,
                           std::string crsId = "") = 0;

    /*!
     * \brief Loads points at a given resolution.
     * \param[in] resolution Target resolution (e.g., average point spacing).
     * \param[in] crsId Optional CRS to reproject into.
     * \copydetails getPoints
     */
    virtual void getPoints(double &x_o, double &y_o, double &z_o,
                           std::vector<std::vector<float> > &coordinates,
                           std::vector<std::string> dimensionsNames,
                           std::vector<std::vector<float> > &dimensionsValues,
                           double resolution,
                           std::string crsId = "") = 0;

    /*!
     * \brief Loads points within a bounding box and resolution constraint.
     * \copydetails getPoints
     */
    virtual void getPoints(double &x_o, double &y_o, double &z_o,
                           std::vector<std::vector<float> > &coordinates,
                           std::vector<std::string> dimensionsNames,
                           std::vector<std::vector<float> > &dimensionsValues,
                           double x_min, double y_min, double z_min,
                           double x_max, double y_max, double z_max,
                           double resolution,
                           std::string crsId = "") = 0;

    /*!
     * \brief Returns the offset used for storing point coordinates.
     *
     * In many formats (e.g., LAS/LAZ), point coordinates are stored as integers with a scale and offset.
     * This method returns the offset that must be applied to decode real-world coordinates.
     *
     * \return A Point3<double> representing the offset in X, Y, and Z dimensions.
     */
    virtual auto getOffset() const -> Point3<double> = 0;

    /*!
     * \brief Retrieves the spatial coordinates of a specific point.
     *
     * \param[in] index Index of the point (zero-based).
     * \return A Point3<double> structure containing the X, Y, and Z coordinates of the specified point.
     *
     * \note Index must be within the valid range of loaded points.
     */
    virtual auto getCoordinates(int index) const -> Point3<double> = 0;

    /*!
     * \brief Retrieves the value of a specific dimension for a given point.
     *
     * \param[in] index Index of the point (zero-based).
     * \param[in] name Name of the dimension (e.g., "Intensity", "Classification").
     * \return The value of the specified dimension for the selected point.
     */
    virtual auto getField(int index, const std::string &name) const -> double = 0;

    /*!
     * \brief Checks whether the point cloud contains RGB color information.
     *
     * This typically corresponds to the presence of "Red", "Green", and "Blue" dimensions.
     *
     * \return True if RGB color attributes are present in the dataset.
     */
    virtual auto hasColors() const -> bool = 0;

    /*!
     * \brief Checks whether the point cloud contains surface normals.
     *
     * This typically corresponds to the presence of dimensions like "NormalX", "NormalY", "NormalZ".
     *
     * \return True if normal vectors are included in the dataset.
     */
    virtual auto hasNormals() const -> bool = 0;

protected:

    GeoTools *mPtrGeoTools;

private:

    Path mFile;
};

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
    static auto create(const Path &file) -> PointCloudReader::Ptr;
};

/*! \} */

}
