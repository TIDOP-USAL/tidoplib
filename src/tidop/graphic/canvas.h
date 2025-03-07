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
#include "tidop/graphic/entities/linestring.h"
#include "tidop/graphic/entities/polygon.h"



namespace tl
{


/*! \addtogroup Graphic
 *  \{
 */


class Painter;

/*!
 * \brief Canvas class
 */
class TL_EXPORT Canvas
{

public:

    Canvas();
    virtual ~Canvas() = default;

    /*!
     * \brief Canvas width
     * \return Width in pixels
     */
    virtual auto width() const -> int = 0;

    /*!
     * \brief Sets the width of the canvas
     * \param[in] width Canvas width
     */
    virtual void setWidth(int width) = 0;

    /*!
     * \brief Canvas height
     * \return Height of the canvas in pixels
     */
    virtual auto height() const -> int = 0;

    /*!
     * \brief Set the height of the canvas
     * \param[in] height Canvas height
     */
    virtual void setHeight(int height) = 0;

    /*!
     * \brief Background color of the canvas
     * \return Background color
     */
    virtual auto backgroundColor() const -> Color = 0;

    /*!
     * \brief Sets the background color of the canvas
     * \param[in] color Background color
     * \see Color
     */
    virtual void setBackgroundColor(const Color &color) = 0;

    /*!
     * \brief  Canvas Size
     */
    virtual auto size() const -> Size<int> = 0;

    /*!
     * \brief Set the canvas size
     * \param[in] size Canvas Size
     */
    virtual void setSize(const Size<int> &size) = 0;

    /*!
     * \brief Set the canvas size
     * \param[in] width Canvas width
     * \param[in] height Canvas height
     */
    virtual void setSize(int width, int height) = 0;

protected:

    /*!
     * \brief Draw a point on the canvas
     * \param[in] point Point
     * \param[in] style Style
     */
    virtual void drawPoint(const Point<double> &point, const GraphicStyle &style) = 0;

    /*!
     * \brief Draw a polyline on the canvas
     * \param[in] lineString Polyline
     * \param[in] style Style
     */
    virtual void drawLineString(const LineStringD &lineString, const GraphicStyle &style) = 0;

    /*!
     * \brief Draw a polygon on the canvas
     * \param[in] polygon Polygon
     * \param[in] style Style
     */
    virtual void drawPolygon(const PolygonD &polygon, const GraphicStyle &style) = 0;

    /*!
     * \brief Draw a text on the canvas
     * \param[in] point Insertion point
     * \param[in] text Text
     * \param[in] style Style
     */
    virtual void drawText(const Point<double> &point, const std::string &text, const GraphicStyle &style) = 0;

    //void setPainter(Painter *painter);

private:

    friend class Painter;
    //Painter *mPainter;
};



#ifdef TL_HAVE_OPENCV

class TL_EXPORT CanvasCV
    : public Canvas
{

public:

    CanvasCV();
    CanvasCV(const CanvasCV &canvas);
    ~CanvasCV() override;

    auto width() const -> int override;
    void setWidth(int width) override;
    auto height() const -> int override;
    void setHeight(int height) override;
    auto backgroundColor() const -> Color override;
    void setBackgroundColor(const Color &color) override;    
    auto size() const->Size<int> override;
    void setSize(const Size<int> &size) override;
    void setSize(int width, int height) override;

    auto bmp() -> cv::Mat;
    void setPicture(const cv::Mat &bmp);

    auto operator =(const CanvasCV &canvas) -> CanvasCV&;

protected:

    void drawPoint(const Point<double> &point, const GraphicStyle &style) override;
    void drawLineString(const LineStringD &lineString, const GraphicStyle &style) override;
    void drawPolygon(const PolygonD &polygon, const GraphicStyle &style) override;
    void drawText(const Point<double> &point, const std::string &text, const GraphicStyle &style) override;

private:

    void update();

    static auto colorToCvScalar(const Color &color) -> cv::Scalar;

private:

    Size<int> mSize;
    Color mBgColor;
    cv::Mat mCanvas;

};






inline auto CanvasCV::width() const -> int
{
    return mSize.width;
}

inline void CanvasCV::setWidth(int width)
{
    mSize.width = width;
    update();
}

inline auto CanvasCV::height() const -> int
{
    return mSize.height;
}

inline void CanvasCV::setHeight(int height)
{
    mSize.height = height;
    update();
}

inline auto CanvasCV::backgroundColor() const -> Color
{
    return mBgColor;
}

inline void CanvasCV::setBackgroundColor(const Color &color)
{
    mBgColor = color;
    update();
}

inline auto CanvasCV::size() const -> Size<int>
{
    return mSize;
}

inline void CanvasCV::setSize(const Size<int> &size)
{
    mSize = size;
    update();
}

inline void CanvasCV::setSize(int width, int height)
{
    mSize.width = width;
    mSize.height = height;
    update();
}

inline auto CanvasCV::bmp() -> cv::Mat
{
    return mCanvas;
}

inline auto CanvasCV::colorToCvScalar(const Color &color) -> cv::Scalar
{
    return {
        static_cast<double>(color.blue()),
        static_cast<double>(color.green()),
        static_cast<double>(color.red())
    };
}

#endif // TL_HAVE_OPENCV


/*! \} */

} // Fin namespace tl
