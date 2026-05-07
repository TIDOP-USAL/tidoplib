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
 * \brief Macro to indicate that a function, class or variable is deprecated.
 *
 * Generates compiler warnings when deprecated code is used and provides
 * information about the version in which it was deprecated and what to use instead.
 *
 * ### Usage Examples
 *
 * **For functions:**
 * \code{.cpp}
 * TL_DEPRECATED("newFunc(int a, float b)", "2.0")
 * void oldFunc(int a, float b);
 * \endcode
 *
 * **For classes or structs:**
 * \code{.cpp}
 * class TL_DEPRECATED("NewClass", "2.0") OldClass
 * {
 *     // ...
 * };
 * \endcode
 *
 * **For enums:**
 * \code{.cpp}
 * enum class TL_DEPRECATED("NewEnum", "2.0") OldEnum
 * {
 *     // ...
 * };
 * \endcode
 *
 * **For template functions:**
 * \code{.cpp}
 * template<typename T> inline
 * TL_DEPRECATED("void newFunc(T a, T b)", "2.0")
 * void oldFunc(T a, T b);
 * \endcode
 *
 * \param msg  Message with the recommended alternative (e.g., "use newFunc() instead")
 * \param version  Version in which it was deprecated (e.g., "2.0")
 *
 * \note
 * - The compiler will show warnings when deprecated code is used
 * - Requires C++14 to use standard attributes [[deprecated]]
 * - For older compilers, compiler-specific attributes are used
 * - Can be disabled by defining TL_WARNING_DEPRECATED_METHOD as 0 or leaving it undefined
 *
 * \see TL_TODO
 */
#ifdef TL_WARNING_DEPRECATED_METHOD 
#  if TL_CPP_VERSION>= 14
#    define TL_DEPRECATED(msg, version)  [[deprecated("Deprecated in version " TL_CONVERT_TO_STRING(version) ". Use " msg " instead")]]
#  else
#    ifdef __GNUC__
#      define TL_DEPRECATED(msg, version) __attribute__((deprecated("Deprecated in version " version ". Use " msg " instead")))
#    elif defined _MSC_VER
#      define TL_DEPRECATED(msg, version) __declspec(deprecated("Deprecated in version " version ". use " msg " instead"))
#    else
#      pragma message("WARNING: You need to implement TL_DEPRECATED for this compiler")
#      define TL_DEPRECATED(msg, version)
#    endif
#  endif
#else
#  define TL_DEPRECATED(msg, version)
#endif

/*!
 * \brief Macro to generate TODO messages during compilation.
 *
 * Displays a compiler warning message during compilation.
 * Useful for remembering pending tasks without breaking compilation.
 *
 * ### Usage Example
 *
 * \code{.cpp}
 * TL_TODO("Implement parameter validation")
 * void functionToDo(int param);
 * \endcode
 *
 * **Expected output (MSVC):**
 * ```
 * file.cpp(42): TODO(TidopLib): Implement parameter validation
 * ```
 *
 * **Expected output (GCC/Clang):**
 * ```
 * file.cpp:42: note: #pragma message: TODO: Implement parameter validation
 * ```
 *
 * \param msg  The message of the pending task
 *
 * \note
 * - The message appears during compilation but does not generate an error
 * - Can be disabled by leaving TL_WARNING_TODO undefined
 * - Different from TL_DEPRECATED: this is for internal reminders
 *
 * \see TL_DEPRECATED, TL_COMPILER_WARNING
 */
#ifdef TL_WARNING_TODO
#  ifdef _MSC_VER
#    define TL_TODO(msg) TL_PRAGMA(message( __FILE__ "(" TL_CONVERT_TO_STRING(__LINE__) "): TODO(TidopLib): " msg  ) )
#  elif defined __GNUC__ || defined __clang__
#    define TL_TODO(x) TL_PRAGMA(message ("TODO: " #x))
#  else
#    define TL_TODO(msg)
#  endif
#else
#  define TL_TODO(msg)
#endif

/*!
 * \brief Macro to generate custom compiler warning messages at compile time.
 *
 * Allows displaying compiler warning messages at specific code locations.
 * Useful for alerting about potential issues without breaking compilation.
 *
 * ### Usage Example
 *
 * \code{.cpp}
 * #if !defined(SPECIAL_CONFIG)
 *   TL_COMPILER_WARNING("SPECIAL_CONFIG not defined. Using default configuration")
 * #endif
 * \endcode
 *
 * \param msg  The warning message to display
 *
 * \note
 * - The message automatically includes the file and line number
 * - MSVC: uses `__pragma(message(...))`
 * - GCC/Clang: uses `_Pragma()` with TL_PRAGMA
 *
 * \see TL_TODO
 */
#ifdef _MSC_VER
#  define TL_COMPILER_WARNING(msg) __pragma(message( __FILE__ "(" TL_CONVERT_TO_STRING(__LINE__) "): warning(TIDOPLIB): " msg  ) )
#else
#  define TL_COMPILER_WARNING(msg) TL_PRAGMA(message( __FILE__ "(" TL_CONVERT_TO_STRING(__LINE__) "): warning(TIDOPLIB): " msg  ) )
#endif

/*! \} */