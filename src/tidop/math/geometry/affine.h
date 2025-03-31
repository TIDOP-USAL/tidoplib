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

#include "tidop/math/geometry/transform.h"
#include "tidop/math/geometry/translation.h"
#include "tidop/math/geometry/scaling.h"
#include "tidop/math/geometry/rotation.h"

namespace tl
{

/*! \addtogroup GeometricTransformations
 *  \{
 */

/*!
 * \brief Affine transformation.
 *
 * The Affine Transformation expresses the relationship between two
 * Cartesian coordinate systems that may differ in the origin location,
 * axis orientation, and unit measurement along the axes.
 *
 * In 2D, the transformation is represented by:
 * \f[
 * a =  \text{scaleX} \cdot \cos(\text{rotation})
 * \f]
 * \f[
 * b = -\text{scaleY} \cdot \sin(\text{rotation})
 * \f]
 * \f[
 * c =  \text{scaleX} \cdot \sin(\text{rotation})
 * \f]
 * \f[
 * d =  \text{scaleY} \cdot \cos(\text{rotation})
 * \f]
 *
 * The transformation equations are:
 * \f[
 * x' = a \cdot x + b \cdot y + x_0
 * \f]
 * \f[
 * y' = c \cdot x + d \cdot y + y_0
 * \f]
 *
 * \tparam T The type of the elements (e.g., float, double).
 * \tparam Dim The number of dimensions (e.g., 2 for 2D, 3 for 3D).
 */
template <typename T, size_t Dim>
class Affine
{

public:

    /*!
     * \brief Number of dimensions.
     */
    enum
    {
        dimensions = Dim
    };

    using value_type = T;
    using size_type = size_t;
    using reference = T &;
    using const_reference = const T &;

private:

    // Utilizar al menos doble para no perder precision aunque los datos de entrada sean float.
    Matrix<T, Dim, Dim + 1> _transform;

public:

    /*!
     * \brief Default constructor.
     */
    Affine();

    /*!
     * \brief Copy constructor.
     * \param[in] affine The Affine object to copy.
     */
    Affine(const Affine &affine);

    /*!
     * \brief Move constructor.
     * \param[in] affine The Affine object to move.
     */
    Affine(Affine &&affine) TL_NOEXCEPT;

    /*!
     * \brief Constructor from a matrix.
     * \param[in] matrix The transformation matrix.
     */
    explicit Affine(const Matrix<T, Dim, Dim + 1> &matrix);

    /*!
     * \brief Constructor for 2D affine transformation.
     * \param[in] sx Scale in the x-direction.
     * \param[in] sy Scale in the y-direction.
     * \param[in] tx Translation in the x-direction.
     * \param[in] ty Translation in the y-direction.
     * \param[in] angle Rotation angle in radians.
     */
    Affine(T sx, T sy, T tx, T ty, T angle);

    /*!
     * \brief Constructor for 3D affine transformation.
     * \param[in] sx Scale in the x-direction.
     * \param[in] sy Scale in the y-direction.
     * \param[in] sz Scale in the z-direction.
     * \param[in] tx Translation in the x-direction.
     * \param[in] ty Translation in the y-direction.
     * \param[in] tz Translation in the z-direction.
     * \param[in] omega Rotation around the x-axis.
     * \param[in] phi Rotation around the y-axis.
     * \param[in] kappa Rotation around the z-axis.
     */
    Affine(T sx, T sy, T sz, T tx, T ty, T tz, T omega, T phi, T kappa);

    /*!
     * \brief Constructor from scale, translation, and rotation for 2D.
     * \param[in] scale The scaling vector.
     * \param[in] translation The translation vector.
     * \param[in] angle Rotation angle in radians.
     */
    Affine(const Vector<T, 2> &scale, const Vector<T, 2> &translation, T angle);

    /*!
     * \brief Constructor from scale, translation, and Euler angles for 3D.
     * \param[in] scale The scaling vector.
     * \param[in] translation The translation vector.
     * \param[in] rotation Euler angles for rotation.
     */
    Affine(const Vector<T, 3> &scale, const Vector<T, 3> &translation, const EulerAngles<T> &rotation);

    /*!
     * \brief Constructor from scale, translation, and rotation matrix for 3D.
     * \param[in] scale The scaling vector.
     * \param[in] translation The translation vector.
     * \param[in] rotation The rotation matrix.
     */
    Affine(const Vector<T, 3> &scale, const Vector<T, 3> &translation, const RotationMatrix<double> &rotation);

