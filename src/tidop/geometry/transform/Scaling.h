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

#include "tidop/math/algebra/matrix/Matrix.h"
#include "tidop/math/algebra/vector/Vector.h"
#include "tidop/math/algebra/decomp/SVD.h"
#include "tidop/geometry/primitives/Point.h"
#include "tidop/math/base/Traits.h"
#include "tidop/math/base/Concepts.h"
#include "tidop/geometry/base/Traits.h"

namespace tl
{

/*! \addtogroup GeometricTransformations
 *  \{
 */

/*!
 * \brief Scaling transformation.
 * 
 * The `Scaling` class represents a scaling transformation that can be applied 
 * to points, vectors, or matrices. Scaling operations change the size of objects 
 * along one or more axes. The class supports both uniform and non-uniform scaling 
 * in different dimensions.
 *
 * \tparam T The type of the scaling factor (e.g., float, double).
 * \tparam Dim The dimensionality of the scaling (e.g., 2 for 2D, 3 for 3D).
 */
template <typename T, size_t Dim>
class Scaling
{

public:

    /*!
     * \brief The dimensionality of the scaling.
     */
    enum
    {
        dimensions = Dim ///< Dimensionality of the scaling (2D or 3D).
    };

    using value_type = T;
    using size_type = size_t;
    using reference = T&;
    using const_reference = const T&;

private:

    Vector<T, Dim> scale;

public:

    /*!
     * \brief Default constructor.
     */
    Scaling();

    /*!
     * \brief Constructor for uniform scaling.
     *
     * Creates a scaling transformation with the same scaling factor for all axes.
     *
     * \param[in] scale The scaling factor.
     */
    explicit Scaling(T scale);

    /*!
     * \brief Constructor for non-uniform scaling in 2D.
     *
     * \param[in] sx Scaling factor along the x-axis.
     * \param[in] sy Scaling factor along the y-axis.
     */
    Scaling(T sx, T sy);

    /*!
     * \brief Constructor for non-uniform scaling in 3D.
     *
     * \param[in] sx Scaling factor along the x-axis.
     * \param[in] sy Scaling factor along the y-axis.
     * \param[in] sz Scaling factor along the z-axis.
     */
    Scaling(T sx, T sy, T sz);

    /*!
     * \brief Constructor from a vector of scaling factors.
     *
     * \param[in] vector A vector containing the scaling factors for each dimension.
     */
    explicit Scaling(const Vector<T, Dim> &vector);

    /*!
     * \brief Copy constructor.
     */
    Scaling(const Scaling &scaling);

    /*!
     * \brief Move constructor.
     */
    Scaling(Scaling &&scaling) TL_NOEXCEPT;

    /*!
     * \brief Default destructor.
     */
    ~Scaling() = default;

    /*!
     * \brief Copy assignment operator.
     */
    auto operator=(const Scaling &scaling) -> Scaling&;

    /*!
     * \brief Move assignment operator.
     */
    auto operator=(Scaling &&scaling) TL_NOEXCEPT -> Scaling &;

     /*!
     * \brief Convert the scaling transformation to a vector.
     * 
     * \return A vector containing the scaling factors.
     */
    auto toVector() const TL_NOEXCEPT -> Vector<T, Dim>;

    /*!
     * \brief Access the scaling factor along the x-axis.
     * \return The scaling factor along the x-axis.
     */
    auto x() const TL_NOEXCEPT -> const_reference;

    /*!
     * \brief Access the scaling factor along the x-axis (non-const version).
     * \return A reference to the scaling factor along the x-axis.
     */
    auto x() TL_NOEXCEPT -> reference;

    /*!
     * \brief Access the scaling factor along the y-axis.
     * \return The scaling factor along the y-axis.
     */
    auto y() const TL_NOEXCEPT -> const_reference;

    /*!
     * \brief Access the scaling factor along the y-axis (non-const version).
     * \return A reference to the scaling factor along the y-axis.
     */
    auto y() TL_NOEXCEPT -> reference;

