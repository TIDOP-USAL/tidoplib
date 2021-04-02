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
 
 # Modulo Core de la librería TIDOP

project(tl_core)

add_library(${PROJECT_NAME} ${LIB_TYPE}
            console.cpp
            console.h
            defs.h
            exception.cpp
            exception.h
            flags.h
            messages.cpp
            messages.h
            log.cpp
            log.h
            utils.cpp
            utils.h
            process.cpp
            process.h
            licence.cpp
            licence.h
            gdalreg.cpp
            gdalreg.h
            chrono.cpp
            chrono.h
            concurrency.cpp
            concurrency.h
)
  

set_property(TARGET ${PROJECT_NAME}
             PROPERTY FOLDER "modules")


if(BUILD_SHARED_LIBS)

    install(TARGETS ${PROJECT_NAME}
            EXPORT "${TARGETS_EXPORT_NAME}"
            RUNTIME DESTINATION ${TL_INSTALL_BINARIES_PREFIX}/bin
            LIBRARY DESTINATION ${TL_INSTALL_BINARIES_PREFIX}/lib
            ARCHIVE DESTINATION ${TL_INSTALL_BINARIES_PREFIX}/lib)
	
    set_target_properties(${PROJECT_NAME} PROPERTIES
                          DEFINE_SYMBOL TIDOPLIB_EXPORTS)

    target_link_libraries(${PROJECT_NAME} 
                          ${Boost_FILESYSTEM_LIBRARY} 
                          ${Boost_SYSTEM_LIBRARY})
    
    if (HAVE_GDAL)
         target_link_libraries(${PROJECT_NAME} ${GDAL_LIBRARY})
    endif(HAVE_GDAL)

else()

    install(TARGETS ${PROJECT_NAME}
            EXPORT "${TARGETS_EXPORT_NAME}"
            LIBRARY DESTINATION ${TL_INSTALL_BINARIES_PREFIX}/lib 
            ARCHIVE DESTINATION ${TL_INSTALL_BINARIES_PREFIX}/lib)

endif()


install(DIRECTORY .
        DESTINATION include/tidop/core
        FILES_MATCHING PATTERN "*.h")


set(TidopLib_LIBRARIES ${TidopLib_LIBRARIES} ${PROJECT_NAME})

set(HAVE_TL_CORE YES CACHE INTERNAL "")
message(STATUS "TidopLib::tl_core enabled")