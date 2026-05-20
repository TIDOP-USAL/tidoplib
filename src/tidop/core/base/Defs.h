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


// ============================================================================
// Operating System Detection
// ============================================================================

#if defined(WIN32) || defined(_WIN32) || defined(__WIN32__)
#  define TL_OS_WINDOWS
#elif defined(linux) || defined(__linux) || defined(__linux__)
#  define TL_OS_LINUX
#elif defined(macintosh) || defined(__APPLE__) || defined(__APPLE_CC__)
#  define TL_OS_MACOSX
#elif defined(__FreeBSD__)
#  define TL_OS_FREEBSD
#elif defined(__NetBSD__)
#  define TL_OS_NETBSD
#elif defined(__OpenBSD__)
#  define TL_OS_OPENBSD
#endif

// ============================================================================
// Compiler Detection and Function Name
// ============================================================================

#if defined __GNUC__ || (defined(__cplusplus) && (__cplusplus >= 201103))
#  define TL_FUNCTION __func__
#elif defined __clang__ && (__clang_minor__ * 100 + __clang_major__ >= 305)
#  define TL_FUNCTION __func__
#elif defined __STDC_VERSION__ && (__STDC_VERSION__ >= 199901)
#  define TL_FUNCTION __func__
#elif defined _MSC_VER
#  define TL_FUNCTION __FUNCTION__
#else
#  define TL_FUNCTION ""
#endif

// ============================================================================
// Pragma Helper Macro
// ============================================================================

#ifdef _MSC_VER
#  define TL_PRAGMA(...) __pragma(__VA_ARGS__)
#elif defined __clang__
#  define TL_PRAGMA(...) _Pragma(#__VA_ARGS__)
#elif defined __GNUC__
#  define TL_PRAGMA(...) _Pragma(#__VA_ARGS__)
#endif

// ============================================================================
// Standard Includes
// ============================================================================

#include <limits>
#include <cstdlib>
#include <cmath>

#ifdef __GNUC__
#  include <cstdio>
#endif

#ifdef TL_HAVE_VLD
#  ifdef VISUAL_LEAK_DETECTOR
#    include <vld.h>
#  endif
#endif

// ============================================================================
// Version Macros (must be defined before including this file)
// ============================================================================

/*! \brief Compute a version number from major, minor, and patch components. */
#define TL_COMPUTE_VERSION(major, minor, patch) ((major<<16)|(minor<<8)|(patch))

// ============================================================================
// Windows-Specific Configuration
// ============================================================================

#if defined TL_OS_WINDOWS
#  ifndef NOMINMAX
#    define NOMINMAX
#  endif
#  define WIN32_LEAN_AND_MEAN
#  include <Windows.h>
#endif

// ============================================================================
// File Path Constants
// ============================================================================

#if defined TL_OS_WINDOWS
#  ifdef _MSC_VER
#    define TL_MAX_PATH   _MAX_PATH
#    define TL_MAX_DRIVE  _MAX_DRIVE
#    define TL_MAX_DIR    _MAX_DIR
#    define TL_MAX_FNAME  _MAX_FNAME
#    define TL_MAX_EXT    _MAX_EXT
#  else
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

// ============================================================================
// String Conversion Macros
// ============================================================================

#define TO_STRING(x) #x
#define TL_CONVERT_TO_STRING(x) TO_STRING(x)

// ============================================================================
// Include Modular Macro Headers
// ============================================================================

#include "tidop/core/base/macros/Deprecation.h"
#include "tidop/core/base/macros/Warnings.h"
#include "tidop/core/base/macros/NonCopyable.h"
#include "tidop/core/base/macros/Export.h"
#include "tidop/core/base/macros/Exception.h"