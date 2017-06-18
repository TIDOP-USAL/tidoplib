#ifndef I3D_IMG_IO_H
#define I3D_IMG_IO_H

#include <memory>
#include <array>

#include "core/config.h"
#include "core/defs.h"
#include "core/utils.h" 

#ifdef HAVE_OPENCV
#include "opencv2/core/core.hpp"
#endif // HAVE_OPENCV

#ifdef HAVE_GDAL
I3D_SUPPRESS_WARNINGS
#include "gdal_priv.h"
#include "cpl_conv.h"
I3D_DEFAULT_WARNINGS
#endif // HAVE_GDAL

#include "graphic_entities/color.h"
#include "geometric_entities/point.h"
#include "transform.h"

#ifdef HAVE_EDSDK
#include "EDSDK.h"
#ifndef HAVE_RAW
#  define HAVE_RAW
#endif
#endif // HAVE_EDSDK

#ifdef HAVE_LIBRAW
I3D_SUPPRESS_WARNINGS
#include "libraw.h"
I3D_DEFAULT_WARNINGS
#ifndef HAVE_RAW
#  define HAVE_RAW
#endif
#endif // HAVE_RAW

namespace I3D
{

enum class DataType : int8_t
{
  I3D_8U,      // Equivalente a CV_8U y GDT_Byte
  I3D_8S,      // Equivalente a CV_8S
  I3D_16U,     // Equivalente a CV_16U y GDT_UInt16
  I3D_16S,     // Equivalente a CV_16S y GDT_Int16
  I3D_32U,     // Equivalente a GDT_UInt32
  I3D_32S,     // Equivalente a CV_32S y GDT_Int32
  I3D_32F,     // Equivalente a CV_32F y GDT_Float32  
  I3D_64F      // Equivalente a CV_64F y GDT_Float64
};


//TODO: Añadir flags de opciones de formato


class I3D_EXPORT RasterOptions
{
public:
  
  enum class Format
  {
    TIFF,
    JPG,
    JP2000,
    PNG,
    BMP
  };

protected:

  Format mFormat;

public:

  RasterOptions(Format format) : mFormat(format) {}
  virtual ~RasterOptions() {}


};


class I3D_EXPORT TiffOptions : RasterOptions
{

public:
  TiffOptions() : RasterOptions(Format::TIFF) {}
  ~TiffOptions() {}

};




class I3D_EXPORT VrtRaster : public File
{

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

public:

  /*!
   * \brief Constructora
   */
  VrtRaster() : File(), mRows(0), mCols(0), mBands(0), mDataType(DataType::I3D_8U), mColorDepth(0) {}

  /*!
   * \brief Destructora
   */
  virtual ~VrtRaster(){}

  /*!
   * \brief Crea una imagen
   * \param[in] row Número de filas de la imagen
   * \param[in] col Número de columnas de la imagen
   * \param[in] bands Número de bandas de la imagen
   * \param[in] type
   * \return
   */
  virtual Status create(int rows, int cols, int bands, DataType type) = 0;

#ifdef HAVE_OPENCV

  /*!
   * \brief Lee el fragmento de imagen correspondiente a una ventana
   * \param[out] image Imagen que se lee
   * \param[in] wRead Ventana de la imagen que se quiere cargar
   * \param[in] scale Escala entre la imagen real y la que se lee. Por defecto 1
   * \param[out] trf Transformación que hay que aplicar a la imagen devuelta
   */
  virtual Status read(cv::Mat *image, const geometry::WindowI &wLoad = geometry::WindowI(), double scale = 1., Helmert2D<geometry::PointI> *trf = NULL) = 0;

  /*!
   * \brief Escribe en la imagen
   * \param[in] image Bloque de imagen que se escribe
   * \param[in] w Ventana del bloque de imagen que se escribe. Por defecto toda la imagen
   * \return
   */
  virtual Status write(const cv::Mat &image, const geometry::WindowI &w = geometry::WindowI()) = 0;

  /*!
   * \brief Escribe en la imagen
   * \param[in] image Bloque de imagen que se escribe
   * \param[in] trf Transformación entre el bloque y la imagen. Si es nula no se aplica transformación
   * \return
   */
  virtual Status write(const cv::Mat &image, const Helmert2D<geometry::PointI> *trf = NULL) = 0;

#else

  /*!
   * \brief Lee el fragmento de imagen correspondiente a una ventana
   * \param[out] image Imagen que se lee
   * \param[in] wRead Ventana de la imagen que se quiere cargar. Por defecto toda la ventana
   * \param[in] scale Escala entre la imagen real y la que se lee. Por defecto 1
   * \param[out] trf Transformación que hay que aplicar a la imagen devuelta
   */
  virtual Status read(uchar *buff, const geometry::WindowI &wLoad = geometry::WindowI(), double scale = 1., Helmert2D<geometry::PointI> *trf = NULL) = 0;

