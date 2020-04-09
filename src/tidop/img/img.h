#ifndef TL_IMG_H
#define TL_IMG_H

#include "config_tl.h"

#include "tidop/core/defs.h"

#include <vector>

#ifdef HAVE_GDAL
TL_SUPPRESS_WARNINGS
#include "gdal.h"
TL_DEFAULT_WARNINGS
#endif // HAVE_GDAL



namespace tl
{

enum class DataType : int8_t
{
  TL_8U,      // Equivalente a CV_8U y GDT_Byte
  TL_8S,      // Equivalente a CV_8S
  TL_16U,     // Equivalente a CV_16U y GDT_UInt16
  TL_16S,     // Equivalente a CV_16S y GDT_Int16
  TL_32U,     // Equivalente a GDT_UInt32
  TL_32S,     // Equivalente a CV_32S y GDT_Int32
  TL_32F,     // Equivalente a CV_32F y GDT_Float32  
  TL_64F      // Equivalente a CV_64F y GDT_Float64
};

#ifdef HAVE_GDAL

//TL_EXPORT std::vector<std::string> gdalValidExtensions();
TL_EXPORT bool gdalValidExtensions(const std::string &extension);

/*!
 * \brief Devuelve el nombre del driver de GDAL correspondiente a una extensión de archivo
 * Si la extensión no se correspondo con un driver disponible devuelve nulo.
 * \param ext Extensión del archivo
 * \return Nombre del Driver de GDAL
 */
TL_EXPORT std::string gdalDriverFromExtension(const std::string &extension);


TL_EXPORT DataType gdalConvertDataType(GDALDataType dataType);

TL_EXPORT GDALDataType dataTypeToGdalDataType(DataType dataType);

TL_EXPORT std::vector<int> gdalBandOrder(int channels);

#ifdef HAVE_OPENCV

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

#endif // HAVE_OPENCV

#endif // HAVE_GDAL

} // End namespace tl

#endif // TL_IMG_H