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
                             features/filter/KeyPointsFilterBySize.cpp
                             features/filter/KeyPointsFilterRemoveDuplicated.cpp
                             features/filter/KeyPointsFilterNBest.cpp
                             io/FeatReader.cpp
                             io/FeatWriter.cpp
                             io/MatchReader.cpp
                             io/MatchWriter.cpp
                             io/PassPoints.cpp
                             io/impl/features/BinaryReader.cpp
                             io/impl/features/BinaryWriter.cpp
                             io/impl/features/OpenCVReader.cpp
                             io/impl/features/OpenCVWriter.cpp
                             io/impl/features/TxtReader.cpp
                             io/impl/features/TxtWriter.cpp
                             io/impl/matches/BinaryReader.cpp
                             io/impl/matches/BinaryWriter.cpp
                             io/impl/matches/OpenCVReader.cpp
                             io/impl/matches/OpenCVWriter.cpp
                             matching/BruteForceMatching.cpp
                             matching/Flann.cpp
                             matching/GMS.cpp
                             matching/RobustMatching.cpp
                             matching/test/CrossTest.cpp
                             matching/test/GeometricTest.cpp
                             matching/test/RatioTest.cpp
                             matching/test/impl/Fundamental7PointsTest.cpp
                             matching/test/impl/Fundamental8PointsTest.cpp
                             matching/test/impl/FundamentalLMedsTest.cpp
                             matching/test/impl/FundamentalRANSACTest.cpp
                             matching/test/impl/FundamentalUsacTest.cpp
                             matching/test/impl/HomographyAllPointsTest.cpp
                             matching/test/impl/HomographyLMedsTest.cpp
                             matching/test/impl/HomographyRANSACTest.cpp
                             matching/test/impl/HomographyRHOTest.cpp
                             matching/test/impl/HomographyUsacTest.cpp
                             ${TidopLib_SOURCE_DIR}/third_party/lss/lss.cpp
                             ${TidopLib_SOURCE_DIR}/third_party/msd/MSD.cpp
                             ${TidopLib_SOURCE_DIR}/third_party/msd/MSDImgPyramid.cpp
                         HEADER_FILES
                             evaluation/Curve.h
                             evaluation/DETCurve.h
                             evaluation/PRCurve.h
                             evaluation/ROCCurve.h
                             features/agast.h
                             features/akaze.h
                             features/boost.h
                             features/brief.h
                             features/brisk.h
                             features/daisy.h
                             features/fast.h
                             features/features.h
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
                             features/filter/KeyPointsFilter.h
                             features/filter/KeyPointsFilterBySize.h
                             features/filter/KeyPointsFilterRemoveDuplicated.h
                             features/filter/KeyPointsFilterNBest.h
                             io/FeatReader.h
                             io/FeatWriter.h
                             io/MatchReader.h
                             io/MatchWriter.h
                             io/PassPoints.h
                             io/impl/features/BinaryReader.h
                             io/impl/features/BinaryWriter.h
                             io/impl/features/OpenCVReader.h
                             io/impl/features/OpenCVWriter.h
                             io/impl/features/TxtReader.h
                             io/impl/features/TxtWriter.h
                             io/impl/matches/BinaryReader.h
                             io/impl/matches/BinaryWriter.h
                             io/impl/matches/OpenCVReader.h
                             io/impl/matches/OpenCVWriter.h
                             matching/BruteForceMatching.h
                             matching/Flann.h
                             matching/GMS.h
                             matching/Matching.h
                             matching/RobustMatching.h
                             matching/test/AllPoints.h
                             matching/test/CrossTest.h
                             matching/test/GeometricTest.h
                             matching/test/LMedsTest.h
                             matching/test/RansacTest.h
                             matching/test/RatioTest.h
                             matching/test/RHOTest.h
                             matching/test/UsacTest.h
                             matching/test/impl/Fundamental7PointsTest.h
                             matching/test/impl/Fundamental8PointsTest.h
                             matching/test/impl/FundamentalLMedsTest.h
                             matching/test/impl/FundamentalRANSACTest.h
                             matching/test/impl/FundamentalUsacTest.h
                             matching/test/impl/HomographyAllPointsTest.h
                             matching/test/impl/HomographyLMedsTest.h
                             matching/test/impl/HomographyRANSACTest.h
                             matching/test/impl/HomographyRHOTest.h
                             matching/test/impl/HomographyUsacTest.h
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
                          TidopLib::Geometry
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
            FILES_MATCHING PATTERN "*.h"
            PATTERN "impl" EXCLUDE)
  
  
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