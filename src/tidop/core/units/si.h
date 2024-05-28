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

#include "tidop/core/common.h"

namespace tl
{

namespace si
{

constexpr double kilo = 1000.;
constexpr double hecto = 100.;
constexpr double deca = 10.;
constexpr double metre = 1.;
constexpr double deci = 0.1;
constexpr double centi = 0.01;
constexpr double milli = 0.001;
constexpr double micro = 0.000001;
constexpr double nano = 0.000000001;


constexpr double square_metre = 1.;
constexpr double square_decametre = 100.;
constexpr double square_hectometre = 10000.;
constexpr double square_kilometre = 1.e6;

}

template<typename T>
constexpr enableIfFloating<T,T> to_kilo = static_cast<T>(si::metre / si::kilo);
template<typename T>
constexpr enableIfFloating<T,T> from_kilo = static_cast<T>(si::kilo);
template<typename T>
constexpr enableIfFloating<T,T> to_hecto = static_cast<T>(si::metre / si::hecto);
template<typename T>
constexpr enableIfFloating<T,T> from_hecto = static_cast<T>(si::hecto);
template<typename T>
constexpr enableIfFloating<T,T> to_deca = static_cast<T>(si::metre / si::deca);
template<typename T>
constexpr enableIfFloating<T,T> from_deca = static_cast<T>(si::deca);
template<typename T>
constexpr enableIfFloating<T,T> to_deci = static_cast<T>(si::metre / si::deci);
template<typename T>
constexpr enableIfFloating<T,T> from_deci = static_cast<T>(si::deci);
template<typename T>
constexpr enableIfFloating<T,T> to_centi = static_cast<T>(si::metre / si::centi);
template<typename T>
constexpr enableIfFloating<T,T> from_centi = static_cast<T>(si::centi);
template<typename T>
constexpr enableIfFloating<T,T> to_milli = static_cast<T>(si::metre / si::milli);
template<typename T>
constexpr enableIfFloating<T,T> from_milli = static_cast<T>(si::milli);
template<typename T>
constexpr enableIfFloating<T,T> to_micro = static_cast<T>(si::metre / si::micro);
template<typename T>
constexpr enableIfFloating<T,T> from_micro = static_cast<T>(si::micro);
template<typename T>
constexpr enableIfFloating<T,T> to_nano = static_cast<T>(1.e+9);
template<typename T>
constexpr enableIfFloating<T,T> from_nano = static_cast<T>(si::nano);

}
