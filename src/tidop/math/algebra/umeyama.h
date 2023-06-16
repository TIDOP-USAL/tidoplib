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


#include "tidop/math/math.h"
#include "tidop/math/algebra/vector.h"
#include "tidop/math/algebra/matrix.h"
#include <tidop/math/statistics.h>
#include <tidop/math/algebra/svd.h>


namespace tl
{

/*! \addtogroup math
 *  \{
 */


/*! \defgroup algebra Algebra
 *
 * Algebra
 *
 *  \{
 */


// S. Umeyama, "Least-squares estimation of transformation parameters 
// between two point patterns," in IEEE Transactions on Pattern Analysis 
// and Machine Intelligence, vol. 13, no. 4, pp. 376-380, April 1991, 
// doi: 10.1109/34.88573.
// https://web.stanford.edu/class/cs273/refs/umeyama.pdf

template<typename T>
class Umeyama;

template<
    template<typename, size_t, size_t>
    class Matrix_t, typename T, size_t _rows, size_t _cols>
class Umeyama<Matrix_t<T, _rows, _cols>>
{

public:

    Umeyama(const Matrix_t<T, _rows, _cols> &src,
            const Matrix_t<T, _rows, _cols> &dst)
    {
        size_t size = src.rows();
        size_t dimension = dst.cols();
        Vector<double> mean_src(dimension);
        Vector<double> mean_dst(dimension);

        for (size_t c = 0; c < dimension; c++) {
            auto src_col = src.col(c);
            mean_src[c] = mean(src_col.begin(), src_col.end());
            auto dst_col = dst.col(c);
            mean_dst[c] = mean(dst_col.begin(), dst_col.end());
        }

        Matrix<double> src_demean = src;
        Matrix<double> dst_demean = dst;
        for (size_t c = 0; c < dimension; c++) {
            for (size_t r = 0; r < size; r++) {
                src_demean[r][c] -= mean_src[c];
                dst_demean[r][c] -= mean_dst[c];
            }
        }

        auto sigma = dst_demean.transpose() * src_demean / static_cast<double>(size);
        SingularValueDecomposition<Matrix<double>> svd(sigma);
        transformMatrix = Matrix<double>::identity(dimension + 1, dimension + 1);
        Matrix<double> S = Matrix<double>::identity(dimension, dimension);
        if (sigma.determinant() < 0) {
            S[dimension - 1][dimension - 1] = -1;
        }

        if (sigma.rank() == dimension - 1) {
            if (svd.u().determinant() * svd.v().determinant() < 0)
                S[dimension - 1][dimension - 1] = -1;
        }

        auto block = transformMatrix.block(0, dimension - 1, 0, dimension - 1);
        block = svd.u() * S * svd.v().transpose();

        double src_var{};
        double module{};
        for (size_t c = 0; c < src_demean.cols(); c++) {
            auto vector = src_demean.col(c);
            module = vector.module();
            src_var += module * module;
        }

        src_var /= size;

        double scale = 1 / src_var * svd.w().dotProduct(S.diagonal());

        transformMatrix.col(dimension)[0] = mean_dst[0];
        transformMatrix.col(dimension)[1] = mean_dst[1];
        transformMatrix.col(dimension)[2] = mean_dst[2];

        transformMatrix.block(0, dimension - 1, 0, dimension - 1) *= scale;
        auto aux = transformMatrix.block(0, dimension - 1, 0, dimension - 1) * mean_src;
        transformMatrix.col(dimension)[0] -= aux[0];
        transformMatrix.col(dimension)[1] -= aux[1];
        transformMatrix.col(dimension)[2] -= aux[2];

    }

    ~Umeyama()
    {
    }

    Matrix<T/*, _cols + 1, _cols + 1*/> transform() const
    {
        return transformMatrix;
    }

    Matrix<T, _cols, _cols> rotation() const
    {
        size_t dimension = transformMatrix.cols();
        Matrix<T, _cols, _cols> rotation(dimension, dimension);

        for (size_t r = 0; r < dimension; r++) {
            for (size_t c = 0; c < dimension; c++) {
                rotation(r, c) = transformMatrix(r, c);
            }
        }

        return rotation / scale();
    }

    //Vector<T, _cols> scale() const
    //{
    //  return Vector<T>{transformMatrix[0][0], transformMatrix[1][1], transformMatrix[2][2]};
    //}

    T scale() const
    {
        Vector<T> scale{transformMatrix[0][0], transformMatrix[1][1], transformMatrix[2][2]};
        return mean(scale.begin(), scale.end());
    }

    Vector<T, _cols> translation() const
    {
        size_t col = transformMatrix.cols() - 1;
        return Vector<T>{transformMatrix[0][col], transformMatrix[1][col], transformMatrix[2][col]};
    }

private:

    Matrix<T/*, _cols + 1, _cols + 1*/> transformMatrix;

};


/*! \} */ // end of algebra

/*! \} */ // end of math

} // End namespace tl


