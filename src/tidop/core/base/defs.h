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

/* Archivo para definir macros y constantes de tipo general */

#pragma once


#if defined(WIN32) || defined(_WIN32) || defined(__WIN32__)
#define TL_OS_WINDOWS
#elif defined(linux) || defined(__linux) || defined(__linux__)
#define TL_OS_LINUX
#elif defined(macintosh) || defined(__APPLE__) || defined(__APPLE_CC__)
#define TL_OS_MACOSX
#elif defined(__FreeBSD__)
#define TL_OS_FREEBSD
#elif defined(__NetBSD__)
# define TL_OS_NETBSD
#elif defined(__OpenBSD__)
#define TL_OS_OPENBSD
#else
#endif


#if defined __GNUC__ || (defined(__cplusplus) && (__cplusplus >= 201103))
#  define TL_FUNCTION __func__
#elif defined __clang__ && (__clang_minor__ * 100 + __clang_major__ >= 305)
#define TL_FUNCTION __func__
#elif defined __STDC_VERSION__ && (__STDC_VERSION__ >= 199901)
# define TL_FUNCTION __func__
#elif defined _MSC_VER
#  define TL_FUNCTION __FUNCTION__
#else
#  define TL_FUNCTION ""
#endif


#ifdef _MSC_VER
#  define TL_PRAGMA(...) __pragma(__VA_ARGS__)
#elif defined __clang__
#  define TL_PRAGMA(...) _Pragma(#__VA_ARGS__)
#elif defined __GNUC__
#  define TL_PRAGMA(...) _Pragma(#__VA_ARGS__)
#endif



#include "tidop/config.h"

#include <limits>
#include <cstdlib>
#include <cmath>

#ifdef __GNUC__
#include <cstdio>
#endif // __GNUC__

#ifdef TL_HAVE_VLD
#ifdef VISUAL_LEAK_DETECTOR
#include <vld.h>
#endif
#endif // TL_HAVE_VLD


#define TL_COMPUTE_VERSION(major, minor, patch) ((major<<16)|(minor<<8)|(patch))
#define TL_VERSION_NUM TL_COMPUTE_VERSION(TL_VERSION_MAJOR, TL_VERSION_MINOR, TL_VERSION_PATCH)


#if defined _MSC_VER && _MSC_VER < 1600

//Copiado de stdint.h. Las versiones antiguas de Visual Studio no incluyen stdint.h
  typedef signed char        int8_t;
  typedef short              int16_t;
  typedef int                int32_t;
  typedef long long          int64_t;
  typedef unsigned char      uint8_t;
  typedef unsigned short     uint16_t;
  typedef unsigned int       uint32_t;
  typedef unsigned long long uint64_t;
#else
  #include <cstdint>
#endif

#if defined TL_OS_WINDOWS
// Para que no den problemas std::numeric_limits<T>().max()
#  ifndef NOMINMAX
#    define NOMINMAX
#  endif
// Necesario para libraw y para PDAL. Incluir antes de windows.h por que si no da error
//#  include <Winsock2.h>
#define WIN32_LEAN_AND_MEAN
#  include <Windows.h>
#endif


/*!
 * \addtogroup Core
 *
 * \{
 */


/* Límites maximos y minimos de tipos numéricos */

#define TL_INT_MAX std::numeric_limits<int>().max()
#define TL_INT_MIN -std::numeric_limits<int>().max()
#define TL_DOUBLE_MAX std::numeric_limits<double>().max()
#define TL_DOUBLE_MIN -std::numeric_limits<double>().max()
#define TL_FLOAT_MAX std::numeric_limits<float>().max()
#define TL_FLOAT_MIN -std::numeric_limits<float>().max()



