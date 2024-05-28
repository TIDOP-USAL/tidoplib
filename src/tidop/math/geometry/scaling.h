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

#include "tidop/math/algebra/matrix.h"
#include "tidop/math/algebra/vector.h"
#include "tidop/math/algebra/svd.h"
#include "tidop/geometry/entities/point.h"

namespace tl
{

/*! \addtogroup Math
 *  \{
 */


/*! \addtogroup Geometry
 *  \{
 */


/*!
 * /brief Scaling
 */
template <typename T, size_t Dim>
class Scaling
{

public:

    enum
    {
        dimensions = Dim
    };

    using value_type = T;
    using size_type = size_t;
    using reference = T&;
    using const_reference = const T&;

private:

    Vector<T, Dim> scale;

public:

    Scaling();
    explicit Scaling(T scale);
    Scaling(T sx, T sy);
    Scaling(T sx, T sy, T sz);
    explicit Scaling(const Vector<T, Dim> &vector);
    Scaling(const Scaling &scaling);
    Scaling(Scaling &&scaling) TL_NOEXCEPT;

    ~Scaling() = default;

    auto operator=(const Scaling &scaling) -> Scaling&;
    auto operator=(Scaling &&scaling) TL_NOEXCEPT -> Scaling &;

    auto toVector() const -> Vector<T, Dim>;

    auto x() const -> T;
    auto x() -> reference;
    auto y() const -> T;
    auto y() -> reference;
    auto z() const -> T;
    auto z() -> reference;

    auto at(size_type position) -> reference;
    auto at(size_type position) const -> const_reference;

    auto operator[](size_t position) -> reference;
    auto operator[](size_t position) const -> const_reference;

    auto inverse() const -> Scaling;

    /// Clases que deberían ser virtuales
    auto transform(const Point<T> &point) const -> Point<T>;
    auto transform(const Point3<T> &point) const -> Point3<T>;
    template<size_t _size>
    auto transform(const Vector<T, _size> &vector) const -> Vector<T, Dim>;
    template<size_t _row, size_t _col>
    auto transform(const Matrix<T, _row, _col> &matrix) const -> Matrix<T, _row, _col>;

    auto operator * (const Point<T> &point) const -> Point<T>;
    auto operator * (const Point3<T> &point) const -> Point3<T>;
    template<size_t _size>
    auto operator * (const Vector<T, _size> &vector) const -> Vector<T, _size>;
    template<size_t _row, size_t _col>
    auto operator * (const Matrix<T, _row, _col> &matrix) const -> Matrix<T, _row, _col>;

    auto operator() (const Point<T> &point) const -> Point<T>;
    auto operator() (const Point3<T> &point) const -> Point3<T>;

    auto operator * (const Scaling<T, Dim> &scaling) const -> Scaling<T, Dim>;
};


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

    template<size_t rows, size_t cols>
    static auto estimate(const Matrix<T, rows, cols> &src,
                         const Matrix<T, rows, cols> &dst) -> Scaling<T, Dim>;

    static auto estimate(const std::vector<Point<T>> &src,
                         const std::vector<Point<T>> &dst) -> Scaling<T, Dim>;

};



/* Scaling implementation */


template<typename T, size_t Dim>
Scaling<T, Dim>::Scaling() 
  : scale(Vector<T, Dim>::unit()) 
{
}

template<typename T, size_t Dim>
Scaling<T, Dim>::Scaling(T scale)
  : scale(Dim, scale)
{
}

template<typename T, size_t Dim>
Scaling<T, Dim>::Scaling(T sx, T sy)
{
    static_assert(dimensions == 2, "Constructor for 2D Scaling. Use the 3D Scaling constructor: Scaling(T sx, T sy, T sz).");

    this->scale[0] = sx;
    this->scale[1] = sy;
}

template<typename T, size_t Dim>
Scaling<T, Dim>::Scaling(T sx, T sy, T sz)
{
    static_assert(dimensions == 3, "Constructor for 3D Scaling. Use the 2D Scaling constructor: Scaling(T sx, T sy).");

    this->scale[0] = sx;
    this->scale[1] = sy;
    this->scale[2] = sz;
}

template<typename T, size_t Dim>
Scaling<T, Dim>::Scaling(const Vector<T, Dim> &vector)
{
    this->scale = vector;
}

template<typename T, size_t Dim>
Scaling<T, Dim>::Scaling(const Scaling &scaling)
    : scale(scaling)
{
}

template<typename T, size_t Dim>
Scaling<T, Dim>::Scaling(Scaling &&scaling) TL_NOEXCEPT
    : scale(std::move(scaling.scale))
{
}

template<typename T, size_t Dim>
auto Scaling<T, Dim>::operator=(const Scaling &scaling) -> Scaling&
{
    if (this != &scaling) {
        this->scale = scaling.scale;
    }

    return (*this);
}

template<typename T, size_t Dim>
auto Scaling<T, Dim>::operator=(Scaling &&scaling) TL_NOEXCEPT -> Scaling &
{
    if (this != &scaling) {
        this->scale = std::move(scaling.scale);
    }

    return (*this);
}

