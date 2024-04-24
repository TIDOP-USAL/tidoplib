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

namespace tl
{

/* Lenght */

template<typename T>
constexpr std::enable_if_t<std::is_floating_point<T>::value, T> foot_to_inches = static_cast<T>(12);
template<typename T>
constexpr std::enable_if_t<std::is_floating_point<T>::value, T> inch_to_feet = static_cast<T>(1./12.);
template<typename T>
constexpr std::enable_if_t<std::is_floating_point<T>::value, T> foot_to_yards = static_cast<T>(1./3.);
template<typename T>
constexpr std::enable_if_t<std::is_floating_point<T>::value, T> yard_to_feet = static_cast<T>(3);
template<typename T>
constexpr std::enable_if_t<std::is_floating_point<T>::value, T> foot_to_chains = static_cast<T>(1. / 66.);
template<typename T>
constexpr std::enable_if_t<std::is_floating_point<T>::value, T> chain_to_feet = static_cast<T>(66);
template<typename T>
constexpr std::enable_if_t<std::is_floating_point<T>::value, T> foot_to_furlongs = static_cast<T>(1. / 660.);
template<typename T>
constexpr std::enable_if_t<std::is_floating_point<T>::value, T> furlong_to_feet = static_cast<T>(660);
template<typename T>
constexpr std::enable_if_t<std::is_floating_point<T>::value, T> foot_to_mile = static_cast<T>(1. / 5280.);
template<typename T>
constexpr std::enable_if_t<std::is_floating_point<T>::value, T> mile_to_feet = static_cast<T>(5280);


/* Mass */

template<typename T>
constexpr std::enable_if_t<std::is_floating_point<T>::value, T> pound_to_ounces = static_cast<T>(16);
template<typename T>
constexpr std::enable_if_t<std::is_floating_point<T>::value, T> ounce_to_pounds = static_cast<T>(1. / 16.);
template<typename T>
constexpr std::enable_if_t<std::is_floating_point<T>::value, T> pound_to_grains = static_cast<T>(7000);
template<typename T>
constexpr std::enable_if_t<std::is_floating_point<T>::value, T> grain_to_pounds = static_cast<T>(1. / 7000.);
template<typename T>
constexpr std::enable_if_t<std::is_floating_point<T>::value, T> pound_to_stone = static_cast<T>(1. / 14.);
template<typename T>
constexpr std::enable_if_t<std::is_floating_point<T>::value, T> stone_to_pounds = static_cast<T>(14);
template<typename T>
constexpr std::enable_if_t<std::is_floating_point<T>::value, T> pound_to_quarter = static_cast<T>(1. / 28.);
template<typename T>
constexpr std::enable_if_t<std::is_floating_point<T>::value, T> quarter_to_pounds = static_cast<T>(28);
template<typename T>
constexpr std::enable_if_t<std::is_floating_point<T>::value, T> pound_to_hundredweights = static_cast<T>(1. / 112.);
template<typename T>
constexpr std::enable_if_t<std::is_floating_point<T>::value, T> hundredweight_to_pounds = static_cast<T>(112);
template<typename T>
constexpr std::enable_if_t<std::is_floating_point<T>::value, T> pound_to_ton = static_cast<T>(1. / 2240.);
template<typename T>
constexpr std::enable_if_t<std::is_floating_point<T>::value, T> ton_to_pounds = static_cast<T>(2240);


/* Area */

//perch – 25.29285264 square meters
//rood – 1011.7141056 square meters
//acre – 4046.8564224 square meters
//square mile - 2589988.110336 square meters
template<typename T>
constexpr T to_square_yard = static_cast<T>(1. / 0.836127);
template<typename T>
constexpr T from_square_yard = static_cast<T>(0.836127);
template<typename T>
constexpr T to_square_foot = static_cast<T>(1. / 0.092903);
template<typename T>
constexpr T from_square_foot = static_cast<T>(0.092903);

}
