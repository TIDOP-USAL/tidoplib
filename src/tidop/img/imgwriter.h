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
#include "tidop/img/img.h"
#include "tidop/math/geometry/affine.h"

namespace tl
{

class ImageOptions;
class ImageMetadata;

/*!
 * \brief Class for writing different image formats
 */
class TL_EXPORT ImageWriter
{

    GENERATE_UNIQUE_PTR(ImageWriter)
        
public:

    ImageWriter(tl::Path file);
    virtual ~ImageWriter() = default;

    /*!
     * \brief Open the image file
     */
    virtual void open() = 0;

    /*!
     * \brief Check if the file has been loaded correctly
     */
    virtual auto isOpen() const -> bool = 0;

    /*!
     * \brief Close the file
     */
    virtual void close() = 0;

    /*!
     * \brief Sets the options for the creation of the image format
     * \param[in] imageOptions Image options
     */
    virtual void setImageOptions(ImageOptions *imageOptions) = 0;

    /*!
     * \brief Sets the image metadata
     */
    virtual void setImageMetadata(const std::shared_ptr<ImageMetadata> &imageMetadata) = 0;

    /*!
     * \brief Create an image
     * \param[in] rows Number of rows in the image
     * \param[in] cols Number of columns of the image
     * \param[in] bands Number of bands in the picture
     * \param[in] type Data type
     * \see DataType
     */
    virtual void create(int rows,
                        int cols,
                        int bands,
                        DataType type) = 0;

    /*!
     * \brief Write on the image
     * \param[in] image Image block to be written
     * \param[in] rect area where the image is written
     */
    virtual void write(const cv::Mat &image,
                       const Rect<int> &rect = Rect<int>()) = 0;

    /*!
     * \brief Write on the image
     * \param[in] image Image block to be written
     * \param[in] window area where the image is written
     */
    virtual void write(const cv::Mat &image,
                       const WindowI &window) = 0;

    /*!
     * \brief Returns the number of rows of the image.
     * \return Number of rows of the image
     */
    virtual auto rows() const -> int = 0;

    /*!
     * \brief Returns the number of columns in the image
     * \return Number of columns of the image
     */
    virtual auto cols() const -> int = 0;

    /*!
     * \brief Returns the number of channels or bands in the image.
     * \return Number of bands in the picture
     */
    virtual auto channels() const -> int = 0;

    /*!
     * \brief Returns the data type
     * \return Data type
     */
    virtual auto dataType() const -> DataType = 0;

    /*!
     * \brief Returns the colour depth or bits per pixel of an image.
     * \return Colour depth
     */
    virtual auto depth() const -> int = 0;

    /*!
     * \brief Sets the georeference of the image
     * \param[in] georeference Georeference
     */
    virtual void setGeoreference(const Affine<double, 2> &georeference) = 0;

    /*!
     * \brief Set the Coordinate Reference System
     * \param[in] crs Coordinate Reference System in WKT format
     */
    virtual void setCRS(const std::string &crs) = 0;

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
 * \brief Factory class for writing different image formats
 */
class TL_EXPORT ImageWriterFactory
{
public:

private:

    ImageWriterFactory() {}

public:

    static auto create(const Path &fileName) -> ImageWriter::Ptr;
};


} // End namespace tl

#endif // TL_HAVE_OPENCV
