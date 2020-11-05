#ifndef TL_IMAGE_READER_H
#define TL_IMAGE_READER_H

#include "config_tl.h"


#ifdef HAVE_OPENCV


#include <string>
#include <memory>

#include "opencv2/core/core.hpp"

#include "tidop/core/defs.h"
#include "tidop/core/utils.h"
#include "tidop/geometry/entities/point.h"
#include "tidop/geometry/transform/affine.h"
#include "tidop/geometry/rect.h"
#include "tidop/img/img.h"

namespace tl
{

class ImageMetadata;

/*!
 * \brief Clase para la lectura de diferentes formatos de imagen
 */
class TL_EXPORT ImageReader
{

public:

  ImageReader(const std::string &fileName);
  virtual ~ImageReader() = default;

  /*!
   * \brief Abre el fichero
   */
  virtual void open() = 0;

  /*!
   * \brief Comprueba si el fichero se ha cargado correctamente
   */
  virtual bool isOpen() = 0;

  /*!
   * \brief Cierra el fichero
   */
  virtual void close() = 0;

  /*!
   * \brief Lee un fragmento de imagen correspondiente a una región
   * \param[in] rect Región de la imagen que se carga. Por defecto toda la imagen
   * \param[in] size Tamaño de la imagen de salida. Por defecto el tamaño de la región de lectura
   * \param[out] trf Transformación que hay que aplicar a la imagen devuelta
   * \return imagen
   */
  virtual cv::Mat read(const Rect<int> &rect = Rect<int>(), 
                       const Size<int> size = Size<int>(), 
                       Affine<PointI> *trf = nullptr) = 0;

  /*!
   * \brief Lee el fragmento de imagen correspondiente a una región
   * \param[in] scaleX Escala horizontal que se aplica a la región leida. Por defecto 1
   * \param[in] scaleY Escala Vertical que se aplica a la región leida. Por defecto 1
   * \param[in] rect Región de la imagen que se carga
   * \param[out] trf Transformación que hay que aplicar a la imagen devuelta
   * \return imagen
   */
  virtual cv::Mat read(double scaleX,
                       double scaleY, 
                       const Rect<int> &rect = Rect<int>(), 
                       Affine<PointI> *trf = nullptr) = 0;

  /*!
   * \brief Lee el fragmento de imagen correspondiente a una ventana
   * \param[in] window Ventana de la imagen que se quiere cargar
   * \param[in] scaleX Escala horizontal que se aplica a la región leida. Por defecto 1
   * \param[in] scaleY Escala Vertical que se aplica a la región leida. Por defecto 1
   * \param[out] trf Transformación que hay que aplicar a la imagen devuelta
   */
  virtual cv::Mat read(const WindowI &window, 
                       double scaleX = 1.,
                       double scaleY = 1., 
                       Affine<PointI> *trf = nullptr) = 0;
 
  /*!
   * \brief Lee el fragmento de imagen correspondiente a una ventana en coordenadas terreno
   * \param[in] terrainWindow Ventana en coordenadas terreno de la imagen que se quiere cargar
   * \param[in] scaleX Escala horizontal que se aplica a la región leida. Por defecto 1
   * \param[in] scaleY Escala Vertical que se aplica a la región leida. Por defecto 1
   * \param[out] trf Transformación que hay que aplicar a la imagen devuelta
   */
  virtual cv::Mat read(const Window<PointD> &terrainWindow, 
                       double scaleX = 1.,
                       double scaleY = 1., 
                       Affine<PointI> *trf = nullptr) = 0;

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

  virtual std::shared_ptr<ImageMetadata> metadata() const = 0;

  /*!
   * \brief Imagen georeferenciada
   */
  virtual bool isGeoreferenced() const = 0;
  
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
  virtual Affine<PointD> georeference() const = 0;

  /*!
   * \brief Sistema de referencia por su código EPSG
   */
  virtual std::string crs() const = 0;

  /*!
   * \brief Ventana envolvente de la imagen en coordenadas terreno
   */
  virtual WindowD window() const = 0;

protected:
  
  void windowRead(const WindowI &wLoad, 
                  WindowI *wRead, 
                  PointI *offset) const;

protected:

  std::string mFileName;
  std::string mEpsgCode;
  Affine<PointD> mAffine;
};


/*!
 * \brief Clase factoria de lectura de formatos de imagen
 */
class TL_EXPORT ImageReaderFactory
{

private:

  ImageReaderFactory() {}

public:

  static std::unique_ptr<ImageReader> createReader(const std::string &fileName);
};

} // End namespace tl

#endif // HAVE_OPENCV

#endif // TL_IMAGE_READER_H
