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

/*! \file Point.h
 * \brief N-dimensional point primitive implementation.
 *
 * This file defines the Point class template, which serves as the fundamental
 * building block for all geometric entities. It supports dimensions 2, 3, and 4
 * and provides vector-like operations through VectorBase. It also supports 
 * meassurent values (M) for 2 and 3 dimensions.
 * * ### Classes
 * - \ref tl::Point : Main template class for N-dimensional points.
 * * ### Type Aliases
 * - \ref tl::Point2d, \ref tl::Point3d, \ref tl::Point4d, tl::Point2dm, \ref tl::Point3dm : Common double-precision aliases.
 * - \ref tl::Point2f, \ref tl::Point3f, \ref tl::Point4f, tl::Point2fm, \ref tl::Point3fm : Common float-precision aliases.
 * - \ref tl::Point2i, \ref tl::Point3i, \ref tl::Point4f, tl::Point2im, \ref tl::Point3im : Common integral-precision aliases.
 * * \see tl::VectorBase, tl::GeometryBase
 */

#pragma once

#include "tidop/core/base/type_conversions.h"
#include "tidop/core/base/Hash.h"
#include "tidop/core/base/Concepts.h"
#include "tidop/geometry/base/Dimension.h"
#include "tidop/geometry/base/Geometry.h"
#include "tidop/geometry/base/Traits.h"
#include "tidop/math/algebra/vector/Vector.h"
#include "tidop/math/algebra/rotations/quaternion.h"

#include <array>
#include <typeindex>

