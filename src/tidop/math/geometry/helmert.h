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

#include "tidop/config.h"

#include <vector>

#include "tidop/math/geometry/affine.h"
#include "tidop/math/algebra/matrix.h"
#include "tidop/math/algebra/decomp/svd.h"


namespace tl
{


/*! \addtogroup Estimators
 *  \{
 */


/// \cond

namespace internal
{

template<typename T, size_t rows, size_t cols>
void helmert(const Matrix<T, rows, cols> &src,
             const Matrix<T, rows, cols> &dst, Affine<T, 2> &affine)
{
    try {

        TL_ASSERT(src.rows() == dst.rows(), "Different matrix sizes. Size src = {} and size dst = {}", src.rows(), dst.rows());
        TL_ASSERT(src.rows() >= 2, "Invalid number of points: {} < {}", src.rows(), 2);

        size_t size = src.rows() * 2;

        Matrix<double> A(size, 4, 0);
        Vector<double> B(size);

        for (size_t i = 0, r = 0; i < src.rows(); i++, r++) {
            A(r, 0) = src(i, 0);
            A(r, 1) = -src(i, 1);
            A(r, 2) = 1;
            B[r] = dst(i,0);

            r++;
            
            A(r, 0) = src(i, 1);
            A(r, 1) = src(i, 0);
            A(r, 3) = 1;

            B[r] = dst(i, 1);
        }

        SingularValueDecomposition<Matrix<double>> svd(A);
        Vector<double> C = svd.solve(B);

        affine(0, 0) = C[0];
        affine(0, 1) = C[1];
        affine(0, 2) = C[2];
        affine(1, 0) = C[1];
        affine(1, 1) = C[0];
        affine(1, 2) = C[3];

    } catch (...) {
        TL_THROW_EXCEPTION_WITH_NESTED("");
    }
}

template<typename T, size_t rows, size_t cols>
void helmert(const Matrix<T, rows, cols> &src,
             const Matrix<T, rows, cols> &dst, Affine<T, 3> &affine)
{
    try {

        TL_ASSERT(src.rows() == dst.rows(), "Different matrix sizes. Size src = {} and size dst = {}", src.rows(), dst.rows());
        TL_ASSERT(src.cols() == 3 && dst.cols() == 3, "");
        TL_ASSERT(src.rows() >= 3 , "Invalid number of points: {} < {}", src.rows(), 3);

        size_t size = src.rows() * 3;

        Matrix<double> A(size, 7, 0);
        Vector<double> B(size);

        for (size_t i = 0, r = 0; i < src.rows(); i++, r++) {

            A(r, 0) = src(i, 0);
            A(r, 2) = -src(i, 2);
            A(r, 3) = src(i, 1);
            A(r, 4) = 1;
            B[r] = dst(i, 0);

            r++;
            
            A(r, 0) = src(i, 1);
            A(r, 1) = src(i, 2);
            A(r, 3) = -src(i, 0);
            A(r, 5) = 1;
            B[r] = dst(i, 1);

            r++;

            A(r, 0) = src(i, 2);
            A(r, 1) = -src(i, 1);
            A(r, 2) = src(i, 0);
            A(r, 6) = 1;
            B[r] = dst(i, 2);
        }

        SingularValueDecomposition<Matrix<double>> svd(A);
        Vector<double> C = svd.solve(B);

        double scale = (C[0] - 1) * pow(10, 6);
        affine = Affine<T, 3>(scale, scale, scale, C[4], C[5], C[6], C[1], C[2], C[3]);

    } catch (...) {
        TL_THROW_EXCEPTION_WITH_NESTED("");
    }
}

} /// end namespace internal

/// \endcond



/*!
 * \brief Estimator for Helmert transformations (affine transformations with uniform scaling and rotation).
 *
 * The `HelmertEstimator` class provides static methods to estimate a Helmert transformation
 * between two sets of points or matrices representing the source and destination coordinates.
 *
 * \tparam T The type of the elements (e.g., float, double).
 * \tparam Dim The dimensionality of the transformation (e.g., 2 for 2D, 3 for 3D).
 */
template <typename T, size_t Dim>
class HelmertEstimator
{
    enum
    {
        dimensions = Dim
    };

public:

