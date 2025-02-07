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


include(TidopUtils)


if (BUILD_TL_FEAT_MATCH AND TL_HAVE_OPENCV)
  
    project(FeatureMatching)
  
    add_files_to_project(${PROJECT_NAME}
                         SOURCE_FILES 
                             #base/features.cpp
                             base/keypointsfilter.cpp
                             features/agast.cpp
                             features/akaze.cpp
                             features/boost.cpp
                             features/brief.cpp
                             features/brisk.cpp
                             features/daisy.cpp
                             features/fast.cpp
                             features/freak.cpp
                             features/gftt.cpp
                             features/hog.cpp
                             features/kaze.cpp
                             features/latch.cpp
                             features/lss.cpp
                             features/lucid.cpp
                             features/msd.cpp
                             features/mser.cpp
                             features/orb.cpp
                             features/sift.cpp
                             features/star.cpp
                             features/surf.cpp
                             features/vgg.cpp
                             io/featreader.cpp
                             io/featwriter.cpp
                             io/matchreader.cpp
                             io/matchwriter.cpp
                             io/passpoints.cpp
                             io/impl/features/binaryreader.cpp
                             io/impl/features/binarywriter.cpp
                             io/impl/features/opencvreader.cpp
                             io/impl/features/opencvwriter.cpp
                             io/impl/features/txtreader.cpp
                             io/impl/features/txtwriter.cpp
                             io/impl/matches/binaryreader.cpp
                             io/impl/matches/binarywriter.cpp
                             io/impl/matches/opencvreader.cpp
                             io/impl/matches/opencvwriter.cpp
                             matching/bfmatch.cpp
                             matching/flann.cpp
                             matching/geomtest.cpp
                             matching/gsm.cpp
                             matching/robustmatch.cpp
                             ${TidopLib_SOURCE_DIR}/third_party/lss/lss.cpp
                             ${TidopLib_SOURCE_DIR}/third_party/msd/MSD.cpp
                             ${TidopLib_SOURCE_DIR}/third_party/msd/MSDImgPyramid.cpp
                         HEADER_FILES
                             base/evaluation.h
                             base/features.h
                             base/keypointsfilter.h
                             base/matcher.h
                             features/agast.h
                             #features/impl/agast.h
                             features/akaze.h
                             features/boost.h
                             features/brief.h
                             features/brisk.h
                             features/daisy.h
                             features/fast.h
                             features/freak.h
                             features/gftt.h
                             features/hog.h
                             features/kaze.h
                             features/latch.h
                             features/lss.h
                             features/lucid.h
                             features/msd.h
                             features/mser.h
                             features/orb.h
                             features/sift.h
                             features/star.h
                             features/surf.h
                             features/vgg.h
                             io/featreader.h
                             io/featwriter.h
                             io/matchreader.h
                             io/matchwriter.h
                             io/passpoints.h
                             io/impl/features/binaryreader.h
                             io/impl/features/binarywriter.h
                             io/impl/features/opencvreader.h
                             io/impl/features/opencvwriter.h
                             io/impl/features/txtreader.h
                             io/impl/features/txtwriter.h
                             io/impl/matches/binaryreader.h
                             io/impl/matches/binarywriter.h
                             io/impl/matches/opencvreader.h
                             io/impl/matches/opencvwriter.h
                             matching/bfmatch.h
                             matching/flann.h
                             matching/geomtest.h
                             matching/gsm.h
                             matching/robustmatch.h
                             ${TidopLib_SOURCE_DIR}/third_party/lss/lss.h
                             ${TidopLib_SOURCE_DIR}/third_party/msd/MSD.h
                             ${TidopLib_SOURCE_DIR}/third_party/msd/MSDImgPyramid.h)
    
    add_library(${PROJECT_NAME} ${LIB_TYPE}
                ${PROJECT_SOURCE_FILES}
                ${PROJECT_HEADER_FILES})

    add_library(TidopLib::${PROJECT_NAME}
                ALIAS ${PROJECT_NAME})

    target_include_directories(${PROJECT_NAME} PRIVATE 
                               $<$<BOOL:${TL_HAVE_GDAL}>:${GDAL_INCLUDE_DIR}> 
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
    message(STATUS "  TidopLib::${PROJECT_NAME} enabled")
  
elseif(BUILD_TL_CORE AND BUILD_TL_GEOMETRY)

    if(NOT TL_HAVE_OPENCV)
        message(STATUS "  TidopLib::FeatureMatching disabled [OPENCV not found]")
    else()
        message(STATUS "  TidopLib::FeatureMatching disabled")
    endif()

endif()