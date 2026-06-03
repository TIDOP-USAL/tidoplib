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
                             #math.h
                             algebra/decomp/Cholesky.h
                             algebra/decomp/Eigen.h
                             algebra/decomp/LU.h
                             algebra/decomp/QR.h
                             algebra/decomp/SVD.h
                             algebra/eval/DiagonalMatrixEval.h
                             algebra/eval/Evaluator.h
                             algebra/eval/MatBinaryEval.h
                             algebra/eval/MatMulEval.h
                             algebra/eval/MatrixBlockEval.h
                             algebra/eval/MatrixColEval.h
                             algebra/eval/MatrixDiagonalEval.h
                             algebra/eval/MatrixEval.h
                             algebra/eval/MatrixRowEval.h
                             algebra/eval/MatScalarEval.h
                             algebra/eval/MatUnaryMinusEval.h
                             algebra/eval/MatVecMulEval.h
                             algebra/eval/TransposeEval.h
                             algebra/eval/VecBinaryEval.h
                             algebra/eval/VecScalarEval.h
                             algebra/eval/VectorEval.h
                             algebra/eval/VecUnaryMinusEval.h
                             algebra/expr/DiagonalMatrixExpr.h
                             algebra/expr/MatBinaryExpr.h
                             algebra/expr/MatMulExpr.h
                             algebra/expr/MatScalarExpr.h
                             algebra/expr/MatUnaryMinusExpr.h
                             algebra/expr/MatVecMulExpr.h
                             algebra/expr/TransposeExpr.h
                             algebra/expr/VecBinaryExpr.h
                             algebra/expr/VecScalarExpr.h
                             algebra/expr/VecUnaryMinusExpr.h
                             algebra/functors/Add.h
                             algebra/functors/Div.h
                             algebra/functors/Mul.h
                             algebra/functors/Sub.h
                             algebra/matrix/Matrix.h
                             algebra/matrix/MatrixBase.h
                             algebra/matrix/MatrixBlock.h
                             algebra/matrix/MatrixCol.h
                             algebra/matrix/MatrixConfig.h
                             algebra/matrix/MatrixDiagonal.h
                             algebra/matrix/MatrixRow.h
                             algebra/matrix/detail/AdjugateMatrix.h
                             algebra/matrix/detail/Assign.h
                             algebra/matrix/detail/Cofactor.h
                             algebra/matrix/detail/CofactorMatrix.h
                             algebra/matrix/detail/Determinant.h
                             algebra/matrix/detail/FirstMinor.h
                             algebra/matrix/detail/Inverse.h
                             algebra/matrix/detail/MatEqual.h
                             algebra/matrix/detail/MulMat.h
                             algebra/matrix/detail/MulMatBLAS.h
                             algebra/matrix/detail/MulMatCPP.h
                             algebra/matrix/detail/MulMatSIMD.h
                             algebra/matrix/detail/ReducedRowEchelonForm.h
                             algebra/matrix/detail/RowEchelonForm.h
                             algebra/rotations/AxisAngle.h
                             algebra/rotations/EulerAngles.h
                             algebra/rotations/EulerAnglesAxis.h
                             algebra/rotations/Quaternion.h
                             algebra/rotations/RotationMatrix.h
                             algebra/rotations/Rotations.h
                             algebra/rotations/detail/RotationConverter.h
                             algebra/vector/Vector.h
                             algebra/vector/VectorBase.h
                             algebra/vector/detail/Assign.h
                             algebra/vector/detail/VecEqual.h
                             algebra/vector/detail/MatVecMul.h
                             algebra/vector/detail/MatVecMulCPP.h
                             algebra/vector/detail/MatVecMulSIMD.h
                             algebra/BaseExpr.h
                             algebra/matrices.h
                             base/Blas.h
                             base/blas/Gemm.h
                             base/blas/Gemv.h
                             base/blas/Symm.h
                             base/blas/Traits.h
                             base/blas/Trmm.h
                             base/blas/Types.h
                             base/Cuda.h
                             base/cuda/CublasTraits.h
                             base/cuda/CusolverTraits.h
                             base/cuda/Gemm.h
                             base/cuda/Gemv.h
                             base/cuda/Lu.h
                             base/Concepts.h
                             base/Constants.h
                             base/Data.h
                             base/Lapack.h
                             base/lapack/Cholesky.h
                             base/lapack/Eigen.h
                             base/lapack/LeastSquares.h
                             base/lapack/Lu.h
                             base/lapack/Qr.h
                             base/lapack/Svd.h
                             base/lapack/Traits.h
                             base/lapack/Triangular.h
                             base/lapack/Types.h
                             base/Simd.h
                             base/Traits.h
                             base/simd/Compare.h
                             base/simd/Intrinsics.h
                             base/simd/Packed.h
                             base/simd/Traits.h
                             base/simd/detail/BitwiseAnd.h
                             base/simd/detail/BitwiseAndNot.h
                             base/simd/detail/BitwiseNot.h
                             base/simd/detail/BitwiseOr.h
                             base/simd/detail/BitwiseXor.h
                             base/simd/detail/ChangeSign.h
                             base/simd/detail/EqualTo.h
                             base/simd/detail/GreaterThan.h
                             base/simd/detail/HorizontalSum.h
                             base/simd/detail/LoadPacked.h
                             base/simd/detail/MaskOp.h
                             base/simd/detail/Max.h
                             base/simd/detail/Min.h
                             base/simd/detail/PackedAdd.h
                             base/simd/detail/PackedDiv.h
                             base/simd/detail/PackedMul.h
                             base/simd/detail/PackedSub.h
                             base/simd/detail/SetPacked.h
                             base/simd/detail/SetZero.h
                             base/simd/detail/SignBias.h
                             base/simd/detail/StorePacked.h
                             geometry/angles/AngleBase.h
                             geometry/angles/Degrees.h
                             geometry/angles/Gradians.h
                             geometry/angles/Radians.h
                             geometry/angles/detail/AngleConverter.h
                             geometry/angles/detail/Utils.h
                             numeric/Arithmetic.h
                             numeric/Comparison.h
                             statistic/DescriptiveStatistics.h
                             statistic/algorithms/association/Covariance.h
                             statistic/algorithms/association/Pearson.h
                             statistic/algorithms/descriptive/Mean.h
                             statistic/algorithms/descriptive/Median.h
                             statistic/algorithms/descriptive/Mode.h
                             statistic/algorithms/descriptive/Moments.h
                             statistic/algorithms/descriptive/Quantile.h
                             statistic/algorithms/descriptive/Range.h
                             statistic/algorithms/descriptive/StandardDeviation.h
                             statistic/algorithms/descriptive/TukeyFences.h
                             statistic/algorithms/descriptive/Variance.h
                             statistic/algorithms/ratios/CV.h
                             statistic/algorithms/ratios/ZScore.h
                             statistic/algorithms/ratios/RMS.h
                             statistic/algorithms/robust/BiweightMidvariance.h
                             statistic/algorithms/robust/IQR.h
                             statistic/algorithms/robust/MAD.h
                             statistic/algorithms/shape/Kurtosis.h
                             statistic/algorithms/shape/Kurtosis.impl.h
                             statistic/algorithms/shape/Skewness.h
                             statistic/algorithms/shape/Skewness.impl.h
                             statistic/base/StatisticAlgorithm.h
                             statistic/base/CentralMoments.h
                             statistic/base/DataFrame.h
                             statistic/base/Series.h
                             statistic/classification/ConfusionMatrix.h)
                
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
