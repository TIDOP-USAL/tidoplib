/**************************************************************************
 *                                                                        *
 * Copyright (C) 2021 by Tidop Research Group                             *
 * Copyright (C) 2021 by Esteban Ruiz de Oña Crespo                       *
 *                                                                        *
 * This file is part of TidopLib                                          *
 *                                                                        *
 * TidopLib is free software: you can redistribute it and/or modify       *
 * it under the terms of the GNU Lesser General Public License as         *
 * published by the Free Software Foundation, either version 3 of the     *
 * License, or (at your option) any later version.                        *
 *                                                                        *
 * TidopLib is distributed in the hope that it will be useful,            *
 * but WITHOUT ANY WARRANTY; without even the implied warranty of         *
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the          *
 * GNU Lesser General Public License for more details.                    *
 *                                                                        *
 * You should have received a copy of the GNU Lesser General Public       *
 * License along with Foobar. If not, see <http://www.gnu.org/licenses/>. *
 *                                                                        *
 * @license LGPL-3.0 <https://www.gnu.org/licenses/lgpl-3.0.html>         *
 *                                                                        *
 **************************************************************************/

#pragma once

#include <vector>

#include "tidop/math/algebra/vector/Vector.h"
#include "tidop/math/algebra/matrix/Matrix.h"
#include "tidop/math/algebra/decomp/SVD.h"
#include "tidop/geometry/base/Concepts.h"
#include "tidop/geometry/primitives/Point.h"
#include "tidop/geometry/transform/Affine.h"

namespace tl
{

/*! \addtogroup Estimators
 *  \{
 */

/*!
 * \brief Estimator for 2D affine transformations.
 *
 * The `Affine2DEstimator` class provides static methods to estimate a 2D affine
 * transformation between two sets of points or matrices representing the source
 * and destination coordinates.
 *
 * \tparam T The type of the elements (e.g., float, double).
 */
template <typename T>
class Affine2DEstimator
{

public:

    Affine2DEstimator() = default;
    ~Affine2DEstimator() = default;

    /*!
     * \brief Estimate a 2D affine transformation between two matrices.
     *
     * This method estimates the affine transformation that maps points
     * from the source matrix \p src to the destination matrix \p dst.
     *
     * \tparam Rows Number of rows in the matrices.
     * \tparam Cols Number of columns in the matrices.
     * \param[in] src The source matrix of points.
     * \param[in] dst The destination matrix of points.
     * \return The estimated 2D affine transformation.
     */
    template<size_t Rows, size_t Cols>
    [[nodiscard]]
    static auto estimate(const Matrix<T, Rows, Cols> &src,
                         const Matrix<T, Rows, Cols> &dst) -> Affine<T, 2>;

    /*!
     * \brief Estimate a 2D affine transformation between two sets of points.
     *
     * This method estimates the affine transformation that maps points
     * from the source vector \p src to the destination vector \p dst.
     *
     * \param[in] src The source vector of points.
     * \param[in] dst The destination vector of points.
     * \return The estimated 2D affine transformation.
     */
    template <Point2DConcept Point>
    [[nodiscard]]
    static auto estimate(const std::vector<Point> &src,
                         const std::vector<Point> &dst) -> Affine<T, 2>;
};

/*! \} */


/* Affine2DEstimator implementation */

template<typename T>
template<size_t rows, size_t cols>
auto Affine2DEstimator<T>::estimate(const Matrix<T, rows, cols> &src, 
                                    const Matrix<T, rows, cols> &dst) -> Affine<T, 2>
{

    Affine<T, 2> affine;

    try {

        TL_ASSERT(src.cols() == 2, "Invalid matrix columns size");
        TL_ASSERT(dst.cols() == 2, "Invalid matrix columns size");
        TL_ASSERT(src.rows() == dst.rows(), "Different matrix sizes. Size src = {} and size dst = {}", src.rows(), dst.rows());
        TL_ASSERT(src.rows() >= 3 , "Invalid number of points: {} < {}", src.rows(), 3);

        size_t size = src.rows() * 2;

        Matrix<double> A(size, 6, 0);
        Vector<double> B(size);

        for (size_t i = 0, r = 0; i < src.rows(); i++, r++) {
            A(r, 0) = src(i, 0);
            A(r, 1) = src(i, 1);
            A(r, 4) = 1;
            B[r] = dst(i,0);

            r++;
            
            A(r, 2) = src(i, 0);
            A(r, 3) = src(i, 1);
            A(r, 5) = 1;

            B[r] = dst(i,1);
        }

        SingularValueDecomposition<Matrix<double>> svd(A);
        Vector<double> C = svd.solve(B);

        affine(0, 0) = C[0];
        affine(0, 1) = C[1];
        affine(0, 2) = C[4];
        affine(1, 0) = C[2];
        affine(1, 1) = C[3];
        affine(1, 2) = C[5];

    } catch (...) {
        TL_THROW_EXCEPTION_WITH_NESTED("");
    }

    return affine;
}

template<typename T>
template <Point2DConcept Point>
auto Affine2DEstimator<T>::estimate(const std::vector<Point> &src,
                                    const std::vector<Point> &dst) -> Affine<T, 2>
{
    static_assert(point_traits<Point>::spatial_dims == 2, "Point dimension must match Affine transformation dimension");

    TL_ASSERT(src.size() == dst.size(), "Size of origin and destination points different");
    TL_ASSERT(src.size() >= 3, "Invalid number of points: {} < {}", src.size(), 3);
    TL_ASSERT(2 == src[0].size(), "Point dimension must match Affine transformation dimension");

    Matrix<T> src_mat(src.size(), 2);
    Matrix<T> dst_mat(dst.size(), 2);
    
    for (size_t r = 0; r < src_mat.rows(); r++) {
        src_mat[r][0] = src[r][0];
        src_mat[r][1] = src[r][1];
        dst_mat[r][0] = dst[r][0];
        dst_mat[r][1] = dst[r][1];
    }
    
    return Affine2DEstimator<T>::estimate(src_mat, dst_mat);
}

} // End namespace tl

