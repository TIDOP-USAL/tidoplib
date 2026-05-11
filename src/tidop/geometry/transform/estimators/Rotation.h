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

#include "tidop/config.h"
#include "tidop/math/algebra/vector/Vector.h"
#include "tidop/math/algebra/matrix/Matrix.h"
#include "tidop/math/algebra/decomp/SVD.h"
#include "tidop/geometry/primitives/Point.h"
#include "tidop/geometry/base/Concepts.h"
#include "tidop/geometry/transform/Rotation.h"

namespace tl
{

/*! \addtogroup Estimators
 *  \{
 */


/*!
 * \brief Estimator for rotation transformations.
 *
 * The `RotationEstimator` class is used to estimate the rotation transformation
 * that maps one set of points or matrices to another. It uses different
 * mathematical methods to compute the optimal rotation between two sets of points
 * or matrices.
 *
 * \tparam T The type of the elements (e.g., float, double).
 * \tparam Dim The dimensionality of the rotation (e.g., 2 for 2D, 3 for 3D).
 */
template <typename T, size_t Dim>
class RotationEstimator
{

public:

    /*!
     * \brief The dimensionality of the rotation.
     */
    enum
    {
        dimensions = Dim,  ///< Dimensionality of the rotation (2D or 3D).
        matrix_size        ///< Matrix size (rows and columns for matrices).
    };

public:

    RotationEstimator() = default;
    ~RotationEstimator() = default;

    /*!
     * \brief Estimate the rotation matrix between two matrices.
     *
     * This method estimates the rotation that maps one matrix of points to another.
     * The matrices should have the same dimensions and represent corresponding sets
     * of points in the source and destination coordinate systems.
     *
     * \tparam rows The number of rows in the matrices.
     * \tparam cols The number of columns in the matrices.
     * \param[in] src The source matrix of points.
     * \param[in] dst The destination matrix of points.
     * \return The estimated rotation.
     */
    template<size_t rows, size_t cols>
    static auto estimate(const Matrix<T, rows, cols> &src,
                         const Matrix<T, rows, cols> &dst) -> Rotation<T, Dim>;

    /*!
     * \brief Estimate the rotation transformation between two sets of points.
     *
     * This method estimates the rotation that maps one set of points in the source
     * coordinate system to another set of corresponding points in the destination
     * coordinate system. Both sets of points must have the same size.
     *
     * \param[in] src The source set of points.
     * \param[in] dst The destination set of points.
     * \return The estimated rotation.
     */
    template <Point2DConcept Point>
    static auto estimate(const std::vector<Point> &src,
                         const std::vector<Point> &dst) -> Rotation<T, Dim>;

};

/*! \} */



/* RotationEstimator implementation */

template<typename T, size_t Dim>
template<size_t rows, size_t cols>
auto RotationEstimator<T, Dim>::estimate(const Matrix<T, rows, cols> &src, 
                                         const Matrix<T, rows, cols> &dst) -> Rotation<T, Dim>
{
    static_assert(dimensions == 2, "Rotation estimator only for 2D Scaling");

    Rotation<T, Dim> rotation;

    try {

        TL_ASSERT(src.cols() == dimensions, "Invalid matrix columns size");
        TL_ASSERT(dst.cols() == dimensions, "Invalid matrix columns size");
        TL_ASSERT(src.rows() == dst.rows(), "Different matrix sizes. Size src = {} and size dst = {}", src.rows(), dst.rows());

        size_t size = src.rows() * dimensions;

        Matrix<double> A(size, 2, 0);
        Vector<double> B(size);

        for (size_t i = 0, r = 0; i < src.rows(); i++, r++) {

            A(r, 0) = src(i, 0);
            A(r, 1) = -src(i, 1);
            B[r] = dst(i, 0);

            r++;

            A(r, 0) = src(i, 1);
            A(r, 1) = src(i, 0);
            B[r] = dst(i, 1);

        }

        SingularValueDecomposition<Matrix<double>> svd(A);
        Vector<double> C = svd.solve(B);

        rotation[0][0] = C[0];
        rotation[0][1] = -C[1];
        rotation[1][0] = C[1];
        rotation[1][1] = C[0];

    } catch (...) {
        TL_THROW_EXCEPTION_WITH_NESTED("");
    }

    return rotation;
}

template<typename T, size_t Dim>
template <Point2DConcept Point>
auto RotationEstimator<T, Dim>::estimate(const std::vector<Point<T>> &src, 
                                         const std::vector<Point<T>> &dst) -> Rotation<T, Dim>
{
    static_assert(point_traits<Point>::spatial_dims == 2, "Rotation estimator only for 2D");

    TL_ASSERT(src.size() == dst.size(), "Size of origin and destination points different");

    Matrix<T> src_mat(src.size(), dimensions);
    Matrix<T> dst_mat(dst.size(), dimensions);

    for (size_t r = 0; r < src_mat.rows(); r++) {
        src_mat[r][0] = src[r].x();
        src_mat[r][1] = src[r].y();

        dst_mat[r][0] = dst[r].x();
        dst_mat[r][1] = dst[r].y();
    }

    return RotationEstimator<T, dimensions>::estimate(src_mat, dst_mat);
}


} // End namespace tl

