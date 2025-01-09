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
#include "tidop/graphic/color.h"

#include <opencv2/imgproc.hpp>
#include <opencv2/core.hpp>

#include "tidop/core/base/defs.h"
#include "tidop/rastertools/process/imgprocess.h"
#include "tidop/core/base/size.h"

namespace tl
{


/*! \addtogroup ImgTransf
 *
 *  \{
 */

/*!
 * \brief Resizes an image.
 *
 * The `Resize` class adjusts the dimensions of an input image, either to a specified size
 * or by applying scaling factors. It supports multiple constructors to accommodate different use cases.
 */
class TL_EXPORT Resize
  : public ImageProcess
{

private:

    int mWidth;
    int mHeight;
    double mScaleX;
    double mScaleY;

public:

    /*!
     * \brief Constructor specifying output image size.
     * \param[in] size Destination image size.
     */
    explicit Resize(const Size<int> &size);

    /*!
     * \brief Constructor specifying width and height.
     * \param[in] width Destination image width.
     * \param[in] height Destination image height. If zero, the aspect ratio is preserved.
     */
    Resize(int width, int height = 0);

    /*!
     * \brief Constructor specifying scaling factors.
     * \param[in] scaleX Scale along the x-axis.
     * \param[in] scaleY Scale along the y-axis. If zero, it takes the same value as \c scaleX.
     */
    Resize(double scaleX, double scaleY = 0.);

    ~Resize() override = default;

    /*!
     * \brief Executes the resizing operation.
     * \param[in] matIn Input image.
     * \param[out] matOut Output image with resized dimensions.
     */
    void run(const cv::Mat &matIn, cv::Mat &matOut) const override;

};



/*!
 * \brief Resizes the canvas of an image.
 *
 * The `ResizeCanvas` class changes the canvas dimensions of an image. If the canvas is expanded,
 * the added areas are filled with a specified color. If reduced, the image is cropped.
 */
class TL_EXPORT ResizeCanvas
  : public ImageProcess
{

public:

    /*!
     * \brief Canvas position relative to the image.
     *
     * Determines how the image is aligned within the canvas after resizing.
     */
    enum class Position
    {
        bottom_center, /*!< Align to the bottom center of the canvas. */
        bottom_left,   /*!< Align to the bottom left of the canvas. */
        bottom_right,  /*!< Align to the bottom right of the canvas. */
        center,        /*!< Align to the center of the canvas. */
        center_left,   /*!< Align to the center left of the canvas. */
        center_right,  /*!< Align to the center right of the canvas. */
        top_center,    /*!< Align to the top center of the canvas. */
        top_left,      /*!< Align to the top left of the canvas. */
        top_right,     /*!< Align to the top right of the canvas. */
        custom         /*!< Use a custom top-left point specified by the user. */
    };

private:

    int mWidth;
    int mHeight;
    cv::Point mTopLeft;
    Color mColor;
    Position mPosition;

public:

    /*!
     * \brief Constructor specifying canvas dimensions and alignment.
     * \param[in] width New canvas width.
     * \param[in] height New canvas height.
     * \param[in] color Background color for expanded areas. Defaults to black.
     * \param[in] position Alignment of the image within the canvas. Defaults to \c top_left.
     * \see Position
     */
    ResizeCanvas(int width,
                 int height,
                 const Color &color = Color(),
                 const Position &position = Position::top_left);

    /*!
     * \brief Constructor specifying canvas dimensions and top-left alignment.
     * \param[in] width New canvas width.
     * \param[in] height New canvas height.
     * \param[in] point Coordinates of the top-left corner.
     * \param[in] color Background color for expanded areas. Defaults to black.
     */
    ResizeCanvas(int width,
                 int height,
                 const cv::Point &point,
                 const Color &color = Color());


    /*!
     * \brief Executes the canvas resizing operation.
     * \param[in] matIn Input image.
     * \param[out] matOut Output image with resized canvas.
     */
    void run(const cv::Mat &matIn, cv::Mat &matOut) const override;

};

/*! \} */ 

} // End namespace tl

