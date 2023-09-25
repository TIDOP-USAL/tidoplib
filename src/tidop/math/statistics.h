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

#ifndef TL_MATH_STATISTICS_H
#define TL_MATH_STATISTICS_H

#include <vector>
#include <map>

#include "tidop/math/math.h"

namespace tl
{

namespace math
{

/*! \addtogroup math
 *  \{
 */


/*! \defgroup statistics Statistics
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


/*! \defgroup CentralTendency Medidas de tendencia central
 *  \{
 */
  
/*!
 * \brief La media es la suma de todos los datos partido del número de datos.
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
  double x{0};
  double i{1};

  while(first != last) {
    x += (static_cast<double>(*first++) - x) / i++;
  }
  return x;
}

template<typename It> inline
typename std::enable_if<
    std::is_floating_point<typename std::iterator_traits<It>::value_type>::value,
    typename std::iterator_traits<It>::value_type>::type
mean(It first, It last)
{
  using T = typename std::remove_cv<typename std::iterator_traits<It>::value_type>::type;

  T x{0};
  T i{1};

  while(first != last) {
    x += (*first++ - x)/i++;
  }

  return x;
}

/*!
 * \brief Quantile
 * \param[in] first Iterador al inicio
 * \param[in] last Iterador al final
 * \param[in] p [0,1]
 * \return
 */
template<typename It> inline
typename std::enable_if<
  std::is_integral<typename std::iterator_traits<It>::value_type>::value,
  double>::type
quantile(It first, It last, double p)
{
  double q;

  auto n = std::distance(first, last);
  std::vector<typename std::iterator_traits<It>::value_type> sort_vector(n);
  std::copy(first, last, sort_vector.begin());
  std::sort(sort_vector.begin(), sort_vector.end());

  double idx = static_cast<double>(n+1) * p - 1.;
  size_t idx_1 = static_cast<size_t>(std::floor(idx));
  size_t idx_2 = static_cast<size_t>(std::ceil(idx));

  if (idx_1 == idx_2) {
    q = static_cast<double>(sort_vector[idx_1]);
  } else {
    q = static_cast<double>(sort_vector[idx_1]) + static_cast<double>(sort_vector[idx_2] - sort_vector[idx_1]) * fabs(idx - static_cast<int>(idx));
  }

  return q;
}

template<typename It> inline
typename std::enable_if<
  std::is_floating_point<typename std::iterator_traits<It>::value_type>::value,
  typename std::iterator_traits<It>::value_type>::type
quantile(It first, It last, double p)
{
  using T = typename std::remove_cv<typename std::iterator_traits<It>::value_type>::type;

  T q;

  auto n = std::distance(first, last);
  std::vector<T> sort_vector(n);
  std::copy(first, last, sort_vector.begin());
  std::sort(sort_vector.begin(), sort_vector.end());

  double idx = static_cast<double>(n + 1) * p - 1.;
  size_t idx_1 = static_cast<size_t>(std::floor(idx));
  size_t idx_2 = static_cast<size_t>(std::ceil(idx));

  if (idx_1 == idx_2) {
    q = sort_vector[idx_1];
  } else {
    q = sort_vector[idx_1] + (sort_vector[idx_2] - sort_vector[idx_1]) * static_cast<T>(fabs(idx - static_cast<int>(idx)));
  }

  return q;
}

/*!
 * \brief La Mediana es el valor de la variable que toma la posición central de la distribución.
 * Es el valor que verifica que el 50% de las variables son mayores o iguales
 * que él y que el otro 50% son menores.
 * La mediana tiene que ir acompañada de una medida de dispersión (por lo general por el Recorrido
 * Intercuartílico)
 * \param[in] first Iterador al inicio
 * \param[in] last Iterador al final
 * \return Valor de la mediana
 */
template<typename It> inline
typename std::enable_if<
  std::is_integral<typename std::iterator_traits<It>::value_type>::value,
  double>::type
median(It first, It last)
{
  return tl::math::quantile(first, last, 0.5);
}

template<typename It> inline
typename std::enable_if<
  std::is_floating_point<typename std::iterator_traits<It>::value_type>::value,
  typename std::iterator_traits<It>::value_type>::type
median(It first, It last)
{
  return tl::math::quantile(first, last, 0.5);
}


/*!
 * \brief La moda es el valor de la variable que mas veces se repite, el que lleva asociada la mayor frecuencia absoluta.
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
 * \brief La moda es el valor de la variable mas veces se repite, el que lleva asociada la mayor frecuencia absoluta.
 * \param[in] container Objeto contenedor
 * \return Valor de la moda
 */
template<typename T> inline
typename T::value_type mode(const T &container)
{
  return mode(container.begin(), container.end());
}

/*! \} */ // end of Measure of Central Tendency


/*! \defgroup dispersion Statistical dispersion
 *  \{
 */

// Medidas de dispersión
// - Recorrido
// - Varianza
// - Desviación típica
// - Coeficiente de variación
// - Recorrido intercuantilico

/*!
 * \brief Sample Variance
 *
 * \f[ \sigma² = \frac{\sum_{i=1}^n (x_i - \overline{x})²}{n}  \f]
 *
 * \param[in] first Iterador al inicio
 * \param[in] last Iterador al final
 * \return Varianza del conjunto de datos
 */
template<typename It> inline
typename std::enable_if<
  std::is_integral<typename std::iterator_traits<It>::value_type>::value,
  double>::type
variance(It first, It last)
{
  auto n = std::distance(first, last);
  if (n <= 1) return consts::one<double>;

  double _mean = mean(first, last);
  double sum{};
  double ep{};
  double aux{};

  while (first != last) {
    aux = *first++ - _mean;
    ep += aux;
    sum += aux * aux;
  }

  return (sum - ep * ep / n) / (n - 1);
}

template<typename It> inline
typename std::enable_if<
  std::is_floating_point<typename std::iterator_traits<It>::value_type>::value,
  typename std::iterator_traits<It>::value_type>::type
variance(It first, It last)
{
  using T = typename std::iterator_traits<It>::value_type;

  auto n = std::distance(first, last);
  if (n <= 1) return consts::one<T>;

  T _mean = mean(first, last);
  T sum{};
  T ep{};
  T aux{};

  while (first != last) {
    aux = *first++ - _mean;
    ep += aux;
    sum += aux * aux;
  }

  return (sum - ep * ep / n) / (n - 1);
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
typename std::enable_if<
  std::is_integral<typename std::iterator_traits<It>::value_type>::value,
  double>::type
populationVariance(It first, It last)
{
  auto n = std::distance(first, last);
  if (n <= 1) return consts::one<double>;

  double _mean = mean(first, last);
  double sum{};
  double ep{};
  double aux{};

  while (first != last) {
    aux = *first++ - _mean;
    ep += aux;
    sum += aux * aux;
  }

  return (sum - ep * ep / n) / n;
}

template<typename It> inline
typename std::enable_if<
  std::is_floating_point<typename std::iterator_traits<It>::value_type>::value,
  typename std::iterator_traits<It>::value_type>::type
populationVariance(It first, It last)
{
  using T = typename std::iterator_traits<It>::value_type;

  auto n = std::distance(first, last);
  if (n <= 1) return consts::one<T>;

  T _mean = mean(first, last);
  T sum{};
  T ep{};
  T aux{};

  while (first != last) {
    aux = *first++ - _mean;
    ep += aux;
    sum += aux * aux;
  }

  return (sum - ep * ep / n) / n;
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
typename std::enable_if<
  std::is_integral<typename std::iterator_traits<It>::value_type>::value,
  double>::type
standarDeviation(It first, It last)
{
  return sqrt(variance(first, last));
}

template<typename It> inline
typename std::enable_if<
  std::is_floating_point<typename std::iterator_traits<It>::value_type>::value,
  typename std::iterator_traits<It>::value_type>::type
standarDeviation(It first, It last)
{
  return sqrt(variance(first, last));
}


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
 * \brief Rango o recorrido intercuartilico
 * El RI es la diferencia entre el tercer quartil (percentil 75) y el 
 * primer cuartil (percentil 25)
 * \f[ RI=Q_3-Q_1 \f]
 * \param[in] first Iterador al inicio
 * \param[in] last Iterador al final
 * \return Valor del rango para el conjunto de datos
 */
template<typename It> inline
typename std::enable_if<
  std::is_integral<typename std::iterator_traits<It>::value_type>::value,
  double>::type
interquartileRange(It first, It last)
{
  double q1 = tl::math::quantile(first, last, 0.25);
  double q3 = tl::math::quantile(first, last, 0.75);

  return q3-q1;
}

template<typename It> inline
typename std::enable_if<
  std::is_floating_point<typename std::iterator_traits<It>::value_type>::value,
  typename std::iterator_traits<It>::value_type>::type
interquartileRange(It first, It last)
{
  using T = typename std::iterator_traits<It>::value_type;

  T q1 = tl::math::quantile(first, last, 0.25);
  T q3 = tl::math::quantile(first, last, 0.75);

  return q3 - q1;
}


/*!
 * \brief Mean Absolute Deviation
 * \param[in] first Iterador al inicio
 * \param[in] last Iterador al final
 */
template<typename It> inline
typename std::enable_if<
  std::is_integral<typename std::iterator_traits<It>::value_type>::value,
  double>::type
meanAbsoluteDeviation(It first, It last)
{
  size_t n = std::distance(first, last);
  if (n <= 1) return consts::zero<double>;

  double _mean = mean(first, last);
  double sum{};

  while (first != last) {
    sum += std::abs(static_cast<double>(*first++) - _mean);
  }
  return sum / n;
}

template<typename It> inline
typename std::enable_if<
  std::is_floating_point<typename std::iterator_traits<It>::value_type>::value,
  typename std::iterator_traits<It>::value_type>::type
meanAbsoluteDeviation(It first, It last)
{
  using T = typename std::iterator_traits<It>::value_type;

  size_t n = std::distance(first, last);
  if (n <= 1) return consts::zero<T>;

  T _mean = mean(first, last);
  T sum{};

  while (first != last) {
    sum += std::abs(*first++ - _mean);
  }
  return sum / n;
}


/*!
 * \brief Median Absolute Deviation
 * \param[in] first Iterador al inicio
 * \param[in] last Iterador al final
 */
template<typename It> inline
typename std::enable_if<
  std::is_integral<typename std::iterator_traits<It>::value_type>::value,
  double>::type
medianAbsoluteDeviation(It first, It last)
{
  size_t n = std::distance(first, last);
  if (n <= 1) return consts::zero<double>;

  double _median = median(first, last);

  std::vector<double> x(n);
  auto x_it = x.begin();

  while (first != last) {
    *x_it++ = std::abs(static_cast<double>(*first++) - _median);
  }

  return median(x.begin(), x.end());
}

template<typename It> inline
typename std::enable_if<
  std::is_floating_point<typename std::iterator_traits<It>::value_type>::value,
  typename std::iterator_traits<It>::value_type>::type
medianAbsoluteDeviation(It first, It last)
{
  using T = typename std::iterator_traits<It>::value_type;

  size_t n = std::distance(first, last);
  if (n <= 1) return consts::zero<T>;

  T _median = median(first, last);
  T sum{};

  std::vector<T> x(n);
  auto x_it = x.begin();


  while (first != last) {
    *x_it++ = std::abs(*first++ - _median);
  }

  return median(x.begin(), x.end());
}

template<typename It> inline
double sumOfSquares(It first, It last)
{
  double _mean = mean(first, last);
  double sum{};

  while (first != last) {
    double dif = static_cast<double>(*first++) - _mean;
    sum += dif * dif;
  }

  return sum;
}

template<typename It> inline
double rootMeanSquare(It first, It last)
{
  double sum{};
  double i{1};
  double x{};

  while (first != last) {
    x = *first++;
    sum += (x * x - sum) / i++;
  }

  return std::sqrt(sum);
}


/*!
 * \brief Coefficient of variation (CV) or Relative Standard Deviation (RSD)
 * \f[ C_V = \frac{\sigma}{|\bar{x}|} \f]
 * \param[in] first Iterador al inicio
 * \param[in] last Iterador al final
 * \return Coeficiente de variación para el conjunto de datos
 */
template<typename It> inline
typename std::enable_if<
  std::is_integral<typename std::iterator_traits<It>::value_type>::value,
  double>::type
coefficientOfVariation(It first, It last)
{
  return standarDeviation(first, last) / std::abs(mean(first, last));
}

template<typename It> inline
typename std::enable_if<
  std::is_floating_point<typename std::iterator_traits<It>::value_type>::value,
  typename std::iterator_traits<It>::value_type>::type
coefficientOfVariation(It first, It last)
{
  return standarDeviation(first, last) / std::abs(mean(first, last));
}


/*!
 * \brief Quartile coefficient of dispersion
 * \f[ \frac{Q_3-Q_1}{Q_3+Q_1} \f]
 * \param[in] first Iterador al inicio
 * \param[in] last Iterador al final
 */
template<typename It> inline
typename std::enable_if<
  std::is_integral<typename std::iterator_traits<It>::value_type>::value,
  double>::type
quartileCoefficientOfDispersion(It first, It last)
{
  double q1 = quantile(first, last, 0.25);
  double q3 = quantile(first, last, 0.75);

  return (q3 - q1) / (q3 + q1);
}

template<typename It> inline
typename std::enable_if<
  std::is_floating_point<typename std::iterator_traits<It>::value_type>::value,
  typename std::iterator_traits<It>::value_type>::type
quartileCoefficientOfDispersion(It first, It last)
{
  using T = typename std::iterator_traits<It>::value_type;

  T q1 = quantile(first, last, 0.25);
  T q3 = quantile(first, last, 0.75);

  return (q3 - q1) / (q3 + q1);
}


/*!
 * \brief Quartile Deviation
 * \f[ \frac{Q_3-Q_1}{2} \f]
 * \param[in] first Iterador al inicio
 * \param[in] last Iterador al final
 */
template<typename It> inline
typename std::enable_if<
  std::is_integral<typename std::iterator_traits<It>::value_type>::value,
  double>::type
quartileDeviation(It first, It last)
{
  double q1 = quantile(first, last, 0.25);
  double q3 = quantile(first, last, 0.75);

  return (q3 - q1) / 2.;
}

template<typename It> inline
typename std::enable_if<
  std::is_floating_point<typename std::iterator_traits<It>::value_type>::value,
  typename std::iterator_traits<It>::value_type>::type
quartileDeviation(It first, It last)
{
  using T = typename std::iterator_traits<It>::value_type;

  T q1 = quantile(first, last, 0.25);
  T q3 = quantile(first, last, 0.75);

  return (q3 - q1) / consts::two<T>;
}


/*!
 * \brief Biweight Midvariance
 * \param[in] first Iterador al inicio
 * \param[in] last Iterador al final
 */
template<typename It> inline
typename std::enable_if<
  std::is_integral<typename std::iterator_traits<It>::value_type>::value,
  double>::type
biweightMidvariance(It first, It last)
{
  size_t n = std::distance(first, last);
  if (n <= 2) return consts::zero<double>;

  double _median = median(first, last);
  double mad = medianAbsoluteDeviation(first, last);

  double num{};
  double den{};
  while (first != last) {
    double x = *first++ - _median;
    double u = x / (9 * mad);
    if (std::abs(u) < consts::one<double>) {
      double u2 = u * u;
      double y = consts::one<double> -u2;
      double y2 = y * y;
      num += x * x * y2 * y2;
      den += y * (consts::one<double> -static_cast<double>(5) * u2);
    }
  }

  if (den == consts::zero<double>)
    return consts::zero<double>;

  return n * num / (den * den);
}

template<typename It> inline
typename std::enable_if<
  std::is_floating_point<typename std::iterator_traits<It>::value_type>::value,
  typename std::iterator_traits<It>::value_type>::type
biweightMidvariance(It first, It last)
{
  using T = typename std::iterator_traits<It>::value_type;

  size_t n = std::distance(first, last);
  if (n <= 2) return consts::zero<T>;

  T _median = median(first, last);
  T mad = medianAbsoluteDeviation(first, last);

  T num{};
  T den{};
  while (first != last) {
    T x = *first++ - _median;
    T u = x / (9 * mad);
    if (std::abs(u) < consts::one<T>) {
      T u2 = u * u;
      T y = consts::one<T> -u2;
      T y2 = y * y;
      num += x * x * y2 * y2;
      den += y * (consts::one<T> -static_cast<T>(5) * u2);
    }
  }

  if (den == consts::zero<T>)
    return consts::zero<T>;

  return n * num / (den * den);
}


/*! \} */ // end of Dispersion



/*!
 * \brief covariance
 * \f[ \S_{xy} = \frac{\sum_{i=1}^n (x_i - \overline{x})(y_i - \overline{y})}{n}  \f]
 * \param first
 * \param last
 * \return
 */
template<typename It> inline
typename std::enable_if<
  std::is_integral<typename std::iterator_traits<It>::value_type>::value,
  double>::type
covariance(It first_x, It last_x, It first_y, It last_y)
{
  auto n_x = std::distance(first_x, last_x);
  auto n_y = std::distance(first_y, last_y);
  if (n_x != n_y || n_x <= 1) return consts::zero<double>;

  double mean_x = mean(first_x, last_x);
  double mean_y = mean(first_y, last_y);
  double sum{};
  double x{};
  double y{};

  while (first_x != last_x) {
    x = *first_x++ - mean_x;
    y = *first_y++ - mean_y;
    sum += x*y;
  }

  return sum / n_x;
}

template<typename It> inline
typename std::enable_if<
  std::is_floating_point<typename std::iterator_traits<It>::value_type>::value,
  typename std::iterator_traits<It>::value_type>::type
covariance(It first_x, It last_x, It first_y, It last_y)
{
  using T = typename std::iterator_traits<It>::value_type;

  auto n_x = std::distance(first_x, last_x);
  auto n_y = std::distance(first_y, last_y);
  if (n_x != n_y || n_x <= 1) return consts::zero<T>;

  T mean_x = mean(first_x, last_x);
  T mean_y = mean(first_y, last_y);
  T sum{};
  T x{};
  T y{};

  while (first_x != last_x) {
    x = *first_x++ - mean_x;
    y = *first_y++ - mean_y;
    sum += x * y;
  }

  return sum / n_x;
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
typename std::enable_if<
  std::is_integral<typename std::iterator_traits<It>::value_type>::value,
  double>::type
pearsonCorrelationCoefficient(It first_x, It last_x,
                              It first_y, It last_y)
{
  auto n_x = std::distance(first_x, last_x);
  auto n_y = std::distance(first_y, last_y);
  if (n_x != n_y || n_x <= 1) return consts::zero<double>;
  return covariance(first_x, last_x, first_y, last_y) /
         (standarDeviation(first_x, last_x)*standarDeviation(first_y, last_y));
}

template<typename It> inline
typename std::enable_if<
  std::is_floating_point<typename std::iterator_traits<It>::value_type>::value,
  typename std::iterator_traits<It>::value_type>::type
pearsonCorrelationCoefficient(It first_x, It last_x,
                              It first_y, It last_y)
{
  using T = typename std::iterator_traits<It>::value_type;

  auto n_x = std::distance(first_x, last_x);
  auto n_y = std::distance(first_y, last_y);
  if (n_x != n_y || n_x <= 1) return consts::zero<T>;
  return covariance(first_x, last_x, first_y, last_y) /
         (standarDeviation(first_x, last_x) * standarDeviation(first_y, last_y));
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


template<typename itIn, typename itOut> inline
void zScore(itIn in_first, itIn in_last, itOut out_first)
{
  double _mean = mean(in_first, in_last);
  double standar_deviation =  standarDeviation(in_first, in_last);

  while (in_first != in_last) {
    *out_first++ = (*in_first++ - _mean) / standar_deviation;
  }
}






/*! \} */ // end of statistic

/*! \} */ // end of math

} // End namespace math

} // End namespace tl

#endif // TL_MATH_STATISTICS_H
