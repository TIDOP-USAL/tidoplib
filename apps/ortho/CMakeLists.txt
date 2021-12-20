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
    
    
    project(ortho LANGUAGES CXX)
    
    if(CMAKE_COMPILER_IS_GNUCXX AND NOT ENABLE_NOISY_WARNINGS)
        set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wno-unused-function -Wno-missing-declarations")
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-unused-function -Wno-missing-declarations")
    endif()
    
      
    add_executable(${PROJECT_NAME} ortho.cpp)

    target_include_directories(${PROJECT_NAME} PRIVATE ${GDAL_INCLUDE_DIR})
    
    target_compile_definitions(${PROJECT_NAME} PUBLIC
                               $<$<BOOL:${OPENBLAS_FOUND}>:HAVE_LAPACK_CONFIG_H>
                               $<$<BOOL:${OPENBLAS_FOUND}>:LAPACK_COMPLEX_STRUCTURE>)
   
    target_link_libraries(${PROJECT_NAME} 
                          tl_core
                          tl_math
                          tl_geom
                          tl_geospatial
                          tl_img
                          tl_vect
                          tl_graphic
                          $<$<BOOL:${GDAL_FOUND}>:${GDAL_LIBRARY}>
                          $<$<BOOL:${PROJ4_FOUND}>:${PROJ4_LIBRARY}>
                          $<$<BOOL:${OPENBLAS_FOUND}>:OpenBLAS::OpenBLAS>)
    					  
    if(HAVE_OPENCV)
        target_link_libraries(${PROJECT_NAME} 
                              ${OpenCV_LIBS})
    endif(HAVE_OPENCV)
    

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
    

    set_property(TARGET ${PROJECT_NAME} PROPERTY FOLDER "apps/ortho")
    
    set_target_properties(${PROJECT_NAME} PROPERTIES
                          OUTPUT_NAME "${PROJECT_NAME}"
                          PROJECT_LABEL "${PROJECT_NAME}")
    
    if(ENABLE_SOLUTION_FOLDERS)
      set_target_properties(${PROJECT_NAME} PROPERTIES FOLDER "apps/ortho")
    endif()
    
    if(WIN32)
      if (MSVC AND NOT BUILD_SHARED_LIBS)
        set_target_properties(${PROJECT_NAME} PROPERTIES LINK_FLAGS "/NODEFAULTLIB:atlthunk.lib /NODEFAULTLIB:atlsd.lib /DEBUG")
      endif()
    endif()
    
endif()
