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

#include <concepts>
#include <ranges>

namespace tl
{

/*!
 * \addtogroup meta
 * \{
 */

template<typename T>
concept Integral = std::integral<T>;

template<typename T>
concept SignedIntegral = std::signed_integral<T>;

template<typename T>
concept UnsignedIntegral = std::unsigned_integral<T>;

template<typename T>
concept Floating = std::floating_point<T>;

template<typename T>
concept Arithmetic = Integral<T> || Floating<T>;

template<typename T>
concept SignedArithmetic = SignedIntegral<T> ||
                           Floating<T>;

template <typename T>
concept ArithmeticNoBool = Arithmetic<T> && !std::same_as<T, bool>;


template<typename R>
concept NumericRange = std::ranges::input_range<R> &&
                       Arithmetic<std::ranges::range_value_t<R>>;

//template<typename R>
//concept FloatingRange = std::ranges::input_range<R> &&
//                        Floating<std::ranges::range_value_t<R>>;

template<typename R>
concept ContiguousNumericRange = NumericRange<R> &&
                                 std::ranges::contiguous_range<R>;

template<typename R>
concept SizedNumericRange = NumericRange<R> &&
                            std::ranges::sized_range<R>;

template<typename R>
concept SimdRange = ContiguousNumericRange<R> &&
                    SizedNumericRange<R>;

//template<typename R>
//concept RandomAccessNumericRange = NumericRange<R> &&
//                                   std::ranges::random_access_range<R>;

/*! \} */

}