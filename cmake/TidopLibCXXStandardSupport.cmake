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


                        
#macro(find_supported_cxx_standard cxx_standard)
#
#    if (${CMAKE_VERSION} VERSION_GREATER_EQUAL 3.8 AND "cxx_std_11" IN_LIST CMAKE_CXX_COMPILE_FEATURES)
#        list (APPEND cxx_standard_values C++11)
#    endif()
#
#    if (${CMAKE_VERSION} VERSION_GREATER_EQUAL 3.8 AND "cxx_std_14" IN_LIST CMAKE_CXX_COMPILE_FEATURES)
#        list (APPEND cxx_standard_values C++14)
#    endif()
#    
#    if(${CMAKE_VERSION} VERSION_GREATER_EQUAL 3.8 AND "cxx_std_17" IN_LIST CMAKE_CXX_COMPILE_FEATURES)
#        list (APPEND cxx_standard_values C++17)
#    endif()
#    
#    if(${CMAKE_VERSION} VERSION_GREATER_EQUAL 3.12 AND "cxx_std_20" IN_LIST CMAKE_CXX_COMPILE_FEATURES)
#        list (APPEND cxx_standard_values C++20)
#    endif()
#    
#    if(${CMAKE_VERSION} VERSION_GREATER_EQUAL 3.20 AND "cxx_std_23" IN_LIST CMAKE_CXX_COMPILE_FEATURES)
#        list (APPEND cxx_standard_values C++23)
#    endif()
#    
#    if(${CMAKE_VERSION} VERSION_GREATER_EQUAL 3.25 AND "cxx_std_26" IN_LIST CMAKE_CXX_COMPILE_FEATURES)
#        list (APPEND cxx_standard_values C++26)
#    endif()
#    
#endmacro(find_supported_architecture)
#
#
#function(find_supported_cxx_standards min_cxx_standard max_cxx_standard)
#    
#    set(cxx_standards C++11 C++14 C++17 C++20)
#    set(ARCH_SUPPORTED "")
#
#    foreach(arch IN ITEMS ${cxx_standards})
#
#        find_supported_architecture(${arch})
#
#        if(${ARCHITECTURE_SUPPORTED})
#
#            set(${arch}_FOUND TRUE PARENT_SCOPE)
#            set(ARCH_SUPPORTED ${arch} "${ARCH_SUPPORTED}")
#            set(SUPPORTED_ARCHITECTURES "${ARCH_SUPPORTED}" PARENT_SCOPE)
#
#        else()
#            set(ARCH_${arch} TRUE PARENT_SCOPE)
#        endif()
#    endforeach()      
#
#    
#endfunction(find_supported_cxx_standards)
#
#
#function(set_cxx_standard cxx_standard)
#
#    if(${cxx_standard} STREQUAL "C++11")
#        set(CMAKE_CXX_STANDARD 11)
#    elseif(${cxx_standard} STREQUAL "C++14")
#        set(CMAKE_CXX_STANDARD 14)
#    elseif(${cxx_standard} STREQUAL "C++17")
#        set(CMAKE_CXX_STANDARD 17)
#    elseif(${cxx_standard} STREQUAL "C++20")
#        set(CMAKE_CXX_STANDARD 20)
#    elseif(${cxx_standard} STREQUAL "C++23")
#        set(CMAKE_CXX_STANDARD 23)
#    elseif(${cxx_standard} STREQUAL "C++26")
#        set(CMAKE_CXX_STANDARD 26)
#    else()
#
#endfunction(set_architecture)


function(_tidop_std_to_number std_name out_num)
    if(std_name STREQUAL "C++11")
        set(num 11)
    elseif(std_name STREQUAL "C++14")
        set(num 14)
    elseif(std_name STREQUAL "C++17")
        set(num 17)
    elseif(std_name STREQUAL "C++20")
        set(num 20)
    elseif(std_name STREQUAL "C++23")
        set(num 23)
    elseif(std_name STREQUAL "C++26")
        set(num 26)
    else()
        set(num 0)
    endif()
    set(${out_num} ${num} PARENT_SCOPE)
endfunction()

