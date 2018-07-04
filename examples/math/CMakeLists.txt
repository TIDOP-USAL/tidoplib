
if(BUILD_TL_CORE AND BUILD_TL_MATH)

  project(examples)

  if(CMAKE_COMPILER_IS_GNUCXX AND NOT ENABLE_NOISY_WARNINGS)
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wno-unused-function -Wno-missing-declarations")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-unused-function -Wno-missing-declarations")
  endif()

  
# -----------------------------------------------------------------------------

  MACRO(DEFINE_EXAMPLE name srcs)

    include_directories(.)
    include_directories(${CMAKE_SOURCE_DIR}/src)
	
    set(sample_kind example)
    set(sample_KIND EXAMPLE)
    #set(sample_subfolder "cpp")

    set(the_target "${sample_kind}_${name}")
    add_executable(${the_target} ${srcs})

    target_link_libraries(${the_target} tl_core tl_geom tl_graphic tl_img tl_imgprocess tl_geospatial tl_vect ${OpenCV_LIBS} ${GDAL_LIBRARY} ${PROJ4_LIBRARY})
    target_link_libraries(${the_target} ${Boost_FILESYSTEM_LIBRARY} ${Boost_SYSTEM_LIBRARY})
    if (UNIX)
      target_link_libraries(${the_target} -lpthread -ldl -lexpat -ljasper -ljpeg -ltiff -lpng -lm -lrt -lpcre)
    endif()

    # Añade el proyecto de ejmplo a la carpeta 'examples'
    set_property(TARGET ${the_target} PROPERTY FOLDER "examples/math")

    set_target_properties(${the_target} PROPERTIES
      OUTPUT_NAME "cpp-${sample_kind}-${name}"
      PROJECT_LABEL "(${sample_KIND}) ${name}")

    if(ENABLE_SOLUTION_FOLDERS)
      set_target_properties(${the_target} PROPERTIES FOLDER "examples/math")
    endif()

    if(WIN32)
      if (MSVC AND NOT BUILD_SHARED_LIBS)
        set_target_properties(${the_target} PROPERTIES LINK_FLAGS "/NODEFAULTLIB:atlthunk.lib /NODEFAULTLIB:atlsd.lib /DEBUG")
      endif()
      install(TARGETS ${the_target}
              RUNTIME DESTINATION "${PROJECT_BINARY_DIR}" COMPONENT samples)
    endif()
  ENDMACRO()

# -----------------------------------------------------------------------------


  file(GLOB_RECURSE cpp_samples RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} *.cpp)
  

  foreach(sample_filename ${cpp_samples})
    get_filename_component(sample ${sample_filename} NAME_WE)
    DEFINE_EXAMPLE(${sample}  ${sample_filename})
  endforeach()

endif()
