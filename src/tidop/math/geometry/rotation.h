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

#include "tidop/math/algebra/vector.h"
#include "tidop/math/algebra/matrix.h"
#include "tidop/math/algebra/svd.h"
#include "tidop/math/algebra/rotation_convert.h"
#include "tidop/math/algebra/euler_angles.h"
#include "tidop/math/algebra/rotation_matrix.h"
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

/* Rotation */

template <typename T, size_t Dim>
class Rotation
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

    Matrix<T, Dim, Dim> rotation;

public:

    Rotation();
    explicit Rotation(T angle);
    Rotation(T omega, T phi, T kappa);
    explicit Rotation(const Matrix<T, Dim, Dim> &rotation);
    explicit Rotation(const Rotation &rotation);
    explicit Rotation(const EulerAngles<T> &eulerAngles);
    explicit Rotation(const AxisAngle<T> &axisAngle);
    explicit Rotation(const Quaternion<T> &quaternion);
    Rotation(Rotation &&rotation) TL_NOEXCEPT;

    ~Rotation() = default;

    auto operator=(const Rotation &rotation) -> Rotation&;
    auto operator=(Rotation &&rotation) TL_NOEXCEPT -> Rotation&;

    auto angle() const -> T;
    auto toMatrix() const -> Matrix<T, Dim, Dim>;

    auto operator()(size_t r, size_t c) -> reference;
    auto operator()(size_t r, size_t c) const -> const_reference;

    auto inverse() const -> Rotation;

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

    auto operator * (const Rotation<T, Dim> &rotation) const -> Rotation<T, Dim>;
};



template <typename T, size_t Dim>
class RotationEstimator
{

public:

    enum
    {
        dimensions = Dim,
        matrix_size
    };

public:

    RotationEstimator() = default;
    ~RotationEstimator() = default;

    template<size_t rows, size_t cols>
    static auto estimate(const Matrix<T, rows, cols> &src,
                         const Matrix<T, rows, cols> &dst) -> Rotation<T, Dim>;

    static auto estimate(const std::vector<Point<T>> &src,
                         const std::vector<Point<T>> &dst) -> Rotation<T, Dim>;

};




/* Rotation implementation */

template<typename T, size_t Dim>
Rotation<T, Dim>::Rotation()
  : rotation(Matrix<T, Dim, Dim>::identity())
{
    static_assert(dimensions == 2 || dimensions == 3, "Only 2 or 3 dimensions allowed");
}

template<typename T, size_t Dim>
Rotation<T, Dim>::Rotation(T angle)
{
    static_assert(dimensions == 2, "Constructor for 2D Rotation. Use the 3D Rotation constructor: Rotation(T omega, T phi, T kappa).");

    this->rotation[0][0] = cos(angle);
    this->rotation[0][1] = -sin(angle);
    this->rotation[1][0] = sin(angle);
    this->rotation[1][1] = cos(angle);
}

template<typename T, size_t Dim>
Rotation<T, Dim>::Rotation(T omega, T phi, T kappa)
{
    static_assert(dimensions == 3, "Constructor for 3D Rotation. Use the 2D Rotation constructor: Rotation(T angle).");

    EulerAngles<T> eulerAngles(omega, phi, kappa);
    RotationMatrix<T> rt = eulerAngles;
    this->rotation = rt;
}

template<typename T, size_t Dim>
Rotation<T, Dim>::Rotation(const Matrix<T, Dim, Dim> &rotation)
  : rotation(rotation)
{
}

template<typename T, size_t Dim>
Rotation<T, Dim>::Rotation(const Rotation &rotation)
  : rotation(rotation.rotation)
{
}

template<typename T, size_t Dim>
Rotation<T, Dim>::Rotation(const EulerAngles<T> &eulerAngles)
{
    static_assert(dimensions == 3, "Constructor for 3D Rotation. Use the 2D Rotation constructor: Rotation(T angle).");

    RotationMatrix<T> rt = const_cast<EulerAngles<T> &>(eulerAngles);
    this->rotation = rt;
}

template<typename T, size_t Dim>
Rotation<T, Dim>::Rotation(const AxisAngle<T> &axisAngle)
{
    static_assert(dimensions == 3, "Constructor for 3D Rotation. Use the 2D Rotation constructor: Rotation(T angle).");

    RotationMatrix<T> rt = const_cast<AxisAngle<T> &>(axisAngle);
    this->rotation = rt;
}

template<typename T, size_t Dim>
Rotation<T, Dim>::Rotation(const Quaternion<T> &quaternion)
{
    static_assert(dimensions == 3, "Constructor for 3D Rotation. Use the 2D Rotation constructor: Rotation(T angle).");

    RotationMatrix<T> rt = const_cast<Quaternion<T> &>(quaternion);
    this->rotation = rt;
}

