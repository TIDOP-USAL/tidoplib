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

#include "tidop/core/defs.h"

namespace tl
{

/*! \addtogroup math
 *  \{
 */


/*! \addtogroup statistics Statistics
 *  \{
 */

/*! \addtogroup CentralTendency Measures of central tendency
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

    while (first != last) {
        x += (static_cast<double>(*first++) - x) / i++;
    }

    return x;
}

template<typename It> inline
typename std::enable_if<
    std::is_floating_point<typename std::iterator_traits<It>::value_type>::value,
    typename std::remove_cv<typename std::iterator_traits<It>::value_type>::type>::type
mean(It first, It last)
{
    using T = typename std::remove_cv<typename std::iterator_traits<It>::value_type>::type;

    T x{0};
    T i{1};

    while (first != last) {
        x += (*first++ - x) / i++;
    }

    return x;
} 
 


/*! \} */ // end of CentralTendency

/*! \} */ // end of statistic

/*! \} */ // end of math

} // End namespace tl