    /*!
     * \brief Access the scaling factor along the z-axis.
     * \return The scaling factor along the z-axis.
     */
    auto z() const TL_NOEXCEPT -> const_reference;

    /*!
     * \brief Access the scaling factor along the z-axis (non-const version).
     * \return A reference to the scaling factor along the z-axis.
     */
    auto z() TL_NOEXCEPT -> reference;

    /*!
     * \brief Access the scaling factor at a given position.
     * \param[in] position The position of the scaling factor.
     * \return A reference to the scaling factor at the given position.
     */
    auto at(size_type position) -> reference;

    /*!
     * \brief Access the scaling factor at a given position (const version).
     * \param[in] position The position of the scaling factor.
     * \return A constant reference to the scaling factor at the given position.
     */
    auto at(size_type position) const -> const_reference;

    /*!
     * \brief Access the scaling factor at a given position using array indexing.
     * \param[in] position The position of the scaling factor.
     * \return A reference to the scaling factor at the given position.
     */
    auto operator[](size_t position) TL_NOEXCEPT -> reference;

    /*!
     * \brief Access the scaling factor at a given position using array indexing (const version).
     * \param[in] position The position of the scaling factor.
     * \return A constant reference to the scaling factor at the given position.
     */
    auto operator[](size_t position) const TL_NOEXCEPT -> const_reference;

    /*!
     * \brief Compute the inverse scaling transformation.
     * 
     * \return The inverse of the scaling transformation.
     */
    auto inverse() const -> Scaling;

    /*!
     * \brief Transform a Point or Vector using the scaling.
     * 
     * \param[in] vector The vector to transform.
     * \return The transformed vector.
     */
    template<VectorExpr Vector_t>
    auto transform(const Vector_t &vector) const -> Vector_t;

    template<typename Tag>
    auto transform(const Point<T, Tag> &point) const -> Point<T, Tag>;

    /*!
     * \brief Transform a matrix using the scaling.
     * 
     * \param[in] matrix The matrix to transform.
     * \return The transformed matrix.
     */
    template<size_t _row, size_t _col>
    auto transform(const Matrix<T, _row, _col> &matrix) const -> Matrix<T, _row, _col>;

    /*!
     * \brief Apply the scaling to a Point or Vector.
     * \param[in] vector The vector to scale.
     * \return The scaled vector.
     */
    template <VectorExpr Vector_t>
    auto operator * (const Vector_t &vector) const -> Vector_t;

    template<typename Tag>
    auto operator * (const Point<T, Tag> &point) const -> Point<T, Tag>;

    /*!
     * \brief Apply the scaling to a matrix.
     * \param[in] matrix The matrix to scale.
     * \return The scaled matrix.
     */
    template<size_t _row, size_t _col>
    auto operator * (const Matrix<T, _row, _col> &matrix) const -> Matrix<T, _row, _col>;

    /*!
     * \brief Apply the scaling to a Point or Vector (using function call).
     * \param[in] point The point to scale.
     * \return The scaled point.
     */
    template <VectorExpr Vector_t>
    auto operator()(const Vector_t &vector) const -> Vector_t;

    template<typename Tag>
    auto operator()(const Point<T, Tag> &point) const -> Point<T, Tag>;

