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
#  TidopLib vector module                                                #
##########################################################################

if (BUILD_TL_VECTOR_TOOLS)

    project(VectorTools)
  
    add_library(${PROJECT_NAME} ${LIB_TYPE}
                vect.cpp
				vect.h
                vectreader.cpp
                vectreader.h
                vectwriter.cpp
                vectwriter.h			
                formats.cpp
                formats.h)

    add_library(TidopLib::${PROJECT_NAME} ALIAS ${PROJECT_NAME})   
    
    target_include_directories(${PROJECT_NAME} PRIVATE 
                               $<$<BOOL:${TL_HAVE_GDAL}>:${GDAL_INCLUDE_DIR}>)

    target_compile_definitions(${PROJECT_NAME} PUBLIC
                               $<$<BOOL:${TL_HAVE_OPENBLAS}>:HAVE_LAPACK_CONFIG_H>
                               $<$<BOOL:${TL_HAVE_OPENBLAS}>:LAPACK_COMPLEX_STRUCTURE>)

    target_link_libraries(${PROJECT_NAME} PRIVATE
                          TidopLib::Core
                          TidopLib::Graphic
                          $<$<BOOL:${GDAL_FOUND}>:${GDAL_LIBRARY}>
                          $<$<BOOL:${TL_HAVE_OPENBLAS}>:OpenBLAS::OpenBLAS>)	

    set_target_properties(${PROJECT_NAME} PROPERTIES 
                          FOLDER "modules"
                          OUTPUT_NAME "tl_vector_tools")
    

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
            DESTINATION include/tidop/vectortools
            FILES_MATCHING PATTERN "*.h")
            
    set(TidopLib_LIBRARIES ${TidopLib_LIBRARIES} ${PROJECT_NAME})
    
    set(TL_HAVE_VECT YES CACHE INTERNAL "")
    message(STATUS "  TidopLib::${PROJECT_NAME} enabled")
 
elseif(BUILD_TL_CORE AND BUILD_TL_GRAPHIC)

    message(STATUS "  TidopLib::VectorTools disabled")
    message(STATUS "  TidopLib::Geospatial disabled   [Module dependent on TidopLib::VectorTools that is deactivated]")

endif(BUILD_TL_VECTOR_TOOLS)
  