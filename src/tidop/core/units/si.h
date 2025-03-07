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

namespace si
{


constexpr double kilo = 1000.;          /*!< Factor to convert 1 kilometer to meters. */
constexpr double hecto = 100.;          /*!< Factor to convert 1 hectometer to meters. */
constexpr double deca = 10.;            /*!< Factor to convert 1 decameter to meters. */
constexpr double metre = 1.;            /*!< Base unit: 1 meter. */
constexpr double deci = 0.1;            /*!< Factor to convert 1 decimeter to meters. */
constexpr double centi = 0.01;          /*!< Factor to convert 1 centimeter to meters. */
constexpr double milli = 0.001;         /*!< Factor to convert 1 millimeter to meters. */
constexpr double micro = 0.000001;      /*!< Factor to convert 1 micrometer to meters. */
constexpr double nano = 0.000000001;    /*!< Factor to convert 1 nanometer to meters. */


constexpr double square_metre = 1.;           /*!< Base unit: 1 square meter. */
constexpr double square_decametre = 100.;     /*!< Factor to convert 1 square decameter to square meters. */
constexpr double square_hectometre = 10000.;  /*!< Factor to convert 1 square hectometer to square meters. */
constexpr double square_kilometre = 1.e6;     /*!< Factor to convert 1 square kilometer to square meters. */

}

template<typename T>
constexpr enableIfFloating<T, T> to_kilo = static_cast<T>(si::metre / si::kilo); /*!< Converts meters to kilometers. */
template<typename T>
constexpr enableIfFloating<T ,T> from_kilo = static_cast<T>(si::kilo); /*!< Converts kilometers to meters. */
template<typename T>
constexpr enableIfFloating<T, T> to_hecto = static_cast<T>(si::metre / si::hecto); /*!< Converts meters to hectometers. */
template<typename T>
constexpr enableIfFloating<T, T> from_hecto = static_cast<T>(si::hecto); /*!< Converts hectometers to meters. */
template<typename T>
constexpr enableIfFloating<T, T> to_deca = static_cast<T>(si::metre / si::deca); /*!< Converts meters to decameters. */
template<typename T>
constexpr enableIfFloating<T, T> from_deca = static_cast<T>(si::deca); /*!< Converts decameters to meters. */
template<typename T>
constexpr enableIfFloating<T, T> to_deci = static_cast<T>(si::metre / si::deci); /*!< Converts meters to decimeters. */
template<typename T>
constexpr enableIfFloating<T, T> from_deci = static_cast<T>(si::deci); /*!< Converts decimeters to meters. */
template<typename T>
constexpr enableIfFloating<T, T> to_centi = static_cast<T>(si::metre / si::centi); /*!< Converts meters to centimeters. */
template<typename T>
constexpr enableIfFloating<T, T> from_centi = static_cast<T>(si::centi); /*!< Converts centimeters to meters. */
template<typename T>
constexpr enableIfFloating<T, T> to_milli = static_cast<T>(si::metre / si::milli); /*!< Converts meters to millimeters. */
template<typename T>
constexpr enableIfFloating<T, T> from_milli = static_cast<T>(si::milli); /*!< Converts millimeters to meters. */
template<typename T>
constexpr enableIfFloating<T, T> to_micro = static_cast<T>(si::metre / si::micro); /*!< Converts meters to micrometers. */
template<typename T>
constexpr enableIfFloating<T, T> from_micro = static_cast<T>(si::micro); /*!< Converts micrometers to meters. */
template<typename T>
constexpr enableIfFloating<T, T> to_nano = static_cast<T>(1.e+9); /*!< Converts meters to nanometers. */
template<typename T>
constexpr enableIfFloating<T, T> from_nano = static_cast<T>(si::nano); /*!< Converts nanometers to meters. */

/* Area */

template<typename T>
constexpr enableIfFloating<T, T> to_square_deca = static_cast<T>(si::square_metre / si::square_decametre);
template<typename T>
constexpr enableIfFloating<T, T> from_square_deca = static_cast<T>(si::square_decametre);
template<typename T>
constexpr enableIfFloating<T, T> to_square_hecto = static_cast<T>(si::square_metre / si::square_hectometre);
template<typename T>
constexpr enableIfFloating<T, T> from_square_hecto = static_cast<T>(si::square_hectometre);
template<typename T>
constexpr enableIfFloating<T, T> to_square_kilo = static_cast<T>(si::square_metre / si::square_kilometre);
template<typename T>
constexpr enableIfFloating<T, T> from_square_kilo = static_cast<T>(si::square_kilometre);


/*! \} */

}
