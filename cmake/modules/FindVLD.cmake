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

#Visual Leak Detector

unset(VLD_FOUND)

SET(VLD_DIR "" CACHE PATH "Root folder of VLD dependency")

list(APPEND VLD_INCLUDE_DIRS ${VLD_DIR}/include)
     
list(APPEND VLD_LIBRARY_DIRS ${VLD_DIR}/lib)
     
if(CMAKE_CL_64)
    list(APPEND VLD_LIBRARY_DIRS ${VLD_DIR}/lib/Win64)
else()
    list(APPEND VLD_LIBRARY_DIRS ${VLD_DIR}/lib/Win32)
endif()

include(FindPackageHandleStandardArgs)


find_path(VLD_INCLUDE_DIR
          NAMES vld.h
          PATHS ${VLD_INCLUDE_DIRS}
)

find_library(VLD_LIBRARY
             NAMES vld
             PATHS ${VLD_LIBRARY_DIRS}
)

find_package_handle_standard_args(VLD
                                  FOUND_VAR VLD_FOUND
                                  REQUIRED_VARS
                                    VLD_LIBRARY
                                    VLD_INCLUDE_DIR
)

if(VLD_FOUND)
  set(VLD_INCLUDE_DIRS ${VLD_INCLUDE_DIR})
  set(VLD_LIBRARIES debug ${VLD_LIBRARY})
  set(VLD_DEFINITIONS ${VLD_CFLAGS_OTHER})
endif()


if (VLD_INCLUDE_DIR AND VLD_LIBRARY)
    set(VLD_FOUND TRUE)
    message(STATUS "\n")
    message(STATUS "Found Visual Leak Detector")
    message(STATUS "  Include   : ${VLD_INCLUDE_DIR}")
    message(STATUS "  Library   : ${VLD_LIBRARY}")
else()
    if(VLD_FIND_REQUIRED)
        message(FATAL_ERROR "Could not find Visual Leak Detector")
    endif()
endif()