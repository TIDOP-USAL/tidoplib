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

#include <algorithm>

#include "tidop/geometry/entities/entity.h"
#include "tidop/geometry/entities/point.h"

namespace tl
{

/*! \addtogroup GeometricEntities
 *  \{
 */


/*!
 * \brief Bounding Box (BBox) class.
 * \tparam Point3_t Type representing a 3D point.
 *
 * The `BoundingBox` class represents a 3D bounding box defined by two corner points.
 * It provides utility functions for checking containment, dimensions, and validity.
 */
template<typename Point3_t>
class BoundingBox final
  : public Entity
{

public:

    using value_type = Point3_t;
    using scalar = typename Point3_t::value_type;

public:

    /*!
     * \brief First corner point of the bounding box.
     */
    Point3_t pt1;

    /*!
     * \brief Second corner point of the bounding box.
     */
    Point3_t pt2;

public:

    /*!
     * \brief Default constructor. Creates an empty bounding box.
     */
    BoundingBox();

    /*!
     * \brief Copy constructor.
     * \param[in] bbox BoundingBox object to copy.
     */
    BoundingBox(const BoundingBox &bbox);

    /*!
     * \brief Move constructor.
     * \param[in] bbox BoundingBox object to move.
     */
    BoundingBox(BoundingBox &&bbox) TL_NOEXCEPT;

    /*!
     * \brief Constructor that defines the bounding box using two corner points.
     * \param[in] pt1 First corner point.
     * \param[in] pt2 Second corner point.
     */
    BoundingBox(const Point3_t &pt1,
        const Point3_t &pt2);

    /*!
     * \brief Constructor that defines the bounding box using a central point and dimensions.
     * \param[in] pt Center of the bounding box.
     * \param[in] width Width of the bounding box.
     * \param[in] depth Depth of the bounding box.
     * \param[in] height Height of the bounding box.
     */
    template<typename T>
    BoundingBox(const Point3_t &pt,
        T width, T depth, T height);

    /*!
     * \brief Constructor that defines a cubic bounding box using a central point and side length.
     * \param[in] pt Center of the bounding box.
     * \param[in] side Length of each side (width = depth = height).
     */
    template<typename T>
    BoundingBox(const Point3_t &pt, T side);

    /*!
     * \brief Constructor that creates a bounding box from a set of 3D points.
     * \param[in] vertices Vector of points used to compute the bounding box.
     */
    explicit BoundingBox(const std::vector<Point3_t> &vertices);

    /*!
     * \brief Constructor that creates a bounding box from a set of 3D points of another type.
     * \param[in] vertices Vector of points of a different type used to compute the bounding box.
     */
    template<typename Point3_t2> BoundingBox(const std::vector<Point3_t2> &vertices);

    /*!
     * \brief Destructor.
     */
    ~BoundingBox() override = default;

    /*!
     * \brief Copy assignment operator.
     * \param[in] bbox BoundingBox object to copy.
     * \return Reference to this BoundingBox.
     */
    auto operator = (const BoundingBox &bbox)->BoundingBox &;

    /*!
     * \brief Move assignment operator.
     * \param[in] bbox BoundingBox object to move.
     * \return Reference to this BoundingBox.
     */
    auto operator = (BoundingBox &&bbox) TL_NOEXCEPT->BoundingBox &;

    /*!
     * \brief Equality operator.
     * \param[in] bbox BoundingBox to compare.
     * \return true if both bounding boxes are equal, false otherwise.
     */
    auto operator == (const BoundingBox &bbox) const -> bool;

    /*!
     * \brief Type conversion operator.
     * \tparam Point3_t2 Target point type.
     * \return BoundingBox of the new point type.
     */
    template<typename Point3_t2> operator BoundingBox<Point3_t2>() const;

    /*!
     * \brief Computes the center point of the bounding box.
     * \return The center point.
     */
    auto center() const -> Point3_t;

    /*!
     * \brief Retrieves the vertices of the bounding box.
     * \return A vector containing the bounding box's corner points.
     */
    auto vertices() const -> std::vector<Point3_t>;

    /*!
     * \brief Retrieves the width of the bounding box.
     * \return The width.
     */
    auto width() const -> scalar;

    /*!
     * \brief Retrieves the height of the bounding box.
     * \return The height.
     */
    auto height() const -> scalar;

    /*!
     * \brief Retrieves the depth of the bounding box.
     * \return The depth.
     */
    auto depth() const -> scalar;

    /*!
     * \brief Checks if the bounding box is empty (i.e., has no volume).
     * \return true if empty, false otherwise.
     */
    auto isEmpty() const -> bool;

    /*!
     * \brief Checks if the bounding box is valid (i.e., pt1 and pt2 define a proper volume).
     * \return true if valid, false otherwise.
     */
    auto isValid() const -> bool;

    /*!
     * \brief Normalizes the bounding box to ensure correct ordering of points.
     */
    void normalized();

    /*!
     * \brief Checks if a point is contained within the bounding box.
     * \param[in] pt Point to check.
     * \return true if the point is inside the bounding box, false otherwise.
     */
    auto containsPoint(const Point3_t &pt) const -> bool;

    /*!
     * \brief Checks if another bounding box is fully contained within this bounding box.
     * \param[in] bbox Bounding box to check.
     * \return true if bbox is entirely inside this bounding box, false otherwise.
     */
    auto containsBox(const BoundingBox<Point3_t> &bbox) const -> bool;

};


using BoundingBoxi = BoundingBox<Point3<int>>;
using BoundingBoxf = BoundingBox<Point3<float>>;
using BoundingBoxd = BoundingBox<Point3<double>>;


template<typename Point3_t> 
BoundingBox<Point3_t>::BoundingBox()
  : Entity(Type::bounding_box),
    pt1(std::numeric_limits<scalar>().max(),
        std::numeric_limits<scalar>().max(),
        std::numeric_limits<scalar>().max()),
    pt2(-std::numeric_limits<scalar>().max(),
        -std::numeric_limits<scalar>().max(),
        -std::numeric_limits<scalar>().max())
{
}

template<typename Point3_t> 
BoundingBox<Point3_t>::BoundingBox(const BoundingBox &bbox) 
  : Entity(Type::bounding_box), 
    pt1(bbox.pt1), 
    pt2(bbox.pt2) 
{
}

template<typename Point3_t> 
BoundingBox<Point3_t>::BoundingBox(BoundingBox &&bbox) TL_NOEXCEPT
  : Entity(std::forward<Entity>(bbox)), 
    pt1(std::move(bbox.pt1)), 
    pt2(std::move(bbox.pt2)) 
{
}

template<typename Point3_t>
BoundingBox<Point3_t>::BoundingBox(const Point3_t &pt1, 
                                   const Point3_t &pt2) 
  : Entity(Type::bounding_box),
    pt1(std::move(pt1)),
    pt2(std::move(pt2))
{
}

template<typename Point3_t> template<typename T> 
BoundingBox<Point3_t>::BoundingBox(const Point3_t &pt,
                                   T width, T depth, T height)
  : Entity(Type::bounding_box)
{
    auto half_width = width / consts::two<scalar>;
    auto half_depth = depth / consts::two<scalar>;
    auto half_height = height / consts::two<scalar>;

#if (CPP_VERSION >= 17)
    if constexpr (std::is_integral<scalar>::value) {
#else
    if (std::is_integral<scalar>::value) {
#endif

        int dx = static_cast<int>(width) % 2;
        int dy = static_cast<int>(depth) % 2;
        int dz = static_cast<int>(height) % 2;

        pt1 = Point3_t(pt.x - half_width,
                       pt.y - half_depth,
                       pt.z - half_height);
        pt2 = Point3_t(pt.x + half_width + dx,
                       pt.y + half_depth + dy,
                       pt.z + half_height + dz);
    } else {
        pt1 = Point3_t(pt.x - half_width,
                       pt.y - half_depth,
                       pt.z - half_height);
        pt2 = Point3_t(pt.x + half_width,
                       pt.y + half_depth,
                       pt.z + half_height);
    }
}

template<typename Point3_t> template<typename T> 
BoundingBox<Point3_t>::BoundingBox(const Point3_t &pt,
                                   T side)
  : Entity(Type::bounding_box)
{
    auto half_side = side / consts::two<scalar>;

#if (CPP_VERSION >= 17)
    if constexpr (std::is_integral<scalar>::value) {
#else
    if (std::is_integral<scalar>::value) {
#endif

        int dxyz = static_cast<int>(side) % 2;

        pt1 = Point3_t(pt.x - half_side,
                       pt.y - half_side,
                       pt.z - half_side);
        pt2 = Point3_t(pt.x + half_side + dxyz,
                       pt.y + half_side + dxyz,
                       pt.z + half_side + dxyz);
    } else {
        pt1 = Point3_t(pt.x - half_side,
                       pt.y - half_side,
                       pt.z - half_side);
        pt2 = Point3_t(pt.x + half_side,
                       pt.y + half_side,
                       pt.z + half_side);
    }
}

template<typename Point3_t>
BoundingBox<Point3_t>::BoundingBox(const std::vector<Point3_t> &vertices)
  : Entity(Type::bounding_box),
    pt1(std::numeric_limits<scalar>().max(),
        std::numeric_limits<scalar>().max(),
        std::numeric_limits<scalar>().max()),
    pt2(-std::numeric_limits<scalar>().max(),
        -std::numeric_limits<scalar>().max(),
        -std::numeric_limits<scalar>().max())
{
    if (vertices.size() >= 2) {
        for (const auto &vertex : vertices) {
            if (pt1.x > vertex.x) pt1.x = vertex.x;
            if (pt1.y > vertex.y) pt1.y = vertex.y;
            if (pt1.z > vertex.z) pt1.z = vertex.z;
            if (pt2.x < vertex.x) pt2.x = vertex.x;
            if (pt2.y < vertex.y) pt2.y = vertex.y;
            if (pt2.z < vertex.z) pt2.z = vertex.z;
        }
    }
}

template<typename Point3_t> template<typename Point3_t2>
BoundingBox<Point3_t>::BoundingBox(const std::vector<Point3_t2> &vertices)
  : Entity(Type::bounding_box),
    pt1(std::numeric_limits<scalar>().max(),
        std::numeric_limits<scalar>().max(),
        std::numeric_limits<scalar>().max()),
    pt2(-std::numeric_limits<scalar>().max(),
        -std::numeric_limits<scalar>().max(),
        -std::numeric_limits<scalar>().max())
{
    if (vertices.size() >= 2) {
        for (const auto &vertex : vertices) {
            if (pt1.x > vertex.x) pt1.x = numberCast<scalar>(vertex.x);
            if (pt1.y > vertex.y) pt1.y = numberCast<scalar>(vertex.y);
            if (pt1.z > vertex.z) pt1.z = numberCast<scalar>(vertex.z);
            if (pt2.x < vertex.x) pt2.x = numberCast<scalar>(vertex.x);
            if (pt2.y < vertex.y) pt2.y = numberCast<scalar>(vertex.y);
            if (pt2.z < vertex.z) pt2.z = numberCast<scalar>(vertex.z);
        }
    }
}

template<typename Point3_t>
auto BoundingBox<Point3_t>::operator = (const BoundingBox &bbox) -> BoundingBox<Point3_t>&
{
    if (this != &bbox) {
        Entity::operator = (bbox);
        this->pt1 = bbox.pt1;
        this->pt2 = bbox.pt2;
    }

    return *this;
}

template<typename Point3_t>
auto BoundingBox<Point3_t>::operator = (BoundingBox &&bbox) TL_NOEXCEPT -> BoundingBox<Point3_t>&
{
    if (this != &bbox) {
        Entity::operator = (std::forward<Entity>(bbox));
        this->pt1 = std::move(bbox.pt1);
        this->pt2 = std::move(bbox.pt2);
    }

    return *this;
}

template<typename Point3_t>
auto BoundingBox<Point3_t>::operator == (const BoundingBox &bbox) const -> bool
{
    return (this->pt1 == bbox.pt1 && this->pt2 == bbox.pt2);
}

template<typename Point3_t> template<typename Point3_t2>
BoundingBox<Point3_t>::operator BoundingBox<Point3_t2>() const
{
    return BoundingBox<Point3_t2>(static_cast<Point3_t2>(pt1), 
                                  static_cast<Point3_t2>(pt2));
}

template<typename Point3_t>
auto BoundingBox<Point3_t>::center() const -> Point3_t
{
    Point3_t pt_center;

    if (!this->isEmpty()) {

        pt_center.x = (pt1.x + pt2.x) / consts::two<scalar>;
        pt_center.y = (pt1.y + pt2.y) / consts::two<scalar>;
        pt_center.z = (pt1.z + pt2.z) / consts::two<scalar>;
    }

    return pt_center;
}

template<typename Point3_t>
auto tl::BoundingBox<Point3_t>::vertices() const -> std::vector<Point3_t>
{
    return {
        {pt1.x, pt1.y, pt1.z},
        {pt1.x, pt2.y, pt1.z},
        {pt2.x, pt2.y, pt1.z},
        {pt2.x, pt1.y, pt1.z},
        {pt1.x, pt1.y, pt2.z},
        {pt1.x, pt2.y, pt2.z},
        {pt2.x, pt2.y, pt2.z},
        {pt2.x, pt1.y, pt2.z}
    };
}

template<typename Point3_t>
auto BoundingBox<Point3_t>::width() const -> scalar
{
    return this->isEmpty() ? consts::zero<scalar> : pt2.x - pt1.x;
}

template<typename Point3_t>
auto BoundingBox<Point3_t>::height() const -> scalar
{
    return this->isEmpty() ? consts::zero<scalar> : pt2.y - pt1.y;
}

template<typename Point3_t>
auto BoundingBox<Point3_t>::depth() const -> scalar
{
    return this->isEmpty() ? consts::zero<scalar> : pt2.z - pt1.z;
}

template<typename Point3_t>
auto BoundingBox<Point3_t>::isEmpty() const -> bool
{
    return (pt1.x == std::numeric_limits<scalar>().max() &&
            pt1.y == std::numeric_limits<scalar>().max() &&
            pt1.z == std::numeric_limits<scalar>().max() &&
            pt2.x == -std::numeric_limits<scalar>().max() &&
            pt2.y == -std::numeric_limits<scalar>().max() &&
            pt2.z == -std::numeric_limits<scalar>().max());
}

template<typename Point3_t>
auto BoundingBox<Point3_t>::isValid() const -> bool
{
    return this->width() > consts::zero<scalar> &&
           this->height() > consts::zero<scalar> &&
           this->depth() > consts::zero<scalar>;
}

template<typename Point3_t>
void BoundingBox<Point3_t>::normalized()
{
    if (!this->isValid()) {
        if (this->pt1.x > this->pt2.x) std::swap(this->pt1.x, this->pt2.x);
        if (this->pt1.y > this->pt2.y) std::swap(this->pt1.y, this->pt2.y);
        if (this->pt1.z > this->pt2.z) std::swap(this->pt1.z, this->pt2.z);
    }
}

template<typename Point3_t>
auto BoundingBox<Point3_t>::containsPoint(const Point3_t &pt) const -> bool
{
    return ((pt2.x >= pt.x) &&
            (pt2.y >= pt.y) &&
            (pt2.z >= pt.z) &&
            (pt1.x <= pt.x) &&
            (pt1.y <= pt.y) &&
            (pt1.z <= pt.z));
}

template<typename Point3_t>
auto BoundingBox<Point3_t>::containsBox(const BoundingBox<Point3_t> &bbox) const -> bool
{
    return (pt1.x <= bbox.pt1.x &&
            pt1.y <= bbox.pt1.y &&
            pt1.z <= bbox.pt1.z &&
            pt2.x >= bbox.pt2.x &&
            pt2.y >= bbox.pt2.y &&
            pt2.z >= bbox.pt2.z);
}

using BoundingBoxI = BoundingBox<Point3<int> >;
using BoundingBoxD = BoundingBox<Point3<double> >;
using BoundingBoxF = BoundingBox<Point3<float> >;


/*!
 * \brief Computes the union of two bounding boxes.
 * \tparam T Bounding box type.
 * \param[in] b1 First bounding box.
 * \param[in] b2 Second bounding box.
 * \return The smallest bounding box that fully contains both \p b1 and \p b2.
 *
 * The function expands the boundaries of the first and second bounding boxes
 * to form a new bounding box that encloses them completely.
 */
template<typename T>
T joinBoundingBoxes(const T &b1, const T &b2)
{
    T bounding_box;

    bounding_box.pt1.x = std::min(b1.pt1.x, b2.pt1.x);
    bounding_box.pt1.y = std::min(b1.pt1.y, b2.pt1.y);
    bounding_box.pt1.z = std::min(b1.pt1.z, b2.pt1.z);
    bounding_box.pt2.x = std::max(b1.pt2.x, b2.pt2.x);
    bounding_box.pt2.y = std::max(b1.pt2.y, b2.pt2.y);
    bounding_box.pt2.z = std::max(b1.pt2.z, b2.pt2.z);

    return bounding_box;
}


/*!
 * \brief Computes the intersection of two bounding boxes.
 * \tparam T Bounding box type.
 * \param[in] b1 First bounding box.
 * \param[in] b2 Second bounding box.
 * \return A bounding box representing the intersection of \p b1 and \p b2.
 *
 * If the bounding boxes do not overlap, the resulting bounding box may be invalid
 * (i.e., its dimensions may be negative).
 */
template<typename T>
T intersectBoundingBoxes(const T &b1, const T &b2)
{
    T bounding_box;

    bounding_box.pt1.x = std::max(b1.pt1.x, b2.pt1.x);
    bounding_box.pt1.y = std::max(b1.pt1.y, b2.pt1.y);
    bounding_box.pt1.z = std::max(b1.pt1.z, b2.pt1.z);
    bounding_box.pt2.x = std::min(b1.pt2.x, b2.pt2.x);
    bounding_box.pt2.y = std::min(b1.pt2.y, b2.pt2.y);
    bounding_box.pt2.z = std::min(b1.pt2.z, b2.pt2.z);

    return bounding_box;
}







//oriented minimum bounding box

template<typename T>
class OrientedBoundingBox;

template<  
  template<typename, typename>
  class Point_t, typename P, typename T
>
class OrientedBoundingBox<Point_t<P, T>>
{

public:

    OrientedBoundingBox();

    ~OrientedBoundingBox() = default;

private:

};


template<
  template<typename, typename>
  class Point_t, typename P, typename T
>
OrientedBoundingBox<Point_t<P, T>>::OrientedBoundingBox()
{
}

/*! \} */ // end of geometry

} // End namespace tl


