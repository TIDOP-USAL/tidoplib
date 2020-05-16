#ifndef TL_IMGPROCESS_COLOR_CONVERSION_H
#define TL_IMGPROCESS_COLOR_CONVERSION_H

#include "config_tl.h"

#ifdef HAVE_OPENCV
#include "opencv2/core/core.hpp"
#include "opencv2/imgproc/imgproc.hpp"

#include "tidop/core/defs.h"
#include "tidop/imgprocess/imgprocess.h"
#include "tidop/graphic/color.h"

namespace tl
{


/*! \addtogroup ImgProc 
 */



/*!
 * \brief Convierte una matriz de RGB a CMYK
 * \param[in] rgb Imagen cmyk
 * \param[out] cmyk Imagen rgb
 */
TL_EXPORT void rgbToCmyk(const cv::Mat &rgb, cv::Mat *cmyk);


/*!
 * \brief Convierte una matriz de CMYK a RGB
 * \param[in] cmyk Imagen cmyk
 * \param[out] rgb Imagen rgb
 */
TL_EXPORT void cmykToRgb(const cv::Mat &cmyk, cv::Mat *rgb);


/*!
 * \brief Convierte una matriz de RGB a HSL
 * \param[in] rgb
 * \param[out] hsl
 */
TL_EXPORT void rgbToHSL(const cv::Mat &rgb, cv::Mat *hsl);


/*!
 * \brief Convierte una matriz de HSL a RGB
 * \param[in] hsl
 * \param[out] rgb
 */
TL_EXPORT void hslToRgb(const cv::Mat &hsl, cv::Mat *rgb);


/*!
 * \brief Convierte una matriz de RGB a HSV
 * \param[in] rgb
 * \param[out] hsv
 */
TL_EXPORT void rgbToHSV(const cv::Mat &rgb, cv::Mat *hsv);


/*!
 * \brief Convierte una matriz de HSV a RGB
 * \param[in] hsv
 * \param[out] rgb
 */
TL_EXPORT void hsvToRgb(const cv::Mat &hsv, cv::Mat *rgb);


/*!
 * \brief Convierte una matriz de RGB a escala de grises
 * \param[in] rgb
 * \param[out] gray
 */
TL_EXPORT void rgbToLuminance(const cv::Mat &rgb, cv::Mat *gray);


/*!
 * \brief Conversión de una imagen a coordenadas cromáticas
 * \param[in] rgb Imagen RGB
 * \param[out] chroma_rgb Imagen en coordenadas cromaticas
 */
TL_EXPORT void chromaticityCoordinates(const cv::Mat &rgb, cv::Mat *chroma_rgb);






/* ---------------------------------------------------------------------------------- */

/*!
 * \brief Conversión del modo de color.
 * Conversión entre distintos tipos de modos de color
 */
class TL_EXPORT ColorConversion : public ImgProcessing
{
public:
  
  /*!
   * Modelos de color
   */
  enum class Model
  {
    rgb,
    rgba,
    cmyk,
    hsl,
    hsv,
    luminance,
    chromaticity
#ifdef TL_ENABLE_DEPRECATED_METHODS
    ,
    RGB = rgb,
    RGBA = rgba,
    CMYK = cmyk,
    HSL = hsl,
    HSV = hsv,
    LUMINANCE = luminance,
    CHROMATICITY = chromaticity
#endif
  };

private:

  Model mModelIn;
  
  Model mModelOut;

public:

  /*!
   * \brief Constructora de la clase
   */
  ColorConversion(Model modelIn, Model modelOut);

  //~ColorConversion();

  /*!
   * \brief Ejecuta el proceso.
   * \param[in] matIn Imagen de entrada.
   * \param[out] matOut Imagen de salida.
   * \return Si los procesos se ejecutan correctamente devuelve ImgProcessing::Status::OK. 
   * \see ImgProcessing::Status
   */
  ImgProcessing::Status execute(const cv::Mat &matIn, cv::Mat *matOut) const override;

  /*!
   * \brief Establece los parámetros
   * \param[in] modelIn Modelo de color de entrada
   * \param[in] modelOut Modelo de color de salida
   */
  void setParameters(Model modelIn, Model modelOut);
};

/* ---------------------------------------------------------------------------------- */

/*! \} */ // end of ImgProc

} // End namespace TL

#endif // HAVE_OPENCV

#endif // TL_IMGPROCESS_COLOR_CONVERSION_H
