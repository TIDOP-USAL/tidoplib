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
#include "tidop/geometry/primitives/Point.h"
#include "tidop/geometry/base/Concepts.h"
#include "tidop/geometry/transform/Scaling.h"

namespace tl
{

/*! \addtogroup Estimators
 *  \{
 */

/*!
 * \brief Scaling Estimation.
 *
 * The `ScalingEstimator` class provides methods to estimate scaling transformations
 * between two sets of points or matrices. The class allows for scaling estimation
 * in both 2D and 3D, depending on the dimensionality (`Dim`) specified during
 * instantiation. It provides methods for estimating scaling from matrices or
 * vectors of points.
 *
 * \tparam T The type of the scaling factor (e.g., float, double).
 * \tparam Dim The dimensionality of the scaling estimation (e.g., 2D or 3D).
 */
template <typename T, size_t Dim>
class ScalingEstimator
{

public:

    enum
    {
        dimensions = Dim,
        matrix_size
    };

public:

    ScalingEstimator() = default;
    ~ScalingEstimator() = default;

    /*!
     * \brief Estimate scaling transformation from matrices.
     * 
     * This method estimates a scaling transformation that maps the source matrix 
     * to the destination matrix.
     * 
     * \tparam rows The number of rows in the source and destination matrices.
     * \tparam cols The number of columns in the source and destination matrices.
     * \param src The source matrix.
     * \param dst The destination matrix.
     * \return The estimated scaling transformation.
     */
    template<size_t rows, size_t cols>
    static auto estimate(const Matrix<T, rows, cols> &src,
                         const Matrix<T, rows, cols> &dst) -> Scaling<T, Dim>;

    /*!
     * \brief Estimate scaling transformation from points.
     * 
     * This method estimates a scaling transformation that maps the source points 
     * to the destination points.
     * 
     * \param src The source points.
     * \param dst The destination points.
     * \return The estimated scaling transformation.
     */
    template <Point2DConcept Point>
    static auto estimate(const std::vector<Point> &src,
                         const std::vector<Point> &dst) -> Scaling<T, Dim>;

};

/*! \} */



/* ScalingEstimator implementation */

template<typename T, size_t Dim>
template<size_t rows, size_t cols>
auto ScalingEstimator<T, Dim>::estimate(const Matrix<T, rows, cols> &src, 
                                        const Matrix<T, rows, cols> &dst) -> Scaling<T, Dim>
{
    static_assert(dimensions == 2, "Scale estimator only for 2D Scaling");

    Scaling<T, Dim> scaling;

    try {

        TL_ASSERT(src.cols() == dimensions, "Invalid matrix columns size");
        TL_ASSERT(dst.cols() == dimensions, "Invalid matrix columns size");
        TL_ASSERT(src.rows() == dst.rows(), "Different matrix sizes. Size src = {} and size dst = {}", src.rows(), dst.rows());

        size_t size = src.rows() * dimensions;

        Matrix<double> A(size, 4, 0);
        Vector<double> B(size);

        for (size_t i = 0, r = 0; i < src.rows(); i++, r++) {

            A(r, 0) = src(i, 0);
            //A(r, 1) = 0;
            B[r] = dst(i, 0);

            r++;

            //A(r, 0) = 0;
            A(r, 1) = src(i, 1);
            B[r] = dst(i, 1);

        }

        SingularValueDecomposition<Matrix<double>> svd(A);
        Vector<double> C = svd.solve(B);

        scaling[0] = C[0];
        scaling[1] = C[1];

    } catch (...) {
        TL_THROW_EXCEPTION_WITH_NESTED("");
    }

    return scaling;
}

template<typename T, size_t Dim>
template <Point2DConcept Point>
auto ScalingEstimator<T, Dim>::estimate(const std::vector<Point> &src,
                                        const std::vector<Point> &dst) -> Scaling<T, Dim>
{
    TL_ASSERT(point_traits<Point>::spatial_dims == Dim, "Size of origin and destination points different");

    Matrix<T> src_mat(src.size(), dimensions);
    Matrix<T> dst_mat(dst.size(), dimensions);

    for (size_t r = 0; r < src_mat.rows(); r++) {
        for (size_t c = 0; c < Dim; c++) {
            src_mat[r][c] = src[r][c];
            dst_mat[r][c] = dst[r][c];
        }
    }

    return ScalingEstimator<T, dimensions>::estimate(src_mat, dst_mat);
}


} // End namespace tl

