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

#include "tidop/core/base/Size.h"
#include "tidop/graphic/Color.h"
#include "tidop/graphic/Styles.h"
//#include "tidop/graphic/entities/linestring.h"
//#include "tidop/graphic/entities/polygon.h"
#include "tidop/geometry/primitives/MultiPoint.h"
#include "tidop/geometry/primitives/MultiLineString.h"
#include "tidop/geometry/primitives/MultiPolygon.h"


namespace tl
{

class Painter;

/*! \addtogroup Graphic
 *  \{
 */

/*!
 * \class Canvas
 * \brief Abstract base class for 2D graphical drawing surfaces.
 *
 * The `Canvas` class provides an abstract interface for rendering 2D geometric primitives
 * (such as points, lines, polygons, and text) onto a drawable surface. It is intended to be
 * subclassed by concrete implementations (e.g., image renderers, PDF backends, OpenGL contexts).
 * 
 * The `Canvas` itself does not perform drawing directly. Instead, it defines a set of protected
 * drawing methods that must be implemented by subclasses. These methods are invoked exclusively
 * through the `Painter` class, which acts as a high-level drawing interface and is declared
 * as a friend of `Canvas`.
 *
 * ### Key Responsibilities:
 * - Defines canvas dimensions (width, height, size).
 * - Manages background color settings.
 * - Declares protected virtual methods to draw geometric primitives with styling.
 * - Enables double-dispatch through `Painter` for abstraction and separation of concerns.
 *
 * ### Usage Example
 * \code{.cpp}
 * class ImageCanvas : public Canvas {
 * protected:
 *     void drawPoint(const Point2d& point, const GraphicStyle& style) override { ... }
 *     void drawLineString(const LineStringD& lineString, const GraphicStyle& style) override { ... }
 *     // implement other drawing methods...
 * };
 *
 * ImageCanvas canvas;
 * canvas.setSize(800, 600);
 * canvas.setBackgroundColor(Color::white);
 *
 * Painter painter(&canvas);
 * painter.draw(myGeometry, myStyle);  // Internally calls drawPoint/drawPolygon, etc.
 * \endcode
 *
 * \note Direct calls to drawing methods like `drawPoint` are not permitted; only `Painter` can invoke them.
 * \see Painter, GraphicStyle, Color, Size
 */
class TL_EXPORT Canvas
{

public:

    Canvas() = default;
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
    virtual void draw(const Point2d &point, const GraphicStyle &style) = 0;
    
    /*!
     * \brief Draw a polyline on the canvas
     * \param[in] lineString Polyline
     * \param[in] style Style
     */
    virtual void draw(const LineString<Point2d> &lineString, const GraphicStyle &style) = 0;

    /*!
     * \brief Draw a polygon on the canvas
     * \param[in] polygon Polygon
     * \param[in] style Style
     */
    virtual void draw(const Polygon<Point2d> &polygon, const GraphicStyle &style) = 0;

    /*!
     * \brief Draws a set of points on the canvas
     * \param[in] multiPoint Collection of points
     * \param[in] style Style to apply to each point
     */
    virtual void draw(const MultiPoint<Point2d> &multiPoint, const GraphicStyle &style) = 0;

    /*!
     * \brief Draws a set of polylines on the canvas
     * \param[in] multiLineString Collection of polylines
     * \param[in] style Style to apply to each polyline
     */
    virtual void draw(const MultiLineString<Point2d> &multiLineString, const GraphicStyle &style) = 0;

    /*!
     * \brief Draws a set of polygons on the canvas
     * \param[in] multiPolygon Collection of polygons
     * \param[in] style Style to apply to each polygon
     */
    virtual void draw(const MultiPolygon<Point2d> &multiPolygon, const GraphicStyle &style) = 0;

    /*!
     * \brief Draw a text on the canvas
     * \param[in] point Insertion point
     * \param[in] text Text
     * \param[in] style Style
     */
    virtual void drawText(const Point2d &point, const std::string &text, const GraphicStyle &style) = 0;

private:

    friend class Painter;
};



#ifdef TL_HAVE_OPENCV

/*!
 * \class CanvasCV
 * \brief OpenCV-based implementation of the Canvas interface.
 *
 * The `CanvasCV` class is a concrete subclass of `Canvas` that provides 2D drawing
 * capabilities using the OpenCV library. It allows rendering of points, lines,
 * polygons, and text onto an OpenCV `cv::Mat` image.
 *
 * This class is especially useful for generating visual output in image formats
 * such as PNG, JPEG, etc., or for in-memory visualization using OpenCV's GUI tools.
 *
 * Drawing is performed through the `Painter` interface, which invokes the protected
 * `drawXXX` methods (e.g., `drawPoint`, `drawPolygon`, etc.) of this class.
 *
 * ### Key Features:
 * - Manages a resizable OpenCV image buffer (`cv::Mat`) as the drawing surface.
 * - Implements all geometric drawing operations using OpenCV primitives.
 * - Handles background color configuration and canvas size updates.
 * - Provides access to the internal image buffer for saving, displaying, or post-processing.
 *
 * ### Usage Example
 * \code{.cpp}
 * auto vector_reader = VectorReaderFactory::create(vector_path);
 * vector_reader->open();
 * if (vector_reader->isOpen()) {
 *     CanvasCV canvas;
 *     canvas.setSize(640, 480);
 *     canvas.setBackgroundColor(Color(Color::Name::white);
 *
 *     Painter painter(&canvas);
 *             
 *     Window<Point2d> window;
 *     for (int i = 0; i < vector_reader->layersCount(); i++) {
 *         auto layer = vector_reader->read(i);
 *         window = joinWindow(window, layer->window());
 *     }
 *
 *     double scale_x = canvas.width() / static_cast<double>(window.width());
 *     double scale_y = canvas.height() / static_cast<double>(window.height());
 *     double scale = std::min(scale_x, scale_y);
 *
 *     Affine<double, 2> affine(scale, -scale, -window.min.x * scale, window.max.y * scale, 0.);
 *     painter.setTransform(affine);
 *     
 *     for (int i = 0; i < vector_reader->layersCount(); i++) {
 *         auto layer = vector_reader->read(i);
 *         layer->draw(painter);
 *     }
 *     cv::imwrite("output.png", canvas.bmp());
 * }
 * \endcode
 *
 * \see Canvas, Painter
 */
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

    /*!
     * \brief Returns a reference to the internal OpenCV image buffer.
     *
     * This buffer stores all drawn content and can be used for saving to disk,
     * displaying via OpenCV GUI functions, or further post-processing.
     *
     * \return A reference to the internal cv::Mat image.
     */
    auto image() -> cv::Mat &;

    /*!
     * \brief Returns a const reference to the internal OpenCV image buffer.
     *
     * Allows read-only access to the canvas content.
     *
     * \return A const reference to the internal cv::Mat image.
     */
    auto image() const -> const cv::Mat &;

    //void setPicture(const cv::Mat &bmp);

    auto operator =(const CanvasCV &canvas) -> CanvasCV&;

#ifdef TL_WARNING_DEPRECATED_METHOD 
    TL_DEPRECATED("image()", "4.0")
    auto bmp() -> cv::Mat;
#endif

protected:

    void draw(const Point2d &point, const GraphicStyle &style) override;
    void draw(const LineString<Point2d> &lineString, const GraphicStyle &style) override;
    void draw(const Polygon<Point2d> &polygon, const GraphicStyle &style) override;
    void draw(const MultiPoint<Point2d> &multiPoint, const GraphicStyle &style) override;
    void draw(const MultiLineString<Point2d> &multiLineString, const GraphicStyle &style) override;
    void draw(const MultiPolygon<Point2d> &multiPolygon, const GraphicStyle &style) override;
    void drawText(const Point2d &point, const std::string &text, const GraphicStyle &style) override;

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

#ifdef TL_WARNING_DEPRECATED_METHOD 
inline auto CanvasCV::bmp() -> cv::Mat
{
    return mCanvas;
}
#endif // TL_WARNING_DEPRECATED_METHOD

inline auto CanvasCV::image() -> cv::Mat&
{
    return mCanvas;
}

inline auto CanvasCV::image() const -> const cv::Mat &
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

} // End namespace tl
