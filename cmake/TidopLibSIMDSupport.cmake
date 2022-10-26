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


                        
macro(find_supported_architecture architecture)

    if(${CMAKE_VERSION} VERSION_GREATER_EQUAL 3.10 AND ${architecture} STREQUAL "SSE")
        cmake_host_system_information(RESULT SSE_SUPPORTED QUERY HAS_SSE)
        set(ARCHITECTURE_SUPPORTED ${SSE_SUPPORTED})
    elseif(${CMAKE_VERSION} VERSION_GREATER_EQUAL 3.10 AND ${architecture} STREQUAL "SSE2")
        cmake_host_system_information(RESULT SSE2_SUPPORTED QUERY HAS_SSE2)
        set(ARCHITECTURE_SUPPORTED ${SSE2_SUPPORTED})
    else()    

        set(ARCHITECTURE_SUPPORTED 0)
        
        if(CMAKE_SYSTEM_NAME STREQUAL "Linux")
            file(READ "/proc/cpuinfo" _cpuinfo)
            string(REGEX REPLACE ".*flags[ \t]*:[ \t]+([^\n]+).*" "\\1" _cpu_flags "${_cpuinfo}")
             
            string(TOLOWER "${architecture}" ${architecture}_lower)
            
            if(_cpu_flags MATCHES ${architecture_lower})
                set(ARCHITECTURE_SUPPORTED 1)
            else()
                set(ARCHITECTURE_SUPPORTED 0)
            endif()
             
        elseif(CMAKE_SYSTEM_NAME STREQUAL "Windows")
            get_filename_component(_vendor_id "[HKEY_LOCAL_MACHINE\\Hardware\\Description\\System\\CentralProcessor\\0;VendorIdentifier]" NAME)
            get_filename_component(_cpu_id "[HKEY_LOCAL_MACHINE\\Hardware\\Description\\System\\CentralProcessor\\0;Identifier]" NAME)
            #mark_as_advanced(_vendor_id _cpu_id)
            string(REGEX REPLACE ".* Family ([0-9]+) .*" "\\1" _cpu_family "${_cpu_id}")
            string(REGEX REPLACE ".* Model ([0-9]+) .*" "\\1" _cpu_model "${_cpu_id}")
            if(_vendor_id STREQUAL "GenuineIntel")
                if(_cpu_family EQUAL 6)
                    if(_cpu_model EQUAL 87) # Knights Landing
                    elseif(_cpu_model EQUAL 92) # goldmont
                    elseif(_cpu_model EQUAL 90 OR _cpu_model EQUAL 76) # silvermont
                        set(architecture_instructions SSE SSE2 SSE3 SSSE3 SSE4.1 SSE4.2 RDRND)
                    elseif(_cpu_model EQUAL 102) # cannonlake
                    elseif(_cpu_model EQUAL 142 OR _cpu_model EQUAL 158) # kaby-lake
                    elseif(_cpu_model EQUAL 85) # skylake-avx512
                    elseif(_cpu_model EQUAL 78 OR _cpu_model EQUAL 94) # skylake
                        #TODO: En mi portatil (desde la terminal con ubuntu) tiene ssse3 pero no sse3. Con CPU-Z si que sale. 
                        set(architecture_instructions SSE SSE2 SSE3 SSSE3 SSE4_1 SSE4_2 AVX RDRND F16C AVX2 FMA BMI BMI2)
                    elseif(_cpu_model EQUAL 61 OR _cpu_model EQUAL 71 OR _cpu_model EQUAL 79 OR _cpu_model EQUAL 86) # broadwell
                    elseif(_cpu_model EQUAL 60 OR _cpu_model EQUAL 69 OR _cpu_model EQUAL 70 OR _cpu_model EQUAL 63) # haswell
                    elseif(_cpu_model EQUAL 58 OR _cpu_model EQUAL 62) # ivy-bridge
                        set(architecture_instructions SSE SSE2 SSE3 SSSE3 SSE4_1 SSE4_2 AVX AVXI RDRND F16C)
                    elseif(_cpu_model EQUAL 42 OR _cpu_model EQUAL 45) # sandy-bridge
                    elseif(_cpu_model EQUAL 37 OR _cpu_model EQUAL 44 OR _cpu_model EQUAL 47) # westmere
                    elseif(_cpu_model EQUAL 26 OR _cpu_model EQUAL 30 OR _cpu_model EQUAL 31 OR _cpu_model EQUAL 46) # nehalem
                    elseif(_cpu_model EQUAL 23 OR _cpu_model EQUAL 29) # penryn
                    elseif(_cpu_model EQUAL 15) # merom
                    elseif(_cpu_model EQUAL 28) # atom
                    elseif(_cpu_model EQUAL 14) # core
                    elseif(_cpu_model LESS 14) 
                        set(architecture_instructions SSE SSE2)
                    else()
                        set(architecture_instructions "")
                    endif()
                elseif(_cpu_family EQUAL 7) # Itanium 
                    set(architecture_instructions "")
                elseif(_cpu_family EQUAL 15) # NetBurst
                    if(_cpu_model GREATER 2) # Not sure whether this must be 3 or even 4 instead
                        set(architecture_instructions SSE SSE2 SSE3)
                    else()
                        set(architecture_instructions SSE SSE2)    
                    endif()
                endif()
            elseif(_vendor_id STREQUAL "AuthenticAMD")
                if(_cpu_family EQUAL 23) # zen
                elseif(_cpu_family EQUAL 22) # AMD 16h
                elseif(_cpu_family EQUAL 21) # 15h
                    if(_cpu_model LESS 2) # bulldozer
                    else() # piledriver
                    endif()
                elseif(_cpu_family EQUAL 20) # AMD 14h
                elseif(_cpu_family EQUAL 18) # 12h
                elseif(_cpu_family EQUAL 16) # barcelona
                elseif(_cpu_family EQUAL 15) # k8
                    if(_cpu_model GREATER 64) # k8-sse3. I don't know the right number to put here. This is just a guess from the hardware I have access to
                    endif()
                endif()
            endif()
            
            if (${architecture} IN_LIST architecture_instructions)
                set(ARCHITECTURE_SUPPORTED 1)
            else()
                set(ARCHITECTURE_SUPPORTED 0)
            endif()
                         
        endif()

    endif()
    
