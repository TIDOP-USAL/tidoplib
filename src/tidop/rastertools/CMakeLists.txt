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
#  TidopLib Raster Tools module                                          #
##########################################################################

include(TidopUtils)

if(BUILD_TL_RASTER_TOOLS AND TL_HAVE_OPENCV)
  
    project(RasterTools)
  
    add_files_to_project(${PROJECT_NAME} 
                         SOURCE_FILES 
                             img.cpp
                             Image.cpp
                             io/Reader.cpp
                             io/Writer.cpp
                             io/Formats.cpp
                             io/Metadata.cpp
                             io/impl/CanonReader.cpp
                             io/impl/GdalReader.cpp
                             io/impl/GdalWriter.cpp
                             io/impl/ImageReader.cpp
                             io/impl/ImageWriter.cpp
                             io/private/DataTypeConverter.cpp
                             io/private/gdal.cpp
                             process/Anaglyph.cpp
                             process/ColorConvert.cpp
                             process/Fourier.cpp
                             process/ImgProcess.cpp
                             process/LineDetector.cpp
                             process/Skeleton.cpp
                             process/filters/Bilateral.cpp
                             process/filters/Blur.cpp
                             process/filters/Box.cpp
                             process/filters/Canny.cpp
                             process/filters/Convolution.cpp
                             process/filters/GaussBlur.cpp
                             process/filters/Laplacian.cpp
                             process/filters/MedianBlur.cpp
                             process/filters/Sobel.cpp
                             process/morph/BlackHat.cpp
                             process/morph/Closing.cpp
                             process/morph/Dilate.cpp
                             process/morph/Erode.cpp
                             process/morph/Gradient.cpp
                             process/morph/MorphOper.cpp
                             process/morph/Opening.cpp
                             process/morph/Tophat.cpp
                             process/transform/Affine.cpp
                             process/transform/Resize.cpp
                             process/whitebalance/Grayworld.cpp
                             process/whitebalance/WhitePatch.cpp
                         HEADER_FILES
                             img.h
                             Image.h
                             DataType.h
                             io/Reader.h
                             io/Writer.h
                             io/Formats.h
                             io/Metadata.h
                             io/impl/CanonReader.h
                             io/impl/GdalReader.h
                             io/impl/GdalWriter.h
                             io/impl/ImageReader.h
                             io/impl/ImageWriter.h
                             io/private/DataTypeConverter.h
                             io/private/gdal.h
                             process/Anaglyph.h
                             process/ColorConvert.h
                             process/Filters.h
                             process/Fourier.h
                             process/ImgProcess.h
                             process/LineDetector.h
                             process/Skeleton.h
                             process/filters/Bilateral.h
                             process/filters/Blur.h
                             process/filters/Box.h
                             process/filters/Canny.h
                             process/filters/Convolution.h
                             process/filters/GaussBlur.h
                             process/filters/Laplacian.h
                             process/filters/MedianBlur.h
                             process/filters/Sobel.h
                             process/morph/BlackHat.h
                             process/morph/Closing.h
                             process/morph/Dilate.h
                             process/morph/Erode.h
                             process/morph/Gradient.h
                             process/morph/MorphOper.h
                             process/morph/Opening.h
                             process/morph/TopHat.h
                             process/transform/Affine.h
                             process/transform/Resize.h
                             process/whitebalance/GrayWorld.h
                             process/whitebalance/WhitePatch.h)

    add_library(${PROJECT_NAME} ${LIB_TYPE}
                ${PROJECT_SOURCE_FILES}
                ${PROJECT_HEADER_FILES})

    add_library(TidopLib::${PROJECT_NAME}
                ALIAS ${PROJECT_NAME})

    if(MSVC)
        target_compile_options(${PROJECT_NAME} PRIVATE "/bigobj")
    endif(MSVC)

    target_include_directories(${PROJECT_NAME} PUBLIC 
                               $<$<BOOL:${TL_HAVE_GDAL}>:${GDAL_INCLUDE_DIR}>)
    
    target_compile_definitions(${PROJECT_NAME} PUBLIC
                               $<$<BOOL:${TL_HAVE_OPENBLAS}>:HAVE_LAPACK_CONFIG_H>
                               $<$<BOOL:${TL_HAVE_OPENBLAS}>:LAPACK_COMPLEX_STRUCTURE>)

    target_link_libraries(${PROJECT_NAME} PUBLIC
                          TidopLib::Core
                          TidopLib::Geometry
                          TidopLib::Math
                          TidopLib::Graphic
                          $<$<BOOL:${TL_HAVE_GDAL}>:${GDAL_LIBRARY}>
                          $<$<BOOL:${TL_HAVE_OPENBLAS}>:OpenBLAS::OpenBLAS>
                          ${OpenCV_LIBS})

    set_target_properties(${PROJECT_NAME} PROPERTIES 
                          FOLDER "modules"
                          OUTPUT_NAME "tl_raster_tools")


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
            DESTINATION include/tidop/rastertools
            FILES_MATCHING PATTERN "*.h"
            PATTERN "impl" EXCLUDE
            PATTERN "private" EXCLUDE)
            
    set(TidopLib_LIBRARIES ${TidopLib_LIBRARIES} ${PROJECT_NAME})
    
    set(TL_HAVE_RASTER_TOOLS YES CACHE INTERNAL "")
    message(STATUS "  TidopLib::${PROJECT_NAME} enabled")

elseif(BUILD_TL_CORE AND BUILD_TL_MATH AND BUILD_TL_GEOMETRY AND BUILD_TL_GRAPHIC)  

    message(STATUS "  TidopLib::RasterTools disabled")
    message(STATUS "  TidopLib::Geospatial disabled [Module dependent on TidopLib::RasterTools that is deactivated]") 

endif()