    HelmertEstimator() = default;
    ~HelmertEstimator() = default;

    /*!
     * \brief Estimate a Helmert transformation between two matrices.
     *
     * This method estimates the Helmert transformation that maps points
     * from the source matrix \p src to the destination matrix \p dst.
     *
     * \tparam rows Number of rows in the matrices.
     * \tparam cols Number of columns in the matrices.
     * \param[in] src The source matrix of points.
     * \param[in] dst The destination matrix of points.
     * \return The estimated Helmert transformation.
     */
    template<size_t rows, size_t cols>
    static auto estimate(const Matrix<T, rows, cols> &src,
                         const Matrix<T, rows, cols> &dst) -> Affine<T, Dim>;

    /*!
     * \brief Estimate a Helmert transformation between two sets of 2D points.
     *
     * This method estimates the Helmert transformation that maps points
     * from the source vector \p src to the destination vector \p dst.
     *
     * \param[in] src The source vector of 2D points.
     * \param[in] dst The destination vector of 2D points.
     * \return The estimated Helmert transformation.
     */
    static auto estimate(const std::vector<Point<T>> &src,
                         const std::vector<Point<T>> &dst) -> Affine<T, Dim>;

    /*!
     * \brief Estimate a Helmert transformation between two sets of 3D points.
     *
     * This method estimates the Helmert transformation that maps points
     * from the source vector \p src to the destination vector \p dst.
     *
     * \param[in] src The source vector of 3D points.
     * \param[in] dst The destination vector of 3D points.
     * \return The estimated Helmert transformation.
     */
    static auto estimate(const std::vector<Point3<T>> &src,
                         const std::vector<Point3<T>> &dst) -> Affine<T, Dim>;
};

/*! \} */



/* Helmert2DEstimator implementation */

template<typename T, size_t Dim>
template<size_t rows, size_t cols>
auto HelmertEstimator<T, Dim>::estimate(const Matrix<T, rows, cols> &src, 
                                        const Matrix<T, rows, cols> &dst) -> Affine<T, Dim>
{
    Affine<T, Dim> affine;

    try {

        internal::helmert(src, dst, affine);

    } catch (...) {
        TL_THROW_EXCEPTION_WITH_NESTED("");
    }

    return affine;
}

template<typename T, size_t Dim>
auto HelmertEstimator<T, Dim>::estimate(const std::vector<Point<T>> &src, 
                                        const std::vector<Point<T>> &dst) -> Affine<T, Dim>
{
    TL_ASSERT(src.size() == dst.size(), "Size of origin and destination points different");

    Matrix<T> src_mat(src.size(), 2);
    Matrix<T> dst_mat(dst.size(), 2);

    for (size_t r = 0; r < src_mat.rows(); r++) {
        src_mat[r][0] = src[r].x;
        src_mat[r][1] = src[r].y;

        dst_mat[r][0] = dst[r].x;
        dst_mat[r][1] = dst[r].y;
    }

    return HelmertEstimator<T, Dim>::estimate(src_mat, dst_mat);
}

template<typename T, size_t Dim>
auto HelmertEstimator<T, Dim>::estimate(const std::vector<Point3<T>> &src, 
                                        const std::vector<Point3<T>> &dst) -> Affine<T, Dim>
{
    TL_ASSERT(src.size() == dst.size(), "Size of origin and destination points different");

    Matrix<T> src_mat(src.size(), 3);
    Matrix<T> dst_mat(dst.size(), 3);

    for (size_t r = 0; r < src_mat.rows(); r++) {
        src_mat[r][0] = src[r].x;
        src_mat[r][1] = src[r].y;
        src_mat[r][2] = src[r].z;

        dst_mat[r][0] = dst[r].x;
        dst_mat[r][1] = dst[r].y;
        dst_mat[r][2] = dst[r].z;
    }

    return HelmertEstimator<T, Dim>::estimate(src_mat, dst_mat);
}


} // End namespace tl

