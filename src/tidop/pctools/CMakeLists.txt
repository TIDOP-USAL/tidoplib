##########################################################################
#                                                                        #
# Copyright (C) 2021 by Tidop Research Group                             #
# Copyright (C) 2021 by David Hernandez Lopez                            #
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
#  TidopLib pctools module                                              #
##########################################################################

include(TidopUtils)

if (BUILD_TL_PCTOOLS)

    project(PointCloudTools)
    
    add_files_to_project(${PROJECT_NAME}
                         SOURCE_FILES
						     PointCloudTools.cpp
							 impl/PointCloudReader.cpp
							 impl/PointCloudReaderPDAL.cpp
							 impl/PointCloudReaderFactory.cpp
							 impl/PlyReader.cpp
							 impl/ply/ply.cpp
                         HEADER_FILES
						     PointCloudToolsDefinitions.h
						     PointCloudTools.h
                             PointCloudReader.h
                             impl/PlyReader.h
							 impl/PointCloudReaderPDAL.h
							 impl/ply/ply.h)

    add_library(${PROJECT_NAME} ${LIB_TYPE}
                ${PROJECT_SOURCE_FILES}
                ${PROJECT_HEADER_FILES})

    add_library(TidopLib::${PROJECT_NAME} 
                ALIAS ${PROJECT_NAME})

    target_include_directories(${PROJECT_NAME} PRIVATE 
                               $<$<BOOL:${TL_HAVE_GDAL}>:${GDAL_INCLUDE_DIR}>
							   ${PDAL_INCLUDE_DIRS}
							   ${PDAL_INCLUDE_DIRS}/pdal)
    
    add_definitions(${PDAL_DEFINITIONS})

    target_link_libraries(${PROJECT_NAME} PUBLIC
                          TidopLib::Core
						  TidopLib::GeoTools
						  TidopLib::Graphic
                          $<$<BOOL:${TL_HAVE_GDAL}>:${GDAL_LIBRARY}>
                          $<$<BOOL:${TL_HAVE_PROJ4}>:${PROJ4_LIBRARY}>
                          $<$<BOOL:${TL_HAVE_PROJ}>:PROJ::proj>
						  ${PDAL_LIBRARIES}
						  COPCLIB::copc-lib 
						  LAZPERF::lazperf)

    
    set_target_properties(${PROJECT_NAME} PROPERTIES 
                          FOLDER "modules"
                          OUTPUT_NAME "tl_pctools")

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
            DESTINATION include/tidop/pctools
            FILES_MATCHING PATTERN "*.h")
            
    set(TidopLib_LIBRARIES ${TidopLib_LIBRARIES} ${PROJECT_NAME})
    
    set(TL_HAVE_PCTOOLS YES CACHE INTERNAL "")
    message(STATUS "  TidopLib::${PROJECT_NAME} enabled")

elseif(BUILD_TL_CORE AND BUILD_TL_GEOTOOLS AND BUILD_TL_GEOMETRY)

     message(STATUS "  TidopLib::GeoTools disabled")

endif()
