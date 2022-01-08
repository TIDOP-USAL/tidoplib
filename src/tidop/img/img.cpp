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

#include "img.h"

#include <tidop/core/flags.h>
#include <tidop/core/utils.h>

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




//std::vector<std::string> gdalValidExtensions()
//{
//  std::vector<std::string> extensions {
//    "bmp",    // Microsoft Windows Device Independent Bitmap (.bmp)
//    "png",    // Portable Network Graphics (.png)
//    "jpg",    // JPEG JFIF (.jpg)
//    "tif",    // TIFF / BigTIFF / GeoTIFF (.tif)
//    "gif",    // Graphics Interchange Format (.gif)
//    "gtx",    // NOAA .gtx vertical datum shift
//    "grd",    // Arc/Info ASCII Grid
//    "asc",    // Arc/Info ASCII Grid
//    "gsb",    // NTv2 Datum Grid Shift
//    "ecw",    // ERDAS Compressed Wavelets (.ecw)
//    "jp2",    // JPEG2000 (.jp2, .j2k)
//    "lan",    // Erdas 7.x .LAN and .GIS
//    "hdr",    // ENVI .hdr Labelled Raster
//    "img",    // Erdas Imagine (.img)
//    "blx",    // Magellan BLX Topo (.blx, .xlb)
//    "xlb",    // Magellan BLX Topo (.blx, .xlb)
//    "map",    // OziExplorer .MAP
//    "e00",    // Arc/Info Export E00 GRID
//    "hdr",    // Vexcel MFF
//    "img",    // Erdas Imagine (.img)
//    "wms"
//  };
//
//  return extensions;
//}

