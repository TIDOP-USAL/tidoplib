#LASLib

unset(LASLib_FOUND)

SET(LASLib_ROOT "" CACHE PATH "Root folder of LASLib")

list(APPEND LASLib_INCLUDE_DIRS 
     ${LASLib_ROOT}/include
  ${LASLib_ROOT}/include/LASlib)
     
list(APPEND LASLib_LIBRARY_DIRS 
     ${LASLib_ROOT}/lib)

include(FindPackageHandleStandardArgs)


find_path(LASLib_INCLUDE_DIR
          NAMES lasutility.hpp
          PATHS ${LASLib_INCLUDE_DIRS}
)

find_library(LASLib_LIBRARY
             NAMES LASlib
             PATHS ${LASLib_LIBRARY_DIRS}/LASlib/Release
)

find_library(LASLib_LIBRARY_DEBUG
             NAMES LASlib
             PATHS ${LASLib_LIBRARY_DIRS}/LASlib/Debug
)

find_package_handle_standard_args(LASLib
                                  FOUND_VAR LASLib_FOUND
                                  REQUIRED_VARS
                                    LASLib_LIBRARY
         LASLib_LIBRARY_DEBUG
                                    LASLib_INCLUDE_DIR
)

if(LASLib_FOUND)
  set(LASLib_INCLUDE_DIRS ${LASLib_INCLUDE_DIR})
  set(LASLib_LIBRARIES debug ${LASLib_LIBRARY_DEBUG} 
                     optimized ${LASLib_LIBRARY})
else()
  set(LASLib_INCLUDE_DIR "")
  set(LASLib_LIBRARY_RELEASE "")
  set(LASLib_LIBRARY_DEBUG "")
endif()


if (LASLib_INCLUDE_DIR AND LASLib_LIBRARY)
    message(STATUS "\n")
    message(STATUS "Found LASLib")
    message(STATUS " Include : ${LASLib_INCLUDE_DIR}")
    message(STATUS " Library : ${LASLib_LIBRARY}")
else()
    if(LASLib_FIND_REQUIRED)
        message(FATAL_ERROR "Could not find LASLib")
    endif()
endif()