if(BUILD_TL_CORE  AND BUILD_TL_EXPERIMENTAL)

include_directories(${CMAKE_CURRENT_SOURCE_DIR})
include_directories(${CMAKE_SOURCE_DIR}/src)


project(extract_frames)

if(CMAKE_COMPILER_IS_GNUCXX AND NOT ENABLE_NOISY_WARNINGS)
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wno-unused-function -Wno-missing-declarations")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-unused-function -Wno-missing-declarations")
endif()

  
add_executable(${PROJECT_NAME} extract_frames.cpp)

target_link_libraries(${PROJECT_NAME} 
                      tl_core
                      tl_experimental)
					  
if(HAVE_OPENCV)
    target_link_libraries(${PROJECT_NAME} 
                          ${OpenCV_LIBS})
endif(HAVE_OPENCV)


target_link_libraries(${PROJECT_NAME} 
                      ${Boost_FILESYSTEM_LIBRARY} 
                      ${Boost_SYSTEM_LIBRARY})


set_property(TARGET ${PROJECT_NAME} PROPERTY FOLDER "apps/frames_from_video")

set_target_properties(${PROJECT_NAME} PROPERTIES
                      OUTPUT_NAME "${PROJECT_NAME}"
                      PROJECT_LABEL "${PROJECT_NAME}")

if(ENABLE_SOLUTION_FOLDERS)
  set_target_properties(${PROJECT_NAME} PROPERTIES FOLDER "apps/frames_from_video")
endif()

if(WIN32)
  if (MSVC AND NOT BUILD_SHARED_LIBS)
    set_target_properties(${PROJECT_NAME} PROPERTIES LINK_FLAGS "/NODEFAULTLIB:atlthunk.lib /NODEFAULTLIB:atlsd.lib /DEBUG")
  endif()
endif()

endif()
