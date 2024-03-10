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
# License along with TidopLib. If not, see <http://www.gnu.org/licenses>.#
#                                                                        #
# @license LGPL-3.0 <https://www.gnu.org/licenses/lgpl-3.0.html>         #
#                                                                        #
##########################################################################

if (CMAKE_CXX_COMPILER_ID STREQUAL "AppleClang")
    set(COMPILER_NAME "Apple Clang for Xcode")
elseif(CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
    set(COMPILER_NAME "Clang")
elseif(CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
    set(COMPILER_NAME "GNU")
elseif(CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
    set(COMPILER_NAME "Microsoft Visual Studio")
elseif(CMAKE_CXX_COMPILER_ID STREQUAL "SunPro")
    set(COMPILER_NAME "Oracle Solaris Studio")
elseif(CMAKE_CXX_COMPILER_ID STREQUAL "Intel")
    set(COMPILER_NAME "Intel compiler")
endif()

message(STATUS "\nCompiler info:\n")
if(CMAKE_CXX_COMPILER_LOADED)
    message(STATUS "  C++ compiler path:    ${CMAKE_CXX_COMPILER}")
    message(STATUS "  C++ compiler name:    ${COMPILER_NAME}")
    message(STATUS "  C++ compiler version: ${CMAKE_CXX_COMPILER_VERSION}")
    message(STATUS "  C++ estandar:         ${CMAKE_CXX_STANDARD}\n")
endif()
