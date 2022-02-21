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

# Modulo vect de la librería
# Lectura y escritura de formatos vectoriales

if (BUILD_TL_VECT)

    project(tl_vect)
  
    add_library(${PROJECT_NAME} ${LIB_TYPE}
                vect.h
                vectreader.cpp
                vectreader.h
                vectwriter.cpp
                vectwriter.h			
                formats.cpp
                formats.h)
      
    target_include_directories(${PROJECT_NAME} PRIVATE ${GDAL_INCLUDE_DIR})

    target_compile_definitions(${PROJECT_NAME} PUBLIC
                               $<$<BOOL:${TL_HAVE_OPENBLAS}>:HAVE_LAPACK_CONFIG_H>
                               $<$<BOOL:${TL_HAVE_OPENBLAS}>:LAPACK_COMPLEX_STRUCTURE>)

    target_link_libraries(${PROJECT_NAME} PRIVATE
                          tl_core
                          tl_graphic
                          $<$<BOOL:${GDAL_FOUND}>:${GDAL_LIBRARY}>
                          $<$<BOOL:${TL_HAVE_OPENBLAS}>:OpenBLAS::OpenBLAS>)	

    set_property(TARGET ${PROJECT_NAME} PROPERTY FOLDER "modules")
    

    if(BUILD_SHARED_LIBS)
        install(TARGETS ${PROJECT_NAME}
                EXPORT "${TARGETS_EXPORT_NAME}"
                RUNTIME DESTINATION ${TL_INSTALL_BINARIES_PREFIX}/bin
                LIBRARY DESTINATION ${TL_INSTALL_BINARIES_PREFIX}/lib
                ARCHIVE DESTINATION ${TL_INSTALL_BINARIES_PREFIX}/lib) 
    	    
        set_target_properties(${PROJECT_NAME} PROPERTIES
                              DEFINE_SYMBOL TIDOPLIB_EXPORTS)
    
    else()
    
        install(TARGETS tl_vect 
                EXPORT "${TARGETS_EXPORT_NAME}"
                LIBRARY DESTINATION ${TL_INSTALL_BINARIES_PREFIX}/lib 
                ARCHIVE DESTINATION ${TL_INSTALL_BINARIES_PREFIX}/lib)
    
    endif()
    
    install(DIRECTORY . 
            DESTINATION include/tidop/vect
            FILES_MATCHING PATTERN "*.h")
            
    set(TidopLib_LIBRARIES ${TidopLib_LIBRARIES} ${PROJECT_NAME})
    
    set(TL_HAVE_VECT YES CACHE INTERNAL "")
    message(STATUS "  TidopLib::${PROJECT_NAME} enabled")
 
elseif(BUILD_TL_CORE AND BUILD_TL_GRAPHIC)

    message(STATUS "  TidopLib::tl_vect disabled")
    message(STATUS "  TidopLib::tl_geospatial disabled   [Module dependent on tl_vect that is deactivated]")

endif(BUILD_TL_VECT)
  