template<typename T, size_t Dim>
auto Scaling<T, Dim>::toVector() const -> Vector<T, Dim>
{
    return this->scale;
}

template<typename T, size_t Dim>
auto Scaling<T, Dim>::x() const -> T
{
    return this->scale[0];
}

template<typename T, size_t Dim>
auto Scaling<T, Dim>::x() -> reference
{
    return this->scale[0];
}

template<typename T, size_t Dim>
auto Scaling<T, Dim>::y() const -> T
{
    return this->scale[1];
}

template<typename T, size_t Dim>
auto Scaling<T, Dim>::y() -> reference
{
    return this->scale[1];
}

template<typename T, size_t Dim>
auto Scaling<T, Dim>::z() const -> T
{
    static_assert(dimensions == 3, "Method not valid for 2D Scaling");
    return this->scale[2];
}

template<typename T, size_t Dim>
auto Scaling<T, Dim>::z() -> reference
{
    static_assert(dimensions == 3, "Method not valid for 2D Scaling");
    return this->scale[2];
}

template<typename T, size_t Dim>
auto Scaling<T, Dim>::at(size_type position) -> reference
{
    return this->scale.at(position);
}

template<typename T, size_t Dim>
auto Scaling<T, Dim>::at(size_type position) const -> const_reference
{
    return this->scale.at(position);
}

template<typename T, size_t Dim>
auto Scaling<T, Dim>::operator[](size_t position) -> reference
{
    return this->scale[position];
}

template<typename T, size_t Dim>
auto Scaling<T, Dim>::operator[](size_t position) const -> const_reference
{
    return this->scale[position];
}

template<typename T, size_t Dim>
auto Scaling<T, Dim>::inverse() const -> Scaling<T, Dim>
{
    Scaling<T, Dim> _scale;
    for (size_t i = 0; i < dimensions; i++)
        _scale[i] = consts::one<T> / this->scale[i];
 
    return _scale;
}

template<typename T, size_t Dim>
auto Scaling<T, Dim>::transform(const Point<T> &point) const -> Point<T>
{
    static_assert(dimensions == 2, "");

    return Point<T>(this->scale[0] * point.x,
                    this->scale[1] * point.y);
}

template<typename T, size_t Dim>
auto Scaling<T, Dim>::transform(const Point3<T> &point) const -> Point3<T>
{
    static_assert(dimensions == 3, "");

    return Point3<T>(this->scale[0] * point.x,
                     this->scale[1] * point.y,
                     this->scale[2] * point.z);
}

template<typename T, size_t Dim>
template<size_t _size>
auto Scaling<T, Dim>::transform(const Vector<T, _size> &vector) const -> Vector<T, Dim>
{
    TL_ASSERT(dimensions == vector.size(), "Invalid Vector dimensions");

    return this->scale * vector;
}

template<typename T, size_t Dim>
template<size_t _row, size_t _col>
auto Scaling<T, Dim>::transform(const Matrix<T, _row, _col> &matrix) const -> Matrix<T, _row, _col>
{
    TL_ASSERT(Dim == matrix.cols(), "Invalid matrix dimensions");

    Matrix<T, _row, _col> aux = matrix;
    for (size_t r = 0; r < matrix.rows(); r++) {
        aux[r] *= this->scale;
    }

    return aux;
}

template<typename T, size_t Dim>
auto Scaling<T, Dim>::operator * (const Point<T> &point) const -> Point<T>
{
    return this->transform(point);
}

template<typename T, size_t Dim>
auto Scaling<T, Dim>::operator * (const Point3<T> &point) const -> Point3<T>
{
    return this->transform(point);
}
 
template<typename T, size_t Dim>
template<size_t _size>
auto Scaling<T, Dim>::operator * (const Vector<T, _size> &vector) const -> Vector<T, _size>
{
    return this->transform(vector);
}

template<typename T, size_t Dim>
template<size_t _row, size_t _col>
auto Scaling<T, Dim>::operator * (const Matrix<T, _row, _col> &matrix) const -> Matrix<T, _row, _col>
{
    return this->transform(matrix);
}

template<typename T, size_t Dim>
auto Scaling<T, Dim>::operator()(const Point<T> &point) const -> Point<T>
{
    return this->transform(point);
}

template<typename T, size_t Dim>
auto Scaling<T, Dim>::operator()(const Point3<T> &point) const -> Point3<T>
{
    return this->transform(point);
}

template<typename T, size_t Dim>
auto Scaling<T, Dim>::operator*(const Scaling<T, Dim> &scaling) const -> Scaling<T, Dim>
{
    return Scaling<T, Dim>(this->scale * scaling.scale);
}




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
auto ScalingEstimator<T, Dim>::estimate(const std::vector<Point<T>> &src, const std::vector<Point<T>> &dst) -> Scaling<T, Dim>
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

    return ScalingEstimator<T, dimensions>::estimate(src_mat, dst_mat);
}


/*! \} */ // end of Geometry

/*! \} */ // end of Math



} // End namespace tl

