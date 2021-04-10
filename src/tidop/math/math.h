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

#ifndef TL_CORE_MATH_H
#define TL_CORE_MATH_H

#include "config_tl.h"
#include "tidop/core/defs.h"

#include <cmath>

namespace tl
{

/*!
 * \defgroup Math Utilidades matemáticas
 *
 * Utilidades matematicas para operaciones entre vectores (tanto en el plano como en espacio),
 * funciones estadísticas, ajuste de nubes de puntos a diversas geometrias, resolución
 * de sistemas de ecuaciones lineales.
 *
 * \{
 */

namespace math
{

/* Definición de constantes de tipo general */

constexpr auto half_pi = 1.5707963267948966192313216916398;
constexpr auto pi = 3.1415926535897932384626433832795;
constexpr auto two_pi = 6.283185307179586476925286766559;


/* Conversión de ángulos */

constexpr auto rad_to_deg = 57.295779513082320876798154814105;
constexpr auto deg_to_rad = 0.01745329251994329576923690768489;
constexpr auto rad_to_grad = 63.661977236758134307553505349006;
constexpr auto grad_to_rad = 0.0157079632679489661923132169164;

} // Fin namespace math


/*! \} */ // end of Math

} // End namespace tl

#endif // TL_CORE_MATH_H