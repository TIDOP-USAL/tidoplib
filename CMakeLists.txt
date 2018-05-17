# versión mínima soportada
cmake_minimum_required(VERSION 3.6.0 FATAL_ERROR)

# Nombre del proyecto
project(TidopLib CXX)

set_property(GLOBAL PROPERTY USE_FOLDERS ON)


# Sufijo para librerias de debug
set( CMAKE_DEBUG_POSTFIX d )

# Tipos de configuración
set(CMAKE_CONFIGURATION_TYPES Release;Debug)

# Nivel de warning
set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /W3")

#/EH (Modelo de control de excepciones)
#/EHsc captura solo las excepciones de C++ e indica al compilador que suponga que las funciones declaradas como extern "C" jamás inician una excepción de C++.
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /EHsc")

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -D_SCL_SECURE_NO_WARNINGS -D_CRT_SECURE_NO_DEPRECATE")

#set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/../bin")
#set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/../bin")
#set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/../bin")

#set_target_properties(PROPERTIES RUNTIME_OUTPUT_DIRECTORY)
	
# Se permite elegir entre libreria estatica o dinamica
OPTION (BUILD_SHARED_LIBS "Build Shared Libraries" OFF)

# Set the LIB_TYPE variable to STATIC
set (LIB_TYPE STATIC)
if (BUILD_SHARED_LIBS)
  # User wants to build Dynamic Libraries, so change the LIB_TYPE variable to CMake keyword 'SHARED'
  set (LIB_TYPE SHARED)
endif (BUILD_SHARED_LIBS)

# Activación desactivación de los diferentes modulos de la libreria
OPTION (BUILD_TL_CORE        "Include core module in TidopLib build"       ON)
OPTION (BUILD_TL_GEOMETRY    "Include geom module in TidopLib build"       ON)
OPTION (BUILD_TL_IMG         "Include img module in TidopLib build"        ON)
OPTION (BUILD_TL_IMG_PROCESS "Include imgprocess module in TidopLib build" ON)
OPTION (BUILD_TL_GRAPHIC     "Include graphic module in TidopLib build"    ON)
OPTION (BUILD_TL_GEOSPATIAL  "Include geospatial module in TidopLib build" ON)

# Activación desactivación de otros componentes de la libreria
OPTION (BUILD_TEST            "Include test"          OFF)
OPTION (BUILD_EXAMPLES        "Include examples"      OFF)		
OPTION (BUILD_DOC             "Build documentation"   OFF)

OPTION (BUILD_Message_handler "Active message handle" ON)

if(BUILD_Message_handler)
  set(TL_MESSAGE_HANDLER YES)
else()
  set(TL_MESSAGE_HANDLER NO)
endif()

if(BUILD_TEST)
  enable_testing() #CMake predefined targets
endif()

# Librerias externas
OPTION (WITH_BOOST  "Include Boost support"                   ON)
OPTION (WITH_OPENCV "Include OpenCV support"                  OFF)
OPTION (WITH_EIGEN  "Include eigen support"                   OFF)
OPTION (WITH_GDAL   "Include GDAL Support"                    OFF)
OPTION (WITH_PROJ4  "Include PROJ.4 Support"                  OFF)
OPTION (WITH_LIBRAW "Include LibRaw Support"                  OFF)
OPTION (WITH_EDSDK  "Include EDSDK Support"                   OFF)
OPTION (WITH_PPL    "Include PPL Support"                     OFF)
OPTION (WITH_VLD    "Include Visual Leak Detector Support"    OFF)

# ----------------------------------------------------------------------------
#  Detect 3rd-party libraries
# ----------------------------------------------------------------------------

#... OpenCV lo hace en este archivo. Lo copio aqui debajo
#include(cmake/OpenCVFindLibsPerf.cmake)

# --- BOOST ---
if(WITH_BOOST)

  set(Boost_USE_STATIC_LIBS        ON ) # only find static libs
  set(Boost_USE_MULTITHREADED      ON)
  set(Boost_USE_STATIC_RUNTIME    OFF)
  
