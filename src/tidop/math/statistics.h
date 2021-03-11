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

#ifndef TL_MATH_STATISTICS_H
#define TL_MATH_STATISTICS_H

#include "config_tl.h"
#include "tidop/core/defs.h"

#include <vector>

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
typename std::enable_if<
    std::is_integral<typename std::iterator_traits<It>::value_type>::value,
    double>::type
mean(It first, It last)
{
  double x = 0.;
  double i = 1.;
  while(first != last) {
    x += (static_cast<double>(*first++) - x)/i;
    i++;
  }
  return x;
}

template<typename It> inline
typename std::enable_if<
    std::is_floating_point<typename std::iterator_traits<It>::value_type>::value,
    typename std::iterator_traits<It>::value_type>::type
mean(It first, It last)
{
  using T = typename std::iterator_traits<It>::value_type;

  T x{0};
  T i{1};

  while(first != last) {
    x += (*first++ - x)/i;
    i++;
  }
  return x;
}

/*!
 * \brief Media
 * La media es la suma de todos los datos partido del número de datos.
 * Se representa como:
 * \f[ \overline{x} = \frac{\sum_{i=1}^n x_i}{n}  \f]
 * La medida de dispersión más utilizada para la media es la desviación típica o
 * desviación estándar, pero tambień puede ser otra como el recorrido, coeficiente
 * de variación, etc.
 * Es muy sensible a datos discordantes (outliers)
 * \param[in] container Objeto contenedor
 * \return Valor de la media
 */
