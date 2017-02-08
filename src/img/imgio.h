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

namespace I3D
{

template<typename T> class Afin;

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

  GDALRasterBand  *pRasterBand;

  GDALDataType mDataType;

public:

  /*!
   * \brief Constructo de la clase RasterGraphics
   */
  RasterGraphics() : pDataset(0), mRows(0), mCols(0), mBands(0)
  {
    RegisterGdal::init();
  }

  /*!
   * \brief Destructora de la clase RasterGraphics
   */
  ~RasterGraphics();

  Status open(char *file, Mode mode = Mode::Read);
  Status save(char *file);


  // Dataset Information

  Status readMetadata();

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

#ifdef HAVE_OPENCV

  void loadImage(cv::Mat *image, const WindowI &w, double scale);

#endif // HAVE_OPENCV


protected:

  virtual void update();

private:

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

private:

  void update() override;

};

} // End namespace I3D



#endif // I3D_IMG_IO_H
