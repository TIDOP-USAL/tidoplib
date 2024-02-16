﻿/**************************************************************************
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

    if (first != last) {
        auto result = std::minmax_element(first, last);
        _r = *result.second - *result.first;
    }

    return _r;
}


/*! \} */ // end of statistic

/*! \} */ // end of math

} // End namespace tl