bool gdalValidExtensions(const std::string &extension)
{
    std::vector<std::string> extensions {
    ".bmp",    // Microsoft Windows Device Independent Bitmap (.bmp)
    ".png",    // Portable Network Graphics (.png)
    ".jpg",".jpeg",".jpe",".jif",".jfif",".jfi",    // JPEG JFIF (.jpg)
    ".tif",".tiff",    // TIFF / BigTIFF / GeoTIFF (.tif)
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
  if (format.compare("BMP") == 0) {
    flag.flagOn(DataType::TL_8U);
  } else if (format.compare("PNG") == 0) {
    flag.flagOn(DataType::TL_8U);
    flag.flagOn(DataType::TL_16U);
  } else if (format.compare("JPEG") == 0) {
    flag.flagOn(DataType::TL_8U);
  } else if (format.compare("GTiff") == 0) {
    flag.flagOn(DataType::TL_8U);
    flag.flagOn(DataType::TL_16U);
    flag.flagOn(DataType::TL_16S);
    flag.flagOn(DataType::TL_32U);
    flag.flagOn(DataType::TL_32S);
    flag.flagOn(DataType::TL_32F);
    flag.flagOn(DataType::TL_64F);
  } else if (format.compare("GIF") == 0) {
    flag.flagOn(DataType::TL_8U);
  } else if (format.compare("GTX") == 0) {
    flag.flagOn(DataType::TL_32F);
  } else if (format.compare("AAIGrid") == 0) {
    flag.flagOn(DataType::TL_32U);
    flag.flagOn(DataType::TL_32F);
    flag.flagOn(DataType::TL_64F);
  } else if (format.compare("NTv2") == 0) {
    flag.flagOn(DataType::TL_32F);
  } else if (format.compare("ECW") == 0) {
    flag.flagOn(DataType::TL_8U);
    flag.flagOn(DataType::TL_16U);
  } else if (format.compare("JP2OpenJPEG") == 0) {
  
  } else if (format.compare("LAN") == 0) {
    flag.flagOn(DataType::TL_8U);
    flag.flagOn(DataType::TL_16S);
  } else if (format.compare("ENVI") == 0) {
    flag.flagOn(DataType::TL_8U);
    flag.flagOn(DataType::TL_16U);
    flag.flagOn(DataType::TL_16S);
    flag.flagOn(DataType::TL_32U);
    flag.flagOn(DataType::TL_32S);
    flag.flagOn(DataType::TL_32F);
    flag.flagOn(DataType::TL_64F);
  } else if (format.compare("HFA") == 0) {
    flag.flagOn(DataType::TL_8U);
    flag.flagOn(DataType::TL_8S);
    flag.flagOn(DataType::TL_16U);
    flag.flagOn(DataType::TL_16S);
    flag.flagOn(DataType::TL_32U);
    flag.flagOn(DataType::TL_32S);
    flag.flagOn(DataType::TL_32F);
    flag.flagOn(DataType::TL_64F);
  } else if (format.compare("BLX") == 0) {
    flag.flagOn(DataType::TL_16U); 
  } else if (format.compare("MAP") == 0) {
  
  } else if (format.compare("E00GRID") == 0) {
  
  } else if (format.compare("MFF") == 0) {
    flag.flagOn(DataType::TL_8U);
    flag.flagOn(DataType::TL_8S);
    flag.flagOn(DataType::TL_16U);
    flag.flagOn(DataType::TL_16S);
    flag.flagOn(DataType::TL_32U);
    flag.flagOn(DataType::TL_32S);
    flag.flagOn(DataType::TL_32F);
    flag.flagOn(DataType::TL_64F);
  } else if (format.compare("HFA") == 0) {
    flag.flagOn(DataType::TL_8U);
    flag.flagOn(DataType::TL_8S);
    flag.flagOn(DataType::TL_16U);
    flag.flagOn(DataType::TL_16S);
    flag.flagOn(DataType::TL_32U);
    flag.flagOn(DataType::TL_32S);
    flag.flagOn(DataType::TL_32F);
    flag.flagOn(DataType::TL_64F);  
  } else if (format.compare("WMS") == 0) {
  
  }
  return flag;
}

/*!
 * \brief Devuelve el nombre del driver de GDAL correspondiente a una extensión de archivo
 * Si la extensión no se correspondo con un driver disponible devuelve nulo.
 * \param ext Extensión del archivo
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


#ifdef TL_HAVE_GDAL

DataType gdalConvertDataType(GDALDataType dataType)
{
  DataType ret;
  switch (dataType) {
  case GDT_Byte:
    ret = DataType::TL_8U;
    break;
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
  case GDT_Float32:
    ret = DataType::TL_32F;
    break;
  case GDT_Float64:
    ret = DataType::TL_64F;
    break;
  default:
    ret = DataType::TL_8U;
    break;
  }
  return ret;
}

GDALDataType dataTypeToGdalDataType(DataType dataType)
{ 
  GDALDataType ret = GDT_Unknown;
  switch (dataType) {
  case DataType::TL_8U:
    ret = GDT_Byte;
    break;
  case DataType::TL_8S:
    ret = GDT_Byte;
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
  default:
    ret = GDT_Unknown;
    break;
  }
  return ret;
}

#endif // TL_HAVE_GDAL

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
    panBandMap = { 1 };
  else if (channels == 3) 
    panBandMap = { 3, 2, 1 };   // Orden de bandas de OpenCV
  else if (channels == 4) 
    panBandMap = { 3, 2, 1, 4 };
#else
  if (channels == 1) 
    panBandMap = { 1 };
  else if (channels == 3) 
    panBandMap = { 1, 2, 3 };
  else if (channels == 4) 
    panBandMap = { 1, 2, 3, 4 };
#endif

  return panBandMap;
}

#if defined TL_HAVE_OPENCV && defined TL_HAVE_GDAL

/*!
 * \brief Obtiene el tipo de dato de OpenCV
 * \param gdalType Tipo de GDAL
 * \param channels Número de canales
 * \return Tipo de OpenCV
 */
int gdalToOpenCv(GDALDataType gdalType, int channels)
{
  int depth;
  if (gdalType == GDT_Byte)
    depth = CV_8U;
  else if (gdalType == GDT_UInt16)
    depth = CV_16U;
  else if (gdalType == GDT_Int16)
    depth = CV_16S;
  else if (gdalType == GDT_UInt32)
    depth = CV_32S;
  else if (gdalType == GDT_Int32)
    depth = CV_32S;
  else if (gdalType == GDT_Float32)
    depth = CV_32F;  
  else if (gdalType == GDT_Float64)
    depth = CV_64F; 
  //else if (gdalType == GDT_CInt16)
  //  depth = CV_16U;   // GDT_CInt16  == 8   CV_16U == 2 
  //else if (gdalType == GDT_CInt32)
  //  depth = CV_32S;   // GDT_CInt32  == 9   CV_32S == 4 
  //else if (gdalType == GDT_CFloat32)
  //  depth = CV_32F;   // GDT_CFloat32==10   CV_32F == 5   
  //else if (gdalType == GDT_CFloat64)
  //  depth = CV_64F;   // GDT_CFloat64==11   CV_64F == 5   
  else
    depth = -1;
  return(CV_MAKETYPE(depth, channels));
}


/*!
 * \brief Pasa del tipo (profundidad de bits) de OpenCV a GDAL
 * \param cvdt Profundidad de bits
 * \return GDALDataType
 */
GDALDataType openCvToGdal(int cvdt)
{
  GDALDataType ret;
  
  if (cvdt == CV_8U)  
    ret = GDT_Byte;      //  CV_8U  == 0     GDT_Byte == 1
  else if (cvdt == CV_8S)  
    ret = GDT_Byte;      //  CV_8S  == 1     GDT_Byte == 1
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

#endif // TL_HAVE_OPENCV


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
