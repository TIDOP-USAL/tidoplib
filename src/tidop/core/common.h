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

#include <tidop/config.h>

namespace tl
{
	
template<typename T, class R = void>
using enableIfIntegral = std::enable_if_t<std::is_integral<T>::value, R>;

template<typename T, class R = void>
using enableIfFloating = std::enable_if_t<std::is_floating_point<T>::value, R>;

template<typename T, class R = void>
using enableIfArithmetic = std::enable_if_t<std::is_arithmetic<T>::value, R>;

template<typename T, class R = void>
using enableIfFloat = std::enable_if_t<std::is_same<float, std::remove_cv_t<T>>::value, R>;

template<typename T, class R = void>
using enableIfDouble = std::enable_if_t<std::is_same<double, std::remove_cv_t<T>>::value, R>;

template<typename T, class R = void>
using enableIfBool = std::enable_if_t<std::is_same<bool, std::remove_cv_t<T>>::value, R>;


template<typename T, class R = void>
using enableIfNotIntegral = std::enable_if_t<!std::is_integral<T>::value, R>;

template <typename T, class R = void>
using enableIfNotFloating = std::enable_if_t<!std::is_floating_point<T>::value, R>;

template<typename T, class R = void>
using enableIfNotArithmetic = std::enable_if_t<!std::is_arithmetic<T>::value, R>;


template<typename It>
using iteratorValueType = typename std::iterator_traits<It>::value_type;

}