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

#include "tidop/graphic/color.h"
#include "tidop/graphic/styles.h"
#include "tidop/graphic/entities/point.h"
#include "tidop/graphic/entities/linestring.h"
#include "tidop/graphic/entities/polygon.h"



namespace tl
{


/*! \defgroup GraphicEntities Entidades gráficas
 *  Puntos, lineas, ...
 *  \{
 */


class Painter;

/*!
 * \brief Clase Canvas
 */
class TL_EXPORT Canvas
{

public:

    /*!
     * \brief Constructora canvas
     */
    Canvas();

    virtual ~Canvas() {}

    /*!
     * \brief Ancho del canvas
     * \return Ancho en pixeles
     */
    virtual int width() const = 0;

    /*!
     * \brief Alto del canvas
     * \return Alto del canvas en pixeles
     */
    virtual int height() const = 0;

    /*!
     * \brief Color de fondo del canvas
     * \return Color de fondo
     */
    virtual Color backgroundColor() const = 0;

    /*!
     * \brief Establece el ancho del canvas
     * \param width Ancho del canvas
     */
    virtual void setWidth(int width) = 0;

    /*!
     * \brief Establece el alto del canvas
     * \param height Alto
     */
    virtual void setHeight(int height) = 0;

    /*!
     * \brief Establece el tamaño del canvas
     * \param width Ancho
     * \param height Alto
     */
    virtual void setSize(int width, int height) = 0;

    virtual void setSize(const Size<int> &size) = 0;
    virtual Size<int> size() const = 0;

    /*!
     * \brief Establece el color de fondo del canvas
     * \param color Color
     * \see Color
     */
    virtual void setBackgroundColor(const Color &color) = 0;

protected:

    /*!
     * \brief Dibuja un punto en el canvas
     * \param point Punto
     */
     //virtual void drawPoint(const GPoint &point) = 0;
    virtual void drawPoint(const Point<double> &point, const GraphicStyle &style) = 0;

    /*!
     * \brief Dibuja una polilinea en el canvas
     * \param lineString Polilinea
     */
     //virtual void drawLineString(const GLineString &lineString) = 0;
    virtual void drawLineString(const LineStringD &lineString, const GraphicStyle &style) = 0;

    /*!
     * \brief Dibuja un poligono en el canvas
     * \param polygon Poligono
     */
     //virtual void drawPolygon(const GPolygon &polygon) = 0;
    virtual void drawPolygon(const PolygonD &polygon, const GraphicStyle &style) = 0;


    //virtual void drawText(const Point<double> &point, const std::string &text) = 0;
    virtual void drawText(const Point<double> &point, const std::string &text, const GraphicStyle &style) = 0;

    void setPainter(Painter *painter);

private:

    friend class Painter;
    Painter *mPainter;
};

#ifdef TL_HAVE_OPENCV

class TL_EXPORT CanvasCV
    : public Canvas
{

public:

    /*!
     * \brief CanvasCV
     */
    CanvasCV();

    /*!
     * \brief Constructor de copia
     * \param[in] canvas Objeto canvas que se copia
     */
    CanvasCV(const CanvasCV &canvas);
    ~CanvasCV() override;

    int width() const override;
    int height() const override;
    Color backgroundColor() const override;
    void setWidth(int width) override;
    void setHeight(int height) override;
    void setSize(int width, int height) override;
    void setSize(const Size<int> &size) override;
    Size<int> size() const override;
    void setBackgroundColor(const Color &color) override;

    cv::Mat bmp();

protected:

    //void drawPoint(const GPoint &point) override;
    void drawPoint(const Point<double> &point, const GraphicStyle &style) override;
    //void drawLineString(const GLineString &lineString) override;
    void drawLineString(const LineStringD &lineString, const GraphicStyle &style) override;
    //void drawPolygon(const GPolygon &polygon) override;
    void drawPolygon(const PolygonD &polygon, const GraphicStyle &style) override;
    //void drawText(const Point<double> &point, const std::string &text) override;
    void drawText(const Point<double> &point, const std::string &text, const GraphicStyle &style) override;

    void setPicture(const cv::Mat &bmp);



    /*!
     * \brief operador asignación
     * \param[in] canvas Objeto canvas que se copia por asignación
     * \return
     */
    CanvasCV &operator =(const CanvasCV &canvas);

private:

    void update();

    cv::Scalar colorToCvScalar(const Color &color);

private:

    Size<int> mSize;
    Color mBgColor;
    cv::Mat mCanvas;

};


#endif // TL_HAVE_OPENCV


/*! \} */ // Fin GraphicEntities

} // Fin namespace tl
