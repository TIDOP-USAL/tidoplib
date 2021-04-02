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

if(HAVE_TL_GEOSPATIAL AND HAVE_GDAL AND HAVE_PROJ4)

include_directories(${CMAKE_CURRENT_SOURCE_DIR})
include_directories(${CMAKE_SOURCE_DIR}/src)


project(crstransform)

if(CMAKE_COMPILER_IS_GNUCXX AND NOT ENABLE_NOISY_WARNINGS)
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wno-unused-function -Wno-missing-declarations")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-unused-function -Wno-missing-declarations")
endif()

  
add_executable(${PROJECT_NAME} crstransform.cpp)

target_link_libraries(${PROJECT_NAME} 
                      tl_core
                      tl_math
                      tl_geom
                      tl_geospatial)
					  
if(HAVE_OPENCV)
    target_link_libraries(${PROJECT_NAME} 
                          ${OpenCV_LIBS})
endif(HAVE_OPENCV)

if(HAVE_GDAL)
    target_link_libraries(${PROJECT_NAME} 
                          ${GDAL_LIBRARY})
endif(HAVE_GDAL)

if(HAVE_PROJ4)
    target_link_libraries(${PROJECT_NAME} 
                          ${PROJ4_LIBRARY})
endif(HAVE_PROJ4)

target_link_libraries(${PROJECT_NAME} 
                      ${Boost_FILESYSTEM_LIBRARY} 
                      ${Boost_SYSTEM_LIBRARY})
if (UNIX)
    target_link_libraries(${PROJECT_NAME} 
                          -lpthread 
                          -ldl 
                          -lexpat 
                          -ljasper 
                          -ljpeg 
                          -ltiff 
                          -lpng 
                          -lm 
                          -lrt 
                          -lpcre)
endif()

if(HAVE_OPENBLAS)
    target_link_libraries(${PROJECT_NAME}
                          OpenBLAS::OpenBLAS)

    add_definitions(-DHAVE_LAPACK_CONFIG_H)
    add_definitions(-DLAPACK_COMPLEX_STRUCTURE)
endif(HAVE_OPENBLAS)

# Añade el proyecto de ejemplo a la carpeta 'examples'
set_property(TARGET ${PROJECT_NAME} PROPERTY FOLDER "apps/crstransform")

set_target_properties(${PROJECT_NAME} PROPERTIES
                      OUTPUT_NAME "${PROJECT_NAME}"
                      PROJECT_LABEL "${PROJECT_NAME}")

if(ENABLE_SOLUTION_FOLDERS)
  set_target_properties(${PROJECT_NAME} PROPERTIES FOLDER "apps/crstransform")
endif()

if(WIN32)
  if (MSVC AND NOT BUILD_SHARED_LIBS)
    set_target_properties(${PROJECT_NAME} PROPERTIES LINK_FLAGS "/NODEFAULTLIB:atlthunk.lib /NODEFAULTLIB:atlsd.lib /DEBUG")
  endif()
endif()

endif()
