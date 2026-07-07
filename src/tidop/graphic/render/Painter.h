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

/*! \file Painter.h
 * \brief High-level drawing interface for 2D geometric primitives.
 *
 * This file defines the `Painter` class, which provides a unified interface
 * for rendering geometric primitives onto a `Canvas`. The `Painter` applies
 * an affine transformation to all geometries before drawing, and manages
 * default visual styles. It is the primary entry point for rendering graphic
 * entities and raw geometries.
 *
 * \ingroup Graphics
 * \see tl::Canvas, tl::GraphicStyle, tl::Affine
 */

#pragma once

#include "tidop/config.h"

#include <vector>

#ifdef TL_HAVE_OPENCV
#include "opencv2/core/core.hpp"
#endif // TL_HAVE_OPENCV

#include "tidop/core/base/Defs.h"
#include "tidop/graphic/entities/GPoint.h"
#include "tidop/graphic/entities/GPolygon.h"
#include "tidop/graphic/entities/GLineString.h"
#include "tidop/graphic/entities/GMultiPoint.h"
#include "tidop/graphic/entities/GMultiLineString.h"
#include "tidop/graphic/entities/GMultiPolygon.h"
#include "tidop/geometry/transform/Affine.h"

namespace tl
{

class Canvas;
class GraphicStyle;

/*! \addtogroup Graphic
 *  \{
 */


/*!
 * \class Painter
 * \brief High-level drawing interface for 2D geometries.
 *
 * The `Painter` class is the main entry point for rendering geometric primitives.
 * It holds a reference to a `Canvas` and applies an affine transformation to all
 * geometries before passing them to the canvas's low-level drawing methods.
 * It also manages a default visual style and resolves style overrides from
 * individual `GraphicEntity` objects.
 *
 * ### Example
 * \code
 * CanvasCV canvas(Size<int>(800, 600));
 * Painter painter(canvas);
 *
 * // Set a default style
 * painter.setDefaultStyle(GraphicStyle().setPen(Pen(Color::Blue, 2.0)));
 *
 * // Draw a point with custom style
 * GPoint point(10, 20);
 * point.setSymbol(Symbol(Symbol::Type::Circle, 10.0));
 * painter.drawPoint(point);
 *
 * // Draw a raw polygon
 * Polygon<Point2d> poly = ...;
 * painter.drawPolygon(poly);
 * \endcode
 *
 * \see Canvas, GraphicStyle, Affine
 */
class TL_EXPORT Painter
{

protected:

    Affine<double, 2> mTransform;
    Canvas &mCanvas;
    GraphicStyle mDefaultStyle;

public:

    /*!
     * \brief Constructs a painter for the given canvas.
     * \param[in] canvas Reference to the canvas where drawing operations are performed.
     */
    Painter(Canvas &canvas);

    ~Painter();

    TL_DISABLE_COPY_AND_MOVE(Painter)

        /*!
         * \brief Draws a point graphic entity.
         * \param[in] point The point graphic entity.
         */
        void drawPoint(const GPoint &point);

    /*!
     * \brief Draws a point from raw coordinates.
     * \param[in] point 2D point.
     */
    void drawPoint(const Point2d &point) const;

    /*!
     * \brief Draws a polyline graphic entity.
     * \param[in] lineString The polyline graphic entity.
     */
    void drawLineString(const GLineString &lineString) const;

    /*!
     * \brief Draws a polyline from raw points.
     * \param[in] lineString The polyline.
     */
    void drawLineString(const LineString<Point2d> &lineString) const;

    /*!
     * \brief Draws a polygon graphic entity.
     * \param[in] polygon The polygon graphic entity.
     */
    void drawPolygon(const GPolygon &polygon) const;

    /*!
     * \brief Draws a polygon from raw points.
     * \param[in] polygon The polygon.
     */
    void drawPolygon(const Polygon<Point2d> &polygon) const;

    /*!
     * \brief Draws a multi-point graphic entity.
     * \param[in] multipoint The multi-point graphic entity.
     */
    void drawMultiPoint(const GMultiPoint &multipoint) const;

    /*!
     * \brief Draws a multi-point from raw points.
     * \param[in] multipoint The multi-point.
     */
    void drawMultiPoint(const MultiPoint<Point2d> &multipoint) const;

    /*!
     * \brief Draws a multi-line string graphic entity.
     * \param[in] multiLineString The multi-line string graphic entity.
     */
    void drawMultiLineString(const GMultiLineString &multiLineString) const;

    /*!
     * \brief Draws a multi-line string from raw geometry.
     * \param[in] multiLineString The multi-line string.
     */
    void drawMultiLineString(const MultiLineString<Point2d> &multiLineString) const;

    /*!
     * \brief Draws a multi-polygon graphic entity.
     * \param[in] multiPolygon The multi-polygon graphic entity.
     */
    void drawMultiPolygon(const GMultiPolygon &multiPolygon) const;

    /*!
     * \brief Draws a multi-polygon from raw geometry.
     * \param[in] multiPolygon The multi-polygon.
     */
    void drawMultiPolygon(const MultiPolygon<Point2d> &multiPolygon) const;

#ifdef TL_HAVE_OPENCV
    /*!
     * \brief Draws a raster image onto the canvas.
     * \param[in] bmp OpenCV image to draw.
     */
    void drawPicture(const cv::Mat &bmp) const;
#endif // TL_HAVE_OPENCV

    /*!
     * \brief Draws text at a specified position.
     * \param[in] point Anchor point for the text.
     * \param[in] text  Text string to render.
     * \note Currently not implemented.
     */
    void drawText(const Point2d &point, std::string_view text) const;

    /*!
     * \brief Sets the affine transformation applied to all drawing operations.
     * \param[in] affine 2D affine transformation.
     */
    void setTransform(Affine<double, 2> affine);

    //void drawImage(const RasterGraphics &image, const geometry::WindowI &w);
    //void drawImage(const RasterGraphics &image, Helmert2D<geometry::Point<int>> *trf);

private:

    [[nodiscard]]
    auto resolveStyle(const GraphicStyle &entityStyle) const -> GraphicStyle;
    void drawImpl(const Point2d &geometry, const GraphicStyle &style) const;
    void drawImpl(const LineString<Point2d> &geometry, const GraphicStyle &style) const;
    void drawImpl(const Polygon<Point2d> &geometry, const GraphicStyle &style) const;
    void drawImpl(const MultiPoint<Point2d> &geometry, const GraphicStyle &style) const;
    void drawImpl(const MultiLineString<Point2d> &geometry, const GraphicStyle &style) const;
    void drawImpl(const MultiPolygon<Point2d> &geometry, const GraphicStyle &style) const;

};


/*! \} */ // Fin GraphicEntities

} // Fin namespace tl
