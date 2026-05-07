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

if(TL_HAVE_PCTOOLS AND TL_HAVE_GEOTOOLS AND TL_HAVE_GDAL)

    include_directories(${CMAKE_CURRENT_SOURCE_DIR})
    
    project(pc_examples LANGUAGES CXX)
    
    if(CMAKE_COMPILER_IS_GNUCXX AND NOT ENABLE_NOISY_WARNINGS)
        set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wno-unused-function -Wno-missing-declarations")
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-unused-function -Wno-missing-declarations")
    endif()
    
      
    add_executable(${PROJECT_NAME} pc_examples.cpp)
    
    target_include_directories(${PROJECT_NAME} PRIVATE 
                               $<$<BOOL:${TL_HAVE_GDAL}>:${GDAL_INCLUDE_DIR}>
                               ${PDAL_INCLUDE_DIRS}
                               ${PDAL_INCLUDE_DIRS}/pdal)
    
    target_compile_definitions(${PROJECT_NAME} PUBLIC)
                               # $<$<BOOL:${OPENBLAS_FOUND}>:HAVE_LAPACK_CONFIG_H>
                               # $<$<BOOL:${OPENBLAS_FOUND}>:LAPACK_COMPLEX_STRUCTURE>)

    target_link_libraries(${PROJECT_NAME} 
                          TidopLib::Core
                          # TidopLib::Math
                          # TidopLib::Geometry
                          # TidopLib::Geospatial
                          TidopLib::GeoTools
                          TidopLib::PointCloudTools
                          $<$<BOOL:${GDAL_FOUND}>:${GDAL_LIBRARY}>
                          $<$<BOOL:${TL_HAVE_PROJ4}>:${PROJ4_LIBRARY}>
                          $<$<BOOL:${TL_HAVE_PROJ}>:PROJ::proj>
                          ${PDAL_LIBRARIES}
                          COPCLIB::copc-lib 
                          LAZPERF::lazperf
                          $<$<BOOL:${TL_HAVE_VLD}>:${VLD_LIBRARY}>)
                          # $<$<BOOL:${OPENBLAS_FOUND}>:${LAPACKE_LIBRARIES}>)

    if(HAVE_OPENCV)
        target_link_libraries(${PROJECT_NAME} 
                              ${OpenCV_LIBS})
    endif(HAVE_OPENCV)
    
    set_target_properties(${PROJECT_NAME} PROPERTIES
                          OUTPUT_NAME ${PROJECT_NAME}
                          PROJECT_LABEL ${PROJECT_NAME}
                          FOLDER "apps/pc_examples")
    
    if(WIN32)
      if (MSVC AND NOT BUILD_SHARED_LIBS)
        set_target_properties(${PROJECT_NAME} PROPERTIES LINK_FLAGS "/NODEFAULTLIB:atlthunk.lib /NODEFAULTLIB:atlsd.lib /DEBUG")
      endif()
    endif()

    install(TARGETS ${PROJECT_NAME}
            EXPORT "${TARGETS_EXPORT_NAME}"
            RUNTIME DESTINATION ${TL_INSTALL_BINARIES_PREFIX}bin) 

    if(MSVC)
        set_user_enviroment_path(${PROJECT_NAME} "${ENV_VARS_LIST}")
    endif(MSVC)

endif()
