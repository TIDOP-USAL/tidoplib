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
#include "tidop/rastertools/img.h"
#include "tidop/math/geometry/affine.h"

namespace tl
{


/*! \addtogroup raster
 *  \{
 */


class ImageMetadata;
class ImageReader;

/*!
 * \brief Class for reading different raster formats
 */
class TL_EXPORT RasterReader
{

public:

    RasterReader();
    RasterReader(tl::Path file);
    ~RasterReader() = default;

    /*!
     * \brief Open the file
     */
    void open(const tl::Path &file) = 0;

    /*!
     * \brief Check if the file has been loaded correctly.
     */
    auto isOpen() const -> bool;

    /*!
     * \brief Close the file
     */
    void close();

    /*!
     * \brief Reads an image area corresponding to a rectangle
     * \param[in] rect Rectangle of the image to be loaded. By default the whole image
     * \param[in] size Output image size. By default the size of the reading area
     * \param[in] affine
     * \return Image
     */
    auto read(const Rect<int> &rect = Rect<int>(),
              const Size<int> &size = Size<int>(),
              Affine<int, 2> *affine = nullptr) -> cv::Mat;

    /*!
     * \brief Reads an image area corresponding to a rectangle
     * \param[in] scaleX Horizontal scale that applies to the area read. Default 1
     * \param[in] scaleY Vertical scale that applies to the area read. Default 1
     * \param[in] rect Area of the image to be loaded. By default the whole image
     * \param[in] affine
     * \return Image
     */
    auto read(double scaleX,
              double scaleY,
              const Rect<int> &rect = Rect<int>(),
              Affine<int, 2> *affine = nullptr) -> cv::Mat;

    /*!
     * \brief Reads the image area corresponding to a window
     * \param[in] window Window of the image to be loaded
     * \param[in] scaleX Horizontal scale that applies to the region read. Default 1
     * \param[in] scaleY Vertical scale that applies to the region read. Default 1
     * \param[in] affine
     * \return Image
     */
    auto read(const WindowI &window,
              double scaleX = 1.,
              double scaleY = 1.,
              Affine<int, 2> *affine = nullptr) -> cv::Mat;

    /*!
     * \brief Reads the image area corresponding to a window in terrestrial coordinates
     * \param[in] terrainWindow Window in terrain coordinates of the image to be loaded
     * \param[in] scaleX Horizontal scale that applies to the region read. Default 1
     * \param[in] scaleY Vertical scale that applies to the region read. Default 1
     * \param[out] affine Transformation to be applied to the returned image
     */
    auto read(const Window<Point<double>> &terrainWindow,
              double scaleX = 1.,
              double scaleY = 1.,
              Affine<int, 2> *affine = nullptr) -> cv::Mat;

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
     * \brief Devuelve la profundidad de color o bits por pixel de una imagen
     * \return Colour depth
     */
    auto depth() const -> int;

    auto metadata() const -> std::shared_ptr<ImageMetadata>;

    /*!
     * \brief Check if the image is geo-referenced.
     */
    auto isGeoreferenced() const -> bool;

    /*!
     * \brief Georeference of the image
     */
    auto georeference() const -> Affine<double, 2>;

    /*!
     * \brief Reference system in WKT format
     */
    auto crsWkt() const -> std::string;

    /*!
     * \brief Image bounding box in terrain coordinates
     */
    auto window() const -> WindowD;

    auto file() const -> tl::Path;

    auto noDataValue(bool *exist = nullptr) const-> double;

protected:

    void windowRead(const WindowI &wLoad,
                    WindowI *wRead,
                    Point<int> *offset) const;

private:

    Path mFile;

};


/*! \} */ // end of raster


} // End namespace tl

#endif // TL_HAVE_OPENCV
