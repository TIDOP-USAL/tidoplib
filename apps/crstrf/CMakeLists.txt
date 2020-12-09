if(BUILD_TL_CORE AND BUILD_TL_GEOMETRY AND BUILD_TL_GEOSPATIAL AND HAVE_GDAL AND HAVE_PROJ4)

include_directories(${CMAKE_CURRENT_SOURCE_DIR})
include_directories(${CMAKE_SOURCE_DIR}/src)


project(crstransform)

if(CMAKE_COMPILER_IS_GNUCXX AND NOT ENABLE_NOISY_WARNINGS)
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wno-unused-function -Wno-missing-declarations")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-unused-function -Wno-missing-declarations")
endif()

  
add_executable(${PROJECT_NAME} crstransform.cpp)

target_link_libraries(${PROJECT_NAME} 
                      tl_core
                      tl_math
                      tl_geom
                      tl_geospatial
                      tl_img
                      tl_experimental)
					  
if(HAVE_OPENCV)
    target_link_libraries(${PROJECT_NAME} 
                          ${OpenCV_LIBS})
endif(HAVE_OPENCV)

if(HAVE_GDAL)
    target_link_libraries(${PROJECT_NAME} 
                          ${GDAL_LIBRARY})
endif(HAVE_GDAL)

if(HAVE_PROJ4)
    target_link_libraries(${PROJECT_NAME} 
                          ${PROJ4_LIBRARY})
endif(HAVE_PROJ4)

target_link_libraries(${PROJECT_NAME} 
                      ${Boost_FILESYSTEM_LIBRARY} 
                      ${Boost_SYSTEM_LIBRARY})
if (UNIX)
    target_link_libraries(${PROJECT_NAME} 
                          -lpthread 
                          -ldl 
                          -lexpat 
                          -ljasper 
                          -ljpeg 
                          -ltiff 
                          -lpng 
                          -lm 
                          -lrt 
                          -lpcre)
endif()

# Añade el proyecto de ejemplo a la carpeta 'examples'
set_property(TARGET ${PROJECT_NAME} PROPERTY FOLDER "apps/crstransform")

set_target_properties(${PROJECT_NAME} PROPERTIES
                      OUTPUT_NAME "${PROJECT_NAME}"
                      PROJECT_LABEL "${PROJECT_NAME}")

if(ENABLE_SOLUTION_FOLDERS)
  set_target_properties(${PROJECT_NAME} PROPERTIES FOLDER "apps/crstransform")
endif()

if(WIN32)
  if (MSVC AND NOT BUILD_SHARED_LIBS)
    set_target_properties(${PROJECT_NAME} PROPERTIES LINK_FLAGS "/NODEFAULTLIB:atlthunk.lib /NODEFAULTLIB:atlsd.lib /DEBUG")
  endif()
endif()

endif()
