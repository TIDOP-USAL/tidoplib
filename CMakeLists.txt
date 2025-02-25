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
# License along with TidopLib. If not, see <http://www.gnu.org/licenses>.#
#                                                                        #
# @license LGPL-3.0 <https://www.gnu.org/licenses/lgpl-3.0.html>         #
#                                                                        #
##########################################################################

cmake_minimum_required(VERSION 3.19)

message(STATUS "\nCMake Version ${CMAKE_MAJOR_VERSION}.${CMAKE_MINOR_VERSION}\n")

if(POLICY CMP0146) 
cmake_policy(SET CMP0146 OLD) 
endif()

project(TidopLib
        VERSION 3.1.0
        DESCRIPTION "TidopLib is an open-source C++ library developed by Tidop Research Group"
        HOMEPAGE_URL "http://tidop.usal.es"
        LANGUAGES CXX)

message(STATUS "\n${PROJECT_NAME} ${TidopLib_VERSION}")
message(STATUS ${TidopLib_DESCRIPTION})
message(STATUS ${TidopLib_HOMEPAGE_URL})


if(CMAKE_SIZEOF_VOID_P EQUAL 8)
  set(TL_ARCH x64)
else()
  set(TL_ARCH x86)
endif()



# General configuration


# C++ standar


set(cxx_standard_values "")

if (${CMAKE_VERSION} VERSION_GREATER_EQUAL 3.8 AND "cxx_std_11" IN_LIST CMAKE_CXX_COMPILE_FEATURES)
list (APPEND cxx_standard_values C++11)
endif()

if (${CMAKE_VERSION} VERSION_GREATER_EQUAL 3.8 AND "cxx_std_14" IN_LIST CMAKE_CXX_COMPILE_FEATURES)
list (APPEND cxx_standard_values C++14)
endif()

if(${CMAKE_VERSION} VERSION_GREATER_EQUAL 3.8 AND "cxx_std_17" IN_LIST CMAKE_CXX_COMPILE_FEATURES)
list (APPEND cxx_standard_values C++17)
endif()

if(${CMAKE_VERSION} VERSION_GREATER_EQUAL 3.12 AND "cxx_std_20" IN_LIST CMAKE_CXX_COMPILE_FEATURES)
list (APPEND cxx_standard_values C++20)
endif()

#if(${CMAKE_VERSION} VERSION_GREATER_EQUAL 3.20 AND "cxx_std_23" IN_LIST CMAKE_CXX_COMPILE_FEATURES)
#list (APPEND cxx_standard_values C++23)
#endif()
#
#if(${CMAKE_VERSION} VERSION_GREATER_EQUAL 3.25 AND "cxx_std_26" IN_LIST CMAKE_CXX_COMPILE_FEATURES)
#list (APPEND cxx_standard_values C++26)
#endif()

set(TIDOPLIB_CXX_STANDARD "C++14" CACHE STRING "")
set_property(CACHE TIDOPLIB_CXX_STANDARD PROPERTY STRINGS ${cxx_standard_values})

if(${TIDOPLIB_CXX_STANDARD} STREQUAL "C++11")
  set(CMAKE_CXX_STANDARD 11)
elseif(${TIDOPLIB_CXX_STANDARD} STREQUAL "C++14")
  set(CMAKE_CXX_STANDARD 14)
elseif(${TIDOPLIB_CXX_STANDARD} STREQUAL "C++17")
  set(CMAKE_CXX_STANDARD 17)
elseif(${TIDOPLIB_CXX_STANDARD} STREQUAL "C++20")
  set(CMAKE_CXX_STANDARD 20)
elseif(${TIDOPLIB_CXX_STANDARD} STREQUAL "C++23")
  set(CMAKE_CXX_STANDARD 23)
else()

endif()

set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)


set_property(GLOBAL PROPERTY USE_FOLDERS ON)


# Establece un sufijo para las librerias de debug. De esta forma se pueden
# instalar en el mismo directorio
set(CMAKE_DEBUG_POSTFIX d)

# Macro para depuración
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -D_DEBUG")





# Directivas del preprocesador
add_definitions(-DUNICODE)

