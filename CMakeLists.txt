############################################################################
#                                                                          #
#  This file is part of TidopLib and can not be copied and/or distributed  #
#  without the express permision of ITOS3D ENGINEERING S.L                 #
#                                                                          #
#  Contact: http://www.itos3d.com                                          #
#           http://tidop.usal.es                                           #
#                                                                          #
#--------------------------------------------------------------------------#
#                                                                          #
#  Copyright (C) 2018, ITOS3D ENGINEERING S.L - All rights reserved        #
#                                                                          #
############################################################################


# Versión mínima soportada de CMake
cmake_minimum_required(VERSION 3.5.0 FATAL_ERROR)

# Nombre del projecto y versión
project(TidopLib
        VERSION 1.1.0
        LANGUAGES CXX)


# Se activa el uso de carpetas para una mejor organización en Visual Studio
set_property(GLOBAL
             PROPERTY USE_FOLDERS ON)


# Establece un sufijo para las librerias de debug. De esta forma se pueden
# instalar en el mismo directorio
set(CMAKE_DEBUG_POSTFIX d)

# Tipos de configuración de compilación disponibles
set(CMAKE_CONFIGURATION_TYPES Release;Debug)

# Macro para depuración
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -D_DEBUG")

if (UNIX)
  set(CMAKE_CXX_STANDARD 14)
  set(CMAKE_CXX_STANDARD_REQUIRED ON)
  set(CMAKE_CXX_EXTENSIONS OFF)
endif()

# Directivas del preprocesador
add_definitions(-DUNICODE)


if(MSVC)
  if(CMAKE_CL_64)
    set(TL_ARCH x64)
  elseif((CMAKE_GENERATOR MATCHES "ARM") OR ("${arch_hint}" STREQUAL "ARM") OR (CMAKE_VS_EFFECTIVE_PLATFORMS MATCHES "ARM|arm"))
    set(TL_ARCH ARM)
  else()
    set(TL_ARCH x86)
  endif()
  if(MSVC_VERSION EQUAL 1400)
    set(TL_RUNTIME vc8)
  elseif(MSVC_VERSION EQUAL 1500)
    set(TL_RUNTIME vc9)
  elseif(MSVC_VERSION EQUAL 1600)
    set(TL_RUNTIME vc10)
  elseif(MSVC_VERSION EQUAL 1700)
    set(TL_RUNTIME vc11)
  elseif(MSVC_VERSION EQUAL 1800)
    set(TL_RUNTIME vc12)
  elseif(MSVC_VERSION EQUAL 1900)
    set(TL_RUNTIME vc14)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /std:c++14")
  elseif(MSVC_VERSION MATCHES "^191[0-9]$")
    set(TL_RUNTIME vc15)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /Zc:__cplusplus /std:c++14")
  elseif(MSVC_VERSION MATCHES "^192[0-9]$")
    set(TL_RUNTIME vc16)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /Zc:__cplusplus /std:c++14")
  else()
    message(WARNING "TidopLib does not recognize MSVC_VERSION \"${MSVC_VERSION}\". Cannot set TL_RUNTIME")
  endif()
elseif(MINGW)
  set(TL_RUNTIME mingw)

  if(MINGW64)
    set(TL_ARCH x64)
  else()
    set(TL_ARCH x86)
  endif()
endif()

# Configuracion especifica de Visual Studio
if(MSVC)

  # Nivel de warning
  set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /W3")

  # /EH (Modelo de control de excepciones)
  # /EHsc captura solo las excepciones de C++ e indica al compilador que suponga que las funciones
  # declaradas como extern "C" jamás inician una excepción de C++.
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /EHsc")

  # Las llamadas a los métodos potencialmente no seguros en la biblioteca estándar de C++ dan lugar
  # a una Advertencia del compilador (nivel 3) C4996.
  # Para deshabilitar esta advertencia se debe definir la macro _SCL_SECURE_NO_WARNINGS
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -D_SCL_SECURE_NO_WARNINGS -D_CRT_SECURE_NO_DEPRECATE")

  # https://docs.microsoft.com/es-es/cpp/build/reference/zc-cplusplus?view=vs-2019
  #set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /Zc:__cplusplus /std:c++latest")

endif()