    /*!
     * \brief Constructor from separate transformations.
     * \param[in] scale The scaling transformation.
     * \param[in] translation The translation transformation.
     * \param[in] rotation The rotation transformation.
     */
    Affine(const Scaling<T, Dim> &scale, const Translation<T, Dim> &translation, const Rotation<T, Dim> &rotation);

    ~Affine() = default;

    /*!
     * \brief Assignment operator.
     * \param[in] affine The Affine object to copy.
     * \return Reference to this object.
     */
    auto operator=(const Affine &affine) -> Affine&;

    /*!
     * \brief Move assignment operator.
     * \param[in] affine The Affine object to move.
     * \return Reference to this object.
     */
    auto operator=(Affine &&affine) TL_NOEXCEPT -> Affine &;

    /*!
     * \brief Access element at row r, column c.
     * \param[in] r Row index.
     * \param[in] c Column index.
     * \return Reference to the element.
     */
    auto operator()(size_t r, size_t c) TL_NOEXCEPT -> reference;

    /*!
     * \brief Access element at row r, column c (const version).
     * \param[in] r Row index.
     * \param[in] c Column index.
     * \return Const reference to the element.
     */
    auto operator()(size_t r, size_t c) const TL_NOEXCEPT -> const_reference;

    /*!
     * \brief Get the scaling transformation.
     * \return The scaling transformation.
     */
    auto scale() const -> Scaling<T, Dim>;

    /*!
     * \brief Get the translation transformation.
     * \return The translation transformation.
     */
    auto translation() const -> Translation<T, Dim>;

    /*!
     * \brief Get the rotation transformation.
     * \return The rotation transformation.
     */
    auto rotation() const -> Rotation<T, Dim>;

    /*!
     * \brief Compute the inverse of the affine transformation.
     * \return The inverse transformation.
     */
    auto inverse() const -> Affine<T, Dim>;

    /*!
     * \brief Convert the affine transformation to a matrix.
     * \return The transformation matrix.
     */
    auto toMatrix() const TL_NOEXCEPT -> Matrix<T, Dim, Dim + 1>;

    /*!
     * \brief Apply the transformation to a point.
     * \param[in] point The point to transform.
     * \return The transformed point.
     */
    auto transform(const Point<T> &point) const -> Point<T>;

    /*!
     * \brief Apply the transformation to a 3D point.
     * \param[in] point The point to transform.
     * \return The transformed point.
     */
    auto transform(const Point3<T> &point) const -> Point3<T>;

    /*!
     * \brief Apply the transformation to a vector.
     * \tparam _size The size of the vector.
     * \param[in] vector The vector to transform.
     * \return The transformed vector.
     */
    template<size_t _size>
    auto transform(const Vector<T, _size> &vector) const -> Vector<T, _size>;

    /*!
      * \brief Apply the transformation to a matrix.
      * \tparam _row The number of rows in the matrix.
      * \tparam _col The number of columns in the matrix.
      * \param[in] matrix The matrix to transform.
      * \return The transformed matrix.
      */
    template<size_t _row, size_t _col>
    auto transform(const Matrix<T, _row, _col> &matrix) const -> Matrix<T, _row, _col>;

    /*!
     * \brief Apply the transformation using the multiplication operator.
     * \param[in] point The point to transform.
     * \return The transformed point.
     */
    auto operator * (const Point<T> &point) const -> Point<T>;

    /*!
     * \brief Apply the transformation using the multiplication operator to a 3D point.
     * \param[in] point The point to transform.
     * \return The transformed point.
     */
    auto operator * (const Point3<T> &point) const -> Point3<T>;
    
    /*!
     * \brief Apply the transformation using the multiplication operator to a vector.
     * \tparam _size The size of the vector.
     * \param[in] vector The vector to transform.
     * \return The transformed vector.
     */
    template<size_t _size>
    auto operator * (const Vector<T, _size> &vector) const-> Vector<T, _size>;

    /*!
     * \brief Apply the transformation using the multiplication operator to a matrix.
     * \tparam _row The number of rows in the matrix.
     * \tparam _col The number of columns in the matrix.
     * \param[in] matrix The matrix to transform.
     * \return The transformed matrix.
     */
    template<size_t _row, size_t _col>
    auto operator * (const Matrix<T, _row, _col> &matrix) const-> Matrix<T, _row, _col>;