    /*!
     * \brief Apply the scaling to another scaling transformation.
     * \param[in] scaling The scaling transformation to apply.
     * \return The combined scaling transformation.
     */
    auto operator * (const Scaling<T, Dim> &scaling) const -> Scaling<T, Dim>;
};

/*! \} */


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
auto Scaling<T, Dim>::toVector() const TL_NOEXCEPT -> Vector<T, Dim>
{
    return this->scale;
}

template<typename T, size_t Dim>
auto Scaling<T, Dim>::x() const TL_NOEXCEPT -> const_reference
{
    return this->scale[0];
}

template<typename T, size_t Dim>
auto Scaling<T, Dim>::x() TL_NOEXCEPT -> reference
{
    return this->scale[0];
}

template<typename T, size_t Dim>
auto Scaling<T, Dim>::y() const TL_NOEXCEPT -> const_reference
{
    return this->scale[1];
}

template<typename T, size_t Dim>
auto Scaling<T, Dim>::y() TL_NOEXCEPT -> reference
{
    return this->scale[1];
}

template<typename T, size_t Dim>
auto Scaling<T, Dim>::z() const TL_NOEXCEPT -> const_reference
{
    static_assert(dimensions == 3, "Method not valid for 2D Scaling");
    return this->scale[2];
}

template<typename T, size_t Dim>
auto Scaling<T, Dim>::z() TL_NOEXCEPT -> reference
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
auto Scaling<T, Dim>::operator[](size_t position) TL_NOEXCEPT -> reference
{
    return this->scale[position];
}

template<typename T, size_t Dim>
auto Scaling<T, Dim>::operator[](size_t position) const TL_NOEXCEPT -> const_reference
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
template<VectorExpr Vector_t>
auto Scaling<T, Dim>::transform(const Vector_t &vector) const -> Vector_t
{
    static_assert(vector.dimensions == DynamicData || vector.dimensions == Dim, "Vector dimension must match Affine transformation dimension");
    TL_ASSERT(dimensions == vector.size(), "Vector dimension must match Affine transformation dimension");
    
    Vector_t result;
    if constexpr (is_vector<Vector_t>::value)
        result = vector.cwiseProduct(this->scale);
    else {
        for (size_t i = 0; i < vector.size(); ++i) {
            result[i] = vector[i] * this->scale[i];
        }
    }
    return result;
}

template<typename T, size_t Dim>
template<typename Tag>
auto Scaling<T, Dim>::transform(const Point<T, Tag> &point) const -> Point<T, Tag>
{
    static_assert(Tag::storage_size == dimensions, "Point dimension must match Affine transformation dimension");
    Point<T, Tag> result;
    for (size_t i = 0; i < dimensions; ++i) {
        result[i] = point[i] * this->scale[i];
    }
    return result;
}


template<typename T, size_t Dim>
template<size_t _row, size_t _col>
auto Scaling<T, Dim>::transform(const Matrix<T, _row, _col> &matrix) const -> Matrix<T, _row, _col>
{
    TL_ASSERT(Dim == matrix.cols(), "Invalid matrix dimensions");

    Matrix<T, _row, _col> aux = matrix;
    for (size_t r = 0; r < matrix.rows(); r++) {
        aux[r].cwiseProductInPlace(this->scale);
    }

    return aux;
}

template<typename T, size_t Dim>
template<VectorExpr Vector_t>
auto Scaling<T, Dim>::operator*(const Vector_t &vector) const -> Vector_t
{
    return this->transform(vector);
}

template<typename T, size_t Dim>
template<typename Tag>
auto Scaling<T, Dim>::operator*(const Point<T, Tag> &point) const -> Point<T, Tag>
{
    return this->transform(point);
}

template<typename T, size_t Dim>
template<size_t _row, size_t _col>
auto Scaling<T, Dim>::operator * (const Matrix<T, _row, _col> &matrix) const -> Matrix<T, _row, _col>
{
    return this->transform(matrix);
}

template<typename T, size_t Dim>
template<VectorExpr Vector_t>
auto Scaling<T, Dim>::operator()(const Vector_t &vector) const -> Vector_t
{
    return this->transform(vector);
}

template<typename T, size_t Dim>
template<typename Tag>
auto Scaling<T, Dim>::operator()(const Point<T, Tag> &point) const -> Point<T, Tag>
{
    return this->transform(point);
}

template<typename T, size_t Dim>
auto Scaling<T, Dim>::operator*(const Scaling<T, Dim> &scaling) const -> Scaling<T, Dim>
{
    return Scaling<T, Dim>(this->scale.cwiseProduct(scaling.scale));
}


} // End namespace tl

