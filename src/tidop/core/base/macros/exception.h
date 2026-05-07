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

#include "tidop/config.h"


/*! \addtogroup Macros
 *  \{
 */

/*!
 * \brief Declare a function as non-throwing.
 *
 * Specifies that a function does not throw exceptions. In C++11 and later,
 * uses the standard `noexcept` keyword. For older C++ standards, uses the
 * deprecated `throw()` exception specification.
 *
 * ### Usage Example
 *
 * \code{.cpp}
 * int getValue() TL_NOEXCEPT
 * {
 *     return 42;
 * }
 *
 * class MyClass
 * {
 * public:
 *     ~MyClass() TL_NOEXCEPT { }
 *     void safeMethod() TL_NOEXCEPT { }
 * };
 * \endcode
 *
 * ### Behavior
 *
 * - **C++11 and later:** Translates to `noexcept`
 *   - If an exception is thrown, calls `std::terminate()`
 *   - Enables compiler optimizations
 * - **Pre-C++11:** Translates to `throw()`
 *   - Legacy exception specification (deprecated in C++11)
 *
 * \note
 * - Always place at the end of the function signature
 * - Can be used in destructors, move operations, and swap functions
 * - Do not use if the function might throw exceptions
 * - More expressive than `TL_NOEXCEPT_OP` when no conditional checking is needed
 *
 * \see TL_NOEXCEPT_OP
 */
#if TL_CPP_VERSION >= 11
#  define TL_NOEXCEPT noexcept
#else
#  define TL_NOEXCEPT throw ()
#endif


/*!
 * \brief Conditionally declare non-throwing behavior.
 *
 * Declares that a function does not throw exceptions if a given condition is true.
 * Useful for template functions where exception safety depends on type traits or
 * template parameters.
 *
 * ### Usage Example
 *
 * \code{.cpp}
 * template<typename T>
 * void swapValues(T& a, T& b) TL_NOEXCEPT_OP(std::is_nothrow_move_assignable<T>::value)
 * {
 *     T temp = std::move(a);
 *     a = std::move(b);
 *     b = std::move(temp);
 * }
 *
 * // For a built-in type like int:
 * // swapValues(x, y);  // TL_NOEXCEPT_OP(true) → noexcept
 *
 * // For a user-defined type that might throw:
 * // swapValues(obj1, obj2);  // TL_NOEXCEPT_OP(false) → not noexcept
 * \endcode
 *
 * \param x  Compile-time boolean condition indicating if the function is non-throwing
 *
 * ### Behavior
 *
 * - **C++11 and later:** Translates to `noexcept(x)`
 *   - True if `x` evaluates to true at compile time
 * - **Pre-C++11:** Has no effect (uses empty expansion)
 *
 * \note
 * - Always place at the end of the function signature
 * - Typically used with `std::is_nothrow_*` type traits
 * - Enables strong exception guarantees for move operations
 * - The condition is evaluated at compile time
 *
 * \see TL_NOEXCEPT
 */
#if TL_CPP_VERSION >= 11
#  define TL_NOEXCEPT_OP(x) noexcept((x))
#else
#  define TL_NOEXCEPT_OP(x)
#endif

/*! \} */