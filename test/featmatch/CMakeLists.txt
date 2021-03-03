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
	
#if(HAVE_LAPACKE)
#    add_definitions(-DHAVE_LAPACK_CONFIG_H)
#    add_definitions(-DLAPACK_COMPLEX_STRUCTURE)
#endif(HAVE_LAPACKE)

    target_link_libraries(${the_target} tl_core tl_geom tl_featmatch)
    target_link_libraries(${the_target} ${Boost_FILESYSTEM_LIBRARY} ${Boost_SYSTEM_LIBRARY} ${Boost_UNIT_TEST_FRAMEWORK_LIBRARY})
    if(HAVE_OPENCV)
        target_link_libraries(${the_target} ${OpenCV_LIBS})
    endif(HAVE_OPENCV)

    if (UNIX)
        target_link_libraries(${the_target} -lpthread -ldl -lexpat -ljasper -ljpeg -ltiff -lpng -lm -lrt -lpcre)
    endif()
    #if(HAVE_LAPACKE)
    #    target_link_libraries(${the_target} ${LAPACKE_LIBRARIES})
    #endif(HAVE_LAPACKE)
    
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
