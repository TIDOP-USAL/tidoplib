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
        
            file(READ "/proc/cpuinfo" cpuinfo)
            string(REGEX REPLACE ".*flags[ \t]*:[ \t]+([^\n]+).*" "\\1" cpuinfo_flags "${cpuinfo}")
             
            string(TOLOWER "${architecture}" architecture_lower)
            if(${cpuinfo_flags} MATCHES ${architecture_lower})
                set(ARCHITECTURE_SUPPORTED 1)
            else()
                set(ARCHITECTURE_SUPPORTED 0)
            endif()
             
        elseif(CMAKE_SYSTEM_NAME STREQUAL "Windows")
        
            get_filename_component(vendor_id "[HKEY_LOCAL_MACHINE\\Hardware\\Description\\System\\CentralProcessor\\0;VendorIdentifier]" NAME)
            get_filename_component(cpu_id "[HKEY_LOCAL_MACHINE\\Hardware\\Description\\System\\CentralProcessor\\0;Identifier]" NAME)
            string(REGEX REPLACE ".* Family ([0-9]+) .*" "\\1" cpu_family "${cpu_id}")
            string(REGEX REPLACE ".* Model ([0-9]+) .*" "\\1" cpu_model "${cpu_id}")
            
            if(vendor_id STREQUAL "GenuineIntel")
                if(cpu_family EQUAL 6)
                    if(cpu_model EQUAL 87) # Knights Landing
                        set(architecture_instructions SSE SSE2 SSE3 SSSE3 SSE4_1 SSE4_2 AVX RDRND F16C AVX2 FMA BMI BMI2 AVX512F AVX512PF AVX512ER AVX512CD)
                    elseif(cpu_model EQUAL 92) # Knights Mill
                        set(architecture_instructions SSE SSE2 SSE3 SSSE3 SSE4_1 SSE4_2 AVX RDRND F16C AVX2 FMA BMI BMI2 AVX512F AVX512CD AVX512DQ AVX512BW AVX512VL AVX512IFMA AVX512VBMI AVX512_4FMAPS)
                    elseif(cpu_model EQUAL 92) # goldmont
                        set(architecture_instructions SSE SSE2 SSE3 SSSE3 SSE4_1 SSE4_2 RDRND)
                    elseif(cpu_model EQUAL 90 OR cpu_model EQUAL 76) # silvermont
                        set(architecture_instructions SSE SSE2 SSE3 SSSE3 SSE4_1 SSE4_2 RDRND)
                    elseif(cpu_model EQUAL 102) # cannonlake
                        set(architecture_instructions SSE SSE2 SSE3 SSSE3 SSE4_1 SSE4_2 AVX RDRND F16C AVX2 FMA BMI BMI2 AVX512F AVX512CD AVX512DQ AVX512BW AVX512VL AVX512IFMA AVX512VBMI)
                    elseif(cpu_model EQUAL 142) # kaby lake
                        set(architecture_instructions SSE SSE2 SSE3 SSSE3 SSE4_1 SSE4_2 AVX RDRND F16C AVX2 FMA BMI BMI2)
                    elseif(cpu_model EQUAL 158) # coffe lake
                        set(architecture_instructions SSE SSE2 SSE3 SSSE3 SSE4_1 SSE4_2 AVX RDRND F16C AVX2 FMA BMI BMI2)
                    elseif(cpu_model EQUAL 85) # skylake avx512
                        set(architecture_instructions SSE SSE2 SSE3 SSSE3 SSE4_1 SSE4_2 AVX RDRND F16C AVX2 FMA BMI BMI2 AVX512F AVX512CD AVX512DQ AVX512BW AVX512VL)
                    elseif(cpu_model EQUAL 78 OR cpu_model EQUAL 94) # skylake
                        set(architecture_instructions SSE SSE2 SSE3 SSSE3 SSE4_1 SSE4_2 AVX RDRND F16C AVX2 FMA BMI BMI2)
                    elseif(cpu_model EQUAL 61 OR cpu_model EQUAL 71 OR cpu_model EQUAL 79 OR cpu_model EQUAL 86) # broadwell
                        set(architecture_instructions SSE SSE2 SSE3 SSSE3 SSE4_1 SSE4_2 AVX RDRND F16C AVX2 FMA BMI BMI2)
                    elseif(cpu_model EQUAL 60 OR cpu_model EQUAL 69 OR cpu_model EQUAL 70 OR cpu_model EQUAL 63) # haswell
                        set(architecture_instructions SSE SSE2 SSE3 SSSE3 SSE4_1 SSE4_2 AVX RDRND F16C AVX2 FMA BMI BMI2)
                    elseif(cpu_model EQUAL 58 OR cpu_model EQUAL 62) # ivy bridge
                        set(architecture_instructions SSE SSE2 SSE3 SSSE3 SSE4_1 SSE4_2 AVX AVXI RDRND F16C)
                    elseif(cpu_model EQUAL 42 OR cpu_model EQUAL 45) # sandy bridge
                        set(architecture_instructions SSE SSE2 SSE3 SSSE3 SSE4_1 SSE4_2 AVX)
                    elseif(cpu_model EQUAL 31 OR cpu_model EQUAL 37 OR cpu_model EQUAL 44 OR cpu_model EQUAL 47) # westmere
                        set(architecture_instructions SSE SSE2 SSE3 SSSE3 SSE4_1 SSE4_2)
                    elseif(cpu_model EQUAL 26 OR cpu_model EQUAL 30 OR cpu_model EQUAL 31 OR cpu_model EQUAL 46) # nehalem
                        set(architecture_instructions SSE SSE2 SSE3 SSSE3 SSE4_1 SSE4_2)
                    elseif(cpu_model EQUAL 23 OR cpu_model EQUAL 29) # penryn
                        set(architecture_instructions SSE SSE2 SSE3 SSSE3)
                    elseif(cpu_model EQUAL 15) # merom
                        set(architecture_instructions SSE SSE2 SSE3 SSSE3)
                    elseif(cpu_model EQUAL 28) # atom
                        set(architecture_instructions SSE SSE2 SSE3 SSSE3)
                    elseif(cpu_model EQUAL 14) # core
                    set(architecture_instructions SSE SSE2 SSE3)
                    elseif(cpu_model LESS 14) 
                        set(architecture_instructions SSE SSE2)
                    else()
                        set(architecture_instructions "")
                    endif()
                elseif(cpu_family EQUAL 7) # Itanium 
                    set(architecture_instructions "")
                elseif(cpu_family EQUAL 15) # NetBurst
                    if(cpu_model GREATER 2) # Not sure whether this must be 3 or even 4 instead
                        set(architecture_instructions SSE SSE2 SSE3)
                    else()
                        set(architecture_instructions SSE SSE2)    
                    endif()
                endif()
            elseif(vendor_id STREQUAL "AuthenticAMD")
                if(cpu_family EQUAL 23) # zen
                    set(architecture_instructions SSE SSE2 SSE3 SSSE3 SSE4_1 SSE4_2 AVX RDRND F16C AVX2 FMA BMI BMI2 SSE4A)
                elseif(cpu_family EQUAL 22) # AMD 16h
                    set(architecture_instructions SSE SSE2 SSE3 SSSE3 SSE4A SSE4_1 SSE4_2 AVX F16C)
                elseif(cpu_family EQUAL 21) # 15h
                    if(cpu_model LESS 2) # bulldozer
                        set(architecture_instructions SSE SSE2 SSE3 SSSE3 SSE4A SSE4_1 SSE4_2 AVX XOP FMA4)
                    else() # piledriver
                        set(architecture_instructions SSE SSE2 SSE3 SSSE3 SSE4A SSE4_1 SSE4_2 AVX XOP MA4 FMA F16C)
                    endif()
                elseif(cpu_family EQUAL 20) # AMD 14h
                    set(architecture_instructions SSE SSE2 SSE3 SSSE3 SSE4A)
                elseif(cpu_family EQUAL 18) # barcelona
                    set(architecture_instructions SSE SSE2 SSE3 SSE4A)
                elseif(cpu_family EQUAL 16) # 
                    set(architecture_instructions SSE SSE2 SSE3 SSE4A)
                elseif(cpu_family EQUAL 15) # k8
                    set(architecture_instructions SSE SSE2)
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
    
    # Minimun architecture required SSE2
    set(architectures SSE2 SSE3 SSE4_1 SSE4_2 AVX AVX2)
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
    #elseif(${architecture} STREQUAL "SSE")
    #    set(TL_HAVE_SSE TRUE PARENT_SCOPE)
    else()
        set(TL_HAVE_SIMD_INTRINSICS FALSE PARENT_SCOPE)
    endif()

    if(MSVC)
        if(${architecture} STREQUAL "AVX2")
            ADD_DEFINITIONS(/arch:AVX2)
        elseif(${architecture} STREQUAL "AVX")
            ADD_DEFINITIONS(/arch:AVX)
        elseif(TL_ARCH EQUAL x86)
            if(${architecture} STREQUAL "SSE4_2")
                ADD_DEFINITIONS(/arch:SSE2)
            elseif(${architecture} STREQUAL "SSE4_1")
                ADD_DEFINITIONS(/arch:SSE2)
            elseif(${architecture} STREQUAL "SSE3")
                ADD_DEFINITIONS(/arch:SSE2)
            elseif(${architecture} STREQUAL "SSE2")
                ADD_DEFINITIONS(/arch:SSE2)
            elseif(${architecture} STREQUAL "SSE")
                ADD_DEFINITIONS(/arch:SSE)
            endif()
        endif()
    endif(MSVC)
endfunction(set_architecture)
