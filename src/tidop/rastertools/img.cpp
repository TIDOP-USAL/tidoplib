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

#include "tidop/rastertools/img.h"

#include "tidop/core/base/flags.h"
#include "tidop/core/base/string_utils.h"

#ifdef TL_HAVE_EDSDK
#include "EDSDK.h"
#ifndef HAVE_RAW
#  define HAVE_RAW
#endif
#endif // TL_HAVE_EDSDK

#ifdef TL_HAVE_OPENCV
#include "opencv2/core/core.hpp"
#endif // TL_HAVE_OPENCV


namespace tl
{




bool gdalValidExtensions(const std::string &extension)
{
    std::vector<std::string> extensions {
        ".bmp",    // Microsoft Windows Device Independent Bitmap (.bmp)
        ".png",    // Portable Network Graphics (.png)
        ".jpg", ".jpeg", ".jpe", ".jif", ".jfif", ".jfi",    // JPEG JFIF (.jpg)
        ".tif", ".tiff",    // TIFF / BigTIFF / GeoTIFF (.tif)
        ".gif",    // Graphics Interchange Format (.gif)
        ".gtx",    // NOAA .gtx vertical datum shift
        ".grd",    // Arc/Info ASCII Grid
        ".asc",    // Arc/Info ASCII Grid
        ".gsb",    // NTv2 Datum Grid Shift
        ".ecw",    // ERDAS Compressed Wavelets (.ecw)
        ".jp2",    // JPEG2000 (.jp2, .j2k)
        ".lan",    // Erdas 7.x .LAN and .GIS
        ".hdr",    // ENVI .hdr Labelled Raster
        ".img",    // Erdas Imagine (.img)
        ".blx",    // Magellan BLX Topo (.blx, .xlb)
        ".xlb",    // Magellan BLX Topo (.blx, .xlb)
        ".map",    // OziExplorer .MAP
        ".e00",    // Arc/Info Export E00 GRID
        ".hdr",    // Vexcel MFF
        ".img",    // Erdas Imagine (.img)
        ".wms"
    };

    for (auto &gdal_extension : extensions) {
        if (compareInsensitiveCase(extension, gdal_extension)) {
            return true;
        }
    }

    return false;
}

EnumFlags<DataType> gdalValidDataTypes(const std::string &format)
{
    EnumFlags<DataType> flag;
    if (format == "BMP") {
        flag.enable(DataType::TL_8U);
    } else if (format == "PNG") {
        flag.enable(DataType::TL_8U);
        flag.enable(DataType::TL_16U);
    } else if (format == "JPEG") {
        flag.enable(DataType::TL_8U);
    } else if (format == "GTiff") {
        flag.enable(DataType::TL_8U);
        flag.enable(DataType::TL_16U);
        flag.enable(DataType::TL_16S);
        flag.enable(DataType::TL_32U);
        flag.enable(DataType::TL_32S);
        flag.enable(DataType::TL_32F);
        flag.enable(DataType::TL_64F);
    } else if (format == "GIF") {
        flag.enable(DataType::TL_8U);
    } else if (format == "GTX") {
        flag.enable(DataType::TL_32F);
    } else if (format == "AAIGrid") {
        flag.enable(DataType::TL_32U);
        flag.enable(DataType::TL_32F);
        flag.enable(DataType::TL_64F);
    } else if (format == "NTv2") {
        flag.enable(DataType::TL_32F);
    } else if (format == "ECW") {
        flag.enable(DataType::TL_8U);
        flag.enable(DataType::TL_16U);
    } else if (format == "JP2OpenJPEG") {

    } else if (format == "LAN") {
        flag.enable(DataType::TL_8U);
        flag.enable(DataType::TL_16S);
    } else if (format == "ENVI") {
        flag.enable(DataType::TL_8U);
        flag.enable(DataType::TL_16U);
        flag.enable(DataType::TL_16S);
        flag.enable(DataType::TL_32U);
        flag.enable(DataType::TL_32S);
        flag.enable(DataType::TL_32F);
        flag.enable(DataType::TL_64F);
    } else if (format == "HFA") {
        flag.enable(DataType::TL_8U);
        flag.enable(DataType::TL_8S);
        flag.enable(DataType::TL_16U);
        flag.enable(DataType::TL_16S);
        flag.enable(DataType::TL_32U);
        flag.enable(DataType::TL_32S);
        flag.enable(DataType::TL_32F);
        flag.enable(DataType::TL_64F);
    } else if (format == "BLX") {
        flag.enable(DataType::TL_16U);
    } else if (format == "MAP") {

    } else if (format == "E00GRID") {

    } else if (format == "MFF") {
        flag.enable(DataType::TL_8U);
        flag.enable(DataType::TL_8S);
        flag.enable(DataType::TL_16U);
        flag.enable(DataType::TL_16S);
        flag.enable(DataType::TL_32U);
        flag.enable(DataType::TL_32S);
        flag.enable(DataType::TL_32F);
        flag.enable(DataType::TL_64F);
    } else if (format == "HFA") {
        flag.enable(DataType::TL_8U);
        flag.enable(DataType::TL_8S);
        flag.enable(DataType::TL_16U);
        flag.enable(DataType::TL_16S);
        flag.enable(DataType::TL_32U);
        flag.enable(DataType::TL_32S);
        flag.enable(DataType::TL_32F);
        flag.enable(DataType::TL_64F);
    } else if (format == "WMS") {

    }

    return flag;
}

/*!
 * \brief Devuelve el nombre del driver de GDAL correspondiente a una extensi�n de archivo
 * Si la extensi�n no se correspondo con un driver disponible devuelve nulo.
 * \param ext Extensi�n del archivo
 * \return Nombre del Driver de GDAL
 */
std::string gdalDriverFromExtension(const std::string &extension)
{
    std::string format;

    if (compareInsensitiveCase(extension, ".bmp"))
        format = "BMP";          // Microsoft Windows Device Independent Bitmap (.bmp)
    else if (compareInsensitiveCase(extension, ".png"))
        format = "PNG";          // Portable Network Graphics (.png)
    else if (compareInsensitiveCase(extension, ".jpg") ||
             compareInsensitiveCase(extension, ".jpeg") ||
             compareInsensitiveCase(extension, ".jpe") ||
             compareInsensitiveCase(extension, ".jif") ||
             compareInsensitiveCase(extension, ".jfif") ||
             compareInsensitiveCase(extension, ".jfi"))
        format = "JPEG";         // JPEG JFIF (.jpg)
    else if (compareInsensitiveCase(extension, ".tif") ||
             compareInsensitiveCase(extension, ".tiff"))
        format = "GTiff";        // TIFF / BigTIFF / GeoTIFF (.tif)
    else if (compareInsensitiveCase(extension, ".gif"))
        format = "GIF";          // Graphics Interchange Format (.gif)
    else if (compareInsensitiveCase(extension, ".gtx"))
        format = "GTX";          // NOAA .gtx vertical datum shift
    else if (compareInsensitiveCase(extension, ".grd") ||
             compareInsensitiveCase(extension, ".asc"))
        format = "AAIGrid";      // Arc/Info ASCII Grid
    else if (compareInsensitiveCase(extension, ".gsb"))
        format = "NTv2";         // NTv2 Datum Grid Shift
    else if (compareInsensitiveCase(extension, ".ecw"))
        format = "ECW";          // ERDAS Compressed Wavelets (.ecw)
    else if (compareInsensitiveCase(extension, ".jp2"))
        format = "JP2OpenJPEG";  // JPEG2000 (.jp2, .j2k)
    else if (compareInsensitiveCase(extension, ".lan"))
        format = "LAN";          // Erdas 7.x .LAN and .GIS
    else if (compareInsensitiveCase(extension, ".hdr"))
        format = "ENVI";         // ENVI .hdr Labelled Raster
    else if (compareInsensitiveCase(extension, ".img"))
        format = "HFA";          // Erdas Imagine (.img)
    else if (compareInsensitiveCase(extension, ".blx") ||
             compareInsensitiveCase(extension, ".xlb"))
        format = "BLX";          // Magellan BLX Topo (.blx, .xlb)
    else if (compareInsensitiveCase(extension, ".map"))
        format = "MAP";          // OziExplorer .MAP
    else if (compareInsensitiveCase(extension, ".e00"))
        format = "E00GRID";      // Arc/Info Export E00 GRID
    else if (compareInsensitiveCase(extension, ".hdr"))
        format = "MFF";          // Vexcel MFF
    else if (compareInsensitiveCase(extension, ".img"))
        format = "HFA";          // Erdas Imagine (.img)
    else if (compareInsensitiveCase(extension, ".wms"))
        format = "WMS";          // WMS
    else
        format = "";

    return format;
}



#ifdef TL_HAVE_OPENCV

int dataTypeToOpenCVDataType(DataType dataType)
{
    int ret;
    switch (dataType) {
    case DataType::TL_8U:
        ret = CV_8U;
        break;
    case DataType::TL_8S:
        ret = CV_8S;
        break;
    case DataType::TL_16U:
        ret = CV_16U;
        break;
    case DataType::TL_16S:
        ret = CV_16S;
        break;
    case DataType::TL_32U:
        ret = CV_32S;
        break;
    case DataType::TL_32S:
        ret = CV_32S;
        break;
    case DataType::TL_32F:
        ret = CV_32F;
        break;
    case DataType::TL_64F:
        ret = CV_64F;
        break;
    default:
        ret = -1;
        break;
    }
    return ret;
}

DataType openCVDataTypeToDataType(int dataType)
{
    DataType data_type;

    switch (dataType) {
    case CV_8U:
        data_type = DataType::TL_8U;
        break;
    case CV_8S:
        data_type = DataType::TL_8S;
        break;
    case CV_16U:
        data_type = DataType::TL_16U;
        break;
    case CV_16S:
        data_type = DataType::TL_16S;
        break;
    case CV_32S:
        data_type = DataType::TL_32S;
        break;
    case CV_32F:
        data_type = DataType::TL_32F;
        break;
    case CV_64F:
        data_type = DataType::TL_64F;
        break;
    default:
        data_type = DataType::TL_8U;
        break;
    }

    return data_type;
}

#endif // TL_HAVE_OPENCV

std::vector<int> gdalBandOrder(int channels)
{
    std::vector<int> panBandMap;
#ifdef TL_HAVE_OPENCV
    if (channels == 1)
        panBandMap = {1};
    if (channels == 2)
        panBandMap = {1, 2};
    else if (channels == 3)
        panBandMap = {3, 2, 1};   // Orden de bandas de OpenCV
    else if (channels == 4)
        panBandMap = {3, 2, 1, 4};
#else
    if (channels == 1)
        panBandMap = {1};
    else if (channels == 3)
        panBandMap = {1, 2, 3};
    else if (channels == 4)
        panBandMap = {1, 2, 3, 4};
#endif

    return panBandMap;
}


#ifdef TL_HAVE_EDSDK

std::unique_ptr<RegisterEDSDK> RegisterEDSDK::sRegisterEDSDK;
std::mutex RegisterEDSDK::sMutex;

RegisterEDSDK::RegisterEDSDK() 
{
    EdsInitializeSDK();
}

RegisterEDSDK::~RegisterEDSDK() 
{
    EdsTerminateSDK();
}

void RegisterEDSDK::init()
{
    if (sRegisterEDSDK.get() == nullptr) {
        std::lock_guard<std::mutex> lck(RegisterEDSDK::sMutex);
        if (sRegisterEDSDK.get() == nullptr) {
            sRegisterEDSDK.reset(new RegisterEDSDK());
        }
    }
}

#endif // TL_HAVE_EDSDK

} // End namespace tl
