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
#include "tidop/math/lapack.h"


namespace tl
{

/*! \addtogroup math
 *  \{
 */


 /*! \addtogroup algebra
  *  \{
  */


  /*!
   * \brief Factorización Cholesky
   *
   * Cualquier matriz cuadrada A con pivotes no nulos puede ser escrita como el producto
   * de una matriz triangular inferior L y una matriz triangular superior U; esto recibe
   * el nombre de factorización LU. Sin embargo, si A es simétrica y definida positiva,
   * se pueden escoger los factores tales que U es la transpuesta de L, y esto se llama
   * la descomposición o factorización de Cholesky.
   *
   * La matriz A tiene que ser simétrica y definida positiva.
   */
template<typename T>
class CholeskyDecomposition;

template<
    template<typename, size_t, size_t>
class Matrix_t, typename T, size_t _rows, size_t _cols
>
class CholeskyDecomposition<Matrix_t<T, _rows, _cols>>
{

public:

    CholeskyDecomposition(const Matrix_t<T, _rows, _cols> &a);

    auto solve(const Vector<T, _rows>& b) -> Vector<T, _rows>;
    auto l() const -> Matrix<T, _rows, _cols>;

private:

    void decompose();

protected:

    Matrix<T, _rows, _cols> L;
    size_t mRows;
};


template<
    template<typename, size_t, size_t>
class Matrix_t, typename T, size_t _rows, size_t _cols
>
CholeskyDecomposition<Matrix_t<T, _rows, _cols>>::CholeskyDecomposition(const Matrix_t<T, _rows, _cols> &a)
  : L(a),
    mRows(a.rows())
{
    static_assert(std::is_floating_point<T>::value, "Integral type not supported");

    this->decompose();

    for (size_t i = 0; i < mRows; i++) {
        for (size_t j = 0; j < i; j++) {
            L[j][i] = consts::zero<T>;
        }
    }
}

template<
    template<typename, size_t, size_t>
class Matrix_t, typename T, size_t _rows, size_t _cols
>
auto CholeskyDecomposition<Matrix_t<T, _rows, _cols>>::solve(const Vector<T, _rows>& b) -> Vector<T, _rows>
{
    TL_ASSERT(b.size() == mRows, "bad lengths in Cholesky");

    T sum;
    Vector<T, _rows> x(b);

    for (size_t r = 0; r < mRows; r++) {

        sum = b[r];

        for (size_t k = r; k > 0; k--)
            sum -= L[r][k - 1] * x[k - 1];

        x[r] = sum / L[r][r];

    }

    for (size_t i = mRows; i > 0; i--) {

        sum = x[i - 1];

        for (size_t k = i; k < mRows; k++)
            sum -= L[k][i - 1] * x[k];

        x[i - 1] = sum / L[i - 1][i - 1];

    }

    return x;
}

template<
    template<typename, size_t, size_t>
class Matrix_t, typename T, size_t _rows, size_t _cols
>
void CholeskyDecomposition<Matrix_t<T, _rows, _cols>>::decompose()
{
#ifdef TL_HAVE_OPENBLAS
    lapack_int info;

    info = lapack::potrf(L.rows(), L.data(), L.cols());

    TL_ASSERT(info >= 0, "Cholesky decomposition failed");
#else
    for (size_t i = 0; i < mRows; i++) {

        for (size_t j = i; j < mRows; j++) {

            T sum = L[i][j];

            for (size_t k = i; k > 0; k--) {
                sum -= L[i][k - 1] * L[j][k - 1];
            }

            if (i == j) {
                TL_ASSERT(sum > 0.0, "Cholesky decomposition failed");
                L[i][i] = sqrt(sum);
            } else {
                L[j][i] = sum / L[i][i];
            }

        }
    }
#endif
}

template<
    template<typename, size_t, size_t>
class Matrix_t, typename T, size_t _rows, size_t _cols
>
auto CholeskyDecomposition<Matrix_t<T, _rows, _cols>>::l() const -> Matrix<T, _rows, _cols>
{
    return L;
}

/*! \} */ // end of algebra

/*! \} */ // end of math

} // End namespace tl