if(MSVC)

  if(MSVC_VERSION EQUAL 1900)
    set(TL_RUNTIME vc14)
  elseif(MSVC_VERSION MATCHES "^191[0-9]$")
    set(TL_RUNTIME vc15)
  elseif(MSVC_VERSION MATCHES "^192[0-9]$")
    set(TL_RUNTIME vc16)
  elseif(MSVC_VERSION MATCHES "^193[0-9]$")
    set(TL_RUNTIME vc17)
  else()
    message(WARNING "TidopLib does not recognize MSVC_VERSION \"${MSVC_VERSION}\". Cannot set TL_RUNTIME")
  endif()

elseif(MINGW)

  set(TL_RUNTIME mingw)

endif()


# Compile options 

add_compile_options("$<$<C_COMPILER_ID:MSVC>:/utf-8>")
add_compile_options("$<$<CXX_COMPILER_ID:MSVC>:/utf-8>")

if(MSVC)
    add_compile_options("/W3" "/EHsc" "/permissive-" "-D_SCL_SECURE_NO_WARNINGS" "-D_CRT_SECURE_NO_DEPRECATE")
else()
    add_compile_options("-Wall" "-Wextra" "-Wpedantic" "-ftrack-macro-expansion=0" "-fno-diagnostics-show-caret")
endif()



if(WIN32 AND CMAKE_HOST_SYSTEM_NAME MATCHES Windows)
  if(DEFINED TL_RUNTIME AND DEFINED TL_ARCH)
    set(TL_INSTALL_BINARIES_PREFIX "${TL_ARCH}/${TL_RUNTIME}/")
  else()
    message(STATUS "Can't detect runtime and/or arch")
    set(TL_INSTALL_BINARIES_PREFIX .)
  endif()
endif()



# Por defecto se establece el tipo de librería estática
set (LIB_TYPE STATIC)





