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

#find_package(COPCLIB REQUIRED)
#find_package(LAZPERF REQUIRED)

include(TidopUtils)

#include_directories(${CMAKE_CURRENT_SOURCE_DIR})
#include_directories(${CMAKE_SOURCE_DIR}/src)

project(TestQt)

#target_include_directories(${PROJECT_NAME} PUBLIC
#                               $<BUILD_INTERFACE:${TidopLib_SOURCE_DIR}/src>
#                               $<INSTALL_INTERFACE:include>)

#target_include_directories(${PROJECT_NAME} PRIVATE 
#                               $<$<BOOL:${TL_HAVE_GDAL}>:${GDAL_INCLUDE_DIR}>
#							   ${PDAL_INCLUDE_DIRS}
#							   ${PDAL_INCLUDE_DIRS}/pdal)

#target_include_directories(${PROJECT_NAME} PRIVATE 
#                               $<$<BOOL:${TL_HAVE_GDAL}>:${GDAL_INCLUDE_DIR}>
#							   ${PDAL_INCLUDE_DIR}
#							   ${PDAL_INCLUDE_DIRS}/pdal)

add_files_to_project(${PROJECT_NAME}
                         SOURCE_FILES 
                             src/main.cpp
                             src/MainWindow.cpp
                         HEADER_FILES
                             src/MainWindow.h
                         FORM_FILES
                             src/MainWindow.ui)

set(TestQt_FORMS
    src/MainWindow.ui
    )

add_executable(${PROJECT_NAME}
               ${PROJECT_SOURCE_FILES}
               ${PROJECT_HEADER_FILES})

# Link libs
target_link_libraries(${PROJECT_NAME} 
                      TidopLib::Viewer)
 #                     COPCLIB::copc-lib 
 #                     LAZPERF::lazperf)

set_target_properties(${PROJECT_NAME} PROPERTIES
                      OUTPUT_NAME ${PROJECT_NAME}
                      PROJECT_LABEL "(TEST) ${PROJECT_NAME}")

set_target_properties(${PROJECT_NAME} PROPERTIES 
                      FOLDER "test/viewer")

add_test(NAME ${PROJECT_NAME} COMMAND $<TARGET_FILE:${PROJECT_NAME}>)

if(MSVC)
    set_user_enviroment_path(${PROJECT_NAME} "$ENV{PATH}")
    set_tests_properties(${PROJECT_NAME} PROPERTIES ENVIRONMENT "PATH=$ENV{PATH}")
endif(MSVC)

