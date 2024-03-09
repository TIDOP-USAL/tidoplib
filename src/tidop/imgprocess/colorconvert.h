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
#include "opencv2/core/core.hpp"

#include "tidop/core/defs.h"
#include "tidop/imgprocess/imgprocess.h"
#include "tidop/graphic/color.h"

namespace tl
{


/*! \addtogroup ImgProc
 */


 /*!
  * \brief Converts an RGB matrix to CMYK
  * \param[in] rgb RGB image
  * \param[out] cmyk CMYK image
  */
TL_EXPORT void rgbToCmyk(const cv::Mat &rgb, cv::Mat &cmyk);


/*!
 * \brief Converts a CMYK matrix to RGB
 * \param[in] cmyk CMYK image
 * \param[out] rgb RGB image
 */
TL_EXPORT void cmykToRgb(const cv::Mat &cmyk, cv::Mat &rgb);


/*!
 * \brief Converts an RGB matrix to HSL
 * \param[in] rgb RGB image
 * \param[out] hsl HSL image
 */
TL_EXPORT void rgbToHSL(const cv::Mat &rgb, cv::Mat &hsl);


/*!
 *\brief Converts an HSL matrix to RGB
 *\param[in] hsl HSL image
 *\param[out] rgb RGB image
 */
TL_EXPORT void hslToRgb(const cv::Mat &hsl, cv::Mat &rgb);


/*!
 * \brief Converts an RGB matrix to HSV
 * \param[in] rgb RGB image
 * \param[out] hsv HSV image
 */
TL_EXPORT void rgbToHSV(const cv::Mat &rgb, cv::Mat &hsv);


/*!
 * \brief Converts an HSV matrix to RGB
 * \param[in] hsv HSV image
 * \param[out] rgb RGB image
 */
TL_EXPORT void hsvToRgb(const cv::Mat &hsv, cv::Mat &rgb);


/*!
 * \brief Converts an RGB matrix to grayscale
 * \param[in] rgb RGB image
 * \param[out] gray Grayscale image
 */
TL_EXPORT void rgbToLuminance(const cv::Mat &rgb, cv::Mat &gray);


/*!
 * \brief Converts an image to chromaticity coordinates
 * \param[in] rgb RGB image
 * \param[out] chromaCoord Chromaticity coordinates image
 */
TL_EXPORT void chromaticityCoordinates(const cv::Mat &rgb, cv::Mat &chromaCoord);






/*!
 * \brief Color mode conversion.
 * Conversion between different color modes.
 */
class TL_EXPORT ColorConversion
  : public ImageProcess
{
public:

    /*!
     * Color models
     */
    enum class ColorModel
    {
        rgb,
        rgba,
        cmyk,
        hsl,
        hsv,
        luminance,
        chromaticity
    };

private:

    ColorModel mModelIn;
    ColorModel mModelOut;

public:

    ColorConversion(ColorModel modelIn, ColorModel modelOut);
    ~ColorConversion() override = default;

    /*!
     * \brief Executes the process.
     * \param[in] matIn Input image.
     * \param[out] matOut Output image.
     */
    void run(const cv::Mat &matIn, cv::Mat &matOut) const override;

    /*!
     * \brief Sets the input color model.
     * \param[in] modelIn Input color model.
     */
    void setInputColorModel(ColorModel modelIn);

    /*!
     * \brief Sets the output color model.
     * \param[in] modelOut Output color model.
     */
    void setOutputColorModel(ColorModel modelOut);

};


/*! \} */ // end of ImgProc

} // End namespace tl

#endif // TL_HAVE_OPENCV

