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

function(add_files_to_source_group group_name files)
   
    set(FILE_LIST ${files} ${ARGN})
	  
    foreach(_source IN ITEMS ${FILE_LIST})
        get_filename_component(_source_path "${_source}" PATH)
        string(REPLACE "/" "\\" _group_path "${_source_path}")
        source_group("${group_name}\\${_group_path}" FILES "${_source}")
    endforeach()
	
endfunction()


function(add_files_to_project TARGET)

    cmake_parse_arguments(
        PARSE_ARGV 1 
        PARAM 
        "DISABLE_FOLDERS"
        "DIRECTORY"
        "SOURCE_FILES;HEADER_FILES;RESOURCE_FILES;FORM_FILES")

    #if (IS_DIRECTORY ${PARAM_DIRECTORY})
    #    # Carga automatica en función de las extensiones de archivo
    #    file(GLOB_RECURSE DIRECTORY_SOURCES_FILES RELATIVE "${PARAM_DIRECTORY}" CONFIGURE_DEPENDS *.cpp)
    #    file(GLOB_RECURSE DIRECTORY_HEADER_FILES RELATIVE "${PARAM_DIRECTORY}" CONFIGURE_DEPENDS *.h *.hpp)
    #    #file(GLOB_RECURSE RESOURCE_FILES RELATIVE ${PARAM_DIRECTORY} CONFIGURE_DEPENDS *.cpp)
    #    #file(GLOB_RECURSE FORM_FILES RELATIVE ${PARAM_DIRECTORY} CONFIGURE_DEPENDS *.h *.hpp)
    #endif()
	
    set(_SOURCES_FILES ${PARAM_SOURCE_FILES} "${PROJECT_SOURCE_FILES}")
    set(_HEADER_FILES ${PARAM_HEADER_FILES} "${PROJECT_HEADER_FILES}")
    set(_RESOURCE_FILES ${PARAM_RESOURCE_FILES} "${PROJECT_RESOURCE_FILES}")
    set(_FORM_FILES ${PARAM_FORM_FILES} "${PROJECT_FORM_FILES}")

    set(PROJECT_SOURCE_FILES "${_SOURCES_FILES}" PARENT_SCOPE)
    set(PROJECT_HEADER_FILES "${_HEADER_FILES}" PARENT_SCOPE)
    set(PROJECT_RESOURCE_FILES "${_RESOURCE_FILES}" PARENT_SCOPE)
    set(PROJECT_FORM_FILES "${_FORM_FILES}" PARENT_SCOPE)

    set(${TARGET}_SOURCE_FILES "${_SOURCES_FILES}" PARENT_SCOPE)
    set(${TARGET}_HEADER_FILES "${_HEADER_FILES}" PARENT_SCOPE)
    set(${TARGET}_RESOURCE_FILES "${_RESOURCE_FILES}" PARENT_SCOPE)
    set(${TARGET}_FORM_FILES "${_FORM_FILES}" PARENT_SCOPE)
 

	
    if (NOT ${PARAM_DISABLE_FOLDERS})
        add_files_to_source_group("Source Files" "${PARAM_SOURCE_FILES}")
        add_files_to_source_group("Header Files" "${PARAM_HEADER_FILES}")
        add_files_to_source_group("Resource Files" "${PARAM_RESOURCE_FILES}")
        add_files_to_source_group("Form Files" "${PARAM_FORM_FILES}")
    endif()
	


endfunction()




function(read_env_file filepath output_var)
    if(EXISTS ${filepath})
        file(READ ${filepath} env_content)
        string(REPLACE ";" "," env_content "${env_content}")
        string(REPLACE "\n" ";" env_content ${env_content})

        set(env_list "")

        foreach(line ${env_content})
            if(line MATCHES "^[^#]+=[^#]*$")
                string(REGEX REPLACE "(.+)=([^#]*)" "\\1" var ${line})
                string(REGEX REPLACE "(.+)=([^#]*)" "\\2" value ${line})
                string(REPLACE "," ";" value "${value}")
                set(ENV{${var}} "${value}")
                list(APPEND env_list "${var}")
            endif()
        endforeach()

        set(${output_var} "${env_list}" PARENT_SCOPE)  # Guardamos en una variable de salida
    else()
        message(WARNING ".env file not found at ${filepath}")
    endif()
endfunction()

function(set_user_enviroment_path project_name env_vars_list)

    set(USER_FILE "${PROJECT_BINARY_DIR}/${project_name}.vcxproj.user")
    set(env_entries "")

    foreach(env_var ${env_vars_list})

        if("${env_var}" STREQUAL "PATH")
			set(env_entries "${env_entries}${env_var}=%PATH%;$ENV{${env_var}}\n")
        else()
            set(env_entries "${env_entries}${env_var}=$ENV{${env_var}}\n")
        endif()

    endforeach()

    file(WRITE ${USER_FILE}
        "<?xml version=\"1.0\" encoding=\"utf-8\"?>\n"
        "<Project ToolsVersion=\"4.0\" xmlns=\"http://schemas.microsoft.com/developer/msbuild/2003\">\n"
        "  <PropertyGroup Condition=\"'$(Configuration)|$(Platform)'=='Debug|x64'\">\n"
        "    <LocalDebuggerEnvironment>${env_entries}</LocalDebuggerEnvironment>\n"
        "    <DebuggerFlavor>WindowsLocalDebugger</DebuggerFlavor>\n"
        "  </PropertyGroup>\n"
        "  <PropertyGroup Condition=\"'$(Configuration)|$(Platform)'=='Release|x64'\">\n"
        "    <LocalDebuggerEnvironment>${env_entries}</LocalDebuggerEnvironment>\n"
        "    <DebuggerFlavor>WindowsLocalDebugger</DebuggerFlavor>\n"
        "  </PropertyGroup>\n"
        "</Project>\n"
    )

endfunction()
