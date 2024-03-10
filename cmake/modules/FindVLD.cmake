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

SET(VLD_ROOT "" CACHE PATH "Root folder of VLD dependency")

list(APPEND VLD_LIBRARY_PATHS ${VLD_ROOT}/lib)
     
if(CMAKE_CL_64)
    list(APPEND VLD_LIBRARY_PATHS ${VLD_ROOT}/lib/Win64)
else()
    list(APPEND VLD_LIBRARY_PATHS ${VLD_ROOT}/lib/Win32)
endif()


find_path(VLD_INCLUDE_DIR
          NAMES 
		    vld.h
          PATHS 
		    ${VLD_ROOT}/include)

find_library(VLD_LIBRARY
             NAMES 
			   vld
             PATHS 
			   ${VLD_LIBRARY_PATHS})



include(FindPackageHandleStandardArgs)

find_package_handle_standard_args(VLD
                                  FOUND_VAR 
								    VLD_FOUND
                                  REQUIRED_VARS
                                    VLD_LIBRARY
                                    VLD_INCLUDE_DIR)