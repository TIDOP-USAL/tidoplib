#ifndef I3D_CONFIG_H
#define I3D_CONFIG_H

// Cabecera para configuración 

///*! \mainpage Página principal
// *
// * \section intro_sec Introducción
// *
// * Tidop Lib
// *
// * \section Modulos
// *
// * \subsection Entidades GeometricEntities 
// *
// * etc...
// */

// Manejo de errores como log y/o mensajes de consola
#define I3D_MESSAGE_HANDLER

// Mensajes por consola
#undef I3D_ENABLE_CONSOLE

#if defined(I3D_MESSAGE_HANDLER) && defined(I3D_ENABLE_CONSOLE)

// manejador de error para OpenCV. Para evitar los mensajes por consola de OpenCV
int handleError( int status, const char* func_name,
            const char* err_msg, const char* file_name,
            int line, void* userdata )
{
    return 0;
}

cv::redirectError(handleError);

#endif

#endif // I3D_CONFIG_H