if(WIN32 AND CMAKE_HOST_SYSTEM_NAME MATCHES Windows)
  if(DEFINED TL_RUNTIME AND DEFINED TL_ARCH)
    set(TL_INSTALL_BINARIES_PREFIX "${TL_ARCH}/${TL_RUNTIME}/")
  else()
    message(STATUS "Can't detect runtime and/or arch")
    set(TL_INSTALL_BINARIES_PREFIX "")
  endif()
#else()
#  ocv_update(TL_INSTALL_BINARIES_PREFIX "")
endif()

# Se permite elegir entre libreria estatica o
OPTION (BUILD_SHARED_LIBS "Build Shared Libraries" OFF)

# Por defecto se establece el tipo de librería estática
set (LIB_TYPE STATIC)

if (BUILD_SHARED_LIBS)
  # Se configura como libreria estática
  set (LIB_TYPE SHARED)

  if (MSVC)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /wd4251")
  endif(MSVC)

endif (BUILD_SHARED_LIBS)


# Activación/desactivación de los diferentes modulos de la libreria
##TODO: CORE tiene que ser obligatorio...
OPTION (BUILD_TL_CORE         "Include core module in TidopLib build"         ON )
OPTION (BUILD_TL_MATH         "Include math module in TidopLib build"         ON )
OPTION (BUILD_TL_GEOMETRY     "Include geom module in TidopLib build"         ON )
OPTION (BUILD_TL_IMG          "Include img module in TidopLib build"          ON )
OPTION (BUILD_TL_IMG_PROCESS  "Include imgprocess module in TidopLib build"   ON )
OPTION (BUILD_TL_FEAT_MATCH   "Include featmatch module in TidopLib build"    ON )
OPTION (BUILD_TL_GRAPHIC      "Include graphic module in TidopLib build"      ON )
OPTION (BUILD_TL_GEOSPATIAL   "Include geospatial module in TidopLib build"   ON )
OPTION (BUILD_TL_VECT         "Include vect module in TidopLib build"         ON )
OPTION (BUILD_TL_EXPERIMENTAL "Include experimental module in TidopLib build" OFF)

# Activación/desactivación de otros componentes de la libreria
OPTION (BUILD_APPS            "APPS"                  ON)
OPTION (BUILD_TEST            "Include test"          OFF)
OPTION (BUILD_DOC             "Build documentation"   OFF)

# Activa el gestor de mensajes. Si se desactiva no se mostrarán los mensajes del la librería
# por consola ni se escribirán en el log
OPTION (BUILD_MESSAGE_HANDLER "Active message handle" ON)

# Si se activa esta opción se pueden utilizar los métodos marcados como obsoletos (deprecated)
OPTION (BUILD_DEPRECATED_METHODS "Show deprecated methods and class" OFF)

if(BUILD_MESSAGE_HANDLER)
  set(TL_MESSAGE_HANDLER YES)
  message(STATUS "[TidopLib] Message manager enabled")
else()
  set(TL_MESSAGE_HANDLER NO)
  message(STATUS "[TidopLib] Message manager disabled")
endif()

if(BUILD_DEPRECATED_METHODS)
  set(TL_ENABLE_DEPRECATED_METHODS YES)
  message(WARNING "[TidopLib] Enable deprecated methods and class")
else()
  set(TL_ENABLE_DEPRECATED_METHODS NO)
  message(STATUS "[TidopLib] Disable deprecated methods and class")
endif()

# Test
if(BUILD_TEST)
  enable_testing() #CMake predefined targets
  if (BUILD_GTEST)
    set(HAVE_GTEST YES)
  endif()
  if (BUILD_GMOCK)
    set(HAVE_GMOCK YES)
  endif()
endif()

# Librerias externas:
# - Sólo es obligatoria Boost
# - Según las dependencias incluidas las funcionalidades de la librería variarán
OPTION (WITH_BOOST  "Include Boost support"                   ON)
OPTION (WITH_OPENCV "Include OpenCV support"                  OFF)
OPTION (WITH_EIGEN  "Include eigen support"                   OFF)
OPTION (WITH_GDAL   "Include GDAL Support"                    OFF)
OPTION (WITH_PROJ4  "Include PROJ4 Support"                   OFF)
OPTION (WITH_LIBRAW "Include LibRaw Support"                  OFF)
OPTION (WITH_EDSDK  "Include EDSDK Support"                   OFF)
OPTION (WITH_PPL    "Include PPL Support"                     OFF)
OPTION (WITH_VLD    "Include Visual Leak Detector Support"    OFF)
OPTION (WITH_LAPACKE "Include LAPACKE"                        OFF)


