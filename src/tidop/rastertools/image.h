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

#include "tidop/rastertools/img.h"
#include "tidop/core/base/size.h"
#include "tidop/graphic/color.h"

namespace tl
{

/*! \addtogroup RasterTools
 *  \{
 */

/*!
 * \class Image
 * \brief A class for handling image data with support for various data types and channels.
 *
 * The `Image` class is designed for representing an image in memory. It supports
 * a variety of image formats by allowing the specification of the number of rows, 
 * columns, data type, and channels. The class also provides functionalities for 
 * initialization, copying, moving, and accessing image data.
 * 
 * \note The `Image` class is designed to support operations on images with different
 *       data types (e.g., unsigned 8-bit, floating point, etc.), and it can handle 
 *       images with multiple channels (e.g., grayscale, RGB, RGBA).
 */
class TL_EXPORT Image
{

private:

    int mRows{0};                      ///< Number of rows (height) in the image.
    int mCols{0};                      ///< Number of columns (width) in the image.
    DataType mType{DataType::TL_8U};   ///< Data type of the image (e.g., TL_8U for 8-bit unsigned).
    int mChannels{1};                  ///< Number of channels in the image (1 for grayscale, 3 for RGB).
    unsigned char *mData{nullptr};     ///< Pointer to the raw image data in memory.


public:

    /*!
     * \brief Default constructor.
     * Initializes an empty image.
     */
    Image();

    /*!
     * \brief Constructs an image with the specified dimensions, type, and channels.
     * \param[in] rows The number of rows (height) of the image.
     * \param[in] cols The number of columns (width) of the image.
     * \param[in] type The data type of the image (e.g., TL_8U, TL_32F).
     * \param[in] channels The number of channels in the image (e.g., 1 for grayscale, 3 for RGB).
     */
    Image(int rows, int cols, DataType type, int channels);

    /*!
     * \brief Constructs an image with the specified dimensions, type, channels, and a fill color.
     * \param[in] rows The number of rows (height) of the image.
     * \param[in] cols The number of columns (width) of the image.
     * \param[in] type The data type of the image.
     * \param[in] channels The number of channels in the image.
     * \param[in] color The color to fill the image.
     */
    Image(int rows, int cols, DataType type, int channels, const Color &color);

    /*!
     * \brief Constructs an image with the specified dimensions, type, channels, and data.
     * \param[in] rows The number of rows (height) of the image.
     * \param[in] cols The number of columns (width) of the image.
     * \param[in] type The data type of the image.
     * \param[in] channels The number of channels in the image.
     * \param[in] data The raw image data.
     */
    Image(int rows, int cols, DataType type, int channels, void *data);

    /*!
     * \brief Constructs an image with the specified size, type, and channels.
     * \param[in] size The size of the image (width and height).
     * \param[in] type The data type of the image.
     * \param[in] channels The number of channels in the image.
     */
    Image(const Size<int> &size, DataType type, int channels);

    /*!
     * \brief Constructs an image with the specified size, type, channels, and a fill color.
     * \param[in] size The size of the image (width and height).
     * \param[in] type The data type of the image.
     * \param[in] channels The number of channels in the image.
     * \param[in] color The color to fill the image.
     */
    Image(const Size<int> &size, DataType type, int channels, const Color &color);

    /*!
     * \brief Constructs an image with the specified size, type, channels, and data.
     * \param[in] size The size of the image (width and height).
     * \param[in] type The data type of the image.
     * \param[in] channels The number of channels in the image.
     * \param[in] data The raw image data.
     */
    Image(const Size<int> &size, DataType type, int channels, void *data);

    /*!
     * \brief Copy constructor.
     * Creates a new image by copying data from another image.
     * \param[in] image The image to copy from.
     */
    Image(const Image &image);

    /*!
     * \brief Move constructor.
     * Transfers resources from another image.
     * \param[in] image The image to move data from.
     */
    Image(Image &&image) TL_NOEXCEPT;

    ~Image();

    /*!
     * \brief Copy assignment operator.
     * Assigns data from one image to another.
     * \param[in] image The image to copy from.
     * \return A reference to the assigned image.
     */
    auto operator =(const Image &image) -> Image&;

    /*!
     * \brief Move assignment operator.
     * Moves data from one image to another.
     * \param[in] image The image to move data from.
     * \return A reference to the assigned image.
     */
    auto operator =(Image &&image) TL_NOEXCEPT-> Image&;

    /*!
     * \brief Returns the number of rows (height) in the image.
     * \return The number of rows in the image.
     */
    auto rows() const -> int;

    /*!
     * \brief Returns the number of columns (width) in the image.
     * \return The number of columns in the image.
     */
    auto cols() const -> int;

    /*!
     * \brief Returns the data type of the image.
     * \return The data type of the image (e.g., TL_8U, TL_32F).
     */
    auto type() const -> DataType;

    /*!
     * \brief Returns the number of channels in the image.
     * \return The number of channels in the image.
     */
    auto channels() const -> int;

    /*!
     * \brief Returns a pointer to the raw image data.
     * \return A pointer to the image data.
     */
    auto data() const -> unsigned char*;

    /*!
     * \brief Returns the depth (bits per pixel) of the image.
     * \return The depth of the image.
     */
    auto depth() const -> int;

    /*!
     * \brief Checks if the image is empty.
     * \return `true` if the image is empty, `false` otherwise.
     */
    auto isEmpty() const -> bool;

private:

    void init();

};

/*! \} */

} // End namespace tl
