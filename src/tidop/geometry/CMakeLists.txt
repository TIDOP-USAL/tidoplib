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


##########################################################################
#  TidopLib Geometry module                                              #
##########################################################################

include(TidopUtils)

if(BUILD_TL_GEOMETRY)

    project(Geom)
  
    add_files_to_project(${PROJECT_NAME} 
                        SOURCE_FILES 
                            entities/segment.cpp
                        HEADER_FILES
                            rect.h
                            size.h
                            entities/bbox.h
                            entities/entity.h
                            entities/entities2d.h
                            entities/entities3d.h
                            entities/linestring.h
                            entities/point.h
                            entities/polygon.h
                            entities/multipoint.h
                            entities/segment.h
                            entities/window.h
                            entities/shapes.h
                            algorithms.h
                            algorithms/vector.h
                            algorithms/distance.h
                            algorithms/angle.h
                            algorithms/projection.h
                            algorithms/buffer.h
                            algorithms/intersect.h
							algorithms/dbscan.h)
        
    add_library(${PROJECT_NAME} ${LIB_TYPE}
                ${PROJECT_SOURCE_FILES}
                ${PROJECT_HEADER_FILES})
    
    add_library(TidopLib::${PROJECT_NAME}
                ALIAS ${PROJECT_NAME})

    target_include_directories(${PROJECT_NAME} PUBLIC
                               $<BUILD_INTERFACE:${TidopLib_SOURCE_DIR}/src>
                               $<INSTALL_INTERFACE:include>)

    target_compile_definitions(${PROJECT_NAME} PUBLIC
                               $<$<BOOL:${TL_HAVE_OPENBLAS}>:HAVE_LAPACK_CONFIG_H>
                               $<$<BOOL:${TL_HAVE_OPENBLAS}>:LAPACK_COMPLEX_STRUCTURE>)

    target_link_libraries(${PROJECT_NAME} PUBLIC
                          TidopLib::Core
                          TidopLib::Math
                          $<$<BOOL:${TL_HAVE_OPENBLAS}>:OpenBLAS::OpenBLAS>)


    set_target_properties(${PROJECT_NAME} PROPERTIES
                          FOLDER "modules"
                          OUTPUT_NAME "tl_geom")

    if(BUILD_SHARED_LIBS)
        set_target_properties(${PROJECT_NAME} PROPERTIES
                              DEFINE_SYMBOL TIDOPLIB_EXPORTS)
    endif()
            
    install(TARGETS ${PROJECT_NAME}
            EXPORT "${TARGETS_EXPORT_NAME}"
            RUNTIME DESTINATION ${TL_INSTALL_BINARIES_PREFIX}bin
            LIBRARY DESTINATION ${TL_INSTALL_BINARIES_PREFIX}lib
            ARCHIVE DESTINATION ${TL_INSTALL_BINARIES_PREFIX}lib)

    install(DIRECTORY . 
            DESTINATION include/tidop/geometry
            FILES_MATCHING PATTERN "*.h")
  		
    set(TidopLib_LIBRARIES ${TidopLib_LIBRARIES} ${PROJECT_NAME})
  
    set(TL_HAVE_GEOMETRY YES CACHE INTERNAL "")
    message(STATUS "  TidopLib::Geom enabled")

elseif(BUILD_TL_CORE AND BUILD_TL_MATH) 

    message(STATUS "  TidopLib::Geom disabled")
    message(STATUS "  TidopLib::Img disabled          [Module dependent on TidopLib::Geom that is deactivated]") 
    message(STATUS "  TidopLib::Graphic disabled      [Module dependent on TidopLib::Geom that is deactivated]") 
    message(STATUS "  TidopLib::FeatMatch disabled    [Module dependent on TidopLib::Geom that is deactivated]")
    message(STATUS "  TidopLib::Geospatial disabled   [Module dependent on TidopLib::Geom that is deactivated]")

endif(BUILD_TL_GEOMETRY)