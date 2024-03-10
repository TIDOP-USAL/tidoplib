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
#  TidopLib image processing module                                      #
##########################################################################


if (BUILD_TL_IMG_PROCESS)

    project(ImgProcess)
  
    add_files_to_project(${PROJECT_NAME} 
                         SOURCE_FILES 
                            anaglyph.cpp
                            colorconvert.cpp
                            fourier.cpp
                            imgprocess.cpp
                            imgtransform.cpp
                            linedetector.cpp
                            skeleton.cpp
                            whitebalance.cpp
                            morphologicaloper.cpp
                            filter/bilateral.cpp
                            filter/blur.cpp
                            filter/box.cpp
                            filter/canny.cpp
                            filter/convolution.cpp
                            filter/gaussblur.cpp
                            filter/laplacian.cpp
                            filter/medianblur.cpp
                            filter/sobel.cpp
                         HEADER_FILES
                            anaglyph.h
                            colorconvert.h
                            filters.h
                            fourier.h
                            imgprocess.h
                            imgtransform.h
                            linedetector.h
                            skeleton.h
                            whitebalance.h
                            morphologicaloper.h
                            filter/bilateral.h
                            filter/blur.h
                            filter/box.h
                            filter/canny.h
                            filter/convolution.h
                            filter/gaussblur.h
                            filter/laplacian.h
                            filter/medianblur.h
                            filter/sobel.h)

    add_library(${PROJECT_NAME} ${LIB_TYPE}
                ${PROJECT_SOURCE_FILES}
                ${PROJECT_HEADER_FILES})

    add_library(TidopLib::${PROJECT_NAME}
                ALIAS ${PROJECT_NAME})

    target_link_libraries(${PROJECT_NAME} PRIVATE
                          TidopLib::Core
                          TidopLib::Graphic
                          ${OpenCV_LIBS})  

    set_target_properties(${PROJECT_NAME} PROPERTIES 
                          FOLDER "modules"
                          OUTPUT_NAME "tl_imgprocess")



    if(BUILD_SHARED_LIBS)
        set_target_properties(${PROJECT_NAME} PROPERTIES
                              DEFINE_SYMBOL TIDOPLIB_EXPORTS)
    endif()
  
    install(TARGETS ${PROJECT_NAME}
            EXPORT "${TARGETS_EXPORT_NAME}"
            RUNTIME DESTINATION ${TL_INSTALL_BINARIES_PREFIX}bin
            LIBRARY DESTINATION ${TL_INSTALL_BINARIES_PREFIX}lib
            ARCHIVE DESTINATION ${TL_INSTALL_BINARIES_PREFIX}lib) 

    install(DIRECTORY . 
            DESTINATION include/tidop/imgprocess
            FILES_MATCHING PATTERN "*.h")
          
    set(TidopLib_LIBRARIES ${TidopLib_LIBRARIES} ${PROJECT_NAME})       
  
    set(TL_HAVE_IMG_PROCESS YES CACHE INTERNAL "")
    message(STATUS "  TidopLib::${PROJECT_NAME} enabled")

elseif(BUILD_TL_CORE AND BUILD_TL_GRAPHIC)

    message(STATUS "  TidopLib::${PROJECT_NAME} disabled")

endif(BUILD_TL_IMG_PROCESS)
