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
 
/*!
 * \file meta.h
 * \brief Meta-programming utilities for compile-time type and argument introspection
 *
 * This module provides compile-time utilities for working with variadic templates,
 * type checking, and range validation. These functions leverage C++ template metaprogramming
 * to perform checks and operations at compile time when possible.
 *
 * ### Classes and Functions
 *
 * - \ref args_size - Count the number of variadic arguments at compile time
 * - \ref args_empty - Check if no arguments were passed
 * - \ref unusedParameter - Suppress compiler warnings for unused parameters
 * - \ref isInRange - Check if a value is within a specified numeric range
 * - \ref instanceof - Runtime type checking for polymorphic types (RTTI)
 *
 * ### Features
 *
 * - Compile-time argument counting with constexpr functions
 * - Type-safe range checking for comparable types
 * - Runtime polymorphic type checking via dynamic_cast
 * - Compiler warning suppression for intentionally unused parameters
 * - No runtime overhead for compile-time checks
 *
 * ### Example Usage
 *
 * \code{.cpp}
 * #include "tidop/core/base/meta.h"
 *
 * // Count arguments at compile time
 * size_t count = tl::args_size(1, 2, 3, 4, 5);  // count = 5
 *
 * // Check if list is empty
 * bool empty = tl::args_empty();                 // empty = true
 * bool notEmpty = tl::args_empty(10, 20);        // notEmpty = false
 *
 * // Suppress unused parameter warnings
 * void myFunction(int used, int unused) {
 *     tl::unusedParameter(unused);
 *     // Do something with 'used'
 * }
 *
 * // Range checking
 * int value = 50;
 * if (tl::isInRange(value, 0, 100)) {
 *     // value is between 0 and 100
 * }
 *
 * // Polymorphic type checking
 * class Animal { virtual ~Animal() = default; };
 * class Dog : public Animal { };
 * Dog* dog = new Dog();
 * if (tl::instanceof<Animal>(dog)) {
 *     // dog is an instance of Animal or derived class
 * }
 * \endcode
 *
 * \see tl::args_size, tl::args_empty, tl::instanceof
 */

#pragma once

#include <tidop/config.h>

#include <iterator>

