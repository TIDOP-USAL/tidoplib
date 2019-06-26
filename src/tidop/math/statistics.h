#ifndef TL_MATH_STATISTICS_H
#define TL_MATH_STATISTICS_H

#include "config_tl.h"
#include "tidop/core/defs.h"

#ifdef HAVE_OPENCV
#include "opencv2/core/core.hpp"
#endif

#include <vector>
#include <map>

namespace tl
{

namespace math
{

/*! \addtogroup Math
 *  \{
 */


/*! \defgroup Statistics Estadistica
 *  \{
 */

/* ---------------------------------------------------------------------------------- */
/*                                 ESTADISTICA                                        */
/* ---------------------------------------------------------------------------------- */

// Medidas de tendencia central:
// Una medida de tendencia central debe ir siempre acompañada de una medida de
// dispersión que nos permita evaluar su representatividad.
// - Media
// - Mediana
// - Moda

/*!
 * \brief Media
 * La media es la suma de todos los datos partido del número de datos.
 * Se representa como:
 * \f[ \overline{x} = \frac{\sum_{i=1}^n x_i}{n}  \f]
 * La medida de dispersión más utilizada para la media es la desviación típica o
 * desviación estándar, pero tambień puede ser otra como el recorrido, coeficiente
 * de variación, etc.
 * Es muy sensible a datos discordantes (outliers)
 * \param[in] first Iterador al inicio
 * \param[in] last Iterador al final
 * \return Valor de la media
 */
template<typename It> inline
double mean(It first, It last)
{
  double x = 0.;
  size_t n = static_cast<double>(std::distance(first, last));
  while (first != last) {
    x += static_cast<double>(*first++);
  }
  return x / n;
}

/*!
 * \brief La Mediana es el valor de la variable que toma la posición central de la distribución.
 * Es el valor que verifica que el 50% de las variables son mayores o iguales
 * que él y que el otro 50% son menores.
 * La mediana tiene que ir acompañada de una medida de dispersión (por lo general por el Recorrido
 * Intercuantilico)
 * \param[in] first Iterador al inicio
 * \param[in] last Iterador al final
 * \return Valor de la mediana
 */
template<typename It> inline
double median(It first, It last)
{
  auto n = std::distance(first, last);
  std::vector<typename std::iterator_traits<It>::value_type> copy_container(n);
  std::copy(first, last, copy_container.begin());
  std::sort(copy_container.begin(), copy_container.end());


  if (n % 2 != 0){
    return static_cast<double>(copy_container[n/2]);
  } else
    return static_cast<double>(copy_container[n / 2 - 1] + copy_container[n / 2]) / 2.;
}

/*!
 * \brief Moda
 * La moda es el valor de la variable mas veces se repite, el que
 * lleva asociada la mayor frecuencia absoluta.
 * \param[in] first Iterador al inicio
 * \param[in] last Iterador al final
 * \return Valor de la moda
 */
template<typename It> inline
typename std::iterator_traits<It>::value_type mode(It first, It last)
{
  using value_type = typename std::iterator_traits<It>::value_type;
  std::map<value_type, int> h;
  while (first != last) {
    h[*first++]++;
  }

  auto max = std::max_element(h.begin(), h.end(),
                              [](const std::pair<value_type, int> &p1,
                                 const std::pair<value_type, int> &p2){
    return p1.second < p2.second;
  });

  return max->first;
}

// Medidas de dispersión
// - Recorrido
// - Varianza
// - Desviación típica
// - Coeficiente de variación
// - Recorrido intercuantilico

/*!
 * \brief Rango o recorrido
 * El Rango es la diferencia entre el valor mayor y el menor del conjunto de datos
 * \f[ R=x_{max}-x_{min} \f]
 * \param[in] first Iterador al inicio
 * \param[in] last Iterador al final
 * \return Valor del rango para el conjunto de datos
 */
template<typename It> inline
typename std::iterator_traits<It>::value_type range(It first, It last)
{
  typename std::iterator_traits<It>::value_type _r{0};
  if (first != last){
    auto result = std::minmax_element(first, last);
    _r = *result.second - *result.first;
  }
  return _r;
}

/*!
 * \brief Rango o recorrido intercuantilico
 * El RI es la diferencia entre el tercer quartil y el primer cuartil
 * \f[ RI=Q_3-Q_1 \f]
 * \param[in] first Iterador al inicio
 * \param[in] last Iterador al final
 * \return Valor del rango para el conjunto de datos
 */
template<typename It> inline
double
interquartileRange(It first, It last)
{
  double q1;
  double q3;

  auto n = std::distance(first, last);
  std::vector<typename std::iterator_traits<It>::value_type> copy_container(n);
  std::copy(first, last, copy_container.begin());
  std::sort(copy_container.begin(), copy_container.end());

  q1 = (n % 4 != 0) ?
        static_cast<double>(copy_container[n/4]) :
        static_cast<double>(copy_container[n / 4 - 1] + copy_container[n / 4]) / 2.;

  q3 = (n % 4 != 0) ?
        static_cast<double>(copy_container[n*3/4]) :
        static_cast<double>(copy_container[n*3/4 - 1] + copy_container[n*3/4]) / 2.;

  return q3-q1;
}

/*!
 * \brief Varianza
 *
 * \f[ \sigma² = \frac{\sum_{i=1}^n (x_i - \overline{x})²}{n}  \f]
 *
 * \param[in] first Iterador al inicio
 * \param[in] last Iterador al final
 * \return Varianza del conjunto de datos
 */
template<typename It> inline
double variance(It first, It last)
{
  auto n = std::distance(first, last);
  if (n <= 1) return 0.0;
  double _mean = mean(first, last);
  double sum = 0.0;
  double ep = 0.0;
  double aux = 0.0;
  while (first != last) {
    aux = *first++ - _mean;
    ep += aux;
    sum += aux*aux;
  }

  return (sum - ep*ep / n) / (n - 1);
}

/*!
 * \brief Desviación típica
 *
 * \f[ \sigma = +\sqrt{\frac{\sum_{i=1}^n (x_i - \overline{x})²}{n}}  \f]
 *
 * \param[in] first Iterador al inicio
 * \param[in] last Iterador al final
 * \return Desviación típica del conjunto de datos
 */
template<typename It> inline
double standarDeviation(It first, It last)
{
  return sqrt(variance(first, last));
}

/*!
 * \brief Coeficiente de variación
 * \f[ C_V = \frac{\sigma}{|\bar{x}|} \f]
 * \param[in] first Iterador al inicio
 * \param[in] last Iterador al final
 * \return Coeficiente de variación para el conjunto de datos
 */
template<typename It> inline
double coefficientOfVariation(It first, It last)
{
  return static_cast<double>(standarDeviation(first, last) / fabs(mean(first, last)));
}




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

  if (variance != 0.) {
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

  if (variance != 0.) {
    _kurtosis /= (n*variance*variance);
    return _kurtosis - 3.;
  } else return 0.;// "No kurtosis when variance = 0 (in moment)")
}

//TL_EXPORT double computeMedian(const std::vector<double> &input);
//
//TL_EXPORT double computeTempMAD(const std::vector<double> &input, double median);
//
//TL_EXPORT bool isOutlier(double temp, double median, double mad);

/*! \} */ // end of Statistic

/*! \} */ // end of Math

} // Fin namespace math

} // Fin namespace TL

#endif // TL_MATH_STATISTICS_H
