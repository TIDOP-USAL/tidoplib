# Test modulo Math

if(BUILD_TL_MATH AND BUILD_TL_CORE AND BUILD_TL_GEOMETRY)

  project(test)


  MACRO(DEFINE_TEST name srcs)

    include_directories(${CMAKE_SOURCE_DIR}/src)
    include_directories(${GTEST_INCLUDE_DIRS})
	
    set(test_kind test)
    set(test_KIND TEST)
    
    set(the_target "${test_kind}_${name}")
    add_executable(${the_target} ${srcs})
    
    target_link_libraries(${the_target} tl_core tl_geom tl_math)
    target_link_libraries(${the_target} ${Boost_FILESYSTEM_LIBRARY} ${Boost_SYSTEM_LIBRARY} ${Boost_UNIT_TEST_FRAMEWORK_LIBRARY})
	if(HAVE_GDAL)
      target_link_libraries(${the_target} ${GDAL_LIBRARY})
	endif(HAVE_GDAL)
	if(HAVE_OPENCV)
      target_link_libraries(${the_target} ${OpenCV_LIBS})
	endif(HAVE_OPENCV)
	
    # Añade el proyecto de test a la carpeta 'test/math'
    set_property(TARGET ${the_target} PROPERTY FOLDER "test/math")
    
    set_target_properties(${the_target} PROPERTIES
      OUTPUT_NAME ${the_target}
      PROJECT_LABEL "(${test_KIND}) ${name}")

    if(ENABLE_SOLUTION_FOLDERS)
      set_target_properties(${the_target} PROPERTIES FOLDER "test/math")
    endif()

    #if(WIN32)
    #  if (MSVC AND NOT BUILD_SHARED_LIBS)
    #    set_target_properties(${the_target} PROPERTIES LINK_FLAGS "/NODEFAULTLIB:atlthunk.lib /NODEFAULTLIB:atlsd.lib /DEBUG")
    #  endif()
    #  install(TARGETS ${the_target}
    #          RUNTIME DESTINATION "${PROJECT_BINARY_DIR}" COMPONENT test)
    #endif()
	
	
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
