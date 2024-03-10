/**************************************************************************
 *                                                                        *
 * Copyright (C) 2021 by Tidop Research Group                             *
 * Copyright (C) 2021 by Esteban Ruiz de O�a Crespo                       *
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
#include "opencv2/imgproc/imgproc.hpp"

#include "tidop/core/defs.h"
//#include "graphic_entities/color.h"
#include "tidop/imgprocess/imgprocess.h"

namespace tl
{

/*! \addtogroup ImgProc
 *  \{
 */


/*! \defgroup Correlation Correlaci�n en imagenes
 *
 *  \{
 */

/*!
 * \brief Correlaci�n o correspondencia de imagenes
 *
 *
 */
//class TL_EXPORT Correlation : public ImgProcessing
//{
//
//public:
//
//  enum class Type
//  {
//    SQDIFF, // cv::TemplateMatchModes::TM_SQDIFF_NORMED
//    CCORR,  // cv::TemplateMatchModes::TM_CCORR_NORMED
//    CCOEFF  // cv::TemplateMatchModes::TM_CCOEFF_NORMED
//  };
//
//private:
//
//  cv::Mat mTemplate;
//  cv::Mat mMask;
//  Type mType;
//
//public:
//
//  /*!
//   * \brief Constructora
//   * \param[in] templ
//   * \param[in] type
//   * \param[in] mask
//   */
//  Correlation(cv::Mat templ, Type type = Type::SQDIFF, cv::Mat mask = cv::Mat());
//
//  /*!
//   * \brief Ejecuta el proceso
//   * \param[in] matIn Imagen de entrada
//   * \param[out] matOut Imagen de salida
//   * \return Si los procesos se ejecutan correctamente devuelve ImgProcessing::Status::OK. 
//   * \see ImgProcessing::Status
//   */
//  ImgProcessing::Status execute(const cv::Mat &matIn, cv::Mat *matOut) const override;
//
//  /*!
//   * \brief 
//   * \param[in] templ
//   * \param[in] type
//   * \param[in] mask
//   */
//  void setParameters(cv::Mat templ, Type type = Type::SQDIFF, cv::Mat mask = cv::Mat());
//};


/*! \} */ // end of Correlation

/*! \} */ // end of ImgProc

} // End namespace tl

#endif // TL_HAVE_OPENCV
