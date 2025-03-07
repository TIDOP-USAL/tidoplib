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

find_package(Doxygen OPTIONAL_COMPONENTS dot mscgen dia)

if(DOXYGEN_FOUND)

    message(STATUS "DOXYGEN_FOUND ${DOXYGEN_FOUND}")

    set(DOXYFILE  ${CMAKE_CURRENT_SOURCE_DIR}/Doxyfile.in)
    set(ROOTFILE  ${CMAKE_CURRENT_SOURCE_DIR}/root.markdown)
    #set(TUTORIALS ${CMAKE_CURRENT_SOURCE_DIR}/tutorials.markdown)
    set(DOC_OUT   ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile)

    configure_file(${DOXYFILE}  ${DOC_OUT} @ONLY)
    #configure_file(${ROOTFILE}  ${DOC_OUT} @ONLY)
    #configure_file(${TUTORIALS} ${DOC_OUT} @ONLY)

    message(STATUS "Doxygen build started.")

    #add_custom_target(doxygen
    #  COMMAND ${DOXYGEN_EXECUTABLE} ${doxyfile}
    #  DEPENDS ${doxyfile} ${rootfile} ${tutorials}
    #)
    #
    #install(DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/doxygen/html
    #  DESTINATION "${TIDOP_DOC_INSTALL_PATH}"
    #  COMPONENT "docs" OPTIONAL
    #)
    
    add_custom_target( doc ALL
          COMMAND ${DOXYGEN_EXECUTABLE} ${DOC_OUT}
          WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
          COMMENT "Generating API documentation with Doxygen"
          VERBATIM )
    
    install(DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/html
            DESTINATION share/doc
            COMPONENT "docs" OPTIONAL)
      
else (DOXYGEN_FOUND)
    message(WARNING "Doxygen need to be installed to generate the doxygen documentation")
endif(DOXYGEN_FOUND)


