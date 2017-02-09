#ifndef I3D_IMG_IO_H
#define I3D_IMG_IO_H

#include <memory>
#include <array>

#include "core/config.h"
#include "core/defs.h"

#ifdef HAVE_OPENCV
#include "opencv2/core/core.hpp"
#endif // HAVE_OPENCV

#ifdef HAVE_GDAL
I3D_SUPPRESS_WARNINGS
#include "gdal_priv.h"
#include "cpl_conv.h"
I3D_DEFAULT_WARNINGS
#endif // HAVE_GDAL


#include "core/defs.h"
#include "graphic_entities/color.h"
#include "geometric_entities/point.h"
#include "transform.h"

namespace I3D
{

#ifdef HAVE_GDAL

class I3D_EXPORT RegisterGdal
{
private:

  static std::unique_ptr<RegisterGdal> sRegisterGdal;

  RegisterGdal() {}

public:

  ~RegisterGdal() {}
  RegisterGdal(RegisterGdal const&) = delete;
  void operator=(RegisterGdal const&) = delete;

  static void init();

private:

};

#endif // HAVE_GDAL


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
class I3D_EXPORT RasterGraphics
{

public:

  enum class Status
  {
    OPEN_OK,
    OPEN_FAIL,
    SAVE_OK,
    SUCCESS,
    FAILURE
  };

  enum class Mode
  {
    Read,
    Update,
    Create
  };

protected:

  /*!
   * \brief Dataset de GDAL
   */
  //std::unique_ptr<GDALDataset> pDataset;
  GDALDataset *pDataset;

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

  bool bTempFile;

  GDALRasterBand  *pRasterBand;

  GDALDataType mDataType;

  std::string mName;

  std::string mTempName;

public:

  /*!
   * \brief Constructo de la clase RasterGraphics
   */
  RasterGraphics() : pDataset(0), mRows(0), mCols(0), mBands(0), bTempFile(false)
  {
    RegisterGdal::init();
  }

  /*!
   * \brief Destructora de la clase RasterGraphics
   */
  ~RasterGraphics();

  Status open(const char *file, Mode mode = Mode::Read);

  /*!
   * \brief Lee el fragmento de imagen correspondiente a una ventana
   * \param[out] image Imagen que se lee
   * \param[in] wRead Ventana de la imagen que se quiere cargar
   * \param[in] scale Escala entre la imagen real y la que se lee. Por defecto 1
   */
  void read(cv::Mat *image, const WindowI &wLoad, double scale = 1.);

  /*!
   * \brief Escribe en la imagen
   * \param[in] image Bloque de imagen que se escribe
   * \param[in] w Ventana del bloque de imagen que se escribe
   */
  Status write(cv::Mat image, WindowI w = WindowI());

  void close();

  /*!
   * \brief Crea una imagen
   * \param[in] row Número de filas de la imagen
   * \param[in] col Número de columnas de la imagen
   * \param[in] bands Número de bandas de la imagen
   * \param[in] type 
   */
  Status create(int rows, int cols, int bands, int type);

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
   * \brief Guarda una imagen con otro nombre o con otro formato
   * \param[in] file Nombre con el que se guarda el fichero
   * \return 
   */
  Status saveAs(const char *file);

  // Dataset Information

  Status readMetadata();

  

protected:

  virtual void update();

private:

  std::vector<int> panBandMap();

};

/*!
 * \brief Clase contenedor y de gestión de imagenes raster georeferenciadas
 */
class I3D_EXPORT GeoRasterGraphics : public RasterGraphics
{
private:

  std::array<double, 6> mGeoTransform;

  std::unique_ptr<Afin<PointD>> mTrfAfin;

public:

  GeoRasterGraphics() : RasterGraphics() 
  { 
    mGeoTransform[0] = 0.;
    mGeoTransform[1] = 1.;
    mGeoTransform[2] = 0.;
    mGeoTransform[3] = 0.;
    mGeoTransform[4] = 0.;
    mGeoTransform[5] = -1.;
    mTrfAfin = std::make_unique<Afin<PointD>>();
  }
  
  ~GeoRasterGraphics() {}

  std::array<double, 6> georeference() const;

  void setGeoreference(const std::array<double, 6> &georef);

  /*!
   * \brief Carga el trozo de imagen correspondiente a una ventana
   * \param[out] image
   * \param[in] wRead Ventana en coordenadas terreno de la imagen que se quiere cargar
   * \param[in] scale Escala entre la imagen real y la que se lee. Por defecto 1
   */
  void loadImage(cv::Mat *image, const WindowD &wLoad, double scale = 1.);

private:

  void update() override;

};

} // End namespace I3D



#endif // I3D_IMG_IO_H