# ----------------------------------------------------------------------------
#  Modulos TidopLib
# ----------------------------------------------------------------------------

set(HAVE_TL_CORE NO)
set(HAVE_TL_MATH NO)
set(HAVE_TL_GEOMETRY NO)
set(HAVE_TL_IMG NO)
set(HAVE_TL_IMG_PROCESS NO)
set(HAVE_TL_FEAT_MATCH NO)
set(HAVE_TL_GRAPHIC NO)
set(HAVE_TL_GEOSPATIAL NO)
set(HAVE_TL_VECT NO)
set(HAVE_TL_EXPERIMENTAL NO)

if(BUILD_TL_CORE)
    set(HAVE_TL_CORE YES)
	if(BUILD_TL_MATH)
	    set(HAVE_TL_MATH YES)
	endif(BUILD_TL_MATH)
	#completar y ver que modulos se activan/desactivan en función de la activación de otros
endif(BUILD_TL_CORE)

# ----------------------------------------------------------------------------
#  Detect 3rd-party libraries
# ----------------------------------------------------------------------------

# ----------------------------------------------------------------------------
# BOOST
# ----------------------------------------------------------------------------
if(WITH_BOOST)
    #unset(Boost_LIBRARIES)
    #set(Boost_USE_STATIC             ON)
    set(Boost_USE_STATIC_LIBS        ON) # only find static libs
    set(Boost_USE_MULTITHREADED      ON)
    set(Boost_USE_STATIC_RUNTIME     OFF)

#TODO: Deber?a establecer la versi?n m?nima de Boost en find_package
    #find_package(Boost 1.50.0 COMPONENTS filesystem system REQUIRED)
    find_package(Boost COMPONENTS filesystem system REQUIRED)
    find_package(Boost COMPONENTS filesystem system unit_test_framework REQUIRED)
    if(Boost_FOUND)
        include_directories(${Boost_INCLUDE_DIRS})
        set(HAVE_BOOST YES)

    #if (WIN32)
        #    add_definitions(-DBOOST_ALL_NO_LIB)
        #    add_definitions(-DBOOST_ALL_DYN_LINK)
        #endif()

    else()
      set(HAVE_BOOST NO)
    endif()

endif()
# ----------------------------------------------------------------------------


# ----------------------------------------------------------------------------
# OpenCV  (optional)
# ----------------------------------------------------------------------------
if(WITH_OPENCV)

    find_package(OpenCV QUIET)

    if(NOT OPENCV_FOUND)
      set(HAVE_OPENCV NO)
    else()
      set(HAVE_OPENCV YES)
      include_directories(${OpenCV_INCLUDE_DIRS})
    endif()

endif(WITH_OPENCV)
# ----------------------------------------------------------------------------


# ----------------------------------------------------------------------------
# Eigen  (optional)
# ----------------------------------------------------------------------------
if(WITH_EIGEN)
 
  find_package(EIGEN3 QUIET)
  if(NOT EIGEN3_FOUND)
    set(HAVE_EIGEN NO)
  else()
    include_directories(${EIGEN3_INCLUDE_DIRS})
    set(HAVE_EIGEN YES)
  endif()

endif(WITH_EIGEN)
# ----------------------------------------------------------------------------


# ----------------------------------------------------------------------------
# GDAL (optional)
# ----------------------------------------------------------------------------
if(WITH_GDAL)
  find_package(GDAL QUIET)

  if(NOT GDAL_FOUND)
    set(HAVE_GDAL NO)
  else()
    set(HAVE_GDAL YES)
    include_directories(${GDAL_INCLUDE_DIR})
  endif()
endif()
# ----------------------------------------------------------------------------


# ----------------------------------------------------------------------------
# PROJ4 (optional)
# ----------------------------------------------------------------------------
if(WITH_PROJ4)
  find_package(PROJ4 QUIET)

  if(NOT PROJ4_FOUND)
    set(HAVE_PROJ4 NO)
  else()
    set(HAVE_PROJ4 YES)
    include_directories(${PROJ4_INCLUDE_DIRS})
	message(STATUS "  Include : ${PROJ4_INCLUDE_DIRS}")
    message(STATUS "  Library : ${PROJ4_LIBRARIES}")
  endif()
