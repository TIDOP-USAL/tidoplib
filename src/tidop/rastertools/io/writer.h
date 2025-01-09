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

#ifdef TL_HAVE_OPENCV

#include <string>
#include <memory>

#include "opencv2/core/core.hpp"

#include "tidop/core/defs.h"
#include "tidop/core/path.h"
#include "tidop/core/ptr.h"
#include "tidop/geometry/rect.h"
#include "tidop/rastertools/img.h"
#include "tidop/math/geometry/affine.h"

namespace tl
{

class ImageOptions;
class ImageMetadata;
class ImageWriter;

/*! \addtogroup raster
 *  \{
 */


/*!
 * \brief Class for writing different raster formats
 */
class TL_EXPORT RasterWriter
{
        
public:

    RasterWriter(tl::Path file);
    ~RasterWriter() = default;

    /*!
     * \brief Open the raster file
     */
    void open(const tl::Path &file);

    /*!
     * \brief Check if the file has been loaded correctly
     */
    auto isOpen() const -> bool;

    /*!
     * \brief Close the file
     */
    void close();

    /*!
     * \brief Sets the image metadata
     */
    void setMetadata(const std::shared_ptr<ImageMetadata> &imageMetadata);

    /*!
     * \brief Create an image
     * \param[in] rows Number of rows in the image
     * \param[in] cols Number of columns of the image
     * \param[in] bands Number of bands in the picture
     * \param[in] type Data type
     * \param[in] imageOptions Image options
     * \see DataType
     */
    void create(int rows,
                int cols,
                int bands,
                DataType type,
                const std::shared_ptr<ImageOptions> &imageOptions = nullptr);

    /*!
     * \brief Write on the image
     * \param[in] image Image block to be written
     * \param[in] rect area where the image is written
     */
    void write(const cv::Mat &image,
               const Rect<int> &rect = Rect<int>());

    /*!
     * \brief Write on the image
     * \param[in] image Image block to be written
     * \param[in] window area where the image is written
     */
    void write(const cv::Mat &image,
               const WindowI &window);

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
     * \brief Sets the georeference of the image
     * \param[in] georeference Georeference
     */
    void setGeoreference(const Affine<double, 2> &georeference);

    /*!
     * \brief Set the Coordinate Reference System
     * \param[in] crs Coordinate Reference System in WKT format
     */
    void setCRS(const std::string &crs);

    void setNoDataValue(double nodata);


protected:

    void windowWrite(const WindowI &window,
                     WindowI *windowWrite,
                     Point<int> *offset) const;
protected:

    Path mFile;
    ImageWriter::Ptr mWriter;

};



/*! \} */ // end of raster


} // End namespace tl

#endif // TL_HAVE_OPENCV