  /*!
   * \brief Escribe en la imagen
   * \param[in] image Bloque de imagen que se escribe
   * \param[in] w Ventana del bloque de imagen que se escribe
   * \return
   */
  virtual int write(const uchar *buff, const geometry::WindowI &w) = 0;

  /*!
   * \brief Escribe en la imagen
   * \param[in] image Bloque de imagen que se escribe
   * \param[in] trf Transformación entre el bloque y la imagen. Si es nula no se aplica transformación
   * \return
   */
  virtual int write(const uchar *buff, const Helmert2D<geometry::PointI> *trf = NULL) = 0;


#endif

  /*!
   * \brief Devuelve el número de filas de la imagen
   * \return Número de filas de la imagen
   */
  virtual int getRows() const { return mRows; }

  /*!
   * \brief Devuelve el número de columnas de la imagen
   * \return Número de columnas de la imagen
   */
  virtual int getCols() const { return mCols; }

  /*!
   * \brief Devuelve el número de canales o bandas de la imagen
   * \return Número de bandas de la imagen
   */
  virtual int getBands() const { return mBands; }

  /*!
   * \brief Devuelve el tipo de dato
   * \return 
   */
  virtual DataType getDataType() const { return mDataType; }

  /*!
   * \brief Devuelve la profundidad de color o bits por pixel de una imangen
   * \return Profundidad de color
   */
  virtual int getColorDepth() const { return mColorDepth; }

protected:
  
  void windowRead(const geometry::WindowI &wLoad, geometry::WindowI *wRead, geometry::PointI *offset) const;
};


#ifdef HAVE_GDAL

/*!
 * \brief Clase singleton para registrar los drivers de GDAL
 *
 */
class I3D_EXPORT RegisterGdal
{
private:

  static std::unique_ptr<RegisterGdal> sRegisterGdal;

  /*!
   * \brief Constructor privado
   */
  RegisterGdal() {}

public:

  ~RegisterGdal() {}

  // Se impide la copia y asignación
  RegisterGdal(RegisterGdal const&) = delete;
  void operator=(RegisterGdal const&) = delete;

  /*!
   * \brief Método para iniciar GDAL una unica vez
   */
  static void init();

};

/*!
 * \brief Ficheros imagen que son gestionados por GDAL
 */
class I3D_EXPORT GdalRaster : public VrtRaster
{

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
   * \brief Abre un fichero imagen
   * \param[in] file Nombre del fichero
   * \param[in] mode Modo de apertura
   * \return
   * \see Mode
   */
  Status open(const char *file, Mode mode = Mode::Read) override;

  /*!
   * \brief Crea una imagen
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
  Status read(cv::Mat *image, const geometry::WindowI &wLoad = geometry::WindowI(), double scale = 1., Helmert2D<geometry::PointI> *trf = NULL) override;

  /*!
   * \brief Escribe en la imagen
   * \param[in] image Bloque de imagen que se escribe
   * \param[in] w Ventana del bloque de imagen que se escribe. Por defecto toda la imagen
   * \return
   */
  Status write(const cv::Mat &image, const geometry::WindowI &w = geometry::WindowI()) override;

  /*!
   * \brief Escribe en la imagen
   * \param[in] image Bloque de imagen que se escribe
   * \param[in] trf Transformación entre el bloque y la imagen. Si es nula no se aplica transformación
   * \return
   */
  Status write(const cv::Mat &image, const Helmert2D<geometry::PointI> *trf = NULL) override;

#else

  /*!
   * \brief Lee el fragmento de imagen correspondiente a una ventana
   * \param[out] image Imagen que se lee
   * \param[in] wRead Ventana de la imagen que se quiere cargar
   * \param[in] scale Escala entre la imagen real y la que se lee. Por defecto 1
   * \param[out] trf Transformación que hay que aplicar a la imagen devuelta
   */
  Status read(uchar *buff, const geometry::WindowI &wLoad = geometry::WindowI(), double scale = 1., Helmert2D<geometry::PointI> *trf = NULL) override;

  /*!
   * \brief Escribe en la imagen
   * \param[in] image Bloque de imagen que se escribe
   * \param[in] w Ventana del bloque de imagen que se escribe
   * \return
   */
  Status write(const uchar *buff, const geometry::WindowI &w) override;

  /*!
   * \brief Escribe en la imagen
   * \param[in] image Bloque de imagen que se escribe
   * \param[in] trf Transformación entre el bloque y la imagen. Si es nula no se aplica transformación
   * \return
   */
  Status write(const uchar *buff, const Helmert2D<geometry::PointI> *trf = NULL) override;


#endif

  /*!
   * \brief Guarda una copia con otro nonbre
   */
  Status createCopy(const char *fileOut) override;

  /*!
   * \brief Devuelve el nombre del driver de GDAL correspondiente a una extensión de archivo
   * Si la extensión no se correspondo con un driver disponible devuelve nulo.
   * \param ext Extensión del archivo
   * \return Nombre del Driver de GDAL
   */
  static const char *getDriverFromExt(const char *ext);

protected:

  virtual void update();

  std::vector<int> panBandMap();
};


/*!
 * \brief Clase contenedor y de gestión de imagenes raster georeferenciadas
 */
class I3D_EXPORT GdalGeoRaster : public GdalRaster
{
private:

  std::array<double, 6> mGeoTransform;

  /*!
   * \brief Transformación afin que relacción las coordenadas terreno con las pixel
   */
  std::unique_ptr<Affine<geometry::PointD>> mTrfAffine;

  /*!
   * \brief Proyección
   */
  std::string mProjection;

public:

  /*!
   * \brief Constructora GdalGeoRaster
   */
  GdalGeoRaster() : GdalRaster() 
  { 
    mGeoTransform[0] = 0.;
    mGeoTransform[1] = 1.;
    mGeoTransform[2] = 0.;
    mGeoTransform[3] = 0.;
    mGeoTransform[4] = 0.;
    mGeoTransform[5] = -1.;
    mTrfAffine = std::make_unique<Affine<geometry::PointD>>();
  }

  /*!
   * \brief Destructora
   */
  ~GdalGeoRaster() {}

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
  geometry::WindowD getWindow() const;

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
  Status read(cv::Mat *image, const geometry::Window<geometry::Point<double>> &wTerrain, double scale = 1.);

#endif // HAVE_OPENCV

private:

  void update() override;

};


#endif // HAVE_GDAL

#ifdef HAVE_RAW


#ifdef HAVE_EDSDK

/*!
 * \brief Clase singleton para registrar la API de canon
 *
 */
class I3D_EXPORT RegisterEDSDK
{
private:

  static std::unique_ptr<RegisterEDSDK> sRegisterEDSDK;

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

};

#endif \\ HAVE_EDSDK 


// http://www.libraw.org/node/2165
class I3D_EXPORT RawImage : public VrtRaster
{
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
  Status open(const char *file, Mode mode = Mode::Read) override;

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
  Status read(cv::Mat *image, const geometry::WindowI &wLoad = geometry::WindowI(), double scale = 1., Helmert2D<geometry::PointI> *trf = NULL) override;

  /*!
   * \brief Escribe en la imagen
   * \param[in] image Bloque de imagen que se escribe
   * \param[in] w Ventana del bloque de imagen que se escribe. Por defecto toda la imagen
   * \return
   */
  Status write(const cv::Mat &image, const geometry::WindowI &w) override;

  /*!
   * \brief Escribe en la imagen
   * \param[in] image Bloque de imagen que se escribe
   * \param[in] trf Transformación entre el bloque y la imagen. Si es nula no se aplica transformación
   * \return
   */
  Status write(const cv::Mat &image, const Helmert2D<geometry::PointI> *trf = NULL) override;

#else

  /*!
   * \brief Lee el fragmento de imagen correspondiente a una ventana
   * \param[out] image Imagen que se lee
   * \param[in] wRead Ventana de la imagen que se quiere cargar
   * \param[in] scale Escala entre la imagen real y la que se lee. Por defecto 1
   * \param[out] trf Transformación que hay que aplicar a la imagen devuelta
   */
  Status read(uchar *buff, const geometry::WindowI &wLoad = geometry::WindowI(), double scale = 1., Helmert2D<geometry::PointI> *trf = NULL) override;

  /*!
   * \brief Escribe en la imagen
   * \param[in] image Bloque de imagen que se escribe
   * \param[in] w Ventana del bloque de imagen que se escribe. Por defecto toda la imagen
   * \return
   */
  Status write(const uchar *buff, const geometry::WindowI &w) override;

  /*!
   * \brief Escribe en la imagen
   * \param[in] image Bloque de imagen que se escribe
   * \param[in] trf Transformación entre el bloque y la imagen. Si es nula no se aplica transformación
   * \return
   */
  Status write(const uchar *buff, const Helmert2D<geometry::PointI> *trf = NULL) override;


#endif

  /*!
   * \brief Guarda una copia con otro nonbre
   */
  Status createCopy(const char *fileOut) override;

  /*!
   * \brief Comprueba si una extensión de archivo se corresponde con una del formato RAW
   * Si la extensión no se corresponde con el formato raw devuelve nulo.
   * \param ext Extensión del archivo
   * \return verdadero si es raw
   */
  static int isRawExt(const char *ext);

private:

