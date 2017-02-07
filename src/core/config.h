#ifndef I3D_CONFIG_H
#define I3D_CONFIG_H

/* Cabecera para configuración */

#define HAVE_OPENCV

#ifdef HAVE_OPENCV

// Modulos contrib de OpenCV
#undef I3D_ENABLE_OPENCV_SFM

#endif

#undef HAVE_GDAL

#define HAVE_RAW // libraw

#undef HAVE_EIGEN // De momento no se usa

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

#endif // I3D_CONFIG_H
