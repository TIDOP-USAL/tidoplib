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
 * License along with TidopLib. If not, see <http://www.gnu.org/licenses>.*
 *                                                                        *
 * @license LGPL-3.0 <https://www.gnu.org/licenses/lgpl-3.0.html>         *
 *                                                                        *
 **************************************************************************/

#pragma once

#include <vector>

#include "tidop/math/algebra/vector.h"
#include "tidop/math/algebra/matrix.h"
#include "tidop/math/statistics.h"
#include "tidop/math/algebra/decomp/svd.h"
#include "tidop/math/geometry/affine.h"

namespace tl
{

/*! \addtogroup Estimators
 *  \{
 */

/*!
 * \brief Umeyama Similarity Transform Estimator
 *
 * This class implements the estimation of the similarity transformation parameters
 * between two point patterns using the method described by S. Umeyama in his paper
 * "Least-squares estimation of transformation parameters between two point patterns,"
 * published in IEEE Transactions on Pattern Analysis and Machine Intelligence, vol. 13,
 * no. 4, pp. 376-380, April 1991 (doi: 10.1109/34.88573).
 *
 * The method solves for the optimal similarity transform that best aligns the source
 * points to the destination points in a least-squares sense.
 *
 * \tparam T The type of the transformation parameters (e.g., float, double).
 * \tparam Dim The dimensionality of the transformation (e.g., 2D, 3D).
 *
 * \see S. Umeyama, "Least-squares estimation of transformation parameters between two point patterns,"
 * IEEE Transactions on Pattern Analysis and Machine Intelligence, 1991, https://web.stanford.edu/class/cs273/refs/umeyama.pdf
 */
template <typename T, size_t Dim>
class Umeyama
{

public:

    enum
    {
        dimensions = Dim,
        matrix_size
    };

public:

    Umeyama() = default;
    ~Umeyama() = default;

    /*!
     * \brief Estimate the similarity transformation between two matrices of points.
     * 
     * This method estimates the transformation parameters between two matrices of corresponding points.
     * \tparam rows The number of rows in the input matrices.
     * \tparam cols The number of columns in the input matrices.
     * \param[in] src The source matrix of points.
     * \param[in] dst The destination matrix of points.
     * \return The estimated similarity transformation represented as an Affine object.
     */
    template<size_t rows, size_t cols>
    static auto estimate(const Matrix<T, rows, cols> &src,
                         const Matrix<T, rows, cols> &dst) -> Affine<T, Dim>;

    /*!
     * \brief Estimate the similarity transformation between two sets of 3D points.
     * 
     * This method estimates the transformation parameters between two sets of 3D corresponding points.
     * \param[in] src The source set of 3D points.
     * \param[in] dst The destination set of 3D points.
     * \return The estimated similarity transformation represented as an Affine object.
     */
    static auto estimate(const std::vector<Point3<T>> &src,
                         const std::vector<Point3<T>> &dst) -> Affine<T, Dim>;

