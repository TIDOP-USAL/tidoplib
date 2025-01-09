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
#include "tidop/core/base/ptr.h"
#include "tidop/geometry/entities/point.h"
#include "tidop/geometry/rect.h"
#include "tidop/rastertools/img.h"
#include "tidop/math/geometry/affine.h"

namespace tl
{


/*! \addtogroup RasterIO
 *  \{
 */


class ImageMetadata;

/*!
 * \brief Class for reading different raster image formats.
 *
 * The `ImageReader` class provides an abstract interface for reading various types of raster image files.
 * It supports reading specific regions, scaling, and accessing metadata such as georeferencing information.
 *
 * <h4>Usage Example</h4>
 *
 * \code
 * tl::Path imagePath("example.tif");
 * auto reader = ImageReaderFactory::create(imagePath);
 * reader->open();
 * if (reader->isOpen()) {
 *     cv::Mat image = reader->read();
 *     // Process the image
 *     reader->close();
 * }
 * \endcode
 */
class TL_EXPORT ImageReader
{
    GENERATE_UNIQUE_PTR(ImageReader)

public:

    /*!
     * \brief Constructs an `ImageReader` with the specified file path.
     * \param file Path to the image file.
     */
    ImageReader(tl::Path file);

    virtual ~ImageReader() = default;

    /*!
     * \brief Opens the image file.
     * This function must be called before attempting to read the image.
     */
    virtual void open() = 0;

    /*!
     * \brief Checks if the image file is successfully opened.
     * \return `true` if the file is open; otherwise `false`.
     */
    virtual auto isOpen() const -> bool = 0;

    /*!
     * \brief Closes the image file.
     */
    virtual void close() = 0;

    /*!
     * \brief Reads a specific area of the image defined by a rectangle.
     * \param[in] rect Rectangle specifying the area to read. Defaults to the entire image.
     * \param[in] size Desired output image size. Defaults to the size of the reading area.
     * \param[out] affine Optional affine transformation for the output image.
     * \return The image data as a `cv::Mat` object.
     * \see Rect Size Affine
     */
    virtual auto read(const Rect<int> &rect = Rect<int>(),
                      const Size<int> &size = Size<int>(),
                      Affine<int, 2> *affine = nullptr) -> cv::Mat = 0;

    /*!
     * \brief Reads a scaled area of the image.
     * \param[in] scaleX Horizontal scaling factor. Default is 1.
     * \param[in] scaleY Vertical scaling factor. Default is 1.
     * \param[in] rect Rectangle specifying the area to read. Defaults to the entire image.
     * \param[out] affine Optional affine transformation for the output image.
     * \return The image data as a `cv::Mat` object.
     * \see Rect Affine
     */
    virtual auto read(double scaleX,
                      double scaleY,
                      const Rect<int> &rect = Rect<int>(),
                      Affine<int, 2> *affine = nullptr) -> cv::Mat = 0;

    /*!
     * \brief Reads a specific window of the image.
     * \param[in] window The window specifying the image region to load.
     * \param[in] scaleX Horizontal scaling factor. Default is 1.
     * \param[in] scaleY Vertical scaling factor. Default is 1.
     * \param[out] affine Optional affine transformation for the output image.
     * \return The image data as a `cv::Mat` object.
     * \see Window Affine
     */
    virtual auto read(const WindowI &window,
                      double scaleX = 1.,
                      double scaleY = 1.,
                      Affine<int, 2> *affine = nullptr) -> cv::Mat = 0;

    /*!
     * \brief Reads an image area in terrestrial coordinates.
     * \param[in] terrainWindow Window in terrain coordinates of the area to read.
     * \param[in] scaleX Horizontal scaling factor. Default is 1.
     * \param[in] scaleY Vertical scaling factor. Default is 1.
     * \param[out] affine Optional affine transformation for the output image.
     * \return The image data as a `cv::Mat` object.
     * \see Window Affine
     */
    virtual auto read(const Window<Point<double>> &terrainWindow,
                      double scaleX = 1.,
                      double scaleY = 1.,
                      Affine<int, 2> *affine = nullptr) -> cv::Mat = 0;

    /*!
     * \brief Retrieves the number of rows in the image.
     * \return The number of rows (height) in the image.
     */
    virtual auto rows() const -> int = 0;

    /*!
     * \brief Retrieves the number of columns in the image.
     * \return The number of columns (width) in the image.
     */
    virtual auto cols() const -> int = 0;

    /*!
     * \brief Retrieves the number of channels in the image.
     * \return The number of color channels or bands.
     */
    virtual auto channels() const -> int = 0;

    /*!
     * \brief Retrieves the data type of the image.
     * \return The data type of the image pixels.
     * \see DataType
     */
    virtual auto dataType() const -> DataType = 0;

    /*!
     * \brief Retrieves the color depth of the image.
     * \return The color depth (bits per pixel).
     */
    virtual auto depth() const -> int = 0;

    /*!
     * \brief Retrieves the metadata associated with the image.
     * \return A shared pointer to the `ImageMetadata`.
     */
    virtual auto metadata() const -> std::shared_ptr<ImageMetadata> = 0;

    /*!
     * \brief Checks if the image is georeferenced.
     * \return `true` if the image has georeferencing information; otherwise `false`.
     */
    virtual auto isGeoreferenced() const -> bool = 0;

    /*!
     * \brief Retrieves the georeferencing information of the image.
     * \return The affine transformation representing the georeference.
     * \see Affine
     */
    virtual auto georeference() const -> Affine<double, 2> = 0;

    /*!
     * \brief Retrieves the coordinate reference system in WKT format.
     * \return The WKT string of the image's CRS.
     */
    virtual auto crsWkt() const -> std::string = 0;

    /*!
     * \brief Retrieves the image's bounding box in terrain coordinates.
     * \return The bounding box as a `WindowD` object.
     * \see Window
     */
    virtual auto window() const -> WindowD = 0;

    /*!
     * \brief Retrieves the file path of the image.
     * \return The file path as a `tl::Path` object.
     */
    auto file() const -> tl::Path;

    /*!
     * \brief Retrieves the no-data value of the image.
     * \param[out] exist Optional pointer to a boolean indicating if no-data value exists.
     * \return The no-data value if available; otherwise a default value.
     */
    virtual auto noDataValue(bool *exist = nullptr) const-> double = 0;

protected:

    void windowRead(const WindowI &wLoad,
                    WindowI *wRead,
                    Point<int> *offset) const;

private:

    Path mFile;

};


/*!
 * \brief Class factory for creating `ImageReader` instances.
 *
 * The `ImageReaderFactory` class provides a static method for creating instances of `ImageReader`
 * based on the file format of the provided image path. It supports various image formats and selects
 * the appropriate `ImageReader` implementation automatically.
 *
 * <h4>Usage Example</h4>
 *
 * \code
 * tl::Path imagePath("example.tif");
 * auto reader = ImageReaderFactory::create(imagePath);
 * reader->open();
 * if (reader->isOpen()) {
 *     cv::Mat image = reader->read();
 *     // Process the image
 *     reader->close();
 * }
 * \endcode
 */
class TL_EXPORT ImageReaderFactory
{

private:

    ImageReaderFactory() = default;

public:

    /*!
     * \brief Creates an `ImageReader` instance for the specified image file.
     * \param[in] file The path to the image file.
     * \return A smart pointer to an `ImageReader` suitable for reading the specified file.
     *
     * The factory method determines the appropriate `ImageReader` implementation
     * based on the file extension or content and returns an instance capable of
     * handling that format.
     */
    static auto create(const Path &file) -> ImageReader::Ptr;
};


/*! \} */


} // End namespace tl
