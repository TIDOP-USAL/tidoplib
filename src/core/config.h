#ifndef I3D_CONFIG_H
#define I3D_CONFIG_H

/* Cabecera para configuración */

#define HAVE_OPENCV

#ifdef HAVE_OPENCV

// Modulos contrib de OpenCV
#undef I3D_ENABLE_OPENCV_SFM

#endif

// Activa GDAL
#define HAVE_GDAL

// libraw: Lectura de formato raw
#undef HAVE_LIBRAW
#undef HAVE_EDSDK

// Eigen Lib: Se utiliza para la resolución de sistemas de ecuaciones lineales
// Es mucho mas rapida que OpenCV
#define HAVE_EIGEN

// OpenMP
#undef HAVE_OMP // De momento no se usa

// Parallel Patterns Library
// https://msdn.microsoft.com/es-es/library/dd492418.aspx
#define HAVE_PPL

// Manejo de errores como log y/o mensajes de consola

#define I3D_MESSAGE_HANDLER

// Mensajes por consola
#define I3D_ENABLE_CONSOLE

#if defined(I3D_MESSAGE_HANDLER) && defined(I3D_ENABLE_CONSOLE)

#endif

// Visual Leak Detector
#undef HAVE_VLD

#endif // I3D_CONFIG_H
