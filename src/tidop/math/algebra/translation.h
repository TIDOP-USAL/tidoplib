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
#include <array>

#include "tidop/math/algebra/vector.h"
#include "tidop/math/algebra/matrix.h"
#include "tidop/math/algebra/svd.h"
#include "tidop/geometry/entities/point.h"

namespace tl
{

/*! \addtogroup Math
 *  \{
 */


/*! \addtogroup Algebra
 *
 * Algebra
 *
 *  \{
 */

/* Translation */

template <typename T, size_t Dim>
class Translation
{

public:

    enum
    {
        dimensions = Dim
    };

    using value_type = T;
    using size_type = size_t;
    using reference = T &;
    using const_reference = const T &;

private:

    Vector<T, Dim> translation;

public:

    Translation();
    Translation(T tx, T ty);
    Translation(T tx, T ty, T tz);
    Translation(const Vector<T, Dim> &vector);
    Translation(const Translation &translation);
    Translation(Translation &&translation) TL_NOEXCEPT;

    ~Translation() = default;

    auto operator=(const Translation &translate) -> Translation&;
    auto operator=(Translation &&translate) TL_NOEXCEPT -> Translation &;

    auto toVector() -> Vector<T, Dim>;

    auto x() const -> T;
    auto y() const -> T;
    auto z() const -> T;

    auto at(size_type position) -> reference;
    auto at(size_type position) const -> const_reference;

    auto operator[](size_t position) -> reference;
    auto operator[](size_t position) const -> const_reference;

    auto inverse() -> Translation;

    /// Clases que deberían ser virtuales
    auto transform(const Point<T> &point) -> Point<T>;
    auto transform(const Point3<T> &point) -> Point3<T>;
    template<size_t _size>
    auto transform(const Vector<T, _size> &vector) -> Vector<T, Dim>;
    template<size_t _row, size_t _col>
    auto transform(const Matrix<T, _row, _col> &matrix) -> Matrix<T, _row, _col>;

    auto operator * (const Point<T> &point) -> Point<T>;
    auto operator * (const Point3<T> &point) -> Point3<T>;
    template<size_t _size>
    auto operator * (const Vector<T, _size> &vector) -> Vector<T, _size>;
    template<size_t _row, size_t _col>
    auto operator * (const Matrix<T, _row, _col> &matrix) -> Matrix<T, _row, _col>;

    auto operator * (const Translation<T, Dim> &translation) -> Translation<T, Dim>;
};



template <typename T, size_t Dim>
class TranslationEstimator
{

public:

    enum
    {
        dimensions = Dim,
        matrix_size
    };

public:

    TranslationEstimator() = default;
    ~TranslationEstimator() = default;

    template<size_t rows, size_t cols>
    static auto estimate(const Matrix<T, rows, cols> &src,
                         const Matrix<T, rows, cols> &dst) -> Matrix<T, matrix_size, matrix_size>;

