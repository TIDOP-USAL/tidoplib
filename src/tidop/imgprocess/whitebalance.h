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
 * License along with Foobar. If not, see <http://www.gnu.org/licenses/>. *
 *                                                                        *
 * @license LGPL-3.0 <https://www.gnu.org/licenses/lgpl-3.0.html>         *
 *                                                                        *
 **************************************************************************/

#ifndef TL_IMGPROCESS_WHITE_BALANCE_H
#define TL_IMGPROCESS_WHITE_BALANCE_H

#include "config_tl.h"

#ifdef HAVE_OPENCV
#include <opencv2/core.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/xphoto/white_balance.hpp>

#include "tidop/core/defs.h"
#include "tidop/graphic/color.h"
#include "tidop/imgprocess/imgprocess.h"

namespace tl
{

// Balance de blancos
// https://courses.cs.washington.edu/courses/cse467/08au/labs/l5/whiteBalance.pdf


/*! \addtogroup ImgProc
 *  \{
 */


/*! \defgroup WhiteBalance Balance de blancos
 *
 *  \{
 */

/* ---------------------------------------------------------------------------------- */

#if defined CV_VERSION_MAJOR && CV_VERSION_MAJOR >= 3
#  if defined CV_VERSION_MINOR && CV_VERSION_MINOR >= 2

/*!
 * \brief Balance de blancos  Gray World
 */
class TL_EXPORT Grayworld 
  : public ImageProcess
{

public:

  /*!
   * \brief Constructora Gray World
   */
  Grayworld();

  /*!
   * \brief Ejecuta el proceso
   * \param[in] matIn Imagen de entrada
   * \param[out] matOut Imagen de salida
   */
  void run(const cv::Mat &matIn, cv::Mat &matOut) const override;

private:

  /*!
   * \brief Tamaño del kernel
   */
  cv::Ptr<cv::xphoto::GrayworldWB> mGrayworld;
};

#  endif // CV_VERSION_MINOR
#endif // CV_VERSION_MAJOR

/* ---------------------------------------------------------------------------------- */

/*!
 * \brief Balance de blancos White Patch
 *
 * Asume que los valores máximos de color en los tres canales de la imagen es el
 * color del blanco bajo la luz de la escena, entonces se elimina el efecto de
 * esa luz y se impone una luz blanca
 * Estimación del color de la luz de la escena de la imagen:
 * Rmax(I): Valor máximo del canal rojo de la imagen I
 * Gmax(I): Valor máximo del canal verde de la imagen I
 * Bmax(I): Valor máximo del canal azul de la imagen I
 * Para una luz blanca de (255,255,255) se elimina la luz de la escena y se
 * introduce la luz blanca:
 * (R, G, B) -> ((255/Rmax(I))*R, (255/Gmax(I))*G, (255/Gmax(I))*G)
 */
class TL_EXPORT WhitePatch 
  : public ImageProcess
{

public:

  /*!
   * \brief Constructora WhitePatch
   * \param[in] white Luz blanca. Por defecto (255, 255, 255)
   */
  WhitePatch(const graph::Color &white = graph::Color(graph::Color::Name::white));

  /*!
   * \brief Ejecuta el proceso
   * \param[in] matIn Imagen de entrada
   * \param[out] matOut Imagen de salida
   */
  void run(const cv::Mat &matIn, cv::Mat &matOut) const override;

  /*!
   * \brief Establece la luz blanca
   * \param[in] white Luz blanca. Por defecto (255, 255, 255)
   */
  void setWhite(const graph::Color &white = graph::Color(graph::Color::Name::white));

private:
  
  double scaleRed(const cv::Mat &red) const;
  double scaleGreen(const cv::Mat &green) const;
  double scaleBlue(const cv::Mat &blue) const;

private:

  graph::Color mWhite;
};

/*! \} */ // end of WhiteBalance

/*! \} */ // end of ImgProc

} // End namespace TL

#endif // HAVE_OPENCV

#endif // TL_IMGPROCESS_WHITE_BALANCE_H