template<typename T, size_t Dim>
Rotation<T, Dim>::Rotation(Rotation &&rotation) TL_NOEXCEPT
    : rotation(std::move(rotation.rotation))
{
}

template<typename T, size_t Dim>
auto Rotation<T, Dim>::operator=(const Rotation &rotation) -> Rotation &
{
    if (this != &rotation) {
        this->rotation = rotation.rotation;
    }

    return (*this);
}

template<typename T, size_t Dim>
auto Rotation<T, Dim>::operator=(Rotation &&rotation) TL_NOEXCEPT -> Rotation &
{
    if (this != &rotation) {
        this->rotation = std::move(rotation.rotation);
    }

    return (*this);
}

template<typename T, size_t Dim>
auto Rotation<T, Dim>::angle() const -> T
{
    return acos(this->rotation(0,0));
}

template<typename T, size_t Dim>
auto Rotation<T, Dim>::toMatrix() const -> Matrix<T, Dim, Dim>
{
    return this->rotation;
}

template<typename T, size_t Dim>
auto Rotation<T, Dim>::operator()(size_t r, size_t c) -> reference
{
    return this->rotation.at(r, c);
}

template<typename T, size_t Dim>
auto Rotation<T, Dim>::operator()(size_t r, size_t c) const -> const_reference
{
    return this->rotation.at(r, c);
}

template<typename T, size_t Dim>
auto Rotation<T, Dim>::inverse() const -> Rotation
{
    return Rotation(rotation.inverse());
}

template<typename T, size_t Dim>
auto Rotation<T, Dim>::transform(const Point<T> &point) const -> Point<T>
{
    static_assert(dimensions == 2, "Transformation not allowed for 2D points");

    return Point<T>(point.x * rotation(0, 0) + point.y * rotation(0, 1),
                    point.x * rotation(1, 0) + point.y * rotation(1, 1));
}

template<typename T, size_t Dim>
auto Rotation<T, Dim>::transform(const Point3<T> &point) const -> Point3<T>
{
    static_assert(dimensions == 3, "Transformation not allowed for 3D points");

    return Point3<T>(rotation(0, 0) * point.x + rotation(0, 1) * point.y + rotation(0, 2) * point.z,
                     rotation(1, 0) * point.x + rotation(1, 1) * point.y + rotation(1, 2) * point.z,
                     rotation(2, 0) * point.x + rotation(2, 1) * point.y + rotation(2, 2) * point.z);
}

template<typename T, size_t Dim>
template<size_t _size>
auto Rotation<T, Dim>::transform(const Vector<T, _size> &vector) const -> Vector<T, Dim>
{
    TL_ASSERT(dimensions == vector.size(), "Invalid Vector dimensions");

    return this->rotation * vector;
}

template<typename T, size_t Dim>
template<size_t _row, size_t _col>
auto Rotation<T, Dim>::transform(const Matrix<T, _row, _col> &matrix) const -> Matrix<T, _row, _col>
{
    TL_ASSERT(dimensions == matrix.cols(), "Invalid matrix dimensions");

    return matrix * rotation;
}

template<typename T, size_t Dim>
auto Rotation<T, Dim>::operator * (const Point<T> &point) const -> Point<T>
{
    return this->transform(point);
}

template<typename T, size_t Dim>
auto Rotation<T, Dim>::operator * (const Point3<T> &point) const -> Point3<T>
{
    return this->transform(point);
}

template<typename T, size_t Dim>
template<size_t _size>
auto Rotation<T, Dim>::operator * (const Vector<T, _size> &vector) const -> Vector<T, _size>
{
    return this->transform(vector);
}

template<typename T, size_t Dim>
template<size_t _row, size_t _col>
auto Rotation<T, Dim>::operator * (const Matrix<T, _row, _col> &matrix) const -> Matrix<T, _row, _col>
{
    return this->transform(matrix);
}

template<typename T, size_t Dim>
auto Rotation<T, Dim>::operator()(const Point<T> &point) const -> Point<T>
{
    return this->transform(point);
}

template<typename T, size_t Dim>
auto Rotation<T, Dim>::operator()(const Point3<T> &point) const -> Point3<T>
{
    return this->transform(point);
}

template<typename T, size_t Dim>
auto Rotation<T, Dim>::operator * (const Rotation<T, Dim> &rotation) const -> Rotation<T, Dim>
{
    return Rotation<T, Dim>( this->rotation + rotation.rotation);
}



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
auto RotationEstimator<T, Dim>::estimate(const std::vector<Point<T>> &src, 
                                         const std::vector<Point<T>> &dst) -> Rotation<T, Dim>
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

    return RotationEstimator<T, dimensions>::estimate(src_mat, dst_mat);
}


/*! \} */ // end of Algebra

/*! \} */ // end of Math




} // End namespace tl

