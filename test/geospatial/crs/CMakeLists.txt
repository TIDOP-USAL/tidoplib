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
 

include_directories(${CMAKE_BUILD_DIR})
include_directories(${CMAKE_SOURCE_DIR}/src)

set(test_filename crs_test.cpp)
get_filename_component(test_name ${test_filename} NAME_WE)

project(${test_name} LANGUAGES CXX)

add_executable(${PROJECT_NAME} 
               ${test_filename})
			   
target_compile_definitions(${PROJECT_NAME} PUBLIC
                               $<$<BOOL:${HAVE_OPENBLAS}>:HAVE_LAPACK_CONFIG_H>
                               $<$<BOOL:${HAVE_OPENBLAS}>:LAPACK_COMPLEX_STRUCTURE>)
							   
target_link_libraries(${PROJECT_NAME}
                      TidopLib::Core 
                      TidopLib::Geom 
                      TidopLib::Geospatial
                      ${Boost_UNIT_TEST_FRAMEWORK_LIBRARY}
                      $<$<BOOL:${HAVE_GDAL}>:${GDAL_LIBRARY}>
                      $<$<BOOL:${TL_HAVE_PROJ4}>:${PROJ4_LIBRARY}>
                      $<$<BOOL:${HAVE_OPENBLAS}>:OpenBLAS::OpenBLAS>
                      ${OpenCV_LIBS})
					  
	
set_target_properties(${PROJECT_NAME} PROPERTIES
                      OUTPUT_NAME ${PROJECT_NAME}
                      PROJECT_LABEL "(TEST) ${PROJECT_NAME}")

set_target_properties(${PROJECT_NAME} PROPERTIES 
                      FOLDER "test/geospatial")

add_test(NAME ${PROJECT_NAME} COMMAND $<TARGET_FILE:${PROJECT_NAME}>)