    /*!
     * \brief Apply the transformation to a point using function call syntax.
     * \param[in] point The point to transform.
     * \return The transformed point.
     */
    auto operator()(const Point<T> &point) const -> Point<T>;

    /*!
     * \brief Apply the transformation to a 3D point using function call syntax.
     * \param[in] point The point to transform.
     * \return The transformed point.
     */
    auto operator()(const Point3<T> &point) const -> Point3<T>;

    /*!
     * \brief Check if the affine transformation is empty (i.e., identity).
     * \return `true` if the transformation is empty, otherwise `false`.
     */
    auto isEmpty() const TL_NOEXCEPT -> bool;

};

/*! \} */


/*! \addtogroup Estimators
 *  \{
 */

/*!
 * \brief Estimator for 2D affine transformations.
 *
 * The `Affine2DEstimator` class provides static methods to estimate a 2D affine
 * transformation between two sets of points or matrices representing the source
 * and destination coordinates.
 *
 * \tparam T The type of the elements (e.g., float, double).
 */
template <typename T>
class Affine2DEstimator
{

public:

    Affine2DEstimator() = default;
    ~Affine2DEstimator() = default;

    /*!
     * \brief Estimate a 2D affine transformation between two matrices.
     *
     * This method estimates the affine transformation that maps points
     * from the source matrix \p src to the destination matrix \p dst.
     *
     * \tparam Rows Number of rows in the matrices.
     * \tparam Cols Number of columns in the matrices.
     * \param[in] src The source matrix of points.
     * \param[in] dst The destination matrix of points.
     * \return The estimated 2D affine transformation.
     */
    template<size_t Rows, size_t Cols>
    static auto estimate(const Matrix<T, Rows, Cols> &src,
                         const Matrix<T, Rows, Cols> &dst) -> Affine<T, 2>;

