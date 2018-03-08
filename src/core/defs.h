/* Archivo para definir macros y constantes de tipo general */

#ifndef I3D_DEFS_H
#define I3D_DEFS_H

#include "core/config.h"

#include <limits>
#include <cstdlib>

#ifdef __GNUC__
#include <cstdio>
#endif // __GNUC__

#ifdef HAVE_VLD
#ifdef VISUAL_LEAK_DETECTOR
#include <vld.h>
#endif
#endif // HAVE_VLD


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

// Definición de constantes de tipo general
#define I3D_PI_2 1.5707963267948966192313216916398
#define I3D_PI   3.1415926535897932384626433832795
#define I3D_2PI  6.283185307179586476925286766559

#define I3D_RAD_TO_DEG	57.295779513082320876798154814105
#define I3D_DEG_TO_RAD	0.01745329251994329576923690768489
#define I3D_RAD_TO_GRAD	63.661977236758134307553505349006
#define I3D_GRAD_TO_RAD	0.0157079632679489661923132169164



#if defined WIN32
// Para que no den problemas std::numeric_limits<T>().max()
#  ifndef NOMINMAX
#    define NOMINMAX
#  endif
#  ifdef HAVE_RAW
#    include <Winsock2.h> // Necesario para libraw. Incluir antes de windows.h por que si no da error
#  endif
#  include <windows.h>
#endif

#define I3D_INT_MAX std::numeric_limits<int>().max()
#define I3D_INT_MIN -std::numeric_limits<int>().max()
#define I3D_DOUBLE_MAX std::numeric_limits<double>().max()
#define I3D_DOUBLE_MIN -std::numeric_limits<double>().max()
#define I3D_FLOAT_MAX std::numeric_limits<float>().max()
#define I3D_FLOAT_MIN -std::numeric_limits<float>().max()



#if defined WIN32 || defined _WIN32
#  ifdef _MSC_VER
#    define I3D_MAX_PATH   _MAX_PATH
#    define I3D_MAX_DRIVE  _MAX_DRIVE
#    define I3D_MAX_DIR    _MAX_DIR
#    define I3D_MAX_FNAME  _MAX_FNAME
#    define I3D_MAX_EXT    _MAX_EXT
#  else //__GNUC__ No encuentra _MAX_DRIVE, _MAX_DIR, _MAX_FNAME y _MAX_EXT
#    ifdef __GNUC__
#      define I3D_MAX_PATH   MAX_PATH
#    else
#      define I3D_MAX_PATH  260
#    endif
#    define I3D_MAX_DRIVE  3
#    define I3D_MAX_DIR    256
#    define I3D_MAX_FNAME  256
#    define I3D_MAX_EXT    256
#  endif
#else
#  include <linux/limits.h>
#  define I3D_MAX_PATH   PATH_MAX
#  define I3D_MAX_FNAME  NAME_MAX
#  define I3D_MAX_DRIVE  PATH_MAX
#  define I3D_MAX_DIR    PATH_MAX
#  define I3D_MAX_EXT    NAME_MAX
#endif


#if (defined WIN32 || defined _WIN32 || defined WINCE || defined __CYGWIN__) && defined I3D_API_EXPORTS
#  define I3D_EXPORT __declspec(dllexport)
//#elif defined __GNUC__ && __GNUC__ >= 4
//#  define I3D_EXPORT __attribute__ ((visibility ("default")))
#else
#  define I3D_EXPORT
#endif



#ifndef I3D_EXTERN_C
#  ifdef __cplusplus
#    define I3D_EXTERN_C extern "C"
#  else
#    define I3D_EXTERN_C
#  endif
#endif



/*!
 * \brief Warning para informar que una función esta obsoleta
 * Se debe anteceder a la función obsoleta añadiendo el método que lo reemplaza:
 *
 * I3D_DEPRECATED("newFunc(int a, float b)")
 * void oldFunc(int a, float b);
 *
 * En el caso de una clase completa se debe añadir entre class y el nombre de la clase:
 *
 * class I3D_DEPRECATED(NewClass) OldClass
 * { ...
 *
 * En el caso de las funciones template:
 *
 * template<typename T> inline
 * I3D_DEPRECATED("void newFunc(T a, T b)")
 * void oldFunc(T a, T b);
 */
#if __cplusplus >= 201402L // c++ 14
#  define I3D_DEPRECATED(msg)  [[deprecated("Deprecated: " msg)]]
#else
#  ifdef __GNUC__
#    define I3D_DEPRECATED(msg) __attribute__((deprecated("Deprecated: " msg)))
#  elif defined _MSC_VER
#    define I3D_DEPRECATED(msg) __declspec(deprecated("Deprecated: use " msg " instead"))
#  else
#    pragma message("WARNING: You need to implement DEPRECATED for this compiler")
#    define I3D_DEPRECATED(msg)
#  endif
#endif



