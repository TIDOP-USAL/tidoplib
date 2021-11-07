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


#Modulo geometry de la librería

if (NOT HAVE_TL_CORE)
  message(WARNING "TidopLib::tl_geom disabled [Module dependent on tl_core that is deactivated]")
elseif(NOT HAVE_TL_MATH)
  message(WARNING "TidopLib::tl_geom disabled [Module dependent on tl_math that is deactivated]") 
else()

  project(tl_geom)
  
  add_library(${PROJECT_NAME} ${LIB_TYPE}
              #operations.cpp
              #operations.h
              transform/transform.h
              transform/affine.h
              transform/helmert2d.h
              transform/helmert3d.h
              #transform/perspective.h
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
              algorithms/intersect.h
  )
    
  # Añade el proyecto a la carpeta 'modules'
  set_property(TARGET ${PROJECT_NAME} 
               PROPERTY FOLDER "modules")
  
  #instalación
  if(BUILD_SHARED_LIBS)
  
      install(TARGETS ${PROJECT_NAME}
              EXPORT "${TARGETS_EXPORT_NAME}"
              RUNTIME DESTINATION ${TL_INSTALL_BINARIES_PREFIX}/bin
              LIBRARY DESTINATION ${TL_INSTALL_BINARIES_PREFIX}/lib
              ARCHIVE DESTINATION ${TL_INSTALL_BINARIES_PREFIX}/lib)
  
      set_target_properties(${PROJECT_NAME} PROPERTIES
                            DEFINE_SYMBOL TIDOPLIB_EXPORTS)
  
      target_link_libraries(${PROJECT_NAME} 
                            tl_core tl_math)
  
      if(HAVE_EIGEN)
          target_link_libraries (${PROJECT_NAME} Eigen3::Eigen)
      endif(HAVE_EIGEN)
  
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
  message(STATUS "TidopLib::${PROJECT_NAME} enabled")
  
endif()