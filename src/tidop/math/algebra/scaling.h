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


/* Scaling */

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
    using reference = T &;
    using const_reference = const T &;

private:

    Vector<T, Dim> scale;

public:

    Scaling();
    Scaling(T scale);
    Scaling(T sx, T sy);
    Scaling(T sx, T sy, T sz);
    Scaling(const Vector<T, Dim> &vector);
    Scaling(const Scaling &scaling);
    Scaling(Scaling &&scaling) TL_NOEXCEPT;

    ~Scaling() = default;

    auto operator=(const Scaling &scaling) -> Scaling&;
    auto operator=(Scaling &&scaling) TL_NOEXCEPT -> Scaling &;

    auto toVector() -> Vector<T, Dim>;

    auto x() -> T;
    auto y() -> T;
    auto z() -> T;

    auto at(size_type position) -> reference;
    auto at(size_type position) const -> const_reference;

    auto operator[](size_t position) -> reference;
    auto operator[](size_t position) const -> const_reference;

    auto inverse() -> Scaling;

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

};



template<typename T, size_t Dim>
inline Scaling<T, Dim>::Scaling() 
    : scale(Vector<T, Dim>::unit()) 
{
}

template<typename T, size_t Dim>
inline Scaling<T, Dim>::Scaling(T scale)
    : scale(Dim, scale)
{
}

template<typename T, size_t Dim>
inline Scaling<T, Dim>::Scaling(T sx, T sy)
{
    static_assert(dimensions == 2, "Constructor for 2D Scaling. Use the 3D Scaling constructor: Scaling(T sx, T sy, T sz).");

    this->scale[0] = sx;
    this->scale[1] = sy;
}

template<typename T, size_t Dim>
inline Scaling<T, Dim>::Scaling(T sx, T sy, T sz)
{
    static_assert(dimensions == 3, "Constructor for 3D Scaling. Use the 2D Scaling constructor: Scaling(T sx, T sy).");

    this->scale[0] = sx;
    this->scale[1] = sy;
    this->scale[2] = sz;
}

template<typename T, size_t Dim>
inline Scaling<T, Dim>::Scaling(const Vector<T, Dim> &vector)
{
    this->scale = vector;
}

template<typename T, size_t Dim>
inline Scaling<T, Dim>::Scaling(const Scaling &scaling)
    : scale(scaling)
{
}

template<typename T, size_t Dim>
inline Scaling<T, Dim>::Scaling(Scaling &&scaling) TL_NOEXCEPT
    : scale(std::move(scaling.scale))
{
}

template<typename T, size_t Dim>
inline auto Scaling<T, Dim>::operator=(const Scaling &scaling) -> Scaling&
{
    if (this != &scaling) {
        this->scale = scaling.scale;
    }

    return (*this);
}

template<typename T, size_t Dim>
inline auto Scaling<T, Dim>::operator=(Scaling &&scaling) TL_NOEXCEPT -> Scaling &
{
    if (this != &scaling) {
        this->scale = std::move(scaling.scale);
    }

    return (*this);
}

template<typename T, size_t Dim>
inline auto Scaling<T, Dim>::toVector() -> Vector<T, Dim>
{
    return this->scale;
}

template<typename T, size_t Dim>
inline auto Scaling<T, Dim>::x() -> T
{
    return this->scale[0];
}

template<typename T, size_t Dim>
inline auto Scaling<T, Dim>::y() -> T
{
    return this->scale[1];
}

template<typename T, size_t Dim>
inline auto Scaling<T, Dim>::z() -> T
{
    static_assert(dimensions == 3, "Method not valid for 2D Scaling");
    return this->scale[2];
}

template<typename T, size_t Dim>
inline auto Scaling<T, Dim>::at(size_type position) -> reference
{
    return this->scale.at(position);
}

template<typename T, size_t Dim>
inline auto Scaling<T, Dim>::at(size_type position) const -> const_reference
{
    return this->scale.at(position);
}

template<typename T, size_t Dim>
inline auto Scaling<T, Dim>::operator[](size_t position) -> reference
{
    return this->scale[position];
}

template<typename T, size_t Dim>
inline auto Scaling<T, Dim>::operator[](size_t position) const -> const_reference
{
    return this->scale[position];
}

template<typename T, size_t Dim>
inline auto Scaling<T, Dim>::inverse() -> Scaling<T, Dim>
{
    Scaling<T, Dim> _scale;
    for (size_t i = 0; i < dimensions; i++)
        _scale[i] = consts::one<T> / this->scale[i];
 
    return _scale;
}

template<typename T, size_t Dim>
inline auto Scaling<T, Dim>::transform(const Point<T> &point) -> Point<T>
{
    static_assert(dimensions == 2, "");

    return Point<T>(this->scale[0] * point.x,
                    this->scale[1] * point.y);
}

template<typename T, size_t Dim>
inline auto Scaling<T, Dim>::transform(const Point3<T> &point) -> Point3<T>
{
    static_assert(dimensions == 3, "");

    return Point3<T>(this->scale[0] * point.x,
                     this->scale[1] * point.y,
                     this->scale[2] * point.z);
}

template<typename T, size_t Dim>
template<size_t _size>
inline auto Scaling<T, Dim>::transform(const Vector<T, _size> &vector) -> Vector<T, Dim>
{
    TL_ASSERT(dimensions == vector.size(), "Invalid Vector dimensions");

    return this->scale * vector;
}

template<typename T, size_t Dim>
template<size_t _row, size_t _col>
inline auto Scaling<T, Dim>::transform(const Matrix<T, _row, _col> &matrix) -> Matrix<T, _row, _col>
{
    TL_ASSERT(Dim == matrix.cols(), "Invalid matrix dimensions");

    Matrix<T, _row, _col> aux = matrix;
    for (size_t r = 0; r < matrix.rows(); r++) {
        aux[r] *= this->scale;
    }

    return aux;
}

template<typename T, size_t Dim>
auto Scaling<T, Dim>::operator * (const Point<T> &point) -> Point<T>
{
    return this->transform(point);
}

template<typename T, size_t Dim>
auto Scaling<T, Dim>::operator * (const Point3<T> &point) -> Point3<T>
{
    return this->transform(point);
}
 
template<typename T, size_t Dim>
template<size_t _size>
auto Scaling<T, Dim>::operator * (const Vector<T, _size> &vector) -> Vector<T, _size>
{
    return this->transform(vector);
}

template<typename T, size_t Dim>
template<size_t _row, size_t _col>
auto Scaling<T, Dim>::operator * (const Matrix<T, _row, _col> &matrix) -> Matrix<T, _row, _col>
{
    return this->transform(matrix);
}

/*! \} */ // end of Algebra

/*! \} */ // end of Math



} // End namespace tl

