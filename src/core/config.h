#ifndef I3D_CONFIG_H
#define I3D_CONFIG_H

/* Cabecera para configuración */

#define I3D_ENABLE_OPENCV 1

#ifdef I3D_ENABLE_OPENCV

// Modulos contrib de OpenCV
#undef I3D_ENABLE_OPENCV_SFM


#endif

// Manejo de errores como log y/o mensajes de consola

#define I3D_MESSAGE_HANDLER

// Mensajes por consola
#define I3D_ENABLE_CONSOLE

#if defined(I3D_MESSAGE_HANDLER) && defined(I3D_ENABLE_CONSOLE)

#endif

#if defined _MSC_VER && _MSC_VER >= 1600
  #define I3D_MSVS_CONCURRENCY
#endif

#endif // I3D_CONFIG_H
