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
 * \brief Push compiler warning settings onto the stack.
 *
 * Saves the current compiler warning state. Must be paired with TL_WARNING_POP.
 * Allows temporary modification of warning levels in specific code sections.
 *
 * ### Usage Example
 *
 * \code{.cpp}
 * TL_WARNING_PUSH
 * TL_DISABLE_WARNING_MSVC(4996)
 * // Code with suppressed warnings
 * deprecated_function();
 * TL_WARNING_POP
 * \endcode
 *
 * \note
 * - Compiler-specific: adapts automatically to MSVC, GCC, or Clang
 * - Must always be followed by TL_WARNING_POP
 * - Can be nested on most compilers
 *
 * \see TL_WARNING_POP, TL_DISABLE_WARNING_MSVC, TL_DISABLE_WARNING_GCC, TL_DISABLE_WARNING_CLANG
 */
#if defined __clang__
#  define TL_WARNING_PUSH TL_PRAGMA(clang diagnostic push)
#elif defined __GNUC__ && (__GNUC__ * 100 + __GNUC_MINOR__ >= 406)
#  define TL_WARNING_PUSH TL_PRAGMA(GCC diagnostic push)
#elif defined _MSC_VER
#  define TL_WARNING_PUSH TL_PRAGMA(warning(push))
#else
#  define TL_WARNING_PUSH
#endif


/*!
 * \brief Pop compiler warning settings from the stack.
 *
 * Restores the compiler warning state saved by TL_WARNING_PUSH.
 * Must be paired with a previous TL_WARNING_PUSH call.
 *
 * ### Usage Example
 *
 * \code{.cpp}
 * TL_WARNING_PUSH
 * TL_DISABLE_WARNING_MSVC(4996)
 * deprecated_function();
 * TL_WARNING_POP  // Restore original warning state
 * \endcode
 *
 * \note
 * - Compiler-specific: adapts automatically to MSVC, GCC, or Clang
 * - Must always be preceded by TL_WARNING_PUSH
 *
 * \see TL_WARNING_PUSH
 */
#if defined __clang__
#  define TL_WARNING_POP TL_PRAGMA(clang diagnostic pop)
#elif defined __GNUC__ && (__GNUC__ * 100 + __GNUC_MINOR__ >= 406)
#  define TL_WARNING_POP TL_PRAGMA(GCC diagnostic pop)
#elif defined _MSC_VER
#  define TL_WARNING_POP TL_PRAGMA(warning(pop))
#else
#  define TL_WARNING_POP
#endif


/*!
 * \brief Disable a specific Clang compiler warning.
 *
 * Suppresses a compiler warning code specific to Clang/LLVM.
 * The warning code should be in Clang's format (e.g., "-Wunused-variable").
 *
 * ### Usage Example
 *
 * \code{.cpp}
 * TL_WARNING_PUSH
 * TL_DISABLE_WARNING_CLANG("-Wunused-parameter")
 * void foo(int unused_param) { }
 * TL_WARNING_POP
 * \endcode
 *
 * \param warn  Clang warning identifier (e.g., "-Wunused-variable")
 *
 * \note
 * - Only effective when compiling with Clang
 * - Warning identifier should include the "-W" prefix
 * - Should be used within TL_WARNING_PUSH / TL_WARNING_POP block
 *
 * \see TL_WARNING_PUSH, TL_WARNING_POP, TL_ENABLE_WARNING_CLANG
 */
#if defined __clang__
#  define TL_DISABLE_WARNING_CLANG(warn) TL_PRAGMA(clang diagnostic ignored warn)
#else
#  define TL_DISABLE_WARNING_CLANG(warn)
#endif