namespace tl
{
	


/*!
 * \addtogroup meta
 * \{
 */

/*!
 * \brief Returns the number of arguments passed to the function.
 *
 * Computes the count of arguments passed to the function at compile time using
 * the `sizeof...` operator. This is a constexpr function with no runtime overhead.
 *
 * \tparam Args Types of the function arguments.
 *
 * \param[in] args A variadic list of arguments.
 *
 * \return The count of arguments passed, as a size_t compile-time constant.
 *
 * ### Example Usage
 * \code{.cpp}
 * size_t count1 = tl::args_size(1, 2, 3);
 * // count1 = 3 (compile-time constant)
 *
 * size_t count2 = tl::args_size("hello", 42, 3.14, true);
 * // count2 = 4
 *
 * size_t count3 = tl::args_size();
 * // count3 = 0
 * \endcode
 *
 * ### Use Cases
 *
 * - Creating compile-time constants based on argument count
 * - Variadic template instantiation control
 * - Type list size computation
 * - Array allocation sizing
 *
 * ### Notes
 *
 * - This is a constexpr function with zero runtime cost
 * - Can be used in constant expressions and template parameters
 * - Works with any number of arguments of any types
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
 * ### Example Usage
 * \code{.cpp}
 * bool isEmpty = args_empty();
 * // isEmpty = true
 *
 * bool isEmpty2 = args_empty(1, 2);
 * // isEmpty2 = false
 * \endcode
 */
template<typename ...Args>
constexpr bool args_empty(Args&&...)
{
    return sizeof...(Args) == 0;
}

/*!
 * \brief Suppresses compiler warnings for intentionally unused parameters.
 *
 * This utility function is used to explicitly indicate that function parameters
 * are intentionally not used, preventing compiler warnings (e.g., -Wunused-parameter
 * in GCC/Clang or C4100 in MSVC). It takes the unused parameters as arguments
 * and does nothing, effectively "using" them in a way the compiler understands.
 *
 * ### Use Cases
 *
 * - Interface implementations where not all parameters are needed
 * - Virtual functions that override base class signatures
 * - Callback functions with fixed signatures
 * - Template instantiations requiring specific parameter lists
 *
 * \tparam T Types of parameters that are not used.
 * \param[in] args The unused parameters to suppress warnings for.
 *
 * ### Example Usage
 * \code{.cpp}
 * // Override with unused parameter
 * void MyHandler::onEvent(const Event& event, void* context) override {
 *     tl::unusedParameter(context);  // context not needed in this handler
 *     // Process event...
 * }
 *
 * // Multiple unused parameters
 * void legacyFunction(int a, int b, int c) {
 *     tl::unusedParameter(b, c);  // Only use parameter 'a'
 *     std::cout << a << std::endl;
 * }
 * \endcode
 *
 * ### Notes
 *
 * - This is a constexpr function with no runtime overhead
 * - The compiler typically optimizes this away completely
 * - Preferred over casting to void for clarity
 *
 * \see args_size, args_empty
 */
template <typename... T>
constexpr void unusedParameter(const T&...)
{
}

/*!
 * \brief Checks if a value is within a specified range (inclusive).
 *
 * Determines whether the provided value falls within the range [min, max].
 * Both boundaries are included in the check.
 *
 * \tparam T Type of the values being compared. Must support operator< and operator>.
 *
 * \param[in] value The value to be checked.
 * \param[in] min The minimum value of the range (inclusive).
 * \param[in] max The maximum value of the range (inclusive).
 *
 * \return `true` if `min <= value <= max`, `false` otherwise.
 *
 * ### Example Usage
 * \code{.cpp}
 * int value = 50;
 * bool inRange = tl::isInRange(value, 0, 100);  // true
 *
 * double temp = 36.5;
 * bool isFever = tl::isInRange(temp, 37.0, 42.0);  // false
 * \endcode
 *
 * ### Notes
 *
 * - Type T must be comparable with operator< and operator>
 * - Both min and max boundaries are inclusive
 * - Returns false if min > max (invalid range)
 *
 * \see instanceof
 */
template <typename T>
auto isInRange(T value, T min, T max) -> bool
{
    return (value >= min) && (value <= max);
}

/*!
 * \brief Runtime type checking for polymorphic types.
 *
 * Determines whether an object pointed to by `ptr` is an instance of the `Base` type
 * or is derived from it. This function uses RTTI (Runtime Type Information) via
 * `dynamic_cast` to safely determine type relationships at runtime.
 *
 * ### When to Use
 *
 * - Checking if a pointer to a base class actually points to a derived type
 * - Safely casting polymorphic pointers before downcasting
 * - Implementing type-dependent behavior in polymorphic code
 * - Avoiding exceptions from failed dynamic_cast operations
 *
 * \tparam Base The base class type to check against.
 * \tparam T The type of the object being checked.
 * \param[in] ptr A pointer to the object to check. Can be a pointer to Base or derived type.
 * \return `true` if `ptr` points to an instance of `Base` or a type derived from `Base`,
 *         `false` otherwise. Also returns `false` if `ptr` is `nullptr`.
 *
 * ### Example Usage
 * \code{.cpp}
 * class Shape { public: virtual ~Shape() = default; };
 * class Circle : public Shape { };
 * class Rectangle : public Shape { };
 *
 * Circle circle;
 * Rectangle rectangle;
 *
 * // Check types
 * if (tl::instanceof<Circle>(&circle)) {
 *     std::cout << "circle is a Circle" << std::endl;  // Prints
 * }
 *
 * if (tl::instanceof<Circle>(&rectangle)) {
 *     std::cout << "rectangle is a Circle" << std::endl;  // Does not print
 * }
 *
 * if (tl::instanceof<Shape>(&circle)) {
 *     std::cout << "circle is a Shape" << std::endl;  // Prints
 * }
 *
 * // Safe without exception
 * Shape* ptr = nullptr;
 * if (tl::instanceof<Circle>(ptr)) {
 *     // Never executes, ptr is nullptr
 * }
 * \endcode
 */
template<typename Base, typename T>
bool instanceof(const T *ptr)
{
    return dynamic_cast<const Base *>(ptr) != nullptr;
}

/*! \} */

}