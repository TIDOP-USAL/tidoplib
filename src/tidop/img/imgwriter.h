/**************************************************************************
 *                                                                        *
 * Copyright (C) 2021 by Tidop Research Group                             *
 * Copyright (C) 2021 by Esteban Ruiz de O�a Crespo                       *
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

#ifndef TL_IMAGE_WRITER_H
#define TL_IMAGE_WRITER_H

#include "tidop/config.h"

#ifdef TL_HAVE_OPENCV

#include <string>
#include <memory>

#include "opencv2/core/core.hpp"

#include "tidop/core/defs.h"
#include "tidop/core/utils.h"
#include "tidop/core/path.h"
#include "tidop/geometry/entities/point.h"
#include "tidop/geometry/transform/affine.h"
#include "tidop/geometry/rect.h"
#include "tidop/img/img.h"
//#ifdef TL_HAVE_GEOSPATIAL 
//#include "tidop/geospatial/crs.h"
//#endif
//#ifdef TL_HAVE_GRAPHIC
//#include "tidop/graphic/color.h"
//#endif

namespace tl
{

class ImageOptions;
class ImageMetadata;

/*!
 * \brief Clase para la escritura de distintos formatos imagen
 */
class TL_EXPORT ImageWriter
{

public:

  ImageWriter(tl::Path file);
  virtual ~ImageWriter() = default;

  /*!
   * \brief Abre el fichero
   */
  virtual void open() = 0;

  /*!
   * \brief Comprueba si el fichero se ha cargado correctamente
   */
  virtual bool isOpen() const = 0;

  /*!
   * \brief Cierra el fichero
   */
  virtual void close() = 0;

  /*!
   * \brief Establece las opciones de creaci�n del formato imagen
   */
  virtual void setImageOptions(ImageOptions *imageOptions) = 0;

  /*!
   * \brief Establece los metadatos de la imagen
   */
  virtual void setImageMetadata(const std::shared_ptr<ImageMetadata> &imageMetadata) = 0;

  /*!
   * \brief Crea una imagen
   * \param[in] rows N�mero de filas de la imagen
   * \param[in] cols N�mero de columnas de la imagen
   * \param[in] bands N�mero de bandas de la imagen
   * \param[in] type Tipo de dato
   * \see DataType
   */
  virtual void create(int rows, 
                      int cols, 
                      int bands, 
                      DataType type) = 0;

  /*!
   * \brief Escribe en la imagen
   * \param[in] image Bloque de imagen que se escribe
   * \param[in] window Ventana del bloque de imagen que se escribe. Por defecto toda la imagen
   */
  virtual void write(const cv::Mat &image, 
                     const Rect<int> &rect = Rect<int>()) = 0;

  /*!
   * \brief Escribe en la imagen
   * \param[in] image Bloque de imagen que se escribe
   * \param[in] window Ventana del bloque de imagen que se escribe.
   */
  virtual void write(const cv::Mat &image, 
                     const WindowI &window) = 0;

  /*!
   * \brief Escribe en la imagen
   * \param[in] image Bloque de imagen que se escribe
   * \param[in] trf Transformaci�n entre el bloque y la imagen.
   */
  virtual void write(const cv::Mat &image, 
                     const Affine<PointI> &trf) = 0;

  /*!
   * \brief Escribe en la imagen
   * \param[in] image Bloque de imagen que se escribe
   * \param[in] w Ventana del bloque de imagen que se escribe
   */
  //virtual void write(const unsigned char *buff, 
  //                   const WindowI &w) = 0;

  /*!
   * \brief Escribe en la imagen
   * \param[in] image Bloque de imagen que se escribe
   * \param[in] trf Transformaci�n entre el bloque y la imagen. Si es nula no se aplica transformaci�n
   */
  //virtual void write(const unsigned char *buff, const Helmert2D<geometry::PointI> *trf = nullptr) = 0;

  /*!
   * \brief Devuelve el n�mero de filas de la imagen
   * \return N�mero de filas de la imagen
   */
  virtual int rows() const = 0;

  /*!
   * \brief Devuelve el n�mero de columnas de la imagen
   * \return N�mero de columnas de la imagen
   */
  virtual int cols() const = 0;

  /*!
   * \brief Devuelve el n�mero de canales o bandas de la imagen
   * \return N�mero de bandas de la imagen
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

  /*!
   * \brief Establece la georeferencia de la imagen
   * \param[in] georeference Georeferencia
   */
  virtual void setGeoreference(const Affine<PointD> &georeference) = 0;

  /*!
   * \brief Set the Coordinate Reference System
   * \param[in] crs Coordinate Reference System in WKT format
   */
  virtual void setCRS(const std::string &crs) = 0;

//#ifdef TL_HAVE_GEOSPATIAL
//  /*!
//   * \brief Set the Coordinate Reference System
//   * \param[in] crs geospatial::Crs object
//   */
//  virtual void setCRS(const geospatial::Crs &crs) = 0;
//#endif
  
  virtual void setNoDataValue(double nodata) = 0;

//#ifdef TL_HAVE_GRAPHIC
//  virtual void setNoDataValue(const graph::Color &nodata) = 0;
//#endif

protected:
  
  void windowWrite(const WindowI &window, 
                   WindowI *windowWrite, 
                   PointI *offset) const;
protected:

  Path mFile;
  Affine<PointD> mAffine;
//#ifdef TL_HAVE_GEOSPATIAL
//  geospatial::Crs mCRS;
//#endif
};


/*!
 * \brief Clase factor�a para la escritura de diferentes formatos de imagen
 */
class TL_EXPORT ImageWriterFactory
{
public:

private:

  ImageWriterFactory() {}

public:

  static std::unique_ptr<ImageWriter> create(const Path &fileName);
  TL_DEPRECATED("create", "2.1")
  static std::unique_ptr<ImageWriter> createWriter(const Path &fileName);
};


} // End namespace tl

#endif // TL_HAVE_OPENCV

#endif // TL_IMAGE_WRITER_H
