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

project(tl_geospatial)

add_library(${PROJECT_NAME} ${LIB_TYPE}
            crs.cpp
            crs.h
            crscache.cpp
            crscache.h
            crstransf.h
            #dtm.cpp
            #dtm.h
            util.cpp
            util.h
            ortho.cpp
            ortho.h
            footprint.cpp
            footprint.h
            diffrect.cpp
            diffrect.h
            camera.cpp
            camera.h
            photo.cpp
            photo.h
)
  
# Añade el proyecto a la carpeta 'modules'
set_property(TARGET ${PROJECT_NAME} 
             PROPERTY FOLDER "modules")

if(HAVE_OPENBLAS)
    add_definitions(-DHAVE_LAPACK_CONFIG_H)
    add_definitions(-DLAPACK_COMPLEX_STRUCTURE)
endif(HAVE_OPENBLAS)

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
                          tl_core 
                          tl_math 
                          tl_geom
                          tl_vect
                          tl_img)
	
	  if (HAVE_GDAL)
        target_link_libraries(${PROJECT_NAME} ${GDAL_LIBRARY})
    endif(HAVE_GDAL)

    if(HAVE_PROJ4)
        target_link_libraries(${PROJECT_NAME} 
                              ${PROJ4_LIBRARY})
    endif(HAVE_PROJ4)

    if(HAVE_OPENBLAS)
        target_link_libraries(${PROJECT_NAME} ${LAPACKE_LIBRARIES})
    endif(HAVE_OPENBLAS)

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
