/* Archivo para definir macros y constantes de tipo general */

#ifndef I3D_DEFS_H
#define I3D_DEFS_H

#include <limits>
#include <cstdlib>

#if _MSC_VER >= 1600
  #include <cstdint>
#else
//Copiado de stdint.h. Las versiones antiguas de Visual Studio no incluyen stdint.h
  typedef signed char        int8_t;
  typedef short              int16_t;
  typedef int                int32_t;
  typedef long long          int64_t;
  typedef unsigned char      uint8_t;
  typedef unsigned short     uint16_t;
  typedef unsigned int       uint32_t;
  typedef unsigned long long uint64_t;
#endif

// Definición de constantes de tipo general

#define I3D_PI   3.1415926535897932384626433832795
#define I3D_2PI  6.283185307179586476925286766559

#define I3D_RAD_TO_DEG	57.295779513082320876798154814105
#define I3D_DEG_TO_RAD	0.01745329251994329576923690768489
#define I3D_RAD_TO_GRAD	63.661977236758134307553505349006
#define I3D_GRAD_TO_RAD	0.0157079632679489661923132169164

#define I3D_INT_MAX std::numeric_limits<int>().max()
#define I3D_INT_MIN -std::numeric_limits<int>().max()
#define I3D_DOUBLE_MAX std::numeric_limits<double>().max()
#define I3D_DOUBLE_MIN -std::numeric_limits<double>().max()
#define I3D_FLOAT_MAX std::numeric_limits<float>().max()
#define I3D_FLOAT_MIN -std::numeric_limits<float>().max()

#if defined WIN32 || defined _WIN32
#  define I3D_MAX_PATH   _MAX_PATH
#  define I3D_MAX_DRIVE  _MAX_DRIVE
#  define I3D_MAX_DIR    _MAX_DIR
#  define I3D_MAX_FNAME  _MAX_FNAME
#  define I3D_MAX_EXT    _MAX_EXT
#else

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



// Warning para informar que una función esta obsoleta
// Se debe anteceder a la función obsoleta añadiendo el método que lo reemplaza;
// DEPRECATED("Use NewFunc(int a, float b) en su lugar") 
// void OldFunc(int a, float b);

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
#  define COMPILER_WARNING(msg) __pragma(message( __FILE__ "(" STRING(__LINE__) "): warning(TIDOPLIB): " msg  ) )
#else
// Ver si funciona _Pragma()
#  define COMPILER_WARNING(msg) _Pragma(message( __FILE__ "(" STRING(__LINE__) "): warning(TIDOPLIB): " msg  ) )
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
#  define I3D_SUPPRESS_WARNINGS __pragma(warning(push, 0))
#  define I3D_DEFAULT_WARNINGS __pragma(warning(pop))
#  define I3D_DISABLE_WARNING(warn) __pragma(warning(disable : warn))
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

#endif // I3D_DEFS_H
