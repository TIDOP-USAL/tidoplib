#ifndef I3D_DEFS_H
#define I3D_DEFS_H

// Definición de constantes de tipo general

#define I3D_PI   3.1415926535897932384626433832795
#define I3D_2PI  6.283185307179586476925286766559

#define _INT_MAX std::numeric_limits<int>().max()
#define _INT_MIN -std::numeric_limits<int>().max()
#define _DOUBLE_MAX std::numeric_limits<double>().max()
#define _DOUBLE_MIN -std::numeric_limits<double>().max()
#define _FLOAT_MAX std::numeric_limits<float>().max()
#define _FLOAT_MIN -std::numeric_limits<float>().max()



#if (defined WIN32 || defined _WIN32 || defined WINCE || defined __CYGWIN__) && defined I3DAPI_EXPORTS
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
#    define I3D_DEPRECATED(msg) __declspec(deprecated("Deprecated: " msg))
#  else
#    pragma message("WARNING: You need to implement DEPRECATED for this compiler")
#    define I3D_DEPRECATED(msg)
#  endif
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

//Types
#ifndef _MSC_VER
#include <stdint.h>
#else
typedef __int8            int8_t;
typedef __int16           int16_t;
typedef __int32           int32_t;
typedef __int64           int64_t;
typedef unsigned __int8   uint8_t;
typedef unsigned __int16  uint16_t;
typedef unsigned __int32  uint32_t;
typedef unsigned __int64  uint64_t;
#endif

#endif // I3D_DEFS_H
