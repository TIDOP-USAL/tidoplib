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
#  TidopLib math module                                                  #
##########################################################################

include(TidopUtils)


if(BUILD_TL_MATH)

    project(Math)
    
    add_files_to_project(${PROJECT_NAME} 
                         HEADER_FILES
                             math.h
                             angles.h
                             statistics.h
                             mathutils.h
                             simd.h
                             data.h
                             algebra/quaternion.h
                             algebra/euler_angles.h
                             algebra/rotations.h
                             algebra/rotation_matrix.h
                             algebra/rotation_convert.h
                             algebra/axis_angle.h
                             algebra/matrix.h
                             algebra/matrices.h
                             algebra/vector.h
                             algebra/svd.h
                             algebra/lu.h
                             algebra/qr.h
                             algebra/cholesky.h
                             algebra/matrix/base.h
                             algebra/matrix/block.h
                             algebra/matrix/col.h
                             algebra/matrix/row.h
                             geometry/affine.h
                             geometry/helmert.h
                             geometry/rotation.h
                             geometry/scaling.h
                             geometry/transform.h
                             geometry/translation.h
                             geometry/umeyama.h
                             statistic/algorithm.h
                             statistic/biweightmidvariance.h
                             statistic/descriptive.h
                             statistic/iqr.h
                             statistic/mad.h
                             statistic/mean.h
                             statistic/median.h
                             statistic/mode.h
                             statistic/pearson.h
                             statistic/quantile.h
                             statistic/range.h
                             statistic/rms.h
                             statistic/series.h
                             statistic/confmat.h
		                     statistic/covariance.h
                             statistic/skewness.h
                             statistic/skewness.impl.h
                             statistic/stddev.h
                             statistic/tukeyfences.h
                             statistic/variance.h
                             lapack.h
                             blas.h
                             cuda.h)
                
    add_library(${PROJECT_NAME} INTERFACE
                ${PROJECT_SOURCE_FILES}
                ${PROJECT_HEADER_FILES})
    
    add_library(TidopLib::${PROJECT_NAME} 
                ALIAS ${PROJECT_NAME})

    if(MSVC)
        target_compile_options(${PROJECT_NAME} INTERFACE "/bigobj")
    endif(MSVC)

    target_include_directories(${PROJECT_NAME} INTERFACE 
                               $<$<BOOL:${TL_HAVE_CUDA}>:${CUDA_INCLUDE_DIRS}>)

    target_compile_definitions(${PROJECT_NAME} INTERFACE
                               $<$<BOOL:${TL_HAVE_OPENBLAS}>:HAVE_LAPACK_CONFIG_H>
                               $<$<BOOL:${TL_HAVE_OPENBLAS}>:LAPACK_COMPLEX_STRUCTURE>)

    target_link_libraries(${PROJECT_NAME} INTERFACE 
                          TidopLib::Core
                          $<$<BOOL:${TL_HAVE_OPENBLAS}>:OpenBLAS::OpenBLAS>
                          $<$<BOOL:${TL_HAVE_CUDA}>:${CUDA_LIBRARIES}>
                          $<$<BOOL:${TL_HAVE_CUDA}>:${CUDA_cublas_LIBRARY}>
                          #$<$<BOOL:${TL_HAVE_CUDA}>:${CUDA_cusolver_LIBRARY}>
                          )
       
    set_target_properties(${PROJECT_NAME} PROPERTIES
                          FOLDER "modules"
                          OUTPUT_NAME "tl_math")

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
            DESTINATION include/tidop/math
            FILES_MATCHING PATTERN "*.h")
            
    set(TidopLib_LIBRARIES ${TidopLib_LIBRARIES} ${PROJECT_NAME})
    
    set(TL_HAVE_MATH YES CACHE INTERNAL "")
    message(STATUS "  TidopLib::Math enabled")

elseif(BUILD_TL_CORE)
    
    message(STATUS "  TidopLib::Math disabled")
    message(STATUS "  TidopLib::Geom disabled         [Module dependent on TidopLib::Math that is deactivated]") 
    message(STATUS "  TidopLib::Img disabled          [Module dependent on TidopLib::Math that is deactivated]") 
    message(STATUS "  TidopLib::Geospatial disabled   [Module dependent on TidopLib::Math that is deactivated]") 

endif(BUILD_TL_MATH)