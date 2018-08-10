if(BUILD_TEST AND BUILD_TL_CORE AND BUILD_TL_GEOMETRY)

  project(test)

  #if(CMAKE_COMPILER_IS_GNUCXX AND NOT ENABLE_NOISY_WARNINGS)
  #  set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wno-unused-function -Wno-missing-declarations")
  #  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-unused-function -Wno-missing-declarations")
  #endif()

  
  # ---------------------------------------------
  #      Define executable targets
  # ---------------------------------------------
  MACRO(DEFINE_TEST name srcs)

    include_directories(${CMAKE_BUILD_DIR})
    include_directories(${CMAKE_SOURCE_DIR}/src)
	include_directories(${GTEST_INCLUDE_DIRS})
	
    set(test_kind test)
    set(test_KIND TEST)

    set(the_target "${test_kind}_${name}")
    add_executable(${the_target} ${srcs})
	
	target_link_libraries(${the_target} gtest_main)
    target_link_libraries(${the_target} tl_core tl_geom)
	target_link_libraries(${the_target} ${OpenCV_LIBS} ${GDAL_LIBRARY})
	target_link_libraries(${the_target} ${Boost_FILESYSTEM_LIBRARY} ${Boost_SYSTEM_LIBRARY})
	
	# Añade el proyecto de ejmplo a la carpeta 'test/core'
    set_property(TARGET ${the_target} PROPERTY FOLDER "test/geometry")

    set_target_properties(${the_target} PROPERTIES
      OUTPUT_NAME "cpp-${test_kind}-${name}"
      PROJECT_LABEL "(${test_KIND}) ${name}")

    if(ENABLE_SOLUTION_FOLDERS)
      set_target_properties(${the_target} PROPERTIES FOLDER "test/geometry")
    endif()

    #if(WIN32)
    #  if (MSVC AND NOT BUILD_SHARED_LIBS)
    #    set_target_properties(${the_target} PROPERTIES LINK_FLAGS "/NODEFAULTLIB:atlthunk.lib /NODEFAULTLIB:atlsd.lib /DEBUG")
    #  endif()
    #  install(TARGETS ${the_target}
    #          RUNTIME DESTINATION "${PROJECT_BINARY_DIR}" COMPONENT test)
    #endif()
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
    if(NOT "${sample_filename}" MATCHES "real_time_pose_estimation/")
        get_filename_component(test ${test_filename} NAME_WE)
        DEFINE_TEST(${test}  ${test_filename})
    endif()
  endforeach()


endif()
