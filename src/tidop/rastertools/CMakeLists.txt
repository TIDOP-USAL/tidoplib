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
                             image.cpp
                             io/imgreader.cpp
                             io/imgwriter.cpp
                             io/formats.cpp
                             io/metadata.cpp
                             process/anaglyph.cpp
                             process/colorconvert.cpp
                             process/fourier.cpp
                             process/imgprocess.cpp
                             process/linedetector.cpp
                             process/skeleton.cpp
                             process/filters/bilateral.cpp
                             process/filters/blur.cpp
                             process/filters/box.cpp
                             process/filters/canny.cpp
                             process/filters/convolution.cpp
                             process/filters/gaussblur.cpp
                             process/filters/laplacian.cpp
                             process/filters/medianblur.cpp
                             process/filters/sobel.cpp
                             process/morph/blackhat.cpp
                             process/morph/closing.cpp
                             process/morph/dilate.cpp
                             process/morph/erode.cpp
                             process/morph/gradient.cpp
                             process/morph/morphoper.cpp
                             process/morph/opening.cpp
                             process/morph/tophat.cpp
                             process/transform/affine.cpp
                             process/transform/resize.cpp
                             process/whitebalance/grayworld.cpp
                             process/whitebalance/whitepatch.cpp
                             impl/io/gdalreader.cpp
                             impl/io/gdalwriter.cpp
                             impl/io/canonreader.cpp
                             impl/io/metadata/metadata.cpp
                             impl/io/metadata/bmp.cpp
                             impl/io/metadata/exif.cpp
                             impl/io/metadata/gif.cpp
                             impl/io/metadata/jpeg.cpp
                             impl/io/metadata/png.cpp
                             impl/io/metadata/tiff.cpp
                         HEADER_FILES
                             img.h
                             image.h
                             io/imgreader.h
                             io/imgwriter.h
                             io/formats.h
                             io/metadata.h
                             process/anaglyph.h
                             process/colorconvert.h
                             process/filters.h
                             process/fourier.h
                             process/imgprocess.h
                             process/linedetector.h
                             process/skeleton.h
                             process/filters/bilateral.h
                             process/filters/blur.h
                             process/filters/box.h
                             process/filters/canny.h
                             process/filters/convolution.h
                             process/filters/gaussblur.h
                             process/filters/laplacian.h
                             process/filters/medianblur.h
                             process/filters/sobel.h
                             process/morph/blackhat.h
                             process/morph/closing.h
                             process/morph/dilate.h
                             process/morph/erode.h
                             process/morph/gradient.h
                             process/morph/morphoper.h
                             process/morph/opening.h
                             process/morph/tophat.h
                             process/transform/affine.h
                             process/transform/resize.h
                             process/whitebalance/grayworld.h
                             process/whitebalance/whitepatch.h
                             impl/io/gdalreader.h
                             impl/io/gdalwriter.h
                             impl/io/canonreader.h
                             impl/io/metadata/metadata.h
                             impl/io/metadata/bmp.h
                             impl/io/metadata/exif.h
                             impl/io/metadata/gif.h
                             impl/io/metadata/jpeg.h
                             impl/io/metadata/png.h
                             impl/io/metadata/tiff.h)

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
            PATTERN "impl" EXCLUDE)
            
    set(TidopLib_LIBRARIES ${TidopLib_LIBRARIES} ${PROJECT_NAME})
    
    set(TL_HAVE_RASTER_TOOLS YES CACHE INTERNAL "")
    message(STATUS "  TidopLib::${PROJECT_NAME} enabled")

elseif(BUILD_TL_CORE AND BUILD_TL_MATH AND BUILD_TL_GEOMETRY AND BUILD_TL_GRAPHIC)  

    message(STATUS "  TidopLib::RasterTools disabled")
    message(STATUS "  TidopLib::Geospatial disabled [Module dependent on TidopLib::RasterTools that is deactivated]") 

endif()