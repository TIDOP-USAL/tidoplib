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

# Test modulo featmatch

if(BUILD_TL_CORE AND BUILD_TL_GEOMETRY AND BUILD_TL_FEAT_MATCH)

  project(test)

  
  MACRO(DEFINE_TEST name srcs)

    include_directories(${CMAKE_BUILD_DIR})
    include_directories(${CMAKE_SOURCE_DIR}/src)
	
    set(test_kind test)
    set(test_KIND TEST)

    set(the_target "${test_kind}_${name}")
    add_executable(${the_target} ${srcs})
	
#if(HAVE_OPENBLAS)
#    add_definitions(-DHAVE_LAPACK_CONFIG_H)
#    add_definitions(-DLAPACK_COMPLEX_STRUCTURE)
#endif(HAVE_OPENBLAS)

    target_link_libraries(${the_target} tl_core tl_geom tl_featmatch)
    target_link_libraries(${the_target} ${Boost_FILESYSTEM_LIBRARY} ${Boost_SYSTEM_LIBRARY} ${Boost_UNIT_TEST_FRAMEWORK_LIBRARY})
    if(HAVE_OPENCV)
        target_link_libraries(${the_target} ${OpenCV_LIBS})
    endif(HAVE_OPENCV)

    if (UNIX)
        target_link_libraries(${the_target} -lpthread -ldl -lexpat -ljasper -ljpeg -ltiff -lpng -lm -lrt -lpcre)
    endif()
    #if(HAVE_OPENBLAS)
    #    target_link_libraries(${the_target} ${LAPACKE_LIBRARIES})
    #endif(HAVE_OPENBLAS)
    
    set_property(TARGET ${the_target} PROPERTY FOLDER "test/featmatch")

    set_target_properties(${the_target} PROPERTIES
      OUTPUT_NAME "cpp-${test_kind}-${name}"
      PROJECT_LABEL "(${test_KIND}) ${name}")

    if(ENABLE_SOLUTION_FOLDERS)
      set_target_properties(${the_target} PROPERTIES FOLDER "test/featmatch")
    endif()
	
    add_test(NAME ${the_target} COMMAND $<TARGET_FILE:${the_target}>)
	
  ENDMACRO()

  file(GLOB_RECURSE cpp_test RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} *.cpp)
  

  foreach(test_filename ${cpp_test})
    get_filename_component(test ${test_filename} NAME_WE)
    DEFINE_TEST(${test}  ${test_filename})
  endforeach()

  
endif()
