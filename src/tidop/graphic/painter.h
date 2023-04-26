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

#ifndef TL_GRAPHIC_PAINTER_H
#define TL_GRAPHIC_PAINTER_H

#include "tidop/config.h"

#ifdef TL_HAVE_OPENCV
#include "opencv2/core/core.hpp"
#endif // TL_HAVE_OPENCV

#include "tidop/core/defs.h"
#include "tidop/geometry/entities/point.h"
#include "tidop/graphic/entities/point.h"
#include "tidop/graphic/entities/polygon.h"
#include "tidop/graphic/entities/linestring.h"

namespace tl
{

class Transform;


/*! \addtogroup GraphicEntities
 *  \{
 */

namespace graph
{

class Canvas;
class GraphicStyle;

/// TODO: Painter virtual
/// setStyle y push_style /pop_style (patron status)


class TL_EXPORT Painter
  : public GraphicStyle
{

public:

  /*!
   * \brief Constructora Painter
   */
  Painter();

  /*!
   * \brief Constructor que recibe el canvas como parámetro
   * \param[in] canvas Canvas
   */
  Painter(Canvas *canvas);

  Painter(const Painter &painter) = delete;
  Painter(Painter &&painter) = delete;
  Painter &operator = (const Painter &painter) = delete;
  Painter &operator = (Painter &&painter) = delete;

  /*!
   * \brief Destructora
   */
  ~Painter();

  void begin(Canvas *canvas){ unusedParameter(canvas); }
  void end(){ }

  /*!
   * \brief Dibuja un punto en el canvas
   * \param[in] point Punto
   */
  void drawPoint(const GPoint &point);
  void drawPoint(const Point<double> &point);

  /*!
   * \brief Dibuja una polilinea
   * \param[in] lineString Polilinea
   */
  void drawLineString(const GLineString &lineString);
  void drawLineString(const LineStringD &lineString);

  /*!
   * \brief Dibuja un poligono
   * \param[in] polygon Poligono
   */
  void drawPolygon(const GPolygon &polygon);
  void drawPolygon(const PolygonD &polygon);

  /*!
   * \brief Dibuja un multipunto
   * \param[in] multipoint Multipunto
   */
  void drawMultiPoint(const GMultiPoint &multipoint);

  /*!
   * \brief Dibuja una multipolilinea
   * \param[in] multiLineString multipolilinea
   */
  void drawMultiLineString(const GMultiLineString &multiLineString);

  /*!
   * \brief Dibuja un multipoligono
   * \param[in] multiPolygon Multipoligono
   */
  void drawMultiPolygon(const GMultiPolygon &multiPolygon);

#ifdef TL_HAVE_OPENCV
  void drawPicture(const cv::Mat &bmp);
#endif // TL_HAVE_OPENCV

  void drawText(const Point<double> &point, const std::string &text);

  /*!
   * \brief Establece el canvas
   * \param[in] canvas Canvas
   */
  void setCanvas(Canvas *canvas);

  ///*!
  // * \brief Establece el estilo de pluma
  // * \param[in] pen Estilo pluma
  // */
  //void setPen(const std::shared_ptr<Pen> &pen);

  ///*!
  // * \brief Establece el estilo de pincel
  // * \param[in] brush Estilo pincel
  // */
  //void setBrush(const std::shared_ptr<Brush> &brush);

  ///*!
  // * \brief Establece el estilo de simbolo
  // * \param[in] symbol Estilo simbolo
  // */
  //void setSymbol(const std::shared_ptr<Symbol> &symbol);

  ///*!
  // * \brief Establece el estilo de etiqueta
  // * \param[in] label Estilo de etiqueta
  // */
  //void setLabel(const std::shared_ptr<Label> &label);

  /*!
   * \brief Establece la transformación que se aplica para dibujar el en canvas
   * \param[in] trf Transformación
   */
  void setTransform(Transform *trf);

  //void drawImage(const RasterGraphics &image, const geometry::WindowI &w);
  //void drawImage(const RasterGraphics &image, Helmert2D<geometry::Point<int>> *trf);

protected:

  Transform *mTrf;
  Canvas *mCanvas;

};


} // Fin namespace graph

/*! \} */ // Fin GraphicEntities

} // Fin namespace TL

#endif // TL_GRAPHIC_PAINTER_H
