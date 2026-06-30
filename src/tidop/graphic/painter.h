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

#include <vector>

#ifdef TL_HAVE_OPENCV
#include "opencv2/core/core.hpp"
#endif // TL_HAVE_OPENCV

#include "tidop/core/base/Defs.h"
#include "tidop/graphic/entities/point.h"
#include "tidop/graphic/entities/polygon.h"
#include "tidop/graphic/entities/linestring.h"
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
 * \brief High-level interface for rendering vector geometries on a canvas.
 *
 * The `Painter` class provides a convenient API for drawing 2D geometries such as points,
 * polylines, polygons, and text onto a `Canvas` surface. It inherits from `GraphicStyle`,
 * so it can carry styling information (pen, brush, symbol, label) directly or through
 * an internal style stack.
 *
 * This class delegates the actual drawing to the underlying canvas implementation, applying
 * style and optional coordinate transformations.
 *
 * \see Canvas, GraphicStyle, Affine
 */
class TL_EXPORT Painter
  //: public GraphicStyle
{

public:

    /*!
     * \brief Default constructor.
     */
    Painter();

    /*!
     * \brief Constructs a painter for the given canvas.
     * \param[in] canvas Canvas to draw on.
     */
    Painter(Canvas *canvas);

    Painter(const Painter &painter) = delete;
    Painter(Painter &&painter) = delete;
    Painter &operator = (const Painter &painter) = delete;
    Painter &operator = (Painter &&painter) = delete;

    ~Painter();

    ///*!
    // * \brief Begins a new drawing session on the given canvas.
    // * \param[in] canvas Target canvas.
    // */
    //void begin(Canvas *canvas) { unusedParameter(canvas); }

    ///*!
    // * \brief Ends the current drawing session.
    // */
    //void end() {}

    /*!
     * \brief Draws a point.
     * \param[in] point Point geometry.
     */
    void drawPoint(const GPoint &point);

    /*!
     * \brief Draws a point using raw coordinates.
     * \param[in] point 2D point.
     */
    void drawPoint(const Point2d &point) const;

    /*!
     * \brief Draws a polyline geometry.
     * \param[in] lineString Polyline.
     */
    void drawLineString(const GLineString &lineString) const;

    /*!
     * \brief Draws a polyline from raw 2D points.
     * \param[in] lineString Polyline.
     */
    void drawLineString(const LineString<Point2d> &lineString) const;

    /*!
     * \brief Draws a polygon geometry.
     * \param[in] polygon Polygon.
     */
    void drawPolygon(const GPolygon &polygon) const;

    /*!
     * \brief Draws a polygon from raw 2D coordinates.
     * \param[in] polygon Polygon.
     */
    void drawPolygon(const Polygon<Point2d> &polygon) const;

    /*!
     * \brief Draws a multipoint geometry.
     * \param[in] multipoint Multipoint object.
     */
    void drawMultiPoint(const GMultiPoint &multipoint) const;
    void drawMultiPoint(const MultiPoint<Point2d> &multipoint) const;

    /*!
     * \brief Draws a multilinestring geometry.
     * \param[in] multiLineString Multi-line object.
     */
    void drawMultiLineString(const GMultiLineString &multiLineString) const;
    void drawMultiLineString(const MultiLineString<Point2d> &multiLineString) const;

    /*!
     * \brief Draws a multipolygon geometry.
     * \param[in] multiPolygon Multi-polygon object.
     */
    void drawMultiPolygon(const GMultiPolygon &multiPolygon) const;
    void drawMultiPolygon(const MultiPolygon<Point2d> &multiPolygon) const;

#ifdef TL_HAVE_OPENCV
    /*!
     * \brief Draws a raster image onto the canvas.
     * \param[in] bmp OpenCV image to draw.
     */
    void drawPicture(const cv::Mat &bmp) const;
#endif // TL_HAVE_OPENCV

    /*!
     * \brief Draws a text string at a specified position.
     * \param[in] point Insertion point.
     * \param[in] text Text to render.
     */
    void drawText(const Point2d &point, const std::string &text) const;

    /*!
     * \brief Sets the canvas used for drawing operations.
     *
     * This assigns the target canvas on which all subsequent drawing commands will be rendered.
     * The canvas must remain valid during the lifetime of the painter session.
     *
     * \param[in] canvas Pointer to the target canvas.
     */
    void setCanvas(Canvas *canvas);

    /*!
     * \brief Pushes a new style onto the style stack.
     * \param[in] style Graphic style to apply.
     */
    //void pushStyle(const GraphicStyle &style);

    /*!
     * \brief Pops the last style from the style stack.
     */
    //void popStyle();

    /*!
     * \brief Sets the affine transform applied to all drawing operations.
     * \param[in] affine Affine 2D transformation.
     */
    void setTransform(const Affine<double, 2> &affine);

    //void drawImage(const RasterGraphics &image, const geometry::WindowI &w);
    //void drawImage(const RasterGraphics &image, Helmert2D<geometry::Point<int>> *trf);

private:

   auto resolvedStyle(const GraphicStyle &entityStyle) const -> GraphicStyle;
   void drawImpl(const Point2d &geometry, const GraphicStyle &style) const;
   void drawImpl(const LineString<Point2d> &geometry, const GraphicStyle &style) const;
   void drawImpl(const Polygon<Point2d> &geometry, const GraphicStyle &style) const;
   void drawImpl(const MultiPoint<Point2d> &geometry, const GraphicStyle &style) const;
   void drawImpl(const MultiLineString<Point2d> &geometry, const GraphicStyle &style) const;
   void drawImpl(const MultiPolygon<Point2d> &geometry, const GraphicStyle &style) const;

protected:

    Affine<double, 2> mTransform;
    Canvas *mCanvas;
    GraphicStyle mDefaultStyle;

};


/*! \} */ // Fin GraphicEntities

} // Fin namespace tl
