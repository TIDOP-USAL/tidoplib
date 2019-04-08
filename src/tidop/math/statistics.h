#ifndef TL_MATH_STATISTICS_H
#define TL_MATH_STATISTICS_H

#include "config_tl.h"
#include "tidop/core/defs.h"

#ifdef HAVE_OPENCV
#include "opencv2/core/core.hpp"
#endif

namespace tl
{

namespace math
{

/*! \addtogroup Math
 *  \{
 */

/* ---------------------------------------------------------------------------------- */
/*                                 ESTADISTICA                                        */
/* ---------------------------------------------------------------------------------- */

template<typename itIn, typename itOut> inline
double rootMeanSquareError(itIn in_first, itIn in_last, itOut out_first)
{
  double rms = 0.;
  while (in_first != in_last) {
  //  rms
  //  trf->transform(*in_first++, &(*out_first++), trfOrder);
  }
  return rms;
}


#ifdef HAVE_OPENCV

/*!
 * \brief Varianza del laplaciano
 * \param[in] src
 * \return
 */
TL_EXPORT double laplacianVariance(const cv::Mat &src);

#endif

template<typename T> inline
double mean(const T &data)
{
  size_t n = data.size();
  if (n <= 1) return 0.0; //TODO: Mensaje de error
  //TODO: Comprobar que es un contenedor con tipos numéricos
  typename T::value_type sum = 0.0;
  for (auto &t : data) {
    sum += t;
  }
  return static_cast<double>(sum / n);
}

template<typename T> inline
double variance(const T &data)
{
  size_t n = data.size();
  if (n <= 1) return 0.0; //TODO: Mensaje de error
  double _mean = mean(data);
  double sum = 0.0;
  double ep = 0.0;
  double aux = 0.0;
  for (auto &t : data) {
    aux = t - _mean;
    ep += aux;
    sum += aux*aux;
  }
  //return sum / (n-1);
  // corrected two-pass algorithm
  return (sum - ep*ep / n) / (n - 1);
}

template<typename T> inline
double standarDeviation(const T &data)
{
  return sqrt(variance(data));
}

//average deviation or mean absolute deviation
// Estimador mas robusto que la desviacion estandar y la varianza
template<typename T> inline
double averageAbsoluteDeviation(const T &data)
{
  size_t n = data.size();
  if (n <= 1) return 0.0; // Mensaje de error
  double _mean = mean(data);
  double sum = 0.0;
  for (auto &t : data) {
    sum += std::abs(t - _mean);
  }
  return sum / n;
}

template<typename T> inline
double skewness(const T &data)
{
  // Para que sea lo mas rapido posible no llamo a las funciones previas
  // Hago todo el desarrollo aqui para no repetir pasos
  size_t n = data.size();
// Varianza
  if (n <= 1) return 0.; // Mensaje de error
  double _mean = mean(data);
  double ep = 0.;
  double aux = 0.;
  double aux2 = 0.;
  double skew = 0.;
  double sum = 0.;
  for (auto &t : data) {
    aux = t - _mean;
    ep += aux;
    sum += (aux2 = aux*aux);
    skew += aux2 * aux;
  }

  double variance =  (sum - ep*ep / n) / (n - 1);  // Varianza Corrected two-pass formula

  if (variance) {
    double stdDev = sqrt(variance); // Desviación tipica
    return skew / (n*variance*stdDev);
  } else return 0.;// "No skew when variance = 0 (in moment)")
}

template<typename T> inline
double kurtosis(const T &data)
{
  size_t n = data.size();
// Varianza
  if (n <= 1) return 0.; // Mensaje de error
  double _mean = mean(data);
  double ep = 0.;
  double aux = 0.;
  double _kurtosis = 0.;
  double aux2 = 0.;
  double sum = 0.;

  for (auto &t : data) {
    aux = t - _mean;
    ep += aux;
    sum += (aux2 = aux*aux);
    _kurtosis += aux2;
  }

  double variance = (sum - ep*ep / n) / (n - 1);  // Varianza Corrected two-pass formula

  if (variance) {
    _kurtosis /= (n*variance*variance);
    return _kurtosis - 3.;
  } else return 0.;// "No kurtosis when variance = 0 (in moment)")
}

TL_EXPORT double computeMedian(const std::vector<double> &input);

TL_EXPORT double computeTempMAD(const std::vector<double> &input, double median);

TL_EXPORT bool isOutlier(double temp, double median, double mad);

/*! \} */ // end of Math

} // Fin namespace math

} // Fin namespace TL

#endif // TL_MATH_STATISTICS_H
