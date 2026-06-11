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
#include "tidop/geometry/transform/Translation.h"

namespace tl
{

/*! \addtogroup Estimators
 *  \{
 */


/*!
 * \brief Estimator for Translation transformations.
 *
 * The `TranslationEstimator` class provides methods to estimate a translation
 * transformation between two sets of points or matrices. The translation is estimated
 * based on the corresponding pairs of source and destination points or matrices,
 * and the result is represented as a `Translation` object.
 *
 * \tparam T The type of the translation components (e.g., float, double).
 * \tparam Dim The dimensionality of the translation (e.g., 2D or 3D).
 */
template <typename T/*, size_t Dim*/>
class TranslationEstimator
{

public:

    enum
    {
        dimensions = 2/*Dim*/,
        matrix_size
    };

public:

    TranslationEstimator() = default;
    ~TranslationEstimator() = default;

    /*!
     * \brief Estimate the translation between two matrices.
     * 
     * This method estimates the translation transformation between two matrices of corresponding points.
     * \tparam rows The number of rows in the input matrices.
     * \tparam cols The number of columns in the input matrices.
     * \param[in] src The source matrix of points.
     * \param[in] dst The destination matrix of points.
     * \return The estimated translation transformation.
     */
    template<size_t rows, size_t cols>
    [[nodiscard]]
    static auto estimate(const Matrix<T, rows, cols> &src,
                         const Matrix<T, rows, cols> &dst) -> Translation<T, dimensions>;

    /*!
     * \brief Estimate the translation between two sets of points.
     * 
     * This method estimates the translation transformation between two sets of corresponding points.
     * \param[in] src The source set of points.
     * \param[in] dst The destination set of points.
     * \return The estimated translation transformation.
     */
    template <Point2DConcept Point>
    [[nodiscard]]
    static auto estimate(const std::vector<Point> &src,
                         const std::vector<Point> &dst) -> Translation<T, dimensions>;

};

/*! \} */


/* TranslationEstimator implementation */

template<typename T>
template<size_t rows, size_t cols>
auto TranslationEstimator<T>::estimate(const Matrix<T, rows, cols> &src, 
                                       const Matrix<T, rows, cols> &dst) -> Translation<T, dimensions>
{
    static_assert(dimensions == 2, "Scale estimator only for 2D Scaling");

    Translation<T, dimensions> translation;

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
            A(r, 2) = 1;
            //A(r, 3) = 0;
            B[r] = dst(i, 0);

            r++;

            //A(r, 0) = 0;
            A(r, 1) = src(i, 1);
            //A(r, 2) = 0;
            A(r, 3) = 1;
            B[r] = dst(i, 1);

        }

        SingularValueDecomposition<Matrix<double>> svd(A);
        Vector<double> C = svd.solve(B);

        translation[0] = C[2];
        translation[1] = C[3];

    } catch (...) {
        TL_THROW_EXCEPTION_WITH_NESTED("");
    }

    return translation;
}

template<typename T>
template <Point2DConcept Point>
auto TranslationEstimator<T>::estimate(const std::vector<Point> &src,
                                       const std::vector<Point> &dst) -> Translation<T, dimensions>
{
    static_assert(point_traits<Point>::spatial_dims == dimensions, "Vector dimension must match Affine transformation dimension");

    TL_ASSERT(src.size() == dst.size(), "Size of origin and destination points different");

    Matrix<T> src_mat(src.size(), dimensions);
    Matrix<T> dst_mat(dst.size(), dimensions);

    for (size_t r = 0; r < src_mat.rows(); r++) {
        for (size_t c = 0; c < dimensions; c++) {
            src_mat[r][c] = src[r][c];
            dst_mat[r][c] = dst[r][c];
        }
    }

    return TranslationEstimator<T>::estimate(src_mat, dst_mat);
}

} // End namespace tl

