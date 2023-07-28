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

include(TidopUtils)


if (BUILD_TL_FEAT_MATCH AND TL_HAVE_OPENCV)
  
    project(FeatMatch)
  
    add_files_to_project(${PROJECT_NAME}
                         SOURCE_FILES 
                             agast.cpp
                             akaze.cpp
                             boost.cpp
                             brief.cpp
                             brisk.cpp
                             daisy.cpp
                             fast.cpp
                             featio.cpp
                             freak.cpp
                             gftt.cpp
                             hog.cpp
                             kaze.cpp
                             latch.cpp
                             lss.cpp
                             lucid.cpp
                             matchio.cpp
                             msd.cpp
                             mser.cpp
                             orb.cpp
                             sift.cpp
                             star.cpp
                             surf.cpp
                             vgg.cpp
                             gsm.cpp
                             robustmatch.cpp
                             bfmatch.cpp
                             flann.cpp
                             keypointsfilter.cpp
                             geomtest.cpp
                             ${TidopLib_SOURCE_DIR}/third_party/lss/lss.cpp
                             ${TidopLib_SOURCE_DIR}/third_party/msd/MSD.cpp
                             ${TidopLib_SOURCE_DIR}/third_party/msd/MSDImgPyramid.cpp
                         HEADER_FILES
                             agast.h
                             akaze.h
                             boost.h
                             brief.h
                             brisk.h
                             daisy.h
                             evaluation.h
                             fast.h
                             features.h
                             featio.h
                             freak.h
                             gftt.h
                             hog.h
                             kaze.h
                             latch.h
                             lss.h
                             lucid.h
                             matcher.h
                             matchio.h
                             msd.h
                             mser.h
                             orb.h
                             sift.h
                             star.h
                             surf.h
                             vgg.h
                             gsm.h
                             robustmatch.h
                             bfmatch.h
                             flann.h
                             keypointsfilter.h
                             geomtest.h
                             ${TidopLib_SOURCE_DIR}/third_party/lss/lss.h
                             ${TidopLib_SOURCE_DIR}/third_party/msd/MSD.h
                             ${TidopLib_SOURCE_DIR}/third_party/msd/MSDImgPyramid.h)
    
    add_library(${PROJECT_NAME} ${LIB_TYPE}
                ${PROJECT_SOURCE_FILES}
                ${PROJECT_HEADER_FILES})

    add_library(TidopLib::${PROJECT_NAME}
                ALIAS ${PROJECT_NAME})

    target_include_directories(${PROJECT_NAME} PRIVATE 
                               ${GDAL_INCLUDE_DIR} 
                               ${CMAKE_SOURCE_DIR}/third_party)

    target_link_libraries(${PROJECT_NAME} PUBLIC
                          TidopLib::Core 
                          TidopLib::Geom
                          TidopLib::Math
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
            RUNTIME DESTINATION ${TL_INSTALL_BINARIES_PREFIX}bin
            LIBRARY DESTINATION ${TL_INSTALL_BINARIES_PREFIX}lib
            ARCHIVE DESTINATION ${TL_INSTALL_BINARIES_PREFIX}lib)

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