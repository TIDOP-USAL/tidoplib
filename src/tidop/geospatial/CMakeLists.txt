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

#Modulo geospatial de la librería


if (BUILD_TL_GEOSPATIAL)

    project(tl_geospatial)
  
    add_library(${PROJECT_NAME} ${LIB_TYPE}
                crs.cpp
                crs.h
                crscache.cpp
                crscache.h
                crstransf.h
                util.cpp
                util.h
                ortho.cpp
                ortho.h
                diffrect.cpp
                diffrect.h
                camera.cpp
                camera.h
                photo.cpp
                photo.h)

     target_include_directories(${PROJECT_NAME} PUBLIC
                                $<BUILD_INTERFACE:${TidopLib_SOURCE_DIR}/src>
                                $<INSTALL_INTERFACE:include/tidop/geospatial>)
  
     target_link_libraries(${PROJECT_NAME} PRIVATE
                           tl_core 
                           tl_math 
                           tl_geom
                           tl_vect
                           tl_img)
    if(HAVE_OPENCV)
        target_link_libraries(${PROJECT_NAME} PRIVATE 
                              ${OpenCV_LIBS})
    endif(HAVE_OPENCV)

    if (HAVE_GDAL)
        target_link_libraries(${PROJECT_NAME} PRIVATE ${GDAL_LIBRARY})
    endif(HAVE_GDAL)
    
    if(HAVE_PROJ4)
        target_link_libraries(${PROJECT_NAME} PRIVATE ${PROJ4_LIBRARY})
    endif(HAVE_PROJ4)
    
    if(HAVE_OPENBLAS)
        target_link_libraries(${PROJECT_NAME} PRIVATE ${LAPACKE_LIBRARIES})
        add_definitions(-DHAVE_LAPACK_CONFIG_H)
        add_definitions(-DLAPACK_COMPLEX_STRUCTURE)
    endif(HAVE_OPENBLAS)

    
    set_property(TARGET ${PROJECT_NAME} 
                 PROPERTY FOLDER "modules")

    if(BUILD_SHARED_LIBS)
    
        install(TARGETS ${PROJECT_NAME}
                EXPORT "${TARGETS_EXPORT_NAME}"
                RUNTIME DESTINATION ${TL_INSTALL_BINARIES_PREFIX}/bin
                LIBRARY DESTINATION ${TL_INSTALL_BINARIES_PREFIX}/lib
                ARCHIVE DESTINATION ${TL_INSTALL_BINARIES_PREFIX}/lib)
    	
        set_target_properties(${PROJECT_NAME} PROPERTIES
                              DEFINE_SYMBOL TIDOPLIB_EXPORTS)
    
    else()
    
        install(TARGETS ${PROJECT_NAME} 
                EXPORT "${TARGETS_EXPORT_NAME}"
                LIBRARY DESTINATION ${TL_INSTALL_BINARIES_PREFIX}/lib 
                ARCHIVE DESTINATION ${TL_INSTALL_BINARIES_PREFIX}/lib)
    
    endif()
    
    install(DIRECTORY . 
            DESTINATION include/tidop/geospatial
            FILES_MATCHING PATTERN "*.h")
            
    set(TidopLib_LIBRARIES ${TidopLib_LIBRARIES} ${PROJECT_NAME})
    
    set(HAVE_TL_GEOSPATIAL YES CACHE INTERNAL "")
    message(STATUS "TidopLib::${PROJECT_NAME} enabled")

elseif(BUILD_TL_CORE AND BUILD_TL_MATH AND BUILD_TL_GEOMETRY AND BUILD_TL_IMG AND BUILD_TL_VECT)

    #if(NOT HAVE_GDAL)
    #  message(WARNING "TidopLib::tl_geospatial disabled [Module dependent on gdal that is deactivated]")
    #elseif(NOT HAVE_PROJ4)
    #  message(WARNING "TidopLib::tl_geospatial disabled [Module dependent on proj that is deactivated]")
    #else()

    message(STATUS "TidopLib::tl_geospatial disabled")

endif()
