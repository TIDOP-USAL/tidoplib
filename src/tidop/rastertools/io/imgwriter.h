/**************************************************************************
 *                                                                        *
 * Copyright (C) 2021 by Tidop Research Group                             *
 * Copyright (C) 2021 by Esteban Ruiz de O�a Crespo                       *
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
#include "tidop/core/base/ptr.h"
#include "tidop/geometry/rect.h"
#include "tidop/rastertools/img.h"
#include "tidop/math/geometry/affine.h"

namespace tl
{

class ImageOptions;
class ImageMetadata;


/*! \addtogroup RasterIO
 *  \{
 */


 /*!
  * \brief Class for writing different image formats.
  *
  * The `ImageWriter` class provides an abstract interface for writing images to various formats.
  * It supports setting metadata, creating images, and writing image blocks to specific regions.
  * Derived classes must implement the pure virtual functions to handle specific image formats.
  *
  * <h4>Example Usage</h4>
  *
  * \code
  * tl::Path outputPath("output.tif");
  * auto writer = ImageWriterFactory::create(outputPath);
  * writer->open();
  * writer->create(100, 200, 3, DataType::UInt8);
  * cv::Mat image = cv::Mat::zeros(100, 200, CV_8UC3);
  * writer->write(image);
  * writer->close();
  * \endcode
  */
class TL_EXPORT ImageWriter
{

    GENERATE_UNIQUE_PTR(ImageWriter)
        
public:

    /*!
     * \brief Constructor for `ImageWriter`.
     * \param[in] file Path to the output image file.
     */
    ImageWriter(tl::Path file);

    virtual ~ImageWriter() = default;

    /*!
     * \brief Opens the image file for writing.
     */
    virtual void open() = 0;

    /*!
     * \brief Checks if the image file is open and ready for writing.
     * \return True if the file is open, false otherwise.
     */
    virtual auto isOpen() const -> bool = 0;

    /*!
     * \brief Closes the image file.
     */
    virtual void close() = 0;

    /*!
     * \brief Sets the metadata for the image.
     * \param[in] imageMetadata Shared pointer to the image metadata.
     * \see ImageMetadata
     */
    virtual void setMetadata(const std::shared_ptr<ImageMetadata> &imageMetadata) = 0;

    /*!
     * \brief Creates an image with the specified dimensions and data type.
     * \param[in] rows Number of rows in the image.
     * \param[in] cols Number of columns in the image.
     * \param[in] bands Number of bands (channels) in the image.
     * \param[in] type %Data type of the image.
     * \param[in] imageOptions Optional image options.
     * \see DataType ImageOptions
     */
    virtual void create(int rows,
                        int cols,
                        int bands,
                        DataType type,
                        const std::shared_ptr<ImageOptions> &imageOptions = nullptr) = 0;

    /*!
     * \brief Writes an image block to a specified rectangular region.
     * \param[in] image Image block to be written.
     * \param[in] rect Rectangle defining the area to write. Default is the entire image.
     * \see Rect
     */
    virtual void write(const cv::Mat &image,
                       const Rect<int> &rect = Rect<int>()) = 0;

    /*!
     * \brief Writes an image block to a specified window region.
     * \param[in] image %Image block to be written.
     * \param[in] window Window defining the area to write.
     * \see Window
     */
    virtual void write(const cv::Mat &image,
                       const WindowI &window) = 0;

    /*!
     * \brief Returns the number of rows in the image.
     * \return Number of rows.
     */
    virtual auto rows() const -> int = 0;

    /*!
     * \brief Returns the number of columns in the image.
     * \return Number of columns.
     */
    virtual auto cols() const -> int = 0;

    /*!
     * \brief Returns the number of channels (bands) in the image.
     * \return Number of channels.
     */
    virtual auto channels() const -> int = 0;

    /*!
     * \brief Returns the data type of the image.
     * \return %Data type.
     * \see DataType
     */
    virtual auto dataType() const -> DataType = 0;

    /*!
     * \brief Returns the color depth or bits per pixel of the image.
     * \return Color depth.
     */
    virtual auto depth() const -> int = 0;

    /*!
     * \brief Sets the georeference for the image.
     * \param[in] georeference Georeference transformation.
     */
    virtual void setGeoreference(const Affine<double, 2> &georeference) = 0;

    /*!
     * \brief Sets the Coordinate Reference System (CRS) for the image.
     * \param[in] crs CRS in WKT format.
     */
    virtual void setCRS(const std::string &crs) = 0;

    /*!
     * \brief Sets the NoData value for the image.
     * \param[in] nodata NoData value.
     */
    virtual void setNoDataValue(double nodata) = 0;


protected:

    void windowWrite(const WindowI &window,
                     WindowI *windowWrite,
                     Point<int> *offset) const;
protected:

    Path mFile;
    Affine<double, 2> affine;

};


/*!
 * \brief Factory class for creating `ImageWriter` instances.
 *
 * The `ImageWriterFactory` provides a static method for creating instances of `ImageWriter`
 * based on the file format of the provided path. It automatically selects the appropriate
 * `ImageWriter` implementation for the specified format.
 *
 * <h4>Example Usage</h4>
 *
 * \code
 * tl::Path outputPath("output.tif");
 * auto writer = ImageWriterFactory::create(outputPath);
 * writer->open();
 * // Perform writing operations...
 * writer->close();
 * \endcode
 */
class TL_EXPORT ImageWriterFactory
{
public:

private:

    ImageWriterFactory() {}

public:

    /*!
     * \brief Private constructor to prevent instantiation.
     *
     * `ImageWriterFactory` is designed to be used as a static factory and
     * should not be instantiated.
     */
    static auto create(const Path &fileName) -> ImageWriter::Ptr;
};

/*! \} */


} // End namespace tl