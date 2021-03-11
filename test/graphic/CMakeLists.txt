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

# Test

if(BUILD_TL_CORE AND BUILD_TL_GEOMETRY AND BUILD_TL_GRAPHIC)

  project(test)

  MACRO(DEFINE_TEST name srcs)

    include_directories(${CMAKE_BUILD_DIR})
    include_directories(${CMAKE_SOURCE_DIR}/src)
	
    set(test_kind test)
    set(test_KIND TEST)

    set(the_target "${test_kind}_${name}")
    add_executable(${the_target} ${srcs})
    
    target_link_libraries(${the_target} tl_core tl_geom tl_graphic)
	    target_link_libraries(${the_target} ${Boost_FILESYSTEM_LIBRARY} ${Boost_SYSTEM_LIBRARY} ${Boost_UNIT_TEST_FRAMEWORK_LIBRARY})
    if(HAVE_GDAL)
      target_link_libraries(${the_target} ${GDAL_LIBRARY})
	endif(HAVE_GDAL)
	if(HAVE_OPENCV)
      target_link_libraries(${the_target} ${OpenCV_LIBS})
	endif(HAVE_OPENCV)
	
	# Añade el proyecto de test a la carpeta 'test/core'
    set_property(TARGET ${the_target} PROPERTY FOLDER "test/graphic")

    set_target_properties(${the_target} PROPERTIES
                          OUTPUT_NAME ${the_target}
                          PROJECT_LABEL "(${test_KIND}) ${name}")

    if(ENABLE_SOLUTION_FOLDERS)
        set_target_properties(${the_target} PROPERTIES FOLDER "test/graphic")
    endif()

    #if(WIN32)
    #  if (MSVC AND NOT BUILD_SHARED_LIBS)
    #    set_target_properties(${the_target} PROPERTIES LINK_FLAGS "/NODEFAULTLIB:atlthunk.lib /NODEFAULTLIB:atlsd.lib /DEBUG")
    #  endif()
    #  install(TARGETS ${the_target}
    #          RUNTIME DESTINATION "${PROJECT_BINARY_DIR}" COMPONENT test)
    #endif()
	#
	#
    #if (BUILD_SHARED_LIBS)
    #  set_target_properties(${the_target}
    #    PROPERTIES
    #    COMPILE_DEFINITIONS "GTEST_LINKED_AS_SHARED_LIBRARY=1")
    #endif()
  
    add_test(NAME ${the_target} COMMAND $<TARGET_FILE:${the_target}>)
	
  ENDMACRO()

  file(GLOB_RECURSE cpp_test RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} *.cpp)


  foreach(test_filename ${cpp_test})
    get_filename_component(test ${test_filename} NAME_WE)
    DEFINE_TEST(${test}  ${test_filename})
  endforeach()


endif()
