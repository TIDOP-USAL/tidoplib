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


/*! \addtogroup Macros
 *  \{
 */

/*!
 * \brief Export symbol for Windows DLL when building the library.
 *
 * When building TidopLib as a shared library (DLL on Windows), classes and functions
 * marked with TL_EXPORT will be visible outside the library. On non-Windows platforms
 * or when building static libraries, this macro does nothing.
 *
 * ### Usage Example
 *
 * \code{.cpp}
 * class TL_EXPORT MyPublicClass
 * {
 * public:
 *     void publicMethod();
 * };
 *
 * TL_EXPORT void publicFunction();
 * \endcode
 *
 * ### How it Works
 *
 * - When `TIDOPLIB_EXPORTS` is defined (during library compilation):
 *   - Windows: `__declspec(dllexport)` makes symbols visible to DLL consumers
 *   - GCC/Clang: `__attribute__((visibility("default")))` makes symbols visible
 * - When `TIDOPLIB_EXPORTS` is not defined (when using the library):
 *   - No effect; symbols are imported via implicit linking
 * - On non-Windows platforms: No effect
 *
 * \note
 * - Only needed for shared libraries (DLL/SO)
 * - Static libraries do not require this macro
 * - Requires `TIDOPLIB_EXPORTS` to be defined during compilation of the library
 * - GCC/Clang requires version 4 or later
 *
 * \see TL_IMPORT
 */
#ifdef TIDOPLIB_EXPORTS
#  if (defined WIN32 || defined _WIN32 || defined WINCE || defined __CYGWIN__)
#    define TL_EXPORT __declspec(dllexport)
#  elif defined __GNUC__ && __GNUC__ >= 4
#    define TL_EXPORT __attribute__ ((visibility ("default")))
#  else
#    define TL_EXPORT
#  endif
#else
#  define TL_EXPORT
#endif


/*!
 * \brief Import symbol for Windows DLL when using the library.
 *
 * When using TidopLib as a shared library (DLL on Windows), classes and functions
 * marked with TL_IMPORT will be correctly imported from the DLL. This is typically
 * handled automatically by the linker, but can be used for explicit declaration.
 *
 * ### Usage Example
 *
 * \code{.cpp}
 * class TL_IMPORT MyPublicClass
 * {
 * public:
 *     void publicMethod();
 * };
 * \endcode
 *
 * \note
 * - Usually not needed; symbol visibility is handled automatically
 * - Provided for completeness and explicit control if needed
 * - On non-Windows platforms: typically has the same effect as TL_EXPORT
 *
 * \see TL_EXPORT
 */
#ifdef TIDOPLIB_EXPORTS
#  if (defined WIN32 || defined _WIN32 || defined WINCE || defined __CYGWIN__)
#    define TL_IMPORT __declspec(dllimport)
#  elif defined __GNUC__ && __GNUC__ >= 4
#    define TL_IMPORT __attribute__ ((visibility("default")))
#  else
#    define TL_IMPORT
#  endif
#else
#  define TL_IMPORT
#endif

/*! \} */