    static auto estimate(const std::vector<Point<T>> &src,
                         const std::vector<Point<T>> &dst) -> Matrix<T, matrix_size, matrix_size>;

};




/* Translation implementation */

template<typename T, size_t Dim>
inline Translation<T, Dim>::Translation()
    : translation(Vector<T, Dim>::zero())
{
    static_assert(dimensions == 2 || dimensions == 3, "Only 2 or 3 dimensions allowed");
}

template<typename T, size_t Dim>
inline Translation<T, Dim>::Translation(T tx, T ty)

{
    static_assert(dimensions == 2, "Constructor for 2D Translation. Use the 3D Translation constructor: Translation(T tx, T ty, T tz).");

    this->translation[0] = tx;
    this->translation[1] = ty;
}

template<typename T, size_t Dim>
inline Translation<T, Dim>::Translation(T tx, T ty, T tz)
{
    static_assert(dimensions == 3, "Constructor for 3D Translation. Use the 2D Translation constructor: Translation(T tx, T ty).");

    this->translation[0] = tx;
    this->translation[1] = ty;
    this->translation[2] = tz;
}

template<typename T, size_t Dim>
inline Translation<T, Dim>::Translation(const Vector<T, Dim> &vector)
{
    this->translation = vector;
}

template<typename T, size_t Dim>
inline Translation<T, Dim>::Translation(const Translation &translate)
    : translation(translate.translation)
{
}

template<typename T, size_t Dim>
inline Translation<T, Dim>::Translation(Translation &&translate) TL_NOEXCEPT
    : translation(std::move(translate.translation))
{
}

template<typename T, size_t Dim>
inline auto Translation<T, Dim>::operator=(const Translation &translate) -> Translation &
{
    if (this != &translate) {
        this->translation = translate.translation;
    }

    return (*this);
}

template<typename T, size_t Dim>
inline auto Translation<T, Dim>::operator=(Translation &&translate) TL_NOEXCEPT -> Translation &
{
    if (this != &translate) {
        this->translation = std::move(translate.translation);
    }

    return (*this);
}

template<typename T, size_t Dim>
inline auto Translation<T, Dim>::toVector() -> Vector<T, Dim>
{
    return this->translation;
}

template<typename T, size_t Dim>
inline auto Translation<T, Dim>::x() const -> T
{
    return this->translation[0];
}

template<typename T, size_t Dim>
inline auto Translation<T, Dim>::y() const -> T
{
    return this->translation[1];
}

template<typename T, size_t Dim>
inline auto Translation<T, Dim>::z() const -> T
{
    static_assert(dimensions == 3, "Method not valid for 2D translations");
    return this->translation[2];
}

template<typename T, size_t Dim>
inline auto Translation<T, Dim>::at(size_type position) -> reference
{
    return this->translation.at(position);
}

template<typename T, size_t Dim>
inline auto Translation<T, Dim>::at(size_type position) const -> const_reference
{
    return this->translation.at(position);
}

template<typename T, size_t Dim>
inline auto Translation<T, Dim>::operator[](size_t position) -> reference
{
    return this->translation[position];
}

template<typename T, size_t Dim>
inline auto Translation<T, Dim>::operator[](size_t position) const -> const_reference
{
    return this->translation[position];
}

template<typename T, size_t Dim>
inline auto Translation<T, Dim>::inverse() -> Translation
{
    return Translation(-this->translation);
}

template<typename T, size_t Dim>
inline auto Translation<T, Dim>::transform(const Point<T> &point) -> Point<T>
{
    static_assert(dimensions == 2, "Transformation not allowed for 2D points");

    return Point<T>(this->translation[0] + point.x,
                    this->translation[1] + point.y);
}

template<typename T, size_t Dim>
inline auto Translation<T, Dim>::transform(const Point3<T> &point) -> Point3<T>
{
    static_assert(dimensions == 3, "Transformation not allowed for 3D points");

    return Point3<T>(this->translation[0] + point.x,
                     this->translation[1] + point.y,
                     this->translation[2] + point.z);
}

template<typename T, size_t Dim>
template<size_t _size>
inline auto Translation<T, Dim>::transform(const Vector<T, _size> &vector) -> Vector<T, Dim>
{
    TL_ASSERT(dimensions == vector.size(), "Invalid Vector dimensions");

    return this->translation + vector;
}

template<typename T, size_t Dim>
template<size_t _row, size_t _col>
inline auto Translation<T, Dim>::transform(const Matrix<T, _row, _col> &matrix) -> Matrix<T, _row, _col>
{
    TL_ASSERT(dimensions == matrix.cols(), "Invalid matrix dimensions");

    Matrix<T, _row, _col> aux = matrix;
    for (size_t r = 0; r < matrix.rows(); r++) {
        aux[r] += this->translation;
    }

    return aux;
}

template<typename T, size_t Dim>
auto Translation<T, Dim>::operator * (const Point<T> &point) -> Point<T>
{
    return this->transform(point);
}

template<typename T, size_t Dim>
auto Translation<T, Dim>::operator * (const Point3<T> &point) -> Point3<T>
{
    return this->transform(point);
}

template<typename T, size_t Dim>
template<size_t _size>
auto Translation<T, Dim>::operator * (const Vector<T, _size> &vector) -> Vector<T, _size>
{
    return this->transform(vector);
}

template<typename T, size_t Dim>
template<size_t _row, size_t _col>
auto Translation<T, Dim>::operator * (const Matrix<T, _row, _col> &matrix) -> Matrix<T, _row, _col>
{
    return this->transform(matrix);
}

template<typename T, size_t Dim>
auto Translation<T, Dim>::operator * (const Translation<T, Dim> &translation) -> Translation<T, Dim>
{
    return Translation<T, Dim>( this->translation + translation.translation);
}



/* TranslationEstimator implementation */

template<typename T, size_t Dim>
template<size_t rows, size_t cols>
inline auto TranslationEstimator<T, Dim>::estimate(const Matrix<T, rows, cols> &src, 
                                                   const Matrix<T, rows, cols> &dst) -> Matrix<T, matrix_size, matrix_size>
{
    static_assert(dimensions == 2, "Scale estimator only for 2D Scaling");

    auto transformMatrix = Matrix<double, matrix_size, matrix_size>::identity();

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

        transformMatrix(0, dimensions) = C[2];
        transformMatrix(1, dimensions) = C[3];

    } catch (...) {
        TL_THROW_EXCEPTION_WITH_NESTED("");
    }

    return transformMatrix;
}

template<typename T, size_t Dim>
inline auto TranslationEstimator<T, Dim>::estimate(const std::vector<Point<T>> &src, 
                                                   const std::vector<Point<T>> &dst) -> Matrix<T, matrix_size, matrix_size>
{
    TL_ASSERT(src.size() == dst.size(), "Size of origin and destination points different");

    Matrix<T> src_mat(src.size(), dimensions);
    Matrix<T> dst_mat(dst.size(), dimensions);

    for (size_t r = 0; r < src_mat.rows(); r++) {
        src_mat[r][0] = src[r].x;
        src_mat[r][1] = src[r].y;

        dst_mat[r][0] = dst[r].x;
        dst_mat[r][1] = dst[r].y;
    }

    return TranslationEstimator<T, dimensions>::estimate(src_mat, dst_mat);
}


/*! \} */ // end of Algebra

/*! \} */ // end of Math




} // End namespace tl