/*!
 * \brief Disable a specific GCC compiler warning.
 *
 * Suppresses a compiler warning code specific to GCC.
 * The warning code should be in GCC's format (e.g., "-Wunused-variable").
 *
 * ### Usage Example
 *
 * \code{.cpp}
 * TL_WARNING_PUSH
 * TL_DISABLE_WARNING_GCC("-Wunused-parameter")
 * void foo(int unused_param) { }
 * TL_WARNING_POP
 * \endcode
 *
 * \param warn  GCC warning identifier (e.g., "-Wunused-variable")
 *
 * \note
 * - Only effective when compiling with GCC (4.6 or later)
 * - Warning identifier should include the "-W" prefix
 * - Should be used within TL_WARNING_PUSH / TL_WARNING_POP block
 *
 * \see TL_WARNING_PUSH, TL_WARNING_POP, TL_ENABLE_WARNING_GCC
 */
#if defined __GNUC__ && (__GNUC__ * 100 + __GNUC_MINOR__ >= 406)
#  define TL_DISABLE_WARNING_GCC(warn) TL_PRAGMA(GCC diagnostic ignored warn)
#else
#  define TL_DISABLE_WARNING_GCC(warn)
#endif


/*!
 * \brief Disable a specific MSVC compiler warning.
 *
 * Suppresses a compiler warning code specific to Microsoft Visual C++.
 * The warning code should be a numeric MSVC error code (e.g., 4996).
 *
 * ### Usage Example
 *
 * \code{.cpp}
 * TL_WARNING_PUSH
 * TL_DISABLE_WARNING_MSVC(4996)  // Suppress "function is deprecated"
 * deprecated_function();
 * TL_WARNING_POP
 * \endcode
 *
 * \param warn  MSVC warning code as number (e.g., 4996, 4267)
 *
 * \note
 * - Only effective when compiling with MSVC
 * - Warning code should be numeric without "C" prefix
 * - Should be used within TL_WARNING_PUSH / TL_WARNING_POP block
 *
 * \see TL_WARNING_PUSH, TL_WARNING_POP, TL_ENABLE_WARNING_MSVC
 */
#if defined _MSC_VER
#  define TL_DISABLE_WARNING_MSVC(warn) TL_PRAGMA(warning(disable : warn))
#else
#  define TL_DISABLE_WARNING_MSVC(warn)
#endif


/*!
 * \brief Enable a specific Clang compiler warning.
 *
 * Restores a previously disabled Clang warning.
 * The warning code should be in Clang's format (e.g., "-Wunused-variable").
 *
 * ### Usage Example
 *
 * \code{.cpp}
 * TL_WARNING_PUSH
 * TL_DISABLE_WARNING_CLANG("-Wunused-parameter")
 * void foo(int unused_param) { }
 * TL_ENABLE_WARNING_CLANG("-Wunused-parameter")
 * TL_WARNING_POP
 * \endcode
 *
 * \param warn  Clang warning identifier (e.g., "-Wunused-variable")
 *
 * \note
 * - Only effective when compiling with Clang
 * - Warning identifier should include the "-W" prefix
 * - Should be used within TL_WARNING_PUSH / TL_WARNING_POP block
 *
 * \see TL_WARNING_PUSH, TL_WARNING_POP, TL_DISABLE_WARNING_CLANG
 */
#if defined __clang__
#  define TL_ENABLE_WARNING_CLANG(warn) TL_PRAGMA(clang diagnostic warning warn)
#else
#  define TL_ENABLE_WARNING_CLANG(warn)
#endif


/*!
 * \brief Enable a specific GCC compiler warning.
 *
 * Restores a previously disabled GCC warning.
 * The warning code should be in GCC's format (e.g., "-Wunused-variable").
 *
 * ### Usage Example
 *
 * \code{.cpp}
 * TL_WARNING_PUSH
 * TL_DISABLE_WARNING_GCC("-Wunused-parameter")
 * void foo(int unused_param) { }
 * TL_ENABLE_WARNING_GCC("-Wunused-parameter")
 * TL_WARNING_POP
 * \endcode
 *
 * \param warn  GCC warning identifier (e.g., "-Wunused-variable")
 *
 * \note
 * - Only effective when compiling with GCC (4.6 or later)
 * - Warning identifier should include the "-W" prefix
 * - Should be used within TL_WARNING_PUSH / TL_WARNING_POP block
 *
 * \see TL_WARNING_PUSH, TL_WARNING_POP, TL_DISABLE_WARNING_GCC
 */
