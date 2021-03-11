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

#Modulo Experimental de la librería

add_library(tl_experimental ${LIB_TYPE}
            desingpatterns.cpp
            designpatterns.h
            #datamodel.cpp
            #datamodel.h
            video/videoio.cpp
            video/videoio.h
            #camera.cpp
            #camera.h
            #photo.cpp
            #photo.h
            )
  
# Añade el proyecto a la carpeta 'modules'
set_property(TARGET tl_experimental PROPERTY FOLDER "modules")

if(HAVE_LAPACKE)
    add_definitions(-DHAVE_LAPACK_CONFIG_H)
    add_definitions(-DLAPACK_COMPLEX_STRUCTURE)
endif(HAVE_LAPACKE)

#instalación
if(BUILD_SHARED_LIBS)

    install(TARGETS tl_experimental
            EXPORT "${TARGETS_EXPORT_NAME}"
            RUNTIME DESTINATION ${TL_INSTALL_BINARIES_PREFIX}/bin
            LIBRARY DESTINATION ${TL_INSTALL_BINARIES_PREFIX}/lib
    	  	  ARCHIVE DESTINATION ${TL_INSTALL_BINARIES_PREFIX}/lib) 
		  
    set_target_properties(tl_experimental PROPERTIES
                          DEFINE_SYMBOL TIDOPLIB_EXPORTS)
						
    target_link_libraries(tl_experimental tl_core)	
						
    if(HAVE_LAPACKE)
        target_link_libraries(tl_experimental ${LAPACKE_LIBRARIES})
    endif(HAVE_LAPACKE)
   
    if(HAVE_OPENCV)
        target_link_libraries(tl_experimental 
                              ${OpenCV_LIBS})
    endif(HAVE_OPENCV)

else()

  install(TARGETS tl_experimental 
          EXPORT "${TARGETS_EXPORT_NAME}"
          LIBRARY DESTINATION ${TL_INSTALL_BINARIES_PREFIX}/lib 
          ARCHIVE DESTINATION ${TL_INSTALL_BINARIES_PREFIX}/lib)

endif()

install(DIRECTORY . 
        DESTINATION include/tidop/experimental
        FILES_MATCHING PATTERN "*.h")

set(TidopLib_LIBRARIES ${TidopLib_LIBRARIES} tl_experimental)