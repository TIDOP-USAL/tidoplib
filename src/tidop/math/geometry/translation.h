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
#include "tidop/math/algebra/decomp/svd.h"
#include "tidop/geometry/entities/point.h"

namespace tl
{

/*! \addtogroup GeometricTransformations
 *  \{
 */

/*!
 * \brief Translation Transformation.
 * 
 * The `Translation` class represents a translation transformation that is applied 
 * to points, vectors, and matrices. It encapsulates the translation vector, which 
 * can be of arbitrary dimension (`Dim`), and provides various methods for transforming 
 * points, vectors, and matrices using the translation transformation.
 * 
 * \tparam T The type of the translation components (e.g., float, double).
 * \tparam Dim The dimensionality of the translation (e.g., 2D or 3D).
 */
template <typename T, size_t Dim>
class Translation
{

public:

    /*!
     * \brief The dimensionality of the translation transformation.
     */
    enum
    {
        dimensions = Dim ///< Dimensionality of the translation (2D or 3D).
    };

    using value_type = T;
    using size_type = size_t;
    using reference = T&;
    using const_reference = const T&;

private:

    Vector<T, Dim> translation;

public:

    /*!
     * \brief Default constructor.
     */
    Translation();

    /*!
     * \brief Constructor that initializes translation for 2D (tx, ty).
     * \param[in] tx The translation in the x-direction.
     * \param[in] ty The translation in the y-direction.
     */
    Translation(T tx, T ty);

    /*!
     * \brief Constructor that initializes translation for 3D (tx, ty, tz).
     * \param[in] tx The translation in the x-direction.
     * \param[in] ty The translation in the y-direction.
     * \param[in] tz The translation in the z-direction.
     */
    Translation(T tx, T ty, T tz);

    /*!
     * \brief Constructor that initializes translation from a vector.
     * \param[in] vector The translation vector.
     */
    Translation(const Vector<T, Dim> &vector);

    /*!
     * \brief Copy constructor.
     * \param[in] translate The translation object to copy.
     */
    Translation(const Translation &translate);

    /*!
     * \brief Move constructor.
     * \param[in] translate The translation object to move.
     */
    Translation(Translation &&translate) TL_NOEXCEPT;

    /*!
     * \brief Destructor.
     */
    ~Translation() = default;

    /*!
     * \brief Copy assignment operator.
     * \param[in] translate The translation object to assign.
     * \return A reference to this translation object.
     */
    auto operator=(const Translation &translate) -> Translation&;

    /*!
     * \brief Move assignment operator.
     * \param[in] translate The translation object to move.
     * \return A reference to this translation object.
     */
    auto operator=(Translation &&translate) TL_NOEXCEPT -> Translation&;

    /*!
     * \brief Converts the translation to a vector.
     * \return The translation vector.
     */
    auto toVector() const TL_NOEXCEPT -> Vector<T, Dim>;

    /*!
     * \brief Access the x-component of the translation.
     * \return The x-component.
     */
    auto x() const TL_NOEXCEPT -> const_reference;

    /*!
     * \brief Access the x-component of the translation (non-const version).
     * \return A reference to the x-component.
     */
    auto x() TL_NOEXCEPT -> reference;

    /*!
     * \brief Access the y-component of the translation.
     * \return The y-component.
     */
    auto y() const TL_NOEXCEPT -> const_reference;

    /*!
     * \brief Access the y-component of the translation (non-const version).
     * \return A reference to the y-component.
     */
    auto y() TL_NOEXCEPT -> reference;

    /*!
     * \brief Access the z-component of the translation.
     * \return The z-component.
     */
    auto z() const TL_NOEXCEPT -> const_reference;

    /*!
     * \brief Access the z-component of the translation (non-const version).
     * \return A reference to the z-component.
     */
    auto z() TL_NOEXCEPT -> reference;

    /*!
     * \brief Access a specific component of the translation by position.
     * \param[in] position The index of the component to access.
     * \return A reference to the translation component at the specified position.
     */
    auto at(size_type position) -> reference;

    /*!
     * \brief Access a specific component of the translation by position (const version).
     * \param[in] position The index of the component to access.
     * \return A constant reference to the translation component at the specified position.
     */
    auto at(size_type position) const -> const_reference;

    /*!
     * \brief Access a specific component of the translation using the subscript operator.
     * \param[in] position The index of the component to access.
     * \return A reference to the translation component at the specified position.
     */
    auto operator[](size_t position) TL_NOEXCEPT -> reference;

    /*!
     * \brief Access a specific component of the translation using the subscript operator (const version).
     * \param[in] position The index of the component to access.
     * \return A constant reference to the translation component at the specified position.
     */
    auto operator[](size_t position) const TL_NOEXCEPT -> const_reference;

    /*!
     * \brief Compute the inverse of the translation.
     * \return A translation representing the inverse of the current translation.
     */
    auto inverse() const->Translation;

