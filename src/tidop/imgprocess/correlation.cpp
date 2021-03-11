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
 
 #include "correlation.h"

#ifdef HAVE_OPENCV

#include "core/messages.h"

namespace TL
{

/* ---------------------------------------------------------------------------------- */

//Correlation::Correlation(cv::Mat templ, Correlation::Type type, cv::Mat mask)
//  : ImgProcessing(process_type::CORRELATION), 
//    mTemplate(templ), 
//    mType(type), 
//    mMask(mask) 
//{
//}
//
//
//ImgProcessing::Status Correlation::execute(const cv::Mat &matIn, cv::Mat *matOut) const
//{
//  if (matIn.empty()) return ImgProcessing::Status::INCORRECT_INPUT_DATA;
//  try {
//    if (!mMask.emty() && mType == Type::CCORR) { 
//      cv::matchTemplate(matIn, mTemplate, *matOut, mMatchMethod, mMask); 
//    } else { 
//      cv::matchTemplate(matIn, mTemplate, *matOut, mMatchMethod); 
//    }
//  } catch (cv::Exception &e){
//    msgError(e.what());
//    return ImgProcessing::Status::PROCESS_ERROR;
//  }
//  return ImgProcessing::Status::OK;
//}
//
//void Correlation::setParameters(cv::Mat templ, Type type, cv::Mat mask)
//{
//  mTemplate = templ;
//  mType = type;
//  mMask = mask;
//}

/* ---------------------------------------------------------------------------------- */

}

#endif // HAVE_OPENCV