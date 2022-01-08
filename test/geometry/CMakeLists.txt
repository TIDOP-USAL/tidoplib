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

if(TL_HAVE_GEOMETRY)

  MACRO(DEFINE_TEST name srcs)

    include_directories(${CMAKE_BUILD_DIR})
    include_directories(${CMAKE_SOURCE_DIR}/src)

    project(${name} LANGUAGES CXX)

    add_executable(${PROJECT_NAME} ${srcs})
	
    target_compile_definitions(${PROJECT_NAME} PUBLIC
                               $<$<BOOL:${HAVE_OPENBLAS}>:HAVE_LAPACK_CONFIG_H>
                               $<$<BOOL:${HAVE_OPENBLAS}>:LAPACK_COMPLEX_STRUCTURE>)


    target_link_libraries(${PROJECT_NAME} 
                          tl_core 
                          tl_geom 
                          tl_math 
                          ${Boost_UNIT_TEST_FRAMEWORK_LIBRARY}
                          $<$<BOOL:${HAVE_OPENBLAS}>:OpenBLAS::OpenBLAS>)

    if (UNIX)
      target_link_libraries(${PROJECT_NAME} -lpthread -ldl -lexpat -ljasper -ljpeg -ltiff -lpng -lm -lrt -lpcre)
    endif()


    set_property(TARGET ${PROJECT_NAME} PROPERTY FOLDER "test/geometry")

    set_target_properties(${PROJECT_NAME} PROPERTIES
                          OUTPUT_NAME ${PROJECT_NAME}
                          PROJECT_LABEL "(TEST) ${PROJECT_NAME}")

    if(ENABLE_SOLUTION_FOLDERS)
        set_target_properties(${PROJECT_NAME} PROPERTIES FOLDER "test/geometry")
    endif()
	
    add_test(NAME ${PROJECT_NAME} COMMAND $<TARGET_FILE:${PROJECT_NAME}>)
	
  ENDMACRO()

  file(GLOB_RECURSE cpp_test RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} *.cpp)
  

  foreach(test_filename ${cpp_test})
    get_filename_component(test ${test_filename} NAME_WE)
    DEFINE_TEST(${test}  ${test_filename})
  endforeach()

  
endif()