namespace tl
{

/*! \addtogroup Primitives
 *  \{
 */

/*!
 * \class Point
 * 
 * \brief N-dimensional point representing a location in space with optional measure (M).
 *
 * This class template represents a point whose coordinate structure is defined by a Tag.
 * It strictly separates spatial dimensions (X, Y, Z) from storage size, allowing
 * support for OGC-compliant types like XYM or XYZM.
 *
 * \tparam T Data type for the coordinates (e.g., float, double, int).
 * \tparam Tag Type tag defining the coordinate layout (e.g., xy_tag, xyz_tag, xym_tag, xyzm_tag).
 * \ingroup GeometricPrimitives
 */
template<typename T, typename Tag = xy_tag>
class Point
  : public Geometry<Point<T, Tag>>/*,
    public VectorBase<Point<T, Tag>>*/
{

    static_assert(Arithmetic<T>,
                  "Point requires an arithmetic type (integral or floating-point)");

public:

    using value_type = T;
    using reference = T &;
    using const_reference = const T &;
    using pointer = T *;
    using const_pointer = const T *;
    //using iterator = typename std::array<T, Tag::storage_size>::iterator;
    //using const_iterator = typename std::array<T, Tag::storage_size>::const_iterator;
    //using reverse_iterator = typename std::array<T, Tag::storage_size>::reverse_iterator;
    //using const_reverse_iterator = typename std::array<T, Tag::storage_size>::const_reverse_iterator;

private:

    static constexpr size_t storage_size = Tag::storage_size;
    static constexpr size_t spatial_dims = Tag::spatial_dims;

    //std::array<T, storage_size> mData;
    Vector<T, storage_size> mData;

public:

    /*!
     * \brief Default constructor. Initializes all components to zero.
     */
    constexpr Point();

    /*!
     * \brief Constructs a Point from individual coordinate arguments.
     * \tparam Args Pack of coordinate values
     * \param args Coordinate values to initialize the point
     * \pre sizeof...(Args) must equal storage_size
     * \pre All arguments must be convertible to T
     */
    template<typename... Args>
        requires (sizeof...(Args) == storage_size &&
        (std::convertible_to<Args, T> && ...))
     explicit constexpr Point(Args... args) noexcept
        : mData{static_cast<T>(args)...}
    {
    }

    /*!
     * \brief Copy constructor.
     * \param[in] point Point object to be copied.
     */
    constexpr Point(const Point &point) = default;

    /*!
     * \brief Move constructor.
     * \param[in] point Point object to be moved.
     */
    constexpr Point(Point &&point) noexcept = default;

    /*!
     * \brief Constructs a Point from an array.
     * \param[in] array Array containing two elements [x, y].
     */
    explicit constexpr Point(const std::array<T, storage_size> &a);

    /*!
     * \brief Constructs a Point from an initializer list.
     * \param[in] init Initializer list with coordinate values
     * \throws std::invalid_argument if init.size() != storage_size
     */
    explicit constexpr Point(std::initializer_list<T> init)
    {
        if (init.size() != storage_size) {
            throw std::invalid_argument("Initializer list size must match point storage size");
        }
        std::copy(init.begin(), init.end(), mData.begin());
    }

    template<VectorExpr Expr>
    Point(const Expr &expr) {
        //if constexpr (is_static_matrix_v<Expr>) {
        //    // Si la matriz/expresión era estática, fallamos en COMPILACIÓN si no cuadra
        //    static_assert(Expr::Rows == point_traits<Point>::spatial_dims,
        //        "El número de filas de la matriz no coincide con la dimensión del punto.");
        //} else {
        //    // Si la matriz era dinámica, fallamos en EJECUCIÓN si no cuadra
        //    assert(expr.rows() == point_traits<Point>::spatial_dims &&
        //        "Dimension mismatch at runtime when evaluating matrix into point.");
        //}

        //// 2. Evaluación de los datos
        //// Aquí extraes los datos evaluados de la expresión a x, y, z
        //for (size_t i = 0; i < point_traits<Point>::spatial_dims; ++i) {
        //    this->operator[](i) = expr(i, 0); // Asumiendo que expr se evalúa como vector columna
        //}
        this->mData = expr;
    }

    /*! \brief Destructor. */
    ~Point() = default;

    /*!
     * \brief Copy assignment operator.
     * \param[in] point Point object to be copied.
     * \return Reference to this Point.
     */
    constexpr auto operator = (const Point &point) -> Point & = default;

    /*!
     * \brief Move assignment operator.
     * \param[in] point Point object to be moved.
     * \return Reference to this Point.
     */
    constexpr auto operator = (Point &&point) noexcept -> Point & = default;

    /*!
     * \brief Access the x-component of the point.
     * \return A const reference to the x-component.
     * \note Only valid for points with size at least 1.
     */
    [[nodiscard]] 
    constexpr auto x() const noexcept -> const_reference;

    /*!
     * \brief Access the x-component of the point (non-const version).
     * \return A reference to the x-component.
     * \note Only valid for points with size at least 1.
     */
    [[nodiscard]] 
    constexpr auto x() noexcept -> reference;

    /*!
     * \brief Access the y-component of the point.
     * \return A const reference to the y-component.
     * \note Only valid for points with size at least 2.
     */
    [[nodiscard]]
    constexpr auto y() const noexcept -> const_reference;

    /*!
     * \brief Access the y-component of the point (non-const version).
     * \return A reference to the y-component.
     * \note Only valid for points with size at least 2.
     */
    [[nodiscard]]
    constexpr auto y() noexcept -> reference;

    /*!
     * \brief Access the z-component of the point.
     * \return A const reference to the z-component.
     * \note Only valid for points with size at least 3.
     */
    [[nodiscard]]
    constexpr auto z() const noexcept -> const_reference;

    /*!
     * \brief Access the z-component of the point (non-const version).
     * \return A reference to the z-component.
     * \note Only valid for points with size at least 3.
     */
    [[nodiscard]] 
    constexpr auto z() noexcept -> reference;

    /*!
     * \brief Access the measure (M) component of the point.
     * \return A const reference to the measure value.
     * \note This method is only available for points using a Tag that includes measures (e.g., xym_tag).
     */
    [[nodiscard]]
    constexpr auto m() const noexcept -> const_reference;

    /*!
     * \brief Access the measure (M) component of the point (non-const version).
     * \return A const reference to the measure value.
     * \note This method is only available for points using a Tag that includes measures (e.g., xym_tag).
     */
    [[nodiscard]]
    constexpr auto m() noexcept -> reference;

    /*!
     * \brief Access the w-component of the point.
     * \return A const reference to the w-component.
     * \note Only valid for points with size at least 4.
     */
    [[nodiscard]] 
    constexpr auto w() const noexcept -> const_reference;

    /*!
     * \brief Access the w-component of the vector (non-const version).
     * \return A reference to the w-component.
     * \note Only valid for vectors with size at least 4.
     */
    [[nodiscard]]
    constexpr auto w() noexcept -> reference;

    // Iterators
    //[[nodiscard]] constexpr auto begin() noexcept -> iterator { return mData.begin(); }
    //[[nodiscard]] constexpr auto begin() const noexcept -> const_iterator { return mData.begin(); }
    //[[nodiscard]] constexpr auto end() noexcept -> iterator { return mData.end(); }
    //[[nodiscard]] constexpr auto end() const noexcept -> const_iterator { return mData.end(); }
    //[[nodiscard]] constexpr auto cbegin() const noexcept -> const_iterator { return mData.cbegin(); }
    //[[nodiscard]] constexpr auto cend() const noexcept -> const_iterator { return mData.cend(); }
    //[[nodiscard]] constexpr auto rbegin() noexcept -> reverse_iterator { return mData.rbegin(); }
    //[[nodiscard]] constexpr auto rbegin() const noexcept -> const_reverse_iterator { return mData.rbegin(); }
    //[[nodiscard]] constexpr auto rend() noexcept -> reverse_iterator { return mData.rend(); }
    //[[nodiscard]] constexpr auto rend() const noexcept -> const_reverse_iterator { return mData.rend(); }

    [[nodiscard]] 
    constexpr auto operator[](std::size_t position) noexcept -> reference;

    [[nodiscard]] 
    constexpr auto operator[](std::size_t position) const noexcept -> const_reference;

    /*!
     * \brief Accesses the element at the specified position with bounds checking.
     *
     * \param[in] position The position of the element to access.
     * \return A reference to the element at the specified position.
     * \throws std::out_of_range if the position is out of bounds.
     */
    [[nodiscard]] 
    auto at(size_t position) -> reference;

    /*!
     * \brief Accesses the element at the specified position with bounds checking (const version).
     *
     * \param[in] position The position of the element to access.
     * \return A const reference to the element at the specified position.
     * \throws std::out_of_range if the position is out of bounds.
     */
    [[nodiscard]] 
    auto at(size_t position) const -> const_reference;

    /*!
     * \brief Returns the spatial dimension of the point.
     * Storage components like Measure (M) are not counted here.
     * \return Number of spatial dimensions.
     */
    [[nodiscard]] 
    constexpr auto size() const noexcept -> size_t { return spatial_dims; }

    [[nodiscard]] 
    constexpr auto storageSize() const noexcept -> size_t { return storage_size; }

    /*!
     * \brief Returns a pointer to the data array of the point.
     * \return A pointer to the data array.
     */
    [[nodiscard]] 
    constexpr auto data() noexcept -> pointer;

    /*!
     * \brief Returns a const pointer to the data array of the point.
     * \return A const pointer to the data array.
     */
    [[nodiscard]] 
    constexpr auto data() const noexcept -> const_pointer;

    /*!
     * \brief Returns a pointer to the data array of the point.
     * \return A pointer to the data array.
     */
    [[nodiscard]]
    constexpr auto vector() noexcept -> Vector<T, storage_size>&;

    /*!
     * \brief Returns a const pointer to the data array of the point.
     * \return A const pointer to the data array.
     */
    [[nodiscard]]
    constexpr auto vector() const noexcept -> const Vector<T, storage_size>&;

    template<typename U, typename Tag2>
    explicit operator Point<U, Tag2>() const;

    [[nodiscard]]
    auto isEmpty() const -> bool
    {
        return false;
    }

    auto operator-(const Point &other) const -> Vector<T, storage_size>
    {
        return this->mData - other.mData;
    }

    template<VectorExpr Vec>
    auto operator+(const Vec &vec) const -> Point
    {
        Point res;
        res.mData = this->mData + vec;
        return res;
    }

    template<VectorExpr Vec>
    auto operator-(const Vec &vec) const -> Point
    {
        Point res;
        res.mData = this->mData - vec;
        return res;
    }

    template<VectorExpr Vec>
    auto operator * (const Vec &vec) const -> Point
    {
        Point res;
        res.mData = this->mData.cwiseProduct(vec);
        return res;
    }

    template<VectorExpr Vec>
    auto operator / (const Vec &vec) const -> Point
    {
        Point res;
        res.mData = this->mData.cwiseDiv(vec);
        return res;
    }

    auto operator*(T scalar) const -> Point
    {
        Point res;
        res.mCoords = this->mCoords * scalar;
        return res;
    }

    auto operator/(T scalar) const -> Point
    {
        Point res;
        res.mCoords = this->mCoords / scalar;
        return res;
    }

    template<MatrixExpr Mat>
    auto operator*(const Mat &matrix) const -> Point
    {
        Point res;
        res.mData = this->mData * matrix;
        return res;
    }
};


// TYPE ALIASES

template<typename T> using Point2 = Point<T, xy_tag>;
template<typename T> using Point3 = Point<T, xyz_tag>;
template<typename T> using Point4 = Point<T, xyzw_tag>;

using Point2f = Point<float, xy_tag>;
using Point2d = Point<double, xy_tag>;
using Point2i = Point<int, xy_tag>;

using Point3f = Point<float, xyz_tag>;
using Point3d = Point<double, xyz_tag>;
using Point3i = Point<int, xyz_tag>;

using Point4f = Point<float, xyzw_tag>;
using Point4d = Point<double, xyzw_tag>;
using Point4i = Point<int, xyzw_tag>;

using Point2fm = Point<float, xym_tag>;
using Point2dm = Point<double, xym_tag>;
using Point2im = Point<int, xym_tag>;

using Point3fm = Point<float, xyzm_tag>;
using Point3dm = Point<double, xyzm_tag>;
using Point3im = Point<int, xyzm_tag>;



//template<typename T, size_t Dim>
//struct make_point
//{ 
//    static constexpr size_t dim = dimension_value(Dim);
//    using type = Point<T, tag_for_dim<dim>::type>;
//};


// IMPLEMENTATION

template<typename T, typename Tag>
constexpr Point<T, Tag>::Point()
{
    mData.fill(static_cast<T>(0));
}

template<typename T, typename Tag>
constexpr Point<T, Tag>::Point(const std::array<T, storage_size> &a)
{
    for (size_t i = 0; i < storage_size; ++i) {
        mData[i] = a[i];
    }
}

template<typename T, typename Tag>
constexpr auto Point<T, Tag>::x() const noexcept -> const_reference
{
    return mData[0];
}

template<typename T, typename Tag>
constexpr auto Point<T, Tag>::x() noexcept -> reference
{
    return mData[0];
}

template<typename T, typename Tag>
constexpr auto Point<T, Tag>::y() const noexcept -> const_reference
{
    return mData[1];
}

template<typename T, typename Tag>
constexpr auto Point<T, Tag>::y() noexcept -> reference
{
    return mData[1];
}

template<typename T, typename Tag>
constexpr auto Point<T, Tag>::z() const noexcept -> const_reference
{
    static_assert(spatial_dims >= 3, "Error: Access to Z coordinate at a point with less than 3 dimensions.");
    return mData[2];
}

template<typename T, typename Tag>
constexpr auto Point<T, Tag>::z() noexcept -> reference
{
    static_assert(spatial_dims >= 3, "Error: Access to Z coordinate at a point with less than 3 dimensions.");
    return mData[2];
}

template<typename T, typename Tag>
constexpr auto Point<T, Tag>::m() const noexcept -> const_reference
{
    static_assert(spatial_dims < storage_size, "Error: Point without measure");
    return mData[spatial_dims];
}

template<typename T, typename Tag>
constexpr auto Point<T, Tag>::m() noexcept -> reference
{
    static_assert(spatial_dims < storage_size, "Error: Point without measure");
    return mData[spatial_dims];
}

template<typename T, typename Tag>
constexpr auto Point<T, Tag>::w() const noexcept -> const_reference
{
    static_assert(spatial_dims == 4, "Error: Access to W coordinate at a point with less than 4 dimensions.");
    return mData[3];
}

template<typename T, typename Tag>
constexpr auto Point<T, Tag>::w() noexcept -> reference
{
    static_assert(spatial_dims == 4, "Error: Access to W coordinate at a point with less than 4 dimensions.");
    return mData[3];
}

template<typename T, typename Tag>
constexpr auto Point<T, Tag>::operator[](std::size_t position) noexcept -> reference
{
    return mData[position];
}

template<typename T, typename Tag>
constexpr auto Point<T, Tag>::operator[](std::size_t position) const noexcept -> const_reference
{
    return mData[position];
}

template<typename T, typename Tag>
auto Point<T, Tag>::at(size_t position) -> reference
{
    if (position >= spatial_dims)
        throw std::out_of_range("Point spatial index out of range");
    return mData.at(position);
}

template<typename T, typename Tag>
auto Point<T, Tag>::at(size_t position) const -> const_reference
{
    if (position >= spatial_dims)
        throw std::out_of_range("Point spatial index out of range");
    return mData.at(position);
}

template<typename T, typename Tag>
constexpr auto Point<T, Tag>::data() noexcept -> pointer
{
    return mData.data();
}

template<typename T, typename Tag>
constexpr auto Point<T, Tag>::data() const noexcept -> const_pointer
{
    return mData.data();
}

template<typename T, typename Tag>
constexpr auto Point<T, Tag>::vector() noexcept -> Vector<T, storage_size>&
{
    return mData;
}

template<typename T, typename Tag>
constexpr auto Point<T, Tag>::vector() const noexcept -> const Vector<T, storage_size>&
{
    return mData;
}

template<typename T, typename Tag>
template<typename U, typename Tag2>
Point<T, Tag>::operator Point<U, Tag2>() const
{
    Point<U, Tag2> result;
    constexpr size_t min_size = (spatial_dims < Tag2::spatial_dims) ? spatial_dims : Tag2::spatial_dims;
    for (size_t i = 0; i < min_size; ++i) {
        result[i] = numberCast<U>(mData[i]);
    }

    // Medida (M)
    if constexpr (Tag2::spatial_dims < Tag2::storage_size) {
        if constexpr (Tag::spatial_dims < Tag::storage_size)
            result.m() = numberCast<U>(this->m());
    }

    return result;
}


/*!
 * \brief Subtract two points to get the vector between them.
 *
 * \tparam T Coordinate type.
 * \tparam D Dimension.
 *
 * \param[in] a First point.
 * \param[in] b Second point.
 *
 * \return Vector from point b to point a.
 */
//template<typename T, typename Tag>
//auto operator-(const Point<T, Tag> &a, const Point<T, Tag> &b) -> typename VectorTraits<Point<T, Tag>>::difference_type
//{
//    typename VectorTraits<Point<T, Tag>>::difference_type v;
//    for (size_t i = 0; i < v.size(); ++i)
//        v[i] = a[i] - b[i];
//    return v;
//}

template<typename T, typename Tag>
auto operator*(T scalar, const Point<T, Tag> &p) -> Point<T, Tag>
{
    Point<T, Tag> res;
    res.mData = p.mData * scalar;
    return res;
}

template<VectorExpr Vec, PointConcept P>
auto operator+(const Vec &vec, const P &p) -> P
{
    return p + vec;
}

template<VectorExpr Vec, PointConcept P>
auto operator-(const Vec &vec, const P &p) -> P
{
    return p - vec;
}

template<VectorExpr Vec, PointConcept P>
auto operator * (const Vec &vec, const P &p) -> P
{
    return p * vec;
}

template<VectorExpr Vec, PointConcept P>
auto operator / (const Vec &vec, const P &p) -> P
{
    return p / vec;
}

//template<MatrixExpr Mat, PointConcept P>
//auto operator*(const Mat &mat, const P &p) -> P
//{
//    return p * mat;
//}
template<MatrixExpr Mat, PointConcept P>
auto operator*(const Mat &mat, const P &p)
{
    return mat * p.vector();
}

template<typename T, typename Tag>
auto operator == (const Point<T, Tag> &pt1, const Point<T, Tag> &pt2) -> bool
{
    static constexpr size_t size = point_traits<Point<T, Tag>>::storage_size;

    for (size_t i = 0; i < size; ++i)
        if (pt1[i] != pt2[i]) return false;

    return true;
}

template<typename T, typename Tag>
auto operator != (const Point<T, Tag> &pt1, const Point<T, Tag> &pt2) -> bool
{
    return !(pt1 == pt2);
}


/* Quaternion */

template<typename T, typename Tag>
auto operator*(const Quaternion<T> &q, const Point<T, Tag> &p) -> Point<T, Tag>
{
    static_assert(point_traits<Point<T, Tag>>::spatial_dims == 3,
        "Quaternion rotation is only defined for 3D entities.");

    Quaternion<T> q_norm = q;
    q_norm.normalize();

    // Rotación: q * v * q'
    auto q_rot = q_norm * Quaternion<T>(p[0], p[1], p[2], consts::zero<T>) * q_norm.conjugate();

    return Point<T, Tag>{q_rot.x, q_rot.y, q_rot.z};
}


template<typename Point_t>
struct Hash
{
    std::size_t operator()(const Point_t &p) const noexcept
    {
        constexpr std::size_t D = point_traits<Point_t>::spatial_dims;

        std::size_t seed = 0;

        for (std::size_t i = 0; i < D; ++i) {
            hash_combine(seed, p[i]);
        }

        return seed;
    }

private:

    template<typename V>
    static void hash_combine(std::size_t &seed, const V &v) noexcept
    {
        // Variante tipo boost::hash_combine
        seed ^= std::hash<V>{}(v)+0x9e3779b97f4a7c15ULL + (seed << 6) + (seed >> 2);
    }
};



/*! \} */

} // End namespace tl


//namespace std
//{
//template<>
//struct hash<tl::Point2d>
//{
//    size_t operator()(const tl::Point2d &p) const noexcept
//    {
//        return tl::Hash<tl::Point2d>{}(p);
//    }
//};
//
//}