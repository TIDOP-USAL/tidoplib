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

#include <tidop/core/defs.h>
#include <tidop/core/messages.h>
#include <tidop/math/statistic/descriptive.h>

namespace tl
{

/*! \addtogroup math
 *  \{
 */


 /*! \defgroup statistics Statistics
  *  \{
  */

  /*!
   * /brief Covariance
   *
   */
template<typename T>
class Covariance
{

public:

    Covariance();
    ~Covariance();

    double eval(const Series<T> &data1,
                const Series<T> &data2);

};


/* Implementation */

template<typename T>
Covariance<T>::Covariance()
{
}

template<typename T>
Covariance<T>::~Covariance()
{
}

template<typename T> inline
double Covariance<T>::eval(const Series<T> &series1,
                           const Series<T> &series2)
{
    DescriptiveStatistics<T> stat1(series1);
    DescriptiveStatistics<T> stat2(series2);

    auto n_x = stat1.size();
    auto n_y = stat2.size();
    if (n_x != n_y || n_x <= 1) return consts::zero<double>;

    double mean_x = stat1.mean();
    double mean_y = stat2.mean();
    double sum{};
    double x{};
    double y{};

    auto it1 = series1.begin();
    auto it2 = series2.begin();
    while (it1 != series1.end()) {
        x = static_cast<double>(*it1++) - mean_x;
        y = static_cast<double>(*it2++) - mean_y;
        sum += x * y;
    }

    return sum / n_x;
}


/*! \} */ // end of statistic

/*! \} */ // end of math

} // End namespace tl