endif()
# ----------------------------------------------------------------------------


# ----------------------------------------------------------------------------
# LibRaw (optional)
# ----------------------------------------------------------------------------
if(WITH_LIBRAW)

  find_package(LIBRAW QUIET)

  if(NOT LIBRAW_FOUND)
    set(HAVE_LIBRAW NO)
  else()
    set(HAVE_LIBRAW YES)
    include_directories(${LIBRAW_INCLUDE_DIR})
  endif()

endif()
# ----------------------------------------------------------------------------


# ----------------------------------------------------------------------------
# EDSDK (optional)
# ----------------------------------------------------------------------------
if(WITH_EDSDK)

#TODO: Esto no esta bien hecho...

  find_path(EDSDK_INCLUDE_DIR EDSDK.h )

  find_library(EDSDK_LIBRARY NAMES EDSDK )

    if(NOT EDSDK_INCLUDE_DIR AND EDSDK_LIBRARY)
        set(HAVE_EDSDK NO)
    else()
        set(HAVE_EDSDK YES)
    include_directories(${EDSDK_INCLUDE_DIR})
    endif()
endif()
# ----------------------------------------------------------------------------

if(BUILD_TL_MATH)

if(WITH_LAPACKE)

  #SET(LAPACKE_LIBRARY "" CACHE FILE "Path to library LAPACKE")
  #SET(LAPACK_LIBRARY "" CACHE FILE "Path to library LAPACK")
  #SET(BLAS_LIBRARY "" CACHE FILE "Path to library BLAS")
  
  add_library(blas SHARED IMPORTED)
  set_property(TARGET blas PROPERTY IMPORTED_LOCATION ${PROJECT_SOURCE_DIR}/third_party/lapacke/lib/libblas.dll)
  set_property(TARGET blas PROPERTY IMPORTED_IMPLIB 	${PROJECT_SOURCE_DIR}/third_party/lapacke/lib/libblas.lib)
  
  add_library(lapack SHARED IMPORTED)
  set_property(TARGET lapack PROPERTY IMPORTED_LOCATION 	${PROJECT_SOURCE_DIR}/third_party/lapacke/lib/liblapack.dll)
  set_property(TARGET lapack PROPERTY IMPORTED_IMPLIB 	${PROJECT_SOURCE_DIR}/third_party/lapacke/lib/liblapack.lib)
  
  add_library(lapacke SHARED IMPORTED)
  set_property(TARGET lapacke PROPERTY IMPORTED_LOCATION 	${PROJECT_SOURCE_DIR}/third_party/lapacke/lib/liblapacke.dll)
  set_property(TARGET lapacke PROPERTY IMPORTED_IMPLIB 	${PROJECT_SOURCE_DIR}/third_party/lapacke/lib/liblapacke.lib)
  
  set(LAPACKE_INCLUDE_DIRS ${PROJECT_SOURCE_DIR}/third_party/lapacke/include)
  set(LAPACKE_LIBRARIES blas lapack lapacke)
    
  include_directories(${LAPACKE_INCLUDE_DIRS})
   
  ## install lapack and blas dependencies
  file(GLOB lapacke_windows_libs 	"${PROJECT_SOURCE_DIR}/third_party/lapacke/lib/*.lib")
  file(GLOB lapacke_windows_dll 	"${PROJECT_SOURCE_DIR}/third_party/lapacke/lib/*.dll")
  if(lapacke_windows_libs AND lapacke_windows_dll)
      set(LAPACKE_LIB_DIR "lib/lapacke")
  			install(FILES ${lapacke_windows_libs} ${lapacke_windows_dll}
  					    DESTINATION ${LAPACKE_LIB_DIR})
  endif()
              
  set(HAVE_LAPACKE YES)
else()
  set(HAVE_LAPACKE NO)
endif()
  
endif(BUILD_TL_MATH)

# --- VLD ---
### TODO: Revisar. por ahora s?lo 64 bits
#if(WITH_VLD AND MSVC)
#  message("Enabling Visual Leak Detector")
#  find_library(VLD_LIBRARY vld
#    HINTS "c:/Program Files (x86)/Visual Leak Detector/lib/Win64" "c:/Program Files/Visual Leak Detector/lib/Win64")
#  find_file(VLD_H vld.h
#    HINTS "c:/Program Files (x86)/Visual Leak Detector/include" "c:/Program Files/Visual Leak Detector/include")
#  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /FIwinsock2.h")
#  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} \"/FI${VLD_H}\"")
#  get_filename_component(VLD_LIBRARY_DIR "${VLD}" PATH)
#  link_directories("${VLD_LIBRARY_DIR}")
#
#  IF(VLD_INCLUDE_DIR)
#   IF (VLD_LIBRARY_VLD OR VLD_LIBRARY_VLDMT OR VLD_LIBRARY_VLDMTDLL)
#     ## do NOT add LIBRARIES because dependencies are handled through pragmas.
#     SET(VLD_FOUND TRUE)
#     MESSAGE("found VLD.")
#   ENDIF (VLD_LIBRARY_VLD OR VLD_LIBRARY_VLDMT OR VLD_LIBRARY_VLDMTDLL)
#  ENDIF(VLD_INCLUDE_DIR)
#
#  if(NOT VLD_FOUND)
#    set(HAVE_VLD NO)
#  else()
#    set(HAVE_VLD YES)
#  endif()
#endif()





# include del directorio principal
include_directories(${CMAKE_BUILD_DIR})



#cabecera con la configuración
configure_file("${PROJECT_SOURCE_DIR}/cmake/config_tl.h.in" "${PROJECT_BINARY_DIR}/config_tl.h")
include_directories("${PROJECT_BINARY_DIR}")

set(TARGETS_EXPORT_NAME "${PROJECT_NAME}Targets")
set(TidopLib_LIBRARIES)

#add_subdirectory(third_party)

# Directorio con el código fuente de los diferentes modulos
add_subdirectory(src)

add_subdirectory(apps)

# Directorio con la documentación
add_subdirectory(doc)

# Se incluye el directorio con los test
add_subdirectory(test)


###############################################################################################################
#                                                Instalación                                                  #
###############################################################################################################

# Fichero de configuracion de TidopLib
install(FILES ${PROJECT_BINARY_DIR}/config_tl.h DESTINATION include)

include(GNUInstallDirs)


#set(TIDOP_CONFIG_CMAKE_INSTALL_DIR "${CMAKE_INSTALL_LIBDIR}/cmake/${PROJECT_NAME}")
set(TIDOP_CONFIG_CMAKE_INSTALL_DIR "${CMAKE_INSTALL_PREFIX}/${TL_INSTALL_BINARIES_PREFIX}/lib/cmake/${PROJECT_NAME}")
set(TIDOP_CONFIG_VERSION_CMAKE_PATH "${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}ConfigVersion.cmake")
set(TIDOP_CONFIG_CMAKE_PATH "${CMAKE_CURRENT_BINARY_DIR}/TidopLibConfig.cmake")
set(TIDOP_NAMESPACE "${PROJECT_NAME}::")
set(INCLUDE_DIRS_TIDOP "${CMAKE_INSTALL_PREFIX}/${CMAKE_INSTALL_INCLUDEDIR}")

# Include module with fuction 'write_basic_package_version_file'
include(CMakePackageConfigHelpers)

write_basic_package_version_file(
  ${TIDOP_CONFIG_VERSION_CMAKE_PATH}
  VERSION ${TidopLib_VERSION}
  COMPATIBILITY AnyNewerVersion
)

configure_package_config_file(
    "cmake/TidopLibConfig.cmake.in"
    ${TIDOP_CONFIG_CMAKE_PATH}
    INSTALL_DESTINATION "${TIDOP_CONFIG_CMAKE_INSTALL_DIR}"
)


install(
    FILES "${TIDOP_CONFIG_CMAKE_PATH}" "${TIDOP_CONFIG_VERSION_CMAKE_PATH}"
    DESTINATION "${TIDOP_CONFIG_CMAKE_INSTALL_DIR}"
)

install(
    EXPORT "${TARGETS_EXPORT_NAME}"
    NAMESPACE "${TIDOP_NAMESPACE}"
    DESTINATION "${TIDOP_CONFIG_CMAKE_INSTALL_DIR}"
)
