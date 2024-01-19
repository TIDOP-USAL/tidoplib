unset(LASzip_FOUND)

SET(LASzip_DIR "" CACHE PATH "Root folder of LASzip dependency")

list(APPEND LASzip_INCLUDE_DIRS 
     ${LASzip_DIR}/include)
     
list(APPEND LASzip_LIBRARY_PATHS
     ${LASzip_DIR}/lib)


find_path(LASzip_INCLUDE_DIR
          NAMES 
            laszip/laszip_api.h
          PATHS 
            ${LASzip_INCLUDE_DIRS})

find_library(LASzip_LIBRARY
             NAMES 
               laszip3
             PATHS 
               ${LASzip_LIBRARY_PATHS})

find_library(LASzip_LIBRARY_DEBUG
             NAMES 
               laszip3d
             PATHS 
               ${LASzip_LIBRARY_PATHS})


include(FindPackageHandleStandardArgs)

find_package_handle_standard_args(LASzip
                                  FOUND_VAR 
                                    LASzip_FOUND
                                  REQUIRED_VARS
                                    LASzip_LIBRARY
                                    LASzip_LIBRARY_DEBUG
                                    LASzip_INCLUDE_DIR)
                                    
if(LASzip_FOUND)
  set(LASzip_INCLUDE_DIRS ${LASzip_INCLUDE_DIR})
  set(LASzip_LIBRARIES debug ${LASzip_LIBRARY_DEBUG} 
                       optimized ${LASzip_LIBRARY})
else()
  set(ZLIB_INCLUDE_DIR "")
  set(ZLIB_LIBRARY_RELEASE "")
  set(ZLIB_LIBRARY_DEBUG "")
endif()


if (LASzip_INCLUDE_DIR AND LASzip_LIBRARY)
    message(STATUS "\n")
    message(STATUS "Found LASzip")
    message(STATUS "  Include   : ${LASzip_INCLUDE_DIR}")
    message(STATUS "  Library   : ${LASzip_LIBRARY}")
else()
    if(LASzip_FIND_REQUIRED)
        message(FATAL_ERROR "Could not find LASzip")
    endif()
endif()