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

#include "tidop/config.h"

#include <string>
#include <memory>

#include <opencv2/core/core.hpp>

#include "tidop/core/base/defs.h"
#include "tidop/core/base/path.h"
#include "tidop/geometry/primitives/Point.h"
#include "tidop/graphic/rect.h"
#include "tidop/rastertools/img.h"
#include "tidop/rastertools/io/Metadata.h"
#include "tidop/math/geometry/affine.h"

namespace tl
{

class ImageOptions;
class ImageReader;

/*! \addtogroup RasterIO
 *  \{
 */

/*!
 * \brief Class for reading different raster formats
 *
 * The RasterReader class provides functionality for reading and optionally updating raster images
 * (e.g., GeoTIFF) using the GDAL library. It supports subregion reads, scaling, and coordinate transformations.
 *
 * ####Example usage:
 * \code
 * #include "tidop/rastertools/io/Reader.h"
 *
 * using namespace tl;
 *
 * RasterReader reader("image.tif");
 * if (reader.isOpen()) {
 *     cv::Mat img = reader.read(); // Read full image
 *     std::cout << "Image size: " << img.cols << "x" << img.rows << std::endl;
 * }
 * \endcode
 */
class TL_EXPORT RasterReader
{
	
public:

    /*! 
     * \enum Mode
     * \brief Access mode for opening the raster image.
     */
    enum class Mode { 
        Read,   /*!< Read-only mode */
        Update  /*!< Update mode. Enables writing to the raster */
    };
	
public:

    /*!
     * \brief Default constructor
     */
    RasterReader();

    /*!
     * \brief Constructor with file path and mode
     * \param[in] file Path to the raster image
     * \param[in] mode Access mode (read or update)
     */
    RasterReader(tl::Path file, Mode mode = Mode::Read);

    ~RasterReader();

    /*!
     * \brief Opens a raster image file
     * \param[in] file File path
     * \param[in] mode Access mode (default: Read)
     */
    void open(const tl::Path &file, Mode mode = Mode::Read);

    /*!
     * \brief Checks whether the file was successfully opened
     * \return true if open, false otherwise
     */
    auto isOpen() const -> bool;

    /*!
     * \brief Close the file
     */
    void close();

    /*!
     * \brief Reads a region of the image
     * \param[in] rect Region to read (default: full image)
     * \param[in] size Size of the output image (default: size of rect)
     * \return Subimage as a cv::Mat
     */
    auto read(const Rect<int> &rect = Rect<int>(),
              const Size<int> &size = Size<int>()) -> cv::Mat;

    /*!
     * \brief Reads a region with scaling factors
     * \param[in] scaleX Horizontal scaling (default: 1)
     * \param[in] scaleY Vertical scaling (default: 1)
     * \param[in] rect Region to read (default: full image)
     * \return Scaled subimage
     */
    auto read(double scaleX,
              double scaleY,
              const Rect<int> &rect = Rect<int>()) -> cv::Mat;

    /*!
     * \brief Reads a region defined in image coordinates (BoundingBox2i)
     * \param[in] window Input window
     * \param[in] scaleX Horizontal scaling (default: 1)
     * \param[in] scaleY Vertical scaling (default: 1)
     * \return Subimage as a cv::Mat
     */
    auto read(const BoundingBox2i &window,
              double scaleX = 1.,
              double scaleY = 1.) -> cv::Mat;

    /*!
     * \brief Reads a region defined in geographic/terrain coordinates
     * \param[in] terrainWindow Terrain window to read
     * \param[in] scaleX Horizontal scaling (default: 1)
     * \param[in] scaleY Vertical scaling (default: 1)
     * \param[out] affine Optional output affine transform
     * \return Subimage as a cv::Mat
     */
    auto read(const BoundingBox2d &terrainWindow,
              double scaleX = 1.,
              double scaleY = 1.,
              Affine<double, 2> *affine = nullptr) -> cv::Mat;

    /*!
     * \brief Writes an image into a region of the raster (in update mode)
     * \param[in] image Image to write
     * \param[in] rect Region to update (default: whole image)
     */
    void update(const cv::Mat &image, const Rect<int> &rect = Rect<int>());

    /*!
     * \brief Writes an image into a region defined by a window (in update mode)
     * \param[in] image Image to write
     * \param[in] window Region to update
     */
    void update(const cv::Mat &image, const BoundingBox2i &window);

    /*!
     * \brief Copies the current image to another file
     * \param[in] outputPath Destination file path
     * \param[in] options Optional image options (compression, tiling, etc.)
     * \param[in] metadata Optional metadata to attach
     * \param[in] epsgCode EPSG code for reprojection or CRS override
     */
    void copy(const tl::Path &outputPath,
              const std::shared_ptr<ImageOptions> &options = nullptr,
              const ImageMetadata &metadata = ImageMetadata(),
              const std::string &epsgCode = "") const;

    void addOverviews(int levels,
                      const std::shared_ptr<ImageOptions> &options = nullptr);

    /*!
     * \brief Returns the number of rows in the image
     * \return Number of rows in the image
     */
    auto rows() const -> int;

    /*!
     * \brief Returns the number of columns in the image
     * \return Number of columns in the image
     */
    auto cols() const -> int;

    /*!
     * \brief Returns the number of channels or bands in the image.
     * \return Number of image bands
     */
    auto channels() const -> int;

    /*!
     * \brief Returns the data type
     * \return
     */
    auto dataType() const -> DataType;

    /*!
     * \brief Bit depth per pixel
     */
    auto depth() const -> int;

    /*!
     * \brief Returns the image metadata
     */
    auto metadata() const -> ImageMetadata;

    /*!
     * \brief Check if the image is geo-referenced.
     */
    auto isGeoreferenced() const -> bool;

    /*!
     * \brief Returns the affine transform (georeference)
     */
    auto georeference() const -> Affine<double, 2>;

    /*!
     * \brief Coordinate Reference System in WKT format
     */
    auto crsWkt() const -> std::string;

    /*!
     * \brief Bounding box of the image in terrain coordinates
     */
    auto window() const ->BoundingBox2d;

    /*!
     * \brief No-data value for the image bands
     * \param exist Optional output flag indicating if a no-data value is defined
     * \return No-data value (if defined)
     */
    auto noDataValue(bool *exist = nullptr) const-> double;

private:

    std::unique_ptr<ImageReader> mReader;
};
ALLOW_BITWISE_FLAG_OPERATIONS(RasterReader::Mode)

/*! \} */ // end of raster


} // End namespace tl
