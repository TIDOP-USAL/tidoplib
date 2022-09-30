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


function(add_files_to_groups TARGET)

    cmake_parse_arguments(
        PARSE_ARGV 1 
        PARAM 
        "DISABLE_FOLDERS"
        ""
        "SOURCE_FILES;HEADER_FILES;RESOURCE_FILES;FORM_FILES")

    set(${TARGET}_SOURCE_FILES "${PARAM_SOURCE_FILES}" PARENT_SCOPE)
    set(${TARGET}_HEADER_FILES "${PARAM_HEADER_FILES}" PARENT_SCOPE)
    set(${TARGET}_RESOURCE_FILES "${PARAM_RESOURCE_FILES}" PARENT_SCOPE)
    set(${TARGET}_FORM_FILES "${PARAM_FORM_FILES}" PARENT_SCOPE)
 
    if (NOT ${PARAM_DISABLE_FOLDERS})
        add_files_to_source_group("Source Files" "${PARAM_SOURCE_FILES}")
        add_files_to_source_group("Header Files" "${PARAM_HEADER_FILES}")
        add_files_to_source_group("Resource Files" "${PARAM_RESOURCE_FILES}")
        add_files_to_source_group("Form Files" "${PARAM_FORM_FILES}")
	endif()
	
endfunction()