// Mensajes de error y warning para mostrar al compilar

#define STRING2(x) #x
#define STRING(x) STRING2(x)

#if _MSC_VER
#  define I3D_COMPILER_WARNING(msg) __pragma(message( __FILE__ "(" STRING(__LINE__) "): warning(TIDOPLIB): " msg  ) )
#else
// Ver si funciona _Pragma()... Da los errores:
// /home/esteban/desarrollo/tidoplib/src/transform.h:331: error: _Pragma takes a parenthesized string literal
// I3D_COMPILER_WARNING("'compute' no esta soportado para TrfMultiple");
//                                                                   ^
// /home/esteban/desarrollo/tidoplib/src/core/defs.h:102: error: there are no arguments to '_Pragma' that depend on a template parameter, so a declaration of '_Pragma' must be available [-fpermissive]
//  #  define I3D_COMPILER_WARNING(msg) _Pragma(message( __FILE__ "(" STRING(__LINE__) "): warning(TIDOPLIB): " msg  ) )
//                                                                                                                   ^
// Por ahora no hace nada...
#  define I3D_COMPILER_WARNING(msg) //_Pragma(message( __FILE__ "(" STRING(__LINE__) "): warning(TIDOPLIB): " msg  ) )
#endif



// __FUNCTION__ no es estandar (Es de Visual Studio).
// __func__ es valida a partir de C99 / C++11
#if defined (__GNUC__) || __cplusplus >= 201103L
#  define I3D_FUNCTION __func__
#elif defined _MSC_VER
#  define I3D_FUNCTION __FUNCTION__
#else
#  define I3D_FUNCTION ""
#endif
//__FUNCSIG__



// Desactivar/activar warnings
#if defined _MSC_VER

/*!
 * \brief Se suprimen todos los mensajes de advertencia
 *
 * <h4>Ejemplo</h4>
 * \code
 * I3D_SUPPRESS_WARNINGS
 * bool f(int v) {      // warning C4100: 'v' : parámetro formal sin referencia
 *   int b = 0;         // warning C4189: 'b' : la variable local se ha inicializado pero no se hace referencia a ella
 *   double d = 1.0;
 *   float f = d;       // warning C4244: '=' : conversión de 'double' a 'float'; posible pérdida de datos
 *   return true;
 * }
 * \endcode
 */
#  define I3D_SUPPRESS_WARNINGS __pragma(warning(push, 0))

/*!
 * \brief Se pone por defecto la configuración de mensajes de advertencia
 */
#  define I3D_DEFAULT_WARNINGS __pragma(warning(pop))

/*!
 * \brief Se activa un mensaje de advertencia especifico
 *
 * <h4>Ejemplo</h4>
 * \code
 * I3D_DISABLE_WARNING(4244)
 * bool f(double d) {
 *   float f = d;       // warning C4244: '=' : conversión de 'double' a 'float'; posible pérdida de datos
 *   return true;
 * }
 * I3D_ENABLE_WARNING(4244)
 * \endcode
 */
#  define I3D_DISABLE_WARNING(warn) __pragma(warning(disable : warn))

/*!
 * \brief Se desactiva un mensaje de advertencia especifico
 *
 * <h4>Ejemplo</h4>
 * \code
 * I3D_DISABLE_WARNING(4244)
 * bool f(double d) {
 *   float f = d;       // warning C4244: '=' : conversión de 'double' a 'float'; posible pérdida de datos
 *   return true;
 * }
 * I3D_ENABLE_WARNING(4244)
 * \endcode
 */
#  define I3D_ENABLE_WARNING(warn) __pragma(warning(default : warn))
#else
#  define I3D_SUPPRESS_WARNINGS
#  define I3D_DEFAULT_WARNINGS
#  define I3D_DISABLE_WARNING(warn)
#  define I3D_ENABLE_WARNING(warn)
#endif



// MACROS

/*!
 * \brief Redondea un doble o float y ademas convierte a entero
 * De esta forma se evita el warning C4244 (conversión de 'double' a 'int')
 * y nos aseguramos de que redondee de la forma correcta.
 */
#define I3D_ROUND_TO_INT(n)  static_cast<int>(round(n))

#ifndef _MSC_VER
//En el compilador de LINUX no incluye printf_s de C++11
// De momento lo hago asi...
#define printf_s(...) printf( __VA_ARGS__)
#define sprintf_s(dest, len, format, ...) snprintf(dest, len, format, __VA_ARGS__)
#endif


#endif // I3D_DEFS_H
