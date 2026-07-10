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

#include "tidop/core/base/Concepts.h"

namespace tl
{

/*! \addtogroup Units
 *  \{
 */

 /* Lenght */

 /*!
  * \brief Factor to convert feet to inches.
  *
  * This variable template provides the multiplicative factor required to
  * convert a length expressed in feet to inches (1 ft = 12 in).
  *
  * \tparam T Floating-point type used for the conversion factor.
  */
template<Floating T>
constexpr T foot_to_inches = static_cast<T>(12);

/*!
 * \brief Factor to convert inches to feet.
 *
 * This variable template provides the multiplicative factor required to
 * convert a length expressed in inches to feet (1 in = 1/12 ft).
 *
 * \tparam T Floating-point type used for the conversion factor.
 */
template<Floating T>
constexpr T inch_to_feet = T{1} / foot_to_inches<T>;

/*!
 * \brief Factor to convert yards to feet.
 *
 * This variable template provides the multiplicative factor required to
 * convert a length expressed in yards to feet (1 yd = 3 ft).
 *
 * \tparam T Floating-point type used for the conversion factor.
 */
template<Floating T>
constexpr T yard_to_feet = static_cast<T>(3);

/*!
 * \brief Factor to convert feet to yards.
 *
 * This variable template provides the multiplicative factor required to
 * convert a length expressed in feet to yards (1 ft = 1/3 yd).
 *
 * \tparam T Floating-point type used for the conversion factor.
 */
template<Floating T>
constexpr T foot_to_yards = T{1} / yard_to_feet<T>;

/*!
 * \brief Factor to convert chains to feet.
 *
 * This variable template provides the multiplicative factor required to
 * convert a length expressed in chains to feet (1 chain = 66 ft).
 *
 * \tparam T Floating-point type used for the conversion factor.
 */
template<Floating T>
constexpr T chain_to_feet = T{66};

/*!
 * \brief Factor to convert feet to chains.
 *
 * This variable template provides the multiplicative factor required to
 * convert a length expressed in feet to chains (1 chain = 66 ft → 1 ft = 1/66 chain).
 *
 * \tparam T Floating-point type used for the conversion factor.
 */
template<Floating T>
constexpr T foot_to_chains = T{1} / chain_to_feet<T>;

/*!
 * \brief Factor to convert furlongs to feet.
 *
 * This variable template provides the multiplicative factor required to
 * convert a length expressed in furlongs to feet (1 furlong = 660 ft).
 *
 * \tparam T Floating-point type used for the conversion factor.
 */
template<Floating T>
constexpr T furlong_to_feet = T{660};

/*!
 * \brief Factor to convert feet to furlongs.
 *
 * This variable template provides the multiplicative factor required to
 * convert a length expressed in feet to furlongs (1 furlong = 660 ft → 1 ft = 1/660 furlong).
 *
 * \tparam T Floating-point type used for the conversion factor.
 */
template<Floating T>
constexpr T foot_to_furlongs = T{1} / furlong_to_feet<T>;

/*!
 * \brief Factor to convert miles to feet.
 *
 * This variable template provides the multiplicative factor required to
 * convert a length expressed in miles to feet (1 mile = 5280 ft).
 *
 * \tparam T Floating-point type used for the conversion factor.
 */
 template<Floating T>
 constexpr T mile_to_feet = T{5280};

/*!
 * \brief Factor to convert feet to miles.
 *
 * This variable template provides the multiplicative factor required to
 * convert a length expressed in feet to miles (1 mile = 5280 ft → 1 ft = 1/5280 mi).
 *
 * \tparam T Floating-point type used for the conversion factor.
 */
template<Floating T>
constexpr T foot_to_mile = T{1} / mile_to_feet<T>;


/* Mass */

/*!
 * \brief Factor to convert pounds to ounces.
 *
 * This variable template provides the multiplicative factor required to
 * convert a mass expressed in pounds to ounces (1 lb = 16 oz).
 *
 * \tparam T Floating-point type used for the conversion factor.
 */
template<Floating T>
constexpr T pound_to_ounces = T{16};

/*!
 * \brief Factor to convert ounces to pounds.
 *
 * This variable template provides the multiplicative factor required to
 * convert a mass expressed in ounces to pounds (1 oz = 1/16 lb).
 *
 * \tparam T Floating-point type used for the conversion factor.
 */
template<Floating T>
constexpr T ounce_to_pounds = T{1} / pound_to_ounces<T>;

/*!
 * \brief Factor to convert pounds to grains.
 *
 * This variable template provides the multiplicative factor required to
 * convert a mass expressed in pounds to grains (1 lb = 7000 gr).
 *
 * \tparam T Floating-point type used for the conversion factor.
 */
template<Floating T>
constexpr T pound_to_grains = T{7000};

/*!
 * \brief Factor to convert grains to pounds.
 *
 * This variable template provides the multiplicative factor required to
 * convert a mass expressed in grains to pounds (1 gr = 1/7000 lb).
 *
 * \tparam T Floating-point type used for the conversion factor.
 */
template<Floating T>
constexpr T grain_to_pounds = T{1} / pound_to_grains<T>;

/*!
 * \brief Factor to convert stones to pounds.
 *
 * This variable template provides the multiplicative factor required to
 * convert a mass expressed in stones to pounds (1 st = 14 lb).
 *
 * \tparam T Floating-point type used for the conversion factor.
 */
template<Floating T>
constexpr T stone_to_pounds = T{14};

/*!
 * \brief Factor to convert pounds to stones.
 *
 * This variable template provides the multiplicative factor required to
 * convert a mass expressed in pounds to stones (1 stone = 14 lb → 1 lb = 1/14 st).
 *
 * \tparam T Floating-point type used for the conversion factor.
 */
template<Floating T>
constexpr T pound_to_stone = T{1} / stone_to_pounds<T>;


/*!
 * \brief Factor to convert quarters to pounds.
 *
 * This variable template provides the multiplicative factor required to
 * convert a mass expressed in quarters to pounds (1 qr = 28 lb).
 *
 * \tparam T Floating-point type used for the conversion factor.
 */
template<Floating T>
constexpr T quarter_to_pounds = T{28};

/*!
 * \brief Factor to convert pounds to quarters.
 *
 * This variable template provides the multiplicative factor required to
 * convert a mass expressed in pounds to quarters (1 quarter = 28 lb → 1 lb = 1/28 qr).
 *
 * \tparam T Floating-point type used for the conversion factor.
 */
template<Floating T>
constexpr T pound_to_quarter = T{1} / quarter_to_pounds<T>;

/*!
 * \brief Factor to convert hundredweights to pounds.
 *
 * This variable template provides the multiplicative factor required to
 * convert a mass expressed in hundredweights to pounds (1 cwt = 112 lb).
 *
 * \tparam T Floating-point type used for the conversion factor.
 */
template<Floating T>
constexpr T hundredweight_to_pounds = T{112};

/*!
 * \brief Factor to convert pounds to hundredweights (long hundredweight).
 *
 * This variable template provides the multiplicative factor required to
 * convert a mass expressed in pounds to hundredweights (1 cwt = 112 lb → 1 lb = 1/112 cwt).
 *
 * \tparam T Floating-point type used for the conversion factor.
 */
template<Floating T>
constexpr T pound_to_hundredweights = T{1} / hundredweight_to_pounds<T>;

/*!
 * \brief Factor to convert tons to pounds (long ton).
 *
 * This variable template provides the multiplicative factor required to
 * convert a mass expressed in long tons to pounds (1 ton = 2240 lb).
 *
 * \tparam T Floating-point type used for the conversion factor.
 */
template<Floating T>
constexpr T ton_to_pounds = T{2240};

/*!
 * \brief Factor to convert pounds to tons (long ton).
 *
 * This variable template provides the multiplicative factor required to
 * convert a mass expressed in pounds to long tons (1 ton = 2240 lb → 1 lb = 1/2240 t).
 *
 * \tparam T Floating-point type used for the conversion factor.
 */
template<Floating T>
constexpr T pound_to_ton = T{1} / ton_to_pounds<T>;


/*! \} */


}
