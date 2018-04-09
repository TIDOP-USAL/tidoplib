#ifndef TL_CONFIG_H
#define TL_CONFIG_H

/* Cabecera para configuración */

#define HAVE_OPENCV

//#ifdef HAVE_OPENCV
//
//// Modulos contrib de OpenCV
//#undef TL_ENABLE_OPENCV_SFM
//
//#endif

// Activa GDAL
/* #undef HAVE_GDAL */

// libraw: Lectura de formato raw
/* #undef HAVE_LIBRAW */
/* #undef HAVE_EDSDK */

// Eigen Lib: Se utiliza para la resolución de sistemas de ecuaciones lineales
// Es mucho mas rapida que OpenCV
#define HAVE_EIGEN

// OpenMP
/* #undef HAVE_OMP */

// Parallel Patterns Library
// https://msdn.microsoft.com/es-es/library/dd492418.aspx
/* #undef HAVE_PPL */

// Manejo de errores como log y/o mensajes de consola

#define TL_MESSAGE_HANDLER

// Mensajes por consola
/* #undef TL_ENABLE_CONSOLE */

//#if defined(TL_MESSAGE_HANDLER) && defined(TL_ENABLE_CONSOLE)

//#endif

// Visual Leak Detector
/* #undef HAVE_VLD */

// minizip
/* #undef HAVE_MINIZIP */

// boost
#define HAVE_BOOST

#endif // TL_CONFIG_H