#if defined TL_OS_WINDOWS
#  ifdef _MSC_VER
#    define TL_MAX_PATH   _MAX_PATH
#    define TL_MAX_DRIVE  _MAX_DRIVE
#    define TL_MAX_DIR    _MAX_DIR
#    define TL_MAX_FNAME  _MAX_FNAME
#    define TL_MAX_EXT    _MAX_EXT
#  else //__GNUC__ No encuentra _MAX_DRIVE, _MAX_DIR, _MAX_FNAME y _MAX_EXT
#    ifdef __GNUC__
#      define TL_MAX_PATH   PATH_MAX
#    else
#      define TL_MAX_PATH  260
#    endif
#    define TL_MAX_DRIVE  3
#    define TL_MAX_DIR    256
#    define TL_MAX_FNAME  256
#    define TL_MAX_EXT    256
#  endif
#else
#  include <limits.h>
#  define TL_MAX_PATH   PATH_MAX
#  define TL_MAX_FNAME  NAME_MAX
#  define TL_MAX_DRIVE  PATH_MAX
#  define TL_MAX_DIR    PATH_MAX
#  define TL_MAX_EXT    NAME_MAX
#endif



#ifdef TIDOPLIB_EXPORTS
#  if (defined WIN32 || defined _WIN32 || defined WINCE || defined __CYGWIN__)
#    define TL_EXPORT __declspec(dllexport)
#    define TL_IMPORT __declspec(dllimport)
#  elif defined __GNUC__ && __GNUC__ >= 4
#    define TL_EXPORT __attribute__ ((visibility ("default")))
#    define TL_IMPORT __attribute__ ((visibility("default")))
#  endif
#else
#  define TL_EXPORT
#endif



#ifndef TL_EXTERN_C
#  ifdef __cplusplus
#    define TL_EXTERN_C extern "C"
#  else
#    define TL_EXTERN_C
#  endif
#endif


// convierte a cadena
#define TO_STRING(x) #x
// Convierte a cadena despues de expandir una macro
#define TL_CONVERT_TO_STRING(x) TO_STRING(x)


/*!
 * \brief Warning para informar que una función esta obsoleta
 *
 * ### Example Usage
 * 
 * Se debe anteceder a la función obsoleta añadiendo el método que lo reemplaza
 * 
 * \code{.cpp}
 * TL_DEPRECATED("newFunc(int a, float b)", "2.0")
 * void oldFunc(int a, float b);
 * \endcode
 * 
 * En el caso de una clase o un enum se debe añadir entre class y el nombre de la clase:
 * 
 * \code{.cpp}
 * class TL_DEPRECATED(NewClass, "2.0") OldClass
 * { ...
 * \endcode
 * 
 * \code{.cpp}
 * En el caso de las funciones template:
 *
 * template<typename T> inline
 * TL_DEPRECATED("void newFunc(T a, T b)", "2.0")
 * void oldFunc(T a, T b);
 * \endcode
 */
#ifdef TL_WARNING_DEPRECATED_METHOD 
#  if CPP_VERSION >= 14
#    define TL_DEPRECATED(msg, version)  [[deprecated("Deprecated in version " TL_CONVERT_TO_STRING(version) ". Use " msg " instead")]]
//#    define TL_DEPRECATED(msg, version)  [[deprecated("Deprecated in version " version ": Use " msg " instead")]]
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



// Mensajes de error y warning para mostrar al compilar


#ifdef _MSC_VER
#  define TL_COMPILER_WARNING(msg) __pragma(message( __FILE__ "(" TL_CONVERT_TO_STRING(__LINE__) "): warning(TIDOPLIB): " msg  ) )
#else
// Ver si funciona _Pragma()... Da los errores:
// /home/esteban/desarrollo/tidoplib/src/transform.h:331: error: _Pragma takes a parenthesized string literal
// TL_COMPILER_WARNING("'compute' no esta soportado para TransformMultiple");
//                                                                   ^
// /home/esteban/desarrollo/tidoplib/src/core/defs.h:102: error: there are no arguments to '_Pragma' that depend on a template parameter, so a declaration of '_Pragma' must be available [-fpermissive]
//  #  define TL_COMPILER_WARNING(msg) _Pragma(message( __FILE__ "(" TL_CONVERT_TO_STRING(__LINE__) "): warning(TIDOPLIB): " msg  ) )
//                                                                                                                   ^
//#  define LINUX_PRAGMA(x) MAKE_LINUX_PRAGMA(x)
#  define TL_COMPILER_WARNING(msg) TL_PRAGMA(message( __FILE__ "(" TL_CONVERT_TO_STRING(__LINE__) "): warning(TIDOPLIB): " msg  ) )
#endif





