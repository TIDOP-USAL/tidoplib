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
#include "tidop/core/exception.h"
#include "tidop/math/algebra/matrix.h"
#include "tidop/math/algebra/vector.h"
#include "tidop/math/lapack.h"
#include "tidop/math/cuda.h"

namespace tl
{

/*! \addtogroup math
 *  \{
 */

/*! \addtogroup algebra
 *  \{
 */


/*!
 * \brief Factorización o descomposición LU
 *
 * Sea A una matriz no singular (si lo fuera, entonces la descomposición podría no ser única)
 *
 * \f[ A=LU \f]
 *
 * donde L y U son matrices inferiores y superiores triangulares respectivamente.
 *
 */
template<typename T>
class LuDecomposition;


template<
    template<typename, size_t, size_t>
class Matrix_t, typename T, size_t _rows, size_t _cols
>
class LuDecomposition<Matrix_t<T, _rows, _cols>>
{

public:

    LuDecomposition(const Matrix_t<T, _rows, _cols> &a);
    ~LuDecomposition();

    auto solve(const Vector<T, _rows> &b) const -> Vector<T, _rows>;
    auto solve(const Matrix_t<T, _rows, _cols> &b) const -> Matrix_t<T, _rows, _cols>;
    auto lu() const -> Matrix_t<T, _rows, _cols>;

    auto determinant() const -> T;

private:

    void decompose();
    auto findMaxElementsByRows() const -> Vector<T, _rows>;

private:

    Matrix_t<T, _rows, _cols> LU;
//#if defined(TL_HAVE_OPENBLAS) || defined(TL_HAVE_CUDA) 
#if defined(TL_HAVE_OPENBLAS)
    int *mPivotIndex;
#else
    Vector<size_t, _rows> mPivotIndex;
#endif
    T d;
    size_t mRows;
};


template<
    template<typename, size_t, size_t>
class Matrix_t, typename T, size_t _rows, size_t _cols
>
LuDecomposition<Matrix_t<T, _rows, _cols>>::LuDecomposition(const Matrix_t<T, _rows, _cols> &a)
    : LU(a),
//#if defined(TL_HAVE_OPENBLAS) || defined(TL_HAVE_CUDA) 
#if defined(TL_HAVE_OPENBLAS)
    mPivotIndex(new int[a.rows()]),
#else
    mPivotIndex(a.rows()),
#endif
    d(consts::one<T>),
    mRows(a.rows()) 
{
    static_assert(std::is_floating_point<T>::value, "Integral type not supported");

    this->decompose();

}

template<
    template<typename, size_t, size_t>
class Matrix_t, typename T, size_t _rows, size_t _cols
>
LuDecomposition<Matrix_t<T, _rows, _cols>>::~LuDecomposition()
{
//#if defined(TL_HAVE_OPENBLAS) || defined(TL_HAVE_CUDA) 
#if defined(TL_HAVE_OPENBLAS)
    if (mPivotIndex) {
        delete[] mPivotIndex;
    }
#endif
}

template<
    template<typename, size_t, size_t>
class Matrix_t, typename T, size_t _rows, size_t _cols
>
auto LuDecomposition<Matrix_t<T, _rows, _cols>>::solve(const Vector<T, _rows>& b) const -> Vector<T, _rows>
{
    TL_ASSERT(b.size() == mRows, "LuDecomposition::solve bad sizes");

    Vector<T, _rows> x(b);

//#ifdef TL_HAVE_CUDA
//
//    int nrhs = 1;
//    int lda = static_cast<int>(mRows);
//    int ldb = 1;
//    cuda::getrs(static_cast<int>(mRows), nrhs, const_cast<T *>(LU.data()), lda, mPivotIndex, x.data(), ldb, nullptr);
//
//#elif defined TL_HAVE_OPENBLAS 
#ifdef TL_HAVE_OPENBLAS 
    lapack_int nrhs = 1;
    lapack_int lda = static_cast<int>(mRows);
    lapack_int ldb = 1;
    lapack_int info = lapack::getrs(static_cast<int>(mRows), nrhs, const_cast<T*>(LU.data()), lda, mPivotIndex, x.data(), ldb);

#else

    T sum;
    size_t ii = 0;
    for (size_t i = 0; i < mRows; i++) {

        size_t pivot_index = mPivotIndex[i];
        sum = x[pivot_index];
        x[pivot_index] = x[i];

        if (ii != 0) {
            for (size_t j = ii - 1; j < i; j++) {
                sum -= LU(i,j) * x[j];
            }
        } else if (sum != consts::zero<T>) {
            ii = i + 1;
        }

        x[i] = sum;
    }

    for (int i = static_cast<int>(mRows-1); i >= 0; i--) {

        sum = x[i];

        for (size_t j = static_cast<size_t>(i+1); j < mRows; j++)
            sum -= LU(i,j) * x[j];

        x[i] = sum / LU(i,i);
    }

#endif

    return x;
}

template<
    template<typename, size_t, size_t>
class Matrix_t, typename T, size_t _rows, size_t _cols
>
auto LuDecomposition<Matrix_t<T, _rows, _cols>>::solve(const Matrix_t<T, _rows, _cols> &b) const -> Matrix_t<T, _rows, _cols>
///Por ahora solo funciona con matrizes dinamicas
{
    TL_ASSERT(b.rows() == mRows, "LuDecomposition::solve bad sizes");

    Matrix_t<T, _rows, _cols> x(b);

//#ifdef TL_HAVE_CUDA
//
//    int nrhs = b.cols();
//    int lda = mRows;
//    int ldb = b.cols();
//    cuda::getrs(mRows, nrhs, const_cast<T *>(LU.data()), lda, mPivotIndex, x.data(), ldb, nullptr);
//
//#elif defined TL_HAVE_OPENBLAS 
#ifdef TL_HAVE_OPENBLAS
    lapack_int info;
    lapack_int nrhs = b.cols();
    lapack_int lda = mRows;
    lapack_int ldb = b.cols();

    info = lapack::getrs(mRows, nrhs, const_cast<T*>(LU.data()), lda, mPivotIndex, x.data(), ldb);

#else

    Vector<T, _rows> xx(mRows);

    for (size_t j = 0; j < x.cols(); j++) {

        for (size_t i = 0; i < mRows; i++) {
            xx[i] = b[i][j];
        }

        xx = this->solve(xx);

        for (size_t i = 0; i < mRows; i++) {
            x[i][j] = xx[i];
        }
    }

#endif

    return x;
}

template<
    template<typename, size_t, size_t>
class Matrix_t, typename T, size_t _rows, size_t _cols
>
void LuDecomposition<Matrix_t<T, _rows, _cols>>::decompose()
{
    
//#ifdef TL_HAVE_CUDA
//
//    int lda = mRows;
//    cuda::getrf(mRows, mRows, LU.data(), lda, mPivotIndex, nullptr);
//
//#elif defined TL_HAVE_OPENBLAS 
#ifdef TL_HAVE_OPENBLAS 
    lapack_int info;
    lapack_int lda = mRows;

    info = lapack::getrf(mRows, mRows, LU.data(), lda, mPivotIndex);

    TL_ASSERT(info == 0, "The algorithm computing LU failed to converge.");

#else
    
    Vector<T, _rows> max_elements = findMaxElementsByRows();

    for (size_t k = 0; k < mRows; k++) {

        T big = consts::zero<T>;
        size_t pivot_row = k;

        for (size_t i = k; i < mRows; i++) {
            T temp = std::abs(LU[i][k]) / max_elements[i];
            if (temp > big) {
                big = temp;
                pivot_row = i;
            }
        }

        if (k != pivot_row) {
            LU.swapRows(pivot_row, k);
            this->d = -this->d;
            std::swap(max_elements[pivot_row], max_elements[k]);
        }

        mPivotIndex[k] = pivot_row;

        if (isNearlyZero(LU[k][k]))
            LU[k][k] = std::numeric_limits<T>().min();

        T llkk = LU[k][k];

        for (size_t i = k + 1; i < mRows; i++) {

            T temp = LU[i][k] /= llkk;

            for (size_t j = k + 1; j < mRows; j++)
                LU[i][j] -= temp * LU[k][j];
        }
    }

#endif // TL_HAVE_OPENBLAS
}

template<
    template<typename, size_t, size_t>
class Matrix_t, typename T, size_t _rows, size_t _cols
>
auto LuDecomposition<Matrix_t<T, _rows, _cols>>::findMaxElementsByRows() const -> Vector<T, _rows>
{
    Vector<T, _rows> max_elements(mRows, consts::zero<T>);

    for (size_t r = 0; r < mRows; r++) {

        T max = consts::zero<T>;

        for (const auto& elem : LU[r]) {
            T abs_elem = std::abs(elem);
            if (abs_elem > max) {
                max = abs_elem;
            }
        }

        TL_ASSERT(!isNearlyZero(max), "Singular matrix");

        max_elements[r] = max;
    }

    return max_elements;
}

template<
    template<typename, size_t, size_t>
class Matrix_t, typename T, size_t _rows, size_t _cols
>
auto LuDecomposition<Matrix_t<T, _rows, _cols>>::lu() const -> Matrix_t<T, _rows, _cols>
{
    return LU;
}

template<
    template<typename, size_t, size_t>
class Matrix_t, typename T, size_t _rows, size_t _cols
>
auto LuDecomposition<Matrix_t<T, _rows, _cols>>::determinant() const -> T
{
    T det = this->d;

    for (size_t i = 0; i < mRows; i++)
        det *= LU[i][i];

    return det;
}


/*! \} */ // end of algebra

/*! \} */ // end of math

} // End namespace tl
