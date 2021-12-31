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
 * License along with Foobar. If not, see <http://www.gnu.org/licenses/>. *
 *                                                                        *
 * @license LGPL-3.0 <https://www.gnu.org/licenses/lgpl-3.0.html>         *
 *                                                                        *
 **************************************************************************/

#ifndef TL_IMG_H
#define TL_IMG_H

#include "config_tl.h"

#include "tidop/core/defs.h"

#ifdef TL_HAVE_GDAL
TL_SUPPRESS_WARNINGS
#include "gdal.h"
TL_DEFAULT_WARNINGS
#endif // TL_HAVE_GDAL

#include <string>
#include <vector>


namespace tl
{

template<typename T> class EnumFlags;

enum class DataType : int16_t
{
  TL_8U = (1 << 0),      // Equivalente a CV_8U y GDT_Byte
  TL_8S = (1 << 1),      // Equivalente a CV_8S
  TL_16U = (1 << 2),     // Equivalente a CV_16U y GDT_UInt16
  TL_16S = (1 << 3),     // Equivalente a CV_16S y GDT_Int16
  TL_32U = (1 << 4),     // Equivalente a GDT_UInt32
  TL_32S = (1 << 5),     // Equivalente a CV_32S y GDT_Int32
  TL_32F = (1 << 6),     // Equivalente a CV_32F y GDT_Float32  
  TL_64F = (1 << 7)      // Equivalente a CV_64F y GDT_Float64
};


//TL_EXPORT std::vector<std::string> gdalValidExtensions();
TL_EXPORT bool gdalValidExtensions(const std::string &extension);
TL_EXPORT EnumFlags<DataType> gdalValidDataTypes(const std::string &format);

/*!
 * \brief Devuelve el nombre del driver de GDAL correspondiente a una extensión de archivo
 * Si la extensión no se correspondo con un driver disponible devuelve nulo.
 * \param ext Extensión del archivo
 * \return Nombre del Driver de GDAL
 */
TL_EXPORT std::string gdalDriverFromExtension(const std::string &extension);

#ifdef TL_HAVE_GDAL

TL_EXPORT DataType gdalConvertDataType(GDALDataType dataType);
TL_EXPORT GDALDataType dataTypeToGdalDataType(DataType dataType);

#endif // TL_HAVE_GDAL

TL_EXPORT int dataTypeToOpenCVDataType(DataType dataType);
TL_EXPORT DataType openCVDataTypeToDataType(int dataType);

TL_EXPORT std::vector<int> gdalBandOrder(int channels);

#if defined TL_HAVE_OPENCV && defined TL_HAVE_GDAL

/*!
 * \brief Obtiene el tipo de dato de OpenCV
 * \param gdalType Tipo de GDAL
 * \param channels Número de canales
 * \return Tipo de OpenCV
 */
int gdalToOpenCv(GDALDataType gdalType, int channels);


/*!
 * \brief Pasa del tipo (profundidad de bits) de OpenCV a GDAL
 * \param cvdt Profundidad de bits
 * \return GDALDataType
 */
GDALDataType openCvToGdal(int cvdt);

#endif



#ifdef TL_HAVE_EDSDK

/*!
 * \brief Clase singleton para registrar la API de canon
 *
 */
class TL_EXPORT RegisterEDSDK
{

private:

  /*!
   * \brief Constructor privado
   */
  RegisterEDSDK();

public:

  ~RegisterEDSDK();

  RegisterEDSDK(RegisterEDSDK const&) = delete;
  void operator=(RegisterEDSDK const&) = delete;

  /*!
   * \brief Inicio de la API EDSDK
   */
  static void init();

private:

  static std::unique_ptr<RegisterEDSDK> sRegisterEDSDK;
  static std::mutex sMutex;
};

#endif // TL_HAVE_EDSDK


} // End namespace tl

#endif // TL_IMG_H
