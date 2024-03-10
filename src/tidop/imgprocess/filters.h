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
#include <opencv2/core.hpp>

#include "tidop/core/defs.h"
#include "tidop/imgprocess/imgprocess.h"
#include "tidop/imgprocess/filter/bilateral.h"
#include "tidop/imgprocess/filter/blur.h"
#include "tidop/imgprocess/filter/box.h"
#include "tidop/imgprocess/filter/canny.h"
#include "tidop/imgprocess/filter/convolution.h"
#include "tidop/imgprocess/filter/gaussblur.h"
#include "tidop/imgprocess/filter/laplacian.h"
#include "tidop/imgprocess/filter/medianblur.h"
#include "tidop/imgprocess/filter/sobel.h"


namespace tl
{

/*! \addtogroup ImgProc
 *  \{
 */


/*! \defgroup Filters Image Filters
 *
 * Types of filters:
 *
 * - Smoothing
 * - Enhancement
 * - Noise reduction
 * - Edge detection
 *
 * <h3>Low-pass filters (image smoothing)</h3>
 *
 * - Mean filter: replaces each pixel with the average value of its neighbors
 * - Median filter: replaces with the median value of neighboring pixels (usually performs better than the mean filter).
 * - Gaussian filter: approximation to the Gaussian distribution.
 *
 *
 * <h3>Image enhancement filters</h3>
 *
 *
 * <h3>Noise reduction filters:</h3>
 *
 * Noise is random variation in brightness or color in digital images produced by the input device.
 *
 * <h4>Types of noise:</h4>
 *
 * - Impulse Noise or "Salt and Pepper": pixels in the image are very different in color or intensity from surrounding pixels. Generally, this type of noise affects a small number of pixels in the image.
 * - Gaussian Noise: In Gaussian noise, each and every pixel making up the image is affected according to a normal or Gaussian distribution.
 * - Uniform Noise: Takes values in a certain interval equiprobably. It occurs in fewer real-life situations.
 *
 * <h3>Edge detection filters</h3>
 *
 *
 *  \{
 */


/*! \} */ // end of Filters

/*! \} */ // end of ImgProc

} // End namespace tl

#endif // TL_HAVE_OPENCV