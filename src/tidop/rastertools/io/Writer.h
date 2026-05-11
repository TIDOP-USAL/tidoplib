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

#include "opencv2/core/core.hpp"

#include "tidop/core/base/defs.h"
#include "tidop/core/base/path.h"
#include "tidop/core/base/macros/smart_ptr.h"
#include "tidop/graphic/rect.h"
#include "tidop/rastertools/img.h"
#include "tidop/rastertools/io/Metadata.h"  
#include "tidop/geometry/transform/Affine.h"

namespace tl
{

class ImageOptions;
class ImageWriter;

/*! \addtogroup RasterIO
 *  \{
 */


/*!
 * \brief Class for writing different raster formats
 *
 * The RasterWriter class provides an interface to create and write raster images 
 * (e.g., GeoTIFF) using the GDAL library. It supports writing image blocks, metadata, 
 * georeferencing, and custom options such as compression or tiling.
 *
 * Example usage:
 * \code
 * using namespace tl;
 * 
 * RasterWriter writer;
 * writer.open("output.tif");
 * writer.create(1000, 1000, 1, DataType::UInt16);
 * 
 * cv::Mat block = cv::Mat::zeros(500, 500, CV_16UC1);
 * writer.write(block, Rect<int>(0, 0, 500, 500));
 * writer.close();
 * \endcode
 */
class TL_EXPORT RasterWriter
{
        
public:

    /*! \brief Default constructor */
    RasterWriter();

    /*!
     * \brief Constructor with file path
     * \param[in] file Output file path
     */
    RasterWriter(tl::Path file);

    ~RasterWriter();
	
    /*!
     * \brief Opens the target file
     * \param[in] file Output file path
     */
    void open(const tl::Path &file);

    /*!
     * \brief Create an image
     * \param[in] rows Number of rows in the image
     * \param[in] cols Number of columns of the image
     * \param[in] bands Number of image bands (channels)
     * \param[in] type Pixel data type
     * \param[in] imageOptions Optional image options (e.g., compression, tiling)
     * \see DataType
     */
    void create(int rows,
                int cols,
                int bands,
                DataType type,
                const std::shared_ptr<ImageOptions> &imageOptions = nullptr);
				
    /*!
     * \brief Checks if the writer is ready and the file is open
     * \return true if open and writable, false otherwise
     */
    auto isOpen() const -> bool;

    /*!
     * \brief Close the file
     */
    void close();

    /*!
     * \brief Sets metadata for the image
     * \param imageMetadata Metadata to associate with the output image
     */
    void setMetadata(const ImageMetadata &imageMetadata);

    /*!
     * \brief Writes a block of image data
     * \param[in] image Image block to be written
     * \param[in] rect Target area within the image (default: whole image)
     */
    void write(const cv::Mat &image,
               const Rect<int> &rect = Rect<int>());

    /*!
     * \brief Writes a block of image data
     * \param[in] image Image block to be written
     * \param[in] window Target area within the image
     */
    void write(const cv::Mat &image,
               const BoundingBox<Point2i> &window);

    /*!
     * \brief Returns the number of rows of the image.
     * \return Number of rows of the image
     */
    auto rows() const -> int;

    /*!
     * \brief Returns the number of columns in the image
     * \return Number of columns of the image
     */
    auto cols() const -> int;

    /*!
     * \brief Returns the number of channels or bands in the image.
     * \return Number of bands in the picture
     */
    auto channels() const -> int;

    /*!
     * \brief Returns the data type
     * \return Data type
     */
    auto dataType() const -> DataType;

    /*!
     * \brief Returns the colour depth or bits per pixel of an image.
     * \return Colour depth
     */
    auto depth() const -> int;

    /*!
     * \brief Sets the georeferencing of the image
     * \param[in] georeference Affine georeferencing transform
     */
    void setGeoreference(const Affine<double, 2> &georeference);

    /*!
     * \brief Sets the Coordinate Reference System (CRS) of the image
     * \param[in] crs Coordinate Reference System in WKT format
     */
    void setCRS(const std::string &crs);

    /*!
     * \brief Sets the no-data value of the image
     * \param[in] nodata Value to be treated as no-data
     */
    void setNoDataValue(double nodata);

private:

    std::unique_ptr<ImageWriter> mWriter;

};



/*! \} */ // end of raster


} // End namespace tl