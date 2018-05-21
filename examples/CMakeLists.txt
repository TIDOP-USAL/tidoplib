# Ejemplos
#TODO: http://www.jamesebutler.com/setting-the-visual-studio-debugger-path-using-cmake/

if(BUILD_EXAMPLES)

  project(examples)

  #ocv_include_directories("${TIDOPLIB_SOURCE_DIR}/include")
  #ocv_include_modules_recurse(${OPENCV_CPP_SAMPLES_REQUIRED_DEPS})

  if(CMAKE_COMPILER_IS_GNUCXX AND NOT ENABLE_NOISY_WARNINGS)
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wno-unused-function -Wno-missing-declarations")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-unused-function -Wno-missing-declarations")
  endif()

  
  # ---------------------------------------------
  #      Define executable targets
  # ---------------------------------------------
  MACRO(OPENCV_DEFINE_CPP_EXAMPLE name srcs)

    include_directories(.)
    include_directories(${CMAKE_SOURCE_DIR}/src)
	
    set(sample_kind example)
    set(sample_KIND EXAMPLE)
    #set(sample_subfolder "cpp")

    set(the_target "${sample_kind}_${name}")
    add_executable(${the_target} ${srcs})
    #ocv_target_link_libraries(${the_target} ${OPENCV_LINKER_LIBS} ${OPENCV_CPP_SAMPLES_REQUIRED_DEPS})
    target_link_libraries(${the_target} tl_core tl_geom tl_graphic tl_img tl_imgprocess tl_geospatial tl_vect ${OpenCV_LIBS} ${GDAL_LIBRARY} ${PROJ4_LIBRARY})
	target_link_libraries(${the_target} ${Boost_FILESYSTEM_LIBRARY} ${Boost_SYSTEM_LIBRARY})
	
	# Añade el proyecto de ejmplo a la carpeta 'examples'
    set_property(TARGET ${the_target} PROPERTY FOLDER "examples")

    set_target_properties(${the_target} PROPERTIES
      OUTPUT_NAME "cpp-${sample_kind}-${name}"
      PROJECT_LABEL "(${sample_KIND}) ${name}")

    if(ENABLE_SOLUTION_FOLDERS)
      set_target_properties(${the_target} PROPERTIES FOLDER "examples")
    endif()

    if(WIN32)
      if (MSVC AND NOT BUILD_SHARED_LIBS)
        set_target_properties(${the_target} PROPERTIES LINK_FLAGS "/NODEFAULTLIB:atlthunk.lib /NODEFAULTLIB:atlsd.lib /DEBUG")
      endif()
      install(TARGETS ${the_target}
              RUNTIME DESTINATION "${PROJECT_BINARY_DIR}" COMPONENT samples)
    endif()
  ENDMACRO()

  file(GLOB_RECURSE cpp_samples RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} *.cpp)

#  if(NOT HAVE_OPENGL)
#    ocv_list_filterout(cpp_samples Qt_sample)
#  endif()
#
#  if(NOT HAVE_opencv_cudaarithm OR NOT HAVE_opencv_cudafilters)
#    ocv_list_filterout(cpp_samples "/gpu/")
#  endif()
#
#  if(NOT TARGET opencv_viz)
#    ocv_list_filterout(cpp_samples "/viz/")
#  endif()
#
#  if(NOT HAVE_IPP_A)
#    ocv_list_filterout(cpp_samples "/ippasync/")
#  endif()

  

  foreach(sample_filename ${cpp_samples})
    if(NOT "${sample_filename}" MATCHES "real_time_pose_estimation/")
        get_filename_component(sample ${sample_filename} NAME_WE)
        OPENCV_DEFINE_CPP_EXAMPLE(${sample}  ${sample_filename})
    endif()
  endforeach()

  #include("tutorial_code/calib3d/real_time_pose_estimation/CMakeLists.txt")
endif()
