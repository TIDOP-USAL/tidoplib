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

include_directories(${CMAKE_CURRENT_SOURCE_DIR})


if(BUILD_TL_CORE)
  #directorio con el módulo 'Core' de la librería
  add_subdirectory(tidop/core)
else()
  message(STATUS "TidopLib::tl_core disabled")
  #message(WARNING "Disable tl_core dependent methods: tl_math, tl_geom, tl_img, tl_imgprocess, tl_graphic, tl_geospatial, tl_vect, tl_experimental")
endif()

if(BUILD_TL_MATH)
  add_subdirectory(tidop/math)
else()
  message(STATUS "TidopLib::tl_math disabled")
endif()
    
if(BUILD_TL_GEOMETRY)
  add_subdirectory(tidop/geometry)
else()
  message(STATUS "TidopLib::tl_geom disabled")
endif()
	
if(BUILD_TL_IMG)
  add_subdirectory(tidop/img)
else()
 message(STATUS "TidopLib::tl_img disabled")
endif()
    
if(BUILD_TL_FEAT_MATCH)
  add_subdirectory(tidop/featmatch)
else()
  message(STATUS "TidopLib::tl_featmatch disabled")
endif()
    
if(BUILD_TL_GRAPHIC)
  add_subdirectory(tidop/graphic)
 else()
  message(STATUS "TidopLib::tl_graphic disabled")
 endif()
    
if(BUILD_TL_IMG_PROCESS)
  add_subdirectory(tidop/imgprocess)
else()
  message(STATUS "TidopLib::tl_imgprocess disabled")
endif()
	
if(BUILD_TL_VECT)
  add_subdirectory(tidop/vect)
else()
  message(STATUS "TidopLib::tl_vect disabled")
endif()

if(BUILD_TL_GEOSPATIAL)
  add_subdirectory(tidop/geospatial)
else()
  message(STATUS "TidopLib::tl_geospatial disabled")
endif()
     
if(BUILD_TL_EXPERIMENTAL)
  add_subdirectory(tidop/experimental)
else()
  message(STATUS "TidopLib::tl_experimental disabled")
endif()
