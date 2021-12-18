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
 
 # TidopLib Core module

if(BUILD_TL_CORE)

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
                progress.cpp
                progress.h
                event.cpp
                event.h
                licence.cpp
                licence.h
                gdalreg.cpp
                gdalreg.h
                chrono.cpp
                chrono.h
                concurrency.cpp
                concurrency.h
                path.cpp
                path.h)
    
    target_include_directories(${PROJECT_NAME} PUBLIC
                               $<BUILD_INTERFACE:${TidopLib_SOURCE_DIR}/src>
                               $<INSTALL_INTERFACE:include/tidop/core>)
    
    target_link_libraries(${PROJECT_NAME} PRIVATE
                          ${Boost_FILESYSTEM_LIBRARY} 
                          ${Boost_SYSTEM_LIBRARY})
    if (HAVE_OPENMP)
        target_link_libraries(${PROJECT_NAME} PRIVATE
                              OpenMP::OpenMP_CXX)
    endif(HAVE_OPENMP)
    
    if (HAVE_GDAL)
        target_link_libraries(${PROJECT_NAME} PRIVATE
                              ${GDAL_LIBRARY})
    endif(HAVE_GDAL)
    
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
    
else()

    message(STATUS "TidopLib::tl_core disabled")
    message(STATUS "TidopLib::tl_math disabled         [Module dependent on tl_core that is deactivated]")
    message(STATUS "TidopLib::tl_geom disabled         [Module dependent on tl_core that is deactivated]")
    message(STATUS "TidopLib::tl_img disabled          [Module dependent on tl_core that is deactivated]")
    message(STATUS "TidopLib::tl_imgprocess disabled   [Module dependent on tl_core that is deactivated]")
    message(STATUS "TidopLib::tl_graphic disabled      [Module dependent on tl_core that is deactivated]")
    message(STATUS "TidopLib::tl_featmatch disabled    [Module dependent on tl_core that is deactivated]")
    message(STATUS "TidopLib::tl_vect disabled         [Module dependent on tl_core that is deactivated]")
    message(STATUS "TidopLib::tl_geospatial disabled   [Module dependent on tl_core that is deactivated]")
    message(STATUS "TidopLib::tl_experimental disabled [Module dependent on tl_core that is deactivated]")
  
endif(BUILD_TL_CORE)