    /*!
     * \brief Transform a point using the translation.
     * \param[in] point The point to transform.
     * \return The transformed point.
     */
    auto transform(const Point<T> &point) const->Point<T>;

    /*!
     * \brief Transform a 3D point using the translation.
     * \param[in] point The point to transform.
     * \return The transformed point.
     */
    auto transform(const Point3<T> &point) const->Point3<T>;

    /*!
     * \brief Transform a vector using the translation.
     * \tparam _size The size of the vector.
     * \param[in] vector The vector to transform.
     * \return The transformed vector.
     */
    template<size_t _size>
    auto transform(const Vector<T, _size> &vector) const->Vector<T, Dim>;

    /*!
     * \brief Transform a matrix using the translation.
     * \tparam _row The number of rows in the matrix.
     * \tparam _col The number of columns in the matrix.
     * \param[in] matrix The matrix to transform.
     * \return The transformed matrix.
     */
    template<size_t _row, size_t _col>
    auto transform(const Matrix<T, _row, _col> &matrix) const->Matrix<T, _row, _col>;

    /*!
     * \brief Apply the translation to a point.
     * \param[in] point The point to apply the translation to.
     * \return The translated point.
     */
    auto operator * (const Point<T> &point) const->Point<T>;

    /*!
     * \brief Apply the translation to a 3D point.
     * \param[in] point The point to apply the translation to.
     * \return The translated 3D point.
     */
    auto operator * (const Point3<T> &point) const->Point3<T>;

    /*!
     * \brief Apply the translation to a vector.
     * \tparam _size The size of the vector.
     * \param[in] vector The vector to apply the translation to.
     * \return The translated vector.
     */
    template<size_t _size>
    auto operator * (const Vector<T, _size> &vector) const->Vector<T, _size>;

    /*!
     * \brief Apply the translation to a matrix.
     * \tparam _row The number of rows in the matrix.
     * \tparam _col The number of columns in the matrix.
     * \param[in] matrix The matrix to apply the translation to.
     * \return The translated matrix.
     */
    template<size_t _row, size_t _col>
    auto operator * (const Matrix<T, _row, _col> &matrix) const->Matrix<T, _row, _col>;

    /*!
     * \brief Apply the translation to another translation.
     * \param[in] translation The translation to apply.
     * \return The resulting translation.
     */
    auto operator * (const Translation<T, Dim> &translation) const->Translation<T, Dim>;
};

/*! \} */


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
    static auto estimate(const Matrix<T, rows, cols> &src,
                         const Matrix<T, rows, cols> &dst) -> Translation<T, Dim>;

    /*!
     * \brief Estimate the translation between two sets of points.
     * 
     * This method estimates the translation transformation between two sets of corresponding points.
     * \param[in] src The source set of points.
     * \param[in] dst The destination set of points.
     * \return The estimated translation transformation.
     */
    static auto estimate(const std::vector<Point<T>> &src,
                         const std::vector<Point<T>> &dst) -> Translation<T, Dim>;

};

/*! \} */


/* Translation implementation */

template<typename T, size_t Dim>
Translation<T, Dim>::Translation()
  : translation(Vector<T, Dim>::zero())
{
    static_assert(dimensions == 2 || dimensions == 3, "Only 2 or 3 dimensions allowed");
}

template<typename T, size_t Dim>
Translation<T, Dim>::Translation(T tx, T ty)
{
    static_assert(dimensions == 2, "Constructor for 2D Translation. Use the 3D Translation constructor: Translation(T tx, T ty, T tz).");

    this->translation[0] = tx;
    this->translation[1] = ty;
}

template<typename T, size_t Dim>
Translation<T, Dim>::Translation(T tx, T ty, T tz)
{
    static_assert(dimensions == 3, "Constructor for 3D Translation. Use the 2D Translation constructor: Translation(T tx, T ty).");

    this->translation[0] = tx;
    this->translation[1] = ty;
    this->translation[2] = tz;
}

template<typename T, size_t Dim>
Translation<T, Dim>::Translation(const Vector<T, Dim> &vector)
{
    this->translation = vector;
}

template<typename T, size_t Dim>
Translation<T, Dim>::Translation(const Translation &translate)
    : translation(translate.translation)
{
}

template<typename T, size_t Dim>
Translation<T, Dim>::Translation(Translation &&translate) TL_NOEXCEPT
    : translation(std::move(translate.translation))
{
}

template<typename T, size_t Dim>
auto Translation<T, Dim>::operator=(const Translation &translate) -> Translation &
{
    if (this != &translate) {
        this->translation = translate.translation;
    }

    return (*this);
}

template<typename T, size_t Dim>
auto Translation<T, Dim>::operator=(Translation &&translate) TL_NOEXCEPT -> Translation &
{
    if (this != &translate) {
        this->translation = std::move(translate.translation);
    }

    return (*this);
}

template<typename T, size_t Dim>
auto Translation<T, Dim>::toVector() const TL_NOEXCEPT -> Vector<T, Dim>
{
    return this->translation;
}