    /*!
     * \brief Estimate a 2D affine transformation between two sets of points.
     *
     * This method estimates the affine transformation that maps points
     * from the source vector \p src to the destination vector \p dst.
     *
     * \param[in] src The source vector of points.
     * \param[in] dst The destination vector of points.
     * \return The estimated 2D affine transformation.
     */
    static auto estimate(const std::vector<Point<T>> &src,
                         const std::vector<Point<T>> &dst) -> Affine<T, 2>;

};

/*! \} */


/* Affine implementation */

template <typename T, size_t Dim>
Affine<T, Dim>::Affine()
  : _transform(Matrix<T, Dim, Dim + 1>::identity())
{
    static_assert(dimensions == 2 || dimensions == 3, "Only 2 or 3 dimensions allowed");
}

template<typename T, size_t Dim>
Affine<T, Dim>::Affine(const Affine &affine)
    : _transform(affine._transform)
{
}

template<typename T, size_t Dim>
Affine<T, Dim>::Affine(Affine &&affine) TL_NOEXCEPT
    : _transform(std::move(affine._transform))
{
}

//template <typename T, size_t dim> inline
//Affine<T, dim>::Affine(const Transform<T, dim> &matrix)
//  : _transform(matrix)
//{
//    static_assert(dim == 2 || dim == 3, "Only 2 or 3 dimensions allowed");
//}

template <typename T, size_t Dim>
Affine<T, Dim>::Affine(const Matrix<T, Dim, Dim + 1> &matrix)
  : _transform(matrix)
{
    static_assert(dimensions == 2 || dimensions == 3, "Only 2 or 3 dimensions allowed");
}

template<typename T, size_t Dim>
Affine<T, Dim>::Affine(T sx, T sy, T tx, T ty, T angle)
{
    static_assert(dimensions == 2, "Constructor for 2D Affine. Use the 3D Affine constructor: Affine(T sx, T sy, T sz, T tx, T ty, T tz, T omega, T phi, T kappa).");

    this->_transform[0][0] = sx * static_cast<T>(cos(angle));
    this->_transform[0][1] = -sy * static_cast<T>(sin(angle));
    this->_transform[0][2] = tx;
    this->_transform[1][0] = sx * static_cast<T>(sin(angle));
    this->_transform[1][1] = sy * static_cast<T>(cos(angle));
    this->_transform[1][2] = ty;
}

template<typename T, size_t Dim>
Affine<T, Dim>::Affine(T sx, T sy, T sz, T tx, T ty, T tz, T omega, T phi, T kappa)
{
    static_assert(dimensions == 3, "Constructor for 3D Affine. Use the 2D Affine constructor: Affine(T sx, T sy, T tx, T ty, T angle).");

    EulerAngles<T> eulerAngles(omega, phi, kappa);
    RotationMatrix<T> rt = eulerAngles;
    this->_transform.block(0, dimensions - 1, 0, dimensions - 1) = rt;
    this->_transform[0][0] *= sx;
    this->_transform[0][1] *= sy;
    this->_transform[0][2] *= sz;
    this->_transform[1][0] *= sx ;
    this->_transform[1][1] *= sy;
    this->_transform[1][2] *= sz;
    this->_transform[2][0] *= sx;
    this->_transform[2][1] *= sy;
    this->_transform[2][2] *= sz;
    this->_transform[0][3] = tx;
    this->_transform[1][3] = ty;
    this->_transform[2][3] = tz;
}

template<typename T, size_t Dim>
Affine<T, Dim>::Affine(const Vector<T, 2> &scale, 
                       const Vector<T, 2> &translation, 
                       T angle)
{
    static_assert(dimensions == 2, "Constructor for 2D Affine. Use the 3D Affine constructor: Affine(T sx, T sy, T sz, T tx, T ty, T tz, T omega, T phi, T kappa).");

    this->_transform[0][0] = scale[0] * cos(angle);
    this->_transform[0][1] = -scale[1] * sin(angle);
    this->_transform[0][2] = translation[0];
    this->_transform[1][0] = scale[0] * sin(angle);
    this->_transform[1][1] = scale[1] * cos(angle);
    this->_transform[1][2] = translation[1];
}

template<typename T, size_t Dim>
Affine<T, Dim>::Affine(const Vector<T, 3> &scale,
                       const Vector<T, 3> &translation, 
                       const EulerAngles<T> &rotation)
{
    static_assert(dimensions == 3, "Constructor for 3D Affine. Use the 2D Affine constructor: Affine(T sx, T sy, T tx, T ty, T angle).");

    Rotation<T, dimensions> rot(rotation);
    this->_transform.block(0, dimensions - 1, 0, dimensions - 1) = rot.toMatrix();
    
    for (size_t i = 0; i < dimensions; i++)
        this->_transform.col(i) *= scale[i];

    this->_transform.col(dimensions) = translation;
}

template<typename T, size_t Dim>
Affine<T, Dim>::Affine(const Vector<T, 3> &scale, 
                       const Vector<T, 3> &translation,
                       const RotationMatrix<double> &rotation)
{
    static_assert(dimensions == 3, "Constructor for 3D Affine. Use the 2D Affine constructor: Affine(T sx, T sy, T tx, T ty, T angle).");

    this->_transform.block(0, dimensions - 1, 0, dimensions - 1) = rotation;

    for (size_t i = 0; i < dimensions; i++)
        this->_transform.col(i) *= scale[i];

    this->_transform.col(dimensions) = translation;
}

template<typename T, size_t Dim>
Affine<T, Dim>::Affine(const Scaling<T, Dim> &scale, 
                       const Translation<T, Dim> &translation, 
                       const Rotation<T, Dim> &rotation)
{
    this->_transform.block(0, dimensions - 1, 0, dimensions - 1) = rotation.toMatrix();

    for (size_t i = 0; i < dimensions; i++)
        this->_transform.col(i) *= scale[i];

    this->_transform.col(dimensions) = translation.toVector();
}

template<typename T, size_t Dim>
auto Affine<T, Dim>::operator=(const Affine &affine) -> Affine&
{
    if (this != &affine) {
        this->_transform = affine._transform;
    }

    return (*this);
}

template<typename T, size_t Dim>
auto Affine<T, Dim>::operator()(size_t r, size_t c) TL_NOEXCEPT -> reference
{
    return this->_transform(r, c);
}

template<typename T, size_t Dim>
auto Affine<T, Dim>::operator()(size_t r, size_t c) const TL_NOEXCEPT -> const_reference
{
    return this->_transform(r, c);
}

template<typename T, size_t Dim>
auto Affine<T, Dim>::scale() const -> Scaling<T, Dim>
{
    Scaling<T, dimensions> scale;

    for (size_t i = 0; i < dimensions; i++)
        scale[i] = this->_transform.col(i).module();

    return scale;
}

template<typename T, size_t Dim>
auto Affine<T, Dim>::translation() const -> Translation<T, Dim>
{
    Translation<T, dimensions> translation;

    for (size_t i = 0; i < dimensions; i++)
        translation[i] = this->_transform(i, dimensions);

    return translation;
}

template<typename T, size_t Dim>
auto Affine<T, Dim>::rotation() const -> Rotation<T, Dim>
{
    Matrix<T, Dim, Dim> _rotation;
    auto _scale = scale().toVector();
    for (size_t c = 0; c < dimensions; c++) {
        T s = _scale[c];
        for (size_t r = 0; r < dimensions; r++)
            _rotation[r][c] = this->_transform(r, c) / s;
    }
    return Rotation<T, Dim>(_rotation);
}

template<typename T, size_t Dim>
auto Affine<T, Dim>::inverse() const -> Affine<T, Dim>
{
    auto _rotation = this->rotation();
    auto _scale = this->scale();
    auto rotation_inverse = _rotation.toMatrix().inverse();
    Matrix<T, Dim, Dim + 1> transform_inverse = Matrix<T, Dim, Dim + 1>::zero();
    for (size_t r = 0; r < rotation_inverse.rows(); r++){
        for (size_t c = 0; c < rotation_inverse.rows(); c++){
            transform_inverse(r, c) = rotation_inverse(r, c) / _scale[r];
        }
    }

    for (size_t r = 0; r < _transform.rows(); r++){
        for (size_t c = 0; c < _transform.rows(); c++){
            transform_inverse(r, dimensions) -= transform_inverse(r, c) * _transform(c, dimensions);
        }
    }

    //Matrix<T, Dim, Dim + 1> transform_inverse;
    //transform_inverse.block(0, Dim - 1, 0, Dim - 1) = matrix.inverse();
    /*transform_inverse.col(Dim) = _transform.col(Dim);*/
    return Affine<T, Dim>(transform_inverse);
}

template<typename T, size_t Dim>
auto Affine<T, Dim>::toMatrix() const TL_NOEXCEPT -> Matrix<T, Dim, Dim + 1>
{
    return this->_transform;
}


template<typename T, size_t Dim>
auto Affine<T, Dim>::operator=(Affine &&affine) TL_NOEXCEPT -> Affine &
{
    if (this != &affine) {
        this->_transform = std::move(affine._transform);
    }

    return (*this);
}

template<typename T, size_t Dim>
auto Affine<T, Dim>::transform(const Point<T> &point) const -> Point<T>
{
    static_assert(dimensions == 2, "Transformation not allowed for 2D points");

    return Point<T>(this->_transform(0, 0) * point.x + this->_transform(0, 1) * point.y + this->_transform(0, 2),
                    this->_transform(1, 0) * point.x + this->_transform(1, 1) * point.y + this->_transform(1, 2));
}

template<typename T, size_t Dim>
auto Affine<T, Dim>::transform(const Point3<T> &point) const -> Point3<T>
{
    static_assert(dimensions == 3, "Transformation not allowed for 3D points");

    return Point3<T>(this->_transform(0, 0) * point.x + this->_transform(0, 1) * point.y + this->_transform(0, 2) * point.z + this->_transform(0, 3),
                     this->_transform(1, 0) * point.x + this->_transform(1, 1) * point.y + this->_transform(1, 2) * point.z + this->_transform(1, 3),
                     this->_transform(2, 0) * point.x + this->_transform(2, 1) * point.y + this->_transform(2, 2) * point.z + this->_transform(2, 3));
}

template<typename T, size_t Dim>
template<size_t _size>
auto Affine<T, Dim>::transform(const Vector<T, _size> &vector) const -> Vector<T, _size>
{
    TL_ASSERT(dimensions == vector.size(), "Invalid Vector dimensions");
    
    Vector<T, _size> _vector(vector);
    for (size_t r = 0; r < dimensions; r++) {
        _vector[r] = 0;// _transform(r, dimensions);
        for (size_t c = 0; c < dimensions; c++) {
            _vector[r] += _transform(r, c) * vector[c];
        }
    }

    //Vector<T> _vector = this->_transform.block(0, dimensions - 1, 0, dimensions - 1) * vector;
    _vector += this->_transform.col(dimensions);

    return _vector;
}

template<typename T, size_t Dim>
template<size_t _row, size_t _col>
auto Affine<T, Dim>::transform(const Matrix<T, _row, _col> &matrix) const -> Matrix<T, _row, _col>
{
    TL_ASSERT(dimensions == matrix.cols(), "Invalid matrix dimensions");
        
    Matrix<T, _row, _col> _matrix(matrix);

    Vector<T> v(dimensions);
    for (size_t r = 0; r < _matrix.rows(); r++) {
        v = _matrix[r];
        _matrix[r] = *this * v;
    }

    return _matrix;
}

template<typename T, size_t Dim>
auto Affine<T, Dim>::operator*(const Point<T> &point) const -> Point<T>
{
    return this->transform(point);
}

template<typename T, size_t Dim>
auto Affine<T, Dim>::operator*(const Point3<T> &point) const -> Point3<T>
{
    return this->transform(point);
}

template<typename T, size_t Dim>
template<size_t _size>
auto Affine<T, Dim>::operator*(const Vector<T, _size>& vector) const -> Vector<T, _size>
{
    return this->transform(vector);
}

template<typename T, size_t Dim>
template<size_t _row, size_t _col>
auto Affine<T, Dim>::operator*(const Matrix<T, _row, _col>& matrix) const -> Matrix<T, _row, _col>
{
    return this->transform(matrix);
}

template<typename T, size_t Dim>
auto Affine<T, Dim>::operator()(const Point<T> &point) const -> Point<T>
{
    return this->transform(point);
}

template<typename T, size_t Dim>
auto Affine<T, Dim>::operator()(const Point3<T> &point) const -> Point3<T>
{
    return this->transform(point);
}

template<typename T, size_t Dim>
auto Affine<T, Dim>::isEmpty() const TL_NOEXCEPT -> bool
{
    return this->_transform == Matrix<T, Dim, Dim + 1>::identity();
}


/* Affine2DEstimator implementation */

template<typename T>
template<size_t rows, size_t cols>
auto Affine2DEstimator<T>::estimate(const Matrix<T, rows, cols> &src, 
                                    const Matrix<T, rows, cols> &dst) -> Affine<T, 2>
{

    Affine<T, 2> affine;

    try {

        TL_ASSERT(src.cols() == 2, "Invalid matrix columns size");
        TL_ASSERT(dst.cols() == 2, "Invalid matrix columns size");
        TL_ASSERT(src.rows() == dst.rows(), "Different matrix sizes. Size src = {} and size dst = {}", src.rows(), dst.rows());
        TL_ASSERT(src.rows() >= 3 , "Invalid number of points: {} < {}", src.rows(), 3);

        size_t size = src.rows() * 2;

        Matrix<double> A(size, 6, 0);
        Vector<double> B(size);

        for (size_t i = 0, r = 0; i < src.rows(); i++, r++) {
            A(r, 0) = src(i, 0);
            A(r, 1) = src(i, 1);
            A(r, 4) = 1;
            B[r] = dst(i,0);

            r++;
            
            A(r, 2) = src(i, 0);
            A(r, 3) = src(i, 1);
            A(r, 5) = 1;

            B[r] = dst(i,1);
        }

        SingularValueDecomposition<Matrix<double>> svd(A);
        Vector<double> C = svd.solve(B);

        affine(0, 0) = C[0];
        affine(0, 0) = C[0];
        affine(0, 1) = C[1];
        affine(0, 2) = C[4];
        affine(1, 0) = C[2];
        affine(1, 1) = C[3];
        affine(1, 2) = C[5];

    } catch (...) {
        TL_THROW_EXCEPTION_WITH_NESTED("");
    }

    return affine;
}

template<typename T>
auto Affine2DEstimator<T>::estimate(const std::vector<Point<T>> &src, 
                                    const std::vector<Point<T>> &dst) -> Affine<T, 2>
{
    TL_ASSERT(src.size() == dst.size(), "Size of origin and destination points different");

    Matrix<T> src_mat(src.size(), 2);
    Matrix<T> dst_mat(dst.size(), 2);

    for (size_t r = 0; r < src_mat.rows(); r++) {
        src_mat[r][0] = src[r].x;
        src_mat[r][1] = src[r].y;

        dst_mat[r][0] = dst[r].x;
        dst_mat[r][1] = dst[r].y;
    }

    return Affine2DEstimator<T>::estimate(src_mat, dst_mat);
}


} // End namespace tl

