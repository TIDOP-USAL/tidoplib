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


                        
macro(find_supported_cxx_standard cxx_standard)

    if (${CMAKE_VERSION} VERSION_GREATER_EQUAL 3.8 AND "cxx_std_11" IN_LIST CMAKE_CXX_COMPILE_FEATURES)
        list (APPEND cxx_standard_values C++11)
    endif()

    if (${CMAKE_VERSION} VERSION_GREATER_EQUAL 3.8 AND "cxx_std_14" IN_LIST CMAKE_CXX_COMPILE_FEATURES)
        list (APPEND cxx_standard_values C++14)
    endif()
    
    if(${CMAKE_VERSION} VERSION_GREATER_EQUAL 3.8 AND "cxx_std_17" IN_LIST CMAKE_CXX_COMPILE_FEATURES)
        list (APPEND cxx_standard_values C++17)
    endif()
    
    if(${CMAKE_VERSION} VERSION_GREATER_EQUAL 3.12 AND "cxx_std_20" IN_LIST CMAKE_CXX_COMPILE_FEATURES)
        list (APPEND cxx_standard_values C++20)
    endif()
    
    if(${CMAKE_VERSION} VERSION_GREATER_EQUAL 3.20 AND "cxx_std_23" IN_LIST CMAKE_CXX_COMPILE_FEATURES)
        list (APPEND cxx_standard_values C++23)
    endif()
    
    if(${CMAKE_VERSION} VERSION_GREATER_EQUAL 3.25 AND "cxx_std_26" IN_LIST CMAKE_CXX_COMPILE_FEATURES)
        list (APPEND cxx_standard_values C++26)
    endif()
    
endmacro(find_supported_architecture)


function(find_supported_cxx_standards min_cxx_standard max_cxx_standard)
    
    set(cxx_standards C++11 C++14 C++17 C++20)
    set(ARCH_SUPPORTED "")

    foreach(arch IN ITEMS ${cxx_standards})

        find_supported_architecture(${arch})

        if(${ARCHITECTURE_SUPPORTED})

            set(${arch}_FOUND TRUE PARENT_SCOPE)
            set(ARCH_SUPPORTED ${arch} "${ARCH_SUPPORTED}")
            set(SUPPORTED_ARCHITECTURES "${ARCH_SUPPORTED}" PARENT_SCOPE)

        else()
            set(ARCH_${arch} TRUE PARENT_SCOPE)
        endif()
    endforeach()      

    
endfunction(find_supported_cxx_standards)


function(set_cxx_standard cxx_standard)

    if(${cxx_standard} STREQUAL "C++11")
        set(CMAKE_CXX_STANDARD 11)
    elseif(${cxx_standard} STREQUAL "C++14")
        set(CMAKE_CXX_STANDARD 14)
    elseif(${cxx_standard} STREQUAL "C++17")
        set(CMAKE_CXX_STANDARD 17)
    elseif(${cxx_standard} STREQUAL "C++20")
        set(CMAKE_CXX_STANDARD 20)
    elseif(${cxx_standard} STREQUAL "C++23")
        set(CMAKE_CXX_STANDARD 23)
    else()

endfunction(set_architecture)