endmacro(find_supported_architecture)

function(find_supported_architectures)

    set(architectures SSE SSE2 SSE3 SSE4_1 SSE4_2 AVX AVX2)
    set(ARCH_SUPPORTED "")

    foreach(arch IN ITEMS ${architectures})

        find_supported_architecture(${arch})

        if(${ARCHITECTURE_SUPPORTED})

            set(${arch}_FOUND TRUE PARENT_SCOPE)
            set(ARCH_SUPPORTED ${arch} "${ARCH_SUPPORTED}")
            set(SUPPORTED_ARCHITECTURES "${ARCH_SUPPORTED}" PARENT_SCOPE)

        else()
            set(ARCH_${arch} TRUE PARENT_SCOPE)
        endif()
    endforeach()      

    
endfunction(find_supported_architectures)



unset(TL_HAVE_SSE CACHE)
unset(TL_HAVE_SSE2 CACHE)
unset(TL_HAVE_SSE3 CACHE)
unset(TL_HAVE_SSE4_1 CACHE)
unset(TL_HAVE_SSE4_2 CACHE)
unset(TL_HAVE_AVX CACHE)
unset(TL_HAVE_AVX2 CACHE)
unset(TL_HAVE_SIMD_INTRINSICS CACHE)


function(set_architecture architecture)

    set(TL_HAVE_SIMD_INTRINSICS TRUE)

    if(${architecture} STREQUAL "AVX2")
        set(TL_HAVE_AVX2 TRUE PARENT_SCOPE)
        set(TL_HAVE_AVX TRUE PARENT_SCOPE)
        set(TL_HAVE_SSE4_2 TRUE PARENT_SCOPE)
        set(TL_HAVE_SSE4_1 TRUE PARENT_SCOPE)
        set(TL_HAVE_SSE3 TRUE PARENT_SCOPE)
        set(TL_HAVE_SSE2 TRUE PARENT_SCOPE)
        set(TL_HAVE_SSE TRUE PARENT_SCOPE)
    elseif(${architecture} STREQUAL "AVX")
        set(TL_HAVE_AVX TRUE PARENT_SCOPE)
        set(TL_HAVE_SSE4_2 TRUE PARENT_SCOPE)
        set(TL_HAVE_SSE4_1 TRUE PARENT_SCOPE)
        set(TL_HAVE_SSE3 TRUE PARENT_SCOPE)
        set(TL_HAVE_SSE2 TRUE PARENT_SCOPE)
        set(TL_HAVE_SSE TRUE PARENT_SCOPE)
    elseif(${architecture} STREQUAL "SSE4_2")
        set(TL_HAVE_SSE4_2 TRUE PARENT_SCOPE)
        set(TL_HAVE_SSE4_1 TRUE PARENT_SCOPE)
        set(TL_HAVE_SSE3 TRUE PARENT_SCOPE)
        set(TL_HAVE_SSE2 TRUE PARENT_SCOPE)
        set(TL_HAVE_SSE TRUE PARENT_SCOPE)
    elseif(${architecture} STREQUAL "SSE4_1")
        set(TL_HAVE_SSE4_1 TRUE PARENT_SCOPE)
        set(TL_HAVE_SSE3 TRUE PARENT_SCOPE)
        set(TL_HAVE_SSE2 TRUE PARENT_SCOPE)
        set(TL_HAVE_SSE TRUE PARENT_SCOPE)
    elseif(${architecture} STREQUAL "SSE3")
        set(TL_HAVE_SSE3 TRUE PARENT_SCOPE)
        set(TL_HAVE_SSE2 TRUE PARENT_SCOPE)
        set(TL_HAVE_SSE TRUE PARENT_SCOPE)
    elseif(${architecture} STREQUAL "SSE2")
        set(TL_HAVE_SSE2 TRUE PARENT_SCOPE)
        set(TL_HAVE_SSE TRUE PARENT_SCOPE)
    elseif(${architecture} STREQUAL "SSE")
        set(TL_HAVE_SSE TRUE PARENT_SCOPE)
    else()
        set(TL_HAVE_SIMD_INTRINSICS FALSE PARENT_SCOPE)
    endif()

    if(MSVC)
        if(${architecture} STREQUAL "AVX2")
        ADD_DEFINITIONS(/arch:AVX2)
        elseif(${architecture} STREQUAL "AVX")
        ADD_DEFINITIONS(/arch:AVX)
        elseif(${architecture} STREQUAL "SSE4_2")
        elseif(${architecture} STREQUAL "SSE4_1")
        elseif(${architecture} STREQUAL "SSE3")
        #TODO: Por defecto en x64
        elseif(${architecture} STREQUAL "SSE2")
        #TODO: Por defecto en x64
        elseif(${architecture} STREQUAL "SSE")
        endif()
    endif(MSVC)
endfunction(set_architecture)
