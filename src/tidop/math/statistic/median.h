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

#include "tidop/math/statistic/quantile.h"

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
    return tl::quantile(first, last, 0.5);
}

template<typename It> inline
typename std::enable_if<
    std::is_floating_point<typename std::iterator_traits<It>::value_type>::value,
    typename std::remove_cv<typename std::iterator_traits<It>::value_type>::type>::type
median(It first, It last)
{
    return tl::quantile(first, last, 0.5);
}


/*! \} */ // end of CentralTendency

/*! \} */ // end of statistic

/*! \} */ // end of math

} // End namespace tl
