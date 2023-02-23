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
 

##########################################################################
#  TidopLib Core module                                                  #
##########################################################################

if(BUILD_TL_CORE)

    project(Core)

    add_library(${PROJECT_NAME} ${LIB_TYPE}
                app.cpp
                app.h
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
                task.cpp
                task.h
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
                path.h
                xml.cpp
                xml.h
                endian.h)

    add_library(TidopLib::${PROJECT_NAME}
                ALIAS ${PROJECT_NAME})

    target_include_directories(${PROJECT_NAME} PUBLIC
                               $<BUILD_INTERFACE:${TidopLib_SOURCE_DIR}/src>
                               $<INSTALL_INTERFACE:include>)
    target_include_directories(${PROJECT_NAME} PRIVATE ${GDAL_INCLUDE_DIR})

    target_link_libraries(${PROJECT_NAME} PUBLIC
                          ${Boost_FILESYSTEM_LIBRARY} 
                          ${Boost_SYSTEM_LIBRARY}
                          $<$<BOOL:${TL_HAVE_OPENMP}>:OpenMP::OpenMP_CXX>
                          $<$<BOOL:${TL_HAVE_GDAL}>:${GDAL_LIBRARY}>
                          $<$<BOOL:${UNIX}>:-lpthread>)
    
    set_target_properties(${PROJECT_NAME} PROPERTIES 
                          FOLDER "modules"
                          OUTPUT_NAME "tl_core")

    if(BUILD_SHARED_LIBS)
  
        set_target_properties(${PROJECT_NAME} PROPERTIES
                              DEFINE_SYMBOL TIDOPLIB_EXPORTS)  
    endif()
    
    install(TARGETS ${PROJECT_NAME}
            EXPORT "${TARGETS_EXPORT_NAME}"
            RUNTIME DESTINATION ${TL_INSTALL_BINARIES_PREFIX}/bin
            LIBRARY DESTINATION ${TL_INSTALL_BINARIES_PREFIX}/lib
            ARCHIVE DESTINATION ${TL_INSTALL_BINARIES_PREFIX}/lib)
                
    install(DIRECTORY .
            DESTINATION include/tidop/core
            FILES_MATCHING PATTERN "*.h")
    
    
    set(TidopLib_LIBRARIES ${TidopLib_LIBRARIES} ${PROJECT_NAME})
    
    set(TL_HAVE_CORE YES CACHE INTERNAL "")
    message(STATUS "  TidopLib::Core enabled")
    
else()

    message(STATUS "  TidopLib::Core disabled")
    message(STATUS "  TidopLib::Math disabled         [Module dependent on TidopLib::Core that is deactivated]")
    message(STATUS "  TidopLib::Geom disabled         [Module dependent on TidopLib::Core that is deactivated]")
    message(STATUS "  TidopLib::Img disabled          [Module dependent on TidopLib::Core that is deactivated]")
    message(STATUS "  TidopLib::ImgProcess disabled   [Module dependent on TidopLib::Core that is deactivated]")
    message(STATUS "  TidopLib::Graphic disabled      [Module dependent on TidopLib::Core that is deactivated]")
    message(STATUS "  TidopLib::FeatMatch disabled    [Module dependent on TidopLib::Core that is deactivated]")
    message(STATUS "  TidopLib::Vect disabled         [Module dependent on TidopLib::Core that is deactivated]")
    message(STATUS "  TidopLib::Geospatial disabled   [Module dependent on TidopLib::Core that is deactivated]")
    message(STATUS "  TidopLib::Experimental disabled [Module dependent on TidopLib::Core that is deactivated]")
  
endif(BUILD_TL_CORE)
