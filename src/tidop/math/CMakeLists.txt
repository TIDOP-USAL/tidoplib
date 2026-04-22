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
                             mathutils.h
                             algebra/decomp/cholesky.h
                             algebra/decomp/eigen.h
                             algebra/decomp/lu.h
                             algebra/decomp/qr.h
                             algebra/decomp/svd.h
                             algebra/expr/AddExpr.h
                             algebra/expr/DivScalarExpr.h
                             algebra/expr/MatMulExpr.h
                             algebra/expr/MulScalarExpr.h
                             algebra/expr/SubExpr.h
                             algebra/expr/TransposeExpr.h
                             algebra/expr/UnaryMinusExpr.h
                             algebra/expr/MatVecMulExpr.h
                             algebra/expr/VecAddExpr.h
                             algebra/expr/VecDivExpr.h
                             algebra/expr/VecDivScalarExpr.h
                             algebra/expr/VecMulExpr.h
                             algebra/expr/VecMulScalarExpr.h
                             algebra/expr/VecSubExpr.h
                             algebra/expr/VecUnaryMinusExpr.h
                             #algebra/matrix/base.h
                             #algebra/matrix/block.h
                             #algebra/matrix/col.h
                             #algebra/matrix/row.h
                             algebra/matrix/Kernel.h
                             algebra/matrix/Matrix.h
                             algebra/matrix/MatrixBase.h
                             algebra/matrix/MatrixBlock.h
                             algebra/matrix/MatrixCol.h
                             algebra/matrix/MatrixRow.h
                             algebra/matrix/detail/AdjugateMatrix.h
                             algebra/matrix/detail/Cofactor.h
                             algebra/matrix/detail/CofactorMatrix.h
                             algebra/matrix/detail/Determinant.h
                             algebra/matrix/detail/FirstMinor.h
                             algebra/matrix/detail/Inverse.h
                             algebra/matrix/detail/ReducedRowEchelonForm.h
                             algebra/matrix/detail/RowEchelonForm.h
                             algebra/rotations/axis_angle.h
                             algebra/rotations/euler_angles.h
                             algebra/rotations/quaternion.h
                             algebra/rotations/rotation_convert.h
                             algebra/rotations/rotation_matrix.h
                             algebra/rotations/rotations.h
                             algebra/vector/Vector.h
                             algebra/vector/VectorBase.h
                             algebra/vector/detail/MatVecMul.h
                             algebra/vector/detail/MatVecMulCPP.h
                             algebra/vector/detail/MatVecMulSIMD.h
                             algebra/BaseExpr.h
                             algebra/matrices.h
                             #algebra/matrix.h
                             #algebra/vector.h
                             base/blas.h
                             base/cuda.h
                             base/Concepts.h
                             base/data.h
                             base/lapack.h
                             base/simd.h
                             base/Traits.h
                             geometry/affine.h
                             geometry/helmert.h
                             geometry/rotation.h
                             geometry/scaling.h
                             geometry/transform.h
                             geometry/translation.h
                             geometry/umeyama.h
                             geometry/analytic/line.h
                             geometry/analytic/plane.h
                             geometry/analytic/sphere.h
                             statistic/descriptive.h
                             statistic/association/pearson.h
                             statistic/association/covariance.h
                             statistic/base/algorithm.h
                             statistic/base/series.h
                             statistic/classification/confmat.h
                             statistic/descriptive/mean.h
                             statistic/descriptive/median.h
                             statistic/descriptive/mode.h                             
                             statistic/descriptive/quantile.h
                             statistic/descriptive/range.h                        
                             statistic/descriptive/stddev.h
                             statistic/descriptive/tukeyfences.h
                             statistic/descriptive/variance.h
                             statistic/shape/kurtosis.h
                             statistic/shape/kurtosis.impl.h
                             statistic/ratios/cv.h
                             statistic/ratios/zscore.h
                             statistic/ratios/rms.h     
                             statistic/robust/biweightmidvariance.h
                             statistic/robust/iqr.h
                             statistic/robust/mad.h
                             statistic/shape/skewness.h
                             statistic/shape/skewness.impl.h)
                
    add_library(${PROJECT_NAME} INTERFACE
                ${PROJECT_SOURCE_FILES}
                ${PROJECT_HEADER_FILES})
    
    add_library(TidopLib::${PROJECT_NAME} 
                ALIAS ${PROJECT_NAME})

    if(MSVC)
        target_compile_options(${PROJECT_NAME} INTERFACE "/bigobj")
    endif(MSVC)

    target_include_directories(${PROJECT_NAME} INTERFACE 
                               $<BUILD_INTERFACE:${TidopLib_SOURCE_DIR}/src>
                               $<$<BOOL:${TL_HAVE_CUDA}>:${CUDA_INCLUDE_DIRS}>)

    target_compile_definitions(${PROJECT_NAME} INTERFACE
                               $<$<BOOL:${TL_HAVE_OPENBLAS}>:HAVE_LAPACK_CONFIG_H>
                               $<$<BOOL:${TL_HAVE_OPENBLAS}>:LAPACK_COMPLEX_STRUCTURE>)

    target_link_libraries(${PROJECT_NAME} INTERFACE 
                          TidopLib::Core
                          $<$<BOOL:${TL_HAVE_OPENBLAS}>:OpenBLAS::OpenBLAS>
                          $<$<BOOL:${TL_HAVE_CUDA}>:${CUDA_LIBRARIES}>
                          $<$<BOOL:${TL_HAVE_CUDA}>:${CUDA_cublas_LIBRARY}>
                          $<$<BOOL:${TL_HAVE_CUDA}>:${CUDA_cusolver_LIBRARY}>)
       
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
    message(STATUS "  TidopLib::Geometry disabled         [Module dependent on TidopLib::Math that is deactivated]") 
    message(STATUS "  TidopLib::RasterTools disabled  [Module dependent on TidopLib::Math that is deactivated]") 
    message(STATUS "  TidopLib::Geospatial disabled   [Module dependent on TidopLib::Math that is deactivated]") 

endif(BUILD_TL_MATH)