#TODO: Debería establecer la versión mínima de Boost en find_package
  find_package(Boost 1.50.0 COMPONENTS filesystem system REQUIRED)

  if(Boost_FOUND)
    include_directories(${Boost_INCLUDE_DIRS})
	  set(HAVE_BOOST YES)
##TODO: Comprobar esto
#    if (WIN32)
#
#      add_definitions(-DBOOST_ALL_NO_LIB)
#      add_definitions(-DBOOST_ALL_DYN_LINK)
#    endif()
	else()
	  set(HAVE_BOOST NO)
  endif()

endif()


# --- OpenCV ---
if(WITH_OPENCV)
    find_package(OPENCV QUIET)

    if(NOT OPENCV_FOUND)
      set(HAVE_OPENCV NO)
    else()
      set(HAVE_OPENCV YES)
      include_directories(${OpenCV_INCLUDE_DIRS})
		#include_directories(${OPENCV_INCLUDE_DIR})
    endif()
endif()

# --- Eigen ---
if(WITH_EIGEN)
  find_path(EIGEN_INCLUDE_PATH "Eigen/Core"
            PATHS /usr/local /opt /usr $ENV{EIGEN_ROOT}/include ENV ProgramFiles ENV ProgramW6432
            PATH_SUFFIXES include/eigen3 include/eigen2 Eigen/include/eigen3 Eigen/include/eigen2
            DOC "The path to Eigen3/Eigen2 headers"
            CMAKE_FIND_ROOT_PATH_BOTH)

  if(EIGEN_INCLUDE_PATH)
    include_directories(${EIGEN_INCLUDE_PATH})
    #ocv_parse_header("${EIGEN_INCLUDE_PATH}/Eigen/src/Core/util/Macros.h" EIGEN_VERSION_LINES EIGEN_WORLD_VERSION EIGEN_MAJOR_VERSION EIGEN_MINOR_VERSION)
    set(HAVE_EIGEN 1)
  endif()
endif(WITH_EIGEN)

# --- GDAL (optional) ---
if(WITH_GDAL)
    find_package(GDAL QUIET)

    if(NOT GDAL_FOUND)
        #ocv_clear_vars(GDAL_LIBRARY GDAL_INCLUDE_DIR)
        set(HAVE_GDAL NO)
    else()
        set(HAVE_GDAL YES)
		include_directories(${GDAL_INCLUDE_DIR})
    endif()
endif()

if(WITH_PROJ4)
  find_package(PROJ4 QUIET)
  
    if(NOT PROJ4_FOUND)
        set(HAVE_PROJ4 NO)
    else()
        set(HAVE_PROJ4 YES)
		include_directories(${PROJ4_INCLUDE})
		#include_directories(${PROJ4_INCLUDE_DIRS})
    endif()
endif()

# --- LibRaw ---
if(WITH_LIBRAW)
    find_package(LIBRAW QUIET)

    if(NOT LIBRAW_FOUND)
        set(HAVE_LIBRAW NO)
    else()
        set(HAVE_LIBRAW YES)
		include_directories(${LIBRAW_INCLUDE_DIR})
    endif()
endif()

# include del directorio principal
include_directories(${CMAKE_BUILD_DIR})

# Directivas del preprocesador
add_definitions(-DUNICODE)

#cabecera con la configuración
configure_file("${PROJECT_SOURCE_DIR}/cmake/config_tl.h.in" "${PROJECT_BINARY_DIR}/config_tl.h")
include_directories("${PROJECT_BINARY_DIR}")

#directorio con el código fuente de los diferentes modulos
add_subdirectory(src)

#directorio con la documentación
add_subdirectory(doc)

#Se incluyen los ejemplos
add_subdirectory(examples)

#Se incluyen los test
add_subdirectory(test)


###############################################################################################################
#                                                Instalación                                                  #
###############################################################################################################



#ejemplos
if(BUILD_EXAMPLES)

	#install(TARGETS  RUNTIME DESTINATION bin)
		
endif()	

install(FILES ${PROJECT_BINARY_DIR}/config_tl.h DESTINATION include)