set(CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake" ${CMAKE_MODULE_PATH})


include(CMakeDependentOption)
include(CompilerInfo)
include(OperatingSystemInfo)
include(TidopLibSIMDSupport)
include(TidopUtils)

##########################################################################
#  Modulos TidopLib                                                      #
#------------------------------------------------------------------------#
#                                                                        #
# Activación/desactivación de los diferentes modulos de la libreria      #
# Configuración de la librería                                           #
#                                                                        #
##########################################################################



message(STATUS "\nTidopLib config:\n")

option(BUILD_TL_CORE         "Include core module in TidopLib build"         ON )
cmake_dependent_option(BUILD_TL_MATH "Include math module in TidopLib build" ON "BUILD_TL_CORE" OFF)
cmake_dependent_option(BUILD_TL_GEOMETRY "Include geometry module in TidopLib build" ON "BUILD_TL_CORE;BUILD_TL_MATH" OFF)
cmake_dependent_option(BUILD_TL_GRAPHIC "Include graphic module in TidopLib build" ON "BUILD_TL_CORE;BUILD_TL_GEOMETRY" OFF)
cmake_dependent_option(BUILD_TL_IMG "Include image module in TidopLib build" ON "BUILD_TL_CORE;BUILD_TL_MATH;BUILD_TL_GEOMETRY;BUILD_TL_GRAPHIC" OFF)
cmake_dependent_option(BUILD_TL_IMG_PROCESS "Include imgprocess module in TidopLib build" ON "BUILD_TL_CORE;BUILD_TL_GRAPHIC" OFF)
cmake_dependent_option(BUILD_TL_FEAT_MATCH "Include featmatch module in TidopLib build" ON "BUILD_TL_CORE;BUILD_TL_GEOMETRY" OFF)
cmake_dependent_option(BUILD_TL_VECT "Include vector module in TidopLib build" ON "BUILD_TL_CORE;BUILD_TL_GRAPHIC" OFF)
cmake_dependent_option(BUILD_TL_GEOSPATIAL "Include geospatial module in TidopLib build" ON "BUILD_TL_CORE;BUILD_TL_MATH;BUILD_TL_GEOMETRY;BUILD_TL_IMG;BUILD_TL_VECT" OFF)
cmake_dependent_option(BUILD_TL_VIEWER "Include viewer module in TidopLib build" OFF "BUILD_TL_CORE;BUILD_TL_GEOMETRY" OFF)
cmake_dependent_option(BUILD_TL_GEOTOOLS "Include geotools module in TidopLib build" ON "BUILD_TL_CORE;BUILD_TL_MATH;BUILD_TL_GEOMETRY" OFF)
cmake_dependent_option(BUILD_TL_PCTOOLS "Include pctools module in TidopLib build" ON "BUILD_TL_CORE;BUILD_TL_GEOTOOLS;BUILD_TL_GEOMETRY" OFF)


# Activación/desactivación de otros componentes de la libreria

OPTION (BUILD_APPS                    "Build Apps"             OFF)
OPTION (BUILD_SHARED_LIBS             "Build Shared Libraries" OFF)
OPTION (BUILD_TEST                    "Build test"             OFF)
OPTION (BUILD_DOC                     "Build documentation"    ON)

# Configuración de la librería

OPTION (TIDOPLIB_USE_SIMD_INTRINSICS    "Use SIMD intrinsics"               ON )
OPTION (TIDOPLIB_DEPRECATED_METHODS     "Use deprecated methods and class"  OFF)

OPTION (TIDOPLIB_WARNING_DEPRECATED_METHODS     "Active deprecated method warning"  OFF)
OPTION (TIDOPLIB_WARNING_TODO                   "Active TODO warning"               OFF)

if (TIDOPLIB_USE_SIMD_INTRINSICS)

    find_supported_architectures()
    
    message(STATUS "  [TidopLib] Supported architectures: ${SUPPORTED_ARCHITECTURES}SSE")

    set(TIDOPLIB_SIMD "SSE2" CACHE STRING "")
    set_property(CACHE TIDOPLIB_SIMD PROPERTY STRINGS 
                 ${SUPPORTED_ARCHITECTURES})

    set(TL_HAVE_SIMD_INTRINSICS TRUE)

    message(STATUS "  [TidopLib] Build with SIMD intrinsics [${TIDOPLIB_SIMD}]")

    set_architecture(${TIDOPLIB_SIMD})

else(TIDOPLIB_USE_SIMD_INTRINSICS)
    unset(TIDOPLIB_SIMD CACHE)
endif(TIDOPLIB_USE_SIMD_INTRINSICS)

if(TIDOPLIB_DEPRECATED_METHODS)
  set(TL_ENABLE_DEPRECATED_METHODS YES)
  message(STATUS "  [TidopLib] Enable deprecated methods and class")
else()
  set(TL_ENABLE_DEPRECATED_METHODS NO)
  message(STATUS "  [TidopLib] Disable deprecated methods and class")
endif()

if(TIDOPLIB_WARNING_DEPRECATED_METHODS)
  set(TL_WARNING_DEPRECATED_METHOD YES)
  message(STATUS "  [TidopLib] Enable deprecated method Warning")
else()
  set(TL_WARNING_DEPRECATED_METHOD NO)
  message(STATUS "  [TidopLib] Disable deprecated method Warning")
endif()

if(TIDOPLIB_WARNING_TODO)
  set(TL_WARNING_TODO YES)
  message(STATUS "  [TidopLib] Enable TODO Warnings")
else()
  set(TL_WARNING_TODO NO)
  message(STATUS "  [TidopLib] Disable TODO Warnings")
endif()

if(BUILD_APPS)
  message(STATUS "  [TidopLib] Build apps")
else()
  message(STATUS "  [TidopLib] Build apps disabled")
endif()

if (BUILD_SHARED_LIBS)
  # Se configura como libreria dinámica
  set (LIB_TYPE SHARED)

  if (MSVC)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /wd4251")
    #TODO: Probar
    #set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON)
    message(STATUS "  [TidopLib] Build with shared libraries.")
  endif(MSVC)
else()
  message(STATUS "  [TidopLib] Build with static libraries")
endif (BUILD_SHARED_LIBS)


if(BUILD_TEST)
  enable_testing()
  message(STATUS "  [TidopLib] Build testing")
else()
  message(STATUS "  [TidopLib] Build testing disabled")
endif()

if(BUILD_DOC)
  message(STATUS "  [TidopLib] Build documentation")
else()
  message(STATUS "  [TidopLib] Build documentation disabled")
endif()


##########################################################################
#  Librerias externas                                                    #
#------------------------------------------------------------------------#
#                                                                        #
# - Sólo es obligatoria Boost.                                           #
# - Según las dependencias incluidas las funcionalidades de la librería  #
#   variarán.                                                            #
##########################################################################

set(CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake/modules" ${CMAKE_MODULE_PATH})

#OPTION (WITH_BOOST  "Include Boost support"                   ON)
OPTION (WITH_OPENCV "Include OpenCV support"                  ON)
OPTION (WITH_GDAL   "Include GDAL Support"                    ON)
OPTION (WITH_PROJ  "Include PROJ Support"                     ON)
cmake_dependent_option(WITH_LIBRAW "Include LibRaw Support" OFF "BUILD_TL_IMG" OFF)
cmake_dependent_option(WITH_EDSDK "Include EDSDK Support" OFF "BUILD_TL_IMG" OFF)
OPTION (WITH_PPL    "Include PPL Support"                     OFF)
OPTION (WITH_VLD    "Include Visual Leak Detector Support"    OFF)
cmake_dependent_option(WITH_OpenBLAS "Include OpenBLAS" OFF "BUILD_TL_MATH" OFF)
OPTION (WITH_OPENMP   "Include OpenMP"                        OFF)
OPTION (WITH_CUDA     "Include CUDA"                        OFF)



unset(TL_HAVE_CORE CACHE)
unset(TL_HAVE_MATH CACHE)
unset(TL_HAVE_GEOMETRY CACHE)
unset(TL_HAVE_IMG CACHE)
unset(TL_HAVE_IMG_PROCESS CACHE)
unset(TL_HAVE_FEAT_MATCH CACHE)
unset(TL_HAVE_GRAPHIC CACHE)
unset(TL_HAVE_GEOSPATIAL CACHE)
unset(TL_HAVE_VECT CACHE)
unset(TL_HAVE_EXPERIMENTAL CACHE)
unset(TL_HAVE_GEOTOOLS CACHE)
unset(TL_HAVE_PCTOOLS CACHE)


message(STATUS "\nDependency loading:\n")


##########################################################################
# fmt (REQUIRED C++ Version < 20)                                        #
##########################################################################


if (CMAKE_CXX_STANDARD GREATER_EQUAL 20 
    AND ((CMAKE_CXX_COMPILER_ID STREQUAL "MSVC" AND MSVC_TOOLSET_VERSION GREATER_EQUAL 143) 
         OR (CMAKE_CXX_COMPILER_ID STREQUAL "GNU" AND CMAKE_CXX_COMPILER_VERSION GREATER_EQUAL 13.1)))
    set(TL_HAVE_FMT NO)
    add_compile_definitions("FORMAT_NAMESPACE=std::")
else()
    find_package(fmt REQUIRED)

    if(fmt_FOUND)
        message(STATUS "  fmt version: ${fmt_VERSION}")
        set(TL_HAVE_FMT YES)
        add_compile_definitions(TL_HAVE_FMT)
        add_compile_definitions("FORMAT_NAMESPACE=fmt::")
    else()
        set(TL_HAVE_FMT NO)
        add_compile_definitions("FORMAT_NAMESPACE=std::")
    endif()
endif()

##########################################################################
# BOOST (REQUIRED C++ Version < 17)                                      #
##########################################################################

if(CMAKE_CXX_STANDARD LESS 17 OR BUILD_TEST)

    set(Boost_USE_STATIC_LIBS        ON) # only find static libs
    set(Boost_USE_MULTITHREADED      ON)
    set(Boost_USE_STATIC_RUNTIME     OFF)
       
    set(CMAKE_MESSAGE_LOG_LEVEL "ERROR")
    find_package(Boost 1.50 COMPONENTS REQUIRED
                filesystem 
                system 
                unit_test_framework)
    set(CMAKE_MESSAGE_LOG_LEVEL "STATUS")

    if(Boost_FOUND)
        message(STATUS "  Boost version: ${Boost_VERSION}")
        include_directories(${Boost_INCLUDE_DIRS})
        set(TL_HAVE_BOOST YES)
    else()
        set(TL_HAVE_BOOST NO)
    endif()

endif()


if(BUILD_TL_VIEWER)

    option(USE_QT6 "Use Qt6 if available" ON)

    set(CMAKE_MESSAGE_LOG_LEVEL "ERROR")

	##########################################################################
	# GLEW                                                      			 #
	##########################################################################

	find_package(GLEW REQUIRED)

    ##########################################################################
	# Assimp                                                     			 #
	##########################################################################

    find_package(assimp REQUIRED)

	##########################################################################
	# Qt                                                                     #
	##########################################################################

    if (USE_QT6)

	    find_package(Qt6 REQUIRED 
				     COMPONENTS 
				     Core
				     Concurrent
				     OpenGL  
				     Widgets
				     OpenGLWidgets)

        set(QT_VERSION 6)

	    if (Qt6_POSITION_INDEPENDENT_CODE)
	      SET(CMAKE_POSITION_INDEPENDENT_CODE ON)
	    endif()

    else()

	    find_package(Qt5 REQUIRED 
				     COMPONENTS 
				     Core
				     Concurrent
				     OpenGL  
				     Widgets
				     OpenGLExtensions)
        set(QT_VERSION 5)

    endif()

    set(CMAKE_MESSAGE_LOG_LEVEL "STATUS")	



	if(QT_VERSION)

        if (USE_QT6)
		    message(STATUS "  Qt version: ${Qt6_VERSION}")
        else()
            message(STATUS "  Qt version: ${Qt5_VERSION}")
        endif()

		set(CMAKE_INCLUDE_CURRENT_DIR ON) # Find includes in corresponding build directories
		set(CMAKE_AUTOMOC ON) # Instruct CMake to run moc automatically when needed
		set(CMAKE_AUTOUIC ON) # Create code from a list of Qt designer ui files
		set(CMAKE_AUTORCC ON) # Create code from a list of Qt designer ui files  
    else()
        message(STATUS "  Qt not found")
	endif()

    
    ##########################################################################
	# LASzip                                                                 #
	##########################################################################
    # find_package

    ##########################################################################
	# LASLib                                                                 #
	##########################################################################
    set(CMAKE_MESSAGE_LOG_LEVEL "ERROR")
    find_package(LASLib REQUIRED)
    set(CMAKE_MESSAGE_LOG_LEVEL "STATUS")

endif(BUILD_TL_VIEWER)


##########################################################################
# OpenCV  (optional)                                                     #
##########################################################################

if(WITH_OPENCV)

    set(CMAKE_MESSAGE_LOG_LEVEL "ERROR")
    find_package(OpenCV QUIET)
    set(CMAKE_MESSAGE_LOG_LEVEL "STATUS")

    if(NOT OPENCV_FOUND)
      set(TL_HAVE_OPENCV NO)
    else()
      message(STATUS "  OpenCV version: ${OpenCV_VERSION}")
      set(TL_HAVE_OPENCV YES)
    endif()

endif(WITH_OPENCV)



##########################################################################
# GDAL (optional)                                                        #
##########################################################################



if(WITH_GDAL) 
  find_package(GDAL QUIET)

  if(NOT GDAL_FOUND)
    set(TL_HAVE_GDAL NO)
    message(STATUS "  GDAL not found")
  else()
    message(STATUS "  GDAL version: ${GDAL_VERSION}")
    set(TL_HAVE_GDAL YES)
  endif()

endif(WITH_GDAL)


##########################################################################
# PROJ (optional)                                                       #
##########################################################################

if(WITH_PROJ)

  set(CMAKE_MESSAGE_LOG_LEVEL "ERROR")
  find_package(PROJ QUIET)
  set(CMAKE_MESSAGE_LOG_LEVEL "STATUS")

  if(NOT PROJ_FOUND)
    unset(PROJ_DIR CACHE)
    set(TL_HAVE_PROJ NO)
	set(CMAKE_MESSAGE_LOG_LEVEL "ERROR")
    find_package(PROJ4 QUIET)
    set(CMAKE_MESSAGE_LOG_LEVEL "STATUS")
    if(NOT PROJ4_FOUND)
      set(TL_HAVE_PROJ4 NO)
    else()
      set(TL_HAVE_PROJ4 YES)
      include_directories(${PROJ4_INCLUDE_DIRS})
      message(STATUS "  PROJ version: ${PROJ4_VERSION}")
    endif()
  else()
    set(TL_HAVE_PROJ YES)
    #include_directories(${PROJ_INCLUDE_DIRS})
    message(STATUS "  PROJ version: ${PROJ_VERSION}")
  endif()
endif(WITH_PROJ)

##########################################################################
# LibRaw (optional)                                                      #
##########################################################################

if(WITH_LIBRAW)

  find_package(LIBRAW QUIET)

  if(NOT LIBRAW_FOUND)
    set(TL_HAVE_LIBRAW NO)
  else()
    set(TL_HAVE_LIBRAW YES)
    include_directories(${LIBRAW_INCLUDE_DIR})
  endif()

endif(WITH_LIBRAW)


##########################################################################
# EDSDK (optional)                                                       #
##########################################################################

if(WITH_EDSDK)

#TODO: Esto no esta bien hecho...

  find_path(EDSDK_INCLUDE_DIR EDSDK.h )

  find_library(EDSDK_LIBRARY NAMES EDSDK )

  if(NOT EDSDK_INCLUDE_DIR AND EDSDK_LIBRARY)
    set(TL_HAVE_EDSDK NO)
  else()
    set(TL_HAVE_EDSDK YES)
    include_directories(${EDSDK_INCLUDE_DIR})
  endif()
  
endif()


#if(BUILD_TL_MATH)


##########################################################################
# OpenBLAS (optional)                                                    #
##########################################################################

if(WITH_OpenBLAS)
       
  find_package(OpenBLAS QUIET)
  
  if(NOT OpenBLAS_FOUND)
    set(TL_HAVE_OPENBLAS NO)
  else()
    set(TL_HAVE_OPENBLAS YES)
    include_directories(${OpenBLAS_INCLUDE_DIRS})
  endif()
  
endif(WITH_OpenBLAS)
  
#endif(BUILD_TL_MATH)


##########################################################################
# OpenMP (optional)                                                      #
##########################################################################

if(WITH_OPENMP)

  find_package(OpenMP)
  
  if(OpenMP_FOUND)
    set(TL_HAVE_OPENMP YES)
    message(STATUS "  OpenMP ${OpenMP_VERSION} found")
  else()
    set(TL_HAVE_OPENMP NO)
  endif()
  
endif()


##########################################################################
# Visual Leak Detector (optional)                                        #
##########################################################################

if(WITH_VLD)

    find_package(VLD)

    if(NOT VLD_FOUND)
        set(TL_HAVE_VLD NO)
    else()
        include_directories(${VLD_INCLUDE_DIR})
        set(TL_HAVE_VLD YES)
    endif()

endif(WITH_VLD)

##########################################################################
# CUDA (optional)                                                        #
##########################################################################

if(WITH_CUDA)

    set(CMAKE_MESSAGE_LOG_LEVEL "ERROR")
    find_package(CUDA)
    set(CMAKE_MESSAGE_LOG_LEVEL "STATUS")

    if(NOT CUDA_FOUND)
        set(TL_HAVE_CUDA NO)
         message(STATUS "  Cuda not found")
    else()
        message(STATUS "  Cuda version: ${CUDA_VERSION}")
        include_directories(${CUDA_INCLUDE_DIRS})
        set(TL_HAVE_CUDA YES)
    endif()

endif(WITH_CUDA)

##########################################################################



# Directorio con la documentación
add_subdirectory(doc)


if(MSVC)

# TidopLib enviroment file
set(TL_ENV_FILE "${CMAKE_SOURCE_DIR}/.env")

if(NOT EXISTS ${TL_ENV_FILE})
    configure_file("${CMAKE_SOURCE_DIR}/.env.in" ${TL_ENV_FILE} COPYONLY)
endif()

# Read the .env file
read_env_file(${TL_ENV_FILE} ENV_VARS_LIST)

endif(MSVC)


message(STATUS "\nTidop modules:\n")

include_directories(${CMAKE_BUILD_DIR})
include_directories("${PROJECT_BINARY_DIR}")

set(TARGETS_EXPORT_NAME "${PROJECT_NAME}Targets")
set(TidopLib_LIBRARIES)

add_subdirectory(third_party)
add_subdirectory(src)
add_subdirectory(apps)
add_subdirectory(test)

message("\n")

# TidopLib config file
configure_file("cmake/config.h.in" "${PROJECT_BINARY_DIR}/tidop/config.h")


##########################################################################
# Install                                                                #
##########################################################################

include(GNUInstallDirs)

# TidopLib config file
install(FILES ${PROJECT_BINARY_DIR}/tidop/config.h 
        DESTINATION include/tidop)




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


#include(cmake/CMakeCPack.cmake)
