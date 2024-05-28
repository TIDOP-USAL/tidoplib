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
 * \brief La moda es el valor de la variable que mas veces se repite, el que lleva asociada la mayor frecuencia absoluta.
 * \param[in] first Iterador al inicio
 * \param[in] last Iterador al final
 * \return Valor de la moda
 */
template<typename It>
auto mode(It first, It last) -> iteratorValueType<It>
{
    using T = std::remove_cv_t<iteratorValueType<It>>;

    std::map<T, int> h;
    while (first != last) {
        h[*first++]++;
    }

    auto max = std::max_element(h.begin(), h.end(),
                                [](const std::pair<T, int> &p1,
                                const std::pair<T, int> &p2) {
                                    return p1.second < p2.second;
                                });

    return max->first;
}

/*!
 * \brief La moda es el valor de la variable mas veces se repite, el que lleva asociada la mayor frecuencia absoluta.
 * \param[in] container Objeto contenedor
 * \return Valor de la moda
 */
template<typename T>
auto mode(const T& container) -> typename T::value_type
{
    return mode(container.begin(), container.end());
}



/*! \} */ // end of CentralTendency

/*! \} */ // end of statistic

/*! \} */ // end of math

} // End namespace tl
