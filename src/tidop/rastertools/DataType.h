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

#include <string>
#include <vector>

#include "tidop/core/base/Defs.h"

namespace tl
{

/*! \addtogroup RasterTools
 *  \{
 */


/*!
 * \enum DataType
 * \brief Enumerates supported pixel data types for raster images.
 *
 * This enum defines the available data types used for image pixel representation,
 * ensuring compatibility across OpenCV (`cv::Mat`) and GDAL (`GDALDataType`).
 * The values are defined as bit flags to allow combinations if needed.
 */
enum class DataType : int16_t
{
    TL_UNKNOWN = 0,        /*!< Unknown or undefined data type. */
    TL_8U = (1 << 0),      /*!< Unsigned 8-bit integer (equivalent to OpenCV CV_8U and GDAL GDT_Byte). */
    TL_8S = (1 << 1),      /*!< Signed 8-bit integer (equivalent to OpenCV CV_8S; not supported by older GDAL versions). */
    TL_16U = (1 << 2),     /*!< Unsigned 16-bit integer (equivalent to OpenCV CV_16U and GDAL GDT_UInt16). */
    TL_16S = (1 << 3),     /*!< Signed 16-bit integer (equivalent to OpenCV CV_16S and GDAL GDT_Int16). */
    TL_32U = (1 << 4),     /*!< Unsigned 32-bit integer (equivalent to GDAL GDT_UInt32; not supported by OpenCV). */
    TL_32S = (1 << 5),     /*!< Signed 32-bit integer (equivalent to OpenCV CV_32S and GDAL GDT_Int32). */
    TL_32F = (1 << 6),     /*!< 32-bit floating point (equivalent to OpenCV CV_32F and GDAL GDT_Float32). */
    TL_64F = (1 << 7)      /*!< 64-bit floating point (equivalent to OpenCV CV_64F and GDAL GDT_Float64). */
};


/*! \} */


} // End namespace tl
