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

#include "tidop/core/base/defs.h"
#include "tidop/rastertools/process/imgprocess.h"
#include "tidop/graphic/color.h"

namespace tl
{


/*! \addtogroup ImgProc
 *  \{
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
 *
 * This class provides the functionality for converting between different color modes in images.
 *
 * The class supports a variety of color models, enabling seamless conversion between them, which
 * is especially useful in scenarios such as printing (CMYK), web graphics (RGB), and image analysis
 * (HSL, HSV).
 *
 * ## Supported Color Models:
 * - **RGB** (Red, Green, Blue): A color model commonly used in display devices such as monitors.
 * - **RGBA** (Red, Green, Blue, Alpha): An extension of RGB that includes an alpha channel for transparency.
 * - **CMYK** (Cyan, Magenta, Yellow, Black): A color model used in color printing.
 * - **HSL** (Hue, Saturation, Lightness): A color model often used in graphics software for adjusting colors.
 * - **HSV** (Hue, Saturation, Value): A color model commonly used in image analysis and computer vision.
 * - **Luminance**: A grayscale representation that reflects the intensity of light in an image.
 * - **Chromaticity**: A representation of color based on its chroma (color intensity) and hue.
 *
 * The class provides methods for setting the input and output color models and performing the conversion
 * between the specified models.
 *
 * ## Example usage:
 * \code
 *   cv::Mat inputImage = cv::imread("image.jpg");
 *   cv::Mat outputImage;
 *   ColorConversion converter(ColorConversion::ColorModel::rgb, ColorConversion::ColorModel::hsl);
 *   converter.run(inputImage, outputImage);
 * \endcode
 *
 * \ingroup ImgProc
 */
class TL_EXPORT ColorConversion
  : public ImageProcess
{
public:

    /*!
     * \brief Color models enumeration.
     *
     * This enum defines the various color models supported by the class for conversion.
     */
    enum class ColorModel
    {
        rgb,           ///< Red, Green, Blue color model
        rgba,          ///< Red, Green, Blue, Alpha color model (with transparency)
        cmyk,          ///< Cyan, Magenta, Yellow, Black color model (used in printing)
        hsl,           ///< Hue, Saturation, Lightness color model
        hsv,           ///< Hue, Saturation, Value color model (used in image analysis)
        luminance,     ///< Grayscale image representation (luminance only)
        chromaticity   ///< Color intensity and hue representation
    };

private:

    ColorModel mModelIn;
    ColorModel mModelOut;

public:

    /*!
     * \brief Constructor.
     *
     * Constructs a `ColorConversion` object with specified input and output color models.
     *
     * \param[in] modelIn  The input color model.
     * \param[in] modelOut The output color model.
     */
    ColorConversion(ColorModel modelIn, ColorModel modelOut);

    /*!
     * \brief Destructor.
     *
     * The destructor of `ColorConversion` class.
     */
    ~ColorConversion() override = default;

    /*!
     * \brief Executes the color conversion process.
     *
     * Converts the input image from the input color model to the output color model.
     *
     * \param[in] matIn  The input image in the source color model.
     * \param[out] matOut The output image in the target color model.
     */
    void run(const cv::Mat &matIn, cv::Mat &matOut) const override;

    /*!
     * \brief Sets the input color model.
     * 
     * Use this method to specify the input color model of the image to be converted.
     * 
     * \param[in] modelIn The input color model.
     */
    void setInputColorModel(ColorModel modelIn);

    /*!
     * \brief Sets the output color model.
     *
     * Use this method to specify the output color model for the converted image.
     *
     * \param[in] modelOut The output color model.
     */
    void setOutputColorModel(ColorModel modelOut);

};


/*! \} */

} // End namespace tl

#endif // TL_HAVE_OPENCV