#if defined __GNUC__ && (__GNUC__ * 100 + __GNUC_MINOR__ >= 406)
#  define TL_ENABLE_WARNING_GCC(warn) TL_PRAGMA(GCC diagnostic warning warn)
#else
#  define TL_ENABLE_WARNING_GCC(warn)
#endif


/*!
 * \brief Enable a specific MSVC compiler warning.
 *
 * Restores a previously disabled MSVC warning.
 * The warning code should be a numeric MSVC error code (e.g., 4996).
 *
 * ### Usage Example
 *
 * \code{.cpp}
 * TL_WARNING_PUSH
 * TL_DISABLE_WARNING_MSVC(4996)
 * deprecated_function();
 * TL_ENABLE_WARNING_MSVC(4996)
 * TL_WARNING_POP
 * \endcode
 *
 * \param warn  MSVC warning code as number (e.g., 4996, 4267)
 *
 * \note
 * - Only effective when compiling with MSVC
 * - Warning code should be numeric without "C" prefix
 * - Should be used within TL_WARNING_PUSH / TL_WARNING_POP block
 *
 * \see TL_WARNING_PUSH, TL_WARNING_POP, TL_DISABLE_WARNING_MSVC
 */
#if defined _MSC_VER
#  define TL_ENABLE_WARNING_MSVC(warn) TL_PRAGMA(warning(default : warn))
#else
#  define TL_ENABLE_WARNING_MSVC(warn)
#endif


/*!
 * \brief Disable all compiler warnings.
 *
 * Suppresses all compiler warnings globally. Useful when including third-party
 * headers that generate many warnings. Must be followed by TL_DEFAULT_WARNINGS
 * to restore the original warning state.
 *
 * ### Usage Example
 *
 * \code{.cpp}
 * TL_DISABLE_WARNINGS
 * #include <third_party_header.h>
 * TL_DEFAULT_WARNINGS
 * \endcode
 *
 * \note
 * - Compiler-specific behavior
 * - MSVC: uses `warning(push, 0)` to disable all warnings
 * - GCC/Clang: disables common warnings like "-Wall"
 * - Must be followed by TL_DEFAULT_WARNINGS to restore state
 *
 * \see TL_DEFAULT_WARNINGS, TL_WARNING_PUSH, TL_WARNING_POP
 */
#if defined __clang__
#  define TL_DISABLE_WARNINGS TL_DISABLE_WARNING_CLANG("-Wall")
#elif defined __GNUC__ && (__GNUC__ * 100 + __GNUC_MINOR__ >= 406)
#  define TL_DISABLE_WARNINGS TL_DISABLE_WARNING_GCC("-Wall")
#elif defined _MSC_VER
#  define TL_DISABLE_WARNINGS TL_PRAGMA(warning(push, 0))
#else
#  define TL_DISABLE_WARNINGS
#endif


/*!
 * \brief Restore default compiler warning level.
 *
 * Restores the compiler warning state after TL_DISABLE_WARNINGS or TL_WARNING_PUSH/POP.
 * Typically used to exit a warning suppression block.
 *
 * ### Usage Example
 *
 * \code{.cpp}
 * TL_DISABLE_WARNINGS
 * #include <third_party_header.h>
 * TL_DEFAULT_WARNINGS
 * \endcode
 *
 * \note
 * - Equivalent to TL_WARNING_POP on most compilers
 * - Should follow TL_DISABLE_WARNINGS or be inside TL_WARNING_PUSH/POP block
 *
 * \see TL_DISABLE_WARNINGS, TL_WARNING_PUSH, TL_WARNING_POP
 */
#define TL_DEFAULT_WARNINGS TL_WARNING_POP

/*! \} */