# -------------------------------------------------------------------
# Function: tidop_configure_cxx_standard
# 
# Configures the cache variable TIDOPLIB_CXX_STANDARD (or the one specified)
# with the supported standards within the range [MIN, MAX].
# 
# Arguments:
#   MIN_VERSION <std>   - minimum standard (e.g., C++11). Default is C++11.
#   MAX_VERSION <std>   - maximum standard (e.g., C++26). Default is the highest supported.
#   DEFAULT <std>       - default value. If not provided, MAX_VERSION is used.
#   CACHE_VAR <name>    - name of the cache variable. Default is TIDOPLIB_CXX_STANDARD.
#   DESCRIPTION <text>  - description for the cache. Default is "C++ standard to use".
# -------------------------------------------------------------------
function(tidop_configure_cxx_standard)
    set(options "")
    set(oneValueArgs MIN_VERSION MAX_VERSION DEFAULT CACHE_VAR DESCRIPTION)
    set(multiValueArgs "")
    cmake_parse_arguments(TIDOP_CXX "${options}" "${oneValueArgs}" "${multiValueArgs}" ${ARGN})

    # Valores por defecto
    if(NOT TIDOP_CXX_MIN_VERSION)
        set(TIDOP_CXX_MIN_VERSION "C++11")
    endif()
    if(NOT TIDOP_CXX_CACHE_VAR)
        set(TIDOP_CXX_CACHE_VAR "TIDOPLIB_CXX_STANDARD")
    endif()
    if(NOT TIDOP_CXX_DESCRIPTION)
        set(TIDOP_CXX_DESCRIPTION "C++ standard to use")
    endif()


    _tidop_std_to_number("${TIDOP_CXX_MIN_VERSION}" min_num)
    if(min_num EQUAL 0)
        message(FATAL_ERROR "MIN_VERSION '${TIDOP_CXX_MIN_VERSION}' not recognized")
    endif()

    if(TIDOP_CXX_MAX_VERSION)
        _tidop_std_to_number("${TIDOP_CXX_MAX_VERSION}" max_num)
        if(max_num EQUAL 0)
            message(FATAL_ERROR "MAX_VERSION '${TIDOP_CXX_MAX_VERSION}' not recognized")
        endif()
    else()
        set(max_num 999)
    endif()


    set(all_standards "")
    if(CMAKE_VERSION VERSION_GREATER_EQUAL 3.8 AND "cxx_std_11" IN_LIST CMAKE_CXX_COMPILE_FEATURES)
        list(APPEND all_standards "C++11")
    endif()
    if(CMAKE_VERSION VERSION_GREATER_EQUAL 3.8 AND "cxx_std_14" IN_LIST CMAKE_CXX_COMPILE_FEATURES)
        list(APPEND all_standards "C++14")
    endif()
    if(CMAKE_VERSION VERSION_GREATER_EQUAL 3.8 AND "cxx_std_17" IN_LIST CMAKE_CXX_COMPILE_FEATURES)
        list(APPEND all_standards "C++17")
    endif()
    if(CMAKE_VERSION VERSION_GREATER_EQUAL 3.12 AND "cxx_std_20" IN_LIST CMAKE_CXX_COMPILE_FEATURES)
        list(APPEND all_standards "C++20")
    endif()
    if(CMAKE_VERSION VERSION_GREATER_EQUAL 3.20 AND "cxx_std_23" IN_LIST CMAKE_CXX_COMPILE_FEATURES)
        list(APPEND all_standards "C++23")
    endif()
    if(CMAKE_VERSION VERSION_GREATER_EQUAL 3.25 AND "cxx_std_26" IN_LIST CMAKE_CXX_COMPILE_FEATURES)
        list(APPEND all_standards "C++26")
    endif()

    if(NOT all_standards)
        message(FATAL_ERROR "No supported C++ standard was detected. Check your compiler.")
    endif()

    if(NOT TIDOP_CXX_MAX_VERSION)
        list(GET all_standards -1 max_std)
        _tidop_std_to_number("${max_std}" max_num)
    endif()

    set(filtered_standards "")
    foreach(std ${all_standards})
        _tidop_std_to_number("${std}" std_num)
        if(std_num GREATER_EQUAL min_num AND std_num LESS_EQUAL max_num)
            list(APPEND filtered_standards "${std}")
        endif()
    endforeach()

    if(NOT filtered_standards)
        message(FATAL_ERROR "There are no C++ standards in the range [${TIDOP_CXX_MIN_VERSION}, ${TIDOP_CXX_MAX_VERSION}] that are supported.")
    endif()

    if(TIDOP_CXX_DEFAULT)
        set(default_value "${TIDOP_CXX_DEFAULT}")
        list(FIND filtered_standards "${default_value}" _idx)
        if(_idx EQUAL -1)
            message(WARNING "The default value '${default_value}' is not supported or is out of range. The highest available value (${max_allowed}).")
            list(GET filtered_standards -1 default_value)
        endif()
    else()
        list(GET filtered_standards -1 default_value)
    endif()

    set(${TIDOP_CXX_CACHE_VAR} "${default_value}" CACHE STRING "${TIDOP_CXX_DESCRIPTION}")
    set_property(CACHE ${TIDOP_CXX_CACHE_VAR} PROPERTY STRINGS ${filtered_standards})

    set(selected "${${TIDOP_CXX_CACHE_VAR}}")
    _tidop_std_to_number("${selected}" standard_num)
    if(standard_num EQUAL 0)
        message(FATAL_ERROR "Unrecognized '${selected}' standard.")
    endif()

    set(selected "${${TIDOP_CXX_CACHE_VAR}}")

    set(CMAKE_CXX_STANDARD ${standard_num} PARENT_SCOPE)
    set(CMAKE_CXX_STANDARD_REQUIRED ON PARENT_SCOPE)
    set(CMAKE_CXX_EXTENSIONS OFF PARENT_SCOPE)

endfunction()