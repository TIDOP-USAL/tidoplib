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

#ifndef TL_IMGPROCESS_ANAGLYPH_H
#define TL_IMGPROCESS_ANAGLYPH_H

#include "config_tl.h"

#ifdef TL_HAVE_OPENCV
#include <opencv2/core.hpp>


#include "tidop/core/defs.h"

namespace tl
{


/*!
 * \brief construye una imagen estereo en modo anaglifo o 'side by side'
 *
 * \param[in] left Imagen derecha
 * \param[in] right Imagen Izquierda
 * \param[out] stimage imagen estereo
 * \param[in] mode Modo de visualización
 * \see StereoMode
 */
class StereoImage
{

public:

  enum class Mode
  {
    normal,
    gray,
    dubois,
    half_color,
    optimized,
    red_blue,
    side_by_side
  };

public:

  /*!
   * \brief Constructor
   *
   * \param[in] mode Modo de visualización
   * \see StereoMode
   */
  StereoImage(Mode mode = Mode::normal);
	~StereoImage();

  cv::Mat run(const cv::Mat &left,
              const cv::Mat &right);

  Mode mode() const;
  void setMode(Mode mode);

private:

  Mode mMode;
};



} // End namespace tl

#endif

#endif // TL_IMGPROCESS_ANAGLYPH_H