#if CPP_VERSION >= 11
#  define TL_NOEXCEPT noexcept
#  define TL_NOEXCEPT_OP(x) noexcept((x))
#else
#  define TL_NOEXCEPT throw ()
#  define TL_NOEXCEPT_OP(x)
#endif



/*-----------------------------------------------------------------------------------*/
/*                                       WARNIGS                                     */
/*-----------------------------------------------------------------------------------*/



#if defined __clang__
#  define TL_WARNING_PUSH TL_PRAGMA(clang diagnostic push)
#  define TL_WARNING_POP TL_PRAGMA(clang diagnostic push)
#  define TL_DISABLE_WARNING_CLANG(warn) TL_PRAGMA(clang diagnostic ignored warn)
#  define TL_DISABLE_WARNING_GCC(warn)
#  define TL_DISABLE_WARNING_MSVC(warn)
#  define TL_ENABLE_WARNING_CLANG(warn) TL_PRAGMA(clang diagnostic warning warn) 
#  define TL_ENABLE_WARNING_GCC(warn)
#  define TL_ENABLE_WARNING_MSVC(warn)
#  define TL_DISABLE_WARNINGS TL_DISABLE_WARNING_CLANG("-Wall")
#elif defined __GNUC__ && (__GNUC__ * 100 + __GNUC_MINOR__ >= 406)
#  define TL_WARNING_PUSH TL_PRAGMA(GCC diagnostic push)
#  define TL_WARNING_POP TL_PRAGMA(GCC diagnostic pop)
#  define TL_DISABLE_WARNING_GCC(warn) TL_PRAGMA(GCC diagnostic ignored warn)
#  define TL_DISABLE_WARNING_CLANG(warn)
#  define TL_DISABLE_WARNING_MSVC(warn)
#  define TL_ENABLE_WARNING_GCC(warn) TL_PRAGMA(GCC diagnostic warning warn)
#  define TL_ENABLE_WARNING_CLANG(warn)
#  define TL_ENABLE_WARNING_MSVC(warn)
#  define TL_DISABLE_WARNINGS TL_DISABLE_WARNING_GCC("-Wall")
#elif defined _MSC_VER
#  define TL_WARNING_PUSH TL_PRAGMA(warning(push))
#  define TL_WARNING_POP TL_PRAGMA(warning(pop))
#  define TL_DISABLE_WARNING_MSVC(warn) TL_PRAGMA(warning(disable : warn))
#  define TL_DISABLE_WARNING_CLANG(warn)
#  define TL_DISABLE_WARNING_GCC(warn)
#  define TL_ENABLE_WARNING_MSVC(warn) TL_PRAGMA(warning(default : warn))
#  define TL_ENABLE_WARNING_CLANG(warn)
#  define TL_ENABLE_WARNING_GCC(warn)
#  define TL_DISABLE_WARNINGS TL_PRAGMA(warning(push, 0))
#else
#  define TL_WARNING_PUSH 
#  define TL_WARNING_POP
#  define TL_DISABLE_WARNING_MSVC(warn)
#  define TL_DISABLE_WARNING_CLANG(warn)
#  define TL_DISABLE_WARNING_GCC(warn)
#  define TL_ENABLE_WARNING_CLANG(warn)
#  define TL_ENABLE_WARNING_GCC(warn)
#  define TL_ENABLE_WARNING_MSVC(warn)
#  define TL_DISABLE_WARNINGS
#endif

#define TL_DEFAULT_WARNINGS TL_WARNING_POP


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



#define TL_DISABLE_COPY(Class)                  \
    Class(const Class &) = delete;              \
    Class &operator = (const Class &) = delete;

#define TL_DISABLE_MOVE(Class)                  \
    Class(Class &&) = delete;                   \
    Class &operator = (Class &&) = delete;

/*! \} */ 