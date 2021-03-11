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
 
#ifndef TL_GRAPHIC_IMG_IO_H
#define TL_GRAPHIC_IMG_IO_H

#include "config_tl.h"

#include <memory>
#include <array>
//#include <mutex>

#include "tidop/core/defs.h"


#ifdef HAVE_OPENCV
#include "opencv2/core/core.hpp"
#endif // HAVE_OPENCV

#ifdef HAVE_GDAL
TL_SUPPRESS_WARNINGS
#include "gdal_priv.h"
#include "cpl_conv.h"
TL_DEFAULT_WARNINGS
#endif // HAVE_GDAL

#include "tidop/core/utils.h"
//#include "graphic_entities/color.h"
#include "tidop/geometry/entities/point.h"
#include "tidop/geometry/transform.h"
#include "tidop/img/img.h"
//#include "img/metadata.h"

#ifdef HAVE_EDSDK
#include "EDSDK.h"
#ifndef HAVE_RAW
#  define HAVE_RAW
#endif
#endif // HAVE_EDSDK

#ifdef HAVE_LIBRAW
TL_SUPPRESS_WARNINGS
#include "libraw.h"
TL_DEFAULT_WARNINGS
#ifndef HAVE_RAW
#  define HAVE_RAW
#endif
#endif // HAVE_RAW

