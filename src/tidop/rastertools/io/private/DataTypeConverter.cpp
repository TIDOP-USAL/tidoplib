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

#include "tidop/rastertools/io/private/DataTypeConverter.h"

#include "tidop/core/base/Exception.h"

namespace tl
{

namespace internal
{

GDALDataType DataTypeConverter::toGdal(DataType dataType)
{
    GDALDataType ret = GDT_Unknown;
    switch (dataType) {
    case DataType::TL_UNKNOWN:
        ret = GDT_Unknown;
        break;
    case DataType::TL_8U:
        ret = GDT_Byte;
        break;
    case DataType::TL_8S:
#if GDAL_VERSION_NUM >= GDAL_COMPUTE_VERSION(3,7,0)
        ret = GDT_Int8;
#else
        ret = GDT_Int16;
#endif
        break;
    case DataType::TL_16U:
        ret = GDT_UInt16;
        break;
    case DataType::TL_16S:
        ret = GDT_Int16;
        break;
    case DataType::TL_32U:
        ret = GDT_UInt32;
        break;
    case DataType::TL_32S:
        ret = GDT_Int32;
        break;
    case DataType::TL_32F:
        ret = GDT_Float32;
        break;
    case DataType::TL_64F:
        ret = GDT_Float64;
        break;
    }

    return ret;
}

GDALDataType DataTypeConverter::toGdal(int cvdt)
{
    GDALDataType ret;

    if (cvdt == CV_8U ||  //  CV_8U  == 0     GDT_Byte == 1
        cvdt == CV_8S)    //  CV_8S  == 1     GDT_Byte == 1
        ret = GDT_Byte;
    else if (cvdt == CV_16U)
        ret = GDT_UInt16;    //  CV_16U == 2     GDT_UInt16 == 2
    else if (cvdt == CV_16S)
        ret = GDT_Int16;     //  CV_16S == 3     GDT_Int16 == 3
    else if (cvdt == CV_32S)
        ret = GDT_Int32;     //  CV_32S == 4     GDT_Int32 == 5
    else if (cvdt == CV_32F)
        ret = GDT_Float32;   //  CV_32F == 5     GDT_Float32 == 6
    else if (cvdt == CV_64F)
        ret = GDT_Float64;   //  CV_64F == 6     GDT_Float64 == 7
    else
        ret = GDT_Unknown;   //                  GDT_Unknown == 0

    return(ret);
}

DataType DataTypeConverter::ToTidopLib(GDALDataType gdalDataType)
{
    DataType ret = DataType::TL_8U;

    switch (gdalDataType) {
    case GDT_Unknown:
        TL_THROW_EXCEPTION("Unknow data type");
    case GDT_Byte:
        ret = DataType::TL_8U;
        break;
#if GDAL_VERSION_NUM >= GDAL_COMPUTE_VERSION(3,7,0)
    case GDT_Int8:
        ret = DataType::TL_8S;
        break;
#endif
    case GDT_UInt16:
        ret = DataType::TL_16U;
        break;
    case GDT_Int16:
        ret = DataType::TL_16S;
        break;
    case GDT_UInt32:
        ret = DataType::TL_32U;
        break;
    case GDT_Int32:
        ret = DataType::TL_32S;
        break;
#if GDAL_VERSION_NUM >= GDAL_COMPUTE_VERSION(3,5,0)
    case GDT_UInt64:
    case GDT_Int64:
        TL_THROW_EXCEPTION("Data type not supported");
#endif
    case GDT_Float32:
        ret = DataType::TL_32F;
        break;
    case GDT_Float64:
        ret = DataType::TL_64F;
        break;
    case GDT_CInt16:
    case GDT_CInt32:
    case GDT_CFloat32:
    case GDT_CFloat64:
    case GDT_TypeCount:
        TL_THROW_EXCEPTION("Data type not supported");
    }

    return ret;
}

int DataTypeConverter::toOpenCV(GDALDataType gdalType, int channels)
{
    int depth;
    if (gdalType == GDT_Byte)
        depth = CV_8U;
    else if (gdalType == GDT_UInt16)
        depth = CV_16U;
    else if (gdalType == GDT_Int16)
        depth = CV_16S;
    else if (gdalType == GDT_UInt32 ||
        gdalType == GDT_Int32)
        depth = CV_32S;
    else if (gdalType == GDT_Float32)
        depth = CV_32F;
    else if (gdalType == GDT_Float64)
        depth = CV_64F;
    else if (gdalType == GDT_CInt16 ||
        gdalType == GDT_CInt32 ||
        gdalType == GDT_CFloat32 ||
        gdalType == GDT_CFloat64)
        TL_THROW_EXCEPTION("Data type not supported");
    else
        depth = -1;

    return(CV_MAKETYPE(depth, channels));
}
} // End namespace internal

} // End namespace tl