template<typename T, size_t Dim>
auto Translation<T, Dim>::x() const TL_NOEXCEPT -> const_reference
{
    return this->translation[0];
}

template<typename T, size_t Dim>
auto Translation<T, Dim>::x() TL_NOEXCEPT -> T&
{
    return this->translation[0];
}

template<typename T, size_t Dim>
auto Translation<T, Dim>::y() const TL_NOEXCEPT -> const_reference
{
    return this->translation[1];
}

template<typename T, size_t Dim>
auto Translation<T, Dim>::y() TL_NOEXCEPT -> reference
{
    return this->translation[1];
}

template<typename T, size_t Dim>
auto Translation<T, Dim>::z() const TL_NOEXCEPT -> const_reference
{
    static_assert(dimensions == 3, "Method not valid for 2D translations");
    return this->translation[2];
}

template<typename T, size_t Dim>
auto Translation<T, Dim>::z() TL_NOEXCEPT -> reference
{
    static_assert(dimensions == 3, "Method not valid for 2D translations");
    return this->translation[2];
}

template<typename T, size_t Dim>
auto Translation<T, Dim>::at(size_type position) -> reference
{
    return this->translation.at(position);
}

template<typename T, size_t Dim>
auto Translation<T, Dim>::at(size_type position) const -> const_reference
{
    return this->translation.at(position);
}

template<typename T, size_t Dim>
auto Translation<T, Dim>::operator[](size_t position) TL_NOEXCEPT -> reference
{
    return this->translation[position];
}

template<typename T, size_t Dim>
auto Translation<T, Dim>::operator[](size_t position) const TL_NOEXCEPT -> const_reference
{
    return this->translation[position];
}

template<typename T, size_t Dim>
auto Translation<T, Dim>::inverse() const -> Translation
{
    return Translation(-this->translation);
}

template<typename T, size_t Dim>
auto Translation<T, Dim>::transform(const Point<T> &point) const -> Point<T>
{
    static_assert(dimensions == 2, "Transformation not allowed for 2D points");

    return Point<T>(this->translation[0] + point.x,
                    this->translation[1] + point.y);
}

template<typename T, size_t Dim>
auto Translation<T, Dim>::transform(const Point3<T> &point) const -> Point3<T>
{
    static_assert(dimensions == 3, "Transformation not allowed for 3D points");

    return Point3<T>(this->translation[0] + point.x,
                     this->translation[1] + point.y,
                     this->translation[2] + point.z);
}

template<typename T, size_t Dim>
template<size_t _size>
auto Translation<T, Dim>::transform(const Vector<T, _size> &vector) const -> Vector<T, Dim>
{
    TL_ASSERT(dimensions == vector.size(), "Invalid Vector dimensions");

    return this->translation + vector;
}

template<typename T, size_t Dim>
template<size_t _row, size_t _col>
auto Translation<T, Dim>::transform(const Matrix<T, _row, _col> &matrix) const -> Matrix<T, _row, _col>
{
    TL_ASSERT(dimensions == matrix.cols(), "Invalid matrix dimensions");

    Matrix<T, _row, _col> aux = matrix;
    for (size_t r = 0; r < matrix.rows(); r++) {
        aux[r] += this->translation;
    }

    return aux;
}

template<typename T, size_t Dim>
auto Translation<T, Dim>::operator * (const Point<T> &point) const -> Point<T>
{
    return this->transform(point);
}

template<typename T, size_t Dim>
auto Translation<T, Dim>::operator * (const Point3<T> &point) const -> Point3<T>
{
    return this->transform(point);
}

template<typename T, size_t Dim>
template<size_t _size>
auto Translation<T, Dim>::operator * (const Vector<T, _size> &vector) const -> Vector<T, _size>
{
    return this->transform(vector);
}

template<typename T, size_t Dim>
template<size_t _row, size_t _col>
auto Translation<T, Dim>::operator * (const Matrix<T, _row, _col> &matrix) const -> Matrix<T, _row, _col>
{
    return this->transform(matrix);
}

template<typename T, size_t Dim>
auto Translation<T, Dim>::operator * (const Translation<T, Dim> &translation) const -> Translation<T, Dim>
{
    return Translation<T, Dim>( this->translation + translation.translation);
}



/* TranslationEstimator implementation */

template<typename T, size_t Dim>
template<size_t rows, size_t cols>
auto TranslationEstimator<T, Dim>::estimate(const Matrix<T, rows, cols> &src, 
                                            const Matrix<T, rows, cols> &dst) -> Translation<T, Dim>
{
    static_assert(dimensions == 2, "Scale estimator only for 2D Scaling");

    Translation<T, Dim> translation;

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

template<typename T, size_t Dim>
auto TranslationEstimator<T, Dim>::estimate(const std::vector<Point<T>> &src, 
                                            const std::vector<Point<T>> &dst) -> Translation<T, Dim>
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

} // End namespace tl

