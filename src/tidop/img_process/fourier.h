#ifndef TL_IMGPROCESS_FOURIER_H
#define TL_IMGPROCESS_FOURIER_H

#include "config_tl.h"
#include "tidop/core/defs.h"

#ifdef HAVE_OPENCV
TL_DISABLE_WARNING(TL_WARNING_DEPRECATED)
#include "opencv2/core/core.hpp"
#include "opencv2/imgproc/imgproc.hpp"
TL_ENABLE_WARNING(TL_WARNING_DEPRECATED)


namespace TL
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
