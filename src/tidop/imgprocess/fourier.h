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

#ifndef TL_IMGPROCESS_FOURIER_H
#define TL_IMGPROCESS_FOURIER_H

#include "config_tl.h"
#include "tidop/core/defs.h"

#ifdef TL_HAVE_OPENCV
TL_DISABLE_WARNING(TL_WARNING_DEPRECATED)
#include <opencv2/core.hpp>
#include <opencv2/imgproc.hpp>
TL_ENABLE_WARNING(TL_WARNING_DEPRECATED)


namespace tl
{

/*!
 * \brief The Fourier class
 * Discrete Fourier Transform
 */
class TL_EXPORT DiscreteFourierTrf
{
public:

  /*!
   * \brief Constuctora transformada discreta de Fourier
   */
  DiscreteFourierTrf(){}

  /*!
   * \brief Transformación directa
   * \param[in] source Imagen de entrada
   * \param[out] out Imagen de salida
   */
  void forward(const cv::Mat &source, cv::Mat *out) const;

  /*!
   * \brief Transformación inversa
   * \param[in] source Imagen de entrada
   * \param[out] out Imagen de salida
   */
  void inverse(const cv::Mat &source, cv::Mat *out) const;

  /*!
   * \brief Intercambia los cuadrantes
   * \param[in] image
   */
  void shift(cv::Mat &image) const;
};

/*!
 * \brief FourierLinesDetection
 * Algoritmo de detección de conductores aplicando la transformada discreta de Fourier
 * \param[in] source
 * \param[in] pts
 * \param[in] angle Angulo medido desde el eje x en el sistema de coordenadas pixel 
 * \return
 */
TL_EXPORT double fourierLinesDetection(const cv::Mat &source, std::vector<int> &cols, std::vector<std::vector<cv::Point>> *pts, double *angle = NULL);

//TL_EXPORT double verticalLinesDetection(cv::Mat &source, std::vector<cv::Point> *pts);
//
//TL_EXPORT double horizontalLinesDetection(cv::Mat &source, std::vector<std::vector<cv::Point>> *pts);

} // End namespace TL

#endif

#endif // TL_IMGPROCESS_FOURIER_H
