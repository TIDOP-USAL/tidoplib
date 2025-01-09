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

#include <type_traits>

#include "tidop/core/base/common.h"

namespace tl
{

/*! \addtogroup Units
 *  \{
 */

/* Lenght */

template<typename T>
constexpr enableIfFloating<T, T> foot_to_inches = static_cast<T>(12); /*!< Factor to convert 1 foot to inches. */
template<typename T>
constexpr enableIfFloating<T, T> inch_to_feet = static_cast<T>(1. / 12.); /*!< Factor to convert 1 inch to feet. */
template<typename T>
constexpr enableIfFloating<T, T> foot_to_yards = static_cast<T>(1. / 3.); /*!< Factor to convert 1 foot to yards. */
template<typename T>
constexpr enableIfFloating<T, T> yard_to_feet = static_cast<T>(3); /*!< Factor to convert 1 yard to feet. */
template<typename T>
constexpr enableIfFloating<T, T> foot_to_chains = static_cast<T>(1. / 66.);  /*!< Factor to convert 1 foot to chains. */
template<typename T>
constexpr enableIfFloating<T, T> chain_to_feet = static_cast<T>(66); /*!< Factor to convert 1 chain to feet. */
template<typename T>
constexpr enableIfFloating<T, T> foot_to_furlongs = static_cast<T>(1. / 660.); /*!< Factor to convert 1 foot to furlongs. */
template<typename T>
constexpr enableIfFloating<T, T> furlong_to_feet = static_cast<T>(660); /*!< Factor to convert 1 furlong to feet. */
template<typename T>
constexpr enableIfFloating<T, T> foot_to_mile = static_cast<T>(1. / 5280.); /*!< Factor to convert 1 foot to miles. */
template<typename T>
constexpr enableIfFloating<T, T> mile_to_feet = static_cast<T>(5280); /*!< Factor to convert 1 mile to feet. */


/* Mass */

template<typename T>
constexpr enableIfFloating<T, T> pound_to_ounces = static_cast<T>(16); /*!< Factor to convert 1 pound to ounces. */
template<typename T>
constexpr enableIfFloating<T, T> ounce_to_pounds = static_cast<T>(1. / 16.); /*!< Factor to convert 1 ounce to pounds. */
template<typename T>
constexpr enableIfFloating<T, T> pound_to_grains = static_cast<T>(7000); /*!< Factor to convert 1 pound to grains. */
template<typename T>
constexpr enableIfFloating<T, T> grain_to_pounds = static_cast<T>(1. / 7000.); /*!< Factor to convert 1 grain to pounds. */
template<typename T>
constexpr enableIfFloating<T, T> pound_to_stone = static_cast<T>(1. / 14.); /*!< Factor to convert 1 pound to stones. */
template<typename T>
constexpr enableIfFloating<T, T> stone_to_pounds = static_cast<T>(14); /*!< Factor to convert 1 stone to pounds. */
template<typename T>
constexpr enableIfFloating<T, T> pound_to_quarter = static_cast<T>(1. / 28.); /*!< Factor to convert 1 pound to quarters. */
template<typename T>
constexpr enableIfFloating<T, T> quarter_to_pounds = static_cast<T>(28); /*!< Factor to convert 1 quarter to pounds. */
template<typename T>
constexpr enableIfFloating<T, T> pound_to_hundredweights = static_cast<T>(1. / 112.); /*!< Factor to convert 1 pound to hundredweights. */
template<typename T>
constexpr enableIfFloating<T, T> hundredweight_to_pounds = static_cast<T>(112); /*!< Factor to convert 1 hundredweight to pounds. */
template<typename T>
constexpr enableIfFloating<T, T> pound_to_ton = static_cast<T>(1. / 2240.); /*!< Factor to convert 1 pound to tons. */
template<typename T>
constexpr enableIfFloating<T, T> ton_to_pounds = static_cast<T>(2240); /*!< Factor to convert 1 ton to pounds. */


/* Area */

//perch – 25.29285264 square meters
//rood – 1011.7141056 square meters
//acre – 4046.8564224 square meters
//square mile - 2589988.110336 square meters
template<typename T>
constexpr T to_square_yard = static_cast<T>(1. / 0.836127); /*!< Factor to convert 1 square meter to square yards. */
template<typename T>
constexpr T from_square_yard = static_cast<T>(0.836127); /*!< Factor to convert 1 square yard to square meters. */
template<typename T>
constexpr T to_square_foot = static_cast<T>(1. / 0.092903); /*!< Factor to convert 1 square meter to square feet. */
template<typename T>
constexpr T from_square_foot = static_cast<T>(0.092903); /*!< Factor to convert 1 square foot to square meters. */


/*! \} */


}