template<typename T> inline
double mean(const T &container)
{
  return mean(container.begin(), container.end());
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
 * \brief La Mediana es el valor de la variable que toma la posición central de la distribución.
 * Es el valor que verifica que el 50% de las variables son mayores o iguales
 * que él y que el otro 50% son menores.
 * La mediana tiene que ir acompañada de una medida de dispersión (por lo general por el Recorrido
 * Intercuantilico)
 * \param[in] container Objeto contenedor
 * \return Valor de la mediana
 */
template<typename T> inline
double median(const T &container)
{
  return median(container.begin(), container.end());
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

/*!
 * \brief Moda
 * La moda es el valor de la variable mas veces se repite, el que
 * lleva asociada la mayor frecuencia absoluta.
 * \param[in] container Objeto contenedor
 * \return Valor de la moda
 */
template<typename T> inline
typename T::value_type mode(const T &container)
{
  return mode(container.begin(), container.end());
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
 * \brief Rango o recorrido
 * El Rango es la diferencia entre el valor mayor y el menor del conjunto de datos
 * \f[ R=x_{max}-x_{min} \f]
 * \param[in] container Objeto contenedor
 * \return Valor del rango para el conjunto de datos
 */
template<typename T> inline
typename T::value_type range(const T &container)
{
  return range(container.begin(), container.end());
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
double interquartileRange(It first, It last)
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
 * \brief Rango o recorrido intercuantilico
 * El RI es la diferencia entre el tercer quartil y el primer cuartil
 * \f[ RI=Q_3-Q_1 \f]
 * \param[in] container Objeto contenedor
 * \return Valor del rango para el conjunto de datos
 */
template<typename T> inline
double interquartileRange(const T &container)
{
  return interquartileRange(container.begin(), container.end());
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
 * \brief Varianza
 *
 * \f[ \sigma² = \frac{\sum_{i=1}^n (x_i - \overline{x})²}{n}  \f]
 *
 * \param[in] container Objeto contenedor
 * \return Varianza del conjunto de datos
 */
template<typename T> inline
double variance(const T &container)
{
  return variance(container.begin(), container.end());
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
 * \brief Desviación típica
 *
 * \f[ \sigma = +\sqrt{\frac{\sum_{i=1}^n (x_i - \overline{x})²}{n}}  \f]
 *
 * \param[in] container Objeto contenedor
 * \return Desviación típica del conjunto de datos
 */
template<typename T> inline
double standarDeviation(const T &container)
{
  return standarDeviation(container.begin(), container.end());
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
  return static_cast<double>(standarDeviation(first, last) 
                             / fabs(mean(first, last)));
}

/*!
 * \brief Coeficiente de variación
 * \f[ C_V = \frac{\sigma}{|\bar{x}|} \f]
 * \param[in] first Iterador al inicio
 * \param[in] last Iterador al final
 * \return Coeficiente de variación para el conjunto de datos
 */
template<typename T> inline
double coefficientOfVariation(const T &container)
{
  return coefficientOfVariation(container.begin(), container.end());
}

/*!
 * \brief covariance
 * \f[ \S_{xy} = \frac{\sum_{i=1}^n (x_i - \overline{x})(y_i - \overline{y})}{n}  \f]
 * \param first
 * \param last
 * \return
 */
template<typename It> inline
double covariance(It first_x, It last_x, It first_y, It last_y)
{
  auto n_x = std::distance(first_x, last_x);
  auto n_y = std::distance(first_y, last_y);
  if (n_x != n_y || n_x <= 1) return 0.;

  double mean_x = mean(first_x, last_x);
  double mean_y = mean(first_y, last_y);
  double sum = 0.0;
  double x = 0.0;
  double y = 0.0;
  while (first_x != last_x) {
    x = *first_x++ - mean_x;
    y = *first_y++ - mean_y;
    sum += x*y;
  }

  return sum / n_x;
}

/*!
 * \brief covariance
 * \f[ \S_{xy} = \frac{\sum_{i=1}^n (x_i - \overline{x})(y_i - \overline{y})}{n}  \f]
 * \param[in] container1 Objeto contenedor
 * \param[in] container2 Objeto contenedor
 * \return
 */
template<typename T> inline
double covariance(const T &container1, const T &container2)
{
  return covariance(container1.begin(), 
                    container1.end(), 
                    container2.begin(), 
                    container2.end());
}

/*!
 * \brief pearsonCorrelationCoefficient
 * \param[in] first_x
 * \param[in] last_x
 * \param[in] first_y
 * \param[in] last_y
 * \return
 */
template<typename It> inline
double pearsonCorrelationCoefficient(It first_x, It last_x, 
                                     It first_y, It last_y)
{
  auto n_x = std::distance(first_x, last_x);
  auto n_y = std::distance(first_y, last_y);
  if (n_x != n_y || n_x <= 1) return 0.;
  return covariance(first_x, last_x, first_y, last_y)
         / (standarDeviation(first_x, last_x)*standarDeviation(first_y, last_y));
}

/*!
 * \brief pearsonCorrelationCoefficient
 * \param[in] container1 Objeto contenedor
 * \param[in] container2 Objeto contenedor
 * \return
 */
template<typename T> inline
double pearsonCorrelationCoefficient(const T &container1, 
                                     const T &container2)
{
  return pearsonCorrelationCoefficient(container1.begin(),
                                       container1.end(), 
                                       container2.begin(), 
                                       container2.end());
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

//average deviation or mean absolute deviation
// Estimador mas robusto que la desviacion estandar y la varianza
template<typename It> inline
double averageAbsoluteDeviation(It first, It last)
{
  size_t n = std::distance(first, last);
  if (n <= 1) return 0.0;
  double _mean = mean(first, last);
  double sum = 0.0;
  while (first != last) {
    sum += std::abs(static_cast<double>(*first++) - _mean);
  }
  return sum / n;
}

template<typename T> inline
double averageAbsoluteDeviation(const T &container)
{
  return averageAbsoluteDeviation(container.begin(), container.end());
}

/*!
 * \brief skewness
 * \param[in] first Iterador al inicio
 * \param[in] last Iterador al final
 * \return
 */
template<typename It> inline
double skewness(It first, It last)
{
  size_t n = std::distance(first, last);
// Varianza
  if (n <= 1) return 0.;
  double _mean = mean(first, last);
  double ep = 0.;
  double aux = 0.;
  double aux2 = 0.;
  double skew = 0.;
  double sum = 0.;
  while (first != last) {
    aux = static_cast<double>(*first++) - _mean;
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

/*!
 * \brief skewness
 * \param container Objeto contenedor
 * \return
 */
template<typename T> inline
double skewness(const T &container)
{
  return skewness(container.begin(), container.end());
}


/*!
 * \brief kurtosis
 * \param[in] first Iterador al inicio
 * \param[in] last Iterador al final
 * \return
 */
template<typename It> inline
double kurtosis(It first, It last)
{
  size_t n = std::distance(first, last);
// Varianza
  if (n <= 1) return 0.;
  double _mean = mean(first, last);
  double ep = 0.;
  double aux = 0.;
  double _kurtosis = 0.;
  double aux2 = 0.;
  double sum = 0.;

  while (first != last) {
    aux = static_cast<double>(*first++) - _mean;
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


template<typename T> inline
double kurtosis(const T &container)
{
  return kurtosis(container.begin(), container.end());
}


#ifdef HAVE_OPENCV

/*!
 * \brief Varianza del laplaciano
 * \param[in] src
 * \return
 */
TL_EXPORT double laplacianVariance(const cv::Mat &src);

#endif

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
