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

#ifdef TL_HAVE_OPENCV

#include <string>
#include <memory>

#include <opencv2/core/core.hpp>

#include "tidop/core/defs.h"
#include "tidop/core/path.h"
#include "tidop/core/ptr.h"
#include "tidop/geometry/entities/point.h"
#include "tidop/geometry/rect.h"
#include "tidop/img/img.h"
#include "tidop/math/geometry/affine.h"

namespace tl
{

class ImageMetadata;

/*!
 * \brief Class for reading different image formats
 */
class TL_EXPORT ImageReader
{
    GENERATE_UNIQUE_PTR(ImageReader)

public:

    ImageReader(tl::Path file);
    virtual ~ImageReader() = default;

    /*!
     * \brief Open the file
     */
    virtual void open() = 0;

    /*!
     * \brief Check if the file has been loaded correctly.
     */
    virtual auto isOpen() const -> bool = 0;

    /*!
     * \brief Close the file
     */
    virtual void close() = 0;

    /*!
     * \brief Reads an image area corresponding to a rectangle
     * \param[in] rect Rectangle of the image to be loaded. By default the whole image
     * \param[in] size Output image size. By default the size of the reading area
     * \param[in] affine
     * \return Image
     */
    virtual auto read(const Rect<int> &rect = Rect<int>(),
                      const Size<int> &size = Size<int>(),
                      Affine<int, 2> *affine = nullptr) -> cv::Mat = 0;

    /*!
     * \brief Reads an image area corresponding to a rectangle
     * \param[in] scaleX Horizontal scale that applies to the area read. Default 1
     * \param[in] scaleY Vertical scale that applies to the area read. Default 1
     * \param[in] rect Area of the image to be loaded. By default the whole image
     * \param[in] affine
     * \return Image
     */
    virtual auto read(double scaleX,
                      double scaleY,
                      const Rect<int> &rect = Rect<int>(),
                      Affine<int, 2> *affine = nullptr) -> cv::Mat = 0;

    /*!
     * \brief Reads the image area corresponding to a window
     * \param[in] window Window of the image to be loaded
     * \param[in] scaleX Horizontal scale that applies to the region read. Default 1
     * \param[in] scaleY Vertical scale that applies to the region read. Default 1
     * \param[in] affine
     * \return Image
     */
    virtual auto read(const WindowI &window,
                      double scaleX = 1.,
                      double scaleY = 1.,
                      Affine<int, 2> *affine = nullptr) -> cv::Mat = 0;

    /*!
     * \brief Reads the image area corresponding to a window in terrestrial coordinates
     * \param[in] terrainWindow Window in terrain coordinates of the image to be loaded
     * \param[in] scaleX Horizontal scale that applies to the region read. Default 1
     * \param[in] scaleY Vertical scale that applies to the region read. Default 1
     * \param[out] affine Transformation to be applied to the returned image
     */
    virtual auto read(const Window<Point<double>> &terrainWindow,
                      double scaleX = 1.,
                      double scaleY = 1.,
                      Affine<int, 2> *affine = nullptr) -> cv::Mat = 0;

    /*!
     * \brief Returns the number of rows in the image
     * \return Number of rows in the image
     */
    virtual auto rows() const -> int = 0;

    /*!
     * \brief Returns the number of columns in the image
     * \return Number of columns in the image
     */
    virtual auto cols() const -> int = 0;

    /*!
     * \brief Returns the number of channels or bands in the image.
     * \return Number of image bands
     */
    virtual auto channels() const -> int = 0;

    /*!
     * \brief Returns the data type
     * \return
     */
    virtual auto dataType() const -> DataType = 0;

    /*!
     * \brief Devuelve la profundidad de color o bits por pixel de una imagen
     * \return Colour depth
     */
    virtual auto depth() const -> int = 0;

    virtual auto metadata() const -> std::shared_ptr<ImageMetadata> = 0;

    /*!
     * \brief Check if the image is geo-referenced.
     */
    virtual auto isGeoreferenced() const -> bool = 0;

    /*!
     * \brief Georeference of the image
     */
    virtual auto georeference() const -> Affine<double, 2> = 0;

    /*!
     * \brief Reference system in WKT format
     */
    virtual auto crsWkt() const -> std::string = 0;

    /*!
     * \brief Image bounding box in terrain coordinates
     */
    virtual auto window() const -> WindowD = 0;

    auto file() const -> tl::Path;

    virtual auto noDataValue(bool *exist = nullptr) const-> double = 0;

protected:

    void windowRead(const WindowI &wLoad,
                    WindowI *wRead,
                    Point<int> *offset) const;

private:

    Path mFile;

};


/*!
 * \brief Class factory for reading image formats
 */
class TL_EXPORT ImageReaderFactory
{

private:

    ImageReaderFactory() = default;

public:

    static auto create(const Path &file) -> ImageReader::Ptr;
};

} // End namespace tl

#endif // TL_HAVE_OPENCV
