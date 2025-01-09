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

#include <iterator>

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



/*!
 * \brief Returns the number of arguments passed to the function.
 *
 * This function computes the number of arguments passed to it at compile time.
 * It uses variadic templates and the sizeof... operator to determine the count of arguments.
 *
 * \tparam Args Types of the function arguments.
 *
 * \param[in] args A variadic list of arguments.
 *
 * \return The number of arguments passed to the function.
 *
 * \example
 * size_t count = args_size(1, 2, 3);
 * // count = 3
 */
template<typename ...Args>
constexpr size_t args_size(Args&&...)
{
    return sizeof...(Args);
}

/*!
 * \brief Checks if no arguments are passed to the function.
 *
 * This function checks at compile time whether any arguments were passed to the function.
 * It uses variadic templates and the sizeof... operator to determine if the argument list is empty.
 *
 * \tparam Args Types of the function arguments.
 *
 * \param[in] args A variadic list of arguments.
 *
 * \return `true` if no arguments were passed, `false` otherwise.
 *
 * \example
 * bool isEmpty = args_empty();
 * // isEmpty = true
 *
 * bool isEmpty2 = args_empty(1, 2);
 * // isEmpty2 = false
 */
template<typename ...Args>
constexpr size_t args_empty(Args&&...)
{
    return sizeof...(Args) == 0;
}

/*!
 * \brief A function to avoid warnings for unused parameters.
 *
 * \tparam T The types of parameters that are not used.
 * \param ...args The unused parameters.
 */
template <typename... T>
constexpr void unusedParameter(const T&...)
{
}

/*!
 * \brief Checks if a value is within a specified range.
 * \param[in] value The value to be checked.
 * \param[in] min The minimum value of the range.
 * \param[in] max The maximum value of the range.
 * \return True if the value is within the range defined by min and max, false otherwise.
 */
template <typename T>
auto isInRange(T value, T min, T max) -> bool
{
    return (value < max) && (value > min);
}

/*!
 * \brief Checks if an object is of a specified type or is derived from it.
 *
 * This function checks whether the object pointed to by `ptr` is an instance of the `Base` type or is derived from it.
 * It uses `dynamic_cast` to safely determine if the pointer can be cast to a pointer of type `Base`. The function returns `true`
 * if the cast is successful (i.e., if `ptr` is an instance of `Base` or a subclass thereof), and `false` otherwise.
 *
 * \tparam Base The base type to check against.
 * \tparam T The type of the object being checked.
 *
 * \param[in] ptr A pointer to the object of type `T` to check.
 *
 * \return `true` if `ptr` is an instance of `Base` or is derived from `Base`, `false` otherwise.
 *
 * \example
 * class Animal { ... };
 * class Dog : public Animal { ... };
 * Dog dog;
 * bool result = instanceof<Animal>(&dog);
 * // result = true, because Dog is derived from Animal.
 */
template<typename Base, typename T>
bool instanceof(const T *ptr)
{
    return dynamic_cast<const Base *>(ptr) != nullptr;
}

}