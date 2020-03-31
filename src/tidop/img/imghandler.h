#ifndef TL_IMAGE_HANDLER_H
#define TL_IMAGE_HANDLER_H

#include "config_tl.h"

#include <memory>

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

#include "tidop/core/utils.h"
#include "tidop/geometry/entities/point.h"
#include "tidop/geometry/transform.h"
#include "tidop/img/img.h"

namespace tl
{



class TL_EXPORT Image
  : public File
{

public:

  Image();
  virtual ~Image();

  /*!
   * \brief Crea una imagen
   * \param[in] rows Número de filas de la imagen
   * \param[in] cols Número de columnas de la imagen
   * \param[in] bands Número de bandas de la imagen
   * \param[in] type Tipo de dato
   * \return
   * \see DataType
   */
  virtual Status create(int rows, int cols, int bands, DataType type) = 0;

#ifdef HAVE_OPENCV

  /*!
   * \brief Lee el fragmento de imagen correspondiente a una ventana
   * \param[out] image Imagen que se lee
   * \param[in] window Ventana de la imagen que se quiere cargar
   * \param[in] scale Escala entre la imagen real y la que se lee. Por defecto 1
   * \param[out] trf Transformación que hay que aplicar a la imagen devuelta
   */
  virtual Status read(cv::Mat *image, 
                      const geometry::WindowI &window = geometry::WindowI(), 
                      double scale = 1., 
                      Helmert2D<geometry::PointI> *trf = nullptr) = 0;

  /*!
   * \brief Escribe en la imagen
   * \param[in] image Bloque de imagen que se escribe
   * \param[in] window Ventana del bloque de imagen que se escribe. Por defecto toda la imagen
   * \return
   */
  virtual Status write(const cv::Mat &image, const geometry::WindowI &window = geometry::WindowI()) = 0;

  /*!
   * \brief Escribe en la imagen
   * \param[in] image Bloque de imagen que se escribe
   * \param[in] trf Transformación entre el bloque y la imagen. Si es nula no se aplica transformación
   * \return
   */
  virtual Status write(const cv::Mat &image, const Helmert2D<geometry::PointI> *trf = nullptr) = 0;

#endif

  /*!
   * \brief Lee el fragmento de imagen correspondiente a una ventana
   * \param[out] image Imagen que se lee
   * \param[in] window Ventana de la imagen que se quiere cargar. Por defecto toda la ventana
   * \param[in] scale Escala entre la imagen real y la que se lee. Por defecto 1
   * \param[out] trf Transformación que hay que aplicar a la imagen devuelta
   */
  virtual Status read(unsigned char *buff, 
                      const geometry::WindowI &window = geometry::WindowI(), 
                      double scale = 1., 
                      Helmert2D<geometry::PointI> *trf = nullptr) = 0;

  /*!
   * \brief Escribe en la imagen
   * \param[in] image Bloque de imagen que se escribe
   * \param[in] window Ventana del bloque de imagen que se escribe
   * \return
   */
  virtual Status write(const unsigned char *buff, const geometry::WindowI &window = geometry::WindowI()) = 0;

  /*!
   * \brief Escribe en la imagen
   * \param[in] image Bloque de imagen que se escribe
   * \param[in] trf Transformación entre el bloque y la imagen. Si es nula no se aplica transformación
   * \return
   */
  virtual Status write(const unsigned char *buff, const Helmert2D<geometry::PointI> *trf = nullptr) = 0;

  /*!
   * \brief Devuelve el número de filas de la imagen
   * \return Número de filas de la imagen
   */
  virtual int rows() const = 0;

  /*!
   * \brief Devuelve el número de columnas de la imagen
   * \return Número de columnas de la imagen
   */
  virtual int cols() const = 0;

  /*!
   * \brief Devuelve el número de canales o bandas de la imagen
   * \return Número de bandas de la imagen
   */
  virtual int channels() const = 0;

  /*!
   * \brief Devuelve el tipo de dato
   * \return 
   */
  virtual DataType dataType() const = 0;

  /*!
   * \brief Devuelve la profundidad de color o bits por pixel de una imagen
   * \return Profundidad de color
   */
  virtual int depth() const = 0;

  virtual std::string metadata(const std::string &item) const = 0;
  virtual void setMetadata(const std::string &tag, const std::string &value) = 0;

protected:
  
  void windowRead(const geometry::WindowI &wLoad, 
                  geometry::WindowI *wRead, 
                  geometry::PointI *offset) const;
};




/*---------------------------------------------------------------------------------*/

#ifdef HAVE_GDAL


class GdalImage
  : public Image
{

public:

  GdalImage();
  GdalImage(const GdalImage &gdalImage);
  ~GdalImage();

// Image interface

public: 

   /*!
   * \brief Abre un fichero imagen especificando las opciones del formato
   * \param[in] file Fichero
   * \param[in] mode Modo de apertura
   * \param[in] options Opciones del formato
   * \return 
   * \see Mode
   */
  //static std::unique_ptr<Image> open(const std::string &file, Mode mode = Mode::update, FileOptions *options = nullptr);

  //virtual bool isOpen() const override;

  virtual Status create(int rows, int cols, int bands, DataType type) override;
  virtual Status read(cv::Mat *image, 
                      const geometry::WindowI &window = geometry::WindowI(), 
                      double scale = 1., 
                      Helmert2D<geometry::PointI> *trf = nullptr) override;
  virtual Status write(const cv::Mat &image, 
                       const geometry::WindowI &window = geometry::WindowI()) override;
  virtual Status write(const cv::Mat &image, 
                       const Helmert2D<geometry::PointI> *trf = nullptr) override;
  virtual Status read(unsigned char *buff, 
                      const geometry::WindowI &window = geometry::WindowI(), 
                      double scale = 1.,
                      Helmert2D<geometry::PointI> *trf = nullptr) override;
  virtual Status write(const unsigned char *buff, 
                       const geometry::WindowI &window = geometry::WindowI()) override;
  virtual Status write(const unsigned char *buff, 
                       const Helmert2D<geometry::PointI> *trf = nullptr) override;
  
  virtual int rows() const override;
  virtual int cols() const override;
  virtual int channels() const override;
  virtual DataType dataType() const override;
  virtual int depth() const override;

// File interface

  virtual Status open(const std::string &file, Mode mode = Mode::read, FileOptions *options = nullptr) override;
  virtual void close() override;
  virtual Status createCopy(const std::string &fileOut) override;

  virtual std::string metadata(const std::string &item) const override;
  virtual void setMetadata(const std::string &tag, const std::string &value) override;

  static std::vector<std::string> validExtensions();
  /*!
   * \brief Devuelve el nombre del driver de GDAL correspondiente a una extensión de archivo
   * Si la extensión no se correspondo con un driver disponible devuelve nulo.
   * \param ext Extensión del archivo
   * \return Nombre del Driver de GDAL
   */
  static const char *driverFromExt(const char *ext);

  static GDALDataType GdalImage::gdalDataType(DataType dataType);
  static DataType convertDataType(GDALDataType dataType);


protected:

  virtual void update();
  int gdalToOpenCv(GDALDataType gdalType, int channels);
  GDALDataType openCvToGdal(int cvdt);

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
  //GDALRasterBand  *pRasterBand;

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

  static char **gdalOpt;
  static int BMP[];
  static int GTiff[];
  static int GTX[];
  static int NTv2[];
  static int LAN[];
};

#endif // HAVE_GDAL


/*---------------------------------------------------------------------------------*/

///TODO: crear interfaz ImageHandler

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
class TL_EXPORT ImageHandler
  : public File
{

public:

  /*!
   * \brief Constructor de la clase ImageHandler
   */
  ImageHandler();

  /*!
   * \brief Destructora de la clase ImageHandler
   */
  virtual ~ImageHandler() override;

#ifdef HAVE_OPENCV

  /*!
   * \brief Lee el fragmento de imagen correspondiente a una ventana
   * \param[out] image Imagen que se lee
   * \param[in] window Ventana de la imagen que se quiere cargar
   * \param[in] scale Escala entre la imagen real y la que se lee. Por defecto 1
   * \param[out] trf Transformación que hay que aplicar a la imagen devuelta
   */
  Status read(cv::Mat *image, const geometry::WindowI &window, 
              double scale = 1., Helmert2D<geometry::PointI> *trf = nullptr);
    
  /*!
   * \brief Escribe en la imagen
   * \param[in] image Bloque de imagen que se escribe
   * \param[in] window Ventana del bloque de imagen que se escribe
   */
  Status write(const cv::Mat &image, const geometry::WindowI &window);

  /*!
   * \brief Escribe en la imagen
   * \param[in] image Bloque de imagen que se escribe
   * \param[in] trf Transformación entre el bloque y la imagen. Si es nula no se aplica transformación 
   */
  Status write(const cv::Mat &image, Helmert2D<geometry::PointI> *trf = nullptr);

#endif // HAVE_OPENCV

  /*!
   * \brief Lee el fragmento de imagen correspondiente a una ventana
   * \param[out] image Imagen que se lee
   * \param[in] window Ventana de la imagen que se quiere cargar
   * \param[in] scale Escala entre la imagen real y la que se lee. Por defecto 1
   * \param[out] trf Transformación que hay que aplicar a la imagen devuelta
   */
  Status read(unsigned char *buff, const geometry::WindowI &window, 
              double scale = 1., Helmert2D<geometry::PointI> *trf = nullptr);
    
  /*!
   * \brief Escribe en la imagen
   * \param[in] image Bloque de imagen que se escribe
   * \param[in] window Ventana del bloque de imagen que se escribe
   */
  Status write(const unsigned char *buff, const geometry::WindowI &window);
    
  /*!
   * \brief Escribe en la imagen
   * \param[in] image Bloque de imagen que se escribe
   * \param[in] trf Transformación entre el bloque y la imagen. Si es nula no se aplica transformación 
   */
  Status write(const unsigned char *buff, Helmert2D<geometry::PointI> *trf = nullptr);

  /*!
   * \brief Devuelve el número de filas de la imagen
   * \return Número de filas de la imagen
   */
  int rows() const;

  /*!
   * \brief Devuelve el número de columnas de la imagen
   * \return Número de columnas de la imagen
   */
  int cols() const;

  /*!
   * \brief Devuelve el número de canales o bandas de la imagen
   * \return Número de bandas de la imagen
   */
  int channels() const;

  /*!
   * \brief Devuelve el tipo de dato
   * \return
   */
  DataType dataType() const;

  /*!
   * \brief Devuelve la profundidad de color
   * \return Profundidad de color
   */
  int depth() const;

  // Dataset Information

  //ImgMetadata metadata() const;

  //char at(const geometry::PointI &pt) const;
  std::string metadata(const std::string &item);
  void setMetadata(const std::string &tag, const std::string &value);

protected:

  void update();
  void init();

// File interface

public:

  virtual Status open(const std::string &file, Mode mode = Mode::read, 
                      FileOptions *options = nullptr) override;
  virtual void close() override;
  virtual Status create(int rows, int cols, int bands, DataType type); //... No me convence el nombre
  virtual Status createCopy(const std::string &fileOut) override;
  
protected:

  std::unique_ptr<Image> mImage;
  std::map<std::string, std::string> mFormats;
};

} // End namespace tl

#endif // TL_IMAGE_HANDLER_H