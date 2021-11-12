##########################################################################
#                                                                        #
# Copyright (C) 2021 by Tidop Research Group                             #
# Copyright (C) 2021 by Esteban Ruiz de Oña Crespo                       #
#                                                                        #
# This file is part of TidopLib                                          #
#                                                                        #
# TidopLib is free software: you can redistribute it and/or modify       #
# it under the terms of the GNU Lesser General Public License as         #
# published by the Free Software Foundation, either version 3 of the     #
# License, or (at your option) any later version.                        #
#                                                                        #
# TidopLib is distributed in the hope that it will be useful,            #
# but WITHOUT ANY WARRANTY; without even the implied warranty of         #
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the          #
# GNU Lesser General Public License for more details.                    #
#                                                                        #
# You should have received a copy of the GNU Lesser General Public       #
# License along with Foobar. If not, see <http://www.gnu.org/licenses/>. #
#                                                                        #
# @license LGPL-3.0 <https://www.gnu.org/licenses/lgpl-3.0.html>         #
#                                                                        #
##########################################################################


# Versión mínima soportada de CMake
cmake_minimum_required(VERSION 3.5.0 FATAL_ERROR)

# Nombre del projecto y versión
project(TidopLib
        VERSION 2.0.0
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
  # Se configura como libreria dinámica
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
#OPTION (WITH_LAPACKE "Include LAPACKE"                        OFF)
OPTION (WITH_OpenBLAS "Include OpenBLAS"                      OFF)
OPTION (WITH_OPENMP   "Include OpenMP"                        OFF)

# ----------------------------------------------------------------------------
#  Modulos TidopLib
# ----------------------------------------------------------------------------


unset(HAVE_TL_CORE CACHE)
unset(HAVE_TL_MATH CACHE)
unset(HAVE_TL_GEOMETRY CACHE)
unset(HAVE_TL_IMG CACHE)
unset(HAVE_TL_IMG_PROCESS CACHE)
unset(HAVE_TL_FEAT_MATCH CACHE)
unset(HAVE_TL_GRAPHIC CACHE)
unset(HAVE_TL_GEOSPATIAL CACHE)
unset(HAVE_TL_VECT CACHE)
unset(HAVE_TL_EXPERIMENTAL CACHE)

# if(BUILD_TL_CORE)
    # set(HAVE_TL_CORE YES)
	# if(BUILD_TL_MATH)
	    # set(HAVE_TL_MATH YES)
	# endif(BUILD_TL_MATH)
	# #completar y ver que modulos se activan/desactivan en función de la activación de otros
# endif(BUILD_TL_CORE)

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
 
  find_package(Eigen3 QUIET NO_MODULE)
  if(NOT Eigen3_FOUND)
    set(HAVE_EIGEN NO)
  else()
    #include_directories(${Eigen3_INCLUDE_DIRS})
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
    message(STATUS "  GDAL not found")
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

if(WITH_OpenBLAS)

  # #SET(LAPACKE_LIBRARY "" CACHE FILE "Path to library LAPACKE")
  # #SET(LAPACK_LIBRARY "" CACHE FILE "Path to library LAPACK")
  # #SET(BLAS_LIBRARY "" CACHE FILE "Path to library BLAS")
  
  # add_library(blas SHARED IMPORTED)
  # set_property(TARGET blas PROPERTY IMPORTED_LOCATION ${PROJECT_SOURCE_DIR}/third_party/lapacke/lib/${TL_ARCH}/libblas.dll)
  # set_property(TARGET blas PROPERTY IMPORTED_IMPLIB 	${PROJECT_SOURCE_DIR}/third_party/lapacke/lib/${TL_ARCH}/libblas.lib)
  
  # add_library(lapack SHARED IMPORTED)
  # set_property(TARGET lapack PROPERTY IMPORTED_LOCATION 	${PROJECT_SOURCE_DIR}/third_party/lapacke/lib/${TL_ARCH}/liblapack.dll)
  # set_property(TARGET lapack PROPERTY IMPORTED_IMPLIB 	${PROJECT_SOURCE_DIR}/third_party/lapacke/lib/${TL_ARCH}/liblapack.lib)
  
  # add_library(lapacke SHARED IMPORTED)
  # set_property(TARGET lapacke PROPERTY IMPORTED_LOCATION 	${PROJECT_SOURCE_DIR}/third_party/lapacke/lib/${TL_ARCH}/liblapacke.dll)
  # set_property(TARGET lapacke PROPERTY IMPORTED_IMPLIB 	${PROJECT_SOURCE_DIR}/third_party/lapacke/lib/${TL_ARCH}/liblapacke.lib)
  
  # set(LAPACKE_INCLUDE_DIRS ${PROJECT_SOURCE_DIR}/third_party/lapacke/include)
  # set(LAPACKE_LIBRARIES blas lapack lapacke)
    
  # include_directories(${LAPACKE_INCLUDE_DIRS})
   
  # ## install lapack and blas dependencies
  # file(GLOB lapacke_windows_libs 	"${PROJECT_SOURCE_DIR}/third_party/lapacke/lib/${TL_ARCH}/*.lib")
  # file(GLOB lapacke_windows_dll 	"${PROJECT_SOURCE_DIR}/third_party/lapacke/lib/${TL_ARCH}/*.dll")
  # if(lapacke_windows_libs AND lapacke_windows_dll)
      # set(LAPACKE_LIB_DIR "lib/lapacke")
  			# install(FILES ${lapacke_windows_libs} ${lapacke_windows_dll}
  					    # DESTINATION ${LAPACKE_LIB_DIR})
  # endif()
        
  find_package(OpenBLAS QUIET)
  
  if(NOT OpenBLAS_FOUND)
    set(HAVE_OPENBLAS NO)
  else()
    set(HAVE_OPENBLAS YES)
    include_directories(${OpenBLAS_INCLUDE_DIRS})
  endif()
  
endif()
  
  # if(WITH_OPENBLAS)
    # set(HAVE_OPENBLAS YES)
  # else()
    # set(HAVE_OPENBLAS NO)
  # endif()
  
  
endif(BUILD_TL_MATH)

# ----------------------------------------------------------------------------
# OMP (optional)
# ----------------------------------------------------------------------------
if(WITH_OPENMP)

  find_package(OpenMP)
  
  if(OpenMP_FOUND)
    set(HAVE_OPENMP YES)
  else()
    set(HAVE_OPENMP NO)
  endif()
  
endif()
# ----------------------------------------------------------------------------


# include del directorio principal
include_directories(${CMAKE_BUILD_DIR})




include_directories("${PROJECT_BINARY_DIR}")

set(TARGETS_EXPORT_NAME "${PROJECT_NAME}Targets")
set(TidopLib_LIBRARIES)

add_subdirectory(third_party)

# Directorio con el código fuente de los diferentes modulos
add_subdirectory(src)

add_subdirectory(apps)

# Directorio con la documentación
add_subdirectory(doc)

# Se incluye el directorio con los test
add_subdirectory(test)


#cabecera con la configuración
configure_file("${PROJECT_SOURCE_DIR}/cmake/config_tl.h.in" "${PROJECT_BINARY_DIR}/config_tl.h")


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
