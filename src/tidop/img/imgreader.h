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
 * License along with TidopLib. If not, see <http://www.gnu.org/licenses>.*
 *                                                                        *
 * @license LGPL-3.0 <https://www.gnu.org/licenses/lgpl-3.0.html>         *
 *                                                                        *
 **************************************************************************/

#ifndef TL_IMAGE_READER_H
#define TL_IMAGE_READER_H

#include "config_tl.h"


#ifdef TL_HAVE_OPENCV


#include <string>
#include <memory>

#include <opencv2/core/core.hpp>

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

class ImageMetadata;

/*!
 * \brief Clase para la lectura de diferentes formatos de imagen
 */
class TL_EXPORT ImageReader
{

public:

  ImageReader(tl::Path file);
  virtual ~ImageReader() = default;

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
   * \brief Lee un fragmento de imagen correspondiente a una región
   * \param[in] rect Región de la imagen que se carga. Por defecto toda la imagen
   * \param[in] size Tamaño de la imagen de salida. Por defecto el tamaño de la región de lectura
   * \param[out] trf Transformación que hay que aplicar a la imagen devuelta
   * \return imagen
   */
  virtual cv::Mat read(const Rect<int> &rect = Rect<int>(), 
                       const Size<int> &size = Size<int>(), 
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
   * \brief Sistema de referencia en formato WKT
   */
  virtual std::string crsWkt() const = 0;

//#ifdef TL_HAVE_GEOSPATIAL
//  /*!
//   * \brief Sistema de referencia
//   */
//  virtual geospatial::Crs crs() const = 0;
//#endif

  /*!
   * \brief Ventana envolvente de la imagen en coordenadas terreno
   */
  virtual WindowD window() const = 0;

  tl::Path file() const;

  virtual double noDataValue(bool *exist = nullptr) const = 0;

//#ifdef TL_HAVE_GRAPHIC
//  virtual graph::Color noDataValue() const = 0;
//#endif

protected:
  
  void windowRead(const WindowI &wLoad,
                  WindowI *wRead, 
                  PointI *offset) const;

private:

  Path mFile;

};


/*!
 * \brief Factoria de clases para la lectura de formatos de imagen
 */
class TL_EXPORT ImageReaderFactory
{

private:

  ImageReaderFactory() = default;

public:

  static std::unique_ptr<ImageReader> createReader(const Path &file);
};

} // End namespace tl

#endif // TL_HAVE_OPENCV

#endif // TL_IMAGE_READER_H
