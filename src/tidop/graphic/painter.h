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

#pragma once

#include "tidop/config.h"

#ifdef TL_HAVE_OPENCV
#include "opencv2/core/core.hpp"
#endif // TL_HAVE_OPENCV

#include "tidop/core/defs.h"
#include "tidop/graphic/entities/point.h"
#include "tidop/graphic/entities/polygon.h"
#include "tidop/graphic/entities/linestring.h"
#include "tidop/math/geometry/affine.h"

namespace tl
{

namespace geom
{
class Transform;
}

/*! \addtogroup GraphicEntities
 *  \{
 */

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

    ~Painter();

    void begin(Canvas *canvas) { unusedParameter(canvas); }
    void end() {}

    /*!
     * \brief Dibuja un punto en el canvas
     * \param[in] point Punto
     */
    void drawPoint(const GPoint &point);
    void drawPoint(const Point<double> &point) const;

    /*!
     * \brief Dibuja una polilinea
     * \param[in] lineString Polilinea
     */
    void drawLineString(const GLineString &lineString) const;
    void drawLineString(const LineStringD &lineString) const;

    /*!
     * \brief Dibuja un poligono
     * \param[in] polygon Poligono
     */
    void drawPolygon(const GPolygon &polygon) const;
    void drawPolygon(const PolygonD &polygon) const;

    /*!
     * \brief Dibuja un multipunto
     * \param[in] multipoint Multipunto
     */
    void drawMultiPoint(const GMultiPoint &multipoint) const;

    /*!
     * \brief Dibuja una multipolilinea
     * \param[in] multiLineString multipolilinea
     */
    void drawMultiLineString(const GMultiLineString &multiLineString) const;

    /*!
     * \brief Dibuja un multipoligono
     * \param[in] multiPolygon Multipoligono
     */
    void drawMultiPolygon(const GMultiPolygon &multiPolygon) const;

#ifdef TL_HAVE_OPENCV
    void drawPicture(const cv::Mat &bmp) const;
#endif // TL_HAVE_OPENCV

    void drawText(const Point<double> &point, const std::string &text) const;

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
    void setTransform(const Affine<double, 2> &affine);

    //void drawImage(const RasterGraphics &image, const geometry::WindowI &w);
    //void drawImage(const RasterGraphics &image, Helmert2D<geometry::Point<int>> *trf);

protected:

    Affine<double, 2> mTransform;
    Canvas *mCanvas;

};


/*! \} */ // Fin GraphicEntities

} // Fin namespace tl
