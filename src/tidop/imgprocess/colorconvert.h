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
TL_EXPORT void rgbToCmyk(const cv::Mat &rgb, cv::Mat &cmyk);


/*!
 * \brief Convierte una matriz de CMYK a RGB
 * \param[in] cmyk Imagen cmyk
 * \param[out] rgb Imagen rgb
 */
TL_EXPORT void cmykToRgb(const cv::Mat &cmyk, cv::Mat &rgb);


/*!
 * \brief Convierte una matriz de RGB a HSL
 * \param[in] rgb
 * \param[out] hsl
 */
TL_EXPORT void rgbToHSL(const cv::Mat &rgb, cv::Mat &hsl);


/*!
 * \brief Convierte una matriz de HSL a RGB
 * \param[in] hsl
 * \param[out] rgb
 */
TL_EXPORT void hslToRgb(const cv::Mat &hsl, cv::Mat &rgb);


/*!
 * \brief Convierte una matriz de RGB a HSV
 * \param[in] rgb
 * \param[out] hsv
 */
TL_EXPORT void rgbToHSV(const cv::Mat &rgb, cv::Mat &hsv);


/*!
 * \brief Convierte una matriz de HSV a RGB
 * \param[in] hsv
 * \param[out] rgb
 */
TL_EXPORT void hsvToRgb(const cv::Mat &hsv, cv::Mat &rgb);


/*!
 * \brief Convierte una matriz de RGB a escala de grises
 * \param[in] rgb
 * \param[out] gray
 */
TL_EXPORT void rgbToLuminance(const cv::Mat &rgb, cv::Mat &gray);


/*!
 * \brief Conversión de una imagen a coordenadas cromáticas
 * \param[in] rgb Imagen RGB
 * \param[out] chromaCoord Imagen en coordenadas cromaticas
 */
TL_EXPORT void chromaticityCoordinates(const cv::Mat &rgb, cv::Mat &chromaCoord);




/* ---------------------------------------------------------------------------------- */

/*!
 * \brief Conversión del modo de color.
 * Conversión entre distintos tipos de modos de color
 */
class TL_EXPORT ColorConversion 
  : public ImageProcess
{
public:
  
  /*!
   * Modelos de color
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

public:

  ColorConversion(ColorModel modelIn, ColorModel modelOut);
  ~ColorConversion() override = default;

  /*!
   * \brief Ejecuta el proceso.
   * \param[in] matIn Imagen de entrada.
   * \param[out] matOut Imagen de salida.
   */
  void run(const cv::Mat &matIn, cv::Mat &matOut) const override;

  /*!
   * \brief Establece el modelo de color de entrada
   * \param[in] modelIn Modelo de color de entrada
   */
  void setInputColorModel(ColorModel modelIn);

  /*!
   * \brief Establece el modelo de color de salida
   * \param[in] modelOut Modelo de color de salida
   */
  void setOutputColorModel(ColorModel modelOut);

private:

  ColorModel mModelIn;
  ColorModel mModelOut;
};

/* ---------------------------------------------------------------------------------- */

/*! \} */ // end of ImgProc

} // End namespace tl

#endif // HAVE_OPENCV

#endif // TL_IMGPROCESS_COLOR_CONVERSION_H
