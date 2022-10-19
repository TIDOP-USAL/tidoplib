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

# Modulo featmatch de la librería TIDOP que incluye detectores/descriptores y matching

if (BUILD_TL_FEAT_MATCH AND TL_HAVE_OPENCV)
  
    project(FeatMatch)
  
    add_library(${PROJECT_NAME} ${LIB_TYPE}
                agast.cpp
                agast.h
                akaze.cpp
                akaze.h
                boost.cpp
                boost.h
                brief.cpp
                brief.h
                brisk.cpp
                brisk.h
                daisy.cpp
                daisy.h
                evaluation.h
                fast.cpp
                fast.h
                features.h
                featio.h
                featio.cpp
                freak.cpp
                freak.h
                gftt.cpp
                gftt.h
                hog.cpp
                hog.h
                kaze.cpp
                kaze.h
                latch.cpp
                latch.h
                #lss.cpp
                #lss.h
                lucid.cpp
                lucid.h
                matcher.h
                matchio.cpp
                matchio.h
                msd.cpp
                msd.h
                mser.cpp
                mser.h
                orb.cpp
                orb.h
                sift.cpp
                sift.h
                star.cpp
                star.h
                surf.cpp
                surf.h
                vgg.h
                vgg.cpp
                gsm.cpp
                gsm.h
                robustmatch.cpp
                robustmatch.h
                bfmatch.cpp
                bfmatch.h
                flann.cpp
                flann.h
                keypointsfilter.cpp
                keypointsfilter.h
                geomtest.cpp
                geomtest.h
                ${TidopLib_SOURCE_DIR}/third_party/msd/MSD.h
                ${TidopLib_SOURCE_DIR}/third_party/msd/MSD.cpp
                ${TidopLib_SOURCE_DIR}/third_party/msd/MSDImgPyramid.h
                ${TidopLib_SOURCE_DIR}/third_party/msd/MSDImgPyramid.cpp)
    
    add_library(TidopLib::${PROJECT_NAME}
                ALIAS ${PROJECT_NAME})

    target_include_directories(${PROJECT_NAME} PRIVATE 
                               ${GDAL_INCLUDE_DIR} 
                               ${CMAKE_SOURCE_DIR}/third_party)

    target_link_libraries(${PROJECT_NAME} PUBLIC
                          TidopLib::Core 
                          TidopLib::Geom
                          $<$<BOOL:${TL_HAVE_GDAL}>:${GDAL_LIBRARY}>
                          ${OpenCV_LIBS})

    set_target_properties(${PROJECT_NAME} PROPERTIES 
                          FOLDER "modules"
                          OUTPUT_NAME "tl_featmatch")
  
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
            DESTINATION include/tidop/featmatch
            FILES_MATCHING PATTERN "*.h")
  
  
    set(TidopLib_LIBRARIES ${TidopLib_LIBRARIES} ${PROJECT_NAME})

    set(TL_HAVE_FEAT_MATCH YES CACHE INTERNAL "")
    message(STATUS "  TidopLib::FeatMatch enabled")
  
elseif(BUILD_TL_CORE AND BUILD_TL_GEOMETRY)

    if(NOT TL_HAVE_OPENCV)
        message(STATUS "  TidopLib::FeatMatch disabled [OPENCV not found]")
    else()
        message(STATUS "  TidopLib::FeatMatch disabled")
    endif()

endif()