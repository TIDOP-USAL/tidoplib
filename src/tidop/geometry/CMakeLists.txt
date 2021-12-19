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


# Geometry module

if(BUILD_TL_GEOMETRY)

    project(tl_geom)
  
    add_library(${PROJECT_NAME} ${LIB_TYPE}
                transform/transform.h
                transform/affine.h
                transform/helmert2d.h
                transform/helmert3d.h
                transform/polynomial.h
                transform/projective.h
                transform/rotation.h
                transform/scale.h
                transform/translation.h
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
                entities/segment.cpp
                entities/segment.h
                entities/window.h
                entities/shapes.h
                algorithms.h
                algorithms/vector.h
                algorithms/distance.h
                algorithms/angle.h
                algorithms/projection.h
                algorithms/buffer.h
                algorithms/intersect.h)
   
    target_include_directories(${PROJECT_NAME} PUBLIC
                               $<BUILD_INTERFACE:${TidopLib_SOURCE_DIR}/src>
                               $<INSTALL_INTERFACE:include>)

    target_compile_definitions(${PROJECT_NAME} PUBLIC
                               $<$<BOOL:${OPENBLAS_FOUND}>:HAVE_LAPACK_CONFIG_H>
                               $<$<BOOL:${OPENBLAS_FOUND}>:LAPACK_COMPLEX_STRUCTURE>)

    target_link_libraries(${PROJECT_NAME} PUBLIC
                          tl_core
                          tl_math
                          $<$<BOOL:${OPENBLAS_FOUND}>:${LAPACKE_LIBRARIES}>)

    #if(HAVE_OPENBLAS)
    #    target_link_libraries(${PROJECT_NAME} PUBLIC ${LAPACKE_LIBRARIES})
    #    add_definitions(-DHAVE_LAPACK_CONFIG_H)
    #    add_definitions(-DLAPACK_COMPLEX_STRUCTURE)
    #endif(HAVE_OPENBLAS)


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
            DESTINATION include/tidop/geometry
            FILES_MATCHING PATTERN "*.h")
  		
    set(TidopLib_LIBRARIES ${TidopLib_LIBRARIES} ${PROJECT_NAME})
  
    set(HAVE_TL_GEOMETRY YES CACHE INTERNAL "")
    message(STATUS "TidopLib::tl_geom enabled")

elseif(BUILD_TL_CORE AND BUILD_TL_MATH) 

    message(STATUS "TidopLib::tl_geom disabled")
    message(STATUS "TidopLib::tl_img disabled          [Module dependent on tl_geom that is deactivated]") 
    message(STATUS "TidopLib::tl_graphic disabled      [Module dependent on tl_geom that is deactivated]") 
    message(STATUS "TidopLib::tl_featmatch disabled    [Module dependent on tl_geom that is deactivated]")
    message(STATUS "TidopLib::tl_geospatial disabled   [Module dependent on tl_geom that is deactivated]")

endif(BUILD_TL_GEOMETRY)