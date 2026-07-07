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

/*! \file Canvas.h
 * \brief Abstract base class for 2D drawing surfaces and OpenCV implementation.
 *
 * This file defines the `Canvas` abstract class, which provides an interface
 * for rendering 2D geometric primitives onto a drawable surface. It also
 * provides a concrete implementation using OpenCV (`CanvasCV`) when the
 * `TL_HAVE_OPENCV` macro is defined.
 *
 * \ingroup Graphics
 * \see tl::Painter, tl::GraphicStyle, tl::Color, tl::Size
 */

#pragma once

#include "tidop/config.h"

#ifdef TL_HAVE_OPENCV
#include "opencv2/core/core.hpp"
#endif // TL_HAVE_OPENCV

#include "tidop/core/base/Size.h"
#include "tidop/core/base/macros/NonCopyable.h"
#include "tidop/graphic/Color.h"
#include "tidop/graphic/Styles.h"
#include "tidop/geometry/primitives/Point.h"
#include "tidop/geometry/primitives/LineString.h"
#include "tidop/geometry/primitives/Polygon.h"


namespace tl
{

class Painter;

/*! \addtogroup Graphic
 *  \{
 */

/*!
 * \class Canvas
 * \brief Abstract base class for 2D drawing surfaces.
 *
 * The `Canvas` class provides an abstract interface for rendering 2D geometric
 * primitives (points, lines, polygons, and text) onto a drawable surface. It is
 * intended to be subclassed by concrete implementations (e.g., image renderers,
 * PDF backends, OpenGL contexts).
 *
 * The canvas manages background color and dimensions, and defines pure virtual
 * `draw()` methods that must be implemented by subclasses. These methods are
 * invoked by the `Painter` class, which provides a higher-level drawing API.
 *
 * ### Example
 * \code
 * class ImageCanvas : public Canvas {
 * public:
 *     void draw(const Point2d& point, const Symbol& symbol, const Pen& pen) override { ... }
 *     void draw(const LineString<Point2d>& line, const Pen& pen) override { ... }
 *     // ... other draw methods
 * };
 *
 * ImageCanvas canvas;
 * canvas.setSize(800, 600);
 * canvas.setBackgroundColor(Color::white);
 * \endcode
 *
 * \see Painter, GraphicStyle, Color, Size
 */
class TL_EXPORT Canvas
{

public:

    Canvas() = default;
    virtual ~Canvas() = default;

    TL_DISABLE_COPY_AND_MOVE(Canvas)

    /*!
     * \brief Returns the canvas width in pixels.
     * \return Width in pixels.
     */
    [[nodiscard]]
    virtual auto width() const -> int = 0;

    /*!
     * \brief Returns the canvas height in pixels.
     * \return Height in pixels.
     */
    [[nodiscard]]
    virtual auto height() const -> int = 0;

    /*!
     * \brief Returns the canvas size.
     * \return Size object containing width and height.
     */
    [[nodiscard]]
    virtual auto size() const noexcept -> Size<int> = 0;

    /*!
     * \brief Sets the canvas size.
     * \param[in] size New size.
     */
    virtual void setSize(Size<int> size) = 0;

    /*!
     * \brief Returns the current background color.
     * \return Background color.
     */
    [[nodiscard]] 
    virtual auto backgroundColor() const noexcept -> Color = 0;

    /*!
     * \brief Sets the background color.
     * \param[in] color New background color.
     */
    virtual void setBackgroundColor(Color color) = 0;

    /*!
     * \brief Clears the canvas with the background color.
     */
    virtual void clear() = 0;

    /*!
     * \brief Draws a point on the canvas.
     * \param[in] point  The point to draw.
     * \param[in] symbol Symbol style (marker shape and size).
     * \param[in] pen    Pen style for the symbol outline.
     */
    virtual void draw(const Point2d &point, const Symbol &symbol, const Pen &pen) = 0;

    /*!
     * \brief Draws a polyline on the canvas.
     * \param[in] line The polyline to draw.
     * \param[in] pen  Pen style for the line.
     */
    virtual void draw(const LineString<Point2d> &line, const Pen &pen) = 0;

    /*!
     * \brief Draws a polygon on the canvas.
     * \param[in] polygon The polygon to draw.
     * \param[in] pen     Pen style for the outline.
     * \param[in] brush   Brush style for the fill.
     */
    virtual void draw(const Polygon<Point2d> &polygon, const Pen &pen, const Brush &brush) = 0;