    /*!
     * \brief Estimate the similarity transformation between two sets of 2D points.
     * 
     * This method estimates the transformation parameters between two sets of 2D corresponding points.
     * \param[in] src The source set of 2D points.
     * \param[in] dst The destination set of 2D points.
     * \return The estimated similarity transformation represented as an Affine object.
     */
    static auto estimate(const std::vector<Point<T>> &src,
                         const std::vector<Point<T>> &dst) -> Affine<T, Dim>;
};


/*! \} */


/* Umeyama implementation */

template<typename T, size_t Dim>
template<size_t rows, size_t cols>
auto Umeyama<T, Dim>::estimate(const Matrix<T, rows, cols> &src, 
                               const Matrix<T, rows, cols> &dst) -> Affine<T, Dim>
{
    Affine<T, Dim> affine;

    try {

        TL_ASSERT(src.cols() == dimensions, "Invalid matrix columns size");
        TL_ASSERT(dst.cols() == dimensions, "Invalid matrix columns size");
        TL_ASSERT(src.rows() == dst.rows(), "Different matrix sizes");

        auto transformMatrix = Matrix<double, matrix_size, matrix_size>::identity();

        size_t size = src.rows();
        Vector<double> mean_src(dimensions);
        Vector<double> mean_dst(dimensions);

        for (size_t c = 0; c < dimensions; c++) {
            auto src_col = src.col(c);
            mean_src[c] = mean(src_col.begin(), src_col.end());
            auto dst_col = dst.col(c);
            mean_dst[c] = mean(dst_col.begin(), dst_col.end());
        }

        Matrix<double> src_demean = src;
        Matrix<double> dst_demean = dst;
        for (size_t c = 0; c < dimensions; c++) {
            for (size_t r = 0; r < size; r++) {
                src_demean[r][c] -= mean_src[c];
                dst_demean[r][c] -= mean_dst[c];
            }
        }

        auto sigma = dst_demean.transpose() * src_demean / static_cast<double>(size);
        SingularValueDecomposition<Matrix<double>> svd(sigma);

        Matrix<double, dimensions, dimensions> S = Matrix<double, dimensions, dimensions>::identity();
        if (sigma.determinant() < 0) {
            S[dimensions - 1][dimensions - 1] = -1;
        }

        if (sigma.rank() == dimensions - 1) {
            if (svd.u().determinant() * svd.v().determinant() < 0)
                S[dimensions - 1][dimensions - 1] = -1;
        }

        transformMatrix.block(0, dimensions - 1, 0, dimensions - 1) = svd.u() * S * svd.v().transpose();

        double src_var{};
        double module{};
        for (size_t c = 0; c < src_demean.cols(); c++) {
            auto vector = src_demean.col(c);
            module = vector.module();
            src_var += module * module;
        }

        src_var /= size;

        double scale = 1 / src_var * svd.w().dotProduct(S.diagonal());

        transformMatrix.col(dimensions)[0] = mean_dst[0];
        transformMatrix.col(dimensions)[1] = mean_dst[1];
        transformMatrix.col(dimensions)[2] = mean_dst[2];

        transformMatrix.block(0, dimensions - 1, 0, dimensions - 1) *= scale;
        auto aux = transformMatrix.block(0, dimensions - 1, 0, dimensions - 1) * mean_src;
        transformMatrix.col(dimensions)[0] -= aux[0];
        transformMatrix.col(dimensions)[1] -= aux[1];
        transformMatrix.col(dimensions)[2] -= aux[2];

        affine = Affine<T, Dim>(transformMatrix.block(0, dimensions - 1, 0, dimensions));

    } catch (...) {
        TL_THROW_EXCEPTION_WITH_NESTED("");
    }

    return affine;
}

template<typename T, size_t Dim>
auto Umeyama<T, Dim>::estimate(const std::vector<Point3<T>> &src, 
                               const std::vector<Point3<T>> &dst) -> Affine<T, Dim>
{
    Affine<T, Dim> affine;

    try {

        TL_ASSERT(src.size() == dst.size(), "Size of origin and destination points different");

        Matrix<T> src_mat(src.size(), dimensions);
        Matrix<T> dst_mat(dst.size(), dimensions);

        for (size_t r = 0; r < src_mat.rows(); r++) {
            src_mat[r][0] = src[r].x;
            src_mat[r][1] = src[r].y;
            src_mat[r][2] = src[r].z;

            dst_mat[r][0] = dst[r].x;
            dst_mat[r][1] = dst[r].y;
            dst_mat[r][2] = dst[r].z;
        }

        affine = Umeyama<T, dimensions>::estimate(src_mat, dst_mat);

    } catch (...) {
        TL_THROW_EXCEPTION_WITH_NESTED("");
    }

    return affine;
}

template<typename T, size_t Dim>
auto Umeyama<T, Dim>::estimate(const std::vector<Point<T>> &src, 
                               const std::vector<Point<T>> &dst) -> Affine<T, Dim>
{
    Affine<T, Dim> affine;

    try {

        TL_ASSERT(src.size() == dst.size(), "Size of origin and destination points different");

        Matrix<T> src_mat(src.size(), dimensions);
        Matrix<T> dst_mat(dst.size(), dimensions);

        for (size_t r = 0; r < src_mat.rows(); r++) {
            src_mat[r][0] = src[r].x;
            src_mat[r][1] = src[r].y;

            dst_mat[r][0] = dst[r].x;
            dst_mat[r][1] = dst[r].y;
        }

        affine = Umeyama<T, dimensions>::estimate(src_mat, dst_mat);

    } catch (...) {
        TL_THROW_EXCEPTION_WITH_NESTED("");
    }

    return affine;
}

} // End namespace tl


