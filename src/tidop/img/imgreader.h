#ifndef TL_IMAGE_READER_H
#define TL_IMAGE_READER_H

#include "config_tl.h"

#include <string>
#include <memory>

#ifdef HAVE_OPENCV
#include "opencv2/core/core.hpp"
#endif // HAVE_OPENCV

#include "tidop/core/defs.h"
#include "tidop/core/utils.h"
#include "tidop/geometry/entities/point.h"
#include "tidop/geometry/transform.h"
#include "tidop/geometry/rect.h"
#include "tidop/img/img.h"

namespace tl
{


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

#ifdef HAVE_OPENCV

  /*!
   * \brief Lee el fragmento de imagen correspondiente a una región
   * \param[out] image Imagen que se lee
   * \param[in] rect Región de la imagen que se carga
   * \param[in] scaleX Escala horizontal que se aplica a la región leida. Por defecto 1
   * \param[in] scaleY Escala Vertical que se aplica a la región leida. Por defecto 1
   * \param[out] trf Transformación que hay que aplicar a la imagen devuelta
   */
  virtual void read(cv::Mat &image, 
                    const Rect<int> &rect = Rect<int>(), 
                    double scaleX = 1.,
                    double scaleY = 1., 
                    Affine<PointI> *trf = nullptr) = 0;

  virtual void read(cv::Mat &image, 
                    const Rect<int> &rect = Rect<int>(), 
                    const Size<int> size = Size<int>(), 
                    Affine<PointI> *trf = nullptr) = 0;

  /*!
   * \brief Lee el fragmento de imagen correspondiente a una ventana
   * \param[out] image Imagen que se lee
   * \param[in] window Ventana de la imagen que se quiere cargar
   * \param[in] scaleX Escala horizontal que se aplica a la región leida. Por defecto 1
   * \param[in] scaleY Escala Vertical que se aplica a la región leida. Por defecto 1
   * \param[out] trf Transformación que hay que aplicar a la imagen devuelta
   */
  virtual void read(cv::Mat &image, 
                    const WindowI &window, 
                    double scaleX = 1.,
                    double scaleY = 1., 
                    Affine<PointI> *trf = nullptr) = 0;

#endif

  /*!
   * \brief Lee el fragmento de imagen correspondiente a una ventana
   * \param[out] image Imagen que se lee
   * \param[in] wRead Ventana de la imagen que se quiere cargar. Por defecto toda la ventana
   * \param[in] scale Escala entre la imagen real y la que se lee. Por defecto 1
   * \param[out] trf Transformación que hay que aplicar a la imagen devuelta
   */
  //virtual void read(Image &image, 
  //                  const WindowI &wLoad = WindowI(), 
  //                  double scale = 1., 
  //                  Affine<PointI> *trf = nullptr) = 0;

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

protected:
  
  void windowRead(const WindowI &wLoad, 
                  WindowI *wRead, 
                  PointI *offset) const;

protected:

  std::string mFileName;
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



#endif // TL_IMAGE_READER_H