    /*!
     * \brief Draws text on the canvas.
     * \param[in] point Anchor point for the text.
     * \param[in] text  Text string to draw.
     * \param[in] label Label style (font, size, color, alignment).
     */
    virtual void drawText(const Point2d &point, std::string_view text, const Label &label) = 0;

};



#ifdef TL_HAVE_OPENCV

/*!
 * \class CanvasCV
 * \brief OpenCV-based implementation of the `Canvas` interface.
 *
 * This class provides a concrete implementation of the `Canvas` abstract class
 * using the OpenCV library. It renders primitives onto an OpenCV `cv::Mat` image,
 * which can be saved to disk, displayed, or further processed.
 *
 * ### Example
 * \code
 * CanvasCV canvas(Size<int>(800, 600));
 * canvas.setBackgroundColor(Color::white);
 *
 * Painter painter(&canvas);
 * painter.draw(point, style);
 * painter.draw(polygon, style);
 *
 * cv::imwrite("output.png", canvas.image());
 * \endcode
 *
 * \see Canvas, Painter
 */
class TL_EXPORT CanvasCV
    : public Canvas
{

public:

    /*!
     * \brief Constructs an OpenCV canvas with the given size.
     * \param[in] size Initial canvas size.
     */
    CanvasCV(Size<int> size);
    ~CanvasCV() override = default;

    /*!
     * \brief Returns the canvas width in pixels.
     * \return Width in pixels.
     */
    [[nodiscard]]
    auto width() const noexcept -> int override;

    /*!
     * \brief Returns the canvas height in pixels.
     * \return Height in pixels.
     */
    [[nodiscard]]
    auto height() const noexcept -> int override;

    /*!
     * \brief Returns the canvas size.
     * \return Size object containing width and height.
     */
    [[nodiscard]]
    auto size() const noexcept -> Size<int> override;

    /*!
     * \brief Returns the current background color.
     * \return Background color.
     */
    [[nodiscard]] 
    auto backgroundColor() const noexcept -> Color override;

    /*!
     * \brief Sets the canvas size (reallocates the image buffer).
     * \param[in] size New size.
     */
    void setSize(Size<int> size) override;

    /*!
     * \brief Sets the background color.
     * \param[in] color New background color.
     */
    void setBackgroundColor(Color color) override;    

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


#ifdef TL_WARNING_DEPRECATED_METHOD 
    TL_DEPRECATED("image()", "4.0")
    auto bmp() -> cv::Mat;
#endif

    /*!
     * \brief Clears the canvas with the background color.
     */
    void clear() override;

    /*!
     * \brief Draws a point on the canvas.
     * \param[in] point  The point to draw.
     * \param[in] symbol Symbol style (marker shape and size).
     * \param[in] pen    Pen style for the symbol outline.
     */
    void draw(const Point2d &point, const Symbol &symbol, const Pen &pen) override;

    /*!
     * \brief Draws a polyline on the canvas.
     * \param[in] line The polyline to draw.
     * \param[in] pen  Pen style for the line.
     */
    void draw(const LineString<Point2d> &line, const Pen &pen) override;

    /*!
     * \brief Draws a polygon on the canvas.
     * \param[in] polygon The polygon to draw.
     * \param[in] pen     Pen style for the outline.
     * \param[in] brush   Brush style for the fill.
     */
    void draw(const Polygon<Point2d> &polygon, const Pen &pen, const Brush &brush) override;

    /*!
     * \brief Draws text on the canvas.
     * \param[in] point Anchor point for the text.
     * \param[in] text  Text string to draw.
     * \param[in] label Label style (font, size, color, alignment).
     */
    void drawText(const Point2d &point, std::string_view text, const Label &label) override;

private:

    void update();

    static auto colorToCvScalar(const Color &color) -> cv::Scalar;

private:

    Size<int> mSize;
    Color mBgColor;
    cv::Mat mCanvas;
};



inline auto CanvasCV::width() const noexcept -> int
{
    return mSize.width();
}

inline auto CanvasCV::height() const noexcept -> int
{
    return mSize.height();
}

inline auto CanvasCV::size() const noexcept -> Size<int>
{
    return mSize;
}

inline auto CanvasCV::backgroundColor() const noexcept -> Color
{
    return mBgColor;
}

inline void CanvasCV::setBackgroundColor(Color color)
{
    mBgColor = std::move(color);
    update();
}

inline void CanvasCV::setSize(Size<int> size)
{
    mSize = std::move(size);
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

inline void CanvasCV::clear()
{
    mBgColor = Color(Color::Name::white);
}

#endif // TL_HAVE_OPENCV


/*! \} */

} // End namespace tl