  void update();

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
class I3D_EXPORT RasterGraphics : public File
{

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
   * \brief Abre un archivo imagen
   * \param file Nombre del fichero
   * \param mode Modo de apertura del fichero
   * \return Status
   * \see Mode
   */
  virtual Status open(const char *file, Mode mode = Mode::Read) override;

  /*!
   * \brief Crea una imagen
   * \param[in] row Número de filas de la imagen
   * \param[in] col Número de columnas de la imagen
   * \param[in] bands Número de bandas de la imagen
   * \param[in] type Tipo de datos
   */
  Status create(int rows, int cols, int bands, DataType type); //... No me convence el nombre

#ifdef HAVE_OPENCV

  /*!
   * \brief Lee el fragmento de imagen correspondiente a una ventana
   * \param[out] image Imagen que se lee
   * \param[in] wRead Ventana de la imagen que se quiere cargar
   * \param[in] scale Escala entre la imagen real y la que se lee. Por defecto 1
   * \param[out] trf Transformación que hay que aplicar a la imagen devuelta
   */
  Status read(cv::Mat *image, const geometry::WindowI &wLoad, double scale = 1., Helmert2D<geometry::PointI> *trf = NULL);

  /*!
   * \brief Escribe en la imagen
   * \param[in] image Bloque de imagen que se escribe
   * \param[in] w Ventana del bloque de imagen que se escribe
   */
  Status write(const cv::Mat &image, const geometry::WindowI &w);

  /*!
   * \brief Escribe en la imagen
   * \param[in] image Bloque de imagen que se escribe
   * \param[in] trf Transformación entre el bloque y la imagen. Si es nula no se aplica transformación 
   */
  Status write(const cv::Mat &image, Helmert2D<geometry::PointI> *trf = NULL);

#else

  /*!
   * \brief Lee el fragmento de imagen correspondiente a una ventana
   * \param[out] image Imagen que se lee
   * \param[in] wRead Ventana de la imagen que se quiere cargar
   * \param[in] scale Escala entre la imagen real y la que se lee. Por defecto 1
   * \param[out] trf Transformación que hay que aplicar a la imagen devuelta
   */
  Status read(uchar *buff, const geometry::WindowI &wLoad, double scale = 1., Helmert2D<geometry::PointI> *trf = NULL);

  /*!
   * \brief Escribe en la imagen
   * \param[in] image Bloque de imagen que se escribe
   * \param[in] w Ventana del bloque de imagen que se escribe
   */
  Status write(const uchar *buff, const geometry::WindowI &w);

  /*!
   * \brief Escribe en la imagen
   * \param[in] image Bloque de imagen que se escribe
   * \param[in] trf Transformación entre el bloque y la imagen. Si es nula no se aplica transformación 
   */
  Status write(const uchar *buff, Helmert2D<geometry::PointI> *trf = NULL);

#endif // HAVE_OPENCV

  /*!
   * \brief Guarda una imagen con otro nombre o con otro formato
   * \param[in] file Nombre con el que se guarda el fichero
   * \return
   */
  virtual Status createCopy(const char *fileOut) override;

  /*!
   * \brief Devuelve el número de filas de la imagen
   * \return Número de filas de la imagen
   */
  int getRows() const;

  /*!
   * \brief Devuelve el número de columnas de la imagen
   * \return Número de columnas de la imagen
   */
  int getCols() const;

  /*!
   * \brief Devuelve el número de canales o bandas de la imagen
   * \return Número de bandas de la imagen
   */
  int getBands() const;

  /*!
   * \brief Devuelve el tipo de dato
   * \return
   */
  DataType getDataType() const;

  /*!
   * \brief Devuelve la profundidad de color
   * \return Profundidad de color
   */
  int getColorDepth() const;

  // Dataset Information

  //Status readMetadata();

protected:

  virtual void update();

};


/*!
 * \brief Clase contenedor y de gestión de imagenes raster georeferenciadas
 */
class I3D_EXPORT GeoRasterGraphics : public RasterGraphics
{
private:

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
  ~GeoRasterGraphics() {}

  /*!
   * \brief open
   * \param file
   * \param mode
   * \return
   */
  File::Status open(const char *file, Mode mode = Mode::Read) override;

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
   * \param[in] wRead Ventana en coordenadas terreno de la imagen que se quiere cargar
   * \param[in] scale Escala entre la imagen real y la que se lee. Por defecto 1
   */
  Status read(cv::Mat *image, const geometry::WindowD &wLoad, double scale = 1.);

#endif // HAVE_OPENCV

private:

  void update() override;

};






} // End namespace I3D



#endif // I3D_IMG_IO_H
