#ifndef I3D_FOURIER_H
#define I3D_FOURIER_H

#include "opencv2/core/core.hpp"
#include "opencv2/imgproc/imgproc.hpp"

#include "core\defs.h"

namespace I3D
{

/*!
 * \brief The Fourier class
 * Discrete Fourier Transform
 */
class I3D_EXPORT DiscreteFourierTrf
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
double I3D_EXPORT fourierLinesDetection(cv::Mat &source, std::vector<int> &cols, std::vector<std::vector<cv::Point>> *pts, double *angle = NULL);

} // End namespace I3D

#endif // I3D_FOURIER_H