namespace tl
{

class RasterOptions;

//enum class DataType : int8_t
//{
//  TL_8U,      // Equivalente a CV_8U y GDT_Byte
//  TL_8S,      // Equivalente a CV_8S
//  TL_16U,     // Equivalente a CV_16U y GDT_UInt16
//  TL_16S,     // Equivalente a CV_16S y GDT_Int16
//  TL_32U,     // Equivalente a GDT_UInt32
//  TL_32S,     // Equivalente a CV_32S y GDT_Int32
//  TL_32F,     // Equivalente a CV_32F y GDT_Float32  
//  TL_64F      // Equivalente a CV_64F y GDT_Float64
//};


class TL_EXPORT VrtRaster
  : public File
{

public:

  /*!
   * \brief Constructora
   */
  VrtRaster();

  /*!
   * \brief Destructora
   */
  virtual ~VrtRaster(){}

  /*!
   * \brief Crea una imagen
   * \param[in] rows Número de filas de la imagen
   * \param[in] cols Número de columnas de la imagen
   * \param[in] bands Número de bandas de la imagen
   * \param[in] type
   * \return
   */
  virtual Status create(int rows, int cols, int bands, DataType type) = 0;

#ifdef HAVE_OPENCV

  /*!
   * \brief Lee el fragmento de imagen correspondiente a una ventana
   * \param[out] image Imagen que se lee
   * \param[in] wLoad Ventana de la imagen que se quiere cargar
   * \param[in] scale Escala entre la imagen real y la que se lee. Por defecto 1
   * \param[out] trf Transformación que hay que aplicar a la imagen devuelta
   */
  virtual Status read(cv::Mat *image, const WindowI &wLoad = WindowI(), double scale = 1., Helmert2D<PointI> *trf = NULL) = 0;

  /*!
   * \brief Escribe en la imagen
   * \param[in] image Bloque de imagen que se escribe
   * \param[in] w Ventana del bloque de imagen que se escribe. Por defecto toda la imagen
   * \return
   */
  virtual Status write(const cv::Mat &image, const WindowI &w = WindowI()) = 0;

  /*!
   * \brief Escribe en la imagen
   * \param[in] image Bloque de imagen que se escribe
   * \param[in] trf Transformación entre el bloque y la imagen. Si es nula no se aplica transformación
   * \return
   */
  virtual Status write(const cv::Mat &image, const Helmert2D<PointI> *trf = NULL) = 0;

#else

  /*!
   * \brief Lee el fragmento de imagen correspondiente a una ventana
   * \param[out] image Imagen que se lee
   * \param[in] wRead Ventana de la imagen que se quiere cargar. Por defecto toda la ventana
   * \param[in] scale Escala entre la imagen real y la que se lee. Por defecto 1
   * \param[out] trf Transformación que hay que aplicar a la imagen devuelta
   */
  virtual Status read(unsigned char *buff, const WindowI &wLoad = WindowI(), double scale = 1., Helmert2D<PointI> *trf = NULL) = 0;

  /*!
   * \brief Escribe en la imagen
   * \param[in] image Bloque de imagen que se escribe
   * \param[in] w Ventana del bloque de imagen que se escribe
   * \return
   */
  virtual Status write(const unsigned char *buff, const WindowI &w) = 0;

  /*!
   * \brief Escribe en la imagen
   * \param[in] image Bloque de imagen que se escribe
   * \param[in] trf Transformación entre el bloque y la imagen. Si es nula no se aplica transformación
   * \return
   */
  virtual Status write(const unsigned char *buff, const Helmert2D<PointI> *trf = nullptr) = 0;


#endif

  /*!
   * \brief Devuelve el número de filas de la imagen
   * \return Número de filas de la imagen
   */
  virtual int rows() const;

  /*!
   * \brief Devuelve el número de columnas de la imagen
   * \return Número de columnas de la imagen
   */
  virtual int cols() const;

  /*!
   * \brief Devuelve el número de canales o bandas de la imagen
   * \return Número de bandas de la imagen
   */
  virtual int channels() const;

  /*!
   * \brief Devuelve el tipo de dato
   * \return 
   */
  virtual DataType dataType() const;

  /*!
   * \brief Devuelve la profundidad de color o bits por pixel de una imagen
   * \return Profundidad de color
   */
  virtual int depth() const;


#ifdef TL_ENABLE_DEPRECATED_METHODS

  /*!
   * \brief Devuelve el número de filas de la imagen
   * \return Número de filas de la imagen
   * \deprecated Use 'rows' en su lugar
   */
  TL_DEPRECATED("rows", "2.0")
  virtual int getRows() const;

  /*!
   * \brief Devuelve el número de columnas de la imagen
   * \return Número de columnas de la imagen
   * \deprecated Use 'cols' en su lugar
   */
  TL_DEPRECATED("cols", "2.0")
  virtual int getCols() const;

  /*!
   * \brief Devuelve el número de canales o bandas de la imagen
   * \return Número de bandas de la imagen
   * \deprecated Use 'channels' en su lugar
   */
  TL_DEPRECATED("channels", "2.0")
  virtual int getBands() const;

  /*!
   * \brief Devuelve el tipo de dato
   * \return 
   * \deprecated Use 'dataType' en su lugar
   */
  TL_DEPRECATED("dataType", "2.0")
  virtual DataType getDataType() const;

  /*!
   * \brief Devuelve la profundidad de color o bits por pixel de una imagen
   * \return Profundidad de color
   * \deprecated Use 'depth' en su lugar
   */
  TL_DEPRECATED("depth", "2.0")
  virtual int getColorDepth() const;

#endif // TL_ENABLE_DEPRECATED_METHODS

  /*!
   * \brief Devuelve los metadatos de la imagen
   * \return Metadatos de la imagen
   */
  //virtual ImgMetadata metadata() const = 0;

  /*!
   * \brief Valor del punto
   * \param[in] pt
   * \return
   */
  virtual char get(const PointI &pt) = 0;

protected:
  
  void windowRead(const WindowI &wLoad, WindowI *wRead, PointI *offset) const;

protected:
  
  /*!
   * \brief Número de filas de la imagen
   */
  int mRows;

  /*!
   * \brief Número de columnas de la imagen
   */
  int mCols;

  /*!
   * \brief Número de bandas o canales de la imagen
   */
  int mBands;

  /*!
   * \brief Tipo de datos
   * \see DataType
   */
  DataType mDataType;

  /*!
   * \brief Profundidad de color o número de bits por pixel
   */
  int mColorDepth;

};




#ifdef HAVE_GDAL

/*!
 * \brief Ficheros imagen que son gestionados por GDAL
 */
class TL_EXPORT GdalRaster
  : public VrtRaster
{

public:

  /*!
   * \brief Constructora
   */
  GdalRaster();

  /*!
   * \brief Constructor de copia
   * \param gdalRaster Objeto GdalRaster
   */
  GdalRaster(const GdalRaster &gdalRaster);

  /*!
   * \brief Destructora
   */
  virtual ~GdalRaster();

  /*!
   * \brief Cierra el fichero imagen
   */
  void close() override;

  /*!
   * \brief Abre un fichero imagen especificando las opciones del formato
   * \param[in] file Fichero
   * \param[in] mode Modo de apertura
   * \param[in] options Opciones del formato
   * \return
   * \see Mode
   */
  Status open(const std::string &file, Mode mode = Mode::update, FileOptions *options = nullptr) override;

  /*!
   * \brief Crea una imagen
   * \param[in] rows Número de filas de la imagen
   * \param[in] cols Número de columnas de la imagen
   * \param[in] bands Número de bandas de la imagen
   * \param[in] type
   * \return
   */
  Status create(int rows, int cols, int bands, DataType type) override;

#ifdef HAVE_OPENCV  

  /*!
   * \brief Lee el fragmento de imagen correspondiente a una ventana
   * \param[out] image Imagen que se lee
   * \param[in] wLoad Ventana de la imagen que se quiere cargar
   * \param[in] scale Escala entre la imagen real y la que se lee. Por defecto 1
   * \param[out] trf Transformación que hay que aplicar a la imagen devuelta
   */
  Status read(cv::Mat *image, const WindowI &wLoad = WindowI(), double scale = 1., Helmert2D<PointI> *trf = nullptr) override;

  /*!
   * \brief Escribe en la imagen
   * \param[in] image Bloque de imagen que se escribe
   * \param[in] w Ventana del bloque de imagen que se escribe. Por defecto toda la imagen
   * \return
   */
  Status write(const cv::Mat &image, const WindowI &w = WindowI()) override;

  /*!
   * \brief Escribe en la imagen
   * \param[in] image Bloque de imagen que se escribe
   * \param[in] trf Transformación entre el bloque y la imagen. Si es nula no se aplica transformación
   * \return
   */
  Status write(const cv::Mat &image, const Helmert2D<PointI> *trf = NULL) override;

#else

  /*!
   * \brief Lee el fragmento de imagen correspondiente a una ventana
   * \param[out] image Imagen que se lee
   * \param[in] wRead Ventana de la imagen que se quiere cargar
   * \param[in] scale Escala entre la imagen real y la que se lee. Por defecto 1
   * \param[out] trf Transformación que hay que aplicar a la imagen devuelta
   */
  Status read(unsigned char *buff, const WindowI &wLoad = WindowI(), double scale = 1., Helmert2D<PointI> *trf = NULL) override;

  /*!
   * \brief Escribe en la imagen
   * \param[in] image Bloque de imagen que se escribe
   * \param[in] w Ventana del bloque de imagen que se escribe
   * \return
   */
  Status write(const unsigned char *buff, const WindowI &w) override;

  /*!
   * \brief Escribe en la imagen
   * \param[in] image Bloque de imagen que se escribe
   * \param[in] trf Transformación entre el bloque y la imagen. Si es nula no se aplica transformación
   * \return
   */
  Status write(const unsigned char *buff, const Helmert2D<PointI> *trf = nullptr) override;


#endif

  /*!
   * \brief Guarda una copia con otro nonbre
   */
  Status createCopy(const std::string &fileOut) override;

  /*!
   * \brief Devuelve el nombre del driver de GDAL correspondiente a una extensión de archivo
   * Si la extensión no se correspondo con un driver disponible devuelve nulo.
   * \param ext Extensión del archivo
   * \return Nombre del Driver de GDAL
   */
  static const char *getDriverFromExt(const char *ext);

  //ImgMetadata metadata() const override;

  char get(const PointI &pt) override;

protected:

  virtual void update();

  std::vector<int> panBandMap();

protected:

  /*!
   * \brief Crea fichero temporal
   */
  bool bTempFile;

  /*!
   * \brief Dataset de GDAL
   */
  GDALDataset *pDataset;

  /*!
   * \brief pRasterBand
   */
  GDALRasterBand  *pRasterBand;

  /*!
   * \brief mDataType
   */
  GDALDataType mGdalDataType;

  /*!
   * \brief Nombre del fichero temporal
   */
  std::string mTempName;

  /*!
   * \brief Driver GDAL
   */
  GDALDriver *pDriver;

  static char **gdalOpt;
  static int BMP[];
  static int GTiff[];
  static int GTX[];
  static int NTv2[];
  static int LAN[];

};


/*!
 * \brief Clase contenedor y de gestión de imagenes raster georeferenciadas
 */
class TL_EXPORT GdalGeoRaster
  : public GdalRaster
{

  std::array<double, 6> mGeoTransform;

  /*!
   * \brief Transformación afin que relacción las coordenadas terreno con las pixel
   */
  std::unique_ptr<Affine<PointD>> mTrfAffine;

  /*!
   * \brief Proyección
   */
  std::string mProjection;

public:

  /*!
   * \brief Constructora GdalGeoRaster
   */
  GdalGeoRaster();

  /*!
   * \brief Destructora
   */
  ~GdalGeoRaster() override {}

  /*!
   * \brief Georeferencia de la imagen
   * Los valores del array se corresponden con:
   * 1 - Coordenada X de la esquina superior izquierda
   * 2 - Resolución en x
   * 3 - Giro x
   * 4 - Coordenada Y de la esquina superior izquierda
   * 5 - Giro y
   * 6 - Resolución en y.
   * \return Georeferencia
   */
  std::array<double, 6> georeference() const;

  /*!
   * \brief Proyección
   * \return
   */
  const char *projection() const;

  /*!
   * \brief Ventana envolvente de la imagen
   * \return Ventana
   */
  geometry::WindowD window() const;

#ifdef TL_ENABLE_DEPRECATED_METHODS

  /*!
   * \brief Ventana envolvente de la imagen
   * \return Ventana
   * \deprecated Use 'window' en su lugar
   */
  TL_DEPRECATED("window", "2.0")
  geometry::WindowD getWindow() const;

#endif // TL_ENABLE_DEPRECATED_METHODS

  /*!
   * \brief Establece la georeferencia de la imagen
   * \param[in] georef Georeferencia
   */
  void setGeoreference(const std::array<double, 6> &georef);

  /*!
   * \brief Establece la proyección de la imagen
   * \param[in] proj Proyección
   */
  void setProjection(const char *proj);

#ifdef HAVE_OPENCV

  /*!
   * \brief Carga el trozo de imagen correspondiente a una ventana
   * \param[out] image
   * \param[in] wTerrain Ventana en coordenadas terreno de la imagen que se quiere cargar
   * \param[in] scale Escala entre la imagen real y la que se lee. Por defecto 1
   */
  Status read(cv::Mat *image, const Window<Point<double>> &wTerrain, double scale = 1.);

#endif // HAVE_OPENCV

  char get(const PointD &pt);

  float getZ(const PointD &pt);

private:

  void update() override;

protected:

  std::array<double, 6> mGeoTransform;

  /*!
   * \brief Transformación afin que relacción las coordenadas terreno con las pixel
   */
  std::unique_ptr<Affine<geometry::PointD>> mTrfAffine;

  /*!
   * \brief Proyección
   */
  std::string mProjection;

};




#endif // HAVE_GDAL

#ifdef HAVE_RAW


#ifdef HAVE_EDSDK

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

#endif \\ HAVE_EDSDK 


// http://www.libraw.org/node/2165
class TL_EXPORT RawImage
  : public VrtRaster
{

public:

  /*!
   * \brief Constructora
   */
  RawImage();

  /*!
   * \brief Destructora
   */
  ~RawImage();

  /*!
   * \brief Cierra el fichero Raw
   */
  void close() override;

  /*!
   * \brief Abre el fichero Raw
   * \param[in] file Nombre del fichero
   * \param[in] mode Modo de apertura
   * \return
   * \see Mode
   */
  Status open(const std::string &file, Mode mode = Mode::Read, FileOptions *options = nullptr) override;

  /*!
   * \brief Crea una imagen raw
   * \param[in] row Número de filas de la imagen
   * \param[in] col Número de columnas de la imagen
   * \param[in] bands Número de bandas de la imagen
   * \param[in] type
   * \return
   */
  Status create(int rows, int cols, int bands, DataType type) override;

#ifdef HAVE_OPENCV  

  /*!
    * \brief Lee el fragmento de imagen correspondiente a una ventana
    * \param[out] image Imagen que se lee
    * \param[in] wRead Ventana de la imagen que se quiere cargar
    * \param[in] scale Escala entre la imagen real y la que se lee. Por defecto 1
    * \param[out] trf Transformación que hay que aplicar a la imagen devuelta
    */
  Status read(cv::Mat *image, const WindowI &wLoad = WindowI(), double scale = 1., Helmert2D<PointI> *trf = NULL) override;

  /*!
   * \brief Escribe en la imagen
   * \param[in] image Bloque de imagen que se escribe
   * \param[in] w Ventana del bloque de imagen que se escribe. Por defecto toda la imagen
   * \return
   */
  Status write(const cv::Mat &image, const WindowI &w) override;

  /*!
   * \brief Escribe en la imagen
   * \param[in] image Bloque de imagen que se escribe
   * \param[in] trf Transformación entre el bloque y la imagen. Si es nula no se aplica transformación
   * \return
   */
  Status write(const cv::Mat &image, const Helmert2D<PointI> *trf = NULL) override;

#else

  /*!
   * \brief Lee el fragmento de imagen correspondiente a una ventana
   * \param[out] image Imagen que se lee
   * \param[in] wRead Ventana de la imagen que se quiere cargar
   * \param[in] scale Escala entre la imagen real y la que se lee. Por defecto 1
   * \param[out] trf Transformación que hay que aplicar a la imagen devuelta
   */
  Status read(uchar *buff, const WindowI &wLoad = WindowI(), double scale = 1., Helmert2D<PointI> *trf = NULL) override;

  /*!
   * \brief Escribe en la imagen
   * \param[in] image Bloque de imagen que se escribe
   * \param[in] w Ventana del bloque de imagen que se escribe. Por defecto toda la imagen
   * \return
   */
  Status write(const uchar *buff, const WindowI &w) override;

  /*!
   * \brief Escribe en la imagen
   * \param[in] image Bloque de imagen que se escribe
   * \param[in] trf Transformación entre el bloque y la imagen. Si es nula no se aplica transformación
   * \return
   */
  Status write(const uchar *buff, const Helmert2D<PointI> *trf = NULL) override;


#endif

  /*!
   * \brief Guarda una copia con otro nonbre
   */
  Status createCopy(const std::string &fileOut) override;

  /*!
   * \brief Comprueba si una extensión de archivo se corresponde con una del formato RAW
   * Si la extensión no se corresponde con el formato raw devuelve nulo.
   * \param ext Extensión del archivo
   * \return verdadero si es raw
   */
  static int isRawExt(const char *ext);

  ImgMetadata metadata() const override;

  uchar get(const PointI &pt) const override;

private:

  void update();

private:

#ifdef HAVE_LIBRAW 
  std::unique_ptr<LibRaw> mRawProcessor;

  libraw_processed_image_t *mProcessedImage;
#endif //HAVE_LIBRAW

#ifdef HAVE_EDSDK
  EdsStreamRef mInputStream;
  EdsImageRef mEdsImage;
#endif 

  //unsigned short mBits;
  //LibRaw_image_formats mType;
  //unsigned char *mData;
  unsigned int mSize;

  /*!
   * \brief Formato raw especifico de canon.
   * Se lee con el EDSDK
   */
  bool bCanon;

};

#endif // HAVE_RAW





/*!
 * \brief Clase contenedor y de gestión de imagenes raster
 *
 * Esta clase permite crear, abrir y guardar una imagen raster. Se puede
 * realizar la conversión entre formatos guardando una imagen abierta con
 * otra extensión.
 *
 * Una vez abierta la imagen se puede cargar la totalidad o parte de una
 * imagen. Se puede cargar a resolución completa o a otra resolución
 */
class TL_EXPORT RasterGraphics
  : public File
{

public:

  /*!
   * \brief Constructor de la clase RasterGraphics
   */
  RasterGraphics();

  /*!
   * \brief Destructora de la clase RasterGraphics
   */
  ~RasterGraphics();

  /*!
   * \brief Cierra el archivo imagen
   */
  virtual void close() override;

  /*!
   * \brief Abre un archivo imagen especificando las opciones del formato
   * \param[in] file Nombre del fichero
   * \param[in] mode Modo de apertura   
   * \param[in] options Opciones del formato
   * \return Error
   * \see Mode
   */
  Status open(const std::string &file, Mode mode = Mode::update, FileOptions *options = nullptr) override;

  /*!
   * \brief Crea una imagen
   * \param[in] rows Número de filas de la imagen
   * \param[in] cols Número de columnas de la imagen
   * \param[in] bands Número de bandas de la imagen
   * \param[in] type Tipo de datos
   */
  Status create(int rows, int cols, int bands, DataType type); //... No me convence el nombre

#ifdef HAVE_OPENCV

  /*!
   * \brief Lee el fragmento de imagen correspondiente a una ventana
   * \param[out] image Imagen que se lee
   * \param[in] wLoad Ventana de la imagen que se quiere cargar
   * \param[in] scale Escala entre la imagen real y la que se lee. Por defecto 1
   * \param[out] trf Transformación que hay que aplicar a la imagen devuelta
   */
  Status read(cv::Mat *image, const WindowI &wLoad, double scale = 1., Helmert2D<PointI> *trf = NULL);

  /*!
   * \brief Escribe en la imagen
   * \param[in] image Bloque de imagen que se escribe
   * \param[in] w Ventana del bloque de imagen que se escribe
   */
  Status write(const cv::Mat &image, const WindowI &w);

  /*!
   * \brief Escribe en la imagen
   * \param[in] image Bloque de imagen que se escribe
   * \param[in] trf Transformación entre el bloque y la imagen. Si es nula no se aplica transformación 
   */
  Status write(const cv::Mat &image, Helmert2D<PointI> *trf = NULL);

#else

  /*!
   * \brief Lee el fragmento de imagen correspondiente a una ventana
   * \param[out] image Imagen que se lee
   * \param[in] wRead Ventana de la imagen que se quiere cargar
   * \param[in] scale Escala entre la imagen real y la que se lee. Por defecto 1
   * \param[out] trf Transformación que hay que aplicar a la imagen devuelta
   */
  Status read(unsigned char *buff, const WindowI &wLoad, double scale = 1., Helmert2D<PointI> *trf = NULL);

  /*!
   * \brief Escribe en la imagen
   * \param[in] image Bloque de imagen que se escribe
   * \param[in] w Ventana del bloque de imagen que se escribe
   */
  Status write(const unsigned char *buff, const WindowI &w);

  /*!
   * \brief Escribe en la imagen
   * \param[in] image Bloque de imagen que se escribe
   * \param[in] trf Transformación entre el bloque y la imagen. Si es nula no se aplica transformación 
   */
  Status write(const unsigned char *buff, Helmert2D<PointI> *trf = NULL);

#endif // HAVE_OPENCV

  /*!
   * \brief Guarda una imagen con otro nombre o con otro formato
   * \param[in] fileOut Nombre con el que se guarda el fichero
   * \return
   */
  virtual Status createCopy(const std::string &fileOut) override;

  /*!
   * \brief Devuelve el número de filas de la imagen
   * \return Número de filas de la imagen
   */
  virtual int rows() const;

  /*!
   * \brief Devuelve el número de columnas de la imagen
   * \return Número de columnas de la imagen
   */
  virtual int cols() const;

  /*!
   * \brief Devuelve el número de canales o bandas de la imagen
   * \return Número de bandas de la imagen
   */
  virtual int channels() const;

  /*!
   * \brief Devuelve el tipo de dato
   * \return 
   */
  virtual DataType dataType() const;

  /*!
   * \brief Devuelve la profundidad de color o bits por pixel de una imagen
   * \return Profundidad de color
   */
  virtual int depth() const;

#ifdef TL_ENABLE_DEPRECATED_METHODS

  /*!
   * \brief Devuelve el número de filas de la imagen
   * \return Número de filas de la imagen
   * \deprecated Use 'rows' en su lugar
   */
  TL_DEPRECATED("rows", "2.0")
  int getRows() const;

  /*!
   * \brief Devuelve el número de columnas de la imagen
   * \return Número de columnas de la imagen
   * \deprecated Use 'cols' en su lugar
   */
  TL_DEPRECATED("cols", "2.0")
  int getCols() const;

  /*!
   * \brief Devuelve el número de canales o bandas de la imagen
   * \return Número de bandas de la imagen
   * \deprecated Use 'channels' en su lugar
   */
  TL_DEPRECATED("channels", "2.0")
  int getBands() const;

  /*!
   * \brief Devuelve el tipo de dato
   * \return
   * \deprecated Use 'dataType' en su lugar
   */
  TL_DEPRECATED("dataType", "2.0")

  DataType getDataType() const;

  /*!
   * \brief Devuelve la profundidad de color
   * \return Profundidad de color
   * \deprecated Use 'depth' en su lugar
   */
  TL_DEPRECATED("depth", "2.0")
  int getColorDepth() const;

#endif // TL_ENABLE_DEPRECATED_METHODS

  // Dataset Information

  //ImgMetadata metadata() const;

  char get(const PointI &pt) const;

protected:

  virtual void update();

protected:

  /*!
   * \brief Número de filas de la imagen
   */
  int mRows;

  /*!
   * \brief Número de columnas de la imagen
   */
  int mCols;

  /*!
   * \brief Número de bandas o canales de la imagen
   */
  int mBands;

  DataType mDataType;

  /*!
   * \brief Profundidad de color o número de bits por pixel
   */
  int mColorDepth;

  std::unique_ptr<VrtRaster> mImageFormat;

};


/*!
 * \brief Clase contenedor y de gestión de imagenes raster georeferenciadas
 */
class TL_EXPORT GeoRasterGraphics
  : public RasterGraphics
{

public:

  /*!
   * \brief Constructora
   */
  GeoRasterGraphics() : RasterGraphics() 
  { 
  }

  /*!
   * \brief Destructora
   */
  ~GeoRasterGraphics() override {}

  /*!
   * \brief Abre un fichero imagen especificando las opciones del formato
   * \param[in] file Fichero
   * \param[in] mode Modo de apertura
   * \param[in] options Opciones del formato
   * \return
   * \see Mode
   */
  Status open(const std::string &file, Mode mode = Mode::update, FileOptions *options = nullptr) override;

  /*!
   * \brief georeference
   * \return
   */
  std::array<double, 6> georeference() const;

  /*!
   * \brief projection
   * \return
   */
  const char *projection() const;

  /*!
   * \brief setGeoreference
   * \param georef
   */
  void setGeoreference(const std::array<double, 6> &georef);

  /*!
   * \brief setProjection
   * \param proj
   */
  void setProjection(const char *proj);

#ifdef HAVE_OPENCV

  /*!
   * \brief Carga el trozo de imagen correspondiente a una ventana
   * \param[out] image
   * \param[in] wLoad Ventana en coordenadas terreno de la imagen que se quiere cargar
   * \param[in] scale Escala entre la imagen real y la que se lee. Por defecto 1
   */
  Status read(cv::Mat *image, const WindowD &wLoad, double scale = 1.);

#endif // HAVE_OPENCV

  char get(const PointD &pt) const;

private:

  void update() override;

};


class TL_EXPORT Mdt
  : public GeoRasterGraphics
{
public:

  Mdt() {}

  float getZ(const PointD &pt) const;
};



} // End namespace tl



#endif // TL_GRAPHIC_IMG